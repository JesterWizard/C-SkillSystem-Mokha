#include "common-chax.h"
#include "item-sys.h"
#include "constants/texts.h"

const struct ItemData ItemData_SkillScroll_4 = {
    .nameTextId = MSG_ITEM_SkillScrollName,
    .descTextId = MSG_ITEM_SkillScrollName,
    .useDescTextId = MSG_ITEM_SkillScrollUseDesc,
#ifdef CONFIG_ITEM_INDEX_SKILL_SCROLL_4
    .number = CONFIG_ITEM_INDEX_SKILL_SCROLL_4,
#endif
    .weaponType = ITYPE_ITEM,
    .pStatBonuses = NULL,
    .maxUses = 1,
    .encodedRange = 0x11,
    .costPerUse = 8000,
    .iconId = 0,
    .useEffectId = IER_CHAX_SKILL_SCROLL,
};
