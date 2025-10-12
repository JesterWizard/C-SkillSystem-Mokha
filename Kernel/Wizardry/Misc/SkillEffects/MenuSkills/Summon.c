#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "mu.h"
#include "jester_headers/class-arrays.h"
#include "jester_headers/class-pairs.h"

extern u16 gUnknown_085A0D4C[];

LYN_REPLACE_CHECK(SummonCommandUsability);
u8 SummonCommandUsability(const struct MenuItemDef * def, int number)
{
    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

    MakeTargetListForSummon(gActiveUnit);

    if (GetSelectTargetCount() == 0)
        return MENU_NOTSHOWN;

    return MENU_ENABLED;
}

LYN_REPLACE_CHECK(SummonCommandEffect);
u8 SummonCommandEffect(struct MenuProc* menu, struct MenuItemProc* menuItem) {

    MakeTargetListForSummon(gActiveUnit);
    NewTargetSelection(&gSelectInfo_Summon);

    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
}

LYN_REPLACE_CHECK(SummonSelection_OnSelect);
u8 SummonSelection_OnSelect(ProcPtr proc, struct SelectTarget* target) {

    GetUnit(gActionData.subjectIndex);

    gActionData.unitActionType = UNIT_ACTION_SUMMON;
    gActionData.xOther = target->x;
    gActionData.yOther = target->y;

    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

///////////////////////////////////////////////////
// Summon+ Menu Creation
///////////////////////////////////////////////////

STATIC_DECLAR const struct MenuItemDef GenerateSummons[];
STATIC_DECLAR u8 GenerateSummons_HelpBox(struct MenuProc * menu, struct MenuItemProc * item);
STATIC_DECLAR u8 GenerateSummons_OnCancel(struct MenuProc * menu, struct MenuItemProc * item);
STATIC_DECLAR u8 GenerateSummons_Usability(const struct MenuItemDef * self, int number);
STATIC_DECLAR int GenerateSummons_OnDraw(struct MenuProc * menu, struct MenuItemProc * item);
STATIC_DECLAR u8 GenerateSummons_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);

STATIC_DECLAR u8 GenerateSummons_HelpBox(struct MenuProc * menu, struct MenuItemProc * item)
{
    bool isSummonerPromoted = !!(UNIT_CATTRIBUTES(gActiveUnit) & CA_PROMOTED);

    int classDescriptionId;

    const u8 menuIndex = MENU_SKILL_INDEX(item->def);

    if (isSummonerPromoted)
        classDescriptionId = GetClassData(classPromotedIndexes[menuIndex])->descTextId;
    else
        classDescriptionId = GetClassData(classIndexes[menuIndex])->descTextId;

    StartHelpBox(
        item->xTile * 8,
        item->yTile * 8,
        classDescriptionId
    );

    return 0;
}

STATIC_DECLAR u8 GenerateSummons_OnCancel(struct MenuProc * menu, struct MenuItemProc * item)
{
    /* Reset action */
    gActionData.unitActionType = 0;

    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);
    HideMoveRangeGraphics();

    /* Prevent other menus from freezing because of our little dpad hack in ProcessMenuDpadInput */
    gActionData.unk08 = 0;

    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

/* Add a condition here later to not display the summon menu if there are no free adjacent spots */
STATIC_DECLAR u8 GenerateSummons_Usability(const struct MenuItemDef * self, int number)
{
    return MENU_ENABLED;
}

STATIC_DECLAR int GenerateSummons_OnDraw(struct MenuProc * menu, struct MenuItemProc * item)
{
#if defined(SID_SummonPlus) && defined(SID_SummonPlus)
    gActionData.unk08 = SID_SummonPlus;
#endif

    const u8 menuIndex = MENU_SKILL_INDEX(item->def);

    Text_SetColor(&item->text, TEXT_COLOR_SYSTEM_GOLD);

    CallARM_FillTileRect(gBG1TilemapBuffer + 0x42, gUnknown_085A0D4C, 0x1000);

    bool isSummonerPromoted = !!(UNIT_CATTRIBUTES(gActiveUnit) & CA_PROMOTED);

    if (isSummonerPromoted)
    {
        Text_DrawString(&item->text, GetStringFromIndex(GetClassData(classPromotedIndexes[menuIndex])->nameTextId));
        PutFace80x72_Core(gBG0TilemapBuffer + 0x63 + 0x40, GetClassData(classPromotedIndexes[0])->defaultPortraitId, 0x200, 5);
    }
    else
    {
        Text_DrawString(&item->text, GetStringFromIndex(GetClassData(classIndexes[menuIndex])->nameTextId));
        PutFace80x72_Core(gBG0TilemapBuffer + 0x63 + 0x40, GetClassData(classIndexes[0])->defaultPortraitId, 0x200, 5);
    }

    PutText(
        &item->text,
        TILEMAP_LOCATED(gBG0TilemapBuffer, item->xTile + 1, item->yTile));

    BG_EnableSyncByMask(BG0_SYNC_BIT);

    return 0;
}

