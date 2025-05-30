#include "common-chax.h"
#include "rn.h"
#include "bwl.h"

struct LearnedSkillList {
    u32 data[8]; /* 8 * 32 = 0x100 */
};

extern struct LearnedSkillList sLearnedSkillPLists[NEW_BWL_ARRAY_NUM];

void SetGameOptions() 
{
    SetGameOption(GAME_OPTION_ANIMATION, 2);  // Set battle animations off
    SetGameOption(GAME_OPTION_TEXT_SPEED, 1); // Set game speed to max
    SetGameOption(GAME_OPTION_GAME_SPEED, 3); // Set text speed to max

    // // SetPartyGoldAmount(10000);

    // struct Unit * unit = GetUnitFromCharId(CHARACTER_EIRIKA);
    // // unit->maxHP = 26;
    // // unit->curHP = 26;

    // // unit->ranks[ITYPE_STAFF] = WPN_EXP_C;
    // // unit->items[0] = MakeNewItem(CONFIG_ITEM_INDEX_MINE_STAFF);

    // // Seed the random number generator
    // InitRandC();

    // // Randomly generate items for the unit
    // for (int i = 0; i < 5; i++)
    //     unit->items[i] = MakeNewItem(RandNextC_N(200));

    // // for (int i = 0; i < SKILL_ROM_DATA_AMT; i++)
    // //     unit->supports[i] = RandNextC_N(254);

    SetFlag(0xAB); // Gamma UI flag
}