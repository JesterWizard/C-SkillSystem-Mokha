#include "common-chax.h"
#include "constants/texts.h"

extern const struct ItemStatBonuses ItemBonus_SkillStealer;

const struct ItemData ItemData_SkillStealer = {
    .nameTextId = MSG_ITEM_SKILLSTEALER_NAME,
    .descTextId = MSG_ITEM_SKILLSTEALER_DESC,
    .useDescTextId = MSG_ITEM_SKILLSTEALER_USEDESC,
#ifdef CONFIG_ITEM_INDEX_SKILL_STEALER
    .number = CONFIG_ITEM_INDEX_SKILL_STEALER,
#endif
    .weaponType = ITYPE_ITEM,
    .pStatBonuses = 0,
    .maxUses = 1,
    .encodedRange = 0x11,
    .costPerUse = 8000,
#ifdef CONFIG_ITEM_INDEX_SKILL_STEALER
    .iconId = CONFIG_ICON_INDEX_SKILL_STEALER,
#endif
    .useEffectId = 17,
};
