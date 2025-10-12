#include "common-chax.h"
#include "kernel-lib.h"
#include "map-anims.h"
#include "skill-system.h"
#include "event-rework.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "unit-expa.h"
#include "action-expa.h"

#ifndef CONFIG_UNIT_ACTION_EXPA_ExecSkill
    #define CONFIG_UNIT_ACTION_EXPA_ExecSkill 20
#endif

#if defined(SID_DrawBack) && (COMMON_SKILL_VALID(SID_DrawBack))

struct Vec2u GetDrawBackCoord(int x1, int x2, int y1, int y2);
void TryDrawBackAllyToTargetList(struct Unit * unit);
void MakeDrawBackTargetListForAdjacentAlly(struct Unit * unit);
extern void ForEachAdjacentUnit(int x, int y, void (*)(struct Unit *));

static inline bool isPosInvalid(s8 x, s8 y)
{
    return ((x < 0) & (x > gBmMapSize.x) & (y < 0) & (y > gBmMapSize.y));
}

u8 DrawBack_Usability(const struct MenuItemDef * def, int number)
{
    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

    if (!HasSelectTarget(gActiveUnit, MakeDrawBackTargetListForAdjacentAlly))
        return MENU_DISABLED;

    return MENU_ENABLED;
}

static u8 DrawBack_OnSelectTarget(ProcPtr proc, struct SelectTarget * target)
{
    gActionData.targetIndex = target->uid;

    gActionData.xOther = target->x;
    gActionData.yOther = target->y;

    HideMoveRangeGraphics();

    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);

    gActionData.unk08 = SID_DrawBack;
    gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

    return TARGETSELECTION_ACTION_ENDFAST | TARGETSELECTION_ACTION_END | TARGETSELECTION_ACTION_SE_6A |
        TARGETSELECTION_ACTION_CLEARBGS;
}

u8 DrawBack_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{
    if (item->availability == MENU_DISABLED)
    {
        MenuFrozenHelpBox(menu, MSG_SKILL_DrawBack_FRtext);
        return MENU_ACT_SND6B;
    }

    ClearBg0Bg1();

    MakeDrawBackTargetListForAdjacentAlly(gActiveUnit);
    BmMapFill(gBmMapMovement, -1);

    StartSubtitleHelp(
        NewTargetSelection_Specialized(&gSelectInfo_PutTrap, DrawBack_OnSelectTarget),
        GetStringFromIndex(MSG_SKILL_Common_Target));

    PlaySoundEffect(0x6A);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
}

static void callback_anim(ProcPtr proc)
{
    PlaySoundEffect(0x269);
    Proc_StartBlocking(ProcScr_DanceringAnim, proc);

    BG_SetPosition(BG_0, -SCREEN_TILE_IX(gActiveUnit->xPos - 1), -SCREEN_TILE_IX(gActiveUnit->yPos - 2));
}

static void callback_exec(ProcPtr proc)
{
    gActiveUnit->state |= US_HAS_MOVED | US_CANTOING;
    // gActiveUnit->state &= ~US_CANTOING;
    struct Unit * targetUnit = GetUnit(gActionData.targetIndex);

    int x1 = gActiveUnit->xPos;
    int y1 = gActiveUnit->yPos;

    int x2 = targetUnit->xPos; // target

    int y2 = targetUnit->yPos; // target

    struct Vec2u dest = GetDrawBackCoord(x1, x2, y1, y2);

    targetUnit->xPos = gActiveUnit->xPos;
    targetUnit->yPos = gActiveUnit->yPos;

    gActionData.xMove = dest.x;
    gActionData.yMove = dest.y;
}

struct Vec2u GetDrawBackCoord(int x1, int x2, int y1, int y2)
{
    struct Vec2u result;
    result.x = x1;
    result.y = y1;
    // int dir = 0;
    if (x1 != x2)
    {
        if (x1 > x2)
        {
            // dir = MU_COMMAND_MOVE_RIGHT; // actor is on the right side of target, so move both of them right
            result.x = x1 + 1;
        }
        else if (x1 < x2)
        {
            // dir = MU_COMMAND_MOVE_LEFT;
            result.x = x1 - 1;
        }
    }
    else if (y1 != y2)
    {
        if (y1 > y2)
        {
            // dir = MU_COMMAND_MOVE_DOWN;
            result.y = y1 + 1;
        }
        else if (y1 < y2)
        {
            // dir = MU_COMMAND_MOVE_UP;
            result.y = y1 - 1;
        }
    }
    return result;
}

void TryDrawBackAllyToTargetList(struct Unit * unit)
{

    bool proceed = true;

    if (!AreUnitsAllied(gSubjectUnit->index, unit->index))
        proceed = false;

#if defined(SID_Wrestler) && (COMMON_SKILL_VALID(SID_Wrestler))
    if (SkillTester(gSubjectUnit, SID_Wrestler))
        proceed = true;
#endif

    if (!proceed)
        return;

    if (unit->state & US_RESCUED)
    {
        return;
    }
    if (unit == gSubjectUnit)
    {
        return;
    }

    int x1 = gSubjectUnit->xPos;
    int x2 = unit->xPos; // target
    int y1 = gSubjectUnit->yPos;
    int y2 = unit->yPos; // target

    struct Vec2u dest = GetDrawBackCoord(x1, x2, y1, y2);

    if (isPosInvalid(dest.x, dest.y))
    {
        return;
    }

    // if (gBmMapMovement[dest.y][dest.x] < 0xF) { // can we actually move there
    if (gBmMapUnit[dest.y][dest.x])
    {
        return; // dest sq is occupied
    }
    if (gBmMapHidden[dest.y][dest.x] & 1)
    {
        return; // hidden unit here
    }

    // If the target unit has anchor they cannot be moved
#if defined(SID_Anchor) && (COMMON_SKILL_VALID(SID_Anchor))
    if (SkillTester(unit, SID_Anchor))
        return;
#endif

    if (CanUnitCrossTerrain(gSubjectUnit, gBmMapTerrain[dest.y][dest.x]))
    { // can we actually move there
        if (CanUnitCrossTerrain(unit, gBmMapTerrain[y1][x1]))
        { // can target be pulled onto here?
            AddTarget(unit->xPos, unit->yPos, unit->index, 0);
        }
    }

    return;
}

void MakeDrawBackTargetListForAdjacentAlly(struct Unit * unit)
{
    InitTargets(0, 0);
    int x = unit->xPos;
    int y = unit->yPos;

    gSubjectUnit = unit;

    BmMapFill(gBmMapRange, 0);

/* Boost the range of this unit's movement skill */
#if defined(SID_Domain) && (COMMON_SKILL_VALID(SID_Domain))
    if (SkillTester(unit, SID_Domain))
    {
        MapAddInRange(x, y, 1 + SKILL_EFF0(SID_Domain), 1);
        ForEachUnitInRange(TryDrawBackAllyToTargetList);
    }
    else
        ForEachAdjacentUnit(x, y, TryDrawBackAllyToTargetList);

#else
    ForEachAdjacentUnit(x, y, TryDrawBackAllyToTargetList);
#endif

    return;
}

bool Action_DrawBack(ProcPtr parent)
{
    NewMuSkillAnimOnActiveUnit(gActionData.unk08, callback_anim, callback_exec);
    return true;
}
#endif