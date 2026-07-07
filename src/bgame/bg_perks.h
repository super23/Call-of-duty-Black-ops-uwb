#pragma once

#include <universal/dvar.h>
#include <universal/q_shared.h>

#ifdef KISAK_SP
#define BG_PERK_COUNT 15
#define BG_PERK_INVALID 15
#else
#define BG_PERK_COUNT 52
#define BG_PERK_INVALID 52
#endif

unsigned int __cdecl BG_GetPerkIndexForName(const char *perkName);
const char *__cdecl BG_GetPerkNameForIndex(unsigned int perkIndex);
void __cdecl Perks_RegisterDvars();

extern const dvar_t *perk_skillSet;
extern const dvar_t *perk_weapSpreadMultiplier;
extern const dvar_t *perk_weapRateMultiplier;
extern const dvar_t *perk_weapReloadMultiplier;
extern const dvar_t *perk_weapSwitchMultiplier;
extern const dvar_t *perk_weapAdsMultiplier;
extern const dvar_t *perk_weapMeleeMultiplier;
extern const dvar_t *perk_extraBreath;
extern const dvar_t *perk_bulletPenetrationMultiplier;
extern const dvar_t *perk_grenadeDeath;
extern const dvar_t *perk_sprintMultiplier;
extern const dvar_t *perk_grenadeTossBackTimer;
extern const dvar_t *perk_killstreakReduction;
extern const dvar_t *perk_extraMoneyMultiplier;
extern const dvar_t *perk_delayExplosiveTime;
extern const dvar_t *perk_disarmExplosiveTime;
extern const dvar_t *perk_speedMultiplier;
extern const dvar_t *perk_deathStreakCountRequired;
extern const dvar_t *perk_sprintRecoveryMultiplier;
extern const dvar_t *perk_healthRegenMultiplier;
extern const dvar_t *perk_damageKickReduction;
extern const dvar_t *perk_shellShockReduction;
extern const dvar_t *perk_mantleReduction;
extern const dvar_t *perk_interactSpeedMultiplier;
extern const dvar_t *perk_throwbackInnerRadius;
extern const dvar_t *perk_flakJacket;
extern const dvar_t *perk_flakJacket_hardcore;
extern const dvar_t *perk_fireproof;
extern const dvar_t *perk_armorPiercing;
extern const dvar_t *perk_dogsAttackGhost;
extern const dvar_t *perk_blackbirdShowsGpsJammer;

inline bool __cdecl BG_HasPerk(const unsigned int *perks, unsigned int perkIndex)
{
    if ( perkIndex >= BG_PERK_COUNT
        && !Assert_MyHandler(
            "c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../bgame/bg_perks.h",
            136,
            0,
            "perkIndex doesn't index PERK_COUNT\n\t%i not in [0, %i)",
            perkIndex,
            BG_PERK_COUNT) )
    {
        __debugbreak();
    }
    return (*(_QWORD *)perks & (1LL << perkIndex)) != 0;
}
