#include "common-chax.h"
#include "skill-system.h"
#include "efx-skill.h"
#include "battle-system.h"
#include "strmag.h"
#include "debuff.h"
#include "feb-compat.h"
#include "kernel-tutorial.h"
#include "constants/skills.h"

#define LOCAL_TRACE 0

bool CheckBattleHpHalve(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_HPHALVE)
		return true;

#if (defined(SID_Eclipse) && (COMMON_SKILL_VALID(SID_Eclipse)))
	if (CheckBattleSkillActivate(attacker, defender, SID_Eclipse, attacker->unit.skl)) {
		RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Eclipse);
		return true;
	}
#endif

	return false;
}

bool CheckDevilAttack(struct BattleUnit *attacker, struct BattleUnit *defender)
{
#if (defined(SID_Counter) && (COMMON_SKILL_VALID(SID_Counter)))
	if (BattleFastSkillTester(defender, SID_Counter) && gBattleStats.range == 1 && !IsMagicAttack(attacker)) {
		RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Counter);
		return true;
	}
#endif

#if (defined(SID_CounterMagic) && (COMMON_SKILL_VALID(SID_CounterMagic)))
	if (BattleFastSkillTester(defender, SID_CounterMagic) && gBattleStats.range >= 2 && IsMagicAttack(attacker)) {
		RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_CounterMagic);
		return true;
	}
#endif

	if (!RollBaseDevilCheck(attacker)) {
		/* Lucky */
		return false;
	}

#if (defined(SID_DevilsLuck) && (COMMON_SKILL_VALID(SID_DevilsLuck)))
	if (BattleFastSkillTester(defender, SID_DevilsLuck) && GetItemWeaponEffect(defender->weapon) == WPN_EFFECT_DEVIL) {
		RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DevilsLuck);
		return true;
	}

	if (BattleFastSkillTester(attacker, SID_DevilsLuck) && GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_DEVIL)
		return false;
#endif

	if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_DEVIL)
		return true;

#if (defined(SID_DevilsPact) && (COMMON_SKILL_VALID(SID_DevilsPact)))
	if (BattleFastSkillTester(defender, SID_DevilsPact)) {
		RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DevilsPact);
		return true;
	}
#endif

#if (defined(SID_DevilsWhim) && (COMMON_SKILL_VALID(SID_DevilsWhim)))
	if (BattleFastSkillTester(defender, SID_DevilsWhim)) {
		RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DevilsWhim);
		return true;
	}

	if (BattleFastSkillTester(attacker, SID_DevilsWhim)) {
		RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DevilsWhim);
		return true;
	}
#endif

	return false;
}

bool CheckBattleInori(struct BattleUnit *attacker, struct BattleUnit *defender)
{
#if (defined(SID_Mercy) && (COMMON_SKILL_VALID(SID_Mercy)))
	if (CheckBattleSkillActivate(attacker, defender, SID_Mercy, attacker->unit.skl)) {
		RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Mercy);
		return true;
	}
#endif

#if (defined(SID_Miracle) && (COMMON_SKILL_VALID(SID_Miracle)))
	if (CheckBattleSkillActivate(defender, attacker, SID_Miracle, defender->unit.lck)) {
		RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Miracle);
		return true;
	}
#endif

#if (defined(SID_Sturdy) && (COMMON_SKILL_VALID(SID_Sturdy)))
	if (BattleFastSkillTester(defender, SID_Sturdy)) {
		if (defender->unit.maxHP == defender->hpInitial) {
			RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Sturdy);
			return true;
		}
	}
#endif

#if (defined(SID_LEGEND_MiracleAtk) && (COMMON_SKILL_VALID(SID_LEGEND_MiracleAtk)))
	if (CheckBattleSkillActivate(defender, attacker, SID_LEGEND_MiracleAtk, 100)) {
		if (TryActivateLegendSkill(&defender->unit, SID_LEGEND_MiracleAtk) == 0) {
			RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_LEGEND_MiracleAtk);
			return true;
		}
	}
