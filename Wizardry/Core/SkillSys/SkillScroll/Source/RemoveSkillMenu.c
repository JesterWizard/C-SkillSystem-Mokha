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

// You can change this constant to however many skill slots you support.
#define SKILL_SLOT_COUNT UNIT_RAM_SKILLS_LEN

#ifdef CONFIG_TURN_ON_ALL_SKILLS
    // Choose the proper scroll index based on the high byte of the skill id.
    #define GET_SKILL_SCROLL_INDEX(sid) (((sid) > 0x2FF) ? CONFIG_ITEM_INDEX_SKILL_SCROLL_4 : \
                                        (((sid) > 0x1FF) ? CONFIG_ITEM_INDEX_SKILL_SCROLL_3 : \
                                        (((sid) > 0x0FF) ? CONFIG_ITEM_INDEX_SKILL_SCROLL_2 : \
                                                           CONFIG_ITEM_INDEX_SKILL_SCROLL_1)))
#else
    #define GET_SKILL_SCROLL_INDEX(sid) CONFIG_ITEM_INDEX_SKILL_SCROLL_1
#endif

#define SKILL_ICON_PAL TILEREF(0, STATSCREEN_BGPAL_ITEMICONS)

// A helper to draw a skill entry – it draws the skill name and its icon.
static void DrawSkillSwapEntry(u16 *tilemap, int xTile, int yTile,
                               struct Unit *unit, int slot, u16 palette, struct Text *text)
{
    int sid = GET_SKILL(unit, slot);

    // Do not clear here – it’s already cleared in the update routine.
    Text_SetParams(text, 0, TEXT_COLOR_SYSTEM_GOLD);
    if (EQUIP_SKILL_VALID(sid)) {
        Text_DrawString(text, GetSkillNameStr(sid));
        DrawIcon(TILEMAP_LOCATED(tilemap, xTile, yTile), SKILL_ICON(sid), palette);
    }
    // Otherwise, leave the text blank.

    // Commit the text into the tilemap at an offset.
    PutText(text, TILEMAP_LOCATED(tilemap, xTile + 2, yTile));
}

// This proc structure stores both units and selection data.
// Proc structure now holds additional fields for a two‐step selection.
struct SkillSwapTradeMenuProc {
    PROC_HEADER;
    struct Unit *leftUnit;   // active unit
    struct Unit *rightUnit;  // target unit
    int leftSelected;        // current slot in left menu
    int rightSelected;       // current slot in right menu
    int activeSide;          // 0 = left active, 1 = right active
    int state;               // 0 = no skill selected; 1 = waiting for target selection
    int selectedColumn;      // the column (0 or 1) from which the skill was first selected
    int selectedRow;         // the row index from the selected side
    // Persistent text buffers for each skill entry:
    struct Text leftText[SKILL_SLOT_COUNT];
    struct Text rightText[SKILL_SLOT_COUNT];
};

// A simple implementation that draws a rectangular border using the given color.
static void DrawUiBox(u16 *tilemap, int x, int y, int width, int height, int color)
{
    int i;
    // Draw top and bottom borders.
    for (i = 0; i < width; i++) {
        tilemap[TILEMAP_INDEX(x + i, y)] = color;
        tilemap[TILEMAP_INDEX(x + i, y + height - 1)] = color;
    }
    // Draw left and right borders.
    for (i = 1; i < height - 1; i++) {
        tilemap[TILEMAP_INDEX(x, y + i)] = color;
        tilemap[TILEMAP_INDEX(x + width - 1, y + i)] = color;
    }
}

