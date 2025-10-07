#include "common-chax.h"
#include "item-sys.h"
#include "battle-system.h"
#include "constants/texts.h"
#include "jester_headers/custom-functions.h"
#include "bmitemuse.h"
#include "bmtarget.h"
#include "bm.h"
#include "proc.h"

// Called when the rewarp tile selection UI starts (kept for compatibility)
void RewarpMapSelect_Init(ProcPtr proc) {
    StartSubtitleHelp(proc, GetStringFromIndex(MSG_ITEM_REWARP_STAFF_SUBTITLE));
}

// Cursor switch hook (not required when using the warp square-select proc)
u8 RewarpMapSelect_SwitchIn(ProcPtr proc, struct SelectTarget* target) {
    return 0;
}

const struct SelectInfo gSelectInfo_RewarpTile =
{
    .onInit = RewarpMapSelect_Init,
    .onEnd = SubtitleMapSelect_End,
    .onSwitchIn = RewarpMapSelect_SwitchIn,
    .onCancel = GenericSelection_BackToUM_CamWait,
};

void ForEachPosInMagBy2Range(void(*func)(int x, int y)) {
    int x = gSubjectUnit->xPos;
    int y = gSubjectUnit->yPos;
    InitTargets(x, y);

    MapAddInRange(x, y, GetUnitMagBy2Range(gSubjectUnit), 1);
    MapAddInRange(x, y, 0, -1);

    ForEachPosInRange(func);
}

void TryAddToRewarpTargetList(int x, int y) {
    if (gBmMapUnit[y][x] != 0)
        return;

    if (!CanUnitCrossTerrain(gSubjectUnit, gBmMapTerrain[y][x]))
        return;

    AddTarget(x, y, 0, 0);
}

void MakeTargetListForRewarp(struct Unit* unit) {
    gSubjectUnit = unit;

    BmMapFill(gBmMapRange, 0);

    ForEachPosInMagBy2Range(TryAddToRewarpTargetList);
}

u8 RewarpOnSelectTarget(ProcPtr proc, struct SelectTarget* target)
{
    EndTargetSelection(proc);

    gActionData.xOther = target->x;
    gActionData.yOther = target->y;

    // ensure both actor and target are set (rewarp moves the actor)
    gActionData.subjectIndex  = gActiveUnit->index;
    gActionData.targetIndex   = gActiveUnit->index;

    SetStaffUseAction(gActiveUnit);

    return TARGETSELECTION_ACTION_ENDFAST | TARGETSELECTION_ACTION_END | TARGETSELECTION_ACTION_SE_6A | TARGETSELECTION_ACTION_CLEARBGS;
}

void DoUseRewarpStaff(struct Unit * unit)
{
    // Ensure both targetIndex and subjectIndex point to the staff user (active unit)
    gActionData.targetIndex  = unit->index;
    gActionData.subjectIndex = unit->index;

    // Start the same square-select warp proc used by vanilla Warp staff.
    // This will call FillWarpRangeMap(...) and DisplayMoveRangeGraphics(1).
    Proc_Start(gProcScr_SquareSelectWarp, PROC_TREE_3);

    PlaySoundEffect(SONG_SE_SYS_WINDOW_SELECT1);
}

/* ExecRewarpStaff: mirror vanilla ExecWarpStaff so animations and post-warp proc run.
   This uses the battle/item effect flow that the engine expects (BattleInitItemEffect, etc.)
*/
void ExecRewarpStaff(ProcPtr proc)
{
    // Initialize the item effect state for the actor and the target, so battle system
    // knows we're performing a staff effect.
    BattleInitItemEffect(gActiveUnit, gActionData.itemSlotIndex);

    BattleInitItemEffectTarget(gActiveUnit);

    // Update the model position for the target unit to the selected tile so that the original position is made empty when the effect runs
    gActiveUnit->xPos = gActionData.xOther;
    gActiveUnit->yPos = gActionData.yOther;

    // Update the action data move coordinates so that the unit actually moves to these new coordinates
    gActionData.xMove = gActionData.xOther;
    gActionData.yMove = gActionData.yOther;

    // Populate the battle-target scratch fields used by the animations/system
    gBattleTarget.changeHP = gActionData.xOther;
    gBattleTarget.changePow = gActionData.yOther;
    
    BattleApplyItemEffect(proc);

    /*
    ** BUG - The unit's active sprite overlays on top of their idle sprite when the teleport effect is in progress
    ** Turning off the map animations for warp to deal with it for now
    **
    // BeginBattleAnimations();

    Proc_StartBlocking(ProcScr_PostWarpStaffAction, proc);

    return;
}


/* IER hooks (ports) */
bool IER_Usability_Rewarp(struct Unit * unit, int item)
{
    return HasSelectTarget(unit, MakeTargetListForRewarp);
}

void IER_Effect_Rewarp(struct Unit *unit, int item)
{
    DoUseRewarpStaff(unit);
}

void IER_Action_Rewarp(ProcPtr proc, struct Unit *unit, int item)
{
    ExecRewarpStaff(proc);
}