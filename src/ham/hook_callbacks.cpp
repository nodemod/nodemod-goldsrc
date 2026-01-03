#include "hook_callbacks.h"
#include "hook.h"
#include "ham_manager.h"
#include "../structures/structures.hpp"
#include "../node/utils.hpp"
#include "extdll.h"
#include <v8.h>
#include <cstring>
#include <cstdio>
#include <unistd.h>  // for write()

extern enginefuncs_t g_engfuncs;

// Debug: trace all callback entries
static void debugCallbackEntry(const char* name, void* hook, void* pthis) {
    printf("[HAM-TRACE] %s: hook=%p pthis=%p\n", name, hook, pthis);
    fflush(stdout);
}

// Debug wrapper - this gets called by the trampoline to verify it's working
extern "C" void debug_trampoline_test(void* arg1, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6) {
    printf("[HAM-TRAMPOLINE] Test called! args: %p %p %p %p %p %p\n",
           arg1, arg2, arg3, arg4, arg5, arg6);
    fflush(stdout);
}

// Early trampoline entry debug - called before any parameter setup
extern "C" void ham_trampoline_entry_debug() {
    printf("[HAM-TRAMPOLINE] *** TRAMPOLINE ENTRY ***\n");
    fflush(stdout);

#if defined(__i386__) || defined(_M_IX86)
    // On x86, at this point we're inside the trampoline
    // Let's print the raw stack to see what the game passed
    void* esp_val;
    void* ebp_val;
    __asm__ volatile("mov %%esp, %0" : "=r"(esp_val));
    __asm__ volatile("mov %%ebp, %0" : "=r"(ebp_val));
    printf("[HAM-TRAMPOLINE] ESP=%p EBP=%p\n", esp_val, ebp_val);

    // Dump raw stack around EBP
    uint32_t* stack = reinterpret_cast<uint32_t*>(ebp_val);
    printf("[HAM-TRAMPOLINE] Stack dump from EBP:\n");
    for (int i = 0; i < 12; i++) {
        printf("  [EBP+%2d] = 0x%08x\n", i * 4, stack[i]);
    }
    fflush(stdout);
#endif
}

// Debug: Dump the current stack to help diagnose calling convention issues
static void dumpStack(const char* label, int numWords) {
    printf("[HAM-STACK] %s - Dumping %d words from stack:\n", label, numWords);
    fflush(stdout);

#if defined(__i386__) || defined(_M_IX86)
    void* esp;
    void* ebp;
    __asm__ volatile("mov %%esp, %0" : "=r"(esp));
    __asm__ volatile("mov %%ebp, %0" : "=r"(ebp));
    printf("  ESP=%p EBP=%p\n", esp, ebp);

    // Dump stack contents relative to EBP (frame pointer)
    uint32_t* stackPtr = reinterpret_cast<uint32_t*>(ebp);
    for (int i = -2; i < numWords; i++) {
        printf("  [EBP%+d] = 0x%08x\n", i * 4, stackPtr[i]);
    }
    fflush(stdout);
#endif
}

namespace Ham {

// Dynamically discovered pev offset within CBaseEntity (like AMX Mod X)
static size_t g_pevOffset = 0;
static bool g_pevOffsetFound = false;

static void findPevOffset() {
    if (g_pevOffsetFound) return;

    edict_t* worldspawn = (*g_engfuncs.pfnPEntityOfEntIndex)(0);
    if (!worldspawn || !worldspawn->pvPrivateData) {
        g_pevOffsetFound = true;
        return;
    }

    entvars_t* pev = &worldspawn->v;
    uint8_t* privateData = static_cast<uint8_t*>(worldspawn->pvPrivateData);

    // Scan for pointer matching pev (same as AMX Mod X HLTypeConversion::search_pev)
    for (size_t i = 0; i < 0xFFF; ++i) {
        if (*reinterpret_cast<entvars_t**>(privateData + i) == pev) {
            g_pevOffset = i;
            break;
        }
    }
    g_pevOffsetFound = true;
}

// Get edict_t* from CBaseEntity* (pthis) using discovered pev offset
static edict_t* getEdictFromThis(void* pthis) {
    if (!pthis) return nullptr;

    findPevOffset();

    entvars_t* pev = *reinterpret_cast<entvars_t**>(static_cast<uint8_t*>(pthis) + g_pevOffset);
    if (!pev) return nullptr;

    return pev->pContainingEntity;
}

// Base executeCallbacks - only passes this_ (entity)
static void executeCallbacks(Hook* hook, void* pthis, bool isPre) {
    HamManager& mgr = HamManager::instance();
    v8::Isolate* isolate = mgr.getIsolate();
    if (!isolate) return;

    v8::Locker locker(isolate);
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);

    auto& callbacks = isPre ? hook->getPreCallbacks() : hook->getPostCallbacks();

    for (auto& cb : callbacks) {
        if (cb.callback.IsEmpty() || cb.context.IsEmpty()) continue;

        v8::Local<v8::Context> context = cb.context.Get(isolate);
        v8::Context::Scope contextScope(context);

        v8::Local<v8::Function> func = cb.callback.Get(isolate);

        // Convert pthis to edict and wrap as Entity object
        edict_t* edict = getEdictFromThis(pthis);
        v8::Local<v8::Value> argv[1] = {
            edict ? structures::wrapEntity(isolate, edict) : v8::Null(isolate).As<v8::Value>()
        };

        v8::TryCatch tryCatch(isolate);
        v8::MaybeLocal<v8::Value> result = func->Call(context, context->Global(), 1, argv);

        if (tryCatch.HasCaught()) {
            continue;
        }

        if (!result.IsEmpty()) {
            v8::Local<v8::Value> retVal = result.ToLocalChecked();
            if (retVal->IsNumber()) {
                int hamResult = retVal->Int32Value(context).FromMaybe(0);
                if (hamResult > static_cast<int>(mgr.getCurrentResult())) {
                    mgr.setCurrentResult(static_cast<HamResult>(hamResult));
                }
            }
        }
    }
}

// Extended executeCallbacks - passes this_ plus additional arguments
// NOTE: Caller must already hold the isolate lock and have proper scopes set up
template<size_t N>
static void executeCallbacksWithArgs(Hook* hook, void* pthis, bool isPre, v8::Isolate* isolate, v8::Local<v8::Value> (&extraArgs)[N]) {
    HamManager& mgr = HamManager::instance();

    auto& callbacks = isPre ? hook->getPreCallbacks() : hook->getPostCallbacks();

    for (auto& cb : callbacks) {
        if (cb.callback.IsEmpty() || cb.context.IsEmpty()) continue;

        v8::Local<v8::Context> context = cb.context.Get(isolate);
        v8::Context::Scope contextScope(context);

        v8::Local<v8::Function> func = cb.callback.Get(isolate);

        // Build argv: this_ + extra args
        edict_t* edict = getEdictFromThis(pthis);
        v8::Local<v8::Value> argv[1 + N];
        argv[0] = edict ? structures::wrapEntity(isolate, edict) : v8::Null(isolate).As<v8::Value>();
        for (size_t i = 0; i < N; i++) {
            argv[1 + i] = extraArgs[i];
        }

        v8::TryCatch tryCatch(isolate);
        v8::MaybeLocal<v8::Value> result = func->Call(context, context->Global(), 1 + N, argv);

        if (tryCatch.HasCaught()) {
            continue;
        }

        if (!result.IsEmpty()) {
            v8::Local<v8::Value> retVal = result.ToLocalChecked();
            if (retVal->IsNumber()) {
                int hamResult = retVal->Int32Value(context).FromMaybe(0);
                if (hamResult > static_cast<int>(mgr.getCurrentResult())) {
                    mgr.setCurrentResult(static_cast<HamResult>(hamResult));
                }
            }
        }
    }
}

// Helper macros for common patterns
#define PUSH_VOID() \
    HamManager& mgr = HamManager::instance(); \
    mgr.setCurrentResult(HAM_UNSET); \
    hook->setExecuting(true);

#define PUSH_INT() \
    HamManager& mgr = HamManager::instance(); \
    mgr.setCurrentResult(HAM_UNSET); \
    int ret = 0; \
    int origret = 0; \
    hook->setExecuting(true);

#define PUSH_FLOAT() \
    HamManager& mgr = HamManager::instance(); \
    mgr.setCurrentResult(HAM_UNSET); \
    float ret = 0.0f; \
    float origret = 0.0f; \
    hook->setExecuting(true);

#define PUSH_BOOL() \
    HamManager& mgr = HamManager::instance(); \
    mgr.setCurrentResult(HAM_UNSET); \
    bool ret = false; \
    bool origret = false; \
    hook->setExecuting(true);

#define PUSH_CBASE() \
    HamManager& mgr = HamManager::instance(); \
    mgr.setCurrentResult(HAM_UNSET); \
    void* ret = nullptr; \
    void* origret = nullptr; \
    hook->setExecuting(true);

#define PUSH_STRING() \
    HamManager& mgr = HamManager::instance(); \
    mgr.setCurrentResult(HAM_UNSET); \
    static char retBuffer[256]; \
    const char* ret = ""; \
    const char* origret = ""; \
    hook->setExecuting(true);

#define PUSH_VECTOR() \
    HamManager& mgr = HamManager::instance(); \
    mgr.setCurrentResult(HAM_UNSET); \
    float retVec[3] = {0, 0, 0}; \
    float origVec[3] = {0, 0, 0}; \
    hook->setExecuting(true);

#define POP() \
    hook->setExecuting(false);

#define CHECK_RETURN_INT() \
    if (mgr.getCurrentResult() < HAM_OVERRIDE) { \
        return origret; \
    } \
    { \
        v8::Isolate* isolate = mgr.getIsolate(); \
        if (isolate) { \
            v8::Locker locker(isolate); \
            v8::Isolate::Scope isolateScope(isolate); \
            v8::HandleScope handleScope(isolate); \
            v8::Local<v8::Value> retVal = mgr.getReturnValue(isolate); \
            v8::Local<v8::Context> ctx = isolate->GetCurrentContext(); \
            if (!retVal.IsEmpty() && retVal->IsNumber()) { \
                ret = retVal->Int32Value(ctx).FromMaybe(origret); \
            } else { \
                ret = origret; \
            } \
        } else { \
            ret = origret; \
        } \
    } \
    return ret;

#define CHECK_RETURN_FLOAT() \
    if (mgr.getCurrentResult() < HAM_OVERRIDE) { \
        return origret; \
    } \
    { \
        v8::Isolate* isolate = mgr.getIsolate(); \
        if (isolate) { \
            v8::Locker locker(isolate); \
            v8::Isolate::Scope isolateScope(isolate); \
            v8::HandleScope handleScope(isolate); \
            v8::Local<v8::Value> retVal = mgr.getReturnValue(isolate); \
            v8::Local<v8::Context> ctx = isolate->GetCurrentContext(); \
            if (!retVal.IsEmpty() && retVal->IsNumber()) { \
                ret = static_cast<float>(retVal->NumberValue(ctx).FromMaybe(origret)); \
            } else { \
                ret = origret; \
            } \
        } else { \
            ret = origret; \
        } \
    } \
    return ret;

