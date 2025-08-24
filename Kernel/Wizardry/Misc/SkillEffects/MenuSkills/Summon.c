#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "mu.h"
#include "jester_headers/class-arrays.h"
#include "jester_headers/class-pairs.h"

LYN_REPLACE_CHECK(SummonCommandUsability);
u8 SummonCommandUsability(const struct MenuItemDef *def, int number)
{
	if (gActiveUnit->state & US_CANTOING)
		return MENU_NOTSHOWN;

	MakeTargetListForSummon(gActiveUnit);
	if (GetSelectTargetCount() == 0)
		return MENU_NOTSHOWN;

	return MENU_ENABLED;
}

LYN_REPLACE_CHECK(GenerateSummonUnitDef);
void GenerateSummonUnitDef(void)
{
    u8 rand100 = DivRem(AdvanceGetLCGRNValue(), 101);

    struct Unit* unit;
    int summonerNum, i;

#if defined(SID_SummonPlus) && defined(SID_SummonPlus)
    if (gActionData.unk08 == SID_SummonPlus)
    {
        // 1. Find summoner number from active unit
        summonerNum = -1;
        for (i = 0; i < (int)ARRAY_COUNT(gNewSummonConfig); ++i) {
            if (UNIT_CHAR_ID(gActiveUnit) == gNewSummonConfig[i][0]) {
                summonerNum = i;
                break;
            }
        }

        if (summonerNum == -1)
            return;
            
        gUnitDef1.charIndex       = gNewSummonConfig[summonerNum][1];
        gUnitDef1.classIndex      = gEventSlots[EVT_SLOT_7];
        gUnitDef1.leaderCharIndex = CHARACTER_NONE;
        gUnitDef1.autolevel       = TRUE;

        if (UNIT_FACTION(gActiveUnit) == FACTION_BLUE)
        gUnitDef1.allegiance = 0;

        else if (UNIT_FACTION(gActiveUnit) == FACTION_RED)
            gUnitDef1.allegiance = 2;

        else if (UNIT_FACTION(gActiveUnit) == FACTION_GREEN)
            gUnitDef1.allegiance = 1;

        gUnitDef1.level           = gActiveUnit->level;
        gUnitDef1.xPosition       = gActionData.xOther;
        gUnitDef1.yPosition       = gActionData.yOther;
        gUnitDef1.redaCount       = 0;
        gUnitDef1.redas           = NULL;
        gUnitDef1.genMonster      = FALSE;
        gUnitDef1.itemDrop        = FALSE;

        int i;
        for (i = 1; i < 0x40; ++i) {
            struct Unit* unit = GetUnit(i);

            if (UNIT_IS_VALID(unit)) {
                if (UNIT_CHAR_ID(unit) == gNewSummonConfig[summonerNum][1])
                    ClearUnit(unit);
            }
        }

        gUnitDef1.items[0] = gEventSlots[EVT_SLOT_8];

        unit = GetUnitFromCharId(gNewSummonConfig[summonerNum][1]);

        if (unit == NULL) {
            struct BattleUnit bu = gBattleActor;
            LoadUnits(&gUnitDef1);
            gBattleActor = bu;
        }

        unit->level = gActiveUnit->level;

#ifndef CONFIG_SUMMONERS_GAIN_EXP_FROM_SUMMON_FIGHTS
        unit->exp   = UNIT_EXP_DISABLED;
#endif

        /* Prevent other menus from freezing because of our little dpad hack in ProcessMenuDpadInput */
        gActionData.unk08 = 0;

    }
    else
    {
        // 1. Find summoner number from active unit
        summonerNum = -1;
        for (i = 0; i < (short)ARRAY_COUNT(gNewSummonConfig); ++i) {
            if (UNIT_CHAR_ID(gActiveUnit) == gNewSummonConfig[i][0]) {
                summonerNum = i;
                break;
            }
        }

        if (summonerNum == -1)
            return;

        // 2. Clear existing summon
        // NOTE: this may have been a macro? (because of different i and unit?)
        {
            int i;
            for (i = 1; i < 0x40; ++i) {
                struct Unit* unit = GetUnit(i);

                if (UNIT_IS_VALID(unit)) {
                    if (UNIT_CHAR_ID(unit) == gNewSummonConfig[summonerNum][1])
                        ClearUnit(unit);
                }
            }
        }

        // 3. Set up unit definition
        unit = NULL;

        // 3.1. Character/Class/Faction/Level/Position
        gUnitDef1.charIndex       = gNewSummonConfig[summonerNum][1];
        gUnitDef1.classIndex      = CLASS_PHANTOM;
        gUnitDef1.leaderCharIndex = CHARACTER_NONE;
        gUnitDef1.autolevel       = TRUE;

        if (UNIT_FACTION(gActiveUnit) == FACTION_BLUE)
            gUnitDef1.allegiance = 0;

        else if (UNIT_FACTION(gActiveUnit) == FACTION_RED)
            gUnitDef1.allegiance = 2;

        else if (UNIT_FACTION(gActiveUnit) == FACTION_GREEN)
            gUnitDef1.allegiance = 1;

        gUnitDef1.level = gActiveUnit->level;

        gUnitDef1.xPosition = gActionData.xOther;
        gUnitDef1.yPosition = gActionData.yOther;

        gUnitDef1.redaCount = 0;
        gUnitDef1.redas = NULL;

        gUnitDef1.genMonster = FALSE;
        gUnitDef1.itemDrop = FALSE;

        // 3.2. Items (generated from random number)
        for (i = 0; i < UNIT_DEFINITION_ITEM_COUNT; ++i)
            gUnitDef1.items[i] = ITEM_NONE;

        if (gActiveUnit->level <= 5)
            gUnitDef1.items[0] = ITEM_AXE_IRON;
        else if (gActiveUnit->level <= 10) {
            if (rand100 < 6)
                gUnitDef1.items[0] = ITEM_AXE_DEVIL;
            else
                gUnitDef1.items[0] = ITEM_AXE_IRON;
        }
        else if (gActiveUnit->level <= 15) {
            if (rand100 < 6)
                gUnitDef1.items[0] = ITEM_AXE_DEVIL;

            else if (rand100 >= 6 && rand100 < 26)
                gUnitDef1.items[0] = ITEM_AXE_KILLER;

            else
                gUnitDef1.items[0] = ITEM_AXE_IRON;
        }
        else if (gActiveUnit->level <= 20) {
            if (rand100 < 6)
                gUnitDef1.items[0] = ITEM_AXE_DEVIL;

            else if (rand100 >= 6 && rand100 < 26)
                gUnitDef1.items[0] = ITEM_AXE_KILLER;

            else if (rand100 >= 26 && rand100 < 37)
                gUnitDef1.items[0] = ITEM_AXE_TOMAHAWK;

            else
                gUnitDef1.items[0] = ITEM_AXE_IRON;
        }

        // 3.3. Ai (is null)
        for (i = 0; i < 4; ++i)
            gUnitDef1.ai[i] = 0;

        // 4. Load unit
        unit = GetUnitFromCharId(gNewSummonConfig[summonerNum][1]);

        if (unit == NULL) {
            struct BattleUnit bu = gBattleActor;
            LoadUnits(&gUnitDef1);
            gBattleActor = bu;
        }

        // 5. Set level and weapon ranks
        unit = GetUnitFromCharId(gNewSummonConfig[summonerNum][1]);

        for (i = 0; i < 4; ++i)
            unit->ranks[i] = 0;

        unit->level = gActiveUnit->level;

#ifndef CONFIG_SUMMONERS_GAIN_EXP_FROM_SUMMON_FIGHTS
        unit->exp   = UNIT_EXP_DISABLED;
#endif

        if (gActiveUnit->level <= 5)
            unit->ranks[ITYPE_AXE] = WPN_EXP_D;
        else if (gActiveUnit->level <= 10)
            unit->ranks[ITYPE_AXE] = WPN_EXP_C;
        else if (gActiveUnit->level <= 15)
            unit->ranks[ITYPE_AXE] = WPN_EXP_B;
        else if (gActiveUnit->level <= 20)
            unit->ranks[ITYPE_AXE] = WPN_EXP_A;
    }
#else
    // 1. Find summoner number from active unit
    summonerNum = -1;
    for (i = 0; i < (short)ARRAY_COUNT(gNewSummonConfig); ++i) {
        if (UNIT_CHAR_ID(gActiveUnit) == gNewSummonConfig[i][0]) {
            summonerNum = i;
            break;
        }
    }

    if (summonerNum == -1)
        return;

    // 2. Clear existing summon
    // NOTE: this may have been a macro? (because of different i and unit?)
    {
        int i;
        for (i = 1; i < 0x40; ++i) {
            struct Unit* unit = GetUnit(i);

            if (UNIT_IS_VALID(unit)) {
                if (UNIT_CHAR_ID(unit) == gNewSummonConfig[summonerNum][1])
                    ClearUnit(unit);
            }
        }
    }

    // 3. Set up unit definition
    unit = NULL;

    // 3.1. Character/Class/Faction/Level/Position
    gUnitDef1.charIndex       = gNewSummonConfig[summonerNum][1];
    gUnitDef1.classIndex      = CLASS_PHANTOM;
    gUnitDef1.leaderCharIndex = CHARACTER_NONE;
    gUnitDef1.autolevel       = TRUE;

    if (UNIT_FACTION(gActiveUnit) == FACTION_BLUE)
        gUnitDef1.allegiance = 0;

    else if (UNIT_FACTION(gActiveUnit) == FACTION_RED)
        gUnitDef1.allegiance = 2;

    else if (UNIT_FACTION(gActiveUnit) == FACTION_GREEN)
        gUnitDef1.allegiance = 1;

    gUnitDef1.level = gActiveUnit->level;

    gUnitDef1.xPosition = gActionData.xOther;
    gUnitDef1.yPosition = gActionData.yOther;

    gUnitDef1.redaCount = 0;
    gUnitDef1.redas = NULL;

    gUnitDef1.genMonster = FALSE;
    gUnitDef1.itemDrop = FALSE;

    // 3.2. Items (generated from random number)
    for (i = 0; i < UNIT_DEFINITION_ITEM_COUNT; ++i)
        gUnitDef1.items[i] = ITEM_NONE;

    if (gActiveUnit->level <= 5)
        gUnitDef1.items[0] = ITEM_AXE_IRON;
    else if (gActiveUnit->level <= 10) {
        if (rand100 < 6)
            gUnitDef1.items[0] = ITEM_AXE_DEVIL;
        else
            gUnitDef1.items[0] = ITEM_AXE_IRON;
    }
    else if (gActiveUnit->level <= 15) {
        if (rand100 < 6)
            gUnitDef1.items[0] = ITEM_AXE_DEVIL;

        else if (rand100 >= 6 && rand100 < 26)
            gUnitDef1.items[0] = ITEM_AXE_KILLER;

        else
            gUnitDef1.items[0] = ITEM_AXE_IRON;
    }
    else if (gActiveUnit->level <= 20) {
        if (rand100 < 6)
            gUnitDef1.items[0] = ITEM_AXE_DEVIL;

        else if (rand100 >= 6 && rand100 < 26)
            gUnitDef1.items[0] = ITEM_AXE_KILLER;

        else if (rand100 >= 26 && rand100 < 37)
            gUnitDef1.items[0] = ITEM_AXE_TOMAHAWK;

        else
            gUnitDef1.items[0] = ITEM_AXE_IRON;
    }

    // 3.3. Ai (is null)
    for (i = 0; i < 4; ++i)
        gUnitDef1.ai[i] = 0;

    // 4. Load unit
    unit = GetUnitFromCharId(gNewSummonConfig[summonerNum][1]);

    if (unit == NULL) {
        struct BattleUnit bu = gBattleActor;
        LoadUnits(&gUnitDef1);
        gBattleActor = bu;
    }

    // 5. Set level and weapon ranks
    unit = GetUnitFromCharId(gNewSummonConfig[summonerNum][1]);

    for (i = 0; i < 4; ++i)
        unit->ranks[i] = 0;

    unit->level = gActiveUnit->level;

#ifndef CONFIG_SUMMONERS_GAIN_EXP_FROM_SUMMON_FIGHTS
    unit->exp   = UNIT_EXP_DISABLED;
#endif

    if (gActiveUnit->level <= 5)
        unit->ranks[ITYPE_AXE] = WPN_EXP_D;
    else if (gActiveUnit->level <= 10)
        unit->ranks[ITYPE_AXE] = WPN_EXP_C;
    else if (gActiveUnit->level <= 15)
        unit->ranks[ITYPE_AXE] = WPN_EXP_B;
    else if (gActiveUnit->level <= 20)
        unit->ranks[ITYPE_AXE] = WPN_EXP_A;
#endif
}