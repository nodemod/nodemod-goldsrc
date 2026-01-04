#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include "ham_manager.h"
#include "hook_callbacks.h"
#include <cstring>
#include <dlfcn.h>
#include "extdll.h"
#include "meta_api.h"
#include "mutil.h"

// External references
extern enginefuncs_t g_engfuncs;
extern mutil_funcs_t *gpMetaUtilFuncs;
extern globalvars_t *gpGlobals;

namespace Ham {

HamFunctionInfo g_hamFunctions[] = {
    // CBaseEntity virtuals (0-54)
    {"spawn",      HAM_RET_VOID,   0, {}},                                                      // 0
    {"precache",   HAM_RET_VOID,   0, {}},                                                      // 1
    {"keyvalue",   HAM_RET_VOID,   1, {HAM_PARAM_STRING}},                                      // 2
    {"objectcaps", HAM_RET_INT,    0, {}},                                                      // 3
    {"activate",   HAM_RET_VOID,   0, {}},                                                      // 4
    {"setobjectcollisionbox", HAM_RET_VOID, 0, {}},                                             // 5
    {"classify",   HAM_RET_INT,    0, {}},                                                      // 6
    {"deathnotice",HAM_RET_VOID,   0, {}},                                                      // 7
    {"traceattack",HAM_RET_VOID,   5, {HAM_PARAM_ENTVAR, HAM_PARAM_FLOAT, HAM_PARAM_VECTOR, HAM_PARAM_TRACE, HAM_PARAM_INT}}, // 8
    {"takedamage", HAM_RET_INT,    4, {HAM_PARAM_ENTVAR, HAM_PARAM_ENTVAR, HAM_PARAM_FLOAT, HAM_PARAM_INT}}, // 9
    {"takehealth", HAM_RET_INT,    2, {HAM_PARAM_FLOAT, HAM_PARAM_INT}},                        // 10
    {"killed",     HAM_RET_VOID,   2, {HAM_PARAM_ENTVAR, HAM_PARAM_INT}},                       // 11
    {"bloodcolor", HAM_RET_INT,    0, {}},                                                      // 12
    {"tracebleed", HAM_RET_VOID,   3, {HAM_PARAM_FLOAT, HAM_PARAM_VECTOR, HAM_PARAM_TRACE}},    // 13
    {"istriggered",HAM_RET_INT,    1, {HAM_PARAM_ENTITY}},                                      // 14
    {"mymonsterpointer", HAM_RET_ENTITY, 0, {}},                                                // 15
    {"mysquadmonsterpointer", HAM_RET_ENTITY, 0, {}},                                           // 16
    {"gettogglestate", HAM_RET_INT, 0, {}},                                                     // 17
    {"addpoints",  HAM_RET_VOID,   2, {HAM_PARAM_INT, HAM_PARAM_INT}},                          // 18
    {"addpointstoteam", HAM_RET_VOID, 2, {HAM_PARAM_INT, HAM_PARAM_INT}},                       // 19
    {"addplayeritem", HAM_RET_INT, 1, {HAM_PARAM_ENTITY}},                                      // 20
    {"removeplayeritem", HAM_RET_INT, 1, {HAM_PARAM_ENTITY}},                                   // 21
    {"giveammo",   HAM_RET_INT,    3, {HAM_PARAM_INT, HAM_PARAM_STRING, HAM_PARAM_INT}},        // 22
    {"getdelay",   HAM_RET_FLOAT,  0, {}},                                                      // 23
    {"ismoving",   HAM_RET_INT,    0, {}},                                                      // 24
    {"overridereset", HAM_RET_VOID, 0, {}},                                                     // 25
    {"damagedecal", HAM_RET_INT,   1, {HAM_PARAM_INT}},                                         // 26
    {"settogglestate", HAM_RET_VOID, 1, {HAM_PARAM_INT}},                                       // 27
    {"startsneaking", HAM_RET_VOID, 0, {}},                                                     // 28
    {"stopsneaking", HAM_RET_VOID, 0, {}},                                                      // 29
    {"oncontrols", HAM_RET_INT,    1, {HAM_PARAM_ENTVAR}},                                      // 30
    {"issneaking", HAM_RET_INT,    0, {}},                                                      // 31
    {"isalive",    HAM_RET_INT,    0, {}},                                                      // 32
    {"isbspmodel", HAM_RET_INT,    0, {}},                                                      // 33
    {"reflectgauss", HAM_RET_INT,  0, {}},                                                      // 34
    {"hastarget",  HAM_RET_INT,    1, {HAM_PARAM_STRING}},                                      // 35
    {"isinworld",  HAM_RET_INT,    0, {}},                                                      // 36
    {"isplayer",   HAM_RET_INT,    0, {}},                                                      // 37
    {"isnetclient",HAM_RET_INT,    0, {}},                                                      // 38
    {"teamid",     HAM_RET_STRING, 0, {}},                                                      // 39
    {"getnexttarget", HAM_RET_ENTITY, 0, {}},                                                   // 40
    {"think",      HAM_RET_VOID,   0, {}},                                                      // 41
    {"touch",      HAM_RET_VOID,   1, {HAM_PARAM_ENTITY}},                                      // 42
    {"use",        HAM_RET_VOID,   4, {HAM_PARAM_ENTITY, HAM_PARAM_ENTITY, HAM_PARAM_INT, HAM_PARAM_FLOAT}}, // 43
    {"blocked",    HAM_RET_VOID,   1, {HAM_PARAM_ENTITY}},                                      // 44
    {"respawn",    HAM_RET_ENTITY, 0, {}},                                                      // 45
    {"updateowner",HAM_RET_VOID,   0, {}},                                                      // 46
    {"fbecomeprone", HAM_RET_INT,  0, {}},                                                      // 47
    {"center",     HAM_RET_VECTOR, 0, {}},                                                      // 48
    {"eyeposition",HAM_RET_VECTOR, 0, {}},                                                      // 49
    {"earposition",HAM_RET_VECTOR, 0, {}},                                                      // 50
    {"bodytarget", HAM_RET_VECTOR, 1, {HAM_PARAM_VECTOR}},                                      // 51
    {"illumination", HAM_RET_INT,  0, {}},                                                      // 52
    {"fvisible",   HAM_RET_INT,    1, {HAM_PARAM_ENTITY}},                                      // 53
    {"fvecvisible",HAM_RET_INT,    1, {HAM_PARAM_VECTOR}},                                      // 54

    // CBasePlayer virtuals (55-62)
    {"player_jump", HAM_RET_VOID,  0, {}},                                                      // 55
    {"player_duck", HAM_RET_VOID,  0, {}},                                                      // 56
    {"player_prethink", HAM_RET_VOID, 0, {}},                                                   // 57
    {"player_postthink", HAM_RET_VOID, 0, {}},                                                  // 58
    {"player_getgunposition", HAM_RET_VECTOR, 0, {}},                                           // 59
    {"player_shouldfadeondeath", HAM_RET_INT, 0, {}},                                           // 60
    {"player_impulsecommands", HAM_RET_VOID, 0, {}},                                            // 61
    {"player_updateclientdata", HAM_RET_VOID, 0, {}},                                           // 62

    // CBasePlayerItem virtuals (63-79)
    {"item_addtoplayer", HAM_RET_INT, 1, {HAM_PARAM_ENTITY}},                                   // 63
    {"item_addduplicate", HAM_RET_INT, 1, {HAM_PARAM_ENTITY}},                                  // 64
    {"item_candeploy", HAM_RET_INT, 0, {}},                                                     // 65
    {"item_deploy", HAM_RET_INT,   0, {}},                                                      // 66
    {"item_canholster", HAM_RET_INT, 0, {}},                                                    // 67
    {"item_holster", HAM_RET_VOID, 1, {HAM_PARAM_INT}},                                         // 68
    {"item_updateiteminfo", HAM_RET_VOID, 0, {}},                                               // 69
    {"item_preframe", HAM_RET_VOID, 0, {}},                                                     // 70
    {"item_postframe", HAM_RET_VOID, 0, {}},                                                    // 71
    {"item_drop", HAM_RET_VOID,    0, {}},                                                      // 72
    {"item_kill", HAM_RET_VOID,    0, {}},                                                      // 73
    {"item_attachtoplayer", HAM_RET_VOID, 1, {HAM_PARAM_ENTITY}},                               // 74
    {"item_primaryammoindex", HAM_RET_INT, 0, {}},                                              // 75
    {"item_secondaryammoindex", HAM_RET_INT, 0, {}},                                            // 76
    {"item_updateclientdata", HAM_RET_INT, 1, {HAM_PARAM_ENTITY}},                              // 77
    {"item_getweaponptr", HAM_RET_ENTITY, 0, {}},                                               // 78
    {"item_itemslot", HAM_RET_INT, 0, {}},                                                      // 79

    // CBasePlayerWeapon virtuals (80-93)
    {"weapon_extractammo", HAM_RET_INT, 1, {HAM_PARAM_ENTITY}},                                 // 80
    {"weapon_extractclipammo", HAM_RET_INT, 1, {HAM_PARAM_ENTITY}},                             // 81
    {"weapon_addweapon", HAM_RET_INT, 0, {}},                                                   // 82
    {"weapon_playemptysound", HAM_RET_INT, 0, {}},                                              // 83
    {"weapon_resetemptysound", HAM_RET_VOID, 0, {}},                                            // 84
    {"weapon_sendweaponanim", HAM_RET_VOID, 3, {HAM_PARAM_INT, HAM_PARAM_INT, HAM_PARAM_INT}},  // 85
    {"weapon_isusable", HAM_RET_INT, 0, {}},                                                    // 86
    {"weapon_primaryattack", HAM_RET_VOID, 0, {}},                                              // 87
    {"weapon_secondaryattack", HAM_RET_VOID, 0, {}},                                            // 88
    {"weapon_reload", HAM_RET_VOID, 0, {}},                                                     // 89
    {"weapon_weaponidle", HAM_RET_VOID, 0, {}},                                                 // 90
    {"weapon_retireweapon", HAM_RET_VOID, 0, {}},                                               // 91
    {"weapon_shouldweaponidle", HAM_RET_INT, 0, {}},                                            // 92
    {"weapon_usedecrement", HAM_RET_INT, 0, {}},                                                // 93

    // Mod-specific: The Specialists (94-96)
    {"ts_breakablerespawn", HAM_RET_INT, 1, {HAM_PARAM_INT}},                                   // 94
    {"ts_canusedthroughwalls", HAM_RET_INT, 0, {}},                                             // 95
    {"ts_respawnwait", HAM_RET_VOID, 0, {}},                                                    // 96 (deprecated)

    // Mod-specific: Counter-Strike (97-100)
    {"cstrike_restart", HAM_RET_VOID, 0, {}},                                                   // 97
    {"cstrike_roundrespawn", HAM_RET_VOID, 0, {}},                                              // 98
    {"cstrike_item_candrop", HAM_RET_INT, 0, {}},                                               // 99
    {"cstrike_item_getmaxspeed", HAM_RET_FLOAT, 0, {}},                                         // 100

    // Mod-specific: Day of Defeat (101-108)
    {"dod_roundrespawn", HAM_RET_VOID, 0, {}},                                                  // 101
    {"dod_roundrespawnent", HAM_RET_VOID, 0, {}},                                               // 102
    {"dod_roundstore", HAM_RET_VOID, 0, {}},                                                    // 103
    {"dod_areasetindex", HAM_RET_VOID, 1, {HAM_PARAM_INT}},                                     // 104
    {"dod_areasendstatus", HAM_RET_VOID, 1, {HAM_PARAM_ENTITY}},                                // 105
    {"dod_getstate", HAM_RET_INT, 0, {}},                                                       // 106
    {"dod_getstateent", HAM_RET_INT, 1, {HAM_PARAM_ENTITY}},                                    // 107
    {"dod_item_candrop", HAM_RET_INT, 0, {}},                                                   // 108

    // Mod-specific: Team Fortress Classic (109-116)
    {"tfc_engineeruse", HAM_RET_INT, 1, {HAM_PARAM_ENTITY}},                                    // 109
    {"tfc_finished", HAM_RET_VOID, 0, {}},                                                      // 110
    {"tfc_empexplode", HAM_RET_VOID, 3, {HAM_PARAM_ENTVAR, HAM_PARAM_FLOAT, HAM_PARAM_FLOAT}},  // 111
    {"tfc_calcempdmgrad", HAM_RET_VOID, 2, {HAM_PARAM_FLOAT, HAM_PARAM_FLOAT}},                 // 112
    {"tfc_takeempblast", HAM_RET_VOID, 1, {HAM_PARAM_ENTVAR}},                                  // 113
    {"tfc_empremove", HAM_RET_VOID, 0, {}},                                                     // 114
    {"tfc_takeconcussionblast", HAM_RET_VOID, 2, {HAM_PARAM_ENTVAR, HAM_PARAM_FLOAT}},          // 115
    {"tfc_concuss", HAM_RET_VOID, 1, {HAM_PARAM_ENTVAR}},                                       // 116

    // Mod-specific: Earth's Special Forces (117-118)
    {"esf_isenvmodel", HAM_RET_INT, 0, {}},                                                     // 117
    {"esf_takedamage2", HAM_RET_INT, 5, {HAM_PARAM_ENTVAR, HAM_PARAM_ENTVAR, HAM_PARAM_FLOAT, HAM_PARAM_FLOAT, HAM_PARAM_INT}}, // 118

    // Mod-specific: Natural Selection (119-122)
    {"ns_getpointvalue", HAM_RET_INT, 0, {}},                                                   // 119
    {"ns_awardkill", HAM_RET_VOID, 1, {HAM_PARAM_ENTVAR}},                                      // 120
    {"ns_resetentity", HAM_RET_VOID, 0, {}},                                                    // 121
    {"ns_updateonremove", HAM_RET_VOID, 0, {}},                                                 // 122

    // More TS functions (123-129)
    {"ts_giveslowmul", HAM_RET_VOID, 0, {}},                                                    // 123
    {"ts_goslow", HAM_RET_VOID, 2, {HAM_PARAM_FLOAT, HAM_PARAM_INT}},                           // 124
    {"ts_inslow", HAM_RET_INT, 0, {}},                                                          // 125
    {"ts_isobjective", HAM_RET_INT, 0, {}},                                                     // 126
    {"ts_enableobjective", HAM_RET_VOID, 1, {HAM_PARAM_INT}},                                   // 127
    {"ts_onfreeentprivatedata", HAM_RET_VOID, 0, {}},                                           // 128
    {"ts_shouldcollide", HAM_RET_INT, 1, {HAM_PARAM_ENTITY}},                                   // 129

    // CBaseMonster virtuals (130-144)
    {"changeyaw", HAM_RET_FLOAT, 1, {HAM_PARAM_INT}},                                           // 130
    {"hashumangibs", HAM_RET_INT, 0, {}},                                                       // 131
    {"hasaliengibs", HAM_RET_INT, 0, {}},                                                       // 132
    {"fademonster", HAM_RET_VOID, 0, {}},                                                       // 133
    {"gibmonster", HAM_RET_VOID, 0, {}},                                                        // 134
    {"becomedead", HAM_RET_VOID, 0, {}},                                                        // 135
    {"irelationship", HAM_RET_INT, 1, {HAM_PARAM_ENTITY}},                                      // 136
    {"painsound", HAM_RET_VOID, 0, {}},                                                         // 137
    {"reportaistate", HAM_RET_VOID, 0, {}},                                                     // 138
    {"monsterinitdead", HAM_RET_VOID, 0, {}},                                                   // 139
    {"look", HAM_RET_VOID, 1, {HAM_PARAM_INT}},                                                 // 140
    {"bestvisibleenemy", HAM_RET_ENTITY, 0, {}},                                                // 141
    {"finviewcone", HAM_RET_INT, 1, {HAM_PARAM_ENTITY}},                                        // 142
    {"fvecinviewcone", HAM_RET_INT, 1, {HAM_PARAM_VECTOR}},                                     // 143
    {"getdeathactivity", HAM_RET_INT, 0, {}},                                                   // 144

    // More CBaseMonster virtuals (145-185)
    {"runai", HAM_RET_VOID, 0, {}},                                                             // 145
    {"monsterthink", HAM_RET_VOID, 0, {}},                                                      // 146
    {"monsterinit", HAM_RET_VOID, 0, {}},                                                       // 147
    {"checklocalmove", HAM_RET_INT, 4, {HAM_PARAM_VECTOR, HAM_PARAM_VECTOR, HAM_PARAM_ENTITY, HAM_PARAM_FLOAT}}, // 148
    {"move", HAM_RET_VOID, 1, {HAM_PARAM_FLOAT}},                                               // 149
    {"moveexecute", HAM_RET_VOID, 3, {HAM_PARAM_ENTITY, HAM_PARAM_VECTOR, HAM_PARAM_FLOAT}},    // 150
    {"shouldadvanceroute", HAM_RET_INT, 1, {HAM_PARAM_FLOAT}},                                  // 151
    {"getstoppedactivity", HAM_RET_INT, 0, {}},                                                 // 152
    {"stop", HAM_RET_VOID, 0, {}},                                                              // 153
    {"checkrangeattack1", HAM_RET_INT, 2, {HAM_PARAM_FLOAT, HAM_PARAM_FLOAT}},                  // 154
    {"checkrangeattack2", HAM_RET_INT, 2, {HAM_PARAM_FLOAT, HAM_PARAM_FLOAT}},                  // 155
    {"checkmeleeattack1", HAM_RET_INT, 2, {HAM_PARAM_FLOAT, HAM_PARAM_FLOAT}},                  // 156
    {"checkmeleeattack2", HAM_RET_INT, 2, {HAM_PARAM_FLOAT, HAM_PARAM_FLOAT}},                  // 157
    {"schedulechange", HAM_RET_VOID, 0, {}},                                                    // 158
    {"canplaysequence", HAM_RET_INT, 2, {HAM_PARAM_INT, HAM_PARAM_INT}},                        // 159
    {"canplaysentence2", HAM_RET_INT, 1, {HAM_PARAM_INT}},                                      // 160
    {"playsentence", HAM_RET_VOID, 4, {HAM_PARAM_STRING, HAM_PARAM_FLOAT, HAM_PARAM_FLOAT, HAM_PARAM_FLOAT}}, // 161
    {"playscriptedsentence", HAM_RET_VOID, 6, {HAM_PARAM_STRING, HAM_PARAM_FLOAT, HAM_PARAM_FLOAT, HAM_PARAM_FLOAT, HAM_PARAM_INT, HAM_PARAM_ENTITY}}, // 162
    {"sentencestop", HAM_RET_VOID, 0, {}},                                                      // 163
    {"getidealstate", HAM_RET_INT, 0, {}},                                                      // 164
    {"setactivity", HAM_RET_VOID, 1, {HAM_PARAM_INT}},                                          // 165
    {"checkenemy", HAM_RET_INT, 1, {HAM_PARAM_ENTITY}},                                         // 166
    {"ftriangulate", HAM_RET_INT, 5, {HAM_PARAM_VECTOR, HAM_PARAM_VECTOR, HAM_PARAM_FLOAT, HAM_PARAM_ENTITY, HAM_PARAM_VECTOR}}, // 167
    {"setyawspeed", HAM_RET_VOID, 0, {}},                                                       // 168
    {"buildnearestroute", HAM_RET_INT, 4, {HAM_PARAM_VECTOR, HAM_PARAM_VECTOR, HAM_PARAM_FLOAT, HAM_PARAM_FLOAT}}, // 169
    {"findcover", HAM_RET_INT, 4, {HAM_PARAM_VECTOR, HAM_PARAM_VECTOR, HAM_PARAM_FLOAT, HAM_PARAM_FLOAT}}, // 170
    {"coverradius", HAM_RET_FLOAT, 0, {}},                                                      // 171
    {"fcancheckattacks", HAM_RET_INT, 0, {}},                                                   // 172
    {"checkammo", HAM_RET_VOID, 0, {}},                                                         // 173
    {"ignoreconditions", HAM_RET_INT, 0, {}},                                                   // 174
    {"fvalidatehinttype", HAM_RET_INT, 1, {HAM_PARAM_INT}},                                     // 175
    {"fcanactiveidle", HAM_RET_INT, 0, {}},                                                     // 176
    {"isoundmask", HAM_RET_INT, 0, {}},                                                         // 177
    {"hearingsensitivity", HAM_RET_FLOAT, 0, {}},                                               // 178
    {"barnaclevictimbitten", HAM_RET_VOID, 1, {HAM_PARAM_ENTVAR}},                              // 179
    {"barnaclevictimreleased", HAM_RET_VOID, 0, {}},                                            // 180
    {"preschedulethink", HAM_RET_VOID, 0, {}},                                                  // 181
    {"deathsound", HAM_RET_VOID, 0, {}},                                                        // 182
    {"alertsound", HAM_RET_VOID, 0, {}},                                                        // 183
    {"idlesound", HAM_RET_VOID, 0, {}},                                                         // 184
    {"stopfollowing", HAM_RET_VOID, 1, {HAM_PARAM_INT}},                                        // 185

    // Counter-Strike specific (186-191)
    {"cstrike_weapon_sendweaponanim", HAM_RET_VOID, 2, {HAM_PARAM_INT, HAM_PARAM_INT}},         // 186
    {"cstrike_player_resetmaxspeed", HAM_RET_VOID, 0, {}},                                      // 187
    {"cstrike_player_isbot", HAM_RET_INT, 0, {}},                                               // 188
    {"cstrike_player_getautoaimvector", HAM_RET_VECTOR, 1, {HAM_PARAM_FLOAT}},                  // 189
    {"cstrike_player_blind", HAM_RET_VOID, 4, {HAM_PARAM_FLOAT, HAM_PARAM_FLOAT, HAM_PARAM_FLOAT, HAM_PARAM_INT}}, // 190
    {"cstrike_player_ontouchingweapon", HAM_RET_VOID, 1, {HAM_PARAM_ENTITY}},                   // 191

    // Day of Defeat specific (192-206)
    {"dod_setscriptreset", HAM_RET_VOID, 0, {}},                                                // 192
    {"dod_item_spawndeploy", HAM_RET_INT, 0, {}},                                               // 193
    {"dod_item_setdmgtime", HAM_RET_VOID, 1, {HAM_PARAM_FLOAT}},                                // 194
    {"dod_item_dropgren", HAM_RET_VOID, 0, {}},                                                 // 195
    {"dod_weapon_isuseable", HAM_RET_INT, 0, {}},                                               // 196
    {"dod_weapon_aim", HAM_RET_VECTOR, 3, {HAM_PARAM_FLOAT, HAM_PARAM_ENTITY, HAM_PARAM_INT}},  // 197
    {"dod_weapon_flaim", HAM_RET_FLOAT, 2, {HAM_PARAM_FLOAT, HAM_PARAM_ENTITY}},                // 198
    {"dod_weapon_removestamina", HAM_RET_VOID, 2, {HAM_PARAM_FLOAT, HAM_PARAM_ENTITY}},         // 199
    {"dod_weapon_changefov", HAM_RET_INT, 1, {HAM_PARAM_INT}},                                  // 200
    {"dod_weapon_zoomout", HAM_RET_INT, 0, {}},                                                 // 201
    {"dod_weapon_zoomin", HAM_RET_INT, 0, {}},                                                  // 202
    {"dod_weapon_getfov", HAM_RET_INT, 0, {}},                                                  // 203
    {"dod_weapon_playeriswatersniping", HAM_RET_INT, 0, {}},                                    // 204
    {"dod_weapon_updatezoomspeed", HAM_RET_VOID, 0, {}},                                        // 205
    {"dod_weapon_special", HAM_RET_VOID, 0, {}},                                                // 206

    // TFC specific (207-209)
    {"tfc_dbgetitemname", HAM_RET_STRING, 0, {}},                                               // 207
    {"tfc_radiusdamage", HAM_RET_VOID, 5, {HAM_PARAM_ENTVAR, HAM_PARAM_ENTVAR, HAM_PARAM_FLOAT, HAM_PARAM_INT, HAM_PARAM_INT}}, // 208
    {"tfc_radiusdamage2", HAM_RET_VOID, 6, {HAM_PARAM_VECTOR, HAM_PARAM_ENTVAR, HAM_PARAM_ENTVAR, HAM_PARAM_FLOAT, HAM_PARAM_INT, HAM_PARAM_INT}}, // 209

    // ESF specific (210-279) - abbreviated, all need proper signatures
    {"esf_isfighter", HAM_RET_INT, 0, {}},                                                      // 210
    {"esf_isbuddy", HAM_RET_INT, 0, {}},                                                        // 211
    {"esf_emitsound", HAM_RET_VOID, 2, {HAM_PARAM_STRING, HAM_PARAM_INT}},                      // 212
    {"esf_emitnullsound", HAM_RET_VOID, 1, {HAM_PARAM_INT}},                                    // 213
    {"esf_increasestrength", HAM_RET_VOID, 2, {HAM_PARAM_ENTITY, HAM_PARAM_INT}},               // 214
    {"esf_increasepl", HAM_RET_VOID, 1, {HAM_PARAM_INT}},                                       // 215
    {"esf_setpowerlevel", HAM_RET_VOID, 1, {HAM_PARAM_INT}},                                    // 216
    {"esf_setmaxpowerlevel", HAM_RET_VOID, 1, {HAM_PARAM_INT}},                                 // 217
    {"esf_stopanitrigger", HAM_RET_VOID, 1, {HAM_PARAM_INT}},                                   // 218
    {"esf_stopfly", HAM_RET_VOID, 0, {}},                                                       // 219
    {"esf_hideweapon", HAM_RET_VOID, 0, {}},                                                    // 220
    {"esf_clientremoveweapon", HAM_RET_VOID, 1, {HAM_PARAM_INT}},                               // 221
    {"esf_sendclientcustommodel", HAM_RET_VOID, 1, {HAM_PARAM_STRING}},                         // 222
    {"esf_canturbo", HAM_RET_INT, 0, {}},                                                       // 223
    {"esf_canprimaryfire", HAM_RET_INT, 0, {}},                                                 // 224
    {"esf_cansecondaryfire", HAM_RET_INT, 0, {}},                                               // 225
    {"esf_canstopfly", HAM_RET_INT, 0, {}},                                                     // 226
    {"esf_canblock", HAM_RET_INT, 0, {}},                                                       // 227
    {"esf_canraiseKi", HAM_RET_INT, 0, {}},                                                     // 228
    {"esf_canraisestamina", HAM_RET_INT, 0, {}},                                                // 229
    {"esf_canteleport", HAM_RET_INT, 0, {}},                                                    // 230
    {"esf_canstartfly", HAM_RET_INT, 0, {}},                                                    // 231
    {"esf_canstartpowerup", HAM_RET_INT, 0, {}},                                                // 232
    {"esf_canjump", HAM_RET_INT, 0, {}},                                                        // 233
    {"esf_canwalljump", HAM_RET_INT, 0, {}},                                                    // 234
    {"esf_issuperjump", HAM_RET_INT, 0, {}},                                                    // 235
    {"esf_ismoveback", HAM_RET_INT, 0, {}},                                                     // 236
    {"esf_checkwalljump", HAM_RET_INT, 0, {}},                                                  // 237
    {"esf_enablewalljump", HAM_RET_VOID, 1, {HAM_PARAM_VECTOR}},                                // 238
    {"esf_disablewalljump", HAM_RET_VOID, 0, {}},                                               // 239
    {"esf_resetwalljumpvars", HAM_RET_VOID, 0, {}},                                             // 240
    {"esf_getwalljumpanim", HAM_RET_INT, 3, {HAM_PARAM_STRING, HAM_PARAM_VECTOR, HAM_PARAM_STRING}}, // 241
    {"esf_getwalljumpanim2", HAM_RET_INT, 2, {HAM_PARAM_STRING, HAM_PARAM_STRING}},             // 242
    {"esf_setwalljumpanimation", HAM_RET_VOID, 0, {}},                                          // 243
    {"esf_setflymovetype", HAM_RET_VOID, 0, {}},                                                // 244
    {"esf_isflymovetype", HAM_RET_INT, 0, {}},                                                  // 245
    {"esf_iswalkmovetype", HAM_RET_INT, 0, {}},                                                 // 246
    {"esf_setwalkmovetype", HAM_RET_VOID, 0, {}},                                               // 247
    {"esf_drawchargebar", HAM_RET_VOID, 1, {HAM_PARAM_INT}},                                    // 248
    {"esf_startblock", HAM_RET_VOID, 0, {}},                                                    // 249
    {"esf_stopblock", HAM_RET_VOID, 0, {}},                                                     // 250
    {"esf_startfly", HAM_RET_VOID, 0, {}},                                                      // 251
    {"esf_getmaxspeed", HAM_RET_FLOAT, 0, {}},                                                  // 252
    {"esf_setanimation", HAM_RET_VOID, 1, {HAM_PARAM_INT}},                                     // 253
    {"esf_playanimation", HAM_RET_VOID, 0, {}},                                                 // 254
    {"esf_getmoveforward", HAM_RET_INT, 0, {}},                                                 // 255
    {"esf_getmoveright", HAM_RET_INT, 0, {}},                                                   // 256
    {"esf_getmoveup", HAM_RET_VOID, 0, {}},                                                     // 257
    {"esf_addblindfx", HAM_RET_VOID, 0, {}},                                                    // 258
    {"esf_removeblindfx", HAM_RET_VOID, 0, {}},                                                 // 259
    {"esf_disablepsbar", HAM_RET_VOID, 0, {}},                                                  // 260
    {"esf_addbeamboxcrosshair", HAM_RET_VOID, 1, {HAM_PARAM_INT}},                              // 261
    {"esf_removebeamboxcrosshair", HAM_RET_VOID, 0, {}},                                        // 262
    {"esf_drawpswinbonus", HAM_RET_VOID, 0, {}},                                                // 263
    {"esf_drawpsbar", HAM_RET_VOID, 2, {HAM_PARAM_FLOAT, HAM_PARAM_FLOAT}},                     // 264
    {"esf_lockcrosshair", HAM_RET_VOID, 0, {}},                                                 // 265
    {"esf_unlockcrosshair", HAM_RET_VOID, 0, {}},                                               // 266
    {"esf_rotatecrosshair", HAM_RET_VOID, 0, {}},                                               // 267
    {"esf_unrotatecrosshair", HAM_RET_VOID, 0, {}},                                             // 268
    {"esf_watermove", HAM_RET_VOID, 0, {}},                                                     // 269
    {"esf_checktimebaseddamage", HAM_RET_VOID, 0, {}},                                          // 270
    {"esf_doessecondaryattack", HAM_RET_INT, 0, {}},                                            // 271
    {"esf_doesprimaryattack", HAM_RET_INT, 0, {}},                                              // 272
    {"esf_removespecialmodes", HAM_RET_VOID, 0, {}},                                            // 273
    {"esf_stopturbo", HAM_RET_VOID, 0, {}},                                                     // 274
    {"esf_takebean", HAM_RET_VOID, 0, {}},                                                      // 275
    {"esf_getpowerlevel", HAM_RET_VOID, 0, {}},                                                 // 276
    {"esf_removeallotherweapons", HAM_RET_VOID, 0, {}},                                         // 277
    {"esf_stopswoop", HAM_RET_VOID, 0, {}},                                                     // 278
    {"esf_setdeathanimation", HAM_RET_VOID, 0, {}},                                             // 279
    {"esf_setmodel", HAM_RET_VOID, 0, {}},                                                      // 280
    {"esf_addattacks", HAM_RET_VOID, 0, {}},                                                    // 281
    {"esf_emitclasssound", HAM_RET_VOID, 3, {HAM_PARAM_STRING, HAM_PARAM_STRING, HAM_PARAM_INT}}, // 282
    {"esf_checklightning", HAM_RET_VOID, 0, {}},                                                // 283
    {"esf_freezecontrols", HAM_RET_VOID, 0, {}},                                                // 284
    {"esf_unfreezecontrols", HAM_RET_VOID, 0, {}},                                              // 285
    {"esf_updateki", HAM_RET_VOID, 0, {}},                                                      // 286
    {"esf_updatehealth", HAM_RET_VOID, 0, {}},                                                  // 287
    {"esf_getteleportdir", HAM_RET_VECTOR, 0, {}},                                              // 288
    {"esf_weapon_holsterwhenmeleed", HAM_RET_VOID, 0, {}},                                      // 289

    // NS specific (290-316)
    {"ns_setbonecontroller", HAM_RET_FLOAT, 2, {HAM_PARAM_INT, HAM_PARAM_FLOAT}},               // 290
    {"ns_savedataforreset", HAM_RET_VOID, 0, {}},                                               // 291
    {"ns_gethull", HAM_RET_INT, 0, {}},                                                         // 292
    {"ns_getmaxwalkspeed", HAM_RET_FLOAT, 0, {}},                                               // 293
    {"ns_setteamid", HAM_RET_STRING, 1, {HAM_PARAM_STRING}},                                    // 294
    {"ns_geteffectiveplayerclass", HAM_RET_INT, 0, {}},                                         // 295
    {"ns_getauthenticationmask", HAM_RET_INT, 0, {}},                                           // 296
    {"ns_effectiveplayerclasschanged", HAM_RET_VOID, 0, {}},                                    // 297
    {"ns_needsteamupdate", HAM_RET_VOID, 0, {}},                                                // 298
    {"ns_sendteamupdate", HAM_RET_VOID, 0, {}},                                                 // 299
    {"ns_sendweaponupdate", HAM_RET_VOID, 0, {}},                                               // 300
    {"ns_initplayerfromspawn", HAM_RET_VOID, 1, {HAM_PARAM_EDICT}},                             // 301
    {"ns_packdeadplayeritems", HAM_RET_VOID, 0, {}},                                            // 302
    {"ns_getanimationforactivity", HAM_RET_VOID, 3, {HAM_PARAM_INT, HAM_PARAM_STRING, HAM_PARAM_INT}}, // 303
    {"ns_startobserver", HAM_RET_VOID, 2, {HAM_PARAM_VECTOR, HAM_PARAM_VECTOR}},                // 304
    {"ns_stopobserver", HAM_RET_VOID, 0, {}},                                                   // 305
    {"ns_getadrenalinefactor", HAM_RET_FLOAT, 0, {}},                                           // 306
    {"ns_givenameditem", HAM_RET_VOID, 2, {HAM_PARAM_STRING, HAM_PARAM_INT}},                   // 307
    {"ns_suicide", HAM_RET_VOID, 0, {}},                                                        // 308
    {"ns_getcanuseweapon", HAM_RET_INT, 0, {}},                                                 // 309
    {"ns_weapon_getweaponprimetime", HAM_RET_FLOAT, 0, {}},                                     // 310
    {"ns_weapon_primeweapon", HAM_RET_VOID, 0, {}},                                             // 311
    {"ns_weapon_getisweaponprimed", HAM_RET_INT, 0, {}},                                        // 312
    {"ns_weapon_getisweaponpriming", HAM_RET_INT, 0, {}},                                       // 313
    {"ns_weapon_defaultdeploy", HAM_RET_INT, 6, {HAM_PARAM_STRING, HAM_PARAM_STRING, HAM_PARAM_INT, HAM_PARAM_STRING, HAM_PARAM_INT, HAM_PARAM_INT}}, // 314
    {"ns_weapon_defaultreload", HAM_RET_INT, 4, {HAM_PARAM_INT, HAM_PARAM_INT, HAM_PARAM_FLOAT, HAM_PARAM_INT}}, // 315
    {"ns_weapon_getdeploytime", HAM_RET_FLOAT, 0, {}},                                          // 316

    // Sven Co-op specific (317-396) - placeholder entries
    {"sc_getclassification", HAM_RET_INT, 1, {HAM_PARAM_INT}},                                  // 317
    {"sc_ismonster", HAM_RET_INT, 0, {}},                                                       // 318
    {"sc_isphysx", HAM_RET_VOID, 0, {}},                                                        // 319 (removed)
    {"sc_ispointentity", HAM_RET_INT, 0, {}},                                                   // 320
    {"sc_ismachine", HAM_RET_INT, 0, {}},                                                       // 321
    {"sc_criticalremove", HAM_RET_INT, 0, {}},                                                  // 322
    {"sc_updateonremove", HAM_RET_VOID, 0, {}},                                                 // 323
    {"sc_fvisible", HAM_RET_INT, 2, {HAM_PARAM_ENTITY, HAM_PARAM_INT}},                         // 324
    {"sc_fvisiblefrompos", HAM_RET_INT, 2, {HAM_PARAM_VECTOR, HAM_PARAM_VECTOR}},               // 325
    {"sc_isfacing", HAM_RET_INT, 2, {HAM_PARAM_ENTVAR, HAM_PARAM_FLOAT}},                       // 326
    {"sc_getpointsfordamage", HAM_RET_FLOAT, 1, {HAM_PARAM_FLOAT}},                             // 327
    {"sc_getdamagepoints", HAM_RET_VOID, 3, {HAM_PARAM_ENTVAR, HAM_PARAM_ENTVAR, HAM_PARAM_FLOAT}}, // 328
    {"sc_oncreate", HAM_RET_VOID, 0, {}},                                                       // 329
    {"sc_ondestroy", HAM_RET_VOID, 0, {}},                                                      // 330
    {"sc_isvalidentity", HAM_RET_VOID, 0, {}},                                                  // 331 (removed)
    {"sc_shouldfadeondeath", HAM_RET_INT, 0, {}},                                               // 332
    {"sc_setupfriendly", HAM_RET_VOID, 0, {}},                                                  // 333
    {"sc_revivethink", HAM_RET_VOID, 0, {}},                                                    // 334 (removed)
    {"sc_revive", HAM_RET_VOID, 0, {}},                                                         // 335
    {"sc_startmonster", HAM_RET_VOID, 0, {}},                                                   // 336
    {"sc_checkrangeattack1_move", HAM_RET_INT, 2, {HAM_PARAM_FLOAT, HAM_PARAM_FLOAT}},          // 337
    {"sc_checkrangeattack2_move", HAM_RET_INT, 2, {HAM_PARAM_FLOAT, HAM_PARAM_FLOAT}},          // 338
    {"sc_checkmeleeattack1_move", HAM_RET_INT, 2, {HAM_PARAM_FLOAT, HAM_PARAM_FLOAT}},          // 339
    {"sc_checkmeleeattack2_move", HAM_RET_INT, 2, {HAM_PARAM_FLOAT, HAM_PARAM_FLOAT}},          // 340
    {"sc_checktankusage", HAM_RET_INT, 0, {}},                                                  // 341
    {"sc_setgaitactivity", HAM_RET_INT, 0, {}},                                                 // 342
    {"sc_ftriangulate", HAM_RET_INT, 7, {HAM_PARAM_VECTOR, HAM_PARAM_VECTOR, HAM_PARAM_FLOAT, HAM_PARAM_ENTITY, HAM_PARAM_VECTOR, HAM_PARAM_VECTOR, HAM_PARAM_INT}}, // 343
    {"sc_ftriangulateextension", HAM_RET_INT, 5, {HAM_PARAM_VECTOR, HAM_PARAM_VECTOR, HAM_PARAM_FLOAT, HAM_PARAM_ENTITY, HAM_PARAM_VECTOR}}, // 344
    {"sc_findcovergrenade", HAM_RET_INT, 4, {HAM_PARAM_VECTOR, HAM_PARAM_VECTOR, HAM_PARAM_FLOAT, HAM_PARAM_FLOAT}}, // 345
    {"sc_findcoverdistance", HAM_RET_INT, 4, {HAM_PARAM_VECTOR, HAM_PARAM_VECTOR, HAM_PARAM_FLOAT, HAM_PARAM_FLOAT}}, // 346
    {"sc_findattackpoint", HAM_RET_INT, 4, {HAM_PARAM_VECTOR, HAM_PARAM_VECTOR, HAM_PARAM_FLOAT, HAM_PARAM_FLOAT}}, // 347
    {"sc_fvalidatecover", HAM_RET_INT, 1, {HAM_PARAM_VECTOR}},                                  // 348
    {"sc_nofriendlyfire1", HAM_RET_INT, 0, {}},                                                 // 349
    {"sc_nofriendlyfire2", HAM_RET_INT, 1, {HAM_PARAM_VECTOR}},                                 // 350
    {"sc_nofriendlyfire3", HAM_RET_INT, 2, {HAM_PARAM_VECTOR, HAM_PARAM_ENTITY}},               // 351
    {"sc_nofriendlyfiretopos", HAM_RET_INT, 1, {HAM_PARAM_VECTOR}},                             // 352
    {"sc_fvisiblegunpos", HAM_RET_INT, 2, {HAM_PARAM_ENTITY, HAM_PARAM_VECTOR}},                // 353
    {"sc_finbulletcone", HAM_RET_INT, 2, {HAM_PARAM_ENTITY, HAM_PARAM_VECTOR}},                 // 354
    {"sc_callgibmonster", HAM_RET_VOID, 0, {}},                                                 // 355
    {"sc_checktimebaseddamage", HAM_RET_VOID, 0, {}},                                           // 356
    {"sc_ismoving", HAM_RET_INT, 0, {}},                                                        // 357
    {"sc_isplayerfollowing", HAM_RET_INT, 0, {}},                                               // 358
    {"sc_startplayerfollowing", HAM_RET_VOID, 2, {HAM_PARAM_ENTITY, HAM_PARAM_INT}},            // 359
    {"sc_stopplayerfollowing", HAM_RET_VOID, 2, {HAM_PARAM_INT, HAM_PARAM_INT}},                // 360
    {"sc_usesound", HAM_RET_VOID, 0, {}},                                                       // 361
    {"sc_unusesound", HAM_RET_VOID, 0, {}},                                                     // 362
    {"sc_ridemonster", HAM_RET_VOID, 1, {HAM_PARAM_ENTITY}},                                    // 363
    {"sc_checkandapplygenericattacks", HAM_RET_VOID, 0, {}},                                    // 364
    {"sc_checkscared", HAM_RET_INT, 0, {}},                                                     // 365
    {"sc_checkcreaturedanger", HAM_RET_VOID, 0, {}},                                            // 366
    {"sc_checkfalldamage", HAM_RET_VOID, 0, {}},                                                // 367
    {"sc_checkrevival", HAM_RET_VOID, 0, {}},                                                   // 368
    {"sc_mediccallsound", HAM_RET_VOID, 0, {}},                                                 // 369
    {"sc_player_menuinputperformed", HAM_RET_VOID, 0, {}},                                      // 370 (removed)
    {"sc_player_ismenuinputdone", HAM_RET_VOID, 0, {}},                                         // 371 (removed)
    {"sc_player_specialspawn", HAM_RET_VOID, 0, {}},                                            // 372
    {"sc_player_isvalidinfoentity", HAM_RET_INT, 0, {}},                                        // 373
    {"sc_player_levelend", HAM_RET_VOID, 0, {}},                                                // 374
    {"sc_player_votestarted", HAM_RET_VOID, 1, {HAM_PARAM_INT}},                                // 375
    {"sc_player_canstartnextvote", HAM_RET_INT, 1, {HAM_PARAM_INT}},                            // 376
    {"sc_player_vote", HAM_RET_VOID, 1, {HAM_PARAM_INT}},                                       // 377
    {"sc_player_hasvoted", HAM_RET_INT, 0, {}},                                                 // 378
    {"sc_player_resetvote", HAM_RET_VOID, 0, {}},                                               // 379
    {"sc_player_lastvoteinput", HAM_RET_INT, 0, {}},                                            // 380
    {"sc_player_initvote", HAM_RET_VOID, 0, {}},                                                // 381
    {"sc_player_timetostartnextvote", HAM_RET_FLOAT, 0, {}},                                    // 382
    {"sc_player_resetview", HAM_RET_VOID, 0, {}},                                               // 383
    {"sc_player_getlogfrequency", HAM_RET_FLOAT, 0, {}},                                        // 384
    {"sc_player_logplayerstats", HAM_RET_INT, 0, {}},                                           // 385
    {"sc_player_disablecollisionwithplayer", HAM_RET_VOID, 0, {}},                              // 386 (removed)
    {"sc_player_enablecollisionwithplayer", HAM_RET_VOID, 0, {}},                               // 387 (removed)
    {"sc_player_cantouchplayer", HAM_RET_VOID, 0, {}},                                          // 388 (removed)
    {"sc_item_materialize", HAM_RET_VOID, 0, {}},                                               // 389
    {"sc_weapon_bulletaccuracy", HAM_RET_VECTOR, 4, {HAM_PARAM_VECTOR, HAM_PARAM_VECTOR, HAM_PARAM_VECTOR, HAM_PARAM_VECTOR}}, // 390
    {"sc_weapon_tertiaryattack", HAM_RET_VOID, 0, {}},                                          // 391
    {"sc_weapon_burstsupplement", HAM_RET_VOID, 0, {}},                                         // 392
    {"sc_weapon_getp_model", HAM_RET_STRING, 1, {HAM_PARAM_STRING}},                            // 393
    {"sc_weapon_getw_model", HAM_RET_STRING, 1, {HAM_PARAM_STRING}},                            // 394
    {"sc_weapon_getv_model", HAM_RET_STRING, 1, {HAM_PARAM_STRING}},                            // 395
    {"sc_weapon_precachecustommodels", HAM_RET_VOID, 0, {}},                                    // 396

    // Continue with remaining SC entries and other mods...
    // Additional entries (397+) - adding key ones
    {"sc_weapon_ismultiplayer", HAM_RET_INT, 0, {}},                                            // 397
    {"sc_weapon_frunfuncs", HAM_RET_INT, 0, {}},                                                // 398
    {"sc_weapon_setfov", HAM_RET_VOID, 1, {HAM_PARAM_INT}},                                     // 399
    {"sc_weapon_fcanrun", HAM_RET_INT, 0, {}},                                                  // 400
    {"sc_weapon_customdecrement", HAM_RET_VOID, 1, {HAM_PARAM_FLOAT}},                          // 401
    {"sc_weapon_setv_model", HAM_RET_VOID, 1, {HAM_PARAM_STRING}},                              // 402
    {"sc_weapon_setp_model", HAM_RET_VOID, 1, {HAM_PARAM_STRING}},                              // 403
    {"sc_weapon_changeweaponskin", HAM_RET_VOID, 1, {HAM_PARAM_INT}},                           // 404

    // TFC 2013 additions (405-408)
    {"tfc_killed", HAM_RET_VOID, 3, {HAM_PARAM_ENTVAR, HAM_PARAM_ENTVAR, HAM_PARAM_INT}},       // 405
    {"tfc_istriggered", HAM_RET_INT, 0, {}},                                                    // 406
    {"tfc_weapon_sendweaponanim", HAM_RET_VOID, 2, {HAM_PARAM_INT, HAM_PARAM_INT}},             // 407
    {"tfc_weapon_getnextattackdelay", HAM_RET_FLOAT, 1, {HAM_PARAM_FLOAT}},                     // 408

    // SC additions (409-413)
    {"sc_takehealth", HAM_RET_INT, 3, {HAM_PARAM_FLOAT, HAM_PARAM_INT, HAM_PARAM_INT}},         // 409
    {"sc_takearmor", HAM_RET_INT, 3, {HAM_PARAM_FLOAT, HAM_PARAM_INT, HAM_PARAM_INT}},          // 410
    {"sc_giveammo", HAM_RET_INT, 4, {HAM_PARAM_INT, HAM_PARAM_STRING, HAM_PARAM_INT, HAM_PARAM_INT}}, // 411
    {"sc_checkattacker", HAM_RET_INT, 1, {HAM_PARAM_ENTITY}},                                   // 412
    {"sc_player_isconnected", HAM_RET_INT, 0, {}},                                              // 413

    // DOD weapon (414)
    {"dod_weapon_sendweaponanim", HAM_RET_VOID, 2, {HAM_PARAM_INT, HAM_PARAM_INT}},             // 414

    // CS item (415)
    {"cstrike_item_isweapon", HAM_RET_INT, 0, {}},                                              // 415

    // Gearbox (416-417)
    {"gearbox_mysquadtalkmonsterpointer", HAM_RET_ENTITY, 0, {}},                               // 416
    {"gearbox_weapontimebase", HAM_RET_FLOAT, 0, {}},                                           // 417

    // TS weapon (418)
    {"ts_weapon_alternateattack", HAM_RET_VOID, 0, {}},                                         // 418

    // Item GetItemInfo (419)
    {"item_getiteminfo", HAM_RET_INT, 1, {HAM_PARAM_INT}},                                      // 419

    // Sven Co-op additional entries (420-480)
    {"sc_prespawn", HAM_RET_VOID, 0, {}},                                                       // 420
    {"sc_postspawn", HAM_RET_VOID, 0, {}},                                                      // 421
    {"sc_onkeyvalueupdate", HAM_RET_VOID, 1, {HAM_PARAM_STRING}},                               // 422
    {"sc_setclassification", HAM_RET_VOID, 1, {HAM_PARAM_INT}},                                 // 423
    {"sc_istriggered", HAM_RET_INT, 0, {}},                                                     // 424
    {"sc_mycustompointer", HAM_RET_ENTITY, 0, {}},                                              // 425
    {"sc_myitempointer", HAM_RET_ENTITY, 0, {}},                                                // 426
    {"sc_addpoints", HAM_RET_VOID, 2, {HAM_PARAM_INT, HAM_PARAM_INT}},                          // 427
    {"sc_addpointstoteam", HAM_RET_VOID, 2, {HAM_PARAM_INT, HAM_PARAM_INT}},                    // 428
    {"sc_removeplayeritem", HAM_RET_INT, 1, {HAM_PARAM_ENTITY}},                                // 429
    {"sc_oncontrols", HAM_RET_INT, 1, {HAM_PARAM_ENTVAR}},                                      // 430
    {"sc_issneaking", HAM_RET_INT, 0, {}},                                                      // 431
    {"sc_isalive", HAM_RET_INT, 0, {}},                                                         // 432
    {"sc_isbspmodel", HAM_RET_INT, 0, {}},                                                      // 433
    {"sc_reflectgauss", HAM_RET_INT, 0, {}},                                                    // 434
    {"sc_hastarget", HAM_RET_INT, 1, {HAM_PARAM_INT}},                                          // 435
    {"sc_isinworld", HAM_RET_INT, 0, {}},                                                       // 436
    {"sc_isplayer", HAM_RET_INT, 0, {}},                                                        // 437
    {"sc_isnetclient", HAM_RET_INT, 0, {}},                                                     // 438
    {"sc_isbreakable", HAM_RET_INT, 0, {}},                                                     // 439
    {"sc_subusetargets", HAM_RET_VOID, 3, {HAM_PARAM_ENTITY, HAM_PARAM_INT, HAM_PARAM_FLOAT}},  // 440
    {"sc_islockedbymaster", HAM_RET_INT, 0, {}},                                                // 441
    {"sc_fbecomeprone", HAM_RET_INT, 1, {HAM_PARAM_ENTITY}},                                    // 442
    {"sc_fvecvisible", HAM_RET_INT, 1, {HAM_PARAM_VECTOR}},                                     // 443
    {"sc_setplayerally", HAM_RET_VOID, 1, {HAM_PARAM_INT}},                                     // 444
    {"sc_onsetoriginbymap", HAM_RET_VOID, 0, {}},                                               // 445
    {"sc_isrevivable", HAM_RET_INT, 0, {}},                                                     // 446
    {"sc_beginrevive", HAM_RET_VOID, 1, {HAM_PARAM_FLOAT}},                                     // 447
    {"sc_endrevive", HAM_RET_VOID, 1, {HAM_PARAM_FLOAT}},                                       // 448
    {"sc_canplaysequence", HAM_RET_INT, 2, {HAM_PARAM_INT, HAM_PARAM_INT}},                     // 449
    {"sc_canplaysentence2", HAM_RET_INT, 1, {HAM_PARAM_INT}},                                   // 450
    {"sc_playscriptedsentence", HAM_RET_VOID, 6, {HAM_PARAM_STRING, HAM_PARAM_FLOAT, HAM_PARAM_FLOAT, HAM_PARAM_FLOAT, HAM_PARAM_INT, HAM_PARAM_ENTITY}}, // 451
    {"sc_item_addtoplayer", HAM_RET_INT, 1, {HAM_PARAM_ENTITY}},                                // 452
    {"sc_item_addduplicate", HAM_RET_INT, 1, {HAM_PARAM_ENTITY}},                               // 453
    {"sc_item_addammofromitem", HAM_RET_INT, 1, {HAM_PARAM_ENTITY}},                            // 454
    {"sc_item_getpickupsound", HAM_RET_STRING, 0, {}},                                          // 455
    {"sc_item_cancollect", HAM_RET_INT, 2, {HAM_PARAM_ENTITY, HAM_PARAM_INT}},                  // 456
    {"sc_item_collect", HAM_RET_VOID, 2, {HAM_PARAM_ENTITY, HAM_PARAM_INT}},                    // 457
    {"sc_item_getiteminfo", HAM_RET_INT, 1, {HAM_PARAM_INT}},                                   // 458
    {"sc_item_candeploy", HAM_RET_INT, 0, {}},                                                  // 459
    {"sc_item_deploy", HAM_RET_INT, 0, {}},                                                     // 460
    {"sc_item_canholster", HAM_RET_INT, 0, {}},                                                 // 461
    {"sc_item_inactiveitempreframe", HAM_RET_VOID, 0, {}},                                      // 462
    {"sc_item_inactiveitempostframe", HAM_RET_VOID, 0, {}},                                     // 463
    {"sc_item_detachfromplayer", HAM_RET_VOID, 0, {}},                                          // 464
    {"sc_item_updateclientdata", HAM_RET_INT, 1, {HAM_PARAM_ENTITY}},                           // 465
    {"sc_item_getrespawntime", HAM_RET_FLOAT, 0, {}},                                           // 466
    {"sc_item_canhaveduplicates", HAM_RET_INT, 0, {}},                                          // 467
    {"sc_weapon_extractammofromitem", HAM_RET_INT, 1, {HAM_PARAM_ENTITY}},                      // 468
    {"sc_weapon_addweapon", HAM_RET_INT, 0, {}},                                                // 469
    {"sc_weapon_getammo1drop", HAM_RET_INT, 0, {}},                                             // 470
    {"sc_weapon_getammo2drop", HAM_RET_INT, 0, {}},                                             // 471
    {"sc_weapon_playemptysound", HAM_RET_INT, 0, {}},                                           // 472
    {"sc_weapon_isusable", HAM_RET_INT, 0, {}},                                                 // 473
    {"sc_weapon_finishreload", HAM_RET_VOID, 0, {}},                                            // 474
    {"sc_weapon_shouldreload", HAM_RET_INT, 0, {}},                                             // 475
    {"sc_weapon_shouldweaponidle", HAM_RET_INT, 0, {}},                                         // 476
    {"sc_weapon_usedecrement", HAM_RET_INT, 0, {}},                                             // 477
    {"sc_player_enteredobserver", HAM_RET_VOID, 0, {}},                                         // 478
    {"sc_player_leftobserver", HAM_RET_VOID, 0, {}},                                            // 479
    {"sc_player_isobserver", HAM_RET_INT, 0, {}},                                               // 480

    {nullptr, HAM_RET_VOID, 0, {}}
};

HamManager& HamManager::instance() {
    static HamManager instance;
    return instance;
}

HamManager::~HamManager() {
    // If shutdown was already called, hooks are already cleared
    // This prevents crashes when static destructor runs after game DLL unloads
    if (!m_isShutdown && !m_hooks.empty()) {
        // Mark all hooks to skip vtable restoration - game DLL may be unloaded
        for (auto& pair : m_hooks) {
            pair.second->skipVTableRestore();
        }
        m_hookIdMap.clear();
        m_hooks.clear();
    }
}

bool HamManager::initialize(const std::string& gameDataPath) {
    if (!m_gameData.loadFromFile(gameDataPath)) {
        return false;
    }
    m_baseOffset = m_gameData.getBaseOffset();

    return true;
}

void HamManager::shutdown() {
    m_hooks.clear();  // This calls Hook destructors which restore vtables and free trampolines
    m_hookIdMap.clear();
    m_returnValue.Reset();
    m_origReturnValue.Reset();
    m_context.Reset();
    m_isShutdown = true;  // Mark as shutdown to prevent destructor from trying to restore vtables
}

int HamManager::getVTableOffset(HamType function) const {
    if (function < 0 || function >= Ham_EndMarker) {
        return -1;
    }

    const char* funcName = g_hamFunctions[function].name;
    if (!funcName) {
        return -1;
    }

    auto offset = m_gameData.getOffset(funcName);
    return offset.getCurrent();
}

const HamFunctionInfo* HamManager::getFunctionInfo(HamType function) const {
    if (function < 0 || function >= Ham_EndMarker) {
        return nullptr;
    }
    return &g_hamFunctions[function];
}

void** HamManager::getEntityVTable(edict_t* ent) const {
    if (!ent || !ent->pvPrivateData) {
        return nullptr;
    }

    // GetVTable: return *((void***)(((char*)pthis) + size))
    char* base = static_cast<char*>(ent->pvPrivateData);
    base += m_baseOffset;
    void** vtable = *reinterpret_cast<void***>(base);
    return vtable;
}

edict_t* HamManager::createEntityByClass(const char* className) {
    // Create a temporary entity
    edict_t* ent = g_engfuncs.pfnCreateEntity();
    if (!ent) {
        return nullptr;
    }

    // Call the game DLL's entity allocation function
    // This populates pvPrivateData with the entity's C++ object
    gpMetaUtilFuncs->pfnCallGameEntity(PLID, className, &ent->v);

    return ent;
}

void HamManager::removeEntity(edict_t* ent) {
    if (ent) {
        g_engfuncs.pfnRemoveEntity(ent);
    }
}

Hook* HamManager::findOrCreateHook(HamType function, const char* entityClass, edict_t* ent) {
    std::string key = std::string(entityClass) + ":" + std::to_string(function);

    auto it = m_hooks.find(key);
    if (it != m_hooks.end()) {
        return it->second.get();
    }

    void** vtable = getEntityVTable(ent);
    if (!vtable) {
        return nullptr;
    }

    int vtableIndex = getVTableOffset(function);
    if (vtableIndex < 0) {
        return nullptr;
    }

    const HamFunctionInfo* info = getFunctionInfo(function);
    if (!info) {
        return nullptr;
    }

    void* callback = getCallbackForFunction(function);
    if (!callback) {
        return nullptr;
    }

    auto hook = std::make_unique<Hook>(vtable, vtableIndex, callback, info->paramCount, entityClass);
    Hook* hookPtr = hook.get();
    m_hooks[key] = std::move(hook);

    return hookPtr;
}

int HamManager::registerHook(
    v8::Isolate* isolate,
    v8::Local<v8::Context> context,
    HamType function,
    const char* entityClass,
    v8::Local<v8::Function> callback,
    bool isPre
) {
    // Validate function
    if (function < 0 || function >= Ham_EndMarker) {
        return -1;
    }

    // Check if vtable offset is configured
    int vtableIndex = getVTableOffset(function);
    if (vtableIndex < 0) {
        return -1;
    }

    // Get callback function
    void* hookCallback = getCallbackForFunction(function);
    if (!hookCallback) {
        return -1;
    }

    // Create a temporary entity to get the vtable
    edict_t* tempEnt = createEntityByClass(entityClass);

    if (!tempEnt || !tempEnt->pvPrivateData) {
        if (tempEnt) {
            removeEntity(tempEnt);
        }
        return -1;
    }

    // Get the vtable from the entity
    void** vtable = getEntityVTable(tempEnt);

    // Remove the temporary entity
    removeEntity(tempEnt);

    if (!vtable) {
        return -1;
    }

    // Check if we already have a hook for this class/function combo
    std::string key = std::string(entityClass) + ":" + std::to_string(function);
    Hook* hook = nullptr;

    auto it = m_hooks.find(key);
    if (it != m_hooks.end()) {
        hook = it->second.get();
    } else {
        // Create a new hook
        const HamFunctionInfo* info = getFunctionInfo(function);
        if (!info) {
            return -1;
        }

        auto newHook = std::make_unique<Hook>(vtable, vtableIndex, hookCallback, info->paramCount, entityClass);
        hook = newHook.get();
        m_hooks[key] = std::move(newHook);
    }

    // Add the callback with context
    int hookId = hook->addCallback(isolate, context, callback, isPre);

    m_hookIdMap[hookId] = hook;
    return hookId;
}

void HamManager::unregisterHook(int hookId) {
    auto it = m_hookIdMap.find(hookId);
    if (it != m_hookIdMap.end()) {
        Hook* hook = it->second;
        hook->removeCallback(hookId);

        // Check if hook has no more callbacks
        if (hook->getPreCallbacks().empty() && hook->getPostCallbacks().empty()) {
            // Find and remove the hook
            for (auto hookIt = m_hooks.begin(); hookIt != m_hooks.end(); ++hookIt) {
                if (hookIt->second.get() == hook) {
                    m_hooks.erase(hookIt);
                    break;
                }
            }
        }

        m_hookIdMap.erase(it);
    }
}

void* HamManager::getOriginalFunction(HamType function, void** vtable) const {
    std::string key;
    for (const auto& pair : m_hooks) {
        if (pair.second->getVTable() == vtable &&
            pair.second->getEntry() == getVTableOffset(function)) {
            return pair.second->getOriginalFunction();
        }
    }
    return nullptr;
}

void HamManager::setReturnValue(v8::Isolate* isolate, v8::Local<v8::Value> value) {
    m_returnValue.Reset(isolate, value);
}

v8::Local<v8::Value> HamManager::getReturnValue(v8::Isolate* isolate) {
    if (m_returnValue.IsEmpty()) {
        return v8::Undefined(isolate);
    }
    return m_returnValue.Get(isolate);
}

void HamManager::setOrigReturnValue(v8::Isolate* isolate, v8::Local<v8::Value> value) {
    m_origReturnValue.Reset(isolate, value);
}

v8::Local<v8::Value> HamManager::getOrigReturnValue(v8::Isolate* isolate) {
    if (m_origReturnValue.IsEmpty()) {
        return v8::Undefined(isolate);
    }
    return m_origReturnValue.Get(isolate);
}

} // namespace Ham