static void SkillSwapTradeMenu_Update(struct SkillSwapTradeMenuProc * proc)
{
    // Clear the BG (adjust BG0 as needed).
    BG_Fill(gBG0TilemapBuffer, 0);

    // Define positions and sizes for the two menus.
    int leftX = 1,  leftY = 2,  menuWidth = 14, menuHeight = (SKILL_SLOT_COUNT + 1) * 2;
    int rightX = 15, rightY = 2;

    // Draw outlines (using a UI frame-drawing helper; if not available, you can draw rectangles manually).
    DrawUiFrame2(leftX, leftY, menuWidth, menuHeight, 0);
    DrawUiFrame2(rightX, rightY, menuWidth, menuHeight, 0);

    // Draw left unit's skill list.
    for (int i = 0; i < SKILL_SLOT_COUNT; i++) {
        int drawX = leftX + 1;
        int drawY = leftY + 1 + (i * 2);
        DrawSkillSwapEntry(gBG0TilemapBuffer, drawX, drawY, proc->leftUnit, i, 0x4000, &proc->leftText[i]);
    }

    // Draw right unit's skill list.
    for (int i = 0; i < SKILL_SLOT_COUNT; i++) {
        int drawX = rightX + 1;
        int drawY = rightY + 1 + (i * 2);
        DrawSkillSwapEntry(gBG0TilemapBuffer, drawX, drawY, proc->rightUnit, i, 0x4000, &proc->rightText[i]);
    }

    // Draw highlight boxes.
    if (proc->state == 0) {
        // Only the active menu shows a highlight.
        if (proc->activeSide == 0)
            DrawUiBox(gBG0TilemapBuffer, leftX, leftY + 1 + (proc->leftSelected * 2), menuWidth, 2, 0);
        else
            DrawUiBox(gBG0TilemapBuffer, rightX, rightY + 1 + (proc->rightSelected * 2), menuWidth, 2, 0);
    } else {
        // In state 1, show the frozen hand on the originally selected skill
        // and a regular highlight on the active menu.
        if (proc->selectedColumn == 0) {
            // Draw frozen highlight on left menu.
            DrawUiBox(gBG0TilemapBuffer, leftX, leftY + 1 + (proc->selectedRow * 2), menuWidth, 2, 1);
        } else {
            DrawUiBox(gBG0TilemapBuffer, rightX, rightY + 1 + (proc->selectedRow * 2), menuWidth, 2, 1);
        }
        // Then draw the active highlight on the current active menu.
        if (proc->activeSide == 0)
            DrawUiBox(gBG0TilemapBuffer, leftX, leftY + 1 + (proc->leftSelected * 2), menuWidth, 2, 0);
        else
            DrawUiBox(gBG0TilemapBuffer, rightX, rightY + 1 + (proc->rightSelected * 2), menuWidth, 2, 0);
    }

    BG_EnableSyncByMask(BG0_SYNC_BIT);
}

