#pragma once

void __thiscall GlassServer::SetState(GlassServer *this, GlassState::State st, float *pos, float *dir);
void __thiscall GlassesServer::Init(GlassesServer *this);
void __thiscall GlassesServer::Shutdown(GlassesServer *this);
void __thiscall GlassesServer::Update(GlassesServer *this);
void __cdecl GlassesServer::ShatterAllCmd();
void __thiscall GlassesServer::ShatterAll(GlassesServer *this);
void __cdecl GlassesServer::ResetAllCmd();
void __thiscall GlassesServer::ResetAll(GlassesServer *this);
void __cdecl GlassSv_Init();
void __cdecl GlassSv_Shutdown();
void __cdecl GlassSv_Update();
unsigned int __cdecl GlassSv_AreaGlasses(
        const float *mins,
        const float *maxs,
        const Glass **glasses,
        unsigned int maxGlasses);
void __cdecl GlassSv_ClipMoveTrace(const moveclip_t *clip, trace_t *results);
void __cdecl GlassSv_PointTrace(const pointtrace_t *clip, trace_t *results);
void __cdecl GlassSv_Damage(unsigned int glassId, int damage, int mod, float *pos, float *dir);
void __cdecl GlassSv_RadiusDamage(
        float *origin,
        float radius,
        float coneAngleCos,
        float *coneDirection,
        float innerDamage,
        float outerDamage,
        int mod);
void __cdecl GlassSv_Touch(unsigned int glassId, gentity_s *other);
void __cdecl GlassSv_PredictTouch(gentity_s *other);
void __thiscall GlassesServer::WriteSnapshotToClient(GlassesServer *this, msg_t *msg, int sinceTime);
void __cdecl GlassSv_WriteSnapshotToClient(msg_t *msg, int sinceTime);
void __cdecl GlassSv_WriteGameState(msg_t *msg);
