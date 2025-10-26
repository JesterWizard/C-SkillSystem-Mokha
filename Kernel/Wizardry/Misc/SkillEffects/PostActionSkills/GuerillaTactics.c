#include "common-chax.h"
#include "debuff.h"
#include "skill-system.h"
#include "map-anims.h"
#include "constants/skills.h"
#include "mu.h"

FORCE_DECLARE static inline bool IsPosInvalid(s8 x, s8 y)
{
    return ((x < 0) & (x > gBmMapSize.x) & (y < 0) & (y > gBmMapSize.y));
}

FORCE_DECLARE static void callback_anim(ProcPtr proc)
{

}

FORCE_DECLARE static void callback_exec(ProcPtr proc)
{
    gActiveUnit->state |= US_HAS_MOVED | US_CANTOING;

    // Set the unit's coordinates to the last moved space
    gActionData.xMove = gpPathArrowProc->pathX[gpPathArrowProc->pathLen - 1];
    gActionData.yMove = gpPathArrowProc->pathY[gpPathArrowProc->pathLen - 1];
}

bool PostActionGuerillaTactics(ProcPtr parent)
{
#if defined(SID_GuerillaTactics) && (COMMON_SKILL_VALID(SID_GuerillaTactics))
    if (gActionData.unitActionType != UNIT_ACTION_COMBAT)
        return false;

    if (gActionData.moveCount == 0)
        return false;

    FORCE_DECLARE struct Unit * unit = gActiveUnit;
    FORCE_DECLARE struct Unit * target = GetUnit(gActionData.targetIndex);

    if (!UNIT_ALIVE(gActiveUnit) || UNIT_STONED(gActiveUnit))
        return false;

    if (!UNIT_ALIVE(target))
        return false;

    if (!SkillTester(unit, SID_GuerillaTactics) || !UNIT_IS_VALID(target))
        return false;

    NewMuSkillAnimOnActiveUnit(gActionData.unk08, callback_anim, callback_exec);

    return true;
#endif
    return false;
}