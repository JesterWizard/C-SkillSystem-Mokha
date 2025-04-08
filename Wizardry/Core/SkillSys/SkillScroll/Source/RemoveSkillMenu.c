#include "common-chax.h"
#include "icon-rework.h"
#include "skill-system.h"
#include "battle-system.h"

STATIC_DECLAR const struct MenuItemDef RemoveSkillMenuItems[];
STATIC_DECLAR const struct MenuItemDef PredationSkillMenuItems[];
STATIC_DECLAR u8 RemoveSkillMenu_HelpBox(struct MenuProc * menu, struct MenuItemProc * item);
STATIC_DECLAR u8 PredationSkillMenu_HelpBox(struct MenuProc * menu, struct MenuItemProc * item);
STATIC_DECLAR u8 RemoveSkillMenu_OnCancel(struct MenuProc * menu, struct MenuItemProc * item);

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
    if (MENU_SKILL_INDEX(item->def) > 6)
    {
        // Use gBattleTarget's first skill for help.
        struct Unit * targetUnit = GetUnit(gBattleTarget.unit.index);
        StartHelpBox(
            item->xTile * 8,
            item->yTile * 8,
            GetSkillDescMsg(UNIT_RAM_SKILLS(targetUnit)[MENU_SKILL_INDEX(item->def) -  7])
        );
    }
    else
    {
        StartHelpBox(
            item->xTile * 8,
            item->yTile * 8,
            GetSkillDescMsg(UNIT_RAM_SKILLS(gActiveUnit)[MENU_SKILL_INDEX(item->def)])
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
        GetSkillDescMsg(UNIT_RAM_SKILLS(targetUnit)[MENU_SKILL_INDEX(item->def)])
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
// STATIC_DECLAR int PredationSkillMenu_OnDraw(struct MenuProc * menu, struct MenuItemProc * item);
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
    .onDraw = RemoveSkillMenu_OnDraw, \
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
    RemoveSkillMenuItem(5),
    RemoveSkillMenuItem(6),

    /* Enemy skills */
    RemoveSkillMenuItem(7),
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
    PredationSkillMenuItem(5),
    PredationSkillMenuItem(6),
    { 0 }
};

/* I'm making it return the menu definition by default for the sake of displaying prospective enemy skills */
STATIC_DECLAR u8 RemoveSkillMenu_Usability(const struct MenuItemDef * self, int number)
{
    // int sid = UNIT_RAM_SKILLS(gActiveUnit)[MENU_SKILL_INDEX(self)];
    // if (EQUIPE_SKILL_VALID(sid))
    //     return MENU_ENABLED;

    // return MENU_NOTSHOWN;

    return MENU_ENABLED;
}

STATIC_DECLAR u8 PredationSkillMenu_Usability(const struct MenuItemDef * self, int number)
{
    struct Unit * targetUnit = GetUnit(gBattleTarget.unit.index);

    int sid = UNIT_RAM_SKILLS(targetUnit)[MENU_SKILL_INDEX(self)];
    if (EQUIPE_SKILL_VALID(sid))
        return MENU_ENABLED;

    return MENU_NOTSHOWN;
}

STATIC_DECLAR int RemoveSkillMenu_OnDraw(struct MenuProc * menu, struct MenuItemProc * item)
{
    int sid;

    struct Unit * targetUnit = GetUnit(gBattleTarget.unit.index);
    sid = UNIT_RAM_SKILLS(targetUnit)[MENU_SKILL_INDEX(item->def)];
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

#if defined(SID_Predation) && (COMMON_SKILL_VALID(SID_Predation))
STATIC_DECLAR void PredationSkillRemove(void) 
{
    struct Unit * targetUnit = GetUnit(gBattleTarget.unit.index);

    RemoveSkill(gActiveUnit, gActiveUnit->supports[gActionData.unk08]);
    AddSkill(gActiveUnit, UNIT_RAM_SKILLS(targetUnit)[0]);
    SetPopupItem((gActiveUnit->supports[gActionData.unk08] << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL);
}
#endif

STATIC_DECLAR void PredationTryAddSkill() 
{
    struct Unit * targetUnit = GetUnit(gBattleTarget.unit.index);

    AddSkill(gActiveUnit, targetUnit->supports[gActionData.unk08]);
    SetPopupItem((gActiveUnit->supports[gActionData.unk08] << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL);
}

STATIC_DECLAR u8 RemoveSkillMenu_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{

    SetItemUseAction(gActiveUnit);
    gActionData.unk08 = MENU_SKILL_INDEX(item->def);

#if defined(SID_Predation) && (COMMON_SKILL_VALID(SID_Predation))
    if (SkillTester(gActiveUnit, SID_Predation) && gBattleActorGlobalFlag.enemy_defeated)
        PredationSkillRemove();
#endif

    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

STATIC_DECLAR u8 PredationSkillMenu_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{    
    struct Unit * targetUnit = GetUnit(gBattleTarget.unit.index);

    // SetItemUseAction(gActiveUnit);
    gActionData.unk08 = MENU_SKILL_INDEX(item->def);

    if (UNIT_RAM_SKILLS(gActiveUnit)[6] == 0)
        PredationTryAddSkill();
    else
    {
        SetPopupItem((targetUnit->supports[gActionData.unk08] << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL);
        // NewPopup_Simple(PopupScr_ObtainedSkill, 0x5A, 0, menu);
        Proc_StartBlocking(ProcScr_PredationSoftLock, menu);
    }

    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}