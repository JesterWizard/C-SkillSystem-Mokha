#include "common-chax.h"
#include "kernel-lib.h"
#include "skill-system.h"

const struct KernelDesigerConfig gKernelDesigerConfig = {

	/**
	 * If set, player can use prep-skill screen to
	 * select skills to equip for each character.
	 */
	.equip_skill_en = true,

	/**
	 * Deside whether to allow players to generate a new skill scroll,
	 * if he try to use skill scroll to a unit who has already filled with skills.
	 *
	 * 1: choose a equipped skill to replace
	 * 0: just learn the skill (equippable in prep-skill screen)
	 */
	.gen_new_scroll = false,

	/**
	 * Max amount of equip able skill for each unit (0-7)
	 */
	.max_equipable_skill = UNIT_RAM_SKILLS_LEN,

#ifdef CONFIG_REMOVE_MOVE_PATH
	.remove_move_path = true,
#endif

#ifdef CONFIG_USE_UTF8_GLYPH
	.use_chinese_character = false,
#endif

#ifdef CONFIG_KTUT_LEVEL
	.kernel_tutorial_level = CONFIG_KTUT_LEVEL,
#endif

#ifdef CONFIG_USE_COMBO_ATTACK
	.combo_attack_en = true, // true, // can be opened by the user
#endif

#ifdef CONFIG_BATTLE_SURROUND
	.battle_surrend_en = true,
#endif

	.hit_decrease_on_range = false,

#ifdef CONFIG_DEBUG_UNIT_LOAD_SKILL
	.debug_autoload_skills = true,
#endif

	.guaranteed_lvup = false,

#ifdef CONFIG_LVUP_MODE_TUTORIAL
	.lvup_mode_tutorial = CONFIG_LVUP_MODE_TUTORIAL,
#endif
#ifdef CONFIG_LVUP_MODE_NORMAL
	.lvup_mode_normal = CONFIG_LVUP_MODE_NORMAL,
#endif
#ifdef CONFIG_LVUP_MODE_HARD
	.lvup_mode_hard = CONFIG_LVUP_MODE_HARD,
#endif
#ifdef CONFIG_PAGE1_WITH_BWL
	.unit_page_style = CONFIG_PAGE1_WITH_BWL,
#endif
#ifdef CONFIG_PAGE3_SKILL_LAYOUT
	.skil_page_style = CONFIG_PAGE3_SKILL_LAYOUT,
#endif

#ifdef CONFIG_NO_SUS_IN_AI_PHASE
	.no_suspend_in_aiphase = true,
#endif

#ifdef CONFIG_INSTALL_KERNEL_SHIELD
	.shield_en = true,
	.shield_ext_equip_config_en = true,
#endif

#ifdef CONFIG_AUTO_NARROW_FONT
	.auto_narrow_font = true,
#endif
};
