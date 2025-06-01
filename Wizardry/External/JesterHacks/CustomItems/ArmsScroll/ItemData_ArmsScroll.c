#include "common-chax.h"
#include "constants/texts.h"

extern const struct ItemStatBonuses ItemBonus_ArmsScroll;

const struct ItemData ItemData_ArmsScroll = {
    .nameTextId = MSG_ITEM_ArmsScroll_NAME,
    .descTextId = MSG_ITEM_ArmsScroll_DESC,
    .useDescTextId = MSG_ITEM_ArmsScroll_USEDESC,
#ifdef CONFIG_ITEM_INDEX_ARMS_SCROLL
    .number = CONFIG_ITEM_INDEX_ARMS_SCROLL,
#endif
    .weaponType = ITYPE_ITEM,
    .pStatBonuses = 0,
    .maxUses = 1,
    .encodedRange = 0x11,
    .costPerUse = 8000,
#ifdef CONFIG_ITEM_INDEX_ARMS_SCROLL
    .iconId = CONFIG_ICON_INDEX_ARMS_SCROLL,
#endif
    .useEffectId = 18,
};