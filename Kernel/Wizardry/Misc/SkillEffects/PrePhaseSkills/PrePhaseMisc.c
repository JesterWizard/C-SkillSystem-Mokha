#include "common-chax.h"
#include "unit-expa.h"
#include "bwl.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "jester_headers/custom-arrays.h"

bool PrePhsae_ClearMiscUES(ProcPtr proc)
{
	int uid;
	struct Unit *unit;

	for (uid = 1; uid < 0xC0; uid++) {
		unit = GetUnit(uid);

		if (UNIT_IS_VALID(unit)) {
			ClearBitUES(unit, UES_BIT_GUARDBEAR_SKILL_USED);
			ClearBitUES(unit, UES_BIT_TSZUKU_SKILL_USED);
			ClearBitUES(unit, UES_BIT_TELEPOTRATION_SKILL_USED);
		}
	}
	return false;
}

void PrePhase_ApplyMpStartingAmount(ProcPtr proc)
{
	struct Unit * unit;
	int unit_id;
	struct NewBwl * bwl;

	if (gPlaySt.faction == FACTION_BLUE)
	{
		for (int uid = FACTION_BLUE; uid < FACTION_GREEN; uid++) {

			unit = GetUnit(uid);
			unit_id = UNIT_CHAR_ID(unit);
			bwl = GetNewBwl(unit_id); 

			if (UNIT_IS_VALID(unit) && bwl != NULL)
			{
				#if defined(SID_MPChanneling) && (COMMON_SKILL_VALID(SID_MPChanneling))
					if (SkillTester(unit, SID_MPChanneling))
						bwl->currentMP += gMpSystemPInfoConfigList[unit_id].generationRate * 2;
					else
						bwl->currentMP += gMpSystemPInfoConfigList[unit_id].generationRate;
				#else
					bwl->currentMP += gMpSystemPInfoConfigList[unit_id].generationRate;
				#endif
			}

			// Clamp the value to max MP using a ternary operator
			bwl->currentMP = (bwl->currentMP > bwl->maxMP) ? bwl->maxMP : bwl->currentMP; 
		}
	}
}