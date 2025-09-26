#include "common-chax.h"
#include "player_interface.h"
#include "jester_headers/custom-functions.h"

#define SECONDARY_GOAL_WIN_X 2
#define SECONDARY_GOAL_WIN_BG1_Y 0
#define SECONDARY_GOAL_WIN_BG0_Y 0
#define SECONDARY_GOAL_WIN_W 11
#define SECONDARY_GOAL_WIN_H 5

const struct ProcCmd gProcScr_SecondaryGoalWindow[] = {
    PROC_NAME("SECONDARY_GOAL"),
    PROC_15,
    PROC_YIELD,
    PROC_CALL(SecondaryGoalWindow_Init),
    PROC_REPEAT(SecondaryGoalWindow_Loop_Display),
    PROC_END,
};

void GetGoalWindowPosition(int* x, int* y) {
    int quadrant = GetCursorQuadrant(); // or get from goal proc if available
    *x = sPlayerInterfaceConfigLut[quadrant].xGoal;
    *y = sPlayerInterfaceConfigLut[quadrant].yGoal;
}

void DrawSecondaryGoalWindow(struct SecondaryGoalWindowProc *proc) {
    int x, y;
    int quadrant = GetCursorQuadrant();
    x = sPlayerInterfaceConfigLut[quadrant].xGoal;
    y = sPlayerInterfaceConfigLut[quadrant].yGoal + 6;

    // Clear area
    TileMap_FillRect(gUiTmScratchB + TILEMAP_INDEX(x, y), SECONDARY_GOAL_WIN_W, SECONDARY_GOAL_WIN_H, 0);
    TileMap_FillRect(gUiTmScratchA + TILEMAP_INDEX(x, y), SECONDARY_GOAL_WIN_W, SECONDARY_GOAL_WIN_H, 0);

    // Draw window frame (reuse GoalBox or use your own TSA as needed)
    CallARM_FillTileRect(gUiTmScratchB + TILEMAP_INDEX(x, y), gTSA_GoalBox_OneLine, TILEREF(0x0, 1));

    // Draw HP text
    ClearText(&proc->text);

    Text_InsertDrawNumberOrBlank(&proc->text, 0, TEXT_COLOR_SYSTEM_WHITE, GetUnitCurrentHp(gActiveUnit));
    Text_InsertDrawString(&proc->text, 0x10, TEXT_COLOR_SYSTEM_WHITE, "/");
    Text_InsertDrawNumberOrBlank(&proc->text, 0x30, TEXT_COLOR_SYSTEM_WHITE, GetUnitMaxHp(gActiveUnit));

    PutText(&proc->text, gUiTmScratchA + TILEMAP_INDEX(x + 1, y + 2));

    // Copy to BG buffers
    TileMap_CopyRect(
        gUiTmScratchB + TILEMAP_INDEX(x, y),
        gBG1TilemapBuffer + TILEMAP_INDEX(x, y),
        SECONDARY_GOAL_WIN_W, SECONDARY_GOAL_WIN_H
    );
    TileMap_CopyRect(
        gUiTmScratchA + TILEMAP_INDEX(x, y),
        gBG0TilemapBuffer + TILEMAP_INDEX(x, y),
        SECONDARY_GOAL_WIN_W, SECONDARY_GOAL_WIN_H
    );
    BG_EnableSyncByMask(BG0_SYNC_BIT | BG1_SYNC_BIT);
}

void SecondaryGoalWindow_Init(struct SecondaryGoalWindowProc *proc) {
    InitText(&proc->text, 8);
    proc->xCursor = proc->yCursor = proc->xCursorPrev = proc->yCursorPrev = -1;
    proc->unitIdPrev = 0;
}

void SecondaryGoalWindow_Loop_Display(struct SecondaryGoalWindowProc *proc) {
    if (!(gBmSt.gameStateBits & BM_FLAG_0) || Proc_Find(gProcScr_GoalDisplay) == NULL) {
        Proc_End(proc);
        return;
    }

    DrawSecondaryGoalWindow(proc);
}