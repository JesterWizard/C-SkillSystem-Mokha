#include "common-chax.h"
#include "icon-rework.h"
#include "skill-system.h"
#include "battle-system.h"
#include "constants/texts.h"

STATIC_DECLAR const struct MenuItemDef RemoveSkillMenuItems[];
STATIC_DECLAR const struct MenuItemDef PredationSkillMenuItems[];
STATIC_DECLAR u8 RemoveSkillMenu_HelpBox(struct MenuProc * menu, struct MenuItemProc * item);
STATIC_DECLAR u8 PredationSkillMenu_HelpBox(struct MenuProc * menu, struct MenuItemProc * item);
STATIC_DECLAR u8 RemoveSkillMenu_OnCancel(struct MenuProc * menu, struct MenuItemProc * item);

#ifdef CONFIG_TURN_ON_ALL_SKILLS
    // Choose the proper scroll index based on the high byte of the skill id.
    #define GET_SKILL_SCROLL_INDEX(sid) (((sid) > 0x2FF) ? CONFIG_ITEM_INDEX_SKILL_SCROLL_4 : \
                                        (((sid) > 0x1FF) ? CONFIG_ITEM_INDEX_SKILL_SCROLL_3 : \
                                        (((sid) > 0x0FF) ? CONFIG_ITEM_INDEX_SKILL_SCROLL_2 : \
                                                           CONFIG_ITEM_INDEX_SKILL_SCROLL_1)))
#else
    #define GET_SKILL_SCROLL_INDEX(sid) CONFIG_ITEM_INDEX_SKILL_SCROLL_1
#endif

const struct MenuDef RemoveSkillMenuDef = {
    {1, 1, 14, 0},
    0,
    RemoveSkillMenuItems,
    0, 0, 0,
    RemoveSkillMenu_OnCancel,
    MenuAutoHelpBoxSelect,
    RemoveSkillMenu_HelpBox
};

const struct MenuDef PredationSkillMenuDef = {
    {1, 1, 14, 0},
    0,
    PredationSkillMenuItems,
    0, 0, 0,
    RemoveSkillMenu_OnCancel,
    MenuAutoHelpBoxSelect,
    PredationSkillMenu_HelpBox
};

STATIC_DECLAR u8 RemoveSkillMenu_HelpBox(struct MenuProc * menu, struct MenuItemProc * item)
{
    if (MENU_SKILL_INDEX(item->def) > UNIT_RAM_SKILLS_LEN - 1)
    {
        // Use gBattleTarget's first skill for help.
        struct Unit * targetUnit = GetUnit(gBattleTarget.unit.index);
        StartHelpBox(
            item->xTile * 8,
            item->yTile * 8,
            GetSkillDescMsg(GET_SKILL(targetUnit, MENU_SKILL_INDEX(item->def) - UNIT_RAM_SKILLS_LEN))
        );
    }
    else
    {
        StartHelpBox(
            item->xTile * 8,
            item->yTile * 8,
            GetSkillDescMsg(GET_SKILL(gActiveUnit, MENU_SKILL_INDEX(item->def)))
        );
    }
    return 0;
}

STATIC_DECLAR u8 PredationSkillMenu_HelpBox(struct MenuProc * menu, struct MenuItemProc * item)
{
    struct Unit * targetUnit = GetUnit(gBattleTarget.unit.index);

    StartHelpBox(
        item->xTile * 8,
        item->yTile * 8,
        GetSkillDescMsg(GET_SKILL(targetUnit, MENU_SKILL_INDEX(item->def)))
    );
    return 0;
}


STATIC_DECLAR u8 RemoveSkillMenu_OnCancel(struct MenuProc * menu, struct MenuItemProc * item)
{
    /* Reset action */
    gActionData.unitActionType = 0;

    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);
    HideMoveRangeGraphics();

    return ItemCommandEffect(menu, item);
}

