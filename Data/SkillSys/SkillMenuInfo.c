#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"

#define MAX 0x410

const struct MenuItemDef gSkillMenuInfos[MAX] = {
#if (defined(SID_Dance) && COMMON_SKILL_VALID(SID_Dance))
    [SID_Dance] = {
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
    [SID_Steal] = {
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
    [SID_Summon] = {
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
    [SID_LockTouch] = {
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
    [SID_HealingFocus] = {
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
    [SID_Teleportation] = {
        .name = "　転移",
        .nameMsgId = MSG_SKILL_Teleportation_NAME,
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
    [SID_LightRune] = {
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
    [SID_Mine] = {
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
    [SID_RallyDefense] = {
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
    [SID_RallyLuck] = {
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
    [SID_RallyMagic] = {
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
    [SID_RallyMovement] = {
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
    [SID_RallyResistance] = {
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
    [SID_RallySkill] = {
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
    [SID_RallySpeed] = {
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
    [SID_RallyStrength] = {
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
    [SID_RallySpectrum] = {
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
    [SID_GoddessDance] = {
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
    [SID_Stride] = {
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
    [SID_Swarp] = {
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
    [SID_Blacksmith] = {
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

#if (defined(SID_Swap) && COMMON_SKILL_VALID(SID_Swap))
    [SID_Swap] = {
        .name = "　交換",
        .nameMsgId = MSG_SKILL_Swap_NAME,
        .helpMsgId = MSG_SKILL_Swap_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = Swap_Usability,
        .onDraw = NULL,
        .onSelected = Swap_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_Transform) && COMMON_SKILL_VALID(SID_Transform))
    [SID_Transform] = {
        .name = "　",
        .nameMsgId = MSG_SKILL_Transform_NAME,
        .helpMsgId = MSG_SKILL_Transform_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = Transform_Usability,
        .onDraw = NULL,
        .onSelected = Transform_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_DrawBack) && COMMON_SKILL_VALID(SID_DrawBack))
    [SID_DrawBack] = {
        .name = "　",
        .nameMsgId = MSG_SKILL_DrawBack_NAME,
        .helpMsgId = MSG_SKILL_DrawBack_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = DrawBack_Usability,
        .onDraw = NULL,
        .onSelected = DrawBack_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_Pivot) && COMMON_SKILL_VALID(SID_Pivot))
    [SID_Pivot] = {
        .name = "　",
        .nameMsgId = MSG_SKILL_Pivot_NAME,
        .helpMsgId = MSG_SKILL_Pivot_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = Pivot_Usability,
        .onDraw = NULL,
        .onSelected = Pivot_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_Reposition) && COMMON_SKILL_VALID(SID_Reposition))
    [SID_Reposition] = {
        .name = "　",
        .nameMsgId = MSG_SKILL_Reposition_NAME,
        .helpMsgId = MSG_SKILL_Reposition_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = Reposition_Usability,
        .onDraw = NULL,
        .onSelected = Reposition_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_Shove) && COMMON_SKILL_VALID(SID_Shove))
    [SID_Shove] = {
        .name = "　",
        .nameMsgId = MSG_SKILL_Shove_NAME,
        .helpMsgId = MSG_SKILL_Shove_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = Shove_Usability,
        .onDraw = NULL,
        .onSelected = Shove_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_Smite) && COMMON_SKILL_VALID(SID_Smite))
    [SID_Smite] = {
        .name = "　",
        .nameMsgId = MSG_SKILL_Smite_NAME,
        .helpMsgId = MSG_SKILL_Smite_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = Smite_Usability,
        .onDraw = NULL,
        .onSelected = Smite_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_StealPlus) && COMMON_SKILL_VALID(SID_StealPlus))
    [SID_StealPlus] = {
        .name = "　",
        .nameMsgId = MSG_SKILL_StealPlus_NAME,
        .helpMsgId = MSG_SKILL_StealPlus_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = StealPlus_Usability,
        .onDraw = NULL,
        .onSelected = StealPlus_Effect,
        .onIdle = 0,
        .onSwitchIn = 0,
        .onSwitchOut = 0
    },
#endif

#if (defined(SID_ArdentSacrifice) && COMMON_SKILL_VALID(SID_ArdentSacrifice))
    [SID_ArdentSacrifice] = {
        .name = "　",
        .nameMsgId = MSG_SKILL_ArdentSacrifice_NAME,
        .helpMsgId = MSG_SKILL_ArdentSacrifice_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = ArdentSacrifice_Usability,
        .onDraw = NULL,
        .onSelected = ArdentSacrifice_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_ReciprocalAid) && COMMON_SKILL_VALID(SID_ReciprocalAid))
    [SID_ReciprocalAid] = {
        .name = "　",
        .nameMsgId = MSG_SKILL_ReciprocalAid_NAME,
        .helpMsgId = MSG_SKILL_ReciprocalAid_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = ReciprocalAid_Usability,
        .onDraw = NULL,
        .onSelected = ReciprocalAid_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_Sacrifice) && COMMON_SKILL_VALID(SID_Sacrifice))
    [SID_Sacrifice] = {
        .name = "　",
        .nameMsgId = MSG_SKILL_Sacrifice_NAME,
        .helpMsgId = MSG_SKILL_Sacrifice_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = Sacrifice_Usability,
        .onDraw = NULL,
        .onSelected = Sacrifice_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_Capture) && COMMON_SKILL_VALID(SID_Capture))
    [SID_Capture] = {
        .name = "　",
        .nameMsgId = MSG_SKILL_Capture_NAME,
        .helpMsgId = MSG_SKILL_Capture_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = Capture_Usability,
        .onDraw = NULL,
        .onSelected = Capture_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_Doppleganger) && COMMON_SKILL_VALID(SID_Doppleganger))
    [SID_Doppleganger] = {
        .name = "　",
        .nameMsgId = MSG_SKILL_Doppleganger_NAME,
        .helpMsgId = MSG_SKILL_Doppleganger_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = Doppleganger_Usability,
        .onDraw = NULL,
        .onSelected = Doppleganger_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_Dismount) && COMMON_SKILL_VALID(SID_Dismount))
    [SID_Dismount] = {
        .name = "　",
        .nameMsgId = MSG_SKILL_Dismount_NAME,
        .helpMsgId = MSG_SKILL_Dismount_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = Dismount_Usability,
        .onDraw = NULL,
        .onSelected = Dismount_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_Entrepreneur) && COMMON_SKILL_VALID(SID_Entrepreneur))
    [SID_Entrepreneur] = {
        .name = "　",
        .nameMsgId = MSG_SKILL_Entrepreneur_NAME,
        .helpMsgId = MSG_SKILL_Entrepreneur_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = Entrepreneur_Usability,
        .onDraw = NULL,
        .onSelected = Entrepreneur_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_Arise) && COMMON_SKILL_VALID(SID_Arise))
    [SID_Arise] = {
        .name = "　",
        .nameMsgId = MSG_SKILL_Arise_NAME,
        .helpMsgId = MSG_SKILL_Arise_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = Arise_Usability,
        .onDraw = NULL,
        .onSelected = Arise_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_AssignDecoy) && COMMON_SKILL_VALID(SID_AssignDecoy))
    [SID_AssignDecoy] = {
        .name = "　",
        .nameMsgId = MSG_SKILL_AssignDecoy_NAME,
        .helpMsgId = MSG_SKILL_AssignDecoy_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = AssignDecoy_Usability,
        .onDraw = NULL,
        .onSelected = AssignDecoy_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_ShadowFlash) && COMMON_SKILL_VALID(SID_ShadowFlash))
    [SID_ShadowFlash] = {
        .name = "　光の結界",
        .nameMsgId = MSG_SKILL_ShadowFlash_NAME,
        .helpMsgId = MSG_SKILL_ShadowFlash_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = ShadowFlash_Usability,
        .onDraw = NULL,
        .onSelected = ShadowFlash_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = ShadowFlash_Hover,
        .onSwitchOut = ShadowFlash_Unhover,
    },
#endif

#if (defined(SID_DeathBlight) && COMMON_SKILL_VALID(SID_DeathBlight))
    [SID_DeathBlight] = {
        .name = "　光の結界",
        .nameMsgId = MSG_SKILL_DeathBlight_NAME,
        .helpMsgId = MSG_SKILL_DeathBlight_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = DeathBlight_Usability,
        .onDraw = NULL,
        .onSelected = DeathBlight_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_DivineReprieve) && COMMON_SKILL_VALID(SID_DivineReprieve))
	[SID_DivineReprieve] = {
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

#if (defined(SID_LoadstarRush) && COMMON_SKILL_VALID(SID_LoadstarRush))
    [SID_LoadstarRush] = {
        .name = "　光の結界",
        .nameMsgId = MSG_SKILL_LoadstarRush_NAME,
        .helpMsgId = MSG_SKILL_LoadstarRush_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = LoadstarRush_Usability,
        .onDraw = NULL,
        .onSelected = LoadstarRush_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_WyvernCrash) && COMMON_SKILL_VALID(SID_WyvernCrash))
    [SID_WyvernCrash] = {
        .name = "　光の結界",
        .nameMsgId = MSG_SKILL_WyvernCrash_NAME,
        .helpMsgId = MSG_SKILL_WyvernCrash_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = WyvernCrash_Usability,
        .onDraw = NULL,
        .onSelected = WyvernCrash_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_Sellsword) && COMMON_SKILL_VALID(SID_Sellsword))
    [SID_Sellsword] = {
        .name = "　光の結界",
        .nameMsgId = MSG_SKILL_Sellsword_NAME,
        .helpMsgId = MSG_SKILL_Sellsword_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = Sellsword_Usability,
        .onDraw = NULL,
        .onSelected = Sellsword_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_Switcher) && COMMON_SKILL_VALID(SID_Switcher))
    [SID_Switcher] = {
        .name = "　光の結界",
        .nameMsgId = MSG_SKILL_Switcher_NAME,
        .helpMsgId = MSG_SKILL_Switcher_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = Switcher_Usability,
        .onDraw = NULL,
        .onSelected = Switcher_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_SummonPlus) && COMMON_SKILL_VALID(SID_SummonPlus))
    [SID_SummonPlus] = {
        .name = "　召喚",
        .nameMsgId = MSG_SKILL_SummonPlus_NAME,
        .helpMsgId = MSG_SKILL_SummonPlus_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = SummonCommandUsability,
        .onDraw = NULL,
        .onSelected = SummonPlusCommandEffect,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL
    },
#endif

#if (defined(SID_LethalityPlus) && COMMON_SKILL_VALID(SID_LethalityPlus))
    [SID_LethalityPlus] = {
        .name = "　光の結界",
        .nameMsgId = MSG_SKILL_LethalityPlus_NAME,
        .helpMsgId = MSG_SKILL_LethalityPlus_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = LethalityPlus_Usability,
        .onDraw = NULL,
        .onSelected = LethalityPlus_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_GrimReaper) && COMMON_SKILL_VALID(SID_GrimReaper))
    [SID_GrimReaper] = {
        .name = "　光の結界",
        .nameMsgId = MSG_SKILL_GrimReaper_NAME,
        .helpMsgId = MSG_SKILL_GrimReaper_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = GrimReaper_Usability,
        .onDraw = NULL,
        .onSelected = GrimReaper_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_Hide) && COMMON_SKILL_VALID(SID_Hide))
    [SID_Hide] = {
        .name = "　光の結界",
        .nameMsgId = MSG_SKILL_Hide_NAME,
        .helpMsgId = MSG_SKILL_Hide_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = Hide_Usability,
        .onDraw = NULL,
        .onSelected = Hide_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_SkillSwap) && COMMON_SKILL_VALID(SID_SkillSwap))
    [SID_SkillSwap] = {
        .name = "　",
        .nameMsgId = MSG_SKILL_SkillSwap_NAME,
        .helpMsgId = MSG_SKILL_SkillSwap_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = SkillSwap_Usability,
        .onDraw = NULL,
        .onSelected = SkillSwap_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_Shuffle) && COMMON_SKILL_VALID(SID_Shuffle))
    [SID_Shuffle] = {
        .name = "　",
        .nameMsgId = MSG_SKILL_Shuffle_NAME,
        .helpMsgId = MSG_SKILL_Shuffle_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = Shuffle_Usability,
        .onDraw = NULL,
        .onSelected = Shuffle_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_ShufflePlus) && COMMON_SKILL_VALID(SID_ShufflePlus))
    [SID_ShufflePlus] = {
        .name = "　",
        .nameMsgId = MSG_SKILL_ShufflePlus_NAME,
        .helpMsgId = MSG_SKILL_ShufflePlus_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = ShufflePlus_Usability,
        .onDraw = NULL,
        .onSelected = ShufflePlus_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_Persuade) && COMMON_SKILL_VALID(SID_Persuade))
    [SID_Persuade] = {
        .name = "　",
        .nameMsgId = MSG_SKILL_Persuade_NAME,
        .helpMsgId = MSG_SKILL_Persuade_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = Persuade_Usability,
        .onDraw = NULL,
        .onSelected = Persuade_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_PersuadePlus) && COMMON_SKILL_VALID(SID_PersuadePlus))
    [SID_PersuadePlus] = {
        .name = "　",
        .nameMsgId = MSG_SKILL_PersuadePlus_NAME,
        .helpMsgId = MSG_SKILL_PersuadePlus_DESC,
        .color = TEXT_COLOR_SYSTEM_GOLD,
        .isAvailable = PersuadePlus_Usability,
        .onDraw = NULL,
        .onSelected = PersuadePlus_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

    [0] = {
        .name = "　光の結界",
        .nameMsgId = MSG_MenuCommand_Refuge_NAME,
        .helpMsgId = MSG_MenuCommand_Refuge_DESC,
        .color = TEXT_COLOR_SYSTEM_WHITE,
        .isAvailable = Refuge_Usability,
        .onDraw = NULL,
        .onSelected = Refuge_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
};
