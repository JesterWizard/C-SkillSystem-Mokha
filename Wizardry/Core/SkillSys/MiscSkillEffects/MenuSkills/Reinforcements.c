#include "common-chax.h"
#include "map-anims.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "unit-expa.h"
#include "action-expa.h"
#include "event-rework.h"
#include "bmtarget.h"
#include "weapon-range.h"

#if defined(SID_Reinforcements) && (COMMON_SKILL_VALID(SID_Reinforcements))

static void TryAddToReinforcementsTargetList(int x, int y) {
    if (gBmMapUnit[y][x] != 0) {
        return;
    }

    if (TerrainTable_MovCost_FlyNormal[gBmMapTerrain[y][x]] <= 0) {
        return;
    }

    AddTarget(x, y, 0, 0);

    return;
}

void ForEachEmptySpaceInRange(int range, void(*func)(int x, int y)) {
    int x = gSubjectUnit->xPos;
    int y = gSubjectUnit->yPos;

    InitTargets(x, y);

    for (int ix = x - range; ix <= x + range; ix++) {
        for (int iy = y - range; iy <= y + range; iy++) {
            if (ix < 0 || iy < 0 || ix >= gBmMapSize.x || iy >= gBmMapSize.y) {
                continue;
            }

            /* Calculate Manhattan distance */
            if (abs(ix - x) + abs(iy - y) <= range) {
                /* Exclude the starting position of the unit */
                if (ix == x && iy == y) {
                    continue;
                }
                if (gBmMapUnit[iy][ix] == 0) {
                    func(ix, iy);
                }
            }
        }
    }
}

static void MakeTargetListForReinforcements(struct Unit* unit) {
    BmMapFill(gBmMapRange, 0);

    ForEachEmptySpaceInRange(SKILL_EFF0(SID_Reinforcements), TryAddToReinforcementsTargetList);

    return;
}

u8 Reinforcements_Usability(const struct MenuItemDef * def, int number)
{
    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

    if (!HasSelectTarget(gActiveUnit, MakeTargetListForReinforcements))
        return MENU_DISABLED;

    return MENU_ENABLED;
}

int Reinforcements_Hover(struct MenuProc * menu, struct MenuItemProc * item)
{
    BmMapFill(gBmMapMovement, -1);
    BmMapFill(gBmMapRange, 0);
    int maxRange = SKILL_EFF0(SID_Reinforcements);
    MapAddInRange(gActiveUnit->xPos, gActiveUnit->yPos, maxRange, +1);
    DisplayMoveRangeGraphics(MOVLIMITV_MMAP_BLUE | MOVLIMITV_RMAP_GREEN);
    return 0;
}

int Reinforcements_Unhover(struct MenuProc * menu, struct MenuItemProc * menuItem)
{
    HideMoveRangeGraphics();
    return 0;
}

STATIC_DECLAR void PrepareMenuReinforcements(void)
{
    EndAllMus();
    RefreshUnitSprites();
}

/* 
   Helper: Summon a green cavalier unit at (x, y).
   You may need to adjust this routine to properly initialize the unit.
*/
static void SummonGreenCavalier(int x, int y)
{
    // Set up the unit definition for a green cavalier:
    gUnitDef1.charIndex       = CHARACTER_MESSENGER;
    gUnitDef1.classIndex      = CLASS_CAVALIER;
    gUnitDef1.autolevel       = TRUE;
    gUnitDef1.allegiance      = FACTION_ID_GREEN;
    gUnitDef1.level           = gActiveUnit->level;
    gUnitDef1.xPosition       = x;
    gUnitDef1.yPosition       = y;
    gUnitDef1.redaCount       = 0;
    gUnitDef1.redas           = NULL;
    gUnitDef1.genMonster      = FALSE;
    gUnitDef1.itemDrop        = FALSE;

    // Summon the unit.
    LoadUnits(&gUnitDef1);
}

/*
    This function spawns reinforcements:
    - One in the selected tile (gActionData.xOther, gActionData.yOther)
    - Two additional ones in an available adjacent (cardinal) space if possible.
*/
static void spawn_reinforcements(void)
{
    int count = 0;
    int mainX = gActionData.xOther;
    int mainY = gActionData.yOther;

    // Summon the main reinforcement unit in the selected space
    SummonGreenCavalier(mainX, mainY);
    count++;

    // Try to summon two additional units in adjacent cardinal spaces.
    int dx[4] = { -1, 1, 0, 0 };
    int dy[4] = {  0, 0,-1, 1 };

    for (int i = 0; i < 4 && count < 3; i++) {
         int tx = mainX + dx[i];
         int ty = mainY + dy[i];

         // Validate map boundaries.
         if (tx < 0 || ty < 0 || tx >= gBmMapSize.x || ty >= gBmMapSize.y)
             continue;
         // Check that the tile is empty and passable.
         if (gBmMapUnit[ty][tx] != 0)
             continue;
         if (TerrainTable_MovCost_FlyNormal[gBmMapTerrain[ty][tx]] <= 0)
             continue;

         SummonGreenCavalier(tx, ty);
         count++;
    }

    // Optionally refresh the unit sprites or map graphics after summoning.
    RefreshUnitSprites();
}

static void set_actor_unit(void)
{
    gEventSlots[EVT_SLOT_2] = gActiveUnit->index;
}

/* Remove the old set_position and move_again functions */
 
static u8 Reinforcements_OnSelectTarget(ProcPtr proc, struct SelectTarget * target)
{
    gActionData.xOther = target->x;
    gActionData.yOther = target->y;

    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);

    gActionData.unk08 = SID_Reinforcements;
    gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

    return TARGETSELECTION_ACTION_ENDFAST | TARGETSELECTION_ACTION_END | TARGETSELECTION_ACTION_SE_6A | TARGETSELECTION_ACTION_CLEARBGS;
}

u8 Reinforcements_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{
    if (item->availability == MENU_DISABLED)
    {
        MenuFrozenHelpBox(menu, MSG_SKILL_CommonFail);
        return MENU_ACT_SND6B;
    }

    /* Removes the menu while targeting */
    ClearBg0Bg1();

    MakeTargetListForReinforcements(gActiveUnit);

    StartSubtitleHelp(
        NewTargetSelection_Specialized(&gSelectInfo_PutTrap, Reinforcements_OnSelectTarget),
        GetStringFromIndex(0x87E));

    PlaySoundEffect(0x6A);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
}

STATIC_DECLAR const EventScr EventScr_MenuReinforcements[] = {

LABEL(0)
    CALL(EventScr_MuSkillAnim)
    ASMC(PrepareMenuReinforcements)
    ASMC(set_actor_unit)
    /* Instead of moving the active unit, we now summon reinforcements. */
    ASMC(spawn_reinforcements)
    LABEL(99)
    NOFADE
    ENDA
};


bool Action_Reinforcements(ProcPtr parent)
{
    KernelCallEvent(EventScr_MenuReinforcements, EV_EXEC_CUTSCENE, parent);
    return true;
}
#endif