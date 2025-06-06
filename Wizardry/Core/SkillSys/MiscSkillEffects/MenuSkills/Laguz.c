#include "common-chax.h"
#include "map-anims.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "debuff.h"
#include "jester_headers/class-pairs.h"
#include "bwl.h"

#ifdef CONFIG_LAGUZ_BARS

u8 Transform_Laguz_Usability(const struct MenuItemDef *def, int number)
{
    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

    FORCE_DECLARE struct NewBwl * bwl;
    bwl = GetNewBwl(UNIT_CHAR_ID(gActiveUnit));

    if (bwl->_pad_[1] != 30)
        return MENU_NOTSHOWN;

    return MENU_ENABLED;
}

u8 Transform_Laguz_OnSelected(struct MenuProc *menu, struct MenuItemProc *item)
{
    if (item->availability == MENU_DISABLED)
    {
        MenuFrozenHelpBox(menu, MSG_SKILL_CommonFail);
        return MENU_ACT_SND6B;
    }

    gActionData.unk08 = 0x401;
    gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

u8 Transform_Laguz_Effect(struct MenuProc * menu, struct MenuItemProc * item)
{
    for (int i = 0; i < laguzListSize; i++)
    {
        if (gActiveUnit->pClassData->number == laguzPairs[i][0])
        {
            gActiveUnit->pClassData = GetClassData(laguzPairs[i][1]);
            SetUnitStatDebuff(gActiveUnit, UNIT_STAT_BUFF_LAGUZ);
            gActiveUnit->curHP += 7;
            gActiveUnit->maxHP += 7;
        }
        else if (gActiveUnit->pClassData->number == laguzPairs[i][1])
        {
            gActiveUnit->pClassData = GetClassData(laguzPairs[i][0]);
            ClearUnitStatDebuff(gActiveUnit, UNIT_STAT_BUFF_LAGUZ);
            gActiveUnit->curHP -= 7;
            gActiveUnit->maxHP -= 7;
        }
    }

    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}
#endif