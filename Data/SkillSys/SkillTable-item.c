#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const u16 gConstSkillTable_Item[0x100][2] = {
<<<<<<< HEAD
    [ITEM_SWORD_SLIM] = {
    },
    [ITEM_SWORD_RAPIER] = {
    },
    [ITEM_AXE_IRON] = {
    },
    [ITEM_STAFF_HEAL] = {
    },
    [ITEM_ANIMA_ELFIRE] = {
    },
    [ITEM_LANCE_SLIM] = {
    },
    [ITEM_LANCE_SILVER] = {
    }
=======
	[ITEM_SWORD_SLIM] = {
#if (defined(SID_RuinedBladePlus) && COMMON_SKILL_VALID(SID_RuinedBladePlus))
		SID_RuinedBladePlus,
#endif
	},
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc
};
