#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "strmag.h"
#include "constants/skills.h"

LYN_REPLACE_CHECK(GetOffensiveStaffAccuracy);
int GetOffensiveStaffAccuracy(struct Unit * actor, struct Unit * target)
{
    int baseAccuracy = (MagGetter(actor) - ResGetter(target)) * 5;
    int unitSkill = SklGetter(actor);
    int distance = RECT_DISTANCE(actor->xPos, actor->yPos, target->xPos, target->yPos);
    int result = (baseAccuracy + 30 + unitSkill) - distance * 2;

#ifdef CONFIG_CUSTOM_STAFF_ACCURACY
    result = CONFIG_CUSTOM_STAFF_ACCURACY;
#endif

    LIMIT_AREA(result, 0, 100);
    return result;
}
