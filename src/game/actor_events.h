#pragma once

double __fastcall Actor_EventDefaultRadiusSqrd(ai_event_t eType);
double __fastcall Actor_EventDefaultHeightDiff(ai_event_t eType);
void __fastcall Actor_GetPerfectInfo(actor_s *self, sentient_s *pOther);
void __fastcall Actor_WasAttackedBy(actor_s *self, sentient_s *pOther);
void __fastcall Actor_BroadcastTeamEvent(sentient_s *sentient, ai_event_t eType);
void __fastcall Actor_BroadcastPointEvent(
        gentity_s *originator,
        ai_event_t eType,
        int teamFlags,
        float *vOrigin,
        float fRadiusSqrd);
char __cdecl Actor_IsOnSameTeam(gentity_s *originator, actor_s *pActor, int teamFlags);
void __fastcall Actor_BroadcastLineEvent(
        gentity_s *originator,
        gentity_s *hitEnt,
        ai_event_t eType,
        int teamFlags,
        const float *vStart,
        const float *vEnd,
        float fRadiusSqrd);
void __fastcall Actor_ReceivePointEvent(
        actor_s *self,
        gentity_s *originator,
        ai_event_t eType,
        float *vOrigin,
        float fDistSqrd,
        float fRadiusSqrd);
void __cdecl Actor_DumpEvents(actor_s *self, ai_event_t event, gentity_s *originator);
void __fastcall Actor_EventFootstep(actor_s *self, sentient_s *originator, const float *vOrigin);
void __fastcall Actor_EventNewEnemy(actor_s *self, sentient_s *originator);
void __fastcall Actor_EventPain(actor_s *self, sentient_s *pCasualty, sentient_s *pAttacker);
void __fastcall Actor_EventDeath(actor_s *self, sentient_s *pCasualty, sentient_s *pAttacker);
void __cdecl Actor_EventExplosion(actor_s *self, gentity_s *originator, float *vOrigin);
void __cdecl Actor_EventGrenadePing(actor_s *self, gentity_s *originator, const float *vOrigin);
void __fastcall Actor_EventGunshot(actor_s *self, sentient_s *originator, const float *vOrigin);
void __fastcall Actor_ReceiveLineEvent(
        actor_s *self,
        gentity_s *originator,
        gentity_s *hitEnt,
        ai_event_t eType,
        const float *vStart,
        const float *vEnd,
        const float *vClosest,
        float fDistSqrd,
        float fRadiusSqrd);
void __fastcall Actor_EventBullet(
        actor_s *self,
        gentity_s *originator,
        const float *vStart,
        const float *vEnd,
        const float *vClosest,
        float fDistSqrd,
        float fRadiusSqrd,
        PARM_SUPPRESSION suppression);
