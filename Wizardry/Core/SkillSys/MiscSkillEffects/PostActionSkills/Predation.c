#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "battle-system.h"
#include "event-rework.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "strmag.h"

// STATIC_DECLAR const EventScr EventScr_PostActionPredation[] = {
//     EVBIT_MODIFY(0x4)
//     ASMC(PostActionPredationSkipMenuIfNotAlly)
//     BEQ(0, EVT_SLOT_C, EVT_SLOT_0)

//     TUTORIALTEXTBOXSTART
//     SVAL(EVT_SLOT_B, 0xffffffff)
//     TEXTSHOW(MSG_SkillEffect_Predation_Confirm)
//     TEXTEND
//     REMA
//     SVAL(EVT_SLOT_7, 0x1)
//     BNE(99, EVT_SLOT_C, EVT_SLOT_7)

// LABEL(0)
// #if defined(SID_Predation) && (COMMON_SKILL_VALID(SID_Predation))
//     SVAL(EVT_SLOT_B, SID_Predation)
//     CALL(EventScr_MuSkillAnim)
// #endif
//     ASMC(PreparePredation)

// LABEL(99)
//     ASMC(MapAnim_CommonEnd)
//     NOFADE
//     ENDA
// };

static void callback_anim(ProcPtr proc)
{
    PlaySoundEffect(0x269);
    Proc_StartBlocking(ProcScr_DanceringAnim, proc);

    BG_SetPosition(
        BG_0,
        -SCREEN_TILE_IX(gActiveUnit->xPos - 1),
        -SCREEN_TILE_IX(gActiveUnit->yPos - 2));
}

static void callback_exec(ProcPtr proc)
{
   // gActiveUnit->state |= US_HAS_MOVED | US_CANTOING;

   struct Unit * targetUnit = GetUnit(gBattleTarget.unit.index);

   /* The first learned skill in the target unit's struct */
   AddSkill(gActiveUnit, UNIT_RAM_SKILLS(targetUnit)[0]);

   SetPopupItem((targetUnit->supports[0] << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL);

   NewPopup_Simple(PopupScr_LearnSkill, 0x5A, 0, proc);
}

bool PostActionPredation(ProcPtr proc)
{
    if (gActionData.unitActionType != UNIT_ACTION_COMBAT)
        return false;

    if (!UNIT_ALIVE(gActiveUnit) || UNIT_STONED(gActiveUnit))
        return false;

    if (gBattleActorGlobalFlag.enemy_defeated == false)
        return false;

#if defined(SID_Predation) && (COMMON_SKILL_VALID(SID_Predation))
        if (!SkillTester(gActiveUnit, SID_Predation))
#else
        if (1)
#endif
        return false;

    // KernelCallEvent(EventScr_PostActionPredation, EV_EXEC_CUTSCENE, proc);
    NewMuSkillAnimOnActiveUnit(gActionData.unk08, callback_anim, callback_exec);

    return true;
}