#define CHECK_RETURN_BOOL() \
    if (mgr.getCurrentResult() < HAM_OVERRIDE) { \
        return origret; \
    } \
    { \
        v8::Isolate* isolate = mgr.getIsolate(); \
        if (isolate) { \
            v8::Locker locker(isolate); \
            v8::Isolate::Scope isolateScope(isolate); \
            v8::HandleScope handleScope(isolate); \
            v8::Local<v8::Value> retVal = mgr.getReturnValue(isolate); \
            if (!retVal.IsEmpty() && retVal->IsBoolean()) { \
                ret = retVal->BooleanValue(isolate); \
            } else { \
                ret = origret; \
            } \
        } else { \
            ret = origret; \
        } \
    } \
    return ret;

// ============================================================================
// getCallbackForFunction - Maps Ham functions to their callback handlers
// ============================================================================

void* getCallbackForFunction(HamType function) {
    switch (function) {
        // Void_Void functions
        case Ham_Spawn:
        case Ham_Precache:
        case Ham_Activate:
        case Ham_SetObjectCollisionBox:
        case Ham_OverrideReset:
        case Ham_StartSneaking:
        case Ham_StopSneaking:
        case Ham_UpdateOwner:
        case Ham_Think:
        case Ham_Player_Jump:
        case Ham_Player_Duck:
        case Ham_Player_PreThink:
        case Ham_Player_PostThink:
        case Ham_Player_ImpulseCommands:
        case Ham_Player_UpdateClientData:
        case Ham_Item_UpdateItemInfo:
        case Ham_Item_PreFrame:
        case Ham_Item_PostFrame:
        case Ham_Item_Drop:
        case Ham_Item_Kill:
        case Ham_Weapon_ResetEmptySound:
        case Ham_Weapon_PrimaryAttack:
        case Ham_Weapon_SecondaryAttack:
        case Ham_Weapon_Reload:
        case Ham_Weapon_WeaponIdle:
        case Ham_Weapon_RetireWeapon:
        case Ham_CS_Restart:
        case Ham_CS_RoundRespawn:
        case Ham_FadeMonster:
        case Ham_GibMonster:
        case Ham_BecomeDead:
        case Ham_PainSound:
        case Ham_ReportAIState:
        case Ham_MonsterInitDead:
        case Ham_RunAI:
        case Ham_MonsterThink:
        case Ham_MonsterInit:
        case Ham_Stop:
        case Ham_ScheduleChange:
        case Ham_SentenceStop:
        case Ham_SetYawSpeed:
        case Ham_CheckAmmo:
        case Ham_BarnacleVictimReleased:
        case Ham_PrescheduleThink:
        case Ham_DeathSound:
        case Ham_AlertSound:
        case Ham_IdleSound:
        case Ham_CS_Player_ResetMaxSpeed:
        case Ham_TS_GiveSlowMul:
        case Ham_TS_OnFreeEntPrivateData:
        case Ham_TS_Weapon_AlternateAttack:
            return reinterpret_cast<void*>(Hook_Void_Void);

        // Int_Void functions
        case Ham_ObjectCaps:
        case Ham_Classify:
        case Ham_BloodColor:
        case Ham_GetToggleState:
        case Ham_IsMoving:
        case Ham_IsSneaking:
        case Ham_IsAlive:
        case Ham_IsBSPModel:
        case Ham_ReflectGauss:
        case Ham_IsInWorld:
        case Ham_IsPlayer:
        case Ham_IsNetClient:
        case Ham_Illumination:
        case Ham_FBecomeProne:
        case Ham_Player_ShouldFadeOnDeath:
        case Ham_Item_CanDeploy:
        case Ham_Item_Deploy:
        case Ham_Item_CanHolster:
        case Ham_Item_PrimaryAmmoIndex:
        case Ham_Item_SecondaryAmmoIndex:
        case Ham_Item_ItemSlot:
        case Ham_Weapon_AddWeapon:
        case Ham_Weapon_PlayEmptySound:
        case Ham_Weapon_IsUsable:
        case Ham_Weapon_ShouldWeaponIdle:
        case Ham_Weapon_UseDecrement:
        case Ham_CS_Item_CanDrop:
        case Ham_TS_CanUsedThroughWalls:
        case Ham_TS_InSlow:
        case Ham_TS_IsObjective:
        case Ham_HasHumanGibs:
        case Ham_HasAlienGibs:
        case Ham_GetDeathActivity:
        case Ham_GetStoppedActivity:
        case Ham_GetIdealState:
        case Ham_FCanCheckAttacks:
        case Ham_IgnoreConditions:
        case Ham_FCanActiveIdle:
        case Ham_ISoundMask:
        case Ham_CS_Player_IsBot:
            return reinterpret_cast<void*>(Hook_Int_Void);

        // Float_Void functions
        case Ham_GetDelay:
        case Ham_CoverRadius:
        case Ham_HearingSensitivity:
        case Ham_CS_Item_GetMaxSpeed:
            return reinterpret_cast<void*>(Hook_Float_Void);

        // Void_Int functions
        case Ham_Keyvalue:
        case Ham_SetToggleState:
        case Ham_Item_Holster:
        case Ham_Look:
        case Ham_SetActivity:
        case Ham_StopFollowing:
        case Ham_TS_EnableObjective:
            return reinterpret_cast<void*>(Hook_Void_Int);

        // Void_Entvar functions
        case Ham_DeathNotice:
        case Ham_BarnacleVictimBitten:
            return reinterpret_cast<void*>(Hook_Void_Entvar);

        // Void_Cbase functions
        case Ham_Touch:
        case Ham_Blocked:
        case Ham_Item_AttachToPlayer:
            return reinterpret_cast<void*>(Hook_Void_Cbase);

        // Void_Entvar_Int functions
        case Ham_Killed:
            return reinterpret_cast<void*>(Hook_Void_Entvar_Int);

        // Int_Cbase functions
        case Ham_IsTriggered:
        case Ham_AddPlayerItem:
        case Ham_RemovePlayerItem:
        case Ham_FVisible:
        case Ham_Item_AddToPlayer:
        case Ham_Item_AddDuplicate:
        case Ham_Item_UpdateClientData:
        case Ham_Weapon_ExtractAmmo:
        case Ham_Weapon_ExtractClipAmmo:
        case Ham_IRelationship:
        case Ham_FInViewCone:
        case Ham_CheckEnemy:
        case Ham_TS_ShouldCollide:
            return reinterpret_cast<void*>(Hook_Int_Cbase);

        // Int_Entvar functions
        case Ham_OnControls:
            return reinterpret_cast<void*>(Hook_Int_Entvar);

        // Int_Int functions
        case Ham_DamageDecal:
        case Ham_HasTarget:
        case Ham_TS_BreakableRespawn:
        case Ham_CanPlaySentence2:
            return reinterpret_cast<void*>(Hook_Int_Int);

        // Int_Float_Int functions
        case Ham_TakeHealth:
            return reinterpret_cast<void*>(Hook_Int_Float_Int);

        // Int_Entvar_Entvar_Float_Int functions (TakeDamage)
        case Ham_TakeDamage:
            return reinterpret_cast<void*>(Hook_Int_Entvar_Entvar_Float_Int);

        // Void_Entvar_Float_Vector_Trace_Int functions (TraceAttack)
        case Ham_TraceAttack:
            return reinterpret_cast<void*>(Hook_Void_Entvar_Float_Vector_Trace_Int);

        // Void_Float_Vector_Trace_Int functions (TraceBleed)
        case Ham_TraceBleed:
            return reinterpret_cast<void*>(Hook_Void_Float_Vector_Trace_Int);

        // Void_Cbase_Cbase_Int_Float functions (Use)
        case Ham_Use:
            return reinterpret_cast<void*>(Hook_Void_Cbase_Cbase_Int_Float);

        // Void_Int_Int functions
        case Ham_AddPoints:
        case Ham_AddPointsToTeam:
        case Ham_CS_Weapon_SendWeaponAnim:
            return reinterpret_cast<void*>(Hook_Void_Int_Int);

        // Int_Int_Str_Int functions (GiveAmmo)
        case Ham_GiveAmmo:
            return reinterpret_cast<void*>(Hook_Int_Int_Str_Int);

        // Cbase_Void functions
        case Ham_MyMonsterPointer:
        case Ham_MySquadMonsterPointer:
        case Ham_GetNextTarget:
        case Ham_Respawn:
        case Ham_Item_GetWeaponPtr:
        case Ham_BestVisibleEnemy:
            return reinterpret_cast<void*>(Hook_Cbase_Void);

        // Vector_Void functions
        case Ham_Center:
        case Ham_EyePosition:
        case Ham_EarPosition:
        case Ham_Player_GetGunPosition:
            return reinterpret_cast<void*>(Hook_Vector_Void);

        // Vector_pVector functions
        case Ham_BodyTarget:
            return reinterpret_cast<void*>(Hook_Vector_pVector);

        // Int_pVector functions
        case Ham_FVecVisible:
        case Ham_FVecInViewCone:
            return reinterpret_cast<void*>(Hook_Int_pVector);

        // Str_Void functions
        case Ham_TeamId:
            return reinterpret_cast<void*>(Hook_Str_Void);

        // Void_Int_Int_Int functions
        case Ham_Weapon_SendWeaponAnim:
            return reinterpret_cast<void*>(Hook_Void_Int_Int_Int);

        // Float_Int functions
        case Ham_ChangeYaw:
            return reinterpret_cast<void*>(Hook_Float_Int);

        // Int_Float_Float functions
        case Ham_CheckRangeAttack1:
        case Ham_CheckRangeAttack2:
        case Ham_CheckMeleeAttack1:
        case Ham_CheckMeleeAttack2:
            return reinterpret_cast<void*>(Hook_Int_Float_Float);

        // Int_Int_Int functions
        case Ham_CanPlaySequence:
            return reinterpret_cast<void*>(Hook_Int_Int_Int);

        // Void_Float functions
        case Ham_Move:
            return reinterpret_cast<void*>(Hook_Void_Float);

        // Void_Float_Int functions
        case Ham_TS_GoSlow:
            return reinterpret_cast<void*>(Hook_Void_Float_Int);

        // Int_Float functions
        case Ham_ShouldAdvanceRoute:
            return reinterpret_cast<void*>(Hook_Int_Float);

        // Vector_Float functions
        case Ham_CS_Player_GetAutoaimVector:
            return reinterpret_cast<void*>(Hook_Vector_Float);

        // Void_Float_Float_Float_Int functions
        case Ham_CS_Player_Blind:
            return reinterpret_cast<void*>(Hook_Void_Float_Float_Float_Int);

        // Deprecated functions
        case Ham_TS_RespawnWait:
            return reinterpret_cast<void*>(Hook_Deprecated);

        // Removed functions
        case Ham_SC_IsPhysX:
        case Ham_SC_IsValidEntity:
        case Ham_SC_Player_CanTouchPlayer:
        case Ham_SC_Player_DisableCollisionWithPlayer:
        case Ham_SC_Player_EnableCollisionWithPlayer:
        case Ham_SC_Player_IsMenuInputDone:
        case Ham_SC_Player_MenuInputPerformed:
        case Ham_SC_ReviveThink:
            return reinterpret_cast<void*>(Hook_Removed);

        // ============================================================================
        // Additional mod-specific functions (DOD, TFC, ESF, NS, SC, CS, OPF)
        // ============================================================================

        // Bool_Cbase functions
        case Ham_SC_RemovePlayerItem:
            return reinterpret_cast<void*>(Hook_Bool_Cbase);

        // Bool_Cbase_Bool functions
        case Ham_SC_FVisible:
            return reinterpret_cast<void*>(Hook_Bool_Cbase_Bool);

        // Bool_Cbase_Int functions
        case Ham_SC_Item_CanCollect:
            return reinterpret_cast<void*>(Hook_Bool_Cbase_Int);

        // Bool_Entvar functions
        case Ham_SC_OnControls:
            return reinterpret_cast<void*>(Hook_Bool_Entvar);

        // Bool_Entvar_Float functions
        case Ham_SC_IsFacings:
            return reinterpret_cast<void*>(Hook_Bool_Entvar_Float);

        // Bool_Float_Int_Int functions
        case Ham_SC_TakeArmor:
        case Ham_SC_TakeHealth:
            return reinterpret_cast<void*>(Hook_Bool_Float_Int_Int);

        // Bool_Int functions
        case Ham_SC_HasTarget:
        case Ham_SC_Player_CanStartNextVote:
            return reinterpret_cast<void*>(Hook_Bool_Int);

        // Bool_Cbase functions
        case Ham_SC_FBecomeProne:
            return reinterpret_cast<void*>(Hook_Bool_Cbase);

        // Bool_Void functions
        case Ham_DOD_Weapon_IsWaterSniping:
        case Ham_SC_CheckScared:
        case Ham_SC_CriticalRemove:
        case Ham_SC_Player_HasVoted:
        case Ham_SC_IsAlive:
        case Ham_SC_IsBSPModel:
        case Ham_SC_IsBreakable:
        case Ham_SC_IsInWorld:
        case Ham_SC_IsLockedByMaster:
        case Ham_SC_IsMachine:
        case Ham_SC_IsMonster:
        case Ham_SC_IsMoving:
        case Ham_SC_IsNetClient:
        case Ham_SC_IsPlayer:
        case Ham_SC_IsPointEntity:
        case Ham_SC_IsRevivable:
        case Ham_SC_IsSneaking:
        case Ham_SC_IsTriggered:
        case Ham_SC_Player_LogPlayerStats:
        case Ham_SC_Player_IsConnected:
        case Ham_SC_Player_IsObserver:
        case Ham_SC_Player_IsValidInfoEntity:
        case Ham_SC_ReflectGauss:
        case Ham_SC_Weapon_FCanRun:
        case Ham_SC_Weapon_FRunfuncs:
        case Ham_SC_Weapon_IsMultiplayer:
        case Ham_SC_Weapon_IsUsable:
        case Ham_SC_Weapon_ShouldReload:
        case Ham_SC_Weapon_ShouldWeaponIdle:
        case Ham_SC_Weapon_UseDecrement:
            return reinterpret_cast<void*>(Hook_Bool_Void);

        // Bool_pVector functions
        case Ham_SC_FVecVisible:
            return reinterpret_cast<void*>(Hook_Bool_pVector);

        // Bool_pVector_pVector functions
        case Ham_SC_FVisibleFromPos:
            return reinterpret_cast<void*>(Hook_Bool_pVector_pVector);

        // Cbase_Void functions
        case Ham_OPF_MySquadTalkMonsterPointer:
        case Ham_SC_MyCustomPointer:
        case Ham_SC_MyItemPointer:
            return reinterpret_cast<void*>(Hook_Cbase_Void);

        // Float_Float functions
        case Ham_SC_GetPointsForDamage:
            return reinterpret_cast<void*>(Hook_Float_Float);

        // Float_Float_Cbase functions
        case Ham_DOD_Weapon_flAim:
            return reinterpret_cast<void*>(Hook_Float_Float_Cbase);

        // Float_Int_Float functions
        case Ham_NS_SetBoneController:
            return reinterpret_cast<void*>(Hook_Float_Int_Float);

        // Float_Void functions
        case Ham_ESF_GetMaxSpeed:
        case Ham_NS_GetAdrenalineFactor:
        case Ham_NS_GetMaxWalkSpeed:
        case Ham_NS_Weapon_GetDeployTime:
        case Ham_NS_Weapon_GetWeapPrimeTime:
        case Ham_OPF_WeaponTimeBase:
        case Ham_SC_Item_GetRespawnTime:
        case Ham_SC_Player_GetLogFrequency:
        case Ham_SC_Player_TimeToStartNextVote:
            return reinterpret_cast<void*>(Hook_Float_Void);

        // Float_Float functions
        case Ham_TFC_Weapon_GetNextAttackDelay:
            return reinterpret_cast<void*>(Hook_Float_Float);

        // Int_Cbase functions
        case Ham_DOD_GetStateEnt:
        case Ham_SC_CheckAttacker:
        case Ham_TFC_EngineerUse:
            return reinterpret_cast<void*>(Hook_Int_Cbase);

        // Int_Cbase_pVector functions
        case Ham_SC_FInBulletCone:
        case Ham_SC_FVisibleGunPos:
            return reinterpret_cast<void*>(Hook_Int_Cbase_pVector);

        // Bool_Cbase functions (SC Item/Weapon)
        case Ham_SC_Item_AddDuplicate:
        case Ham_SC_Item_AddToPlayer:
        case Ham_SC_Item_UpdateClientData:
        case Ham_SC_Weapon_ExtractAmmoFromItem:
            return reinterpret_cast<void*>(Hook_Bool_Cbase);

        // Bool_Void functions (SC Weapon_AddWeapon)
        case Ham_SC_Weapon_AddWeapon:
            return reinterpret_cast<void*>(Hook_Bool_Void);

        // Int_Entvar_Entvar_Float_Float_Int functions
        case Ham_ESF_TakeDamage2:
            return reinterpret_cast<void*>(Hook_Int_Entvar_Entvar_Float_Float_Int);

        // Int_Float_Float functions
        case Ham_SC_CheckMeleeAttack1_Move:
        case Ham_SC_CheckMeleeAttack2_Move:
        case Ham_SC_CheckRangeAttack1_Move:
        case Ham_SC_CheckRangeAttack2_Move:
            return reinterpret_cast<void*>(Hook_Int_Float_Float);

        // Int_Int functions
        case Ham_DOD_Weapon_ChangeFOV:
        case Ham_SC_GetClassification:
            return reinterpret_cast<void*>(Hook_Int_Int);

        // Int_Int_Int_Float_Int functions
        case Ham_NS_Weapon_DefaultReload:
            return reinterpret_cast<void*>(Hook_Int_Int_Int_Float_Int);

        // Int_Int_Str_Int_Bool functions
        case Ham_SC_GiveAmmo:
            return reinterpret_cast<void*>(Hook_Int_Int_Str_Int_Bool);

        // Int_ItemInfo functions
        case Ham_Item_GetItemInfo:
            return reinterpret_cast<void*>(Hook_Int_ItemInfo);

        // Bool_ItemInfo functions
        case Ham_SC_Item_GetItemInfo:
            return reinterpret_cast<void*>(Hook_Bool_ItemInfo);

        // Int_Short functions
        case Ham_FValidateHintType:
            return reinterpret_cast<void*>(Hook_Int_Short);

        // Int_Str_Str functions
        case Ham_ESF_GetWallJumpAnim2:
            return reinterpret_cast<void*>(Hook_Int_Str_Str);

        // Int_Str_Str_Int_Str_Int_Int functions
        case Ham_NS_Weapon_DefaultDeploy:
            return reinterpret_cast<void*>(Hook_Int_Str_Str_Int_Str_Int_Int);

        // Int_Str_Vector_Str functions
        case Ham_ESF_GetWallJumpAnim:
            return reinterpret_cast<void*>(Hook_Int_Str_Vector_Str);

        // Int_Vector functions
        case Ham_SC_NoFriendlyFire2:
        case Ham_SC_NoFriendlyFireToPos:
            return reinterpret_cast<void*>(Hook_Int_Vector);

        // Int_Vector_Cbase functions
        case Ham_SC_NoFriendlyFire3:
            return reinterpret_cast<void*>(Hook_Int_Vector_Cbase);

        // Int_Vector_Vector_Float_Float functions
        case Ham_BuildNearestRoute:
        case Ham_FindCover:
        case Ham_SC_FindAttackPoint:
        case Ham_SC_FindCoverDistance:
        case Ham_SC_FindCoverGrenade:
            return reinterpret_cast<void*>(Hook_Int_Vector_Vector_Float_Float);

        // Int_Void functions (additional)
        case Ham_CS_Item_IsWeapon:
        case Ham_DOD_GetState:
        case Ham_DOD_Item_CanDrop:
        case Ham_DOD_Item_SpawnDeploy:
        case Ham_DOD_Weapon_GetFOV:
        case Ham_DOD_Weapon_IsUseable:
        case Ham_DOD_Weapon_ZoomIn:
        case Ham_DOD_Weapon_ZoomOut:
        case Ham_ESF_CanBlock:
        case Ham_ESF_CanJump:
        case Ham_ESF_CanPrimaryFire:
        case Ham_ESF_CanRaiseKi:
        case Ham_ESF_CanRaiseStamina:
        case Ham_ESF_CanSecondaryFire:
        case Ham_ESF_CanStartFly:
        case Ham_ESF_CanStartPowerup:
        case Ham_ESF_CanStopFly:
        case Ham_ESF_CanTeleport:
        case Ham_ESF_CanTurbo:
        case Ham_ESF_CanWallJump:
        case Ham_ESF_CheckWallJump:
        case Ham_ESF_DoesPrimaryAttack:
        case Ham_ESF_DoesSecondaryAttack:
        case Ham_ESF_GetMoveForward:
        case Ham_ESF_GetMoveRight:
        case Ham_ESF_IsBuddy:
        case Ham_ESF_IsEnvModel:
        case Ham_ESF_IsFighter:
        case Ham_ESF_IsFlyMoveType:
        case Ham_ESF_IsMoveBack:
        case Ham_ESF_IsSuperJump:
        case Ham_ESF_IsWalkMoveType:
        case Ham_NS_GetAuthenticationMask:
        case Ham_NS_GetCanUseWeapon:
        case Ham_NS_GetEffectivePlayerClass:
        case Ham_NS_GetHull:
        case Ham_NS_GetPointValue:
        case Ham_NS_Weapon_GetIsWeaponPrimed:
        case Ham_NS_Weapon_GetIsWeaponPriming:
        case Ham_SC_CheckTankUsage:
        case Ham_SC_IsPlayerFollowing:
        case Ham_SC_NoFriendlyFire1:
        case Ham_SC_Player_LastVoteInput:
        case Ham_SC_SetGaitActivity:
        case Ham_SC_ShouldFadeOnDeath:
        case Ham_SC_Weapon_GetAmmo1Drop:
        case Ham_SC_Weapon_GetAmmo2Drop:
        case Ham_TFC_IsTriggered:
            return reinterpret_cast<void*>(Hook_Int_Void);

        // Bool_Void functions (SC Item/Weapon - override from Int_Void)
        case Ham_SC_Item_CanDeploy:
        case Ham_SC_Item_CanHaveDuplicates:
        case Ham_SC_Item_CanHolster:
        case Ham_SC_Item_Deploy:
        case Ham_SC_Weapon_PlayEmptySound:
            return reinterpret_cast<void*>(Hook_Bool_Void);

        // Int_pVector functions
        case Ham_SC_FValidateCover:
            return reinterpret_cast<void*>(Hook_Int_pVector);

        // Int_pVector_pVector_Cbase_pFloat functions
        case Ham_CheckLocalMove:
            return reinterpret_cast<void*>(Hook_Int_pVector_pVector_Cbase_pFloat);

        // Int_pVector_pVector_Float_Cbase_pVector functions
        case Ham_FTriangulate:
        case Ham_SC_FTriangulateExtension:
            return reinterpret_cast<void*>(Hook_Int_pVector_pVector_Float_Cbase_pVector);

        // Int_pVector_pVector_Float_Cbase_pVector_pVector_Bool functions
        case Ham_SC_FTriangulate:
            return reinterpret_cast<void*>(Hook_Int_pVector_pVector_Float_Cbase_pVector_pVector_Bool);

        // Str_Str functions
        case Ham_NS_SetTeamID:
        case Ham_SC_Weapon_GetP_Model:
        case Ham_SC_Weapon_GetV_Model:
        case Ham_SC_Weapon_GetW_Model:
            return reinterpret_cast<void*>(Hook_Str_Str);

        // Str_Void functions (additional)
        case Ham_SC_Item_GetPickupSound:
        case Ham_TFC_DB_GetItemName:
            return reinterpret_cast<void*>(Hook_Str_Void);

        // Vector_Float_Cbase_Int functions
        case Ham_DOD_Weapon_Aim:
            return reinterpret_cast<void*>(Hook_Vector_Float_Cbase_Int);

        // Vector_Vector_Vector_Vector functions
        case Ham_SC_Weapon_BulletAccuracy:
            return reinterpret_cast<void*>(Hook_Vector_Vector_Vector_Vector);

        // Vector_Void functions (additional)
        case Ham_ESF_GetTeleportDir:
            return reinterpret_cast<void*>(Hook_Vector_Void);

        // Void_Bool functions
        case Ham_SC_SetPlayerAlly:
            return reinterpret_cast<void*>(Hook_Void_Bool);

        // Void_Bool_Bool functions
        case Ham_SC_StopPlayerFollowing:
            return reinterpret_cast<void*>(Hook_Void_Bool_Bool);

        // Void_Cbase functions (additional)
        case Ham_CS_Player_OnTouchingWeapon:
        case Ham_DOD_AreaSendStatus:
        case Ham_SC_RideMonster:
            return reinterpret_cast<void*>(Hook_Void_Cbase);

        // Void_Cbase_Bool functions
        case Ham_SC_StartPlayerFollowing:
            return reinterpret_cast<void*>(Hook_Void_Cbase_Bool);

        // Void_Cbase_Int functions
        case Ham_ESF_IncreaseStrength:
        case Ham_SC_Item_Collect:
            return reinterpret_cast<void*>(Hook_Void_Cbase_Int);

        // Void_Cbase_Int_Float functions
        case Ham_SC_SUB_UseTargets:
            return reinterpret_cast<void*>(Hook_Void_Cbase_Int_Float);

        // Void_Cbase_pVector_Float functions
        case Ham_MoveExecute:
            return reinterpret_cast<void*>(Hook_Void_Cbase_pVector_Float);

        // Void_Edict functions
        case Ham_NS_InitPlayerFromSpawn:
            return reinterpret_cast<void*>(Hook_Void_Edict);

        // Void_Entvar functions (additional)
        case Ham_NS_AwardKill:
        case Ham_TFC_Concuss:
        case Ham_TFC_TakeEmpBlast:
            return reinterpret_cast<void*>(Hook_Void_Entvar);

        // Void_Entvar_Entvar_Float functions
        case Ham_SC_GetDamagePoints:
            return reinterpret_cast<void*>(Hook_Void_Entvar_Entvar_Float);

        // Void_Entvar_Entvar_Float_Int_Int functions
        case Ham_TFC_RadiusDamage:
            return reinterpret_cast<void*>(Hook_Void_Entvar_Entvar_Float_Int_Int);

        // Void_Entvar_Entvar_Int functions
        case Ham_TFC_Killed:
            return reinterpret_cast<void*>(Hook_Void_Entvar_Entvar_Int);

        // Void_Entvar_Float functions
        case Ham_TFC_TakeConcussionBlast:
            return reinterpret_cast<void*>(Hook_Void_Entvar_Float);

        // Void_Entvar_Float_Float functions
        case Ham_TFC_EmpExplode:
            return reinterpret_cast<void*>(Hook_Void_Entvar_Float_Float);

        // Void_Float functions (additional)
        case Ham_DOD_Item_SetDmgTime:
        case Ham_SC_BeginRevive:
        case Ham_SC_EndRevive:
        case Ham_SC_Weapon_CustomDecrement:
            return reinterpret_cast<void*>(Hook_Void_Float);

        // Void_Float_Cbase functions
        case Ham_DOD_Weapon_RemoveStamina:
            return reinterpret_cast<void*>(Hook_Void_Float_Cbase);

        // Void_Float_Float functions
        case Ham_ESF_DrawPSBar:
            return reinterpret_cast<void*>(Hook_Void_Float_Float);

        // Void_Int functions (additional)
        case Ham_DOD_AreaSetIndex:
        case Ham_ESF_AddBeamBoxCrosshair:
        case Ham_ESF_ClientRemoveWeapon:
        case Ham_ESF_DrawChargeBar:
        case Ham_ESF_EmitNullSound:
        case Ham_ESF_IncreasePL:
        case Ham_ESF_SetAnimation:
        case Ham_ESF_SetMaxPowerLevel:
        case Ham_ESF_SetPowerLevel:
        case Ham_ESF_StopAniTrigger:
        case Ham_SC_Player_Vote:
        case Ham_SC_Player_VoteStarted:
        case Ham_SC_SetClassification:
        case Ham_SC_Weapon_SetFOV:
            return reinterpret_cast<void*>(Hook_Void_Int);

        // Void_Int_Bool functions
        case Ham_SC_AddPoints:
        case Ham_SC_AddPointsToTeam:
            return reinterpret_cast<void*>(Hook_Void_Int_Bool);

        // Void_Int_Int functions (additional)
        case Ham_DOD_Weapon_SendWeaponAnim:
        case Ham_TFC_Weapon_SendWeaponAnim:
            return reinterpret_cast<void*>(Hook_Void_Int_Int);

        // Void_Int_Str_Bool functions
        case Ham_NS_GetAnimationForActivity:
            return reinterpret_cast<void*>(Hook_Void_Int_Str_Bool);

        // Void_Short functions
        case Ham_SC_Weapon_ChangeWeaponSkin:
            return reinterpret_cast<void*>(Hook_Void_Short);

        // Void_Str functions
        case Ham_ESF_SendClientsCustomModel:
        case Ham_SC_OnKeyValueUpdate:
        case Ham_SC_Weapon_SetP_Model:
        case Ham_SC_Weapon_SetV_Model:
            return reinterpret_cast<void*>(Hook_Void_Str);

        // Void_Str_Bool functions
        case Ham_NS_GetNamedItem:
            return reinterpret_cast<void*>(Hook_Void_Str_Bool);

        // Void_Str_Float_Float_Float functions
        case Ham_PlaySentence:
            return reinterpret_cast<void*>(Hook_Void_Str_Float_Float_Float);

        // Void_Str_Float_Float_Float_Bool_Cbase functions
        case Ham_SC_PlayScriptedSentence:
            return reinterpret_cast<void*>(Hook_Void_Str_Float_Float_Float_Bool_Cbase);

        // Void_Str_Float_Float_Float_Int_Cbase functions
        case Ham_PlayScriptedSentence:
            return reinterpret_cast<void*>(Hook_Void_Str_Float_Float_Float_Int_Cbase);

        // Void_Str_Int functions
        case Ham_ESF_EmitSound:
            return reinterpret_cast<void*>(Hook_Void_Str_Int);

        // Void_Str_Str_Int functions
        case Ham_ESF_EmitClassSound:
            return reinterpret_cast<void*>(Hook_Void_Str_Str_Int);

        // Void_Vector functions
        case Ham_ESF_EnableWallJump:
            return reinterpret_cast<void*>(Hook_Void_Vector);

        // Void_Vector_Entvar_Entvar_Float_Int_Int functions
        case Ham_TFC_RadiusDamage2:
            return reinterpret_cast<void*>(Hook_Void_Vector_Entvar_Entvar_Float_Int_Int);

        // Void_Vector_Vector functions
        case Ham_NS_StartObserver:
            return reinterpret_cast<void*>(Hook_Void_Vector_Vector);

        // Void_Void functions (additional)
        case Ham_DOD_Item_DropGren:
        case Ham_DOD_RoundRespawn:
        case Ham_DOD_RoundRespawnEnt:
        case Ham_DOD_RoundStore:
        case Ham_DOD_SetScriptReset:
        case Ham_DOD_Weapon_Special:
        case Ham_DOD_Weapon_UpdateZoomSpeed:
        case Ham_ESF_AddAttacks:
        case Ham_ESF_AddBlindFX:
        case Ham_ESF_CheckLightning:
        case Ham_ESF_CheckTimeBasedDamage:
        case Ham_ESF_DisablePSBar:
        case Ham_ESF_DisableWallJump:
        case Ham_ESF_DrawPSWinBonus:
        case Ham_ESF_FreezeControls:
        case Ham_ESF_GetMoveUp:
        case Ham_ESF_GetPowerLevel:
        case Ham_ESF_HideWeapon:
        case Ham_ESF_LockCrosshair:
        case Ham_ESF_PlayAnimation:
        case Ham_ESF_RemoveAllOtherWeapons:
        case Ham_ESF_RemoveBeamBoxCrosshair:
        case Ham_ESF_RemoveBlindFX:
        case Ham_ESF_RemoveSpecialModes:
        case Ham_ESF_ResetWallJumpVars:
        case Ham_ESF_RotateCrosshair:
        case Ham_ESF_SetDeathAnimation:
        case Ham_ESF_SetFlyMoveType:
        case Ham_ESF_SetModel:
        case Ham_ESF_SetWalkMoveType:
        case Ham_ESF_SetWallJumpAnimation:
        case Ham_ESF_StartBlock:
        case Ham_ESF_StartFly:
        case Ham_ESF_StopBlock:
        case Ham_ESF_StopFly:
        case Ham_ESF_StopSwoop:
        case Ham_ESF_StopTurbo:
        case Ham_ESF_TakeBean:
        case Ham_ESF_UnFreezeControls:
        case Ham_ESF_UnLockCrosshair:
        case Ham_ESF_UnRotateCrosshair:
        case Ham_ESF_UpdateHealth:
        case Ham_ESF_UpdateKi:
        case Ham_ESF_WaterMove:
        case Ham_ESF_Weapon_HolsterWhenMeleed:
        case Ham_NS_EffectivePlayerClassChanged:
        case Ham_NS_NeedsTeamUpdate:
        case Ham_NS_PackDeadPlayerItems:
        case Ham_NS_ResetEntity:
        case Ham_NS_SaveDataForReset:
        case Ham_NS_SendTeamUpdate:
        case Ham_NS_SendWeaponUpdate:
        case Ham_NS_StopObserver:
        case Ham_NS_Suicide:
        case Ham_NS_UpdateOnRemove:
        case Ham_NS_Weapon_PrimeWeapon:
        case Ham_SC_CallGibMonster:
        case Ham_SC_CheckApplyGenericAttacks:
        case Ham_SC_CheckCreatureDanger:
        case Ham_SC_CheckFallDamage:
        case Ham_SC_CheckRevival:
        case Ham_SC_CheckTimeBasedDamage:
        case Ham_SC_Item_DetachFromPlayer:
        case Ham_SC_Item_InactiveItemPostFrame:
        case Ham_SC_Item_InactiveItemPreFrame:
        case Ham_SC_Item_Materialize:
        case Ham_SC_MedicCallSound:
        case Ham_SC_OnCreate:
        case Ham_SC_OnDestroy:
        case Ham_SC_OnSetOriginByMap:
        case Ham_SC_Player_EnteredObserver:
        case Ham_SC_Player_InitVote:
        case Ham_SC_Player_LeftObserver:
        case Ham_SC_Player_LevelEnd:
        case Ham_SC_Player_ResetView:
        case Ham_SC_Player_ResetVote:
        case Ham_SC_Player_SpecialSpawn:
        case Ham_SC_PostSpawn:
        case Ham_SC_PreSpawn:
        case Ham_SC_Revive:
        case Ham_SC_SetupFriendly:
        case Ham_SC_StartMonster:
        case Ham_SC_UnUseSound:
        case Ham_SC_UpdateOnRemove:
        case Ham_SC_UseSound:
        case Ham_SC_Weapon_BurstSupplement:
        case Ham_SC_Weapon_FinishReload:
        case Ham_SC_Weapon_PrecacheCustomModels:
        case Ham_SC_Weapon_TertiaryAttack:
        case Ham_TFC_EmpRemove:
        case Ham_TFC_Finished:
            return reinterpret_cast<void*>(Hook_Void_Void);

        // Void_pFloat_pFloat functions
        case Ham_TFC_CalcEmpDmgRad:
            return reinterpret_cast<void*>(Hook_Void_pFloat_pFloat);

        // SC CanPlaySequence/CanPlaySentence2 overrides
        case Ham_SC_CanPlaySequence:
            return reinterpret_cast<void*>(Hook_Bool_Bool_Int);

        case Ham_SC_CanPlaySentence2:
            return reinterpret_cast<void*>(Hook_Bool_Bool);

        // SC Item_AddAmmoFromItem (Bool_Cbase)
        case Ham_SC_Item_AddAmmoFromItem:
            return reinterpret_cast<void*>(Hook_Bool_Cbase);

        default:
            return nullptr;
    }
}

