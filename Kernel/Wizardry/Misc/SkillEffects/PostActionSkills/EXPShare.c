#include "common-chax.h"
#include "debuff.h"
#include "skill-system.h"
#include "kernel-lib.h"
#include "constants/skills.h"

bool PostAction_EXPShare(ProcPtr parent)
{
	FORCE_DECLARE struct Unit *unit = gActiveUnit;

	if (!UnitAvaliable(gActiveUnit) || UNIT_STONED(gActiveUnit))
		return false;

	if (gActionData.unitActionType == UNIT_ACTION_COMBAT) {
#if defined(SID_EXPShare) && (COMMON_SKILL_VALID(SID_EXPShare))
		for (int i = 0; i < ARRAY_COUNT_RANGE2x2; i++)
		{
			int _x = unit->xPos + gVecs_2x2[i].x;
			int _y = unit->yPos + gVecs_2x2[i].y;

			struct Unit *unit_ally = GetUnitAtPosition(_x, _y);

			if (!UNIT_IS_VALID(unit_ally))
				continue;

			if (!AreUnitsAllied(unit->index, unit_ally->index))
				continue;

			if (unit_ally->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
				continue;

			/* JESTER - I want to have the EXP bar display but I just do not understand procs enough to make it happen */

			if (SkillListTester(unit_ally, SID_EXPShare)) 
				unit_ally->exp += 10;
		
		}
#endif
	}
	return false;
}
