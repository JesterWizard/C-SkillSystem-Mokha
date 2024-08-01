#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "class-types.h"
#include "skill-system.h"
#include "constants/skills.h"

bool PrePhase_TickPastelVeilSkillStatus(ProcPtr proc)
{
#if (defined(SID_PastelVeil) && (COMMON_SKILL_VALID(SID_PastelVeil)))
    int i, j;

    for (i = gPlaySt.faction + 1; i <= (gPlaySt.faction + GetFactionUnitAmount(gPlaySt.faction)); ++i)
    {
        struct Unit * unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
            continue;

        if (!SkillTester(unit, SID_PastelVeil))
            continue;

        for (j = 0; j < ARRAY_COUNT_RANGE3x3; j++)
        {
            int _x = unit->xPos + gVecs_3x3[j].x;
            int _y = unit->yPos + gVecs_3x3[j].y;

            struct Unit * unit_ally = GetUnitAtPosition(_x, _y);
            if (!UNIT_IS_VALID(unit_ally))
                continue;

            if (unit_ally->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                continue;

            if (!AreUnitsAllied(unit->index, unit_ally->index))
                continue;

            if (unit_ally->statusIndex == UNIT_STATUS_POISON)
                unit_ally->statusIndex = UNIT_STATUS_NONE;
        }

        if (unit->statusIndex == UNIT_STATUS_POISON)
            unit->statusIndex = UNIT_STATUS_NONE;
    }
#endif
    return false;
}
