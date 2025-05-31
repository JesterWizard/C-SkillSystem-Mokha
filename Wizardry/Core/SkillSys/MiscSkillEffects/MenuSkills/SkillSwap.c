#include "common-chax.h"
#include "map-anims.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "debuff.h"
#include "action-expa.h"
#include "icon-rework.h"
#include "jester_headers/miscellaenous.h"

#if defined(SID_SkillSwap) && (COMMON_SKILL_VALID(SID_SkillSwap))

#define SKILL_SLOT_COUNT UNIT_RAM_SKILLS_LEN
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
    
    // Process directional input.
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
        
    // Process A button: first press selects, second press trades.
    if (gKeyStatusPtr->newKeys & A_BUTTON) {
        if (proc->state == 0) {
            proc->selectedColumn = proc->activeSide;
            proc->selectedRow = (proc->activeSide == 0) ? proc->leftSelected : proc->rightSelected;
            proc->state = 1;
            proc->activeSide ^= 1; // switch side for target selection
        } else {
            // Swap the two skills (a swap even if target slot is empty)
            if (proc->selectedColumn == 0) {
                int leftSkill  = GET_SKILL(proc->leftUnit, proc->selectedRow);
                int rightSkill = GET_SKILL(proc->rightUnit, proc->rightSelected);
                SET_SKILL(proc->leftUnit, proc->selectedRow, rightSkill);
                SET_SKILL(proc->rightUnit, proc->rightSelected, leftSkill);
                ClearText(&proc->leftText[proc->selectedRow]);
                ClearText(&proc->rightText[proc->rightSelected]);
            } else {
                int rightSkill = GET_SKILL(proc->rightUnit, proc->selectedRow);
                int leftSkill  = GET_SKILL(proc->leftUnit, proc->leftSelected);
                SET_SKILL(proc->rightUnit, proc->selectedRow, leftSkill);
                SET_SKILL(proc->leftUnit, proc->leftSelected, rightSkill);
                ClearText(&proc->rightText[proc->selectedRow]);
                ClearText(&proc->leftText[proc->leftSelected]);
            }
            proc->state = 0;
            SkillSwapTradeMenu_Update(proc);
        }
    }
    
    // Process B button: cancel selection or exit.
    if (gKeyStatusPtr->newKeys & B_BUTTON) {
        if (proc->state == 1) {
            proc->state = 0;
            proc->activeSide ^= 1;
        } else {
            BG_Fill(gBG0TilemapBuffer, 0);
            BG_Fill(gBG1TilemapBuffer, 0);
            BG_Fill(gBG2TilemapBuffer, 0);
            BG_EnableSyncByMask(BG0_SYNC_BIT | BG1_SYNC_BIT | BG2_SYNC_BIT);
            Proc_End(proc);
            return;
        }
    }
    
    // Redraw the complete menus.
    SkillSwapTradeMenu_Update(proc);
    
    // Draw UI hand overlays and redraw overlapping lines.
    if (proc->state == 0) {
        // Normal state.
        if (proc->activeSide == 0) {
            int handX = (1 + 1) * 8;
            int handY = (2 + 1 + proc->leftSelected * 2) * 8;
            DisplayUiHand(handX, handY);
            {   // Redraw left active slot.
                int drawX = (1 + 1);
                int drawY = (2 + 1 + proc->leftSelected * 2);
                DrawSkillSwapEntry(gBG0TilemapBuffer, drawX, drawY,
                                  proc->leftUnit, proc->leftSelected, SKILL_ICON_PAL,
                                  &proc->leftText[proc->leftSelected]);
            }
            {   // Redraw corresponding right side slot.
                int drawX = (15 + 1);
                int drawY = (2 + 1 + proc->leftSelected * 2);
                DrawSkillSwapEntry(gBG0TilemapBuffer, drawX, drawY,
                                  proc->rightUnit, proc->leftSelected, SKILL_ICON_PAL,
                                  &proc->rightText[proc->leftSelected]);
            }
        } else {
            int handX = (15 + 1) * 8;
            int handY = (2 + 1 + proc->rightSelected * 2) * 8;
            DisplayUiHand(handX, handY);
            {   // Redraw right active slot.
                int drawX = (15 + 1);
                int drawY = (2 + 1 + proc->rightSelected * 2);
                DrawSkillSwapEntry(gBG0TilemapBuffer, drawX, drawY,
                                  proc->rightUnit, proc->rightSelected, SKILL_ICON_PAL,
                                  &proc->rightText[proc->rightSelected]);
            }
            {   // Redraw corresponding left side slot.
                int drawX = (1 + 1);
                int drawY = (2 + 1 + proc->rightSelected * 2);
                DrawSkillSwapEntry(gBG0TilemapBuffer, drawX, drawY,
                                  proc->leftUnit, proc->rightSelected, SKILL_ICON_PAL,
                                  &proc->leftText[proc->rightSelected]);
            }
        }
    } else {
        // State 1: A skill has been selected and the frozen UI hand should appear on the originating side.
        if (proc->selectedColumn == 0) {
            int frozenX = (1 + 1) * 8;
            int frozenY = (2 + 1 + proc->selectedRow * 2) * 8;
            DisplayFrozenUiHand(frozenX, frozenY);
            {   // Redraw frozen left slot.
                int drawX = (1 + 1);
                int drawY = (2 + 1 + proc->selectedRow * 2);
                DrawSkillSwapEntry(gBG0TilemapBuffer, drawX, drawY,
                                  proc->leftUnit, proc->selectedRow, SKILL_ICON_PAL,
                                  &proc->leftText[proc->selectedRow]);
            }
            {   // Redraw corresponding right slot.
                int drawX = (15 + 1);
                int drawY = (2 + 1 + proc->selectedRow * 2);
                DrawSkillSwapEntry(gBG0TilemapBuffer, drawX, drawY,
                                  proc->rightUnit, proc->selectedRow, SKILL_ICON_PAL,
                                  &proc->rightText[proc->selectedRow]);
            }
        } else { // Selected column == 1.
            int frozenX = (15 + 1) * 8;
            int frozenY = (2 + 1 + proc->selectedRow * 2) * 8;
            DisplayFrozenUiHand(frozenX, frozenY);
            {   // Redraw frozen right slot.
                int drawX = (15 + 1);
                int drawY = (2 + 1 + proc->selectedRow * 2);
                DrawSkillSwapEntry(gBG0TilemapBuffer, drawX, drawY,
                                  proc->rightUnit, proc->selectedRow, SKILL_ICON_PAL,
                                  &proc->rightText[proc->selectedRow]);
            }
            {   // Redraw corresponding left slot.
                int drawX = (1 + 1);
                int drawY = (2 + 1 + proc->selectedRow * 2);
                DrawSkillSwapEntry(gBG0TilemapBuffer, drawX, drawY,
                                  proc->leftUnit, proc->selectedRow, SKILL_ICON_PAL,
                                  &proc->leftText[proc->selectedRow]);
            }
        }
        // Also redraw the current active selection overlay.
        if (proc->activeSide == 0) {
            int handX = (1 + 1) * 8;
            int handY = (2 + 1 + proc->leftSelected * 2) * 8;
            DisplayUiHand(handX, handY);
            {
                int drawX = (1 + 1);
                int drawY = (2 + 1 + proc->leftSelected * 2);
                DrawSkillSwapEntry(gBG0TilemapBuffer, drawX, drawY,
                                  proc->leftUnit, proc->leftSelected, SKILL_ICON_PAL,
                                  &proc->leftText[proc->leftSelected]);
            }
        } else {
            int handX = (15 + 1) * 8;
            int handY = (2 + 1 + proc->rightSelected * 2) * 8;
            DisplayUiHand(handX, handY);
            {
                int drawX = (15 + 1);
                int drawY = (2 + 1 + proc->rightSelected * 2);
                DrawSkillSwapEntry(gBG0TilemapBuffer, drawX, drawY,
                                  proc->rightUnit, proc->rightSelected, SKILL_ICON_PAL,
                                  &proc->rightText[proc->rightSelected]);
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

u8 SkillSwap_Usability(const struct MenuItemDef * def, int number)
{
    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

    if (!HasSelectTarget(gActiveUnit, MakeTargetListForAdjacentSameFaction))
		return MENU_NOTSHOWN;

    return MENU_ENABLED;
}

static u8 SkillSwap_OnSelectTarget(ProcPtr proc, struct SelectTarget * target)
{
    gActionData.targetIndex = target->uid;

    gActionData.xOther = target->x;
    gActionData.yOther = target->y;

    HideMoveRangeGraphics();

    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);

    gActionData.unk08 = SID_SkillSwap;
    gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

    return TARGETSELECTION_ACTION_ENDFAST | TARGETSELECTION_ACTION_END | TARGETSELECTION_ACTION_SE_6A | TARGETSELECTION_ACTION_CLEARBGS;
}

u8 SkillSwap_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{
 if (item->availability == MENU_DISABLED)
    {
        MenuFrozenHelpBox(menu, MSG_No_Allies);
        return MENU_ACT_SND6B;
    }

    ClearBg0Bg1();

    MakeTargetListForAdjacentSameFaction(gActiveUnit);
    BmMapFill(gBmMapMovement, -1);

    StartSubtitleHelp(
        NewTargetSelection_Specialized(&gSelectInfo_PutTrap, SkillSwap_OnSelectTarget),
        GetStringFromIndex(MSG_SKILL_Common_Target));

    PlaySoundEffect(0x6A);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
}

static void callback_anim(ProcPtr proc)
{
}

static void callback_exec(ProcPtr proc)
{	
    struct Unit * unit_tar = GetUnit(gActionData.targetIndex);
    StartSkillSwapTradeMenu(gActiveUnit, unit_tar);
}

bool Action_SkillSwap(ProcPtr parent)
{
	NewMuSkillAnimOnActiveUnit(gActionData.unk08, callback_anim, callback_exec);
	return true;
}
#endif