#endif

#if (defined(SID_LEGEND_MiracleAvo) && (COMMON_SKILL_VALID(SID_LEGEND_MiracleAvo)))
	if (CheckBattleSkillActivate(defender, attacker, SID_LEGEND_MiracleAvo, 100)) {
		if (TryActivateLegendSkill(&defender->unit, SID_LEGEND_MiracleAvo) == 0) {
			RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_LEGEND_MiracleAvo);
			return true;
		}
	}
#endif

#if (defined(SID_LEGEND_MiracleDef) && (COMMON_SKILL_VALID(SID_LEGEND_MiracleDef)))
	if (CheckBattleSkillActivate(defender, attacker, SID_LEGEND_MiracleDef, 100)) {
		if (TryActivateLegendSkill(&defender->unit, SID_LEGEND_MiracleDef) == 0) {
			RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_LEGEND_MiracleDef);
			return true;
		}
	}
#endif

	return false;
}

/**
 * This is an additional API
 */
void AppendHpDrain(struct BattleUnit *attacker, struct BattleUnit *defender, int drain)
{
	struct ExtBattleHit *ext_hit = GetCurrentExtBattleHit();
	int modified = ext_hit->hp_drain + drain;

	if (attacker->unit.maxHP < (attacker->unit.curHP + modified))
		modified = attacker->unit.maxHP - attacker->unit.curHP;

	if (modified > 0) {
		attacker->unit.curHP += modified;
		ext_hit->hp_drain += modified;
		gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_HPSTEAL;
	}

	LTRACEF("drain=%d, modified=%d, total=%d", drain, modified, ext_hit->hp_drain);
}

void BattleHit_CalcWeaponHpCost(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	int cost = gpWeaponHpCostConfig[ITEM_INDEX(attacker->weapon)];

	if (cost > 0) {
		int round = GetCurrentBattleHitRound();

		if (cost >= attacker->unit.curHP)
			cost = attacker->unit.curHP - 1;

		AddBattleHpCost(attacker, round, cost);
	}
}

void BattleHit_CalcHpDrain(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	struct ExtBattleHit *ext_hit = GetCurrentExtBattleHit();
	int percentage = 0;
	int drain = ext_hit->hp_drain;

#ifdef CONFIG_AUTO_DETECT_EFXRESIRE_WEAPON
	if (CheckWeaponIsEfxResire(attacker->weapon))
#else
	if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_HPDRAIN)
#endif
	{
		/**
		 * If the weapon itself is set as hpdrain,
		 * then it may directly call EfxHpBarResire() in banim,
		 * at which time we must set hp-steal flag for battle-parse.
		 */
		gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_HPSTEAL;
	}

	/**
	 * Step 1: calculate drain percentage
	 */
	if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_HPDRAIN) {
		int weapon_drain = gpWeaponHpDrainConfig[ITEM_INDEX(attacker->weapon)];

		if (weapon_drain == 0)
			weapon_drain = gpKernelBattleDesignerConfig->nosferatu_hpdrain_perc;

		percentage += weapon_drain;
	}

	if (gBattleTemporaryFlag.skill_activated_aether)
		percentage += 100;

#if (defined(SID_Sol) && (COMMON_SKILL_VALID(SID_Sol)))
	if (CheckBattleSkillActivate(attacker, defender, SID_Sol, attacker->unit.skl)) {
		RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Sol);
		percentage += 100;
	}
#endif

	if (percentage == 0)
		return;

	/**
	 * Step 2: calculate real amount
	 */
	drain += k_udiv(gBattleStats.damage * percentage, 100);

	/**
	 * Step 3: detect overflow
	 */
	if (attacker->unit.maxHP < (attacker->unit.curHP + drain))
		drain = attacker->unit.maxHP - attacker->unit.curHP;

	LTRACEF("hpdrain: dmg=%d, perc=%d, drain=%d, cur=%d, max=%d",
		gBattleStats.damage, percentage, drain, attacker->unit.curHP, attacker->unit.maxHP);

	if (drain > 0) {
		GetCurrentExtBattleHit()->hp_drain = drain;
		attacker->unit.curHP += drain;
		gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_HPSTEAL;
	}
}

