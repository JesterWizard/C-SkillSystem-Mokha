#include "common-chax.h"
#include "debuff.h"
#include "action-expa.h"
#include "skill-system.h"
#include "battle-system.h"
#include "constants/skills.h"

bool PostActionTakerSkills(ProcPtr parent)
{
    FORCE_DECLARE struct Unit * unit = gActiveUnit;

    if (!UNIT_IS_VALID(unit))
        return false;

    switch (gActionData.unitActionType) {
    case UNIT_ACTION_COMBAT:
#if defined(SID_TakerStrength) && (COMMON_SKILL_VALID(SID_TakerStrength))
        if (SkillTester(unit, SID_TakerStrength) && gBattleActorGlobalFlag.enimy_defeated)
            SetUnitStatDebuff(unit, UNIT_STAT_BUFF_TAKER_POW);
#endif
    }

    return true;
}
