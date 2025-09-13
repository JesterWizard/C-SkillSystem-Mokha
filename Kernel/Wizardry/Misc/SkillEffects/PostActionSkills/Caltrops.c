#include "common-chax.h"
#include "debuff.h"
#include "skill-system.h"
#include "kernel-lib.h"
#include "constants/skills.h"

bool PostAction_Caltrops(ProcPtr parent)
{
	FORCE_DECLARE struct Unit *target = GetUnit(gActionData.targetIndex);

	if (gActionData.unitActionType == UNIT_ACTION_COMBAT) {
#if defined(SID_Caltrops) && (COMMON_SKILL_VALID(SID_Caltrops))
		if (SkillTester(gActiveUnit, SID_Caltrops) && (UnitAvaliable(gActiveUnit) && !UNIT_STONED(gActiveUnit)))
			AddFireTile(target->xPos, target->yPos, 1, 1);

		if (SkillTester(target, SID_Caltrops) && (UnitAvaliable(target) && !UNIT_STONED(target))) 
			AddFireTile(gActiveUnit->xPos, gActiveUnit->yPos, 1, 1);
#endif
	}
	return false;
}
