#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const u16 gConstSkillTable_Weapon[0x100][2] = {
<<<<<<< HEAD
    [ITEM_SWORD_IRON] = {
    },
    [ITEM_SWORD_SLIM] = {
    },

    [ITEM_BLADE_SILVER] = {
    },

    [ITEM_SWORD_RAPIER] = {
    },
=======
	[ITEM_SWORD_IRON] = {
#if (defined(SID_COMBAT_Hexblade) && COMMON_SKILL_VALID(SID_COMBAT_Hexblade))
		SID_COMBAT_Hexblade,
#endif
	},

	[ITEM_SWORD_SLIM] = {
#if (defined(SID_RuinedBladePlus) && COMMON_SKILL_VALID(SID_RuinedBladePlus))
		SID_RuinedBladePlus,
#endif
	},

	[ITEM_BLADE_SILVER] = {
#if (defined(SID_COMBAT_BaneOfMonsters) && COMMON_SKILL_VALID(SID_COMBAT_BaneOfMonsters))
		SID_COMBAT_BaneOfMonsters,
#endif
	},

	[ITEM_SWORD_RAPIER] = {
#if (defined(SID_COMBAT_Grounder) && COMMON_SKILL_VALID(SID_COMBAT_Grounder))
		SID_COMBAT_Grounder,
#endif
	},
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc
};
