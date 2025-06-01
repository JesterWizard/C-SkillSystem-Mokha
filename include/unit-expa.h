#pragma once

#include "common-chax.h"
#include "skill-system.h"

/**
 * We use unit->_u3A and unit->_u3B
 * These 16 bits is saved in suspand data
 */

enum unit_expa_sus_bitfile_idx {
#if defined(SID_GuardBearing) && (COMMON_SKILL_VALID(SID_GuardBearing))
    UES_BIT_GUARDBEAR_SKILL_USED,
#endif

/**
 * Taking this out of the ifdef guard because it looks like multiple skills
 * use it and I can't be f**ked figuring out which ones right now
 */
// #if defined(SID_Tsuzuku) && (COMMON_SKILL_VALID(SID_Tsuzuku))
//     UES_BIT_TSZUKU_SKILL_USED,
// #endif
    UES_BIT_TSZUKU_SKILL_USED,
#if defined(SID_Teleportation) && (COMMON_SKILL_VALID(SID_Teleportation))
    UES_BIT_TELEPORTATION_SKILL_USED,
#endif
#if defined(SID_Swarp) && (COMMON_SKILL_VALID(SID_Swarp))
    UES_BIT_SWARP_SKILL_USED,
#endif
#if defined(SID_Swap) && (COMMON_SKILL_VALID(SID_Swap))
    UES_BIT_SWAP_SKILL_USED,
#endif
#if defined(SID_Roll12) && (COMMON_SKILL_VALID(SID_Roll12))
    UES_BIT_ROLL12_SKILL_USED,
#endif
#if defined(SID_Capture) && (COMMON_SKILL_VALID(SID_Capture))
    UES_BIT_CAPTURE_SKILL_USED,
#endif
#if defined(SID_Turncoat) && (COMMON_SKILL_VALID(SID_Turncoat))
    UES_BIT_CHANGED_FACTIONS,
#endif
#if defined(SID_Resurrection) && (COMMON_SKILL_VALID(SID_Resurrection))
    UES_BIT_RESURRECTION_SKILL_USED,
#endif
#if defined(SID_ShadowFlash) && (COMMON_SKILL_VALID(SID_ShadowFlash))
    UES_BIT_SHADOWFLASH_SKILL_USED,
#endif
#if defined(SID_TwinCrests) && (COMMON_SKILL_VALID(SID_TwinCrests))
    UES_BIT_TWIN_CRESTS_SKILL_USED,
#endif
#if defined(SID_DeathBlight) && (COMMON_SKILL_VALID(SID_DeathBlight))
    UES_BIT_DEATHBLIGHT_SKILL_USED,
#endif
#if defined(SID_LoadstarRush) && (COMMON_SKILL_VALID(SID_LoadstarRush))
    UES_BIT_LOADSTAR_RUSH_SKILL_USED,
#endif
#if defined(SID_WyvernCrash) && (COMMON_SKILL_VALID(SID_WyvernCrash))
    UES_BIT_WYVERN_CRASH_SKILL_USED,
#endif
#if defined(SID_Sellsword) && (COMMON_SKILL_VALID(SID_Sellsword))
    UES_BIT_SELLSWORD_SKILL_USED,
#endif
#if defined(SID_Switcher) && (COMMON_SKILL_VALID(SID_Switcher))
    UES_BIT_SWITCHER_SKILL_USED,
#endif
#if defined(SID_LethalityPlus) && (COMMON_SKILL_VALID(SID_LethalityPlus))
    UES_BIT_LETHALITY_PLUS_SKILL_USED,
#endif
#if defined(SID_GrimReaper) && (COMMON_SKILL_VALID(SID_GrimReaper))
    UES_BIT_GRIM_REAPER_SKILL_USED,
#endif
    UES_BIT_MAX
};

void SetBitUES(struct Unit * unit, int bit);
void SetBitUES_BU(struct BattleUnit * bu, int bit);
void ClearBitUES(struct Unit * unit, int bit);
bool CheckBitUES(struct Unit * unit, int bit);

void ResetUnitsExpaSus(void);
