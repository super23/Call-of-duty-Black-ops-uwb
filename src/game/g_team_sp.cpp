#include "g_team_sp.h"
#include "g_main_sp.h"

bool __cdecl OnSameTeam(gentity_s *ent1, gentity_s *ent2)
{
    if ( !ent1->client || !ent2->client )
        return false;
    if ( ent1->client->sess.cs.team )
        return ent1->client->sess.cs.team == ent2->client->sess.cs.team;
    return false;
}
