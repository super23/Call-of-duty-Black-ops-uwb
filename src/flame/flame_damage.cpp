#include "flame_damage.h"
#include <game_mp/g_main_mp.h>
#include <game_mp/g_combat_mp.h>

// Decomp: CoDMPServer.c:705513
// Tapering afterburn for flame_timed_damage slots. G_RunFrame only calls this on
// non-client entities (movers, misc ents); players take burn damage solely from
// Flame_Impact_Process G_Damage gated by end_timestamp / damageInterval.
void __cdecl SV_Flame_Apply_Damage(gentity_s *ent)
{
    int damageSlotIndex;
    int curTime;
    int totalTime;
    int pendingDamage;
    int dflags;
    float dir[3];
    float hitPos[3];

    dflags = 0;
    dir[0] = 0.0f;
    dir[1] = 1.0f;
    dir[2] = 0.0f;
    hitPos[0] = 0.0f;
    hitPos[1] = 1.0f;
    hitPos[2] = 0.0f;

    curTime = G_GetTime();
    for ( damageSlotIndex = 0; damageSlotIndex < 4; ++damageSlotIndex )
    {
        if ( !ent->flame_timed_damage[damageSlotIndex].start_timestamp )
            continue;

        // Port fix: zero/negative interval makes the decomp while-loop spin forever.
        if ( ent->flame_timed_damage[damageSlotIndex].damageInterval < 1.0f )
            continue;

        totalTime = ent->flame_timed_damage[damageSlotIndex].start_timestamp
            + (int)ent->flame_timed_damage[damageSlotIndex].damageDuration;
        pendingDamage = 0;

        if ( !ent->flame_timed_damage[damageSlotIndex].lastupdate_timestamp )
            ent->flame_timed_damage[damageSlotIndex].lastupdate_timestamp = ent->flame_timed_damage[damageSlotIndex].start_timestamp;

        while ( ent->flame_timed_damage[damageSlotIndex].lastupdate_timestamp < totalTime
             && ent->flame_timed_damage[damageSlotIndex].lastupdate_timestamp + (int)ent->flame_timed_damage[damageSlotIndex].damageInterval <= curTime )
        {
            ent->flame_timed_damage[damageSlotIndex].lastupdate_timestamp += (int)ent->flame_timed_damage[damageSlotIndex].damageInterval;
            pendingDamage += ent->flame_timed_damage[damageSlotIndex].damage
                - (int)(float)((float)(ent->flame_timed_damage[damageSlotIndex].lastupdate_timestamp
                                     - ent->flame_timed_damage[damageSlotIndex].start_timestamp)
                             / ent->flame_timed_damage[damageSlotIndex].damageInterval);
        }

        if ( curTime > totalTime )
        {
            ent->flame_timed_damage[damageSlotIndex].start_timestamp = 0;
            ent->flame_timed_damage[damageSlotIndex].lastupdate_timestamp = 0;
            ent->flame_timed_damage[damageSlotIndex].end_timestamp = 0;
        }

        if ( (ent->health >= 0 || ent->destructible) && pendingDamage > 0 )
        {
            const int damageTime = G_GetTime();
            G_Damage(
                ent,
                ent->flame_timed_damage[damageSlotIndex].attacker,
                ent->flame_timed_damage[damageSlotIndex].attacker,
                dir,
                hitPos,
                pendingDamage,
                dflags,
                17,
                0xFFFFFFFF,
                HITLOC_NONE,
                0,
                0,
                level.time - damageTime);
        }
    }
}