STATIC_DECLAR u8 RemoveSkillMenu_Usability(const struct MenuItemDef * self, int number);
STATIC_DECLAR int RemoveSkillMenu_OnDraw(struct MenuProc * menu, struct MenuItemProc * item);
STATIC_DECLAR u8 RemoveSkillMenu_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);

STATIC_DECLAR u8 PredationSkillMenu_Usability(const struct MenuItemDef * self, int number);
STATIC_DECLAR int PredationSkillMenu_OnDraw(struct MenuProc * menu, struct MenuItemProc * item);
STATIC_DECLAR u8 PredationSkillMenu_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);

#define RemoveSkillMenuItem(i) \
{ \
    .helpMsgId = (i), \
    .isAvailable = RemoveSkillMenu_Usability, \
    .onDraw = RemoveSkillMenu_OnDraw, \
    .onSelected = RemoveSkillMenu_OnSelected, \
}

#define PredationSkillMenuItem(i) \
{ \
    .helpMsgId = (i), \
    .isAvailable = PredationSkillMenu_Usability, \
    .onDraw = PredationSkillMenu_OnDraw, \
    .onSelected = PredationSkillMenu_OnSelected, \
}

/* This is the unit's skill list plus an optional enemy skill for predation */
STATIC_DECLAR const struct MenuItemDef RemoveSkillMenuItems[] = 
{
    RemoveSkillMenuItem(0),
    RemoveSkillMenuItem(1),
    RemoveSkillMenuItem(2),
    RemoveSkillMenuItem(3),
    RemoveSkillMenuItem(4),
#ifndef CONFIG_TURN_ON_ALL_SKILLS
    RemoveSkillMenuItem(5),
    RemoveSkillMenuItem(6),

    /* Enemy skills */
    RemoveSkillMenuItem(7),
#else
    /* Enemy skills */
    RemoveSkillMenuItem(5),
#endif
    { 0 }
};

/* Enemy's skill list*/
STATIC_DECLAR const struct MenuItemDef PredationSkillMenuItems[] = 
{
    PredationSkillMenuItem(0),
    PredationSkillMenuItem(1),
    PredationSkillMenuItem(2),
    PredationSkillMenuItem(3),
    PredationSkillMenuItem(4),
#ifndef CONFIG_TURN_ON_ALL_SKILLS
    PredationSkillMenuItem(5),
    PredationSkillMenuItem(6),
#endif
    { 0 }
};

/* I'm making it return the menu definition by default for the sake of displaying prospective enemy skills */
STATIC_DECLAR u8 RemoveSkillMenu_Usability(const struct MenuItemDef * self, int number)
{
    // int sid = UNIT_RAM_SKILLS(gActiveUnit)[MENU_SKILL_INDEX(self)];
    // if (EQUIP_SKILL_VALID(sid))
    //     return MENU_ENABLED;

    // return MENU_NOTSHOWN;

    return MENU_ENABLED;
}

STATIC_DECLAR u8 PredationSkillMenu_Usability(const struct MenuItemDef * self, int number)
{
    struct Unit * targetUnit = GetUnit(gBattleTarget.unit.index);

    int sid = GET_SKILL(targetUnit, MENU_SKILL_INDEX(self));
    if (EQUIP_SKILL_VALID(sid))
        return MENU_ENABLED;

    return MENU_NOTSHOWN;
}

STATIC_DECLAR int PredationSkillMenu_OnDraw(struct MenuProc * menu, struct MenuItemProc * item)
{
    int sid;

    struct Unit * targetUnit = GetUnit(gBattleTarget.unit.index);
    sid = GET_SKILL(targetUnit, MENU_SKILL_INDEX(item->def));
    Text_SetColor(&item->text, TEXT_COLOR_SYSTEM_GOLD);

    Text_DrawString(&item->text, GetSkillNameStr(sid));
    DrawIcon(
        TILEMAP_LOCATED(gBG0TilemapBuffer, item->xTile, item->yTile),
        SKILL_ICON(sid), 0x4000);

    PutText(
        &item->text,
        TILEMAP_LOCATED(gBG0TilemapBuffer, item->xTile + 2, item->yTile));

    BG_EnableSyncByMask(BG0_SYNC_BIT);
    return 0;
}

