#include "common-chax.h"
#include "battle-system.h"
#include "skill-system.h"
#include "unit-expa.h"
#include "kernel-lib.h"
#include "combat-art.h"
#include "efx-skill.h"
#include "strmag.h"
#include "kernel-tutorial.h"
#include "constants/skills.h"
#include "constants/combat-arts.h"
#include "debuff.h"

#define LOCAL_TRACE 0

typedef void (*BattleDamageCalcFunc) (struct BattleUnit* buA, struct BattleUnit* buB);
extern BattleDamageCalcFunc const* const gpBattleDamageCalcFuncs;

typedef int (*BattleRealDamageCalcFunc) (int old, struct BattleUnit* buA, struct BattleUnit* buB);
extern BattleRealDamageCalcFunc const* const gpBattleRealDamageCalcFuncs;

int CalcBattleRealDamage(struct BattleUnit* attacker, struct BattleUnit* defender)
{
	const BattleRealDamageCalcFunc* it;

	int damage = 0;

#if defined(SID_RuinedBlade) && (COMMON_SKILL_VALID(SID_RuinedBlade))
	if (BattleFastSkillTester(attacker, SID_RuinedBlade))
		damage += SKILL_EFF2(SID_RuinedBlade);
#endif

#if defined(SID_RuinedBladePlus) && (COMMON_SKILL_VALID(SID_RuinedBladePlus))
	if (BattleFastSkillTester(attacker, SID_RuinedBladePlus))
		damage += SKILL_EFF1(SID_RuinedBladePlus);
#endif

#if defined(SID_LunaAttack) && (COMMON_SKILL_VALID(SID_LunaAttack))
	if (BattleFastSkillTester(attacker, SID_LunaAttack))
		damage += perc_of(defender->battleDefense, SKILL_EFF0(SID_LunaAttack));
#endif

#if defined(SID_Bushido) && (COMMON_SKILL_VALID(SID_Bushido))
	if (BattleFastSkillTester(attacker, SID_Bushido))
		damage += SKILL_EFF1(SID_Bushido);
#endif

#if defined(SID_Scendscale) && (COMMON_SKILL_VALID(SID_Scendscale))
	if (BattleFastSkillTester(attacker, SID_Scendscale))
		damage += perc_of(attacker->battleAttack, SKILL_EFF0(SID_Scendscale));
#endif

	for (it = gpBattleRealDamageCalcFuncs; *it; it++)
		damage = (*it)(damage, attacker, defender);

	return damage;
}

int GetMaxDamage(struct BattleUnit* attacker, struct BattleUnit* defender)
{
	/* TODO: better to put this to designer config */
	return CONFIG_BATTLE_MAX_DAMAGE;
}

void PreBattleCalcInit_BaseDamage(struct BattleUnit* attacker, struct BattleUnit* defender)
{
	gActorBaseDmg.increase = 100;
	gActorBaseDmg.decrease = 0x100;
	gActorBaseDmg.real_damage = CalcBattleRealDamage(&gBattleActor, &gBattleTarget);

	gTargetBaseDmg.increase = 100;
	gTargetBaseDmg.decrease = 0x100;
	gTargetBaseDmg.real_damage = CalcBattleRealDamage(&gBattleTarget, &gBattleActor);
}

int BattleHit_CalcDamage(struct BattleUnit* attacker, struct BattleUnit* defender)
{
	const BattleDamageCalcFunc* it;

	FORCE_DECLARE bool barricadePlus_activated;
	int base_damage, crit_correction, result;
	struct BaseDmg* base_dmg = GetBaseDmg(attacker);
	int max_damage = GetMaxDamage(attacker, defender);

	FORCE_DECLARE struct BattleGlobalFlags* act_flags, * tar_flags;

	/**
	 * result = ([atk + gDmg.correction - def])
	 *			  * (100% + increase%)
	 *			  * (100% + crit_correction%)
	 *			  / (100% + decrease%)
	 *			  + real_damage
	 */

	if (attacker == &gBattleActor) {
		act_flags = &gBattleActorGlobalFlag;
		tar_flags = &gBattleTargetGlobalFlag;
	}
	else {
		act_flags = &gBattleTargetGlobalFlag;
		tar_flags = &gBattleActorGlobalFlag;
	}

	gDmg.crit_atk = false;
	gDmg.correction = 0;
	gDmg.crit_correction = gpKernelBattleDesignerConfig->crit_correction;

	gDmg.increase = base_dmg->increase;
	gDmg.decrease = base_dmg->decrease;
	gDmg.real_damage = base_dmg->real_damage;

	/**
	 * Roll critical and silencer attack
	 */
	if (BattleRoll1RN(gBattleStats.critRate, false)) {
		gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_CRIT;
		gDmg.crit_atk = true;

		if (BattleRoll1RN(gBattleStats.silencerRate, false)) {
			/* Directly return on silencer attack to fasten calc */
			gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_SILENCER;
			gDmg.result = max_damage;
			return max_damage;
		}
	}


	/**
	 * calc-loop
	 */
	for (it = gpBattleDamageCalcFuncs; *it; it++)
		(*it)(attacker, defender);

	/**
	 * Calculate result
	 */
	if (gBattleHitIterator->attributes & BATTLE_HIT_ATTR_SILENCER) {
		gDmg.result = max_damage;
		return max_damage;
	}
	else if (gBattleHitIterator->attributes & BATTLE_HIT_ATTR_GREATSHLD) {
#if defined(SID_DamageToMP) && (COMMON_SKILL_VALID(SID_DamageToMP))
		if (gActionData.unk08 != SID_DamageToMP)
		{
			gDmg.result = 0;
			return 0;
		}
#else
		gDmg.result = 0;
		return 0;
#endif
	}

	base_damage = gBattleStats.attack + gDmg.correction - gBattleStats.defense;
	crit_correction = (gDmg.crit_atk) ? gDmg.crit_correction : 100;

	if (base_damage <= 0)
		base_damage = 0;

	if (base_damage <= 0 && gDmg.real_damage <= 0) {
		/* If no damage taken, directly end the damage calculation */
		gDmg.result = 0;
		return 0;
	}

	/**
	 * Step6: Calculate result
	 */
	{
		int dividend;

		dividend = k_udiv(base_damage * gDmg.increase * crit_correction, 100 * 100);

		if (dividend > max_damage)
			dividend = max_damage;

		result = k_udiv(dividend * 0x100, gDmg.decrease);
	}

	if (result == 0 && base_damage > 0)
		result = 1; // at least 1 damage left.

	result += gDmg.real_damage;

	/* Reduce incoming damage by 25% if the defending unit has the 'default' status */
#if defined(SID_BravelyDefault) && (COMMON_SKILL_VALID(SID_BravelyDefault))
	if (BattleFastSkillTester(defender, SID_BravelyDefault))
		if (GetUnitStatusIndex(GetUnit(attacker->unit.index)) == NEW_UNIT_STATUS_DEFAULT)
			result -= (result / 4);
#endif

	LTRACEF("[round %d] dmg=%d: max=%d, base=%d (atk=%d, def=%d, cor=%d), inc=%d, crt=%d, dec=%d, real=%d",
		GetBattleHitRound(gBattleHitIterator), result, max_damage, base_damage,
		gBattleStats.attack, gBattleStats.defense, gDmg.correction, gDmg.increase, crit_correction, gDmg.decrease, gDmg.real_damage);

	if (result > max_damage)
		result = max_damage;

	gDmg.result = result;
	return result;
}
