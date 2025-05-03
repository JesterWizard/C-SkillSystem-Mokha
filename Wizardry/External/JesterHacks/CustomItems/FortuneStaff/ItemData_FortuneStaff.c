#include "common-chax.h"
#include "constants/texts.h"

const struct ItemData ItemData_FortuneStaff = {
    .nameTextId = MSG_ITEM_FORTUNE_STAFF_NAME,
    .descTextId = MSG_ITEM_FORTUNE_STAFF_DESC,
    .useDescTextId = MSG_ITEM_FORTUNE_STAFF_USEDESC,
    .number = CONFIG_ITEM_INDEX_FORTUNE_STAFF,
    .weaponType = ITYPE_STAFF,
    .pStatBonuses = 0,
    .maxUses = 15,
    .encodedRange = 0x11, // Adjacent allies only 1-1
    .costPerUse = 500,
    .iconId = CONFIG_ICON_INDEX_FORTUNE_STAFF,
    .useEffectId = 0xF, // Barrier
    .weaponRank = WPN_EXP_C,
    .weaponExp = 4,
    .hit = 100,
    .attributes = IA_STAFF,
};
