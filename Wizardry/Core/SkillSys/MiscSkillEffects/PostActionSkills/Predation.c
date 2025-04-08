#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "battle-system.h"
#include "event-rework.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "strmag.h"

static void callback_anim(ProcPtr proc)
{

}
#if defined(SID_PredationPlus) && (COMMON_SKILL_VALID(SID_PredationPlus))
static void callback_exec_predationPlus(ProcPtr proc)
{
    struct Unit * targetUnit = GetUnit(gBattleTarget.unit.index);

    /* If the target unit's first learned skill slot is empty, we can assume they have no skills learned */
    if (UNIT_RAM_SKILLS(targetUnit)[0] == 0)
        return;

    Proc_StartBlocking(ProcScr_PredationPlusSoftLock, proc);
}
#endif 

#if defined(SID_Predation) && (COMMON_SKILL_VALID(SID_Predation))
static void callback_exec_predation(ProcPtr proc)
{
    struct Unit * targetUnit = GetUnit(gBattleTarget.unit.index);

    /* If the target unit's first learned skill slot is empty, we can assume they have no skills learned */
    if (UNIT_RAM_SKILLS(targetUnit)[0] == 0)
        return;

     /* The active unit has space for an additional skill */
    if (UNIT_RAM_SKILLS(gActiveUnit)[6] == 0)
    {
        /* The first learned skill in the target unit's struct */
        AddSkill(gActiveUnit, UNIT_RAM_SKILLS(targetUnit)[0]);
        SetPopupItem((targetUnit->supports[0] << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL);
        NewPopup_Simple(PopupScr_LearnSkill, 0x5A, 0, proc);
    }
    else
    {
        SetPopupItem((targetUnit->supports[0] << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL);
        NewPopup_Simple(PopupScr_ObtainedSkill, 0x5A, 0, proc);
        Proc_StartBlocking(ProcScr_PredationSoftLock, proc);
    }
}
#endif

bool PostActionPredation(ProcPtr proc)
{
    if (gActionData.unitActionType != UNIT_ACTION_COMBAT)
        return false;

    if (!UNIT_ALIVE(gActiveUnit) || UNIT_STONED(gActiveUnit))
        return false;

    if (gBattleActorGlobalFlag.enemy_defeated == false)
        return false;

#if defined(SID_PredationPlus) && (COMMON_SKILL_VALID(SID_PredationPlus))
        if (SkillTester(gActiveUnit, SID_PredationPlus))
        {
            NewMuSkillAnimOnActiveUnit(gActionData.unk08, callback_anim, callback_exec_predationPlus);
            return true;
        }
#endif

#if defined(SID_Predation) && (COMMON_SKILL_VALID(SID_Predation))
        if (SkillTester(gActiveUnit, SID_Predation))
        {
            NewMuSkillAnimOnActiveUnit(gActionData.unk08, callback_anim, callback_exec_predation);
            return true;
        }
#endif

    return false;
}
