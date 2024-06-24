#include "global.h"
#include "bmunit.h"
#include "mu.h"
#include "proc.h"
#include "bmmind.h"

#include "common-chax.h"
#include "debuff.h"
#include "skill-system.h"
#include "battle-system.h"
#include "constants/skills.h"

extern u8 gPostActionPowerStaffFlag;

bool PostActionPowerStaff(ProcPtr parent)
{

#if defined(SID_PowerStaff) && (COMMON_SKILL_VALID(SID_PowerStaff))
    struct Unit * unit = gActiveUnit;

    gPostActionPowerStaffFlag = false;

    switch (gActionData.unitActionType ) {
    case UNIT_ACTION_STAFF:
        if (SkillTester(unit, SID_PowerStaff))
        {
            gPostActionPowerStaffFlag = true;
            MU_EndAll();
            StartStatusHealEffect(unit, parent);
            return true;
        }
    }
#endif
    return false;
}
