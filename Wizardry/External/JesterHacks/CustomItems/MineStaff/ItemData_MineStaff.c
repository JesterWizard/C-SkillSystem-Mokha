#include "common-chax.h"
#include "constants/texts.h"

const struct ItemData ItemData_MineStaff = {
    .nameTextId = MSG_ITEM_MINE_STAFF_NAME,
    .descTextId = MSG_ITEM_MINE_STAFF_DESC,
    .useDescTextId = MSG_ITEM_MINE_STAFF_USEDESC,
#ifdef CONFIG_ITEM_INDEX_MINE_STAFF
    .number = CONFIG_ITEM_INDEX_MINE_STAFF,
#endif
    .weaponType = ITYPE_STAFF,
    .pStatBonuses = 0,
    .maxUses = 15,
    .encodedRange = 0x11, // Adjacent allies only 1-1
    .costPerUse = 500,
#ifdef CONFIG_ITEM_INDEX_MINE_STAFF
    .iconId = CONFIG_ICON_INDEX_MINE_STAFF,
#endif
    .useEffectId = 0x1B, // Mine
    .weaponRank = WPN_EXP_C,
    .weaponExp = 4,
    .hit = 100,
    .attributes = IA_STAFF,
};
