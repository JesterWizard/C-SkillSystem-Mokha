#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"

int _GetUnitCon(struct Unit * unit)
{
    int status = UNIT_CON(unit);
    return status;
}

int ConGetterSkills(int status, struct Unit * unit)
{
#if defined(SID_ConBonus) && (COMMON_SKILL_VALID(SID_ConBonus))
    if (SkillTester(unit, SID_ConBonus))
        status += SKILL_EFF0(SID_ConBonus);
#endif

    return status;
}