STATIC_DECLAR int RemoveSkillMenu_OnDraw(struct MenuProc * menu, struct MenuItemProc * item)
{
    int sid = 0xFFFF;
    
    if (MENU_SKILL_INDEX(item->def) > UNIT_RAM_SKILLS_LEN - 1)
    {
        struct Unit * targetUnit = GetUnit(gBattleTarget.unit.index);
#if defined(SID_PredationPlus) && (COMMON_SKILL_VALID(SID_PredationPlus))
        if (SkillTester(gActiveUnit, SID_PredationPlus) && gBattleActorGlobalFlag.enemy_defeated)
        {
            sid = GET_SKILL(targetUnit, gActionData.unk0A);
        }
#endif

#if defined(SID_Predation) && (COMMON_SKILL_VALID(SID_Predation))
        if (SkillTester(gActiveUnit, SID_Predation) && gBattleActorGlobalFlag.enemy_defeated)
        {
            sid = GET_SKILL(targetUnit, gActionData.unk08);
        }
#endif

#if defined(SID_SkillSwap) && (COMMON_SKILL_VALID(SID_SkillSwap))
        if (SkillTester(gActiveUnit, SID_SkillSwap) && gBattleActorGlobalFlag.enemy_defeated)
        {
            sid = GET_SKILL(targetUnit, gActionData.unk0A);
        }
#endif

        if (sid == 0xFFFF)
        {
            if (gBattleActorGlobalFlag.enemy_defeated)
                sid = GET_SKILL(targetUnit, MENU_SKILL_INDEX(item->def) - UNIT_RAM_SKILLS_LEN);
            // If using skill scroll
            else
            {
                int slot = gActionData.itemSlotIndex;
                int item = gActiveUnit->items[slot];
                int itemIndex = ITEM_INDEX(item);
#ifdef CONFIG_ITEM_INDEX_SKILL_SCROLL_1
                if (itemIndex == CONFIG_ITEM_INDEX_SKILL_SCROLL_1)
                    sid = ITEM_USES(item);
#endif
#ifdef CONFIG_ITEM_INDEX_SKILL_SCROLL_2
                if (itemIndex == CONFIG_ITEM_INDEX_SKILL_SCROLL_2)
                    sid = ITEM_USES(item) + 0xFF;
#endif
#ifdef CONFIG_ITEM_INDEX_SKILL_SCROLL_3
                if (itemIndex == CONFIG_ITEM_INDEX_SKILL_SCROLL_3)
                    sid = ITEM_USES(item) + 0x1FF;
#endif
#ifdef CONFIG_ITEM_INDEX_SKILL_SCROLL_4
                if (itemIndex == CONFIG_ITEM_INDEX_SKILL_SCROLL_4)
                    sid = ITEM_USES(item) + 0x2FF;
#endif
            }
        }

        Text_SetColor(&item->text, TEXT_COLOR_SYSTEM_GOLD);
    }
    else
    {
        sid = GET_SKILL(gActiveUnit, MENU_SKILL_INDEX(item->def));
        Text_SetColor(&item->text, TEXT_COLOR_SYSTEM_WHITE);
    }

    Text_DrawString(&item->text, GetSkillNameStr(sid));
    DrawIcon(
        TILEMAP_LOCATED(gBG0TilemapBuffer, item->xTile, item->yTile),
        SKILL_ICON(sid), 0x4000);

    PutText(
        &item->text,
        TILEMAP_LOCATED(gBG0TilemapBuffer, item->xTile + 2, item->yTile));

    BG_EnableSyncByMask(BG0_SYNC_BIT);
    return 0;
}

