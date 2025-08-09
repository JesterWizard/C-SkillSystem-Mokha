#include "common-chax.h"
#include "rn.h"
#include "bwl.h"
#include "bmunit.h"
#include "skill-system.h"
#include "jester_headers/miscellaenous.h"
#include "debuff.h"
#include "jester_headers/Forging.h"
#include "jester_headers/gaiden-magic.h"

// struct LearnedSkillList {
//     u32 data[8]; /* 8 * 32 = 0x100 */
// };

// extern struct LearnedSkillList sLearnedSkillPLists[NEW_BWL_ARRAY_NUM];

void SetGameOptions_Chapter0() 
{
    SetGameOption(GAME_OPTION_ANIMATION, 2);  // Set battle animations off
    SetGameOption(GAME_OPTION_TEXT_SPEED, 1); // Set game speed to max
    SetGameOption(GAME_OPTION_GAME_SPEED, 3); // Set text speed to max

    SetPartyGoldAmount(100000);

    // Call the reset function to ensure the magic lists are properly initialized
    // This is the most likely cause of your crash, as the 'amt' counter is
    // uninitialized without this call.
    // Gaiden_ResetMagicLists();

    struct Unit * unit_eirika = GetUnitFromCharId(CHARACTER_EIRIKA);
    // struct Unit * unit_seth = GetUnitFromCharId(CHARACTER_SETH);
    // unit_eirika->maxHP = 26;
    // unit_eirika->curHP = 6;

    // struct GaidenMagicSpellList *eirika_magic_list = &sBlackMagicPList[UNIT_CHAR_ID(unit_eirika)];
    // CpuFill16(0, eirika_magic_list, sizeof(struct GaidenMagicSpellList));

    Gaiden_AddBlackMagic(unit_eirika, ITEM_ANIMA_FIRE);
    //Gaiden_GetUnitBlackMagicList(unit_eirika);

    unit_eirika->ranks[ITYPE_STAFF] = WPN_EXP_A;
    unit_eirika->items[0] = MakeNewItem(ITEM_SWORD_IRON);
    // unit_eirika->items[1] = MakeNewItem(CONFIG_ITEM_INDEX_FORGE_STAFF);

    struct NewBwl * bwl_eirika = GetNewBwl(CHARACTER_EIRIKA);
    struct NewBwl * bwl_seth = GetNewBwl(CHARACTER_SETH);
    //bwl_eirika->currentMP += 10;
    bwl_eirika->maxMP = 30;
    bwl_seth->maxMP = 30;


    // unit_eirika->ranks[ITYPE_STAFF] = WPN_EXP_A;
    // unit_eirika->items[1] = ITEM_KNIGHTCREST;
    // unit_eirika->items[2] = ITEM_MASTERSEAL;

    // unit_seth->pClassData = GetClassData(CLASS_CAVALIER);
    // unit_seth->level = 10;

    // unit_eirika->pClassData = GetClassData(CLASS_ARMOR_KNIGHT);
    // unit_eirika->level = 10;
    
    // // Seed the random number generator
    // InitRandC();

    // // Randomly generate items for the unit
    // // u32 random_value = RandC();
    // // int i;
    // // for (i = 0; i < 4; i++) {
    // //     unit_eirika->items[i] = random_value & 0xF;
    // //     random_value >>= 4;
    // // }
}

