#include "common-chax.h"
#include "rn.h"
#include "bwl.h"
#include "bmunit.h"
#include "skill-system.h"
#include "jester_headers/miscellaenous.h"
#include "debuff.h"
// struct LearnedSkillList {
//     u32 data[8]; /* 8 * 32 = 0x100 */
// };

// extern struct LearnedSkillList sLearnedSkillPLists[NEW_BWL_ARRAY_NUM];

void SetGameOptions_Chapter0() 
{
    SetGameOption(GAME_OPTION_ANIMATION, 2);  // Set battle animations off
    SetGameOption(GAME_OPTION_TEXT_SPEED, 1); // Set game speed to max
    SetGameOption(GAME_OPTION_GAME_SPEED, 3); // Set text speed to max

    // SetPartyGoldAmount(10000);

    struct Unit * unit = GetUnitFromCharId(CHARACTER_EIRIKA);
    // unit->maxHP = 26;
    // unit->curHP = 6;

    unit->ranks[ITYPE_STAFF] = WPN_EXP_C;
    unit->items[0] = MakeNewItem(ITEM_VULNERARY);

    // // Seed the random number generator
    // InitRandC();

    // // Randomly generate items for the unit
    // for (int i = 0; i < 5; i++)
    //     unit->items[i] = MakeNewItem(RandNextC_N(200));

    // for (int i = 0; i < SKILL_ROM_DATA_AMT; i++)
    //     SET_SKILL(unit, i, RandNextC_N(711));

    // SetFlag(0xAB); // Gamma UI flag

    // NoCashGBAPrintf("Chapter 1 - Total BEXP BEFORE is: %d", sBEXP);

    // sBEXP.value += 100;

    // NoCashGBAPrintf("Chapter 1 - Total BEXP AFTER is: %d", sBEXP);

    // gBmMapTerrain[5][3] = TERRAIN_TILE_00;
 

    // SetUnitStatusIndex(unit_seth, NEW_UNIT_STATUS_DOOM);
    // SetUnitStatusDuration(unit_seth, 2);
}