#if defined(SID_Predation) && (COMMON_SKILL_VALID(SID_Predation))
STATIC_DECLAR void PredationSkillRemove(void) 
{
    struct Unit * targetUnit = GetUnit(gBattleTarget.unit.index);

    // Remove using the full 10-bit value.
    RemoveSkill(gActiveUnit, GET_SKILL(gActiveUnit, gActionData.unk08));
    AddSkill(gActiveUnit, GET_SKILL(targetUnit, 0));
    
    // Build popup item with the full 10-bit skill ID.
    SetPopupItem((GET_SKILL(gActiveUnit, gActionData.unk08) << 8) | 
                 GET_SKILL_SCROLL_INDEX(GET_SKILL(gActiveUnit, gActionData.unk08)));
}
#endif

#if defined(SID_PredationPlus) && (COMMON_SKILL_VALID(SID_PredationPlus))
STATIC_DECLAR void PredationPlusSkillRemove(void) 
{
    struct Unit * targetUnit = GetUnit(gBattleTarget.unit.index);

   // Remove using the full 10-bit value.
    RemoveSkill(gActiveUnit, GET_SKILL(gActiveUnit, gActionData.unk08));
    AddSkill(gActiveUnit, GET_SKILL(targetUnit, 0));
    
    // Build popup item with the full 10-bit skill ID.
    SetPopupItem((GET_SKILL(gActiveUnit, gActionData.unk08) << 8) | 
                 GET_SKILL_SCROLL_INDEX(GET_SKILL(gActiveUnit, gActionData.unk08)));
}
#endif

STATIC_DECLAR void PredationTryAddSkill() 
{
    struct Unit * targetUnit = GetUnit(gBattleTarget.unit.index);

    AddSkill(gActiveUnit, GET_SKILL(targetUnit, 0));
    
    // Build popup item with the full 10-bit skill ID.
    SetPopupItem((GET_SKILL(gActiveUnit, gActionData.unk08) << 8) | 
                 GET_SKILL_SCROLL_INDEX(GET_SKILL(gActiveUnit, gActionData.unk08)));
}

STATIC_DECLAR u8 RemoveSkillMenu_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{

    SetItemUseAction(gActiveUnit);
    gActionData.unk08 = MENU_SKILL_INDEX(item->def);
    gEventSlots[EVT_SLOT_7] = 0xFFFF;

#if defined(SID_PredationPlus) && (COMMON_SKILL_VALID(SID_PredationPlus))
    if (SkillTester(gActiveUnit, SID_PredationPlus) && gBattleActorGlobalFlag.enemy_defeated)
    {
        PredationPlusSkillRemove();
        return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
    }
#endif

#if defined(SID_Predation) && (COMMON_SKILL_VALID(SID_Predation))
    if (SkillTester(gActiveUnit, SID_Predation) && gBattleActorGlobalFlag.enemy_defeated)
    {
        PredationSkillRemove();
        return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR; 
    }
#endif

    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

STATIC_DECLAR u8 PredationSkillMenu_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{    
    struct Unit * targetUnit = GetUnit(gBattleTarget.unit.index);

    /* Not sure what unk0A is used for, so this assignment might need to change if the variable is important */
    gActionData.unk0A = MENU_SKILL_INDEX(item->def);

    if (GET_SKILL(gActiveUnit, UNIT_RAM_SKILLS_LEN - 1) == 0)
    {  
        PredationTryAddSkill();
    } 
    else
    {
        SetPopupItem((GET_SKILL(targetUnit, gActionData.unk0A) << 8) | GET_SKILL_SCROLL_INDEX(GET_SKILL(targetUnit, gActionData.unk0A)));
        EndSubtitleHelp();

        StartSubtitleHelp(
            StartOrphanMenu(&RemoveSkillMenuDef),
            GetStringFromIndex(MSG_RemoveSkillSubtitle)
        );
    }

    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}