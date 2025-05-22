#include "common-chax.h"
#include "constants/texts.h"

const struct ItemData ItemData_IronStaff = {
    .nameTextId = MSG_ITEM_IRON_STAFF_NAME,
    .descTextId = MSG_ITEM_IRON_STAFF_DESC,
    .useDescTextId = MSG_ITEM_IRON_STAFF_USEDESC,
#ifdef CONFIG_ITEM_INDEX_IRON_STAFF
    .number = CONFIG_ITEM_INDEX_IRON_STAFF,
#endif
    .weaponType = ITYPE_STAFF,
    .pStatBonuses = 0,
    .maxUses = 15,
    .encodedRange = 0x11, // Adjacent allies only 1-1
    .costPerUse = 500,
#ifdef CONFIG_ITEM_INDEX_IRON_STAFF
    .iconId = CONFIG_ICON_INDEX_IRON_STAFF,
#endif
    .useEffectId = 0xF, // Barrier
    .weaponRank = WPN_EXP_C,
    .weaponExp = 4,
    .hit = 100,
    .attributes = IA_STAFF,
};
