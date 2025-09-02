#include "common-chax.h"
#include "debuff.h"
#include "skill-system.h"
#include "kernel-lib.h"
#include "constants/skills.h"

bool PostAction_Pyrotechnics(ProcPtr parent)
{
	FORCE_DECLARE struct Unit *unit = gActiveUnit;

	if (!UnitAvaliable(gActiveUnit) || UNIT_STONED(gActiveUnit))
		return false;

	if (gActionData.unitActionType == UNIT_ACTION_COMBAT) {
#if defined(SID_Pyrotechnics) && (COMMON_SKILL_VALID(SID_Pyrotechnics))
		if (SkillListTester(unit, SID_Pyrotechnics)) 
		{
			for (int i = 0; i < ARRAY_COUNT_RANGE2x2; i++)
			{
				int _x = unit->xPos + gVecs_2x2[i].x;
				int _y = unit->yPos + gVecs_2x2[i].y;

				struct Unit *unit_enemy = GetUnitAtPosition(_x, _y);

				if (!UNIT_IS_VALID(unit_enemy))
					continue;

				if (unit_enemy->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
					continue;

				int temp_hp = unit_enemy->curHP - (unit_enemy->maxHP / 5);

				if (temp_hp > 0) {
					unit_enemy->curHP = temp_hp;
				} else {
					unit_enemy->curHP = 1;
				}
			}
		}
#endif
	}
	return false;
}
