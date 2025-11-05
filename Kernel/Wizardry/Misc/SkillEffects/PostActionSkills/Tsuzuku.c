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
#include "playst-expa.h"

extern u8 gPostActionGaleforceFlag;

FORCE_DECLARE static void refresh_turn_once(struct Unit * unit, ProcPtr parent)
{
    if (!UNIT_ALIVE(unit) || UNIT_STONED(unit))
        return;

    gActionDataExpa.refrain_action = true;
    EndAllMus();
    StartStatusHealEffect(unit, parent);
    return;
}

FORCE_DECLARE static void refresh_turn_repeatedly(struct Unit * unit, ProcPtr parent)
{
    if (!UNIT_ALIVE(unit) || UNIT_STONED(unit))
        return;

    gActionDataExpa.refrain_action = true;
    EndAllMus();
    StartStatusHealEffect(unit, parent);
}

FORCE_DECLARE static void refresh_turn_repeatedly_no_animation(struct Unit * unit, ProcPtr parent)
{
    if (!UNIT_ALIVE(unit) || UNIT_STONED(unit))
        return;

    gActionDataExpa.refrain_action = true;
}

FORCE_DECLARE static void refresh_turn_once_aura(struct Unit * unit, ProcPtr parent)
{
    for (int i = 0; i < ARRAY_COUNT_RANGE1x1; i++)
    {
        int _x = unit->xPos + gVecs_1x1[i].x;
        int _y = unit->yPos + gVecs_1x1[i].y;

        struct Unit *unit_ally = GetUnitAtPosition(_x, _y);

        if (!UNIT_IS_VALID(unit_ally) || UNIT_STONED(unit_ally) || !AreUnitsAllied(unit->index, unit_ally->index))
            continue;
        unit_ally->state &= ~(US_UNSELECTABLE | US_HAS_MOVED | US_HAS_MOVED_AI);
    }
}

bool PostActionTsuzuku(ProcPtr parent)
{
	FORCE_DECLARE struct Unit *unit = gActiveUnit;

	if (!UNIT_IS_VALID(unit))
		return false;

#ifdef CONFIG_MISC_UNIT_COUNTERS

#if defined(SID_BravelyDefault) && (COMMON_SKILL_VALID(SID_BravelyDefault))
	if (SkillTester(unit, SID_BravelyDefault) && unit->counters > 0)
	{
		unit->counters -= 1;
		refresh_turn_once(unit, parent);
	}
#endif

#endif

#if defined(SID_Cultured) && (COMMON_SKILL_VALID(SID_Cultured))
    bool nice_thighs = false;

    if (SkillTester(unit, SID_Cultured))
        for (int i = 0; i < ARRAY_COUNT_RANGE1x1; i++)
        {
            int _x = unit->xPos + gVecs_1x1[i].x;
            int _y = unit->yPos + gVecs_1x1[i].y;

            unit = GetUnitAtPosition(_x, _y);

#if defined(SID_NiceThighs) && (COMMON_SKILL_VALID(SID_NiceThighs))
            if (SkillTester(unit, SID_NiceThighs))
            {
                nice_thighs = true;
                break;
            }
#endif
        }

    if (nice_thighs)
        refresh_turn_once(unit, parent);
#endif

#if defined(SID_AdrenalineRush) && (COMMON_SKILL_VALID(SID_AdrenalineRush))
            if (SkillTester(unit, SID_AdrenalineRush))
            {
                if (unit->curHP <= unit->maxHP / 4)
                    refresh_turn_once(unit, parent);
            }
#endif

#if defined(SID_SpiritedSteps) && (COMMON_SKILL_VALID(SID_SpiritedSteps) && defined(SID_Dance) && (COMMON_SKILL_VALID(SID_Dance)))
            if (SkillTester(unit, SID_SpiritedSteps))
            {
                if (gActionData.unk08 == SID_Dance && gBattleActorGlobalFlag.skill_activated_dance)
                {
                    if (NextRN_100() <= unit->level)
                    {
                        /* A bit of a hack to prevent this skill from triggering on other commands afterwards */
                        gBattleActorGlobalFlag.skill_activated_dance = false;
                        refresh_turn_repeatedly(unit, parent);
                    }
                }
            }
#endif

#if defined(SID_Switcher) && (COMMON_SKILL_VALID(SID_Switcher))
            if (SkillTester(unit, SID_Switcher))
            {
                 if (gActionData.unk08 == SID_Switcher)
                    refresh_turn_repeatedly_no_animation(unit, parent);
            }
#endif

	switch (gActionData.unitActionType) {
	case UNIT_ACTION_COMBAT:
	case CONFIG_UNIT_ACTION_EXPA_GaidenMagicCombat:
#if defined(SID_Galeforce) && (COMMON_SKILL_VALID(SID_Galeforce))
        if (SkillTester(unit, SID_Galeforce) && gBattleActorGlobalFlag.skill_activated_galeforce)
            refresh_turn_once(unit, parent);
#endif

#if defined(SID_FailGale) && (COMMON_SKILL_VALID(SID_FailGale))
        if (SkillTester(unit, SID_FailGale) && !gBattleActor.nonZeroDamage)
            refresh_turn_once(unit, parent);
#endif

#if defined(SID_LeadByExample) && (COMMON_SKILL_VALID(SID_LeadByExample))
        if (SkillTester(unit, SID_LeadByExample) && gBattleActorGlobalFlag.skill_activated_lead_by_example)
            refresh_turn_once_aura(unit, parent);
#endif

#if defined(SID_CoinFlip) && (COMMON_SKILL_VALID(SID_CoinFlip))
        if (SkillTester(unit, SID_CoinFlip) && PlayStExpa_CheckBit(PLAYSTEXPA_BIT_CoinFlip_Used))
        {
            refresh_turn_once(unit, parent);
            PlayStExpa_ClearBit(PLAYSTEXPA_BIT_CoinFlip_Used);
        }
#endif

#if defined(SID_DoubleTime) && (COMMON_SKILL_VALID(SID_DoubleTime))
        if (PlayStExpa_CheckBit(PLAYSTEXPA_BIT_DoubleTime_Used))
        {
            refresh_turn_once(unit, parent);
            PlayStExpa_ClearBit(PLAYSTEXPA_BIT_DoubleTime_Used);
        }
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

#if defined(SID_CoinFlip) && (COMMON_SKILL_VALID(SID_CoinFlip))
        if (SkillTester(unit, SID_CoinFlip) && PlayStExpa_CheckBit(PLAYSTEXPA_BIT_CoinFlip_Used))
        {
            refresh_turn_once(unit, parent);
            PlayStExpa_ClearBit(PLAYSTEXPA_BIT_CoinFlip_Used);
        }
#endif

#if defined(SID_DoubleTime) && (COMMON_SKILL_VALID(SID_DoubleTime))
        if (PlayStExpa_CheckBit(PLAYSTEXPA_BIT_DoubleTime_Used))
        {
            refresh_turn_once(unit, parent);
            PlayStExpa_ClearBit(PLAYSTEXPA_BIT_DoubleTime_Used);
        }
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
				refresh_turn_once(unit, parent);
				break;
			default:
				break;
			}
		}
