#include "common-chax.h"
#include "constants/texts.h"

const struct ItemData ItemData_SkillScroll_3 = {
    .nameTextId = MSG_ITEM_SkillScrollName,
    .descTextId = MSG_ITEM_SkillScrollName,
    .useDescTextId = MSG_ITEM_SkillScrollUseDesc,
#ifdef CONFIG_ITEM_INDEX_SKILL_SCROLL_3
    .number = CONFIG_ITEM_INDEX_SKILL_SCROLL_3,
#endif
    .weaponType = ITYPE_ITEM,
    .pStatBonuses = NULL,
    .maxUses = 1,
    .encodedRange = 0x11,
    .costPerUse = 8000,
    .iconId = 0,
    .useEffectId = 17,
};