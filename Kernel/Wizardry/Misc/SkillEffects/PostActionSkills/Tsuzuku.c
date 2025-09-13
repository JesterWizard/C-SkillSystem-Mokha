#include "common-chax.h"
#include "debuff.h"
#include "status-getter.h"
#include "action-expa.h"
#include "unit-expa.h"
#include "skill-system.h"
#include "battle-system.h"
#include "combat-art.h"
#include "kernel-lib.h"
#include "constants/skills.h"
#include "constants/combat-arts.h"

extern u8 gPostActionGaleforceFlag;

static void refreshUnit(struct Unit * unit, ProcPtr parent)
{
	if (!UnitAvaliable(unit) || UNIT_STONED(unit))
		return;

	gActionDataExpa.refrain_action = true;
	EndAllMus();
	StartStatusHealEffect(unit, parent);
	return;
}

bool PostActionTsuzuku(ProcPtr parent)
{
	FORCE_DECLARE struct Unit *unit = gActiveUnit;

	if (!UNIT_IS_VALID(unit) || CheckBitUES(unit, UES_BIT_TSZUKU_SKILL_USED))
		return false;

#ifdef CONFIG_MISC_UNIT_COUNTERS

#if defined(SID_BravelyDefault) && (COMMON_SKILL_VALID(SID_BravelyDefault))
	if (SkillTester(unit, SID_BravelyDefault) && unit->counters > 0)
	{
		unit->counters -= 1;
		refreshUnit(unit, parent);
	}
#endif

#endif

	switch (gActionData.unitActionType) {
	case UNIT_ACTION_COMBAT:
	case CONFIG_UNIT_ACTION_EXPA_GaidenMagicCombat:
#if defined(SID_Galeforce) && (COMMON_SKILL_VALID(SID_Galeforce))
		if (SkillListTester(unit, SID_Galeforce) && gBattleActorGlobalFlag.skill_activated_galeforce)
			goto L_exec_rafrain_action_anim;
#endif

#if defined(SID_FailGale) && (COMMON_SKILL_VALID(SID_FailGale))
		if (SkillListTester(unit, SID_FailGale) && !gBattleActor.nonZeroDamage)
			goto L_exec_rafrain_action_anim;
#endif
		if ((GetCombatArtInForce(unit) == CID_Galeforce) && gBattleActorGlobalFlag.enemy_defeated)
			goto L_exec_rafrain_action_anim;

	/* fall through */

	case UNIT_ACTION_STAFF:
	case CONFIG_UNIT_ACTION_EXPA_GaidenMagicStaff:
#if defined(SID_PowerStaff) && (COMMON_SKILL_VALID(SID_PowerStaff))
		if (CheckActiveUnitSkillActivate(SID_PowerStaff, GetUnitLuck(unit)))
			goto L_exec_rafrain_action_anim;
#endif

	/* fall through */

	case UNIT_ACTION_RESCUE:
#if defined(SID_Samaritan) && (COMMON_SKILL_VALID(SID_Samaritan))
		if (SkillTester(unit, SID_Samaritan))
			refreshUnit(unit, parent);
#endif

	/* fall through */

	case UNIT_ACTION_USE_ITEM:
#if defined(SID_QuickSalve) && (COMMON_SKILL_VALID(SID_QuickSalve))
		if (SkillTester(unit, SID_QuickSalve))
		{
			switch (GetItemIndex(gBattleActor.weaponBefore))
			{
			case ITEM_VULNERARY:
			case ITEM_VULNERARY_2:
			case ITEM_ELIXIR:
				refreshUnit(unit, parent);
				break;
			default:
				break;
			}
		}
#endif

	/* fall through */

	default:
#if defined(SID_Tsuzuku) && (COMMON_SKILL_VALID(SID_Tsuzuku))
		if (CheckActiveUnitSkillActivate(SID_Tsuzuku, GetUnitSkill(unit)))
			goto L_exec_rafrain_action_anim;
#endif

#if defined(SID_Turret) && (COMMON_SKILL_VALID(SID_Turret))
		if (SkillTester(unit, SID_Turret) && gActionData.moveCount == 0)
			refreshUnit(unit, parent);
#endif

#if defined(SID_SpellBlade) && (COMMON_SKILL_VALID(SID_SpellBlade))
		if (SkillTester(unit, SID_SpellBlade) && gActionData.unk08 == SID_SpellBlade)
			refreshUnit(unit, parent);
#endif
		break;

	case UNIT_ACTION_WAIT:
		break;
	}

	return false;

L_exec_rafrain_action_anim:
	if (!UnitAvaliable(unit) || UNIT_STONED(unit))
		return false;

	SetBitUES(unit, UES_BIT_TSZUKU_SKILL_USED);
	gActionDataExpa.refrain_action = true;
	EndAllMus();
	StartStatusHealEffect(unit, parent);
	return true;
}