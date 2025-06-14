#include "common-chax.h"
#include "map-anims.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "debuff.h"
#include "jester_headers/miscellaenous.h"

#if defined(SID_Acidic) && (COMMON_SKILL_VALID(SID_Acidic))

u8 Acidic_Usability(const struct MenuItemDef * def, int number)
{
    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

    if (!HasSelectTarget(gActiveUnit, MakeTargetListForAdjacentEnemies))
		return MENU_NOTSHOWN;

    return MENU_ENABLED;
}

u8 Acidic_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{

    gActionData.unk08 = SID_Acidic;
    gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

    // return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
    return ItemCommandEffect(menu, item);
}

bool Action_Acidic(ProcPtr parent)
{
	return true;
}
#endif