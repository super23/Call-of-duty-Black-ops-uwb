#include "flame_damage.h"

void __cdecl SV_Flame_Apply_Damage(gentity_s *ent)
{
  int Time; // eax
  int damage; // [esp+8h] [ebp-2Ch]
  float dir[3]; // [esp+Ch] [ebp-28h] BYREF
  int cur_time; // [esp+18h] [ebp-1Ch]
  int dflags; // [esp+1Ch] [ebp-18h]
  float hitPos[3]; // [esp+20h] [ebp-14h] BYREF
  int i; // [esp+2Ch] [ebp-8h]
  int total_time; // [esp+30h] [ebp-4h]

  dflags = 0;
  dir[0] = *(float *)&FLOAT_0_0;
  dir[1] = FLOAT_1_0;
  dir[2] = *(float *)&FLOAT_0_0;
  hitPos[0] = *(float *)&FLOAT_0_0;
  hitPos[1] = FLOAT_1_0;
  hitPos[2] = *(float *)&FLOAT_0_0;
  cur_time = G_GetTime();
  for ( i = 0; i < 4; ++i )
  {
    if ( ent->flame_timed_damage[i].start_timestamp )
    {
      total_time = ent->flame_timed_damage[i].start_timestamp + (int)ent->flame_timed_damage[i].damageDuration;
      damage = 0;
      if ( !ent->flame_timed_damage[i].lastupdate_timestamp )
        ent->flame_timed_damage[i].lastupdate_timestamp = ent->flame_timed_damage[i].start_timestamp;
      while ( ent->flame_timed_damage[i].lastupdate_timestamp < total_time
           && ent->flame_timed_damage[i].lastupdate_timestamp + (int)ent->flame_timed_damage[i].damageInterval <= cur_time )
      {
        ent->flame_timed_damage[i].lastupdate_timestamp += (int)ent->flame_timed_damage[i].damageInterval;
        damage += ent->flame_timed_damage[i].damage
                - (int)(float)((float)(ent->flame_timed_damage[i].lastupdate_timestamp
                                     - ent->flame_timed_damage[i].start_timestamp)
                             / ent->flame_timed_damage[i].damageInterval);
      }
      if ( cur_time > total_time )
      {
        ent->flame_timed_damage[i].start_timestamp = 0;
        ent->flame_timed_damage[i].lastupdate_timestamp = 0;
        ent->flame_timed_damage[i].end_timestamp = 0;
      }
      if ( (ent->health >= 0 || ent->destructible) && damage > 0 )
      {
        Time = G_GetTime();
        G_Damage(
          ent,
          ent->flame_timed_damage[i].attacker,
          ent->flame_timed_damage[i].attacker,
          dir,
          hitPos,
          damage,
          dflags,
          17,
          0xFFFFFFFF,
          HITLOC_NONE,
          0,
          0,
          level.time - Time);
      }
    }
  }
}

