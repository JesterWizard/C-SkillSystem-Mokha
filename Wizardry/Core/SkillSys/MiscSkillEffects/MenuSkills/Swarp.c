#include "common-chax.h"
#include "kernel-lib.h"
#include "map-anims.h"
#include "skill-system.h"
#include "event-rework.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "unit-expa.h"
#include "action-expa.h"
#include "strmag.h"

#ifndef CONFIG_UNIT_ACTION_EXPA_ExecSkill
    #define CONFIG_UNIT_ACTION_EXPA_ExecSkill 20
#endif

#if defined(SID_Swarp) && (COMMON_SKILL_VALID(SID_Swarp))

void ForEachUnitInMagBy2Range(void(*func)(struct Unit* unit));

void TryAddUnitToSwarpTargetList(struct Unit* unit) {

    bool proceed = true;

    if (!AreUnitsAllied(gSubjectUnit->index, unit->index))
        proceed = false;

#if defined(SID_Wrestler) && (COMMON_SKILL_VALID(SID_Wrestler))
    if (SkillTester(gSubjectUnit, SID_Wrestler))
        proceed = true;
#endif

    if (!proceed)
        return;

// If the target unit has anchor they cannot be moved
#if defined(SID_Anchor) && (COMMON_SKILL_VALID(SID_Anchor))
    if (SkillTester(unit, SID_Anchor))
        return;
#endif

    AddTarget(unit->xPos, unit->yPos, unit->index, 0);

    return;
}

void MakeTargetListForSwarp(struct Unit* unit) {

    BmMapFill(gBmMapRange, 0);

/* Boost the range of this unit's movement skill */
#if defined(SID_Domain) && (COMMON_SKILL_VALID(SID_Domain))
    int x = unit->xPos;
    int y = unit->yPos;
    
    if (SkillTester(unit, SID_Domain))
    {
        MapAddInRange(x, y, (GetUnitMagic(unit) / 2) + SKILL_EFF0(SID_Domain), 1);
        ForEachUnitInRange(TryAddUnitToSwarpTargetList);
    }
    else
        ForEachUnitInMagBy2Range(TryAddUnitToSwarpTargetList);
#endif

    return;
}

u8 Swarp_Usability(const struct MenuItemDef * def, int number)
{
    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

    if (!HasSelectTarget(gActiveUnit, MakeTargetListForSwarp))
        return MENU_DISABLED;

    return MENU_ENABLED;
}

STATIC_DECLAR void PrepareMenuPositionSwarp(void)
{
    EndAllMus();
    RefreshUnitSprites();
}

static void set_target_unit(void)
{
    struct Unit * unit_tar = GetUnit(gActionData.targetIndex);
    gEventSlots[EVT_SLOT_2] = unit_tar->index;
}

static void set_actor_unit(void)
{
    gEventSlots[EVT_SLOT_2] = gActiveUnit->index;
}

static void set_position(void) 
{
    struct Unit * unita = gActiveUnit;
    struct Unit * unitb = GetUnit(gActionData.targetIndex);

    int x = unita->xPos;
    int y = unita->yPos;

    unita->xPos = unitb->xPos;
    unita->yPos = unitb->yPos;

    gActionData.xMove = unitb->xPos;
    gActionData.yMove = unitb->yPos;

    unitb->xPos = x;
    unitb->yPos = y;
}

static u8 Swarp_OnSelectTarget(ProcPtr proc, struct SelectTarget * target)
{
    gActionData.targetIndex = target->uid;

    gActionData.xOther = target->x;
    gActionData.yOther = target->y;

    HideMoveRangeGraphics();

    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);

    gActionData.unk08 = SID_Swarp;
    gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

    return TARGETSELECTION_ACTION_ENDFAST | TARGETSELECTION_ACTION_END | TARGETSELECTION_ACTION_SE_6A | TARGETSELECTION_ACTION_CLEARBGS;
}

u8 Swarp_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{
    if (item->availability == MENU_DISABLED)
    {
        MenuFrozenHelpBox(menu, MSG_No_Allies);
        return MENU_ACT_SND6B;
    }

    ClearBg0Bg1();

    MakeTargetListForSwarp(gActiveUnit);
    BmMapFill(gBmMapMovement, -1);

    StartSubtitleHelp(
        NewTargetSelection_Specialized(&gSelectInfo_Rescue, Swarp_OnSelectTarget),
        GetStringFromIndex(MSG_SKILL_Common_Target));

    PlaySoundEffect(0x6A);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
}

STATIC_DECLAR const EventScr EventScr_MenuPositionSwarp[] = {

LABEL(0)
    SVAL(EVT_SLOT_B, SID_Swarp)
    CALL(EventScr_MuSkillAnim)
    ASMC(PrepareMenuPositionSwarp)
    ASMC(set_actor_unit)
    CALL(EventScr_UidWarpOUT)
    STAL(20)
    ASMC(set_target_unit)
    CALL(EventScr_UidFlushingOUT)
    STAL(60)
    ASMC(set_position)
    ASMC(set_target_unit)
    CALL(EventScr_UidFlushingIN)
    STAL(20)
    ASMC(set_actor_unit)
    CALL(EventScr_UidWarpIN)
    STAL(20)

LABEL(99)
    NOFADE
    ENDA
};

bool Action_Swarp(ProcPtr parent)
{
    KernelCallEvent(EventScr_MenuPositionSwarp, EV_EXEC_CUTSCENE, parent);
    return true;
}
#endif