STATIC_DECLAR void InjectNegativeStatusExt(struct BattleUnit *bu, int debuff)
{
	int old_debuff;

	if (debuff < 0)
		return;

	old_debuff = (bu->statusOut >= 0)
			   ? bu->statusOut
			   : GetUnitStatusIndex(&bu->unit);

	switch (old_debuff) {
	case UNIT_STATUS_PETRIFY:
	case UNIT_STATUS_13:
		// "Ungray" defender if it was petrified (as it won't be anymore)
		bu->unit.state &= ~US_UNSELECTABLE;
		break;

	default:
		break;
	}

	bu->statusOut = debuff;

	switch (debuff) {
	case UNIT_STATUS_PETRIFY:
	case UNIT_STATUS_13:
		switch (gPlaySt.faction) {
		case FACTION_BLUE:
			if (UNIT_FACTION(&bu->unit) == FACTION_BLUE)
				bu->statusOut = UNIT_STATUS_13;
			else
				bu->statusOut = UNIT_STATUS_PETRIFY;
			break;

		case FACTION_RED:
			if (UNIT_FACTION(&bu->unit) == FACTION_RED)
				bu->statusOut = UNIT_STATUS_13;
			else
				bu->statusOut = UNIT_STATUS_PETRIFY;
			break;

		case FACTION_GREEN:
			if (UNIT_FACTION(&bu->unit) == FACTION_GREEN)
				bu->statusOut = UNIT_STATUS_13;
			else
				bu->statusOut = UNIT_STATUS_PETRIFY;
			break;
		}
		gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_PETRIFY;
		break;

	case UNIT_STATUS_POISON:
		gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_POISON;
		break;

	default:
		break;
	}
}

void BattleHit_InjectNegativeStatus(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	struct ExtBattleHit *exthit = GetCurrentExtBattleHit();

	exthit->act_debuff = -1;
	exthit->tar_debuff = -1;

	if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_PETRIFY)
		exthit->tar_debuff = UNIT_STATUS_PETRIFY;
#if (defined(SID_Petrify) && (COMMON_SKILL_VALID(SID_Petrify)))
	else if (CheckBattleSkillActivate(attacker, defender, SID_Petrify, attacker->unit.skl)) {
		RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Petrify);
		exthit->tar_debuff = UNIT_STATUS_PETRIFY;
	}
#endif
	else if (gBattleTemporaryFlag.skill_activated_dead_eye)
		exthit->tar_debuff = UNIT_STATUS_SLEEP;
	else if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_POISON)
		exthit->tar_debuff = UNIT_STATUS_POISON;

#if (defined(SID_PoisonPoint) && (COMMON_SKILL_VALID(SID_PoisonPoint)))
	else if (BattleFastSkillTester(attacker, SID_PoisonPoint))
		exthit->tar_debuff = UNIT_STATUS_POISON;
#endif

#if (defined(SID_Enrage) && (COMMON_SKILL_VALID(SID_Enrage)))
	else if (CheckBattleSkillActivate(attacker, defender, SID_Enrage, attacker->unit.skl)) {
		RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Enrage);
		exthit->tar_debuff = UNIT_STATUS_BERSERK;
	}
#endif

#if (defined(SID_EffectSpore) && (COMMON_SKILL_VALID(SID_EffectSpore)))
	else if (CheckBattleSkillActivate(defender, attacker, SID_EffectSpore, SKILL_EFF0(SID_EffectSpore))) {
		if (!IsDebuff(GetUnitStatusIndex(&attacker->unit)) && !IsDebuff(attacker->statusOut)) {
			static const u8 _debuffs[3] = { UNIT_STATUS_POISON, UNIT_STATUS_SILENCED, UNIT_STATUS_SLEEP };

			exthit->act_debuff = _debuffs[NextRN_N(ARRAY_COUNT(_debuffs))];
			RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_EffectSpore);
		}
	}
