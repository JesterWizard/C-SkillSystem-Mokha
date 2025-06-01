#include "common-chax.h"
#include "skill-system.h"

LYN_REPLACE_CHECK(StealCommandUsability);
u8 StealCommandUsability(const struct MenuItemDef * def, int number)
{
#if !CHAX
    if (!(UNIT_CATTRIBUTES(gActiveUnit) & CA_STEAL))
        return MENU_NOTSHOWN;
#endif

    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

    MakeTargetListForSteal(gActiveUnit);
    if (GetSelectTargetCount() == 0)
        return MENU_NOTSHOWN;

    if (GetUnitItemCount(gActiveUnit) == UNIT_ITEM_COUNT)
        return MENU_DISABLED;

    return MENU_ENABLED;
}

LYN_REPLACE_CHECK(StealCommandEffect);
u8 StealCommandEffect(struct MenuProc* menu, struct MenuItemProc* menuItem) {
    if (menuItem->availability == MENU_DISABLED) {
        MenuFrozenHelpBox(menu, 0x862); // TODO msgid "Your inventory is full.[.]"
        return MENU_ACT_SND6B;
    }

    ClearBg0Bg1();

    MakeTargetListForSteal(gActiveUnit);

    NewTargetSelection(&gSelectInfo_Steal);

    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
}