#endif

#if defined(SID_QuickHands) && (COMMON_SKILL_VALID(SID_QuickHands))
        if (SkillTester(unit, SID_QuickHands))
            refresh_turn_repeatedly(unit, parent);
#endif

#if defined(SID_CoinFlip) && (COMMON_SKILL_VALID(SID_CoinFlip))
        if (SkillTester(unit, SID_CoinFlip) && PlayStExpa_CheckBit(PLAYSTEXPA_BIT_CoinFlip_Used))
        {
            refresh_turn_once(unit, parent);
            PlayStExpa_ClearBit(PLAYSTEXPA_BIT_CoinFlip_Used);
        }
#endif

#if defined(SID_DoubleTime) && (COMMON_SKILL_VALID(SID_DoubleTime))
        if (PlayStExpa_CheckBit(PLAYSTEXPA_BIT_DoubleTime_Used))
        {
            refresh_turn_once(unit, parent);
            PlayStExpa_ClearBit(PLAYSTEXPA_BIT_DoubleTime_Used);
        }
#endif

	/* fall through */

	case UNIT_ACTION_RESCUE:
#if defined(SID_Samaritan) && (COMMON_SKILL_VALID(SID_Samaritan))
		if (SkillTester(unit, SID_Samaritan))
			refresh_turn_once(unit, parent);
#endif

#if defined(SID_Heroics) && (COMMON_SKILL_VALID(SID_Heroics))
        if (SkillTester(unit, SID_Heroics))
            refresh_turn_repeatedly(unit, parent);
#endif

#if defined(SID_CoinFlip) && (COMMON_SKILL_VALID(SID_CoinFlip))
        if (SkillTester(unit, SID_CoinFlip) && PlayStExpa_CheckBit(PLAYSTEXPA_BIT_CoinFlip_Used))
        {
            refresh_turn_once(unit, parent);
            PlayStExpa_ClearBit(PLAYSTEXPA_BIT_CoinFlip_Used);
        }
#endif

#if defined(SID_DoubleTime) && (COMMON_SKILL_VALID(SID_DoubleTime))
        if (PlayStExpa_CheckBit(PLAYSTEXPA_BIT_DoubleTime_Used))
        {
            refresh_turn_once(unit, parent);
            PlayStExpa_ClearBit(PLAYSTEXPA_BIT_DoubleTime_Used);
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
			refresh_turn_once(unit, parent);
#endif

#if defined(SID_SpellBlade) && (COMMON_SKILL_VALID(SID_SpellBlade))
		if (SkillTester(unit, SID_SpellBlade) && gActionData.unk08 == SID_SpellBlade)
			refresh_turn_once(unit, parent);
#endif
		break;

	case UNIT_ACTION_WAIT:
#if defined(SID_CoinFlip) && (COMMON_SKILL_VALID(SID_CoinFlip))
        if (SkillTester(unit, SID_CoinFlip) && PlayStExpa_CheckBit(PLAYSTEXPA_BIT_CoinFlip_Used))
        {
            refresh_turn_once(unit, parent);
            PlayStExpa_ClearBit(PLAYSTEXPA_BIT_CoinFlip_Used);
        }
#endif

#if defined(SID_DoubleTime) && (COMMON_SKILL_VALID(SID_DoubleTime))
        if (PlayStExpa_CheckBit(PLAYSTEXPA_BIT_DoubleTime_Used))
        {
            refresh_turn_once(unit, parent);
            PlayStExpa_ClearBit(PLAYSTEXPA_BIT_DoubleTime_Used);
        }
#endif
		break;
	}

	return false;

L_exec_rafrain_action_anim:
	if (!UnitAvaliable(unit) || UNIT_STONED(unit))
		return false;

	// SetBitUES(unit, UES_BIT_TSZUKU_SKILL_USED);
	gActionDataExpa.refrain_action = true;
	EndAllMus();
	StartStatusHealEffect(unit, parent);
	return true;
}