#endif

#if (defined(SID_BlackMagic) && (COMMON_SKILL_VALID(SID_BlackMagic)))
	else if (CheckBattleSkillActivate(attacker, defender, SID_BlackMagic, attacker->unit.skl)) {
		static const u8 _debuffs[] = {
			// Vanilla
			UNIT_STATUS_POISON,
			UNIT_STATUS_SLEEP,
			UNIT_STATUS_SILENCED,
			UNIT_STATUS_BERSERK,

			// CHAX
			NEW_UNIT_STATUS_PIERCE_ARMOR,
			NEW_UNIT_STATUS_PIERCE_MAGIC,
			NEW_UNIT_STATUS_HEAVY_GRAVITY,
			NEW_UNIT_STATUS_WEAKEN,
			NEW_UNIT_STATUS_BREAK,
			NEW_UNIT_STATUS_TOXIC_POISON,
			NEW_UNIT_STATUS_DOOM,
			NEW_UNIT_STATUS_SLOW,
		};

		exthit->tar_debuff = _debuffs[NextRN_N(ARRAY_COUNT(_debuffs))];
		RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_BlackMagic);
	}
#endif

	InjectNegativeStatusExt(attacker, exthit->act_debuff);
	InjectNegativeStatusExt(defender, exthit->tar_debuff);
}

void BattleHit_ConsumeWeapon(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	int target_weapon_cost;
	bool weapon_cost;

	/**
	 * Consume enemy weapons
	 */
	target_weapon_cost = 0;

#if (defined(SID_Corrosion) && (COMMON_SKILL_VALID(SID_Corrosion)))
	if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS) && CheckBattleSkillActivate(attacker, defender, SID_Corrosion, attacker->unit.skl)) {
		RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Corrosion);
		target_weapon_cost += attacker->levelPrevious;
	}
#endif

	if (CheckUnbreakableSpecialSlot(defender->weaponSlotIndex))
		target_weapon_cost = 0;

	if (target_weapon_cost > 0) {
		u16 weapon_pre = defender->weapon;

		while (target_weapon_cost > 0) {
			u16 weapon = GetItemAfterUse(defender->weapon);

			defender->weapon = weapon;
			if (!weapon)
				break;

			target_weapon_cost--;
		}

		if (!defender->weapon && weapon_pre) {
			LTRACE("target weapon broken!");
			defender->weaponBroke = TRUE;
		}
	}

	/**
	 * Consumes the durability of the own weapon
	 */
	if (CheckUnbreakableSpecialSlot(attacker->weaponSlotIndex))
		return;

	weapon_cost = false;
	if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS))
		weapon_cost = true;
	else if (attacker->weaponAttributes & (IA_UNCOUNTERABLE | IA_MAGIC))
		weapon_cost = true;
	else if (CheckWeaponCostForMissedBowAttack(attacker) == true)
		weapon_cost = true;

#if defined(SID_Armsthrift) && (COMMON_SKILL_VALID(SID_Armsthrift))
	if (CheckBattleSkillActivate(attacker, defender, SID_Armsthrift, attacker->unit.lck)) {
		weapon_cost = false;
		RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Armsthrift);
	}
#endif

	/* This is for SID_MasterForge where item uses are set to 0xFF */
	if (GetItemUses(attacker->weapon) == 0xFF)
		weapon_cost = false;

	if (weapon_cost) {
#ifdef CHAX
		/* Check on combat-art */
		int cost = GetWeaponCost(attacker, attacker->weapon);

		while (cost-- > 0) {
			u16 weapon = GetItemAfterUse(attacker->weapon);

			attacker->weapon = weapon;
			if (!weapon)
				break;
		}
#else
		attacker->weapon = GetItemAfterUse(attacker->weapon);
#endif
		if (!attacker->weapon && attacker->weaponBefore) {
			LTRACE("attacker weapon broken!");
			attacker->weaponBroke = TRUE;
		}
	}
}
