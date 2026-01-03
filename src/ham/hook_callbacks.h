#ifndef HAM_HOOK_CALLBACKS_H
#define HAM_HOOK_CALLBACKS_H

#include "ham_const.h"

namespace Ham {

class Hook;

void* getCallbackForFunction(HamType function);

// Callback prototypes for different function signatures
// These are called by the trampolines and match AMX Mod X signatures

// ============================================================================
// Basic signatures
// ============================================================================

// void(this)
void Hook_Void_Void(Hook* hook, void* pthis);

// int(this)
int Hook_Int_Void(Hook* hook, void* pthis);

// float(this)
float Hook_Float_Void(Hook* hook, void* pthis);

// ============================================================================
// Single parameter signatures
// ============================================================================

// void(this, int)
void Hook_Void_Int(Hook* hook, void* pthis, int i1);

// void(this, float)
void Hook_Void_Float(Hook* hook, void* pthis, float f1);

// void(this, entvar*)
void Hook_Void_Entvar(Hook* hook, void* pthis, void* entvar);

// void(this, cbase*)
void Hook_Void_Cbase(Hook* hook, void* pthis, void* cbase);

// void(this, edict*)
void Hook_Void_Edict(Hook* hook, void* pthis, void* edict);

// int(this, int)
int Hook_Int_Int(Hook* hook, void* pthis, int i1);

// int(this, float)
int Hook_Int_Float(Hook* hook, void* pthis, float f1);

// int(this, entvar*)
int Hook_Int_Entvar(Hook* hook, void* pthis, void* entvar);

// int(this, cbase*)
int Hook_Int_Cbase(Hook* hook, void* pthis, void* cbase);

// int(this, pVector)
int Hook_Int_pVector(Hook* hook, void* pthis, float* vec);

// float(this, int)
float Hook_Float_Int(Hook* hook, void* pthis, int i1);

// float(this, float)
float Hook_Float_Float(Hook* hook, void* pthis, float f1);

// ============================================================================
// Two parameter signatures
// ============================================================================

// void(this, int, int)
void Hook_Void_Int_Int(Hook* hook, void* pthis, int i1, int i2);

// void(this, float, float)
void Hook_Void_Float_Float(Hook* hook, void* pthis, float f1, float f2);

// void(this, float, int)
void Hook_Void_Float_Int(Hook* hook, void* pthis, float f1, int i1);

// void(this, entvar*, int)
void Hook_Void_Entvar_Int(Hook* hook, void* pthis, void* entvar, int value);

// void(this, entvar*, float)
void Hook_Void_Entvar_Float(Hook* hook, void* pthis, void* entvar, float f1);

// void(this, cbase*, int)
void Hook_Void_Cbase_Int(Hook* hook, void* pthis, void* cbase, int i1);

// void(this, cbase*, float)
void Hook_Void_Cbase_Float(Hook* hook, void* pthis, void* cbase, float f1);

// int(this, int, int)
int Hook_Int_Int_Int(Hook* hook, void* pthis, int i1, int i2);

// int(this, float, int)
int Hook_Int_Float_Int(Hook* hook, void* pthis, float f, int i);

// int(this, float, float)
int Hook_Int_Float_Float(Hook* hook, void* pthis, float f1, float f2);

// int(this, pVector, pVector)
int Hook_Int_pVector_pVector(Hook* hook, void* pthis, float* vec1, float* vec2);

// ============================================================================
// Three parameter signatures
// ============================================================================

// void(this, int, int, int)
void Hook_Void_Int_Int_Int(Hook* hook, void* pthis, int i1, int i2, int i3);

// void(this, entvar*, entvar*, float)
void Hook_Void_Entvar_Entvar_Float(Hook* hook, void* pthis, void* entvar1, void* entvar2, float f1);

// void(this, entvar*, float, float)
void Hook_Void_Entvar_Float_Float(Hook* hook, void* pthis, void* entvar, float f1, float f2);

// int(this, int, str, int) - GiveAmmo
int Hook_Int_Int_Str_Int(Hook* hook, void* pthis, int amount, const char* name, int max);

// int(this, cbase*, int)
int Hook_Int_Cbase_Int(Hook* hook, void* pthis, void* cbase, int i1);

// ============================================================================
// Four parameter signatures
// ============================================================================

// void(this, cbase*, cbase*, int, float) - Use
void Hook_Void_Cbase_Cbase_Int_Float(Hook* hook, void* pthis, void* activator, void* caller, int useType, float value);

// int(this, entvar*, entvar*, float, int) - TakeDamage
int Hook_Int_Entvar_Entvar_Float_Int(Hook* hook, void* pthis, void* inflictor, void* attacker, float damage, int damageBits);

// void(this, float, float, float, int)
void Hook_Void_Float_Float_Float_Int(Hook* hook, void* pthis, float f1, float f2, float f3, int i1);

// ============================================================================
// Five parameter signatures
// ============================================================================

// void(this, float, Vector, TraceResult*, int) - TraceBleed
void Hook_Void_Float_Vector_Trace_Int(Hook* hook, void* pthis, float damage, float* dir, void* tr, int damageBits);

// int(this, entvar*, entvar*, float, float, int) - ESF TakeDamage2
int Hook_Int_Entvar_Entvar_Float_Float_Int(Hook* hook, void* pthis, void* inflictor, void* attacker, float damage, float unknown, int damageBits);

// ============================================================================
// Six parameter signatures
// ============================================================================

// void(this, entvar*, float, Vector, TraceResult*, int) - TraceAttack
void Hook_Void_Entvar_Float_Vector_Trace_Int(Hook* hook, void* pthis, void* attacker, float damage, float* dir, void* tr, int damageBits);

// ============================================================================
// Return type: CBaseEntity*
// ============================================================================

// CBaseEntity*(this)
void* Hook_Cbase_Void(Hook* hook, void* pthis);

// ============================================================================
// Return type: Vector
// ============================================================================

// Vector(this)
#ifdef _WIN32
void Hook_Vector_Void(Hook* hook, void* pthis, float* out);
#else
void Hook_Vector_Void(Hook* hook, float* out, void* pthis);
#endif

// Vector(this, pVector)
#ifdef _WIN32
void Hook_Vector_pVector(Hook* hook, void* pthis, float* out, float* vec);
#else
void Hook_Vector_pVector(Hook* hook, float* out, void* pthis, float* vec);
#endif

// Vector(this, float)
#ifdef _WIN32
void Hook_Vector_Float(Hook* hook, void* pthis, float* out, float f1);
#else
void Hook_Vector_Float(Hook* hook, float* out, void* pthis, float f1);
#endif

// ============================================================================
// Return type: const char*
// ============================================================================

// const char*(this)
const char* Hook_Str_Void(Hook* hook, void* pthis);

// const char*(this, str)
const char* Hook_Str_Str(Hook* hook, void* pthis, const char* str);

// ============================================================================
// Bool signatures (for mods that use bool)
// ============================================================================

// bool(this)
bool Hook_Bool_Void(Hook* hook, void* pthis);

// bool(this, int)
bool Hook_Bool_Int(Hook* hook, void* pthis, int i1);

// bool(this, bool)
bool Hook_Bool_Bool(Hook* hook, void* pthis, bool b1);

// bool(this, cbase*)
bool Hook_Bool_Cbase(Hook* hook, void* pthis, void* cbase);

// bool(this, entvar*)
bool Hook_Bool_Entvar(Hook* hook, void* pthis, void* entvar);

// bool(this, pVector)
bool Hook_Bool_pVector(Hook* hook, void* pthis, float* vec);

// bool(this, pVector, pVector)
bool Hook_Bool_pVector_pVector(Hook* hook, void* pthis, float* vec1, float* vec2);

// bool(this, bool, int)
bool Hook_Bool_Bool_Int(Hook* hook, void* pthis, bool b1, int i1);

// bool(this, cbase*, int)
bool Hook_Bool_Cbase_Int(Hook* hook, void* pthis, void* cbase, int i1);

// bool(this, cbase*, bool)
bool Hook_Bool_Cbase_Bool(Hook* hook, void* pthis, void* cbase, bool b1);

// bool(this, entvar*, float)
bool Hook_Bool_Entvar_Float(Hook* hook, void* pthis, void* entvar, float f1);

// bool(this, float, int, int)
bool Hook_Bool_Float_Int_Int(Hook* hook, void* pthis, float f1, int i1, int i2);

// bool(this, ItemInfo*)
bool Hook_Bool_ItemInfo(Hook* hook, void* pthis, void* iteminfo);

// ============================================================================
// Additional void signatures
// ============================================================================

// void(this, bool)
void Hook_Void_Bool(Hook* hook, void* pthis, bool b1);

// void(this, short)
void Hook_Void_Short(Hook* hook, void* pthis, short s1);

// void(this, str)
void Hook_Void_Str(Hook* hook, void* pthis, const char* str);

// void(this, Vector)
void Hook_Void_Vector(Hook* hook, void* pthis, float* vec);

// void(this, int, bool)
void Hook_Void_Int_Bool(Hook* hook, void* pthis, int i1, bool b1);

// void(this, bool, bool)
void Hook_Void_Bool_Bool(Hook* hook, void* pthis, bool b1, bool b2);

// void(this, str, int)
void Hook_Void_Str_Int(Hook* hook, void* pthis, const char* str, int i1);

// void(this, str, bool)
void Hook_Void_Str_Bool(Hook* hook, void* pthis, const char* str, bool b1);

// void(this, cbase*, bool)
void Hook_Void_Cbase_Bool(Hook* hook, void* pthis, void* cbase, bool b1);

// void(this, pFloat, pFloat)
void Hook_Void_pFloat_pFloat(Hook* hook, void* pthis, float* f1, float* f2);

// void(this, Vector, Vector)
void Hook_Void_Vector_Vector(Hook* hook, void* pthis, float* v1, float* v2);

// void(this, entvar*, entvar*, int)
void Hook_Void_Entvar_Entvar_Int(Hook* hook, void* pthis, void* ev1, void* ev2, int i1);

// void(this, int, str, bool)
void Hook_Void_Int_Str_Bool(Hook* hook, void* pthis, int i1, const char* str, bool b1);

// void(this, str, str, int)
void Hook_Void_Str_Str_Int(Hook* hook, void* pthis, const char* s1, const char* s2, int i1);

// void(this, float, cbase*)
void Hook_Void_Float_Cbase(Hook* hook, void* pthis, float f1, void* cbase);

// void(this, cbase*, int, float)
void Hook_Void_Cbase_Int_Float(Hook* hook, void* pthis, void* cbase, int i1, float f1);

// void(this, cbase*, pVector, float)
void Hook_Void_Cbase_pVector_Float(Hook* hook, void* pthis, void* cbase, float* vec, float f1);

// void(this, str, float, float, float)
void Hook_Void_Str_Float_Float_Float(Hook* hook, void* pthis, const char* str, float f1, float f2, float f3);

// void(this, str, float, float, float, int, cbase*)
void Hook_Void_Str_Float_Float_Float_Int_Cbase(Hook* hook, void* pthis, const char* str, float f1, float f2, float f3, int i1, void* cbase);

// void(this, str, float, float, float, bool, cbase*)
void Hook_Void_Str_Float_Float_Float_Bool_Cbase(Hook* hook, void* pthis, const char* str, float f1, float f2, float f3, bool b1, void* cbase);

// void(this, entvar*, entvar*, float, int, int)
void Hook_Void_Entvar_Entvar_Float_Int_Int(Hook* hook, void* pthis, void* ev1, void* ev2, float f1, int i1, int i2);

// void(this, Vector, entvar*, entvar*, float, int, int)
void Hook_Void_Vector_Entvar_Entvar_Float_Int_Int(Hook* hook, void* pthis, float* vec, void* ev1, void* ev2, float f1, int i1, int i2);

// ============================================================================
// Additional int signatures
// ============================================================================

// int(this, short)
int Hook_Int_Short(Hook* hook, void* pthis, short s1);

// int(this, str)
int Hook_Int_Str(Hook* hook, void* pthis, const char* str);

// int(this, Vector)
int Hook_Int_Vector(Hook* hook, void* pthis, float* vec);

// int(this, cbase*, bool)
int Hook_Int_Cbase_Bool(Hook* hook, void* pthis, void* cbase, bool b1);

// int(this, entvar*, float)
int Hook_Int_Entvar_Float(Hook* hook, void* pthis, void* entvar, float f1);

// int(this, cbase*, pVector)
int Hook_Int_Cbase_pVector(Hook* hook, void* pthis, void* cbase, float* vec);

// int(this, Vector, cbase*)
int Hook_Int_Vector_Cbase(Hook* hook, void* pthis, float* vec, void* cbase);

// int(this, Vector, Vector)
int Hook_Int_Vector_Vector(Hook* hook, void* pthis, float* v1, float* v2);

// int(this, str, str)
int Hook_Int_Str_Str(Hook* hook, void* pthis, const char* s1, const char* s2);

// int(this, ItemInfo*)
int Hook_Int_ItemInfo(Hook* hook, void* pthis, void* iteminfo);

// int(this, float, int, int)
int Hook_Int_Float_Int_Int(Hook* hook, void* pthis, float f1, int i1, int i2);

// int(this, int, int, float, int)
int Hook_Int_Int_Int_Float_Int(Hook* hook, void* pthis, int i1, int i2, float f1, int i3);

// int(this, int, str, int, int)
int Hook_Int_Int_Str_Int_Int(Hook* hook, void* pthis, int i1, const char* str, int i2, int i3);

// int(this, int, str, int, bool)
int Hook_Int_Int_Str_Int_Bool(Hook* hook, void* pthis, int i1, const char* str, int i2, bool b1);

// int(this, str, Vector, str)
int Hook_Int_Str_Vector_Str(Hook* hook, void* pthis, const char* s1, float* vec, const char* s2);

// int(this, str, str, int, str, int, int)
int Hook_Int_Str_Str_Int_Str_Int_Int(Hook* hook, void* pthis, const char* s1, const char* s2, int i1, const char* s3, int i2, int i3);

// int(this, Vector, Vector, float, float)
int Hook_Int_Vector_Vector_Float_Float(Hook* hook, void* pthis, float* v1, float* v2, float f1, float f2);

// int(this, pVector, pVector, cbase*, pFloat)
int Hook_Int_pVector_pVector_Cbase_pFloat(Hook* hook, void* pthis, float* v1, float* v2, void* cbase, float* pf);

// int(this, pVector, pVector, float, cbase*, pVector)
int Hook_Int_pVector_pVector_Float_Cbase_pVector(Hook* hook, void* pthis, float* v1, float* v2, float f1, void* cbase, float* v3);

// int(this, pVector, pVector, float, cbase*, pVector, pVector, bool)
int Hook_Int_pVector_pVector_Float_Cbase_pVector_pVector_Bool(Hook* hook, void* pthis, float* v1, float* v2, float f1, void* cbase, float* v3, float* v4, bool b1);

// ============================================================================
// Additional float signatures
// ============================================================================

// float(this, int, float)
float Hook_Float_Int_Float(Hook* hook, void* pthis, int i1, float f1);

// float(this, float, cbase*)
float Hook_Float_Float_Cbase(Hook* hook, void* pthis, float f1, void* cbase);

// ============================================================================
// Additional Vector signatures
// ============================================================================

// Vector(this, float, cbase*, int)
#ifdef _WIN32
void Hook_Vector_Float_Cbase_Int(Hook* hook, void* pthis, float* out, float f1, void* cbase, int i1);
#else
void Hook_Vector_Float_Cbase_Int(Hook* hook, float* out, void* pthis, float f1, void* cbase, int i1);
#endif

// Vector(this, Vector, Vector, Vector)
#ifdef _WIN32
void Hook_Vector_Vector_Vector_Vector(Hook* hook, void* pthis, float* out, float* v1, float* v2, float* v3);
#else
void Hook_Vector_Vector_Vector_Vector(Hook* hook, float* out, void* pthis, float* v1, float* v2, float* v3);
#endif

// ============================================================================
// Deprecated/removed function placeholder
// ============================================================================
void Hook_Deprecated(Hook* hook);
void Hook_Removed(Hook* hook);

} // namespace Ham

#endif // HAM_HOOK_CALLBACKS_H