STATIC_DECLAR u8 GenerateSummons_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{
    const u8 menuIndex = MENU_SKILL_INDEX(item->def);

    bool isSummonerPromoted = !!(UNIT_CATTRIBUTES(gActiveUnit) & CA_PROMOTED);

    if (isSummonerPromoted)
    {
        gEventSlots[EVT_SLOT_7] = classPromotedIndexes[menuIndex];
        gEventSlots[EVT_SLOT_8] = classPromotedWeapons[menuIndex];
    }
    else
    {
        gEventSlots[EVT_SLOT_7] = classIndexes[menuIndex];
        gEventSlots[EVT_SLOT_8] = classWeapons[menuIndex];
    }

    MakeTargetListForSummon(gActiveUnit);
    NewTargetSelection(&gSelectInfo_Summon);

    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

#define GenerateSummon(i) \
{ \
    .helpMsgId = (i), \
    .isAvailable = GenerateSummons_Usability, \
    .onDraw = GenerateSummons_OnDraw, \
    .onSelected = GenerateSummons_OnSelected, \
}

STATIC_DECLAR const struct MenuItemDef GenerateSummons[] = 
{
    GenerateSummon(0),
    GenerateSummon(1),
    GenerateSummon(2),
    GenerateSummon(3),
    GenerateSummon(4),
    GenerateSummon(5),
    { 0 }
};

const struct MenuDef gSelectInfo_SummonPlus = {
    {15, 1, 12, 0},
    0,
    GenerateSummons,
    0, 0, 0,
    GenerateSummons_OnCancel,
    MenuAutoHelpBoxSelect,
    GenerateSummons_HelpBox
};

#if defined(SID_SummonPlus) && defined(SID_SummonPlus)
u8 SummonPlusCommandEffect(struct MenuProc* menu, struct MenuItemProc* menuItem) 
{
    StartSubtitleHelp(
        StartOrphanMenu(&gSelectInfo_SummonPlus),
        GetStringFromIndex(MSG_SelectSummon)
    );

    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}
#endif

LYN_REPLACE_CHECK(GenerateSummonUnitDef);
void GenerateSummonUnitDef(void)
{
    u8 rand100 = DivRem(AdvanceGetLCGRNValue(), 101);

    struct Unit* unit;
    short summonerNum, i;

#if defined(SID_SummonPlus) && defined(SID_SummonPlus)
    if (gActionData.unk08 == SID_SummonPlus)
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
        unit->exp   = UNIT_EXP_DISABLED;

        /* Prevent other menus from freezing because of our little dpad hack in ProcessMenuDpadInput */
        gActionData.unk08 = 0;

    }
    else
    {
        // 1. Find summoner number from active unit
        summonerNum = -1;
        for (i = 0; i < (short)ARRAY_COUNT(gSummonConfig); ++i) {
            if (UNIT_CHAR_ID(gActiveUnit) == gSummonConfig[i][0]) {
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
                    if (UNIT_CHAR_ID(unit) == gSummonConfig[summonerNum][1])
                        ClearUnit(unit);
                }
            }
        }

        // 3. Set up unit definition
        unit = NULL;

        // 3.1. Character/Class/Faction/Level/Position
        gUnitDef1.charIndex       = gSummonConfig[summonerNum][1];
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
        unit = GetUnitFromCharId(gSummonConfig[summonerNum][1]);

        if (unit == NULL) {
            struct BattleUnit bu = gBattleActor;
            LoadUnits(&gUnitDef1);
            gBattleActor = bu;
        }

        // 5. Set level and weapon ranks
        unit = GetUnitFromCharId(gSummonConfig[summonerNum][1]);

        for (i = 0; i < 4; ++i)
            unit->ranks[i] = 0;

        unit->level = gActiveUnit->level;
        unit->exp   = UNIT_EXP_DISABLED;

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
    for (i = 0; i < (short)ARRAY_COUNT(gSummonConfig); ++i) {
        if (UNIT_CHAR_ID(gActiveUnit) == gSummonConfig[i][0]) {
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
                if (UNIT_CHAR_ID(unit) == gSummonConfig[summonerNum][1])
                    ClearUnit(unit);
            }
        }
    }

    // 3. Set up unit definition
    unit = NULL;

    // 3.1. Character/Class/Faction/Level/Position
    gUnitDef1.charIndex       = gSummonConfig[summonerNum][1];
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
    unit = GetUnitFromCharId(gSummonConfig[summonerNum][1]);

    if (unit == NULL) {
        struct BattleUnit bu = gBattleActor;
        LoadUnits(&gUnitDef1);
        gBattleActor = bu;
    }

    // 5. Set level and weapon ranks
    unit = GetUnitFromCharId(gSummonConfig[summonerNum][1]);

    for (i = 0; i < 4; ++i)
        unit->ranks[i] = 0;

    unit->level = gActiveUnit->level;
    unit->exp   = UNIT_EXP_DISABLED;

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