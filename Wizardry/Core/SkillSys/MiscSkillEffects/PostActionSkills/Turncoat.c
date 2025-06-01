#include "common-chax.h"
#include "debuff.h"
#include "map-anims.h"
#include "battle-system.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "unit-expa.h"
#include "action-expa.h"

bool PostActionTurncoat(ProcPtr parent)
{
#if defined(SID_Turncoat) && defined(SID_Turncoat)
    struct Unit *unit = gActiveUnit;
    struct Unit *unit_tar = GetUnit(gActionData.targetIndex);

    // if (CheckBitUES(UNIT, UES_BIT_TURNCOAT))
    // {
    //     if (UNIT_FACTION(unit) == FACTION_RED)
    //         UnitChangeFaction(unit, FACTION_BLUE);
    //     else
    //         UnitChangeFaction(unit, FACTION_RED);

    //     gActionDataExpa.refrain_action = true;
    //     EndAllMus();
    // }

    if (!UNIT_ALIVE(unit) || UNIT_STONED(unit))
        return false;

    if (!UNIT_ALIVE(unit_tar) || UNIT_STONED(unit_tar))
        return false;

#if defined(SID_Turncoat) && (COMMON_SKILL_VALID(SID_Turncoat))
    if (SkillTester(unit, SID_Turncoat))
    {
        SetBitUES(unit_tar, UES_BIT_CHANGED_FACTIONS);

        if (UNIT_FACTION(unit_tar) == FACTION_RED)
            UnitChangeFaction(unit_tar, FACTION_BLUE);
        else
            UnitChangeFaction(unit_tar, FACTION_RED);
    }
#endif

    return true;
#endif
    return false;
}