// ============================================================================
// Hook implementations
// ============================================================================

void Hook_Void_Void(Hook* hook, void* pthis) {
    PUSH_VOID()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis);
    }

    executeCallbacks(hook, pthis, false);
    POP()
}

int Hook_Int_Void(Hook* hook, void* pthis) {
    printf("[HAM] Hook_Int_Void ENTRY: hook=%p pthis=%p\n", hook, pthis);
    fflush(stdout);
    printf("[HAM] Hook_Int_Void called: hook=%p pthis=%p entity=%s\n",
           hook, pthis, hook ? hook->getEntityName().c_str() : "null");
    fflush(stdout);
    PUSH_INT()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis);
    }

    executeCallbacks(hook, pthis, false);
    POP()
    CHECK_RETURN_INT()
}

float Hook_Float_Void(Hook* hook, void* pthis) {
    PUSH_FLOAT()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef float (*OrigFunc)(void*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis);
    }

    executeCallbacks(hook, pthis, false);
    POP()
    CHECK_RETURN_FLOAT()
}

void Hook_Void_Int(Hook* hook, void* pthis, int value) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                v8::Integer::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, value);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                v8::Integer::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Float(Hook* hook, void* pthis, float value) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                v8::Number::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, float);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, value);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                v8::Number::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Entvar(Hook* hook, void* pthis, void* entvars) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                entvars ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars)) : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, void*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, entvars);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                entvars ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars)) : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Cbase(Hook* hook, void* pthis, void* entity) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, void*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, entity);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Edict(Hook* hook, void* pthis, void* edict) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                edict ? structures::wrapEntity(isolate, static_cast<edict_t*>(edict)) : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, void*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, edict);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                edict ? structures::wrapEntity(isolate, static_cast<edict_t*>(edict)) : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

int Hook_Int_Int(Hook* hook, void* pthis, int value) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                v8::Integer::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, value);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                v8::Integer::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

int Hook_Int_Float(Hook* hook, void* pthis, float value) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                v8::Number::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, float);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, value);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                v8::Number::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

int Hook_Int_Entvar(Hook* hook, void* pthis, void* entvars) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                entvars ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars)) : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, void*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, entvars);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                entvars ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars)) : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

int Hook_Int_Cbase(Hook* hook, void* pthis, void* entity) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, void*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, entity);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

int Hook_Int_pVector(Hook* hook, void* pthis, float* vec) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                utils::vect2js(isolate, vec)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, float*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, vec);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                utils::vect2js(isolate, vec)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

float Hook_Float_Int(Hook* hook, void* pthis, int i1) {
    PUSH_FLOAT()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef float (*OrigFunc)(void*, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, i1);
    }

    executeCallbacks(hook, pthis, false);
    POP()
    CHECK_RETURN_FLOAT()
}

float Hook_Float_Float(Hook* hook, void* pthis, float f1) {
    PUSH_FLOAT()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef float (*OrigFunc)(void*, float);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, f1);
    }

    executeCallbacks(hook, pthis, false);
    POP()
    CHECK_RETURN_FLOAT()
}

void Hook_Void_Int_Int(Hook* hook, void* pthis, int score, int allowNegative) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                v8::Integer::New(isolate, score),
                v8::Integer::New(isolate, allowNegative)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, int, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, score, allowNegative);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                v8::Integer::New(isolate, score),
                v8::Integer::New(isolate, allowNegative)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Float_Float(Hook* hook, void* pthis, float value1, float value2) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                v8::Number::New(isolate, value1),
                v8::Number::New(isolate, value2)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, float, float);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, value1, value2);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                v8::Number::New(isolate, value1),
                v8::Number::New(isolate, value2)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Float_Int(Hook* hook, void* pthis, float duration, int mode) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                v8::Number::New(isolate, duration),
                v8::Integer::New(isolate, mode)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, float, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, duration, mode);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                v8::Number::New(isolate, duration),
                v8::Integer::New(isolate, mode)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Entvar_Int(Hook* hook, void* pthis, void* attacker, int gibType) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                attacker ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(attacker)) : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, gibType)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, void*, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, attacker, gibType);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                attacker ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(attacker)) : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, gibType)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Entvar_Float(Hook* hook, void* pthis, void* entvars, float value) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                entvars ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars)) : v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, void*, float);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, entvars, value);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                entvars ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars)) : v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Cbase_Int(Hook* hook, void* pthis, void* entity, int value) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, void*, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, entity, value);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Cbase_Float(Hook* hook, void* pthis, void* entity, float value) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, void*, float);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, entity, value);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

int Hook_Int_Int_Int(Hook* hook, void* pthis, int value1, int value2) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                v8::Integer::New(isolate, value1),
                v8::Integer::New(isolate, value2)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, int, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, value1, value2);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                v8::Integer::New(isolate, value1),
                v8::Integer::New(isolate, value2)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

int Hook_Int_Float_Int(Hook* hook, void* pthis, float value, int value2) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                v8::Number::New(isolate, value),
                v8::Integer::New(isolate, value2)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, float, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, value, value2);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                v8::Number::New(isolate, value),
                v8::Integer::New(isolate, value2)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

int Hook_Int_Float_Float(Hook* hook, void* pthis, float value1, float value2) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                v8::Number::New(isolate, value1),
                v8::Number::New(isolate, value2)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, float, float);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, value1, value2);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                v8::Number::New(isolate, value1),
                v8::Number::New(isolate, value2)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

int Hook_Int_pVector_pVector(Hook* hook, void* pthis, float* vec1, float* vec2) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                utils::vect2js(isolate, vec1),
                utils::vect2js(isolate, vec2)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, float*, float*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, vec1, vec2);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                utils::vect2js(isolate, vec1),
                utils::vect2js(isolate, vec2)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

void Hook_Void_Int_Int_Int(Hook* hook, void* pthis, int value1, int value2, int value3) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[3] = {
                v8::Integer::New(isolate, value1),
                v8::Integer::New(isolate, value2),
                v8::Integer::New(isolate, value3)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, int, int, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, value1, value2, value3);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[3] = {
                v8::Integer::New(isolate, value1),
                v8::Integer::New(isolate, value2),
                v8::Integer::New(isolate, value3)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Entvar_Entvar_Float(Hook* hook, void* pthis, void* entvars1, void* entvars2, float value) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[3] = {
                entvars1 ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars1)) : v8::Null(isolate).As<v8::Value>(),
                entvars2 ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars2)) : v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, void*, void*, float);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, entvars1, entvars2, value);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[3] = {
                entvars1 ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars1)) : v8::Null(isolate).As<v8::Value>(),
                entvars2 ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars2)) : v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Entvar_Float_Float(Hook* hook, void* pthis, void* entvars, float value1, float value2) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[3] = {
                entvars ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars)) : v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, value1),
                v8::Number::New(isolate, value2)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, void*, float, float);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, entvars, value1, value2);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[3] = {
                entvars ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars)) : v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, value1),
                v8::Number::New(isolate, value2)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

int Hook_Int_Int_Str_Int(Hook* hook, void* pthis, int amount, const char* name, int max) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[3] = {
                v8::Integer::New(isolate, amount),
                name ? v8::String::NewFromUtf8(isolate, name).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, max)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, int, const char*, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, amount, name, max);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[3] = {
                v8::Integer::New(isolate, amount),
                name ? v8::String::NewFromUtf8(isolate, name).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, max)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

int Hook_Int_Cbase_Int(Hook* hook, void* pthis, void* entity, int value) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, void*, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, entity, value);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

void Hook_Void_Cbase_Cbase_Int_Float(Hook* hook, void* pthis, void* activator, void* caller, int useType, float value) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[4] = {
                getEdictFromThis(activator) ? structures::wrapEntity(isolate, getEdictFromThis(activator)) : v8::Null(isolate).As<v8::Value>(),
                getEdictFromThis(caller) ? structures::wrapEntity(isolate, getEdictFromThis(caller)) : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, useType),
                v8::Number::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, void*, void*, int, float);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, activator, caller, useType, value);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[4] = {
                getEdictFromThis(activator) ? structures::wrapEntity(isolate, getEdictFromThis(activator)) : v8::Null(isolate).As<v8::Value>(),
                getEdictFromThis(caller) ? structures::wrapEntity(isolate, getEdictFromThis(caller)) : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, useType),
                v8::Number::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

int Hook_Int_Entvar_Entvar_Float_Int(Hook* hook, void* pthis, void* inflictor, void* attacker, float damage, int damageBits) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[4] = {
                inflictor ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(inflictor)) : v8::Null(isolate).As<v8::Value>(),
                attacker ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(attacker)) : v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, damage),
                v8::Integer::New(isolate, damageBits)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, void*, void*, float, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, inflictor, attacker, damage, damageBits);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[4] = {
                inflictor ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(inflictor)) : v8::Null(isolate).As<v8::Value>(),
                attacker ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(attacker)) : v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, damage),
                v8::Integer::New(isolate, damageBits)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

void Hook_Void_Float_Float_Float_Int(Hook* hook, void* pthis, float value1, float value2, float value3, int value) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[4] = {
                v8::Number::New(isolate, value1),
                v8::Number::New(isolate, value2),
                v8::Number::New(isolate, value3),
                v8::Integer::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, float, float, float, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, value1, value2, value3, value);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[4] = {
                v8::Number::New(isolate, value1),
                v8::Number::New(isolate, value2),
                v8::Number::New(isolate, value3),
                v8::Integer::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Float_Vector_Trace_Int(Hook* hook, void* pthis, float damage, float* direction, void* trace, int damageBits) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[4] = {
                v8::Number::New(isolate, damage),
                v8::Null(isolate).As<v8::Value>(),
                trace ? structures::wrapTraceResult(isolate, static_cast<TraceResult*>(trace)) : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, damageBits)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, float, float*, void*, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, damage, direction, trace, damageBits);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[4] = {
                v8::Number::New(isolate, damage),
                v8::Null(isolate).As<v8::Value>(),
                trace ? structures::wrapTraceResult(isolate, static_cast<TraceResult*>(trace)) : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, damageBits)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

int Hook_Int_Entvar_Entvar_Float_Float_Int(Hook* hook, void* pthis, void* entvars1, void* entvars2, float value1, float value2, int value) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[5] = {
                entvars1 ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars1)) : v8::Null(isolate).As<v8::Value>(),
                entvars2 ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars2)) : v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, value1),
                v8::Number::New(isolate, value2),
                v8::Integer::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, void*, void*, float, float, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, entvars1, entvars2, value1, value2, value);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[5] = {
                entvars1 ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars1)) : v8::Null(isolate).As<v8::Value>(),
                entvars2 ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars2)) : v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, value1),
                v8::Number::New(isolate, value2),
                v8::Integer::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

void Hook_Void_Entvar_Float_Vector_Trace_Int(Hook* hook, void* pthis, void* attacker, float damage, float* direction, void* trace, int damageBits) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[5] = {
                attacker ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(attacker)) : v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, damage),
                v8::Null(isolate).As<v8::Value>(),
                trace ? structures::wrapTraceResult(isolate, static_cast<TraceResult*>(trace)) : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, damageBits)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, void*, float, float*, void*, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, attacker, damage, direction, trace, damageBits);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[5] = {
                attacker ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(attacker)) : v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, damage),
                v8::Null(isolate).As<v8::Value>(),
                trace ? structures::wrapTraceResult(isolate, static_cast<TraceResult*>(trace)) : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, damageBits)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void* Hook_Cbase_Void(Hook* hook, void* pthis) {
    PUSH_CBASE()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void* (*OrigFunc)(void*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis);
    }

    executeCallbacks(hook, pthis, false);
    POP()

    if (mgr.getCurrentResult() < HAM_OVERRIDE) {
        return origret;
    }
    return origret;  // For now, no override support for CBase returns
}

#ifdef _WIN32
void Hook_Vector_Void(Hook* hook, void* pthis, float* out) {
#else
void Hook_Vector_Void(Hook* hook, float* out, void* pthis) {
#endif
    PUSH_VECTOR()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
#ifdef _WIN32
        typedef void (*OrigFunc)(void*, float*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, origVec);
#else
        typedef void (*OrigFunc)(float*, void*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(origVec, pthis);
#endif
    }

    executeCallbacks(hook, pthis, false);
    POP()

    if (mgr.getCurrentResult() < HAM_OVERRIDE) {
        memcpy(out, origVec, sizeof(float) * 3);
    } else {
        memcpy(out, origVec, sizeof(float) * 3);  // For now, no override support for Vector returns
    }
}

#ifdef _WIN32
void Hook_Vector_pVector(Hook* hook, void* pthis, float* out, float* vec) {
#else
void Hook_Vector_pVector(Hook* hook, float* out, void* pthis, float* vec) {
#endif
    PUSH_VECTOR()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
#ifdef _WIN32
        typedef void (*OrigFunc)(void*, float*, float*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, origVec, vec);
#else
        typedef void (*OrigFunc)(float*, void*, float*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(origVec, pthis, vec);
#endif
    }

    executeCallbacks(hook, pthis, false);
    POP()

    memcpy(out, origVec, sizeof(float) * 3);
}

#ifdef _WIN32
void Hook_Vector_Float(Hook* hook, void* pthis, float* out, float f1) {
#else
void Hook_Vector_Float(Hook* hook, float* out, void* pthis, float f1) {
#endif
    PUSH_VECTOR()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
#ifdef _WIN32
        typedef void (*OrigFunc)(void*, float*, float);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, origVec, f1);
#else
        typedef void (*OrigFunc)(float*, void*, float);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(origVec, pthis, f1);
#endif
    }

    executeCallbacks(hook, pthis, false);
    POP()

    memcpy(out, origVec, sizeof(float) * 3);
}

const char* Hook_Str_Void(Hook* hook, void* pthis) {
    PUSH_STRING()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef const char* (*OrigFunc)(void*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis);
    }

    executeCallbacks(hook, pthis, false);
    POP()

    if (mgr.getCurrentResult() < HAM_OVERRIDE) {
        return origret;
    }
    return origret;  // For now, no override support for string returns
}

const char* Hook_Str_Str(Hook* hook, void* pthis, const char* str) {
    PUSH_STRING()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef const char* (*OrigFunc)(void*, const char*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, str);
    }

    executeCallbacks(hook, pthis, false);
    POP()

    return origret;
}

bool Hook_Bool_Void(Hook* hook, void* pthis) {
    PUSH_BOOL()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef bool (*OrigFunc)(void*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis);
    }

    executeCallbacks(hook, pthis, false);
    POP()
    CHECK_RETURN_BOOL()
}

bool Hook_Bool_Int(Hook* hook, void* pthis, int i1) {
    PUSH_BOOL()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef bool (*OrigFunc)(void*, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, i1);
    }

    executeCallbacks(hook, pthis, false);
    POP()
    CHECK_RETURN_BOOL()
}

bool Hook_Bool_Cbase(Hook* hook, void* pthis, void* cbase) {
    PUSH_BOOL()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef bool (*OrigFunc)(void*, void*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, cbase);
    }

    executeCallbacks(hook, pthis, false);
    POP()
    CHECK_RETURN_BOOL()
}

bool Hook_Bool_Entvar(Hook* hook, void* pthis, void* entvar) {
    PUSH_BOOL()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef bool (*OrigFunc)(void*, void*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, entvar);
    }

    executeCallbacks(hook, pthis, false);
    POP()
    CHECK_RETURN_BOOL()
}

// ============================================================================
// Additional bool implementations
// ============================================================================

bool Hook_Bool_Bool(Hook* hook, void* pthis, bool b1) {
    PUSH_BOOL()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef bool (*OrigFunc)(void*, bool);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, b1);
    }

    executeCallbacks(hook, pthis, false);
    POP()
    CHECK_RETURN_BOOL()
}

bool Hook_Bool_pVector(Hook* hook, void* pthis, float* vec) {
    PUSH_BOOL()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef bool (*OrigFunc)(void*, float*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, vec);
    }

    executeCallbacks(hook, pthis, false);
    POP()
    CHECK_RETURN_BOOL()
}

bool Hook_Bool_pVector_pVector(Hook* hook, void* pthis, float* vec1, float* vec2) {
    PUSH_BOOL()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef bool (*OrigFunc)(void*, float*, float*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, vec1, vec2);
    }

    executeCallbacks(hook, pthis, false);
    POP()
    CHECK_RETURN_BOOL()
}

bool Hook_Bool_Bool_Int(Hook* hook, void* pthis, bool b1, int i1) {
    PUSH_BOOL()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef bool (*OrigFunc)(void*, bool, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, b1, i1);
    }

    executeCallbacks(hook, pthis, false);
    POP()
    CHECK_RETURN_BOOL()
}

bool Hook_Bool_Cbase_Int(Hook* hook, void* pthis, void* cbase, int i1) {
    PUSH_BOOL()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef bool (*OrigFunc)(void*, void*, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, cbase, i1);
    }

    executeCallbacks(hook, pthis, false);
    POP()
    CHECK_RETURN_BOOL()
}

bool Hook_Bool_Cbase_Bool(Hook* hook, void* pthis, void* cbase, bool b1) {
    PUSH_BOOL()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef bool (*OrigFunc)(void*, void*, bool);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, cbase, b1);
    }

    executeCallbacks(hook, pthis, false);
    POP()
    CHECK_RETURN_BOOL()
}

bool Hook_Bool_Entvar_Float(Hook* hook, void* pthis, void* entvar, float f1) {
    PUSH_BOOL()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef bool (*OrigFunc)(void*, void*, float);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, entvar, f1);
    }

    executeCallbacks(hook, pthis, false);
    POP()
    CHECK_RETURN_BOOL()
}

bool Hook_Bool_Float_Int_Int(Hook* hook, void* pthis, float f1, int i1, int i2) {
    PUSH_BOOL()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef bool (*OrigFunc)(void*, float, int, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, f1, i1, i2);
    }

    executeCallbacks(hook, pthis, false);
    POP()
    CHECK_RETURN_BOOL()
}

bool Hook_Bool_ItemInfo(Hook* hook, void* pthis, void* iteminfo) {
    PUSH_BOOL()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef bool (*OrigFunc)(void*, void*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, iteminfo);
    }

    executeCallbacks(hook, pthis, false);
    POP()
    CHECK_RETURN_BOOL()
}

// ============================================================================
// Additional void implementations
// ============================================================================

void Hook_Void_Bool(Hook* hook, void* pthis, bool flag) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                v8::Boolean::New(isolate, flag)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, bool);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, flag);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                v8::Boolean::New(isolate, flag)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Short(Hook* hook, void* pthis, short value) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                v8::Integer::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, short);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, value);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                v8::Integer::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Str(Hook* hook, void* pthis, const char* str) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                str ? v8::String::NewFromUtf8(isolate, str).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, const char*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, str);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                str ? v8::String::NewFromUtf8(isolate, str).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Vector(Hook* hook, void* pthis, float* vec) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, float*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, vec);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Int_Bool(Hook* hook, void* pthis, int value, bool flag) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                v8::Integer::New(isolate, value),
                v8::Boolean::New(isolate, flag)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, int, bool);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, value, flag);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                v8::Integer::New(isolate, value),
                v8::Boolean::New(isolate, flag)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Bool_Bool(Hook* hook, void* pthis, bool flag1, bool flag2) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                v8::Boolean::New(isolate, flag1),
                v8::Boolean::New(isolate, flag2)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, bool, bool);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, flag1, flag2);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                v8::Boolean::New(isolate, flag1),
                v8::Boolean::New(isolate, flag2)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Str_Int(Hook* hook, void* pthis, const char* str, int value) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                str ? v8::String::NewFromUtf8(isolate, str).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, const char*, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, str, value);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                str ? v8::String::NewFromUtf8(isolate, str).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Str_Bool(Hook* hook, void* pthis, const char* str, bool flag) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                str ? v8::String::NewFromUtf8(isolate, str).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                v8::Boolean::New(isolate, flag)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, const char*, bool);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, str, flag);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                str ? v8::String::NewFromUtf8(isolate, str).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                v8::Boolean::New(isolate, flag)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Cbase_Bool(Hook* hook, void* pthis, void* entity, bool flag) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>(),
                v8::Boolean::New(isolate, flag)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, void*, bool);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, entity, flag);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>(),
                v8::Boolean::New(isolate, flag)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_pFloat_pFloat(Hook* hook, void* pthis, float* value1, float* value2) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                value1 ? v8::Number::New(isolate, *value1) : v8::Null(isolate).As<v8::Value>(),
                value2 ? v8::Number::New(isolate, *value2) : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, float*, float*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, value1, value2);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                value1 ? v8::Number::New(isolate, *value1) : v8::Null(isolate).As<v8::Value>(),
                value2 ? v8::Number::New(isolate, *value2) : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Vector_Vector(Hook* hook, void* pthis, float* vec1, float* vec2) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                v8::Null(isolate).As<v8::Value>(),
                v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, float*, float*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, vec1, vec2);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                v8::Null(isolate).As<v8::Value>(),
                v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Entvar_Entvar_Int(Hook* hook, void* pthis, void* entvars1, void* entvars2, int value) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[3] = {
                entvars1 ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars1)) : v8::Null(isolate).As<v8::Value>(),
                entvars2 ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars2)) : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, void*, void*, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, entvars1, entvars2, value);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[3] = {
                entvars1 ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars1)) : v8::Null(isolate).As<v8::Value>(),
                entvars2 ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars2)) : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Int_Str_Bool(Hook* hook, void* pthis, int value, const char* str, bool flag) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[3] = {
                v8::Integer::New(isolate, value),
                str ? v8::String::NewFromUtf8(isolate, str).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                v8::Boolean::New(isolate, flag)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, int, const char*, bool);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, value, str, flag);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[3] = {
                v8::Integer::New(isolate, value),
                str ? v8::String::NewFromUtf8(isolate, str).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                v8::Boolean::New(isolate, flag)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Str_Str_Int(Hook* hook, void* pthis, const char* str1, const char* str2, int value) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[3] = {
                str1 ? v8::String::NewFromUtf8(isolate, str1).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                str2 ? v8::String::NewFromUtf8(isolate, str2).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, const char*, const char*, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, str1, str2, value);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[3] = {
                str1 ? v8::String::NewFromUtf8(isolate, str1).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                str2 ? v8::String::NewFromUtf8(isolate, str2).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Float_Cbase(Hook* hook, void* pthis, float value, void* entity) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                v8::Number::New(isolate, value),
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, float, void*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, value, entity);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                v8::Number::New(isolate, value),
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Cbase_Int_Float(Hook* hook, void* pthis, void* entity, int value, float value2) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[3] = {
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, value),
                v8::Number::New(isolate, value2)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, void*, int, float);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, entity, value, value2);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[3] = {
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, value),
                v8::Number::New(isolate, value2)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Cbase_pVector_Float(Hook* hook, void* pthis, void* entity, float* vec, float value) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[3] = {
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>(),
                utils::vect2js(isolate, vec),
                v8::Number::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, void*, float*, float);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, entity, vec, value);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[3] = {
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>(),
                utils::vect2js(isolate, vec),
                v8::Number::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Str_Float_Float_Float(Hook* hook, void* pthis, const char* str, float value1, float value2, float value3) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[4] = {
                str ? v8::String::NewFromUtf8(isolate, str).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, value1),
                v8::Number::New(isolate, value2),
                v8::Number::New(isolate, value3)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, const char*, float, float, float);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, str, value1, value2, value3);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[4] = {
                str ? v8::String::NewFromUtf8(isolate, str).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, value1),
                v8::Number::New(isolate, value2),
                v8::Number::New(isolate, value3)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Str_Float_Float_Float_Int_Cbase(Hook* hook, void* pthis, const char* str, float value1, float value2, float value3, int value, void* entity) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[6] = {
                str ? v8::String::NewFromUtf8(isolate, str).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, value1),
                v8::Number::New(isolate, value2),
                v8::Number::New(isolate, value3),
                v8::Integer::New(isolate, value),
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, const char*, float, float, float, int, void*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, str, value1, value2, value3, value, entity);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[6] = {
                str ? v8::String::NewFromUtf8(isolate, str).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, value1),
                v8::Number::New(isolate, value2),
                v8::Number::New(isolate, value3),
                v8::Integer::New(isolate, value),
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Str_Float_Float_Float_Bool_Cbase(Hook* hook, void* pthis, const char* str, float value1, float value2, float value3, bool flag, void* entity) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[6] = {
                str ? v8::String::NewFromUtf8(isolate, str).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, value1),
                v8::Number::New(isolate, value2),
                v8::Number::New(isolate, value3),
                v8::Boolean::New(isolate, flag),
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, const char*, float, float, float, bool, void*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, str, value1, value2, value3, flag, entity);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[6] = {
                str ? v8::String::NewFromUtf8(isolate, str).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, value1),
                v8::Number::New(isolate, value2),
                v8::Number::New(isolate, value3),
                v8::Boolean::New(isolate, flag),
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Entvar_Entvar_Float_Int_Int(Hook* hook, void* pthis, void* entvars1, void* entvars2, float value, int value1, int value2) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[5] = {
                entvars1 ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars1)) : v8::Null(isolate).As<v8::Value>(),
                entvars2 ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars2)) : v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, value),
                v8::Integer::New(isolate, value1),
                v8::Integer::New(isolate, value2)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, void*, void*, float, int, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, entvars1, entvars2, value, value1, value2);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[5] = {
                entvars1 ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars1)) : v8::Null(isolate).As<v8::Value>(),
                entvars2 ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars2)) : v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, value),
                v8::Integer::New(isolate, value1),
                v8::Integer::New(isolate, value2)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

void Hook_Void_Vector_Entvar_Entvar_Float_Int_Int(Hook* hook, void* pthis, float* vec, void* entvars1, void* entvars2, float value, int value1, int value2) {
    PUSH_VOID()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[6] = {
                v8::Null(isolate).As<v8::Value>(),
                entvars1 ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars1)) : v8::Null(isolate).As<v8::Value>(),
                entvars2 ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars2)) : v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, value),
                v8::Integer::New(isolate, value1),
                v8::Integer::New(isolate, value2)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef void (*OrigFunc)(void*, float*, void*, void*, float, int, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, vec, entvars1, entvars2, value, value1, value2);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[6] = {
                v8::Null(isolate).As<v8::Value>(),
                entvars1 ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars1)) : v8::Null(isolate).As<v8::Value>(),
                entvars2 ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars2)) : v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, value),
                v8::Integer::New(isolate, value1),
                v8::Integer::New(isolate, value2)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
}

// ============================================================================
// Additional int implementations
// ============================================================================

int Hook_Int_Short(Hook* hook, void* pthis, short value) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                v8::Integer::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, short);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, value);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                v8::Integer::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

int Hook_Int_Str(Hook* hook, void* pthis, const char* str) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                str ? v8::String::NewFromUtf8(isolate, str).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, const char*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, str);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                str ? v8::String::NewFromUtf8(isolate, str).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

int Hook_Int_Vector(Hook* hook, void* pthis, float* vec) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, float*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, vec);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

int Hook_Int_Cbase_Bool(Hook* hook, void* pthis, void* entity, bool flag) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>(),
                v8::Boolean::New(isolate, flag)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, void*, bool);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, entity, flag);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>(),
                v8::Boolean::New(isolate, flag)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

int Hook_Int_Entvar_Float(Hook* hook, void* pthis, void* entvars, float value) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                entvars ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars)) : v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, void*, float);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, entvars, value);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                entvars ? structures::wrapEntvars(isolate, static_cast<entvars_t*>(entvars)) : v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, value)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

int Hook_Int_Cbase_pVector(Hook* hook, void* pthis, void* entity, float* vec) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>(),
                utils::vect2js(isolate, vec)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, void*, float*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, entity, vec);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>(),
                utils::vect2js(isolate, vec)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

