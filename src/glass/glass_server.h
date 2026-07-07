#pragma once
#include "glass.h"

struct dvar_s;
struct gentity_s;
struct moveclip_t;
struct pointtrace_t;
struct msg_t;
struct trace_t;

constexpr int MAX_GLASSES = 1000;
constexpr int GLASS_MIN_DAMAGE = 5;

struct GlassServer // sizeof=0x2C
{
    GlassState state;
    Glass *glass;
    int stateChangeTime;
    float health;
    float crackedDamageRate;
    float hitPos[3];
    float hitDir[3];

    // Decomp: CoDMPServer.c:859672 — transition state; pos/dir used when entering SHATTERED
    void __thiscall SetState(GlassState::State st, float *pos, float *dir);
};

struct GlassesServer // sizeof=0xABF8
{
    int nextShatterTime;
    int shatterIntervalsMS;
    bool inited;
    // padding byte
    // padding byte
    // padding byte
    unsigned int numGlasses;
    GlassServer glasses[MAX_GLASSES];
    const dvar_s *damageMultiplier;
    const dvar_s *crackedDamageRateRange;

    // Decomp: CoDMPServer.c:859705
    void __thiscall Init();
    // Decomp: CoDMPServer.c:17709 (proto; sets numGlasses = 0)
    void __thiscall Shutdown();
    // Decomp: CoDMPServer.c:859773
    void __thiscall Update();
    // Decomp: CoDMPServer.c:859858
    static void __cdecl ShatterAllCmd();
    // Decomp: CoDMPServer.c:859864
    void __thiscall ShatterAll();
    // Decomp: CoDMPServer.c:859918
    static void __cdecl ResetAllCmd();
    // Decomp: CoDMPServer.c:859924
    void __thiscall ResetAll();
    // Decomp: CoDMPServer.c:860455
    void __thiscall WriteSnapshotToClient(msg_t *msg, int sinceTime);
};

extern GlassesServer svGlasses;

// Decomp: CoDMPServer.c:859941
void __cdecl GlassSv_Init();
// Decomp: CoDMPServer.c:859947
void __cdecl GlassSv_Shutdown();
// Decomp: CoDMPServer.c:859953
void __cdecl GlassSv_Update();
// Decomp: CoDMPServer.c:859959
unsigned int __cdecl GlassSv_AreaGlasses(
    const float *mins,
    const float *maxs,
    const Glass **glasses,
    unsigned int maxGlasses);
// Decomp: CoDMPServer.c:859989
void __cdecl GlassSv_ClipMoveTrace(const moveclip_t *clip, trace_t *results);
// Decomp: CoDMPServer.c:860084
void __cdecl GlassSv_PointTrace(const pointtrace_t *clip, trace_t *results);
// Decomp: CoDMPServer.c:860143
void __cdecl GlassSv_Damage(unsigned int glassId, int damage, int mod, float *pos, float *dir);
// Decomp: CoDMPServer.c:860187
void __cdecl GlassSv_RadiusDamage(
    float *origin,
    float radius,
    float coneAngleCos,
    float *coneDirection,
    float innerDamage,
    float outerDamage,
    int mod);
// Decomp: CoDMPServer.c:860269
void __cdecl GlassSv_Touch(unsigned int glassId, gentity_s *other);
// Decomp: CoDMPServer.c:860311
void __cdecl GlassSv_PredictTouch(gentity_s *other);
// Decomp: CoDMPServer.c:860550
void __cdecl GlassSv_WriteSnapshotToClient(msg_t *msg, int sinceTime);
// Decomp: CoDMPServer.c:860556 — delegates to WriteSnapshotToClient(msg, 0)
void __cdecl GlassSv_WriteGameState(msg_t *msg);
