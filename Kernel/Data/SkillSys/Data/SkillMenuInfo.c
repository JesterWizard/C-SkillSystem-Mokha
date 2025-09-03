#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"

struct MenuItemDef const *const gSkillMenuInfos[MAX_SKILL_NUM + 1] = {
#if (defined(SID_Dance) && COMMON_SKILL_VALID(SID_Dance))
	[SID_Dance] = &(const struct MenuItemDef) {
		.name = "　踊る",
		.nameMsgId = 0x67E,
		.helpMsgId = 0x6C2,
		.color = TEXT_COLOR_SYSTEM_GOLD,
		.isAvailable = DanceCommandUsability,
		.onDraw = NULL,
		.onSelected = PlayCommandEffect,
		.onIdle = NULL,
		.onSwitchIn = NULL,
		.onSwitchOut = NULL,
	},
#endif

#if (defined(SID_Steal) && COMMON_SKILL_VALID(SID_Steal))
	[SID_Steal] = &(const struct MenuItemDef) {
		.name = "　盗む",
		.nameMsgId = 0x67F,
		.helpMsgId = 0x6C4,
		.color = TEXT_COLOR_SYSTEM_GOLD,
		.isAvailable = StealCommandUsability,
		.onDraw = NULL,
		.onSelected = StealCommandEffect,
		.onIdle = 0,
		.onSwitchIn = 0,
		.onSwitchOut = 0
	},
#endif

#if (defined(SID_Summon) && COMMON_SKILL_VALID(SID_Summon))
	[SID_Summon] = &(const struct MenuItemDef) {
		.name = "　召喚",
		.nameMsgId = 0x693,
		.helpMsgId = 0x6DD,
		.color = TEXT_COLOR_SYSTEM_GOLD,
		.isAvailable = SummonCommandUsability,
		.onDraw = 0,
		.onSelected = SummonCommandEffect,
		.onIdle = 0,
		.onSwitchIn = 0,
		.onSwitchOut = 0
	},
#endif

#if (defined(SID_LockTouch) && COMMON_SKILL_VALID(SID_LockTouch))
	[SID_LockTouch] = &(const struct MenuItemDef) {
		.name = "　かぎ開",
		.nameMsgId = 0x694,
		.helpMsgId = 0x6DE,
		.color = TEXT_COLOR_SYSTEM_GOLD,
		.isAvailable = PickCommandUsability,
		.onDraw = NULL,
		.onSelected = PickCommandEffect,
		.onIdle = NULL,
		.onSwitchIn = NULL,
		.onSwitchOut = NULL,
	},
#endif

#if (defined(SID_HealingFocus) && COMMON_SKILL_VALID(SID_HealingFocus))
	[SID_HealingFocus] = &(const struct MenuItemDef) {
		.name = "　瞑想",
		.nameMsgId = MSG_SKILL_HealingFocus_NAME,
		.helpMsgId = MSG_SKILL_HealingFocus_DESC,
		.color = TEXT_COLOR_SYSTEM_GOLD,
		.isAvailable = HealingFocus_Usability,
		.onDraw = NULL,
		.onSelected = HealingFocus_OnSelected,
		.onIdle = NULL,
		.onSwitchIn = NULL,
		.onSwitchOut = NULL,
	},
#endif

#if (defined(SID_Teleportation) && COMMON_SKILL_VALID(SID_Teleportation))
	[SID_Teleportation] = &(const struct MenuItemDef) {
		.name = "　転移",
#if !CONFIG_MENU_SKILL_DISP_MSG_EN_N
		.nameMsgId = MSG_SKILL_Teleportation_NAME,
#endif
		.helpMsgId = MSG_SKILL_Teleportation_DESC,
		.color = TEXT_COLOR_SYSTEM_GOLD,
		.isAvailable = Teleportation_Usability,
		.onDraw = NULL,
		.onSelected = Teleportation_OnSelected,
		.onIdle = NULL,
		.onSwitchIn = NULL,
		.onSwitchOut = NULL,
	},
#endif

#if (defined(SID_LightRune) && COMMON_SKILL_VALID(SID_LightRune))
	[SID_LightRune] = &(const struct MenuItemDef) {
		.name = "　光の結界",
		.nameMsgId = MSG_SKILL_LightRune_NAME,
		.helpMsgId = MSG_SKILL_LightRune_DESC,
		.color = TEXT_COLOR_SYSTEM_GOLD,
		.isAvailable = LightRune_Usability,
		.onDraw = NULL,
		.onSelected = LightRune_OnSelected,
		.onIdle = NULL,
		.onSwitchIn = NULL,
		.onSwitchOut = NULL,
	},
#endif

#if (defined(SID_Mine) && COMMON_SKILL_VALID(SID_Mine))
	[SID_Mine] = &(const struct MenuItemDef) {
		.name = "　地雷",
		.nameMsgId = MSG_SKILL_Mine_NAME,
		.helpMsgId = MSG_SKILL_Mine_DESC,
		.color = TEXT_COLOR_SYSTEM_GOLD,
		.isAvailable = Mine_Usability,
		.onDraw = NULL,
		.onSelected = Mine_OnSelected,
		.onIdle = NULL,
		.onSwitchIn = NULL,
		.onSwitchOut = NULL,
	},
#endif

#if (defined(SID_RallyDefense) && COMMON_SKILL_VALID(SID_RallyDefense))
	[SID_RallyDefense] = &(const struct MenuItemDef) {
		.name = "　ラリー防御",
		.nameMsgId = MSG_SKILL_RallyDefense_NAME,
		.helpMsgId = MSG_SKILL_RallyDefense_DESC,
		.color = TEXT_COLOR_SYSTEM_GOLD,
		.isAvailable = Rally_Usability,
		.onDraw = NULL,
		.onSelected = Rally_OnSelected,
		.onIdle = NULL,
		.onSwitchIn = Rally_Hover,
		.onSwitchOut = Rally_Unhover,
	},
#endif

#if (defined(SID_RallyLuck) && COMMON_SKILL_VALID(SID_RallyLuck))
	[SID_RallyLuck] = &(const struct MenuItemDef) {
		.name = "　ラリー運",
		.nameMsgId = MSG_SKILL_RallyLuck_NAME,
		.helpMsgId = MSG_SKILL_RallyLuck_DESC,
		.color = TEXT_COLOR_SYSTEM_GOLD,
		.isAvailable = Rally_Usability,
		.onDraw = NULL,
		.onSelected = Rally_OnSelected,
		.onIdle = NULL,
		.onSwitchIn = Rally_Hover,
		.onSwitchOut = Rally_Unhover,
	},
#endif

#if (defined(SID_RallyMagic) && COMMON_SKILL_VALID(SID_RallyMagic))
	[SID_RallyMagic] = &(const struct MenuItemDef) {
		.name = "　ラリーマジック",
		.nameMsgId = MSG_SKILL_RallyMagic_NAME,
		.helpMsgId = MSG_SKILL_RallyMagic_DESC,
		.color = TEXT_COLOR_SYSTEM_GOLD,
		.isAvailable = Rally_Usability,
		.onDraw = NULL,
		.onSelected = Rally_OnSelected,
		.onIdle = NULL,
		.onSwitchIn = Rally_Hover,
		.onSwitchOut = Rally_Unhover,
	},
#endif

#if (defined(SID_RallyMovement) && COMMON_SKILL_VALID(SID_RallyMovement))
	[SID_RallyMovement] = &(const struct MenuItemDef) {
		.name = "　ラリーの動き",
		.nameMsgId = MSG_SKILL_RallyMovement_NAME,
		.helpMsgId = MSG_SKILL_RallyMovement_DESC,
		.color = TEXT_COLOR_SYSTEM_GOLD,
		.isAvailable = Rally_Usability,
		.onDraw = NULL,
		.onSelected = Rally_OnSelected,
		.onIdle = NULL,
		.onSwitchIn = Rally_Hover,
		.onSwitchOut = Rally_Unhover,
	},
#endif

#if (defined(SID_RallyResistance) && COMMON_SKILL_VALID(SID_RallyResistance))
	[SID_RallyResistance] = &(const struct MenuItemDef) {
		.name = "　ラリー耐性",
		.nameMsgId = MSG_SKILL_RallyResistance_NAME,
		.helpMsgId = MSG_SKILL_RallyResistance_DESC,
		.color = TEXT_COLOR_SYSTEM_GOLD,
		.isAvailable = Rally_Usability,
		.onDraw = NULL,
		.onSelected = Rally_OnSelected,
		.onIdle = NULL,
		.onSwitchIn = Rally_Hover,
		.onSwitchOut = Rally_Unhover,
	},
#endif

#if (defined(SID_RallySkill) && COMMON_SKILL_VALID(SID_RallySkill))
	[SID_RallySkill] = &(const struct MenuItemDef) {
		.name = "　ラリースキル",
		.nameMsgId = MSG_SKILL_RallySkill_NAME,
		.helpMsgId = MSG_SKILL_RallySkill_DESC,
		.color = TEXT_COLOR_SYSTEM_GOLD,
		.isAvailable = Rally_Usability,
		.onDraw = NULL,
		.onSelected = Rally_OnSelected,
		.onIdle = NULL,
		.onSwitchIn = Rally_Hover,
		.onSwitchOut = Rally_Unhover,
	},
#endif

#if (defined(SID_RallySpeed) && COMMON_SKILL_VALID(SID_RallySpeed))
	[SID_RallySpeed] = &(const struct MenuItemDef) {
		.name = "　ラリースピード",
		.nameMsgId = MSG_SKILL_RallySpeed_NAME,
		.helpMsgId = MSG_SKILL_RallySpeed_DESC,
		.color = TEXT_COLOR_SYSTEM_GOLD,
		.isAvailable = Rally_Usability,
		.onDraw = NULL,
		.onSelected = Rally_OnSelected,
		.onIdle = NULL,
		.onSwitchIn = Rally_Hover,
		.onSwitchOut = Rally_Unhover,
	},
#endif

#if (defined(SID_RallyStrength) && COMMON_SKILL_VALID(SID_RallyStrength))
	[SID_RallyStrength] = &(const struct MenuItemDef) {
		.name = "　ラリーパワー",
		.nameMsgId = MSG_SKILL_RallyStrength_NAME,
		.helpMsgId = MSG_SKILL_RallyStrength_DESC,
		.color = TEXT_COLOR_SYSTEM_GOLD,
		.isAvailable = Rally_Usability,
		.onDraw = NULL,
		.onSelected = Rally_OnSelected,
		.onIdle = NULL,
		.onSwitchIn = Rally_Hover,
		.onSwitchOut = Rally_Unhover,
	},
#endif

#if (defined(SID_RallySpectrum) && COMMON_SKILL_VALID(SID_RallySpectrum))
	[SID_RallySpectrum] = &(const struct MenuItemDef) {
		.name = "　ラリースペクトラム",
		.nameMsgId = MSG_SKILL_RallySpectrum_NAME,
		.helpMsgId = MSG_SKILL_RallySpectrum_DESC,
		.color = TEXT_COLOR_SYSTEM_GOLD,
		.isAvailable = Rally_Usability,
		.onDraw = NULL,
		.onSelected = Rally_OnSelected,
		.onIdle = NULL,
		.onSwitchIn = Rally_Hover,
		.onSwitchOut = Rally_Unhover,
	},
#endif

#if (defined(SID_GoddessDance) && COMMON_SKILL_VALID(SID_GoddessDance))
	[SID_GoddessDance] = &(const struct MenuItemDef) {
		.name = "　女神の踊り",
		.nameMsgId = MSG_SKILL_GoddessDance_NAME,
		.helpMsgId = MSG_SKILL_GoddessDance_DESC,
		.color = TEXT_COLOR_SYSTEM_GOLD,
		.isAvailable = GoddessDance_Usability,
		.onDraw = NULL,
		.onSelected = GoddessDance_OnSelected,
		.onIdle = NULL,
		.onSwitchIn = GoddessDance_Hover,
		.onSwitchOut = GoddessDance_Unhover,
	},
#endif

#if (defined(SID_Stride) && COMMON_SKILL_VALID(SID_Stride))
	[SID_Stride] = &(const struct MenuItemDef) {
		.name = "　大股",
		.nameMsgId = MSG_SKILL_Stride_NAME,
		.helpMsgId = MSG_SKILL_Stride_DESC,
		.color = TEXT_COLOR_SYSTEM_GOLD,
		.isAvailable = Stride_Usability,
		.onDraw = NULL,
		.onSelected = Stride_OnSelected,
		.onIdle = NULL,
		.onSwitchIn = Stride_Hover,
		.onSwitchOut = Stride_Unhover,
	},
#endif

#if (defined(SID_Swarp) && COMMON_SKILL_VALID(SID_Swarp))
	[SID_Swarp] = &(const struct MenuItemDef) {
		.name = "　スワープ",
		.nameMsgId = MSG_SKILL_Swarp_NAME,
		.helpMsgId = MSG_SKILL_Swarp_DESC,
		.color = TEXT_COLOR_SYSTEM_GOLD,
		.isAvailable = Swarp_Usability,
		.onDraw = NULL,
		.onSelected = Swarp_OnSelected,
		.onIdle = NULL,
		.onSwitchIn = NULL,
		.onSwitchOut = NULL,
	},
#endif

#if (defined(SID_Blacksmith) && COMMON_SKILL_VALID(SID_Blacksmith))
	[SID_Blacksmith] = &(const struct MenuItemDef) {
		.name = "　鍛冶屋",
		.nameMsgId = MSG_SKILL_Blacksmith_NAME,
		.helpMsgId = MSG_SKILL_Blacksmith_DESC,
		.color = TEXT_COLOR_SYSTEM_GOLD,
		.isAvailable = Blacksmith_Usability,
		.onDraw = NULL,
		.onSelected = Blacksmith_OnSelected,
		.onIdle = NULL,
		.onSwitchIn = NULL,
		.onSwitchOut = NULL,
	},
#endif

#if (defined(SID_DivineReprieve) && COMMON_SKILL_VALID(SID_DivineReprieve))
	[SID_DivineReprieve] = &(const struct MenuItemDef) {
		.name = "　神裁の恩寵",
		.nameMsgId = MSG_SKILL_DivineReprieve_NAME,
		.helpMsgId = MSG_SKILL_DivineReprieve_DESC,
		.color = TEXT_COLOR_SYSTEM_GOLD,
		.isAvailable = DivineReprieve_Usability,
		.onDraw = NULL,
		.onSelected = DivineReprieve_OnSelected,
		.onIdle = NULL,
		.onSwitchIn = NULL,
		.onSwitchOut = NULL,
	},
#endif

#if (defined(SID_Replicate) && COMMON_SKILL_VALID(SID_Replicate))
	[SID_Replicate] = &(const struct MenuItemDef) {
		.name = "　召喚",
		.nameMsgId = MSG_SKILL_Replicate_NAME,
		.helpMsgId = MSG_SKILL_Replicate_DESC,
		.color = TEXT_COLOR_SYSTEM_GOLD,
		.isAvailable = Replicate_Usability,
		.onDraw = NULL,
		.onSelected = Replicate_OnSelected,
		.onIdle = NULL,
		.onSwitchIn = NULL,
		.onSwitchOut = NULL
	},
#endif
};