static void SkillSwapTradeMenu_OnLoop(struct SkillSwapTradeMenuProc *proc)
{
    u16 keys = gKeyStatusPtr->repeatedKeys;

    // Process input: move selections, switch active side, etc.
    if (keys & DPAD_UP) {
        if (proc->activeSide == 0 && proc->leftSelected > 0)
            proc->leftSelected--;
        else if (proc->activeSide == 1 && proc->rightSelected > 0)
            proc->rightSelected--;
    }
    if (keys & DPAD_DOWN) {
        if (proc->activeSide == 0 && proc->leftSelected < SKILL_SLOT_COUNT - 1)
            proc->leftSelected++;
        else if (proc->activeSide == 1 && proc->rightSelected < SKILL_SLOT_COUNT - 1)
            proc->rightSelected++;
    }
    if (keys & (DPAD_LEFT | DPAD_RIGHT))
        proc->activeSide ^= 1;

    // Handle A: first press selects, second press trades.
    if (gKeyStatusPtr->newKeys & A_BUTTON) {
        if (proc->state == 0) {
            proc->selectedColumn = proc->activeSide;
            proc->selectedRow = (proc->activeSide == 0) ? proc->leftSelected : proc->rightSelected;
            proc->state = 1;
            proc->activeSide ^= 1; // switch side for target selection
        } else {
            if (proc->selectedColumn == 0) {
                int leftSkill = GET_SKILL(proc->leftUnit, proc->selectedRow);
                int rightSkill = GET_SKILL(proc->rightUnit, proc->rightSelected);
                SET_SKILL(proc->leftUnit, proc->selectedRow, rightSkill);
                SET_SKILL(proc->rightUnit, proc->rightSelected, leftSkill);
            } else {
                int leftSkill = GET_SKILL(proc->leftUnit, proc->leftSelected);
                int rightSkill = GET_SKILL(proc->rightUnit, proc->selectedRow);
                SET_SKILL(proc->leftUnit, proc->leftSelected, rightSkill);
                SET_SKILL(proc->rightUnit, proc->selectedRow, leftSkill);
            }
            proc->state = 0;
            // Immediately refresh the menus so traded icons update.
            SkillSwapTradeMenu_Update(proc);
        }
    }

    // Handle B: cancel selection or exit menu.
    if (gKeyStatusPtr->newKeys & B_BUTTON) {
        if (proc->state == 1) {
            proc->state = 0;
            proc->activeSide ^= 1;
        } else {
            Proc_End(proc);
            return;
        }
    }

    // Redraw the complete menus.
    SkillSwapTradeMenu_Update(proc);

    // Now draw the UI hand overlays.
    if (proc->state == 0) {
        if (proc->activeSide == 0) {
            int handX = (1 + 1) * 8;
            int handY = (2 + 1 + proc->leftSelected * 2) * 8;
            DisplayUiHand(handX, handY);
            // Redraw the left active slot icon and text (over the UI hand)
            {
                int drawX = 1 + 1;
                int drawY = 2 + 1 + proc->leftSelected * 2;
                // First, redraw text by calling DrawSkillSwapEntry using the persistent left text.
                DrawSkillSwapEntry(gBG0TilemapBuffer, drawX, drawY,
                                proc->leftUnit, proc->leftSelected, SKILL_ICON_PAL, &proc->leftText[proc->leftSelected]);
            }
            // --- NEW: Redraw the corresponding right side line to remove any overlap ---
            if (proc->leftSelected < SKILL_SLOT_COUNT) {
                int drawX = 15 + 1;
                int drawY = 2 + 1 + proc->leftSelected * 2;
                DrawSkillSwapEntry(gBG0TilemapBuffer, drawX, drawY,
                                   proc->rightUnit, proc->leftSelected, SKILL_ICON_PAL,
                                   &proc->rightText[proc->leftSelected]);
            }
        } else {
            int handX = (15 + 1) * 8;
            int handY = (2 + 1 + proc->rightSelected * 2) * 8;
            DisplayUiHand(handX, handY);
            {
                int drawX = (15 + 1);
                int drawY = 2 + 1 + proc->rightSelected * 2;
                DrawSkillSwapEntry(gBG0TilemapBuffer, drawX, drawY,
                                proc->rightUnit, proc->rightSelected, SKILL_ICON_PAL, &proc->rightText[proc->rightSelected]);
            }
        }
    } else {
        // In state 1, similarly redraw the frozen slot text/icon as well as the active slot.
        if (proc->selectedColumn == 0) {
            int frozenX = (1 + 1) * 8;
            int frozenY = (2 + 1 + proc->selectedRow * 2) * 8;
            DisplayFrozenUiHand(frozenX, frozenY);
            {
                int drawX = (1 + 1);
                int drawY = 2 + 1 + proc->selectedRow * 2;
                DrawSkillSwapEntry(gBG0TilemapBuffer, drawX, drawY,
                                proc->leftUnit, proc->selectedRow, SKILL_ICON_PAL, &proc->leftText[proc->selectedRow]);
            }
        } else {
            int frozenX = (15 + 1) * 8;
            int frozenY = (2 + 1 + proc->selectedRow * 2) * 8;
            DisplayFrozenUiHand(frozenX, frozenY);
            {
                int drawX = (15 + 1);
                int drawY = 2 + 1 + proc->selectedRow * 2;
                DrawSkillSwapEntry(gBG0TilemapBuffer, drawX, drawY,
                                proc->rightUnit, proc->selectedRow, SKILL_ICON_PAL, &proc->rightText[proc->selectedRow]);
            }
        }
    // And redraw the normally active selection on top as well.
    if (proc->activeSide == 0) {
        int handX = (1 + 1) * 8;
        int handY = (2 + 1 + proc->leftSelected * 2) * 8;
        DisplayUiHand(handX, handY);
        {
            int drawX = (1 + 1);
            int drawY = 2 + 1 + proc->leftSelected * 2;
            DrawSkillSwapEntry(gBG0TilemapBuffer, drawX, drawY,
                               proc->leftUnit, proc->leftSelected, SKILL_ICON_PAL, &proc->leftText[proc->leftSelected]);
        }
    } else {
        int handX = (15 + 1) * 8;
        int handY = (2 + 1 + proc->rightSelected * 2) * 8;
        DisplayUiHand(handX, handY);
        {
            int drawX = (15 + 1);
            int drawY = 2 + 1 + proc->rightSelected * 2;
            DrawSkillSwapEntry(gBG0TilemapBuffer, drawX, drawY,
                               proc->rightUnit, proc->rightSelected, SKILL_ICON_PAL, &proc->rightText[proc->rightSelected]);
        }
    }
}
}

static const struct ProcCmd ProcScr_SkillSwapTradeMenu[] = {
    PROC_REPEAT(SkillSwapTradeMenu_OnLoop),
    PROC_END
};

void StartSkillSwapTradeMenu(struct Unit * leftUnit, struct Unit * rightUnit)
{
    struct SkillSwapTradeMenuProc *proc = Proc_StartBlocking(ProcScr_SkillSwapTradeMenu, Proc_Find(gProcScr_PlayerPhase));
    proc->leftUnit = leftUnit;
    proc->rightUnit = rightUnit;
    proc->leftSelected = 0;
    proc->rightSelected = 0;
    proc->activeSide = 0; // start with left active
    proc->state = 0;      // no skill selected yet
    proc->selectedColumn = -1;
    proc->selectedRow = -1;

    for (int i = 0; i < SKILL_SLOT_COUNT; i++) {
        // Allocate enough width for the skill name.
        InitText(&proc->leftText[i], 16);
        InitText(&proc->rightText[i], 16);
    }
}

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