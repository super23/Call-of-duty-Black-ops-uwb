#pragma once

#include "g_main_sp.h"

// SP MP-shared tail fields live at the start of spLevelMidPad in the CoDSP layout.
struct level_sp_tail_fields_t
{
    int teamScores[4];
    int lastTeammateHealthTime;
    unsigned int teamHasSpyplane[4];
    unsigned int teamHasSatellite[4];
    int voteTime;
    int voteExecuteTime;
    int voteYes;
    int voteNo;
};

static_assert(sizeof(level_sp_tail_fields_t) == 0x44, "level_sp_tail_fields_t must match CoDSP tail header");

inline level_sp_tail_fields_t &G_LevelTailFields()
{
    return *reinterpret_cast<level_sp_tail_fields_t *>(level.spLevelMidPad);
}

#define level_tail G_LevelTailFields()