int Hook_Int_Vector_Cbase(Hook* hook, void* pthis, float* vec, void* entity) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                v8::Null(isolate).As<v8::Value>(),
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, float*, void*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, vec, entity);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                v8::Null(isolate).As<v8::Value>(),
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

int Hook_Int_Vector_Vector(Hook* hook, void* pthis, float* vec1, float* vec2) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                v8::Null(isolate).As<v8::Value>(),
                v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, float*, float*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, vec1, vec2);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                v8::Null(isolate).As<v8::Value>(),
                v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

int Hook_Int_Str_Str(Hook* hook, void* pthis, const char* str1, const char* str2) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                str1 ? v8::String::NewFromUtf8(isolate, str1).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                str2 ? v8::String::NewFromUtf8(isolate, str2).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, const char*, const char*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, str1, str2);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[2] = {
                str1 ? v8::String::NewFromUtf8(isolate, str1).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                str2 ? v8::String::NewFromUtf8(isolate, str2).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

int Hook_Int_ItemInfo(Hook* hook, void* pthis, void* itemInfo) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, void*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, itemInfo);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[1] = {
                v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

int Hook_Int_Float_Int_Int(Hook* hook, void* pthis, float value, int value1, int value2) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[3] = {
                v8::Number::New(isolate, value),
                v8::Integer::New(isolate, value1),
                v8::Integer::New(isolate, value2)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, float, int, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, value, value1, value2);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[3] = {
                v8::Number::New(isolate, value),
                v8::Integer::New(isolate, value1),
                v8::Integer::New(isolate, value2)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

int Hook_Int_Int_Int_Float_Int(Hook* hook, void* pthis, int value1, int value2, float value, int value3) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[4] = {
                v8::Integer::New(isolate, value1),
                v8::Integer::New(isolate, value2),
                v8::Number::New(isolate, value),
                v8::Integer::New(isolate, value3)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, int, int, float, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, value1, value2, value, value3);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[4] = {
                v8::Integer::New(isolate, value1),
                v8::Integer::New(isolate, value2),
                v8::Number::New(isolate, value),
                v8::Integer::New(isolate, value3)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

int Hook_Int_Int_Str_Int_Int(Hook* hook, void* pthis, int value1, const char* str, int value2, int value3) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[4] = {
                v8::Integer::New(isolate, value1),
                str ? v8::String::NewFromUtf8(isolate, str).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, value2),
                v8::Integer::New(isolate, value3)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, int, const char*, int, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, value1, str, value2, value3);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[4] = {
                v8::Integer::New(isolate, value1),
                str ? v8::String::NewFromUtf8(isolate, str).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, value2),
                v8::Integer::New(isolate, value3)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

int Hook_Int_Int_Str_Int_Bool(Hook* hook, void* pthis, int value1, const char* str, int value2, bool flag) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[4] = {
                v8::Integer::New(isolate, value1),
                str ? v8::String::NewFromUtf8(isolate, str).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, value2),
                v8::Boolean::New(isolate, flag)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, int, const char*, int, bool);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, value1, str, value2, flag);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[4] = {
                v8::Integer::New(isolate, value1),
                str ? v8::String::NewFromUtf8(isolate, str).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, value2),
                v8::Boolean::New(isolate, flag)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

int Hook_Int_Str_Vector_Str(Hook* hook, void* pthis, const char* str1, float* vec, const char* str2) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[3] = {
                str1 ? v8::String::NewFromUtf8(isolate, str1).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                v8::Null(isolate).As<v8::Value>(),
                str2 ? v8::String::NewFromUtf8(isolate, str2).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, const char*, float*, const char*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, str1, vec, str2);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[3] = {
                str1 ? v8::String::NewFromUtf8(isolate, str1).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                v8::Null(isolate).As<v8::Value>(),
                str2 ? v8::String::NewFromUtf8(isolate, str2).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

int Hook_Int_Str_Str_Int_Str_Int_Int(Hook* hook, void* pthis, const char* str1, const char* str2, int value1, const char* str3, int value2, int value3) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[6] = {
                str1 ? v8::String::NewFromUtf8(isolate, str1).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                str2 ? v8::String::NewFromUtf8(isolate, str2).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, value1),
                str3 ? v8::String::NewFromUtf8(isolate, str3).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, value2),
                v8::Integer::New(isolate, value3)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, const char*, const char*, int, const char*, int, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, str1, str2, value1, str3, value2, value3);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[6] = {
                str1 ? v8::String::NewFromUtf8(isolate, str1).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                str2 ? v8::String::NewFromUtf8(isolate, str2).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, value1),
                str3 ? v8::String::NewFromUtf8(isolate, str3).ToLocalChecked().As<v8::Value>() : v8::Null(isolate).As<v8::Value>(),
                v8::Integer::New(isolate, value2),
                v8::Integer::New(isolate, value3)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

int Hook_Int_Vector_Vector_Float_Float(Hook* hook, void* pthis, float* vec1, float* vec2, float value1, float value2) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[4] = {
                v8::Null(isolate).As<v8::Value>(),
                v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, value1),
                v8::Number::New(isolate, value2)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, float*, float*, float, float);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, vec1, vec2, value1, value2);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[4] = {
                v8::Null(isolate).As<v8::Value>(),
                v8::Null(isolate).As<v8::Value>(),
                v8::Number::New(isolate, value1),
                v8::Number::New(isolate, value2)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

int Hook_Int_pVector_pVector_Cbase_pFloat(Hook* hook, void* pthis, float* vec1, float* vec2, void* entity, float* value) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[4] = {
                utils::vect2js(isolate, vec1),
                utils::vect2js(isolate, vec2),
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>(),
                value ? v8::Number::New(isolate, *value) : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, float*, float*, void*, float*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, vec1, vec2, entity, value);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[4] = {
                utils::vect2js(isolate, vec1),
                utils::vect2js(isolate, vec2),
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>(),
                value ? v8::Number::New(isolate, *value) : v8::Null(isolate).As<v8::Value>()
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

int Hook_Int_pVector_pVector_Float_Cbase_pVector(Hook* hook, void* pthis, float* vec1, float* vec2, float value, void* entity, float* vec3) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[5] = {
                utils::vect2js(isolate, vec1),
                utils::vect2js(isolate, vec2),
                v8::Number::New(isolate, value),
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>(),
                utils::vect2js(isolate, vec3)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, float*, float*, float, void*, float*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, vec1, vec2, value, entity, vec3);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[5] = {
                utils::vect2js(isolate, vec1),
                utils::vect2js(isolate, vec2),
                v8::Number::New(isolate, value),
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>(),
                utils::vect2js(isolate, vec3)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

int Hook_Int_pVector_pVector_Float_Cbase_pVector_pVector_Bool(Hook* hook, void* pthis, float* vec1, float* vec2, float value, void* entity, float* vec3, float* vec4, bool flag) {
    PUSH_INT()

    // Execute pre-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[7] = {
                utils::vect2js(isolate, vec1),
                utils::vect2js(isolate, vec2),
                v8::Number::New(isolate, value),
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>(),
                utils::vect2js(isolate, vec3),
                utils::vect2js(isolate, vec4),
                v8::Boolean::New(isolate, flag)
            };
            executeCallbacksWithArgs(hook, pthis, true, isolate, extraArgs);
        }
    }

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef int (*OrigFunc)(void*, float*, float*, float, void*, float*, float*, bool);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, vec1, vec2, value, entity, vec3, vec4, flag);
    }

    // Execute post-callbacks with parameters
    {
        v8::Isolate* isolate = mgr.getIsolate();
        if (isolate) {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Local<v8::Value> extraArgs[7] = {
                utils::vect2js(isolate, vec1),
                utils::vect2js(isolate, vec2),
                v8::Number::New(isolate, value),
                getEdictFromThis(entity) ? structures::wrapEntity(isolate, getEdictFromThis(entity)) : v8::Null(isolate).As<v8::Value>(),
                utils::vect2js(isolate, vec3),
                utils::vect2js(isolate, vec4),
                v8::Boolean::New(isolate, flag)
            };
            executeCallbacksWithArgs(hook, pthis, false, isolate, extraArgs);
        }
    }
    POP()
    CHECK_RETURN_INT()
}

// ============================================================================
// Additional float implementations
// ============================================================================

float Hook_Float_Int_Float(Hook* hook, void* pthis, int i1, float f1) {
    PUSH_FLOAT()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef float (*OrigFunc)(void*, int, float);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, i1, f1);
    }

    executeCallbacks(hook, pthis, false);
    POP()
    CHECK_RETURN_FLOAT()
}

float Hook_Float_Float_Cbase(Hook* hook, void* pthis, float f1, void* cbase) {
    PUSH_FLOAT()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
        typedef float (*OrigFunc)(void*, float, void*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        origret = orig(pthis, f1, cbase);
    }

    executeCallbacks(hook, pthis, false);
    POP()
    CHECK_RETURN_FLOAT()
}

// ============================================================================
// Additional Vector implementations
// ============================================================================

#ifdef _WIN32
void Hook_Vector_Float_Cbase_Int(Hook* hook, void* pthis, float* out, float f1, void* cbase, int i1) {
#else
void Hook_Vector_Float_Cbase_Int(Hook* hook, float* out, void* pthis, float f1, void* cbase, int i1) {
#endif
    PUSH_VECTOR()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
#ifdef _WIN32
        typedef void (*OrigFunc)(void*, float*, float, void*, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, origVec, f1, cbase, i1);
#else
        typedef void (*OrigFunc)(float*, void*, float, void*, int);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(origVec, pthis, f1, cbase, i1);
#endif
    }

    executeCallbacks(hook, pthis, false);
    POP()

    memcpy(out, origVec, sizeof(float) * 3);
}

#ifdef _WIN32
void Hook_Vector_Vector_Vector_Vector(Hook* hook, void* pthis, float* out, float* v1, float* v2, float* v3) {
#else
void Hook_Vector_Vector_Vector_Vector(Hook* hook, float* out, void* pthis, float* v1, float* v2, float* v3) {
#endif
    PUSH_VECTOR()

    executeCallbacks(hook, pthis, true);

    if (mgr.getCurrentResult() < HAM_SUPERCEDE) {
#ifdef _WIN32
        typedef void (*OrigFunc)(void*, float*, float*, float*, float*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(pthis, origVec, v1, v2, v3);
#else
        typedef void (*OrigFunc)(float*, void*, float*, float*, float*);
        OrigFunc orig = reinterpret_cast<OrigFunc>(hook->getOriginalFunction());
        orig(origVec, pthis, v1, v2, v3);
#endif
    }

    executeCallbacks(hook, pthis, false);
    POP()

    memcpy(out, origVec, sizeof(float) * 3);
}

// ============================================================================
// Deprecated/Removed placeholder
// ============================================================================

void Hook_Deprecated(Hook* hook) {
    // Deprecated function - do nothing
}

void Hook_Removed(Hook* hook) {
    // Removed function - do nothing
}

} // namespace Ham
