#include "common-chax.h"
#include "skill-system.h"
#include "battle-system.h"
#include "strmag.h"
#include "debuff.h"
#include "bwl.h"
#include "kernel-lib.h"
#include "unit-expa.h"
#include "class-types.h"
#include "combat-art.h"
#include "kernel-tutorial.h"
#include "constants/skills.h"
#include "unit-expa.h"
#include "jester_headers/custom-arrays.h"
#include "jester_headers/custom-functions.h"

#define LOCAL_TRACE 0

typedef void (*PreBattleCalcFunc) (struct BattleUnit* buA, struct BattleUnit* buB);
extern PreBattleCalcFunc const* const gpPreBattleCalcFuncs;
void PreBattleCalcWeaponTriangle(struct BattleUnit* attacker, struct BattleUnit* defender);

LYN_REPLACE_CHECK(ComputeBattleUnitSpeed);
void ComputeBattleUnitSpeed(struct BattleUnit* bu)
{
	// Make sure BattleUnit::battleAttack has been setup

	int wt = GetItemWeight(bu->weaponBefore);
	int con = bu->unit.conBonus;

	con += k_udiv(bu->battleAttack * gpKernelBattleDesignerConfig->as_calc_atk_perc, 100);

#ifdef CONFIG_S_RANK_NO_WEAPON_WEIGHT
	int itemType = GetItemType(bu->weaponBefore);

	if (GetUnit(bu->unit.index)->ranks[itemType] == WPN_EXP_S)
		wt = 0;
	else
		wt -= con;
#else
	wt -= con;
#endif

#if (defined(SID_GracefulWielder) && (COMMON_SKILL_VALID(SID_GracefulWielder)))
	if (SkillTester(GetUnit(bu->unit.index), SID_GracefulWielder))
	{
		if (wt > 0)
		{
			int unitWeaponRank = bu->unit.ranks[GetItemType(bu->weapon)];

			if (unitWeaponRank >= WPN_EXP_S)
				wt -= 5;
			else if (unitWeaponRank >= WPN_EXP_A)
				wt -= 4;
			else if (unitWeaponRank >= WPN_EXP_B)
				wt -= 3;
			else if (unitWeaponRank >= WPN_EXP_C)
				wt -= 2;
			else if (unitWeaponRank >= WPN_EXP_D)
				wt -= 1;
		}
	}
#endif

	if (wt < 0)
		wt = 0;

	bu->battleSpeed = bu->unit.spd - wt;

	if (bu->battleSpeed < 0)
		bu->battleSpeed = 0;
}

LYN_REPLACE_CHECK(ComputeBattleUnitAttack);
void ComputeBattleUnitAttack(struct BattleUnit* attacker, struct BattleUnit* defender)
{
	int status;
	bool effective = false;
	int effective_amplifier = 100;
	int effective_reduce = 0x100;
	FORCE_DECLARE bool dualWieldPlus = false;

	status = GetItemMight(attacker->weapon);

#if defined(SID_GorillaTactics) && (COMMON_SKILL_VALID(SID_GorillaTactics))
	if (BattleFastSkillTester(attacker, SID_GorillaTactics) && CheckBitUES(GetUnit(attacker->unit.index), UES_BIT_GorillaTactics_USED))
		status += GetItemMight(attacker->weapon) / 2;
#endif

	if (IsItemEffectiveAgainst(attacker->weapon, &defender->unit)) {
		effective = true;
		effective_amplifier = CalcWeaponEffectivenessScale(attacker->weapon);
	}
	else if (IsUnitEffectiveAgainst(&attacker->unit, &defender->unit)) {
		effective = true;
		effective_amplifier = 300;
	}

	if (effective) {
#if (defined(SID_Resourceful) && (COMMON_SKILL_VALID(SID_Resourceful)))
		if (BattleFastSkillTester(attacker, SID_Resourceful))
			effective_amplifier += SKILL_EFF0(SID_Resourceful);
#endif

#if (defined(SID_SolidRock) && (COMMON_SKILL_VALID(SID_SolidRock)))
		if (BattleFastSkillTester(defender, SID_SolidRock))
			effective_reduce += DAMAGE_DECREASE(SKILL_EFF0(SID_SolidRock));
#endif

		/**
		 * Calc result
		 */
		status = k_udiv(status * effective_amplifier * 0x100, 100 * effective_reduce);
	}

	bool nonStrMagStatBase = false;

#if (defined(SID_SwiftAttack) && (COMMON_SKILL_VALID(SID_SwiftAttack)))
	if (BattleFastSkillTester(attacker, SID_SwiftAttack))
	{
		nonStrMagStatBase = true;
		status = status + attacker->unit.spd;
	}
#endif

#if defined(SID_DualWieldPlus) && (COMMON_SKILL_VALID(SID_DualWieldPlus))
	if (BattleFastSkillTester(attacker, SID_DualWieldPlus))
	{
		for (int i = 1; i < UNIT_MAX_INVENTORY; i++)
		{
			if (GetItemMight(attacker->unit.items[i]) > 0 && CanUnitUseWeapon(GetUnit(attacker->unit.index), attacker->unit.items[i]))
			{
				status += GetItemMight(attacker->unit.items[i]) / 2;
				dualWieldPlus = true;
			}
		}
	}
#endif

#if defined(SID_DualWield) && (COMMON_SKILL_VALID(SID_DualWield))
	if (BattleFastSkillTester(attacker, SID_DualWield) && !dualWieldPlus)
	{
		for (int i = 1; i < UNIT_MAX_INVENTORY; i++)
		{
			if (GetItemMight(attacker->unit.items[i]) > 0 && CanUnitUseWeapon(GetUnit(attacker->unit.index), attacker->unit.items[i]))
				status += GetItemMight(attacker->unit.items[i]) / 2;
		}
	}
#endif

	if (!nonStrMagStatBase)
	{
		if (IsMagicAttack(attacker))
			status = status + UNIT_MAG(&attacker->unit);
		else
			status = status + attacker->unit.pow;
	}

	attacker->battleAttack = status;
}

LYN_REPLACE_CHECK(ComputeBattleUnitDefense);
void ComputeBattleUnitDefense(struct BattleUnit* attacker, struct BattleUnit* defender)
{
	int status, def, res;

	def = attacker->unit.def + attacker->terrainDefense;
	res = attacker->unit.res + attacker->terrainResistance;

	if (IsMagicAttack(defender))
		status = res;
	else
		status = def;

#if (defined(SID_MysticBoost) && COMMON_SKILL_VALID(SID_MysticBoost))
	if (!BattleFastSkillTester(attacker, SID_MysticBoost))
#else
	if (1)
#endif
	{
#if (defined(SID_SorceryBlade) && (COMMON_SKILL_VALID(SID_SorceryBlade)))
		if (BattleFastSkillTester(defender, SID_SorceryBlade))
			status = def < res ? def : res;
#endif
	}
	attacker->battleDefense = status;
}

void ComputeBattleUnitAvoidRate_Rework(struct BattleUnit* bu)
{
	int status;

	/**
	 * vanilla
	 */
	ComputeBattleUnitAvoidRate(bu);

	status = bu->battleAvoidRate;

	if (!CheckOutdoorTerrain(bu->terrainId)) {
		int jid = UNIT_CLASS_ID(&bu->unit);

		if (CheckClassFlier(jid) || CheckClassCavalry(jid))
			status -= gpKernelBattleDesignerConfig->rider_debuff_indoor;
	}

#ifdef CONFIG_BIORHYTHM
	status += GetBiorhythmBonus(bu, gPlaySt.chapterTurnNumber);
#endif

	bu->battleAvoidRate = status;
	if (bu->battleAvoidRate < 0)
		bu->battleAvoidRate = 0;
}

LYN_REPLACE_CHECK(ComputeBattleUnitCritRate);
void ComputeBattleUnitCritRate(struct BattleUnit* bu)
{
	int status;
	int jid = UNIT_CLASS_ID(&bu->unit);

	status = bu->unit.skl / 2;

#if defined(SID_SuperLuck) && (COMMON_SKILL_VALID(SID_SuperLuck))
	if (BattleFastSkillTester(bu, SID_SuperLuck))
		status = bu->unit.lck;
#endif

#if defined(SID_CriticalForce) && (COMMON_SKILL_VALID(SID_CriticalForce))
	if (BattleFastSkillTester(bu, SID_CriticalForce))
		status = bu->unit.skl + bu->unit.skl / 2;
#endif

	status += GetItemCrit(bu->weapon);
	status += gpCriticalBonus[jid];

	if (UNIT_CATTRIBUTES(&bu->unit) & CA_CRITBONUS)
		status += gpKernelBattleDesignerConfig->critical_rate_bonus_attr;

	if (CheckClassCavalry(jid))
		status += gpKernelBattleDesignerConfig->critical_rate_bonus_cavalry;

	bu->battleCritRate = status;
}

LYN_REPLACE_CHECK(ComputeBattleUnitHitRate);
void ComputeBattleUnitHitRate(struct BattleUnit* bu) {
	int status;

	status = bu->unit.skl * 2;

	status += GetItemHit(bu->weapon);
	status += bu->unit.lck / 2;
	status += bu->wTriangleHitBonus;

#ifdef CONFIG_BIORHYTHM
	status += GetBiorhythmBonus(bu, gPlaySt.chapterTurnNumber);
#endif

	bu->battleHitRate = status;
}

LYN_REPLACE_CHECK(ComputeBattleUnitSupportBonuses);
void ComputeBattleUnitSupportBonuses(struct BattleUnit* attacker, struct BattleUnit* defender) {
    if (!(gBattleStats.config & BATTLE_CONFIG_ARENA) || gPlaySt.chapterWeatherId) {
        struct SupportBonuses tmpBonuses;

        GetUnitSupportBonuses(&attacker->unit, &tmpBonuses);

#if (defined(SID_Extrovert) && COMMON_SKILL_VALID(SID_Extrovert))
    if (BattleFastSkillTester(attacker, SID_Extrovert))
    {
        tmpBonuses.bonusAttack *= 2;
        tmpBonuses.bonusDefense *= 2;
        tmpBonuses.bonusHit *= 2;
        tmpBonuses.bonusAvoid *= 2;
        tmpBonuses.bonusCrit *= 2;
        tmpBonuses.bonusDodge *= 2;
    }

#endif

        attacker->battleAttack    += tmpBonuses.bonusAttack;
        attacker->battleDefense   += tmpBonuses.bonusDefense;
        attacker->battleHitRate   += tmpBonuses.bonusHit;
        attacker->battleAvoidRate += tmpBonuses.bonusAvoid;
        attacker->battleCritRate  += tmpBonuses.bonusCrit;
        attacker->battleDodgeRate += tmpBonuses.bonusDodge;
    }
}

STATIC_DECLAR void Local_PreBattleCalcInitExt(struct BattleUnit* attacker, struct BattleUnit* defender)
{
	struct BattleStatus* st;

	ComputeBattleUnitDefense(attacker, defender);
	ComputeBattleUnitAttack(attacker, defender);
	ComputeBattleUnitSpeed(attacker);
	ComputeBattleUnitHitRate(attacker);

#if CHAX
	ComputeBattleUnitAvoidRate_Rework(attacker);
#else
	ComputeBattleUnitAvoidRate(attacker);
#endif

	ComputeBattleUnitCritRate(attacker);
	ComputeBattleUnitDodgeRate(attacker);
	ComputeBattleUnitSupportBonuses(attacker, defender);
	ComputeBattleUnitWeaponRankBonuses(attacker);
	ComputeBattleUnitStatusBonuses(attacker);

	/* Calc silencer rate at pre-battle-calc rather than battle-calc-real */
	attacker->battleSilencerRate = 0;

	st = BattleUnitOriginalStatus(attacker);
	if (UNIT_IS_VALID(&attacker->unit)) {
		st->atk = attacker->battleAttack;
		st->def = attacker->battleDefense;
		st->as = attacker->battleSpeed;
		st->hit = attacker->battleHitRate;
		st->avo = attacker->battleAvoidRate;
		st->crit = attacker->battleCritRate;
		st->dodge = attacker->battleDodgeRate;
		st->silencer = attacker->battleSilencerRate;
	}
	else {
		st->atk = st->def = st->as = st->hit = st->avo = st->crit = st->dodge = st->silencer = 0;
	}

	PreBattleCalcInit_BaseDamage(attacker, defender);
}

void PreBattleCalcInit(struct BattleUnit* attacker, struct BattleUnit* defender)
{

#if (defined(SID_Trace) && COMMON_SKILL_VALID(SID_Trace))
    if (BattleFastSkillTester(attacker, SID_Trace))
    {
        struct SkillList * attackerList =  GetUnitSkillList(&attacker->unit);
        struct SkillList * defenderList =  GetUnitSkillList(&defender->unit);
        
        u16 defenderFirstSkill = defenderList->sid[0];

        for (int i = 0; i < attackerList->amt; i++)
        {
            if (attackerList->sid[i] == SID_Trace)
                attackerList->sid[i] = defenderFirstSkill; 
        }
    }
#endif

	/* Only calc at once */
	if (attacker == &gBattleActor) {
		Local_PreBattleCalcInitExt(attacker, defender);
		Local_PreBattleCalcInitExt(defender, attacker);
	}
}

void PreBattleCalcEnd(struct BattleUnit* attacker, struct BattleUnit* defender)
{
	if (attacker->battleAttack < 0)
		attacker->battleAttack = 0;

	if (attacker->battleDefense < 0)
		attacker->battleDefense = 0;

	if (attacker->battleSpeed < 0)
		attacker->battleSpeed = 0;

	if (attacker->battleHitRate < 0)
		attacker->battleHitRate = 0;

	if (attacker->battleAvoidRate < 0)
		attacker->battleAvoidRate = 0;

	if (attacker->battleCritRate < 0)
		attacker->battleCritRate = 0;

	if (attacker->battleDodgeRate < 0)
		attacker->battleDodgeRate = 0;

	/* Some special effects */

#if (defined(SID_RuinedBladePlus) && (COMMON_SKILL_VALID(SID_RuinedBladePlus)))
	/* RuinedBladePlus cannot crit attack */
	if (BattleFastSkillTester(attacker, SID_RuinedBladePlus)) {
		attacker->battleCritRate = 0;
		attacker->battleSilencerRate = 0;
	}
#endif

#if (defined(SID_CriticalPierce) && (COMMON_SKILL_VALID(SID_CriticalPierce)))
	if (BattleFastSkillTester(defender, SID_CriticalPierce))
		attacker->battleDodgeRate = 0;
#endif

	/* If defender cannot get silencer */
	if (UNIT_CATTRIBUTES(&defender->unit) & CA_NEGATE_LETHALITY)
		attacker->battleSilencerRate = 0;

	if (attacker->battleSilencerRate < 0)
		attacker->battleSilencerRate = 0;
}

void PreBattleCalcDefenderSkills(struct BattleUnit* attacker, struct BattleUnit* defender)
{
	int _skill_list_cnt;
	struct SkillList* list;

	/**
	 * Skip arena judgement
	 */
	if (gBattleStats.config & BATTLE_CONFIG_ARENA)
		return;

	list = GetUnitSkillList(&defender->unit);
	for (_skill_list_cnt = 0; _skill_list_cnt < list->amt; _skill_list_cnt++) {
		switch (list->sid[_skill_list_cnt]) {
#if (defined(SID_StunningSmile) && (COMMON_SKILL_VALID(SID_StunningSmile)))
		case SID_StunningSmile:
			// todo: move to extra info
			if (!(UNIT_CATTRIBUTES(&attacker->unit) & CA_FEMALE))
				attacker->battleAvoidRate -= 20;
			break;
#endif

#if (defined(SID_MeleeManiac) && COMMON_SKILL_VALID(SID_MeleeManiac))
		case SID_MeleeManiac:
			// todo: move to extra info
			if (gBattleStats.range != 1)
				GetBaseDmg(attacker)->increase += 100;
			break;
#endif

#if (defined(SID_DragonSkin) && (COMMON_SKILL_VALID(SID_DragonSkin)))
		case SID_DragonSkin:
			GetBaseDmg(attacker)->decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_DragonSkin));
			break;
#endif

#if (defined(SID_BeastAssault) && (COMMON_SKILL_VALID(SID_BeastAssault)))
		case SID_BeastAssault:
			GetBaseDmg(attacker)->decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_BeastAssault));
			break;
#endif

#if (defined(SID_Spurn) && (COMMON_SKILL_VALID(SID_Spurn)))
		case SID_Spurn:
		{
			int tmp = BattleUnitOriginalStatus(defender)->as - BattleUnitOriginalStatus(attacker)->as;

			LIMIT_AREA(tmp, 0, 10);
			GetBaseDmg(attacker)->decrease += DAMAGE_DECREASE(tmp * SKILL_EFF1(SID_Spurn));
			break;
		}
#endif

#if (defined(SID_Bushido) && (COMMON_SKILL_VALID(SID_Bushido)))
		case SID_Bushido:
		{
			int tmp = BattleUnitOriginalStatus(defender)->as - BattleUnitOriginalStatus(attacker)->as;

			LIMIT_AREA(tmp, 0, 10);
			GetBaseDmg(attacker)->decrease += DAMAGE_DECREASE(tmp * SKILL_EFF0(SID_Bushido));
			break;
		}
#endif

#if (defined(SID_DragonWall) && (COMMON_SKILL_VALID(SID_DragonWall)))
		case SID_DragonWall:
		{
			int tmp = defender->unit.res - attacker->unit.res;

			LIMIT_AREA(tmp, 0, 10);
			GetBaseDmg(attacker)->decrease += DAMAGE_DECREASE(tmp * SKILL_EFF0(SID_DragonWall));
			break;
		}
#endif

#if (defined(SID_BlueLionRule) && (COMMON_SKILL_VALID(SID_BlueLionRule)))
		case SID_BlueLionRule:
		{
			int tmp = defender->unit.def - attacker->unit.def;

			LIMIT_AREA(tmp, 0, 10);
			GetBaseDmg(attacker)->decrease += DAMAGE_DECREASE(tmp * SKILL_EFF0(SID_BlueLionRule));
			break;
		}
#endif

#if defined(SID_ImmovableObject) && (COMMON_SKILL_VALID(SID_ImmovableObject))
		case SID_ImmovableObject:
			GetBaseDmg(attacker)->decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_ImmovableObject));
			break;
#endif

#if defined(SID_KeenFighter) && (COMMON_SKILL_VALID(SID_KeenFighter))
		case SID_KeenFighter:
		{
			int tmp = 0;

			if (attacker == &gBattleActor)
				tmp = UNWIND_DOUBLE_ACT;
			else
				tmp = UNWIND_DOUBLE_TAR;

			if (gBattleFlagExt.round_mask & tmp)
				GetBaseDmg(attacker)->decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_KeenFighter));

			break;
		}
#endif

#if defined(SID_SteadyBrawler) && (COMMON_SKILL_VALID(SID_SteadyBrawler))
		case SID_SteadyBrawler:
			if (gBattleStats.config & BATTLE_CONFIG_SIMULATE) {
				int local_mask = (defender == &gBattleActor) ? UNWIND_DOUBLE_ACT : UNWIND_DOUBLE_TAR;

				if (!(gBattleFlagExt.round_mask & local_mask))
					GetBaseDmg(attacker)->decrease += DAMAGE_DECREASE(SKILL_EFF1(SID_SteadyBrawler));
			}
			break;
#endif

#if (defined(SID_Multiscale) && (COMMON_SKILL_VALID(SID_Multiscale)))
		case SID_Multiscale:
			if (gBattleStats.config & BATTLE_CONFIG_SIMULATE)
				if (defender->unit.curHP == defender->unit.maxHP)
					GetBaseDmg(attacker)->decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_Multiscale));

			break;
#endif

#if defined(SID_GuardBearing) && (COMMON_SKILL_VALID(SID_GuardBearing))
		case SID_GuardBearing:
			if (gBattleStats.config & BATTLE_CONFIG_SIMULATE) {
				if (!AreUnitsAllied(defender->unit.index, gPlaySt.faction))
					GetBaseDmg(attacker)->decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_GuardBearing));
			}
			break;
#endif

#if (defined(SID_CounterRoar) && (COMMON_SKILL_VALID(SID_CounterRoar)))
		case SID_CounterRoar:
			if (gBattleStats.config & BATTLE_CONFIG_SIMULATE)
				GetBaseDmg(attacker)->decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_CounterRoar));

			break;
#endif

#if (defined(SID_DragonsWrath) && (COMMON_SKILL_VALID(SID_DragonsWrath)))
		case SID_DragonsWrath:
			if (gBattleStats.config & BATTLE_CONFIG_SIMULATE)
				gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_DragonsWrath));

			break;
#endif

		case MAX_SKILL_NUM:
			Fatal("ENOSUPP");
			break;

		default:
			break;
		}
	}
}

void PreBattleCalcAttackerSkills(struct BattleUnit* attacker, struct BattleUnit* defender)
{
	FORCE_DECLARE int tmp, i;
	int _skill_list_cnt;
	struct SkillList* list;
	FORCE_DECLARE struct NewBwl* bwl = GetNewBwl(UNIT_CHAR_ID(GetUnit(attacker->unit.index)));
	FORCE_DECLARE bool wrathPlusActivated = false;
	FORCE_DECLARE int unitRank = 0;

	/**
	 * Skip arena judgement
	 */
	if (gBattleStats.config & BATTLE_CONFIG_ARENA)
		return;

	list = GetUnitSkillList(&attacker->unit);
	for (_skill_list_cnt = 0; _skill_list_cnt < list->amt; _skill_list_cnt++) {
		switch (list->sid[_skill_list_cnt]) {
#if (defined(SID_DefiantCrit) && (COMMON_SKILL_VALID(SID_DefiantCrit)))
		case SID_DefiantCrit:
			if ((attacker->hpInitial * 4) < attacker->unit.maxHP)
				attacker->battleCritRate += SKILL_EFF0(SID_DefiantCrit);

			break;
#endif

#if (defined(SID_DefiantAvoid) && (COMMON_SKILL_VALID(SID_DefiantAvoid)))
		case SID_DefiantAvoid:
			if ((attacker->hpInitial * 4) < attacker->unit.maxHP)
				attacker->battleAvoidRate += SKILL_EFF0(SID_DefiantAvoid);

			break;
#endif

			/* Blow skills */
#if (defined(SID_BlowDarting) && (COMMON_SKILL_VALID(SID_BlowDarting)))
		case SID_BlowDarting:
			if (attacker == &gBattleActor)
				attacker->battleSpeed += SKILL_EFF0(SID_BlowDarting);

			break;
#endif

#if (defined(SID_BlowDeath) && (COMMON_SKILL_VALID(SID_BlowDeath)))
		case SID_BlowDeath:
			if (attacker == &gBattleActor && !IsMagicAttack(attacker))
				attacker->battleAttack += SKILL_EFF0(SID_BlowDeath);

			break;
#endif

#if (defined(SID_BlowFiendish) && (COMMON_SKILL_VALID(SID_BlowFiendish)))
		case SID_BlowFiendish:
			if (attacker == &gBattleActor && IsMagicAttack(attacker))
				attacker->battleAttack += SKILL_EFF0(SID_BlowFiendish);

			break;
#endif

#if (defined(SID_BlowArmored) && (COMMON_SKILL_VALID(SID_BlowArmored)))
		case SID_BlowArmored:
			if (attacker == &gBattleActor && !IsMagicAttack(defender))
				attacker->battleDefense += SKILL_EFF0(SID_BlowArmored);

			break;
#endif

#if (defined(SID_BlowWarding) && (COMMON_SKILL_VALID(SID_BlowWarding)))
		case SID_BlowWarding:
			if (attacker == &gBattleActor && IsMagicAttack(defender))
				attacker->battleDefense += SKILL_EFF0(SID_BlowWarding);

			break;
#endif

#if (defined(SID_BlowDuelist) && (COMMON_SKILL_VALID(SID_BlowDuelist)))
		case SID_BlowDuelist:
			if (attacker == &gBattleActor)
				attacker->battleAvoidRate += SKILL_EFF0(SID_BlowDuelist);

			break;
#endif

#if (defined(SID_BlowUncanny) && (COMMON_SKILL_VALID(SID_BlowUncanny)))
		case SID_BlowUncanny:
			if (attacker == &gBattleActor)
				attacker->battleHitRate += SKILL_EFF0(SID_BlowUncanny);

			break;
#endif

#if (defined(SID_BlowKilling) && (COMMON_SKILL_VALID(SID_BlowKilling)))
		case SID_BlowKilling:
			if (attacker == &gBattleActor)
				attacker->battleCritRate += SKILL_EFF0(SID_BlowKilling);

			break;
#endif

#if (defined(SID_ArcaneBlade) && (COMMON_SKILL_VALID(SID_ArcaneBlade)))
		case SID_ArcaneBlade:
			if (attacker == &gBattleActor && gBattleStats.range == 1) {
				attacker->battleCritRate += UNIT_MAG(&attacker->unit) / 2 + SKILL_EFF0(SID_ArcaneBlade);
				attacker->battleHitRate += UNIT_MAG(&attacker->unit) / 2 + SKILL_EFF0(SID_ArcaneBlade);
			}
			break;
#endif

#if (defined(SID_Prescience) && (COMMON_SKILL_VALID(SID_Prescience)))
		case SID_Prescience:
			if (attacker == &gBattleActor && gBattleStats.range == 1) {
				attacker->battleHitRate += SKILL_EFF0(SID_Prescience);
				attacker->battleAvoidRate += SKILL_EFF1(SID_Prescience);
			}
			break;
#endif

			/* Stance skills */
#if (defined(SID_StanceBracing) && (COMMON_SKILL_VALID(SID_StanceBracing)))
		case SID_StanceBracing:
			if (attacker == &gBattleTarget)
				attacker->battleDefense += SKILL_EFF0(SID_StanceBracing);

			break;
#endif

#if (defined(SID_StanceDarting) && (COMMON_SKILL_VALID(SID_StanceDarting)))
		case SID_StanceDarting:
			if (attacker == &gBattleTarget)
				attacker->battleSpeed += SKILL_EFF0(SID_StanceDarting);

			break;
#endif

#if (defined(SID_StanceFierce) && (COMMON_SKILL_VALID(SID_StanceFierce)))
		case SID_StanceFierce:
			if (attacker == &gBattleTarget)
				attacker->battleAttack += SKILL_EFF0(SID_StanceFierce);

			break;
#endif

#if (defined(SID_StanceKestrel) && (COMMON_SKILL_VALID(SID_StanceKestrel)))
		case SID_StanceKestrel:
			if (attacker == &gBattleTarget) {
				attacker->battleAttack += SKILL_EFF0(SID_StanceKestrel);
				attacker->battleSpeed += SKILL_EFF1(SID_StanceKestrel);
			}
			break;
#endif

#if (defined(SID_StanceMirror) && (COMMON_SKILL_VALID(SID_StanceMirror)))
		case SID_StanceMirror:
			if (attacker == &gBattleTarget) {
				attacker->battleAttack += SKILL_EFF0(SID_StanceMirror);
				if (IsMagicAttack(defender))
					attacker->battleDefense += SKILL_EFF1(SID_StanceMirror);
			}
			break;
#endif

#if (defined(SID_StanceSwift) && (COMMON_SKILL_VALID(SID_StanceSwift)))
		case SID_StanceSwift:
			if (attacker == &gBattleTarget) {
				attacker->battleSpeed += SKILL_EFF0(SID_StanceSwift);
				if (IsMagicAttack(defender))
					attacker->battleDefense += SKILL_EFF1(SID_StanceSwift);
			}
			break;
#endif

#if (defined(SID_StanceReady) && (COMMON_SKILL_VALID(SID_StanceReady)))
		case SID_StanceReady:
			if (attacker == &gBattleTarget) {
				attacker->battleSpeed += SKILL_EFF0(SID_StanceReady);
				if (!IsMagicAttack(defender))
					attacker->battleDefense += SKILL_EFF1(SID_StanceReady);
			}
			break;
#endif

#if (defined(SID_StanceSturdy) && (COMMON_SKILL_VALID(SID_StanceSturdy)))
		case SID_StanceSturdy:
			if (attacker == &gBattleTarget) {
				attacker->battleAttack += SKILL_EFF0(SID_StanceSturdy);
				if (!IsMagicAttack(defender))
					attacker->battleDefense += SKILL_EFF1(SID_StanceSturdy);
			}
			break;
#endif

#if (defined(SID_StanceSteady) && (COMMON_SKILL_VALID(SID_StanceSteady)))
		case SID_StanceSteady:
			if (attacker == &gBattleTarget && !IsMagicAttack(defender))
				attacker->battleDefense += SKILL_EFF0(SID_StanceSteady);

			break;
#endif

#if (defined(SID_StanceWarding) && (COMMON_SKILL_VALID(SID_StanceWarding)))
		case SID_StanceWarding:
			if (attacker == &gBattleTarget && IsMagicAttack(defender))
				attacker->battleDefense += SKILL_EFF0(SID_StanceWarding);

			break;
#endif

#if (defined(SID_StanceSpectrum) && (COMMON_SKILL_VALID(SID_StanceSpectrum)))
		case SID_StanceSpectrum:
			if (attacker == &gBattleTarget) {
				attacker->battleAttack += SKILL_EFF0(SID_StanceSpectrum);
				attacker->battleSpeed += SKILL_EFF1(SID_StanceSpectrum);
				attacker->battleDefense += SKILL_EFF2(SID_StanceSpectrum);
			}
			break;
#endif

#if (defined(SID_Patience) && (COMMON_SKILL_VALID(SID_Patience)))
		case SID_Patience:
			if (attacker == &gBattleTarget)
				attacker->battleAvoidRate += SKILL_EFF0(SID_Patience);

			break;
#endif

#if (defined(SID_Pursuit) && (COMMON_SKILL_VALID(SID_Pursuit)))
		case SID_Pursuit:
			if (attacker == &gBattleTarget)
				attacker->battleSpeed += SKILL_EFF0(SID_Pursuit);

			break;
#endif

			/* Misc */
#if (defined(SID_Lethality) && (COMMON_SKILL_VALID(SID_Lethality)))
		case SID_Lethality:
			attacker->battleSilencerRate += attacker->unit.skl;
			break;
#endif

#if (defined(SID_WatchfulEye) && (COMMON_SKILL_VALID(SID_WatchfulEye)))
		case SID_WatchfulEye:
			attacker->battleHitRate += SKILL_EFF0(SID_WatchfulEye);
			break;
#endif

#if (defined(SID_Crit) && (COMMON_SKILL_VALID(SID_Crit)))
		case SID_Crit:
			attacker->battleCritRate += SKILL_EFF0(SID_Crit);
			break;
#endif

#if (defined(SID_Avoid) && (COMMON_SKILL_VALID(SID_Avoid)))
		case SID_Avoid:
			attacker->battleAvoidRate += SKILL_EFF0(SID_Avoid);
			break;
#endif

			/* wType related */
#if (defined(SID_CritSword) && (COMMON_SKILL_VALID(SID_CritSword)))
		case SID_CritSword:
			if (attacker->weaponType == ITYPE_SWORD)
				attacker->battleCritRate += SKILL_EFF0(SID_CritSword);

			break;
#endif

#if (defined(SID_FaireSword) && (COMMON_SKILL_VALID(SID_FaireSword)))
		case SID_FaireSword:
			if (attacker->weaponType == ITYPE_SWORD)
				attacker->battleAttack += SKILL_EFF0(SID_FaireSword);

			break;
#endif

#if (defined(SID_AvoidSword) && (COMMON_SKILL_VALID(SID_AvoidSword)))
		case SID_AvoidSword:
			if (attacker->weaponType == ITYPE_SWORD)
				attacker->battleAvoidRate += SKILL_EFF0(SID_AvoidSword);

			break;
#endif

#if (defined(SID_CritLance) && (COMMON_SKILL_VALID(SID_CritLance)))
		case SID_CritLance:
			if (attacker->weaponType == ITYPE_LANCE)
				attacker->battleCritRate += SKILL_EFF0(SID_CritLance);

			break;
#endif

#if (defined(SID_FaireLance) && (COMMON_SKILL_VALID(SID_FaireLance)))
		case SID_FaireLance:
			if (attacker->weaponType == ITYPE_LANCE)
				attacker->battleAttack += SKILL_EFF0(SID_FaireLance);

			break;
#endif

#if (defined(SID_CritAxe) && (COMMON_SKILL_VALID(SID_CritAxe)))
		case SID_CritAxe:
			if (attacker->weaponType == ITYPE_AXE)
				attacker->battleCritRate += SKILL_EFF0(SID_CritAxe);

			break;
#endif

#if (defined(SID_FaireAxe) && (COMMON_SKILL_VALID(SID_FaireAxe)))
		case SID_FaireAxe:
			if (attacker->weaponType == ITYPE_AXE)
				attacker->battleAttack += SKILL_EFF0(SID_FaireAxe);

			break;
#endif

#if (defined(SID_CritBow) && (COMMON_SKILL_VALID(SID_CritBow)))
		case SID_CritBow:
			if (attacker->weaponType == ITYPE_BOW)
				attacker->battleCritRate += SKILL_EFF0(SID_CritBow);

			break;
#endif

#if (defined(SID_FaireBow) && (COMMON_SKILL_VALID(SID_FaireBow)))
		case SID_FaireBow:
			if (attacker->weaponType == ITYPE_BOW)
				attacker->battleAttack += SKILL_EFF0(SID_FaireBow);

			break;
#endif

#if (defined(SID_FaireBMag) && (COMMON_SKILL_VALID(SID_FaireBMag)))
		case SID_FaireBMag:
			switch (attacker->weaponType) {
			case ITYPE_ANIMA:
			case ITYPE_LIGHT:
			case ITYPE_DARK:
				attacker->battleAttack += SKILL_EFF0(SID_FaireBMag);
				break;
			}
			break;
#endif

#if (defined(SID_FaireAnima) && (COMMON_SKILL_VALID(SID_FaireAnima)))
		case SID_FaireAnima:
			switch (attacker->weaponType) {
			case ITYPE_ANIMA:
				attacker->battleAttack += SKILL_EFF0(SID_FaireAnima);
				break;
			}
			break;
#endif

#if (defined(SID_FaireLight) && (COMMON_SKILL_VALID(SID_FaireLight)))
		case SID_FaireLight:
			switch (attacker->weaponType) {
			case ITYPE_LIGHT:
				attacker->battleAttack += SKILL_EFF0(SID_FaireLight);
				break;
			}
			break;
#endif

#if (defined(SID_FaireDark) && (COMMON_SKILL_VALID(SID_FaireDark)))
		case SID_FaireDark:
			switch (attacker->weaponType) {
			case ITYPE_DARK:
				attacker->battleAttack += SKILL_EFF0(SID_FaireDark);
				break;
			}
			break;
#endif

#if defined(SID_HolyAura) && (COMMON_SKILL_VALID(SID_HolyAura))
		case SID_HolyAura:
			if (attacker->weaponType == ITYPE_LIGHT) {
				attacker->battleAttack += SKILL_EFF0(SID_HolyAura);
				attacker->battleCritRate += SKILL_EFF1(SID_HolyAura);
				attacker->battleHitRate += SKILL_EFF2(SID_HolyAura);
				attacker->battleAvoidRate += SKILL_EFF3(SID_HolyAura);
			}
			break;
#endif

#if (defined(SID_BlueFlame) && (COMMON_SKILL_VALID(SID_BlueFlame)))
		case SID_BlueFlame:
			attacker->battleAttack += SKILL_EFF0(SID_BlueFlame);
			break;
#endif

#if (defined(SID_FaerghusAncestry) && (COMMON_SKILL_VALID(SID_FaerghusAncestry)))
		case SID_FaerghusAncestry:
			attacker->battleAttack += GetItemMight(attacker->weapon);
			break;
#endif

#if (defined(SID_LadyBlade) && (COMMON_SKILL_VALID(SID_LadyBlade)))
		case SID_LadyBlade:
			if ((UNIT_CATTRIBUTES(&attacker->unit) & CA_FEMALE))
				attacker->battleAttack += GetItemMight(attacker->weapon);

			break;
#endif

#if (defined(SID_Frenzy) && (COMMON_SKILL_VALID(SID_Frenzy)))
		case SID_Frenzy:
			if ((attacker->unit.maxHP - attacker->hpInitial) >= 4)
				attacker->battleAttack += (attacker->unit.maxHP - attacker->hpInitial) / 4;

			break;
#endif

#if (defined(SID_Zen) && (COMMON_SKILL_VALID(SID_Zen)))
		case SID_Zen:
			if ((attacker->unit.maxHP - attacker->hpInitial) >= 4)
				attacker->battleDefense += (attacker->unit.maxHP - attacker->hpInitial) / 4;

			break;
#endif


#if (defined(SID_KillingMachine) && (COMMON_SKILL_VALID(SID_KillingMachine)))
		case SID_KillingMachine:
			attacker->battleCritRate *= 2;
			break;
#endif

#if (defined(SID_HeavyStrikes) && (COMMON_SKILL_VALID(SID_HeavyStrikes)))
		case SID_HeavyStrikes:
			attacker->battleCritRate += GetItemWeight(attacker->weapon);
			break;
#endif

#if (defined(SID_Technician) && (COMMON_SKILL_VALID(SID_Technician)))
		case SID_Technician:
			if (GetItemRequiredExp(attacker->weapon) < WPN_EXP_D)
				attacker->battleAttack += GetItemMight(attacker->weapon) / 2;
			break;
#endif

#if (defined(SID_BattleVeteran) && (COMMON_SKILL_VALID(SID_BattleVeteran)))
		case SID_BattleVeteran:
			tmp = k_udiv(attacker->levelPrevious + GetUnitHiddenLevel(&attacker->unit), 10);
			attacker->battleHitRate += SKILL_EFF0(SID_RuinedBlade) * tmp;
			attacker->battleAttack += SKILL_EFF1(SID_RuinedBlade) * tmp;
			break;
#endif

#if (defined(SID_StoneBody) && (COMMON_SKILL_VALID(SID_StoneBody)))
		case SID_StoneBody:
			tmp = attacker->unit.conBonus - defender->unit.conBonus;
			if (tmp > 0)
				attacker->battleAttack += tmp;

			break;
#endif

#if (defined(SID_Chivalry) && (COMMON_SKILL_VALID(SID_Chivalry)))
		case SID_Chivalry:
			if (defender->hpInitial == defender->unit.maxHP) {
				attacker->battleAttack += SKILL_EFF0(SID_Chivalry);
				attacker->battleDefense += SKILL_EFF1(SID_Chivalry);
			}
			break;
#endif

#if (defined(SID_Pragmatic) && (COMMON_SKILL_VALID(SID_Pragmatic)))
		case SID_Pragmatic:
			if (defender->hpInitial < defender->unit.maxHP) {
				attacker->battleAttack += SKILL_EFF0(SID_Pragmatic);
				attacker->battleDefense += SKILL_EFF1(SID_Pragmatic);
			}
			break;
#endif

#if (defined(SID_Perfectionist) && (COMMON_SKILL_VALID(SID_Perfectionist)))
		case SID_Perfectionist:
			if (attacker->hpInitial == attacker->unit.maxHP) {
				attacker->battleHitRate += SKILL_EFF0(SID_Perfectionist);
				attacker->battleAvoidRate += SKILL_EFF1(SID_Perfectionist);
			}
			break;
#endif

#if (defined(SID_WindDisciple) && (COMMON_SKILL_VALID(SID_WindDisciple)))
		case SID_WindDisciple:
			if (attacker->hpInitial < attacker->unit.maxHP) {
				attacker->battleHitRate += SKILL_EFF0(SID_WindDisciple);
				attacker->battleAvoidRate += SKILL_EFF1(SID_WindDisciple);
			}
			break;
#endif

#if (defined(SID_FieryBlood) && (COMMON_SKILL_VALID(SID_FieryBlood)))
		case SID_FieryBlood:
			if (attacker->hpInitial < attacker->unit.maxHP)
				attacker->battleAttack += SKILL_EFF0(SID_FieryBlood);

			break;
#endif

#if (defined(SID_FireBoost) && (COMMON_SKILL_VALID(SID_FireBoost)))
		case SID_FireBoost:
			if (attacker->hpInitial >= (defender->hpInitial + SKILL_EFF1(SID_FireBoost)))
				attacker->battleAttack += SKILL_EFF0(SID_FireBoost);

			break;
#endif

#if (defined(SID_WindBoost) && (COMMON_SKILL_VALID(SID_WindBoost)))
		case SID_WindBoost:
			if (attacker->hpInitial >= (defender->hpInitial + SKILL_EFF1(SID_WindBoost)))
				attacker->battleSpeed += SKILL_EFF0(SID_WindBoost);

			break;
#endif

#if (defined(SID_EarthBoost) && (COMMON_SKILL_VALID(SID_EarthBoost)))
		case SID_EarthBoost:
			if (attacker->hpInitial >= (defender->hpInitial + SKILL_EFF1(SID_EarthBoost)) && !IsMagicAttack(defender))
				attacker->battleDefense += SKILL_EFF0(SID_EarthBoost);

			break;
#endif

#if (defined(SID_Wrath) && (COMMON_SKILL_VALID(SID_Wrath)))
		case SID_Wrath:
			if (attacker->unit.maxHP > (attacker->unit.curHP * 2))
				attacker->battleCritRate += SKILL_EFF0(SID_Wrath);

			break;
#endif

#if (defined(SID_WaterBoost) && (COMMON_SKILL_VALID(SID_WaterBoost)))
		case SID_WaterBoost:
			if (attacker->hpInitial >= (defender->hpInitial + SKILL_EFF1(SID_WaterBoost)) && IsMagicAttack(defender))
				attacker->battleDefense += SKILL_EFF0(SID_WaterBoost);

			break;
#endif

#if (defined(SID_ChaosStyle) && (COMMON_SKILL_VALID(SID_ChaosStyle)))
		case SID_ChaosStyle:
			if (IsMagicAttack(attacker) != IsMagicAttack(defender))
				attacker->battleSpeed += SKILL_EFF0(SID_ChaosStyle);

			break;
#endif

#if (defined(SID_Charge) && (COMMON_SKILL_VALID(SID_Charge)))
		case SID_Charge:
			attacker->battleAttack += (gActionData.moveCount * SKILL_EFF0(SID_Charge)) / 2;
			break;
#endif

#if (defined(SID_NaturalCover) && (COMMON_SKILL_VALID(SID_NaturalCover)))
		case SID_NaturalCover:
			if (attacker->terrainDefense > 0 || attacker->terrainAvoid > 0 || attacker->terrainResistance > 0)
				attacker->battleDefense += SKILL_EFF0(SID_NaturalCover);

			break;
#endif

#if (defined(SID_ElbowRoom) && (COMMON_SKILL_VALID(SID_ElbowRoom)))
		case SID_ElbowRoom:
			if (attacker->terrainDefense == 0 && attacker->terrainAvoid == 0 && attacker->terrainResistance == 0)
				attacker->battleAttack += SKILL_EFF0(SID_ElbowRoom);

			break;
#endif

#if (defined(SID_Vigilance) && (COMMON_SKILL_VALID(SID_Vigilance)))
		case SID_Vigilance:
			attacker->battleAvoidRate += 20;
			break;
#endif

#if (defined(SID_QuickDraw) && (COMMON_SKILL_VALID(SID_QuickDraw)))
		case SID_QuickDraw:
			if (attacker == &gBattleActor)
				attacker->battleAttack += SKILL_EFF0(SID_QuickDraw);

			break;
#endif

#if (defined(SID_StrongRiposte) && (COMMON_SKILL_VALID(SID_StrongRiposte)))
		case SID_StrongRiposte:
			if (attacker == &gBattleTarget)
				attacker->battleAttack += SKILL_EFF0(SID_StrongRiposte);

			break;
#endif

#if (defined(SID_OutdoorFighter) && (COMMON_SKILL_VALID(SID_OutdoorFighter)))
		case SID_OutdoorFighter:
			if (CheckOutdoorTerrain(attacker->terrainId)) {
				attacker->battleHitRate += SKILL_EFF0(SID_OutdoorFighter);
				attacker->battleAvoidRate += SKILL_EFF1(SID_OutdoorFighter);
			}
			break;
#endif

#if (defined(SID_KnightAspirant) && (COMMON_SKILL_VALID(SID_KnightAspirant)))
		case SID_KnightAspirant:
			if ((attacker->hpInitial * 4) > (attacker->unit.maxHP * 3)) {
				attacker->battleAttack += SKILL_EFF0(SID_KnightAspirant);
				attacker->battleAvoidRate += SKILL_EFF1(SID_KnightAspirant);
			}
			break;
#endif

#if (defined(SID_Outrider) && (COMMON_SKILL_VALID(SID_Outrider)))
		case SID_Outrider:
			attacker->battleDefense += gActionData.moveCount;
			attacker->battleCritRate += (gActionData.moveCount * 3);
			break;
#endif

#if (defined(SID_SilentPride) && (COMMON_SKILL_VALID(SID_SilentPride)))
		case SID_SilentPride:
			tmp = attacker->unit.maxHP - attacker->hpInitial;
			if (tmp > 0) {
				tmp = k_udiv(tmp * 4, attacker->unit.maxHP);
				attacker->battleAttack += SKILL_EFF0(SID_SilentPride) * tmp;
				attacker->battleDefense += SKILL_EFF1(SID_SilentPride) * tmp;
			}
			break;
#endif

#if (defined(SID_Guts) && (COMMON_SKILL_VALID(SID_Guts)))
		case SID_Guts:
			if (GetUnitStatusIndex(&attacker->unit) != UNIT_STATUS_NONE)
				attacker->battleAttack += SKILL_EFF0(SID_Guts);

			break;
#endif

#if (defined(SID_StrongCon) && (COMMON_SKILL_VALID(SID_StrongCon)))
		case SID_StrongCon:
			if (GetUnitStatusIndex(&attacker->unit) != UNIT_STATUS_NONE)
				attacker->battleDefense += SKILL_EFF0(SID_StrongCon);

			break;
#endif

#if (defined(SID_TowerShield) && (COMMON_SKILL_VALID(SID_TowerShield)))
		case SID_TowerShield:
			if (gBattleStats.range >= 2)
				attacker->battleDefense += SKILL_EFF0(SID_TowerShield);

			break;
#endif

#if (defined(SID_ShortShield) && (COMMON_SKILL_VALID(SID_ShortShield)))
		case SID_ShortShield:
			if (gBattleStats.range == 1)
				attacker->battleDefense += SKILL_EFF0(SID_ShortShield);

			break;
#endif

#if defined(SID_Trample) && (COMMON_SKILL_VALID(SID_Trample))
		case SID_Trample:
			if (!(UNIT_CATTRIBUTES(&defender->unit) & CA_MOUNTEDAID))
				attacker->battleAttack += SKILL_EFF0(SID_Trample);

			break;
#endif

#if defined(SID_Opportunist) && (COMMON_SKILL_VALID(SID_Opportunist))
		case SID_Opportunist:
			if (!defender->canCounter)
				attacker->battleAttack += SKILL_EFF0(SID_Opportunist);

			break;
#endif

#if defined(SID_Vanity) && (COMMON_SKILL_VALID(SID_Vanity))
		case SID_Vanity:
			if (gBattleStats.range == 2) {
				attacker->battleAttack += SKILL_EFF0(SID_Vanity);
				attacker->battleHitRate += SKILL_EFF1(SID_Vanity);
			}
			break;
#endif

#if (defined(SID_EvenRhythm) && (COMMON_SKILL_VALID(SID_EvenRhythm)))
		case SID_EvenRhythm:
			if ((gPlaySt.chapterTurnNumber % 2) == 0) {
				attacker->battleHitRate += SKILL_EFF0(SID_EvenRhythm);
				attacker->battleAvoidRate += SKILL_EFF1(SID_EvenRhythm);
			}
			break;
#endif

#if (defined(SID_OddRhythm) && (COMMON_SKILL_VALID(SID_OddRhythm)))
		case SID_OddRhythm:
			if ((gPlaySt.chapterTurnNumber % 2) == 1) {
				attacker->battleHitRate += SKILL_EFF0(SID_OddRhythm);
				attacker->battleAvoidRate += SKILL_EFF1(SID_OddRhythm);
			}
			break;
#endif

#if (defined(SID_QuickBurn) && (COMMON_SKILL_VALID(SID_QuickBurn)))
		case SID_QuickBurn:
			tmp = gPlaySt.chapterTurnNumber > SKILL_EFF0(SID_QuickBurn)
				? 0
				: SKILL_EFF0(SID_QuickBurn) - gPlaySt.chapterTurnNumber;

			attacker->battleHitRate += tmp;
			attacker->battleAvoidRate += tmp;
			break;
#endif

#if (defined(SID_SlowBurn) && (COMMON_SKILL_VALID(SID_SlowBurn)))
		case SID_SlowBurn:
			tmp = gPlaySt.chapterTurnNumber > SKILL_EFF0(SID_SlowBurn)
				? SKILL_EFF0(SID_SlowBurn)
				: gPlaySt.chapterTurnNumber;

			attacker->battleHitRate += tmp;
			attacker->battleAvoidRate += tmp;
			break;
#endif

#if (defined(SID_Skyguard) && COMMON_SKILL_VALID(SID_Skyguard))
		case SID_Skyguard:
			if (!(gBattleStats.config & BATTLE_CONFIG_ARENA) && CheckClassFlier(UNIT_CLASS_ID(&defender->unit))) {
				for (i = 0; i < ARRAY_COUNT_RANGE3x3; i++) {
					int _x = attacker->unit.xPos + gVecs_3x3[i].x;
					int _y = attacker->unit.yPos + gVecs_3x3[i].y;

					struct Unit* unit_ally = GetUnitAtPosition(_x, _y);

					if (!UNIT_IS_VALID(unit_ally))
						continue;

					if (unit_ally->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
						continue;

					if (AreUnitsAllied(attacker->unit.index, unit_ally->index) && CheckClassFlier(UNIT_CLASS_ID(unit_ally))) {
						attacker->battleDefense += SKILL_EFF0(SID_Skyguard);
						break;
					}
				}
			}
			break;
#endif

#if (defined(SID_Horseguard) && COMMON_SKILL_VALID(SID_Horseguard))
		case SID_Horseguard:
			if (!(gBattleStats.config & BATTLE_CONFIG_ARENA) && CheckClassCavalry(UNIT_CLASS_ID(&defender->unit))) {
				for (i = 0; i < ARRAY_COUNT_RANGE3x3; i++) {
					int _x = attacker->unit.xPos + gVecs_3x3[i].x;
					int _y = attacker->unit.yPos + gVecs_3x3[i].y;

					struct Unit* unit_ally = GetUnitAtPosition(_x, _y);

					if (!UNIT_IS_VALID(unit_ally))
						continue;

					if (unit_ally->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
						continue;

					if (AreUnitsAllied(attacker->unit.index, unit_ally->index) && CheckClassCavalry(UNIT_CLASS_ID(unit_ally))) {
						attacker->battleDefense += SKILL_EFF0(SID_Horseguard);
						break;
					}
				}
			}
			break;
#endif

#if (defined(SID_Armorboost) && COMMON_SKILL_VALID(SID_Armorboost))
		case SID_Armorboost:
			if (!(gBattleStats.config & BATTLE_CONFIG_ARENA) && CheckClassArmor(UNIT_CLASS_ID(&defender->unit))) {
				for (i = 0; i < ARRAY_COUNT_RANGE3x3; i++) {
					int _x = attacker->unit.xPos + gVecs_3x3[i].x;
					int _y = attacker->unit.yPos + gVecs_3x3[i].y;

					struct Unit* unit_ally = GetUnitAtPosition(_x, _y);

					if (!UNIT_IS_VALID(unit_ally))
						continue;

					if (unit_ally->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
						continue;

					if (AreUnitsAllied(attacker->unit.index, unit_ally->index) && CheckClassArmor(UNIT_CLASS_ID(unit_ally))) {
						attacker->battleAttack += SKILL_EFF0(SID_Armorboost);

						if (!IsMagicAttack(defender))
							attacker->battleDefense += SKILL_EFF1(SID_Armorboost);

						break;
					}
				}
			}
			break;
#endif

#if (defined(SID_Admiration) && COMMON_SKILL_VALID(SID_Admiration))
		case SID_Admiration:
			tmp = 0;

			if (gBattleStats.config & BATTLE_CONFIG_ARENA)
				goto L_SID_Admiration_done;

			for (i = 0; i < 4; i++) {
				int _j;

				int _x = attacker->unit.xPos + gVecs_1x1[i].x;
				int _y = attacker->unit.yPos + gVecs_1x1[i].y;
				struct Unit* _unit = GetUnitAtPosition(_x, _y);

				if (!_unit)
					continue;

				if (!AreUnitsAllied(attacker->unit.index, _unit->index))
					continue;

				for (_j = 0; _j < 4; _j++) {
					int _x2 = _unit->xPos + gVecs_1x1[i].x;
					int _y2 = _unit->yPos + gVecs_1x1[i].y;

					struct Unit* _unit2 = GetUnitAtPosition(_x2, _y2);

					if (!_unit2)
						continue;

					if (!AreUnitsAllied(_unit2->index, _unit->index))
						continue;

					if (!!(UNIT_CATTRIBUTES(_unit2) & CA_FEMALE) == !!(UNIT_CATTRIBUTES(_unit) & CA_FEMALE)) {
						tmp = 1;
						goto L_SID_Admiration_done;
					}
				}
			}

		L_SID_Admiration_done:
			if (tmp == 1)
				attacker->battleDefense += 2;

			break;
#endif

#if (defined(SID_FairyTaleFolk) && COMMON_SKILL_VALID(SID_FairyTaleFolk))
		case SID_FairyTaleFolk:
			tmp = 0;

			if (gBattleStats.config & BATTLE_CONFIG_ARENA)
				goto L_FairyTaleFolk_done;

			for (i = 0; i < 4; i++) {
				int _j;
				int _x = attacker->unit.xPos + gVecs_1x1[i].x;
				int _y = attacker->unit.yPos + gVecs_1x1[i].y;
				struct Unit* _unit = GetUnitAtPosition(_x, _y);

				if (!_unit)
					continue;

				if (!AreUnitsAllied(attacker->unit.index, _unit->index))
					continue;

				for (_j = 0; _j < 4; _j++) {
					int _x2 = _unit->xPos + gVecs_1x1[i].x;
					int _y2 = _unit->yPos + gVecs_1x1[i].y;

					struct Unit* _unit2 = GetUnitAtPosition(_x2, _y2);

					if (!_unit2)
						continue;

					if (!AreUnitsAllied(_unit2->index, _unit->index))
						continue;

					if (!!(UNIT_CATTRIBUTES(_unit2) & CA_FEMALE) != !!(UNIT_CATTRIBUTES(_unit) & CA_FEMALE)) {
						tmp = 1;
						goto L_FairyTaleFolk_done;
					}
				}
			}

		L_FairyTaleFolk_done:
			if (tmp == 1)
				attacker->battleAttack += 2;

			break;
#endif

#if (defined(SID_Analytic) && (COMMON_SKILL_VALID(SID_Analytic)))
		case SID_Analytic:
			tmp = defender->battleSpeed - attacker->battleSpeed;
			if (tmp >= SKILL_EFF0(SID_Analytic))
				attacker->battleAttack += SKILL_EFF1(SID_Analytic);
			else if (tmp > 0)
				attacker->battleAttack += SKILL_EFF2(SID_Analytic);

			break;
#endif

#if (defined(SID_Chlorophyll) && (COMMON_SKILL_VALID(SID_Chlorophyll)))
		case SID_Chlorophyll:
			if (gPlaySt.chapterWeatherId == WEATHER_FLAMES)
				attacker->battleSpeed += BattleUnitOriginalStatus(attacker)->as;
			break;
#endif

#if (defined(SID_SlushRush) && (COMMON_SKILL_VALID(SID_SlushRush)))
		case SID_SlushRush:
			if (gPlaySt.chapterWeatherId == WEATHER_SNOW)
				attacker->battleSpeed += BattleUnitOriginalStatus(attacker)->as;
			break;
#endif

#if (defined(SID_SandRush) && (COMMON_SKILL_VALID(SID_SandRush)))
		case SID_SandRush:
			if (gPlaySt.chapterWeatherId == WEATHER_SANDSTORM)
				attacker->battleSpeed += BattleUnitOriginalStatus(attacker)->as;
			break;
#endif

#if (defined(SID_SwiftSwim) && (COMMON_SKILL_VALID(SID_SwiftSwim)))
		case SID_SwiftSwim:
			if (gPlaySt.chapterWeatherId == WEATHER_RAIN)
				attacker->battleSpeed += BattleUnitOriginalStatus(attacker)->as;
			break;
#endif

#if (defined(SID_MageSlayer) && (COMMON_SKILL_VALID(SID_MageSlayer)))
		case SID_MageSlayer:
			if (defender->unit.ranks[ITYPE_ANIMA] != 0 || defender->unit.ranks[ITYPE_LIGHT] != 0 || defender->unit.ranks[ITYPE_DARK] != 0 || defender->unit.ranks[ITYPE_STAFF]) {
				attacker->battleAttack += SKILL_EFF0(SID_MageSlayer);
				attacker->battleCritRate += SKILL_EFF1(SID_MageSlayer);
			}
			break;
#endif

#if (defined(SID_AirRaidAttack) && (COMMON_SKILL_VALID(SID_AirRaidAttack)))
		case SID_AirRaidAttack:
			if (!CanUnitCrossTerrain(&defender->unit, gBmMapTerrain[attacker->unit.yPos][attacker->unit.xPos]))
				attacker->battleAttack += SKILL_EFF0(SID_AirRaidAttack);
			break;
#endif

#if (defined(SID_AirRaidAvoid) && (COMMON_SKILL_VALID(SID_AirRaidAvoid)))
		case SID_AirRaidAvoid:
			if (!CanUnitCrossTerrain(&defender->unit, gBmMapTerrain[attacker->unit.yPos][attacker->unit.xPos]))
				attacker->battleAvoidRate += SKILL_EFF0(SID_AirRaidAvoid);
			break;
#endif

#if (defined(SID_AirRaidCrit) && (COMMON_SKILL_VALID(SID_AirRaidCrit)))
		case SID_AirRaidCrit:
			if (!CanUnitCrossTerrain(&defender->unit, gBmMapTerrain[attacker->unit.yPos][attacker->unit.xPos]))
				attacker->battleCritRate += SKILL_EFF0(SID_AirRaidCrit);
			break;
#endif

#if (defined(SID_AirDefense) && (COMMON_SKILL_VALID(SID_AirRaidDefense)))
		case SID_AirRaidDefense:
			if (!CanUnitCrossTerrain(&defender->unit, gBmMapTerrain[attacker->unit.yPos][attacker->unit.xPos]) && !IsMagicAttack(attacker))
				attacker->battleDefense += SKILL_EFF0(SID_AirRaidDefense);
			break;
#endif

#if (defined(SID_AirRaidHit) && (COMMON_SKILL_VALID(SID_AirRaidHit)))
		case SID_AirRaidHit:
			if (!CanUnitCrossTerrain(&defender->unit, gBmMapTerrain[attacker->unit.yPos][attacker->unit.xPos]))
				attacker->battleHitRate += SKILL_EFF0(SID_AirRaidHit);
			break;
#endif

#if (defined(SID_AirRaidResistance) && (COMMON_SKILL_VALID(SID_AirRaidResistance)))
		case SID_AirRaidResistance:
			if (!CanUnitCrossTerrain(&defender->unit, gBmMapTerrain[attacker->unit.yPos][attacker->unit.xPos]) && IsMagicAttack(attacker))
				attacker->battleDefense += SKILL_EFF0(SID_AirRaidResistance);
			break;
#endif

#if (defined(SID_AirRaidSpeed) && (COMMON_SKILL_VALID(SID_AirRaidSpeed)))
		case SID_AirRaidSpeed:
			if (!CanUnitCrossTerrain(&defender->unit, gBmMapTerrain[attacker->unit.yPos][attacker->unit.xPos]))
				attacker->battleSpeed += SKILL_EFF0(SID_AirRaidSpeed);
			break;
#endif

#if defined(SID_CloseCombat) && (COMMON_SKILL_VALID(SID_CloseCombat))
		case SID_CloseCombat:
			if (gBattleStats.range == 1)
				attacker->battleAttack += SKILL_EFF0(SID_CloseCombat);
			break;
#endif

#if (defined(SID_Pursuer) && (COMMON_SKILL_VALID(SID_Pursuer)))
		case SID_Pursuer:
			if (attacker == &gBattleActor) {
				attacker->battleSpeed += SKILL_EFF0(SID_Pursuer);
				attacker->battleAttack += attacker->unit.spd / SKILL_EFF1(SID_Pursuer);
			}
			break;
#endif

#if (defined(SID_DominantArm) && (COMMON_SKILL_VALID(SID_DominantArm)))
		case SID_DominantArm:
			if (GetItemType(attacker->weapon) == GetUnitBestWRankType(&attacker->unit))
				attacker->battleAttack += Div(GetItemMight(attacker->weapon) * SKILL_EFF0(SID_DominantArm), 100);

			break;
#endif

#if (defined(SID_MeleeManiac) && COMMON_SKILL_VALID(SID_MeleeManiac))
		case SID_MeleeManiac:
			// todo: move to extra info
			GetBaseDmg(attacker)->increase += 100;
			break;
#endif

#if (defined(SID_UnstoppableForce) && COMMON_SKILL_VALID(SID_UnstoppableForce))
		case SID_UnstoppableForce:
			// todo: move to extra info
			GetBaseDmg(attacker)->increase += 100;
			break;
#endif

#if defined(SID_SolarPower) && (COMMON_SKILL_VALID(SID_SolarPower))
		case SID_SolarPower:
			if (gPlaySt.chapterWeatherId == WEATHER_FLAMES && IsMagicAttack(attacker))
				GetBaseDmg(attacker)->increase += SKILL_EFF0(SID_SolarPower);

			break;
#endif

#if (defined(SID_LunarBrace) && (COMMON_SKILL_VALID(SID_LunarBrace)))
		case SID_LunarBrace:
			if (&gBattleActor == attacker)
				attacker->battleAttack +=
				perc_of(BattleUnitOriginalStatus(defender)->def, SKILL_EFF0(SID_LunarBrace));
			break;
#endif

#if (defined(SID_WyvernFlight) && (COMMON_SKILL_VALID(SID_WyvernFlight)))
		case SID_WyvernFlight:
			if (BattleUnitOriginalStatus(attacker)->as > (BattleUnitOriginalStatus(defender)->as - SKILL_EFF0(SID_WyvernFlight))) {
				tmp = BattleUnitOriginalStatus(attacker)->def - BattleUnitOriginalStatus(defender)->def;

				LIMIT_AREA(tmp, 0, SKILL_EFF1(SID_WyvernFlight));

				attacker->battleDefense += tmp;

				if (!IsMagicAttack(attacker))
					attacker->battleAttack += tmp;
			}
			break;
#endif

#if (defined(SID_LionFlame) && (COMMON_SKILL_VALID(SID_LionFlame)))
		case SID_LionFlame:
			if (gBattleStats.range <= 1) {
				attacker->battleCritRate += SKILL_EFF0(SID_LionFlame);
				attacker->battleAttack += SKILL_EFF1(SID_LionFlame);
			}
			else {
				attacker->battleCritRate -= SKILL_EFF2(SID_LionFlame);
				attacker->battleAttack -= SKILL_EFF3(SID_LionFlame);
			}
			break;
#endif

#if (defined(SID_Witch) && (COMMON_SKILL_VALID(SID_Witch)))
		case SID_Witch:
			switch (attacker->weaponSlotIndex) {
			case CHAX_BUISLOT_GAIDEN_BMAG1 ... CHAX_BUISLOT_GAIDEN_WMAG7:
				attacker->battleAttack += SKILL_EFF0(SID_Witch);
				break;

			default:
				break;
			}
			break;
#endif

#if (defined(SID_DishonorBreaker) && (COMMON_SKILL_VALID(SID_DishonorBreaker)))
		case SID_DishonorBreaker:
			if (!defender->canCounter) {
				attacker->battleHitRate -= 50;
				attacker->battleAvoidRate += 50;
			}
			break;
#endif

#if (defined(SID_BloodSurge) && COMMON_SKILL_VALID(SID_BloodSurge))
		case SID_BloodSurge:
			if (gBattleStats.config & BATTLE_CONFIG_SIMULATE)
				if (TryBattleHpCost(attacker, SKILL_EFF0(SID_BloodSurge)))
					attacker->battleAttack += perc_of(attacker->unit.maxHP, SKILL_EFF1(SID_BloodSurge));

			break;
#endif

#if (defined(SID_DragonsWrath) && (COMMON_SKILL_VALID(SID_DragonsWrath)))
		case SID_DragonsWrath:
			if (gBattleStats.config & BATTLE_CONFIG_SIMULATE)
				attacker->battleAttack += perc_of(BattleUnitOriginalStatus(attacker)->atk, SKILL_EFF0(SID_DragonsWrath));

			break;
#endif

#if defined(SID_SteadyBrawler) && (COMMON_SKILL_VALID(SID_SteadyBrawler))
		case SID_SteadyBrawler:
			if (gBattleStats.config & BATTLE_CONFIG_SIMULATE) {
				int local_mask = (attacker == &gBattleActor) ? UNWIND_DOUBLE_ACT : UNWIND_DOUBLE_TAR;

				if (gBattleFlagExt.round_mask & local_mask)
					GetBaseDmg(attacker)->increase += SKILL_EFF0(SID_SteadyBrawler);
			}
			break;
#endif

#if (defined(SID_Stubborn) && (COMMON_SKILL_VALID(SID_Stubborn)))
		case SID_Stubborn:
			if ((BattleUnitOriginalStatus(attacker)->atk + BattleUnitOriginalStatus(attacker)->def) <
				(BattleUnitOriginalStatus(defender)->atk + BattleUnitOriginalStatus(defender)->def)) {
				attacker->battleAttack += SKILL_EFF0(SID_Stubborn);
				attacker->battleSpeed += SKILL_EFF1(SID_Stubborn);
			}
			break;
#endif

#if (defined(SID_TurboMP) && (COMMON_SKILL_VALID(SID_TurboMP)))
		case SID_TurboMP:
			attacker->battleAttack += ((attacker->battleAttack / 3) * 2);
			break;
#endif

#if defined(SID_Crescendo) && (COMMON_SKILL_VALID(SID_Crescendo))
		case SID_Crescendo:
			attacker->battleCritRate += SKILL_EFF0(SID_Crescendo) * (gPlaySt.chapterTurnNumber > 10 ? 10 : gPlaySt.chapterTurnNumber);
			break;
#endif

#if (defined(SID_CriticalOverload) && (COMMON_SKILL_VALID(SID_CriticalOverload)))
		case SID_CriticalOverload:
			if (attacker->battleHitRate > 100)
			{
				int _crit_overflow = BattleUnitOriginalStatus(attacker)->hit - 100;
				if (_crit_overflow > 0)
					attacker->battleCritRate += _crit_overflow / SKILL_EFF0(SID_CriticalOverload);
			}
			break;
#endif

#if (defined(SID_Insomnia) && (COMMON_SKILL_VALID(SID_Insomnia)))
		case SID_Insomnia:
			if (GetUnitStatusIndex(GetUnit(attacker->unit.index)) == UNIT_STATUS_SLEEP)
				SetUnitStatus(GetUnit(attacker->unit.index), UNIT_STATUS_NONE);
			break;
#endif

#if (defined(SID_GoodAsGold) && (COMMON_SKILL_VALID(SID_GoodAsGold)))
		case SID_GoodAsGold:
			for (int i = 0; i < 9; i++)
			{
				if (GetUnitStatusIndex(GetUnit(attacker->unit.index)) == debuffs[i])
				{
					SetUnitStatus(GetUnit(attacker->unit.index), UNIT_STATUS_NONE);
					break;
				}
			}
			break;
#endif

#if (defined(SID_Cultured) && (COMMON_SKILL_VALID(SID_Cultured)))
#if (defined(SID_NiceThighs) && (COMMON_SKILL_VALID(SID_NiceThighs)))
		case SID_Cultured:
			if (BattleFastSkillTester(defender, SID_NiceThighs))
				attacker->battleHitRate -= SKILL_EFF0(SID_Cultured);
			break;
#endif
#endif

#if (defined(SID_Flank) && (COMMON_SKILL_VALID(SID_Flank)))
		case SID_Flank:
			if (gBattleStats.range == 1)
			{
				int activeX = attacker->unit.xPos;
				int targetX = defender->unit.xPos;
				int activeY = attacker->unit.yPos;
				int targetY = defender->unit.yPos;
				int dirX = activeX - targetX;
				int dirY = activeY - targetY;
				int deploymentID = attacker->unit.index;
				int allyID = 0;
				if ((dirX > 0) && (activeX > 1))
					allyID = gBmMapUnit[activeY][activeX - 2];
				if (dirX < 0)
					allyID = gBmMapUnit[activeY][activeX + 2];
				if ((dirY > 0) && (activeY > 1))
					allyID = gBmMapUnit[activeY - 2][activeX];
				if (dirY < 0)
					allyID = gBmMapUnit[activeY + 2][activeX];

				if ((allyID) && (AreUnitsAllied(deploymentID, allyID)))
				{
					int dmg = attacker->battleAttack - defender->battleDefense;
					if (dmg < 0)
						dmg = 0;
					int addDmg = ((dmg) * (SKILL_EFF0(SID_Flank))) / 100;
					attacker->battleAttack += addDmg;
				}
			}
			break;
#endif

#if (defined(SID_AdaptiveStance) && (COMMON_SKILL_VALID(SID_AdaptiveStance)))
		case SID_AdaptiveStance:
			int attackerRes = GetUnit(attacker->unit.index)->res;
			int attackerDef = GetUnit(attacker->unit.index)->def;

			attacker->battleDefense = (attackerRes >= attackerDef) ? attackerRes : attackerDef;
			break;
#endif

#if (defined(SID_DemolitionExpert) && (COMMON_SKILL_VALID(SID_DemolitionExpert)))
		case SID_DemolitionExpert:
			if (defender->unit.pCharacterData == GetCharacterData(CHARACTER_WALL) ||
				defender->unit.pCharacterData == GetCharacterData(CHARACTER_SNAG))
			{
				attacker->battleAttack = defender->unit.maxHP;
			}
			break;
#endif

#if (defined(SID_BeginnersLuck) && (COMMON_SKILL_VALID(SID_BeginnersLuck)))
		case SID_BeginnersLuck:
			tmp = gPlaySt.chapterIndex > SKILL_EFF0(SID_BeginnersLuck)
				? 0
				: SKILL_EFF0(SID_BeginnersLuck) - gPlaySt.chapterIndex;

			attacker->battleHitRate += tmp;
			attacker->battleAvoidRate += tmp;
			break;
#endif

#if (defined(SID_ExpertsExperience) && (COMMON_SKILL_VALID(SID_ExpertsExperience)))
		case SID_ExpertsExperience:
			tmp = SKILL_EFF0(SID_ExpertsExperience) + gPlaySt.chapterIndex;

			attacker->battleHitRate += tmp;
			attacker->battleAvoidRate += tmp;
			break;
#endif

#if (defined(SID_SwordProwess) && (COMMON_SKILL_VALID(SID_SwordProwess)))
		case SID_SwordProwess:
			unitRank = attacker->unit.ranks[ITYPE_SWORD];
			if (unitRank >= WPN_EXP_D && unitRank < WPN_EXP_C)
			{
				attacker->battleHitRate += SKILL_EFF0(SID_SwordProwess);
				attacker->battleAvoidRate += SKILL_EFF1(SID_SwordProwess);
				attacker->battleDodgeRate += SKILL_EFF0(SID_SwordProwess);
			}
			else if (unitRank >= WPN_EXP_C && unitRank < WPN_EXP_B)
			{
				attacker->battleHitRate += SKILL_EFF2(SID_SwordProwess);
				attacker->battleAvoidRate += SKILL_EFF3(SID_SwordProwess);
				attacker->battleDodgeRate += SKILL_EFF2(SID_SwordProwess);
			}
			else if (unitRank >= WPN_EXP_B && unitRank < WPN_EXP_A)
			{
				attacker->battleHitRate += SKILL_EFF4(SID_SwordProwess);
				attacker->battleAvoidRate += SKILL_EFF5(SID_SwordProwess);
				attacker->battleDodgeRate += SKILL_EFF4(SID_SwordProwess);
			}
			else if (unitRank >= WPN_EXP_A && unitRank < WPN_EXP_S)
			{
				attacker->battleHitRate += SKILL_EFF6(SID_SwordProwess);
				attacker->battleAvoidRate += SKILL_EFF7(SID_SwordProwess);
				attacker->battleDodgeRate += SKILL_EFF6(SID_SwordProwess);
			}
			else if (unitRank >= WPN_EXP_S)
			{
				attacker->battleHitRate += SKILL_EFF8(SID_SwordProwess);
				attacker->battleAvoidRate += SKILL_EFF9(SID_SwordProwess);
				attacker->battleDodgeRate += SKILL_EFF8(SID_SwordProwess);
			}
			break;
#endif

#if (defined(SID_LanceProwess) && (COMMON_SKILL_VALID(SID_LanceProwess)))
		case SID_LanceProwess:
			unitRank = attacker->unit.ranks[ITYPE_LANCE];
			if (unitRank >= WPN_EXP_D && unitRank < WPN_EXP_C)
			{
				attacker->battleHitRate += SKILL_EFF0(SID_LanceProwess);
				attacker->battleAvoidRate += SKILL_EFF1(SID_LanceProwess);
				attacker->battleDodgeRate += SKILL_EFF0(SID_LanceProwess);
			}
			else if (unitRank >= WPN_EXP_C && unitRank < WPN_EXP_B)
			{
				attacker->battleHitRate += SKILL_EFF2(SID_LanceProwess);
				attacker->battleAvoidRate += SKILL_EFF3(SID_LanceProwess);
				attacker->battleDodgeRate += SKILL_EFF2(SID_LanceProwess);
			}
			else if (unitRank >= WPN_EXP_B && unitRank < WPN_EXP_A)
			{
				attacker->battleHitRate += SKILL_EFF4(SID_LanceProwess);
				attacker->battleAvoidRate += SKILL_EFF5(SID_LanceProwess);
				attacker->battleDodgeRate += SKILL_EFF4(SID_LanceProwess);
			}
			else if (unitRank >= WPN_EXP_A && unitRank < WPN_EXP_S)
			{
				attacker->battleHitRate += SKILL_EFF6(SID_LanceProwess);
				attacker->battleAvoidRate += SKILL_EFF7(SID_LanceProwess);
				attacker->battleDodgeRate += SKILL_EFF6(SID_LanceProwess);
			}
			else if (unitRank >= WPN_EXP_S)
			{
				attacker->battleHitRate += SKILL_EFF8(SID_LanceProwess);
				attacker->battleAvoidRate += SKILL_EFF9(SID_LanceProwess);
				attacker->battleDodgeRate += SKILL_EFF8(SID_LanceProwess);
			}
			break;
#endif

#if (defined(SID_AxeProwess) && (COMMON_SKILL_VALID(SID_AxeProwess)))
		case SID_AxeProwess:
			unitRank = attacker->unit.ranks[ITYPE_AXE];
			if (unitRank >= WPN_EXP_D && unitRank < WPN_EXP_C)
			{
				attacker->battleHitRate += SKILL_EFF0(SID_AxeProwess);
				attacker->battleAvoidRate += SKILL_EFF1(SID_AxeProwess);
				attacker->battleDodgeRate += SKILL_EFF0(SID_AxeProwess);
			}
			else if (unitRank >= WPN_EXP_C && unitRank < WPN_EXP_B)
			{
				attacker->battleHitRate += SKILL_EFF2(SID_AxeProwess);
				attacker->battleAvoidRate += SKILL_EFF3(SID_AxeProwess);
				attacker->battleDodgeRate += SKILL_EFF2(SID_AxeProwess);
			}
			else if (unitRank >= WPN_EXP_B && unitRank < WPN_EXP_A)
			{
				attacker->battleHitRate += SKILL_EFF4(SID_AxeProwess);
				attacker->battleAvoidRate += SKILL_EFF5(SID_AxeProwess);
				attacker->battleDodgeRate += SKILL_EFF4(SID_AxeProwess);
			}
			else if (unitRank >= WPN_EXP_A && unitRank < WPN_EXP_S)
			{
				attacker->battleHitRate += SKILL_EFF6(SID_AxeProwess);
				attacker->battleAvoidRate += SKILL_EFF7(SID_AxeProwess);
				attacker->battleDodgeRate += SKILL_EFF6(SID_AxeProwess);
			}
			else if (unitRank >= WPN_EXP_S)
			{
				attacker->battleHitRate += SKILL_EFF8(SID_AxeProwess);
				attacker->battleAvoidRate += SKILL_EFF9(SID_AxeProwess);
				attacker->battleDodgeRate += SKILL_EFF8(SID_AxeProwess);
			}
			break;
#endif

#if (defined(SID_BowProwess) && (COMMON_SKILL_VALID(SID_BowProwess)))
		case SID_BowProwess:
			unitRank = attacker->unit.ranks[ITYPE_BOW];
			if (unitRank >= WPN_EXP_D && unitRank < WPN_EXP_C)
			{
				attacker->battleHitRate += SKILL_EFF0(SID_BowProwess);
				attacker->battleAvoidRate += SKILL_EFF1(SID_BowProwess);
				attacker->battleDodgeRate += SKILL_EFF0(SID_BowProwess);
			}
			else if (unitRank >= WPN_EXP_C && unitRank < WPN_EXP_B)
			{
				attacker->battleHitRate += SKILL_EFF2(SID_BowProwess);
				attacker->battleAvoidRate += SKILL_EFF3(SID_BowProwess);
				attacker->battleDodgeRate += SKILL_EFF2(SID_BowProwess);
			}
			else if (unitRank >= WPN_EXP_B && unitRank < WPN_EXP_A)
			{
				attacker->battleHitRate += SKILL_EFF4(SID_BowProwess);
				attacker->battleAvoidRate += SKILL_EFF5(SID_BowProwess);
				attacker->battleDodgeRate += SKILL_EFF4(SID_BowProwess);
			}
			else if (unitRank >= WPN_EXP_A && unitRank < WPN_EXP_S)
			{
				attacker->battleHitRate += SKILL_EFF6(SID_BowProwess);
				attacker->battleAvoidRate += SKILL_EFF7(SID_BowProwess);
				attacker->battleDodgeRate += SKILL_EFF6(SID_BowProwess);
			}
			else if (unitRank >= WPN_EXP_S)
			{
				attacker->battleHitRate += SKILL_EFF8(SID_BowProwess);
				attacker->battleAvoidRate += SKILL_EFF9(SID_BowProwess);
				attacker->battleDodgeRate += SKILL_EFF8(SID_BowProwess);
			}
			break;
#endif

#if (defined(SID_AnimaProwess) && (COMMON_SKILL_VALID(SID_AnimaProwess)))
		case SID_AnimaProwess:
			unitRank = attacker->unit.ranks[ITYPE_ANIMA];
			if (unitRank >= WPN_EXP_D && unitRank < WPN_EXP_C)
			{
				attacker->battleHitRate += SKILL_EFF0(SID_AnimaProwess);
				attacker->battleAvoidRate += SKILL_EFF1(SID_AnimaProwess);
				attacker->battleDodgeRate += SKILL_EFF0(SID_AnimaProwess);
			}
			else if (unitRank >= WPN_EXP_C && unitRank < WPN_EXP_B)
			{
				attacker->battleHitRate += SKILL_EFF2(SID_AnimaProwess);
				attacker->battleAvoidRate += SKILL_EFF3(SID_AnimaProwess);
				attacker->battleDodgeRate += SKILL_EFF2(SID_AnimaProwess);
			}
			else if (unitRank >= WPN_EXP_B && unitRank < WPN_EXP_A)
			{
				attacker->battleHitRate += SKILL_EFF4(SID_AnimaProwess);
				attacker->battleAvoidRate += SKILL_EFF5(SID_AnimaProwess);
				attacker->battleDodgeRate += SKILL_EFF4(SID_AnimaProwess);
			}
			else if (unitRank >= WPN_EXP_A && unitRank < WPN_EXP_S)
			{
				attacker->battleHitRate += SKILL_EFF6(SID_AnimaProwess);
				attacker->battleAvoidRate += SKILL_EFF7(SID_AnimaProwess);
				attacker->battleDodgeRate += SKILL_EFF6(SID_AnimaProwess);
			}
			else if (unitRank >= WPN_EXP_S)
			{
				attacker->battleHitRate += SKILL_EFF8(SID_AnimaProwess);
				attacker->battleAvoidRate += SKILL_EFF9(SID_AnimaProwess);
				attacker->battleDodgeRate += SKILL_EFF8(SID_AnimaProwess);
			}
			break;
#endif

#if (defined(SID_DarkProwess) && (COMMON_SKILL_VALID(SID_DarkProwess)))
		case SID_DarkProwess:
			unitRank = attacker->unit.ranks[ITYPE_DARK];
			if (unitRank >= WPN_EXP_D && unitRank < WPN_EXP_C)
			{
				attacker->battleHitRate += SKILL_EFF0(SID_DarkProwess);
				attacker->battleAvoidRate += SKILL_EFF1(SID_DarkProwess);
				attacker->battleDodgeRate += SKILL_EFF0(SID_DarkProwess);
			}
			else if (unitRank >= WPN_EXP_C && unitRank < WPN_EXP_B)
			{
				attacker->battleHitRate += SKILL_EFF2(SID_DarkProwess);
				attacker->battleAvoidRate += SKILL_EFF3(SID_DarkProwess);
				attacker->battleDodgeRate += SKILL_EFF2(SID_DarkProwess);
			}
			else if (unitRank >= WPN_EXP_B && unitRank < WPN_EXP_A)
			{
				attacker->battleHitRate += SKILL_EFF4(SID_DarkProwess);
				attacker->battleAvoidRate += SKILL_EFF5(SID_DarkProwess);
				attacker->battleDodgeRate += SKILL_EFF4(SID_DarkProwess);
			}
			else if (unitRank >= WPN_EXP_A && unitRank < WPN_EXP_S)
			{
				attacker->battleHitRate += SKILL_EFF6(SID_DarkProwess);
				attacker->battleAvoidRate += SKILL_EFF7(SID_DarkProwess);
				attacker->battleDodgeRate += SKILL_EFF6(SID_DarkProwess);
			}
			else if (unitRank >= WPN_EXP_S)
			{
				attacker->battleHitRate += SKILL_EFF8(SID_DarkProwess);
				attacker->battleAvoidRate += SKILL_EFF9(SID_DarkProwess);
				attacker->battleDodgeRate += SKILL_EFF8(SID_DarkProwess);
			}
			break;
#endif

#if (defined(SID_LightProwess) && (COMMON_SKILL_VALID(SID_LightProwess)))
		case SID_LightProwess:
			unitRank = attacker->unit.ranks[ITYPE_LIGHT];
			if (unitRank >= WPN_EXP_D && unitRank < WPN_EXP_C)
			{
				attacker->battleHitRate += SKILL_EFF0(SID_LightProwess);
				attacker->battleAvoidRate += SKILL_EFF1(SID_LightProwess);
				attacker->battleDodgeRate += SKILL_EFF0(SID_LightProwess);
			}
			else if (unitRank >= WPN_EXP_C && unitRank < WPN_EXP_B)
			{
				attacker->battleHitRate += SKILL_EFF2(SID_LightProwess);
				attacker->battleAvoidRate += SKILL_EFF3(SID_LightProwess);
				attacker->battleDodgeRate += SKILL_EFF2(SID_LightProwess);
			}
			else if (unitRank >= WPN_EXP_B && unitRank < WPN_EXP_A)
			{
				attacker->battleHitRate += SKILL_EFF4(SID_LightProwess);
				attacker->battleAvoidRate += SKILL_EFF5(SID_LightProwess);
				attacker->battleDodgeRate += SKILL_EFF4(SID_LightProwess);
			}
			else if (unitRank >= WPN_EXP_A && unitRank < WPN_EXP_S)
			{
				attacker->battleHitRate += SKILL_EFF6(SID_LightProwess);
				attacker->battleAvoidRate += SKILL_EFF7(SID_LightProwess);
				attacker->battleDodgeRate += SKILL_EFF6(SID_LightProwess);
			}
			else if (unitRank >= WPN_EXP_S)
			{
				attacker->battleHitRate += SKILL_EFF8(SID_LightProwess);
				attacker->battleAvoidRate += SKILL_EFF9(SID_LightProwess);
				attacker->battleDodgeRate += SKILL_EFF8(SID_LightProwess);
			}
			break;
#endif

#if (defined(SID_KillStreak) && (COMMON_SKILL_VALID(SID_KillStreak)) && defined(CONFIG_RESET_BWL_STATS_EACH_CHAPTER))
		case SID_KillStreak:
			attacker->battleCritRate += (bwl->winAmt * SKILL_EFF0(SID_KillStreak));
			break;
#endif

#if (defined(SID_HyperFocus) && (COMMON_SKILL_VALID(SID_HyperFocus)) && defined(CONFIG_RESET_BWL_STATS_EACH_CHAPTER))
		case SID_HyperFocus:
			attacker->battleHitRate += (bwl->battleAmt * SKILL_EFF0(SID_HyperFocus));
			break;
#endif

#if (defined(SID_MountainMan) && (COMMON_SKILL_VALID(SID_MountainMan)))
		case SID_MountainMan:
			attacker->battleHitRate += attacker->terrainAvoid;
			break;
#endif

#if (defined(SID_Emulate) && (COMMON_SKILL_VALID(SID_Emulate)))
		case SID_Emulate:
			attacker->hpInitial = defender->hpInitial;
			attacker->unit.curHP = defender->unit.curHP;
			attacker->unit.maxHP = defender->unit.maxHP;
			break;
#endif

#if (defined(SID_Capture) && (COMMON_SKILL_VALID(SID_Capture)))
		case SID_Capture:
			if (gActionData.unk08 == SID_Capture)
			{
				attacker->battleAttack -= Div(attacker->battleAttack * SKILL_EFF0(SID_Capture), 100);
				attacker->battleHitRate -= Div(attacker->battleHitRate * SKILL_EFF0(SID_Capture), 100);
				attacker->battleSpeed -= Div(attacker->battleSpeed * SKILL_EFF0(SID_Capture), 100);
			}
			break;
#endif

#if (defined(SID_Geomancy) && (COMMON_SKILL_VALID(SID_Geomancy)))
		case SID_Geomancy:
			if (gActionData.unk08 == SID_Geomancy)
			{
				switch (gBmMapTerrain[attacker->unit.yPos][attacker->unit.xPos])
				{
				case TERRAIN_PLAINS:
					attacker->battleCritRate += 15;
					break;
				case TERRAIN_MOUNTAIN:
				case TERRAIN_PEAK:
					attacker->battleAttack += 5;
					break;
				case TERRAIN_RIVER:
				case TERRAIN_WATER:
				case TERRAIN_SEA:
				case TERRAIN_LAKE:
					attacker->battleAvoidRate += 20;
					break;
				case TERRAIN_SAND:
					attacker->battleDodgeRate += 10;
					break;
				default:
					break;
				}
			}
			break;
#endif

#if (defined(SID_Calibration) && (COMMON_SKILL_VALID(SID_Calibration)))
		case SID_Calibration:
			attacker->battleHitRate += GetItemMaxUses(GetUnitEquippedWeapon(GetUnit(attacker->unit.index))) - ITEM_USES(GetUnitEquippedWeapon(GetUnit(attacker->unit.index)));
			break;
#endif

#if (defined(SID_Transpose) && (COMMON_SKILL_VALID(SID_Transpose)))
		case SID_Transpose:
			defender->battleAvoidRate -= defender->terrainAvoid * 2;
			break;
#endif


		case MAX_SKILL_NUM:
			Fatal("ENOSUPP");
			break;

		default:
			break;
		}
	}
}

void PreBattleCalcAuraEffect(struct BattleUnit* attacker, struct BattleUnit* defender)
{
	u32 i, _x, _y;
	struct Unit* unit;

	int allies_gRange3_In3x3 = 0;
	int allies_gRange2_In3x3 = 0;
	int allies_gRange1_In3x3 = 0;

	int enmies_gRange3_In3x3 = 0;
	int enmies_gRange2_In3x3 = 0;
	int enmies_gRange1_In3x3 = 0;

	bool lord_gRange2_In3x3 = false;

	if (gBattleStats.config & BATTLE_CONFIG_ARENA)
		return;

	for (i = 0; i < ARRAY_COUNT_RANGE3x3; i++) {
		_x = attacker->unit.xPos + gVecs_3x3[i].x;
		_y = attacker->unit.yPos + gVecs_3x3[i].y;

		unit = GetUnitAtPosition(_x, _y);
		if (!UNIT_IS_VALID(unit))
			continue;

		if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
			continue;

		if (AreUnitsAllied(attacker->unit.index, unit->index)) {
			/* Buffs */
			if (gRange1_In3x3[i] == 1) {
#if (defined(SID_DivinelyInspiring) && (COMMON_SKILL_VALID(SID_DivinelyInspiring)))
				if (SkillTester(unit, SID_DivinelyInspiring)) {
					attacker->battleAttack += SKILL_EFF0(SID_DivinelyInspiring);
					attacker->battleDefense += SKILL_EFF1(SID_DivinelyInspiring);
				}
#endif

#if (defined(SID_LilysPoise) && (COMMON_SKILL_VALID(SID_LilysPoise)))
				if (SkillTester(unit, SID_LilysPoise)) {
					attacker->battleAttack += SKILL_EFF0(SID_LilysPoise);
					attacker->battleDefense += SKILL_EFF1(SID_LilysPoise);
				}
#endif

#if (defined(SID_BloodTide) && (COMMON_SKILL_VALID(SID_BloodTide)))
				if (SkillTester(unit, SID_BloodTide)) {
					attacker->battleAttack += SKILL_EFF0(SID_BloodTide);
					attacker->battleHitRate += SKILL_EFF1(SID_BloodTide);
				}
#endif

#if (defined(SID_WhitePool) && (COMMON_SKILL_VALID(SID_WhitePool)))
				if (SkillTester(unit, SID_WhitePool)) {
					attacker->battleAttack += SKILL_EFF0(SID_WhitePool);
					attacker->battleSpeed += SKILL_EFF1(SID_WhitePool);
				}
#endif

#if (defined(SID_NightTide) && (COMMON_SKILL_VALID(SID_NightTide)))
				if (SkillTester(unit, SID_NightTide))
					attacker->battleDefense += SKILL_EFF0(SID_NightTide);
#endif

#if (defined(SID_SpurStr) && (COMMON_SKILL_VALID(SID_SpurStr)))
				if (SkillTester(unit, SID_SpurStr)) {
					if (!IsMagicAttack(attacker))
						attacker->battleAttack += SKILL_EFF0(SID_SpurStr);
				}
#endif

#if (defined(SID_SpurMag) && (COMMON_SKILL_VALID(SID_SpurMag)))
				if (SkillTester(unit, SID_SpurMag)) {
					if (IsMagicAttack(attacker))
						attacker->battleAttack += SKILL_EFF0(SID_SpurMag);
				}
#endif

#if (defined(SID_SpurDef) && (COMMON_SKILL_VALID(SID_SpurDef)))
				if (SkillTester(unit, SID_SpurDef)) {
					if (!IsMagicAttack(defender))
						attacker->battleDefense += SKILL_EFF0(SID_SpurDef);
				}
#endif

#if (defined(SID_SpurRes) && (COMMON_SKILL_VALID(SID_SpurRes)))
				if (SkillTester(unit, SID_SpurRes)) {
					if (IsMagicAttack(defender))
						attacker->battleDefense += SKILL_EFF0(SID_SpurRes);
				}
#endif

#if (defined(SID_SpurSpd) && (COMMON_SKILL_VALID(SID_SpurSpd)))
				if (SkillTester(unit, SID_SpurSpd))
					attacker->battleSpeed += SKILL_EFF0(SID_SpurSpd);
#endif

#if (defined(SID_Solidarity) && (COMMON_SKILL_VALID(SID_Solidarity)))
				if (SkillTester(unit, SID_Solidarity)) {
					attacker->battleCritRate += SKILL_EFF0(SID_Solidarity);
					attacker->battleDodgeRate += SKILL_EFF1(SID_Solidarity);
				}
#endif

#if defined(SID_UpWithArch) && (COMMON_SKILL_VALID(SID_UpWithArch))
                if (SkillTester(unit, SID_UpWithArch))
                {
                    char name[] = "Arch";
                    if (strcmp(GetStringFromIndex(GetUnit(attacker->unit.index)->pCharacterData->nameTextId), name) == 0)
                        attacker->battleAttack += GetUnitPower(unit);
                }
#endif
			}

			if (gRange2_In3x3[i] == 1) {
#if (defined(SID_Charm) && (COMMON_SKILL_VALID(SID_Charm)))
				if (SkillTester(unit, SID_Charm))
					attacker->battleAttack += SKILL_EFF0(SID_Charm);
#endif

#if (defined(SID_DriveStr) && (COMMON_SKILL_VALID(SID_DriveStr)))
				if (SkillTester(unit, SID_DriveStr)) {
					if (!IsMagicAttack(attacker))
						attacker->battleAttack += SKILL_EFF0(SID_DriveStr);
				}
#endif

#if (defined(SID_DriveMag) && (COMMON_SKILL_VALID(SID_DriveMag)))
				if (SkillTester(unit, SID_DriveMag)) {
					if (IsMagicAttack(attacker))
						attacker->battleAttack += SKILL_EFF0(SID_DriveMag);
				}
#endif

#if (defined(SID_DriveDef) && (COMMON_SKILL_VALID(SID_DriveDef)))
				if (SkillTester(unit, SID_DriveDef)) {
					if (!IsMagicAttack(defender))
						attacker->battleDefense += SKILL_EFF0(SID_DriveDef);
				}
#endif

#if (defined(SID_DriveRes) && (COMMON_SKILL_VALID(SID_DriveRes)))
				if (SkillTester(unit, SID_DriveRes)) {
					if (IsMagicAttack(defender))
						attacker->battleDefense += SKILL_EFF0(SID_DriveRes);
				}
#endif

#if (defined(SID_DriveSpd) && (COMMON_SKILL_VALID(SID_DriveSpd)))
				if (SkillTester(unit, SID_DriveSpd))
					attacker->battleSpeed += SKILL_EFF0(SID_DriveSpd);
#endif

#if (defined(SID_Inspiration) && (COMMON_SKILL_VALID(SID_Inspiration)))
				if (SkillTester(unit, SID_Inspiration)) {
					attacker->battleAttack += SKILL_EFF0(SID_Inspiration);
					attacker->battleDefense += SKILL_EFF1(SID_Inspiration);
				}
#endif

#if (defined(SID_Peacebringer) && (COMMON_SKILL_VALID(SID_Peacebringer)))
				if (SkillTester(unit, SID_Peacebringer))
					attacker->battleAttack -= SKILL_EFF0(SID_Peacebringer);
#endif

#if (defined(SID_Gentilhomme) && (COMMON_SKILL_VALID(SID_Gentilhomme)))
				if (SkillTester(unit, SID_Gentilhomme) && (UNIT_CATTRIBUTES(&attacker->unit) && CA_FEMALE))
					attacker->battleDefense += SKILL_EFF0(SID_Gentilhomme);
#endif

#if (defined(SID_Demoiselle) && (COMMON_SKILL_VALID(SID_Demoiselle)))
				if (SkillTester(unit, SID_Demoiselle) && !(UNIT_CATTRIBUTES(&attacker->unit) && CA_FEMALE))
					attacker->battleDefense += SKILL_EFF0(SID_Demoiselle);
#endif
			}

			/* Since we just calc in 3x3, so here is always true */
#if (defined(SID_Bond) && (COMMON_SKILL_VALID(SID_Bond)))
			if (SkillTester(unit, SID_Bond)) {
				attacker->battleAttack += SKILL_EFF0(SID_Bond);
				attacker->battleHitRate += SKILL_EFF1(SID_Bond);
			}
#endif

#if (defined(SID_Charisma) && (COMMON_SKILL_VALID(SID_Charisma)))
			if (SkillTester(unit, SID_Charisma)) {
				attacker->battleHitRate += SKILL_EFF0(SID_Charisma);
				attacker->battleAvoidRate += SKILL_EFF1(SID_Charisma);
			}
#endif

#if (defined(SID_ForeignPrincess) && (COMMON_SKILL_VALID(SID_ForeignPrincess)))
            if (SkillTester(unit, SID_ForeignPrincess) && CA_FEMALE)
            {
                attacker->battleAttack += SKILL_EFF0(SID_ForeignPrincess);
                attacker->battleDefense += SKILL_EFF0(SID_ForeignPrincess);
            }
#endif

#if (defined(SID_NiceThighs) && (COMMON_SKILL_VALID(SID_NiceThighs)))
            if (SkillTester(unit, SID_NiceThighs))
            {
#if (defined(SID_Thighdeology) && (COMMON_SKILL_VALID(SID_Thighdeology)))
                if (SkillTester(GetUnit(attacker->unit.index), SID_Thighdeology))
                {
                    attacker->battleAttack += SKILL_EFF0(SID_Thighdeology);
                    attacker->battleHitRate += SKILL_EFF1(SID_Thighdeology);
                }
#endif

#if (defined(SID_ThotSlayer) && (COMMON_SKILL_VALID(SID_ThotSlayer)))
                if (SkillTester(GetUnit(attacker->unit.index), SID_ThotSlayer))
                    attacker->battleCritRate += SKILL_EFF0(SID_ThotSlayer);
#endif
            }
#endif

#if (defined(SID_MarkOfTheBeast) && (COMMON_SKILL_VALID(SID_MarkOfTheBeast)))
        if (SkillTester(unit, SID_MarkOfTheBeast) && isMonsterClass(attacker->unit.pClassData->number))
        {
            attacker->battleAttack += SKILL_EFF0(SID_MarkOfTheBeast);
            attacker->battleDefense += SKILL_EFF0(SID_MarkOfTheBeast);
            attacker->battleCritRate += SKILL_EFF0(SID_MarkOfTheBeast);
            attacker->battleSpeed += SKILL_EFF0(SID_MarkOfTheBeast);
            attacker->battleAvoidRate += SKILL_EFF0(SID_MarkOfTheBeast);
            attacker->battleDodgeRate += SKILL_EFF0(SID_MarkOfTheBeast);
        }
#endif

			/* Since we just calc in 3x3, so here is always true */
			allies_gRange3_In3x3++;

			if (gRange2_In3x3[i]) {
				allies_gRange2_In3x3++;
				if (UNIT_CATTRIBUTES(unit) && CA_LORD)
					lord_gRange2_In3x3 = true;
			}

			if (gRange1_In3x3[i])
				allies_gRange1_In3x3++;
		}
		else {
			if (gRange1_In3x3[i] == 1) {
#if (defined(SID_Hex) && (COMMON_SKILL_VALID(SID_Hex)))
				if (SkillTester(unit, SID_Hex))
					attacker->battleAvoidRate -= SKILL_EFF0(SID_Hex);
#endif
			}

			if (gRange2_In3x3[i] == 1) {
#if (defined(SID_Intimidate) && (COMMON_SKILL_VALID(SID_Intimidate)))
				if (SkillTester(unit, SID_Intimidate))
					attacker->battleAvoidRate -= SKILL_EFF0(SID_Intimidate);
#endif

#if (defined(SID_VoiceOfPeace) && (COMMON_SKILL_VALID(SID_VoiceOfPeace)))
				if (SkillTester(unit, SID_VoiceOfPeace))
					attacker->battleAttack -= SKILL_EFF0(SID_VoiceOfPeace);
#endif

#if (defined(SID_Peacebringer) && (COMMON_SKILL_VALID(SID_Peacebringer)))
				if (SkillTester(unit, SID_Peacebringer))
					attacker->battleAttack -= SKILL_EFF0(SID_Peacebringer);
#endif

#if (defined(SID_MaleficAura) && (COMMON_SKILL_VALID(SID_MaleficAura)))
				if (SkillTester(unit, SID_MaleficAura)) {
					if (IsMagicAttack(defender))
						attacker->battleDefense -= SKILL_EFF0(SID_MaleficAura);
				}
#endif
			}

#if (defined(SID_Anathema) && (COMMON_SKILL_VALID(SID_Anathema)))
			if (SkillTester(unit, SID_Anathema)) {
				attacker->battleAvoidRate -= SKILL_EFF0(SID_Anathema);
				attacker->battleDodgeRate -= SKILL_EFF1(SID_Anathema);
			}
#endif

#if (defined(SID_Daunt) && (COMMON_SKILL_VALID(SID_Daunt)))
			if (SkillTester(unit, SID_Daunt)) {
				attacker->battleHitRate -= SKILL_EFF0(SID_Daunt);
				attacker->battleCritRate -= SKILL_EFF1(SID_Daunt);
			}
#endif

#if (defined(SID_RadiantEdict) && (COMMON_SKILL_VALID(SID_RadiantEdict)))
			if (SkillTester(unit, SID_RadiantEdict)) {
				const int buffLength = sizeof(buffs) / sizeof(buffs[0]);
				for (int i = 0; i < buffLength; i++)
				{
					if (GetUnitStatusIndex(&attacker->unit) == buffs[i])
					{
						SetUnitStatusIndex(GetUnit(attacker->unit.index), 0);
						break;
					}
				}
			}
#endif

			/* Since we just calc in 3x3, so here is always true */
			enmies_gRange3_In3x3++;

			if (gRange2_In3x3[i])
				enmies_gRange2_In3x3++;

			if (gRange1_In3x3[i])
				enmies_gRange1_In3x3++;
		}
	}

	if (allies_gRange3_In3x3 != 0) {
		/* Todo */
	}
	else {
#if (defined(SID_Tantivy) && (COMMON_SKILL_VALID(SID_Tantivy)))
		if (BattleFastSkillTester(attacker, SID_Tantivy)) {
			attacker->battleHitRate += SKILL_EFF0(SID_Tantivy);
			attacker->battleAvoidRate += SKILL_EFF1(SID_Tantivy);
		}
#endif

#if (defined(SID_Focus) && (COMMON_SKILL_VALID(SID_Focus)))
		if (BattleFastSkillTester(attacker, SID_Focus))
			attacker->battleCritRate += SKILL_EFF0(SID_Focus);
#endif
	}

	if (enmies_gRange3_In3x3 != 0) {
		/* Todo */
	}
	else {
		/* Todo */
	}

	if (enmies_gRange2_In3x3 >= 2) {
#if (defined(SID_Infiltrator) && (COMMON_SKILL_VALID(SID_Infiltrator)))
		if (BattleFastSkillTester(attacker, SID_Infiltrator)) {
			attacker->battleAttack += SKILL_EFF0(SID_Infiltrator);
			attacker->battleHitRate += SKILL_EFF1(SID_Infiltrator);
		}
#endif
	}

	if (allies_gRange1_In3x3 > 0) {
#if (defined(SID_BlueFlame) && (COMMON_SKILL_VALID(SID_BlueFlame)))
		if (BattleFastSkillTester(attacker, SID_BlueFlame))
			attacker->battleAttack += SKILL_EFF1(SID_BlueFlame);
#endif
	}

	/* AND skills */
	if (allies_gRange3_In3x3 == 0) {
#if (defined(SID_InnerFlame1) && (COMMON_SKILL_VALID(SID_InnerFlame1)))
		if (BattleFastSkillTester(attacker, SID_InnerFlame1))
			attacker->battleAttack += SKILL_EFF0(SID_InnerFlame1);
#endif
	}

	if (allies_gRange2_In3x3 == 0) {
#if (defined(SID_InnerFlame2) && (COMMON_SKILL_VALID(SID_InnerFlame2)))
		if (BattleFastSkillTester(attacker, SID_InnerFlame2))
			attacker->battleAttack += SKILL_EFF0(SID_InnerFlame2);
#endif
	}
	else if (allies_gRange2_In3x3 >= 2) {
#if (defined(SID_LawsOfSacae) && (COMMON_SKILL_VALID(SID_LawsOfSacae)))
		if (attacker == &gBattleTarget && BattleFastSkillTester(attacker, SID_LawsOfSacae)) {
			if (!IsMagicAttack(attacker))
				attacker->battleAttack += SKILL_EFF0(SID_LawsOfSacae);

			attacker->battleSpeed += SKILL_EFF0(SID_LawsOfSacae);
			attacker->battleDefense += SKILL_EFF0(SID_LawsOfSacae);
		}
#endif
	}

	if (allies_gRange1_In3x3 == 0) {
#if (defined(SID_InnerFlame3) && (COMMON_SKILL_VALID(SID_InnerFlame3)))
		if (BattleFastSkillTester(attacker, SID_InnerFlame3))
			attacker->battleAttack += SKILL_EFF0(SID_InnerFlame3);
#endif
	}

	if (lord_gRange2_In3x3) {
#if (defined(SID_Loyalty) && (COMMON_SKILL_VALID(SID_Loyalty)))
		if (BattleFastSkillTester(attacker, SID_Loyalty)) {
			attacker->battleHitRate += SKILL_EFF0(SID_Loyalty);
			attacker->battleDefense += SKILL_EFF1(SID_Loyalty);
		}
#endif
	}

	if (gpKernelDesigerConfig->battle_surrend_en && attacker == &gBattleTarget && (gBattleStats.config & BATTLE_CONFIG_REAL)) {
		/* Flyer in outdoor environments are not affected by this effect (todo) */
		if (!(UNIT_CATTRIBUTES(&attacker->unit) & CA_FLYER) || (0)) {
			int surround_enemies = enmies_gRange1_In3x3 - 1;

			/**
			 * When a unit is attacked and adjacent to the enemy,
			 * each side with enemy may cause unit avoid -10%
			 */
			if (surround_enemies > 0) {
				TriggerKtutorial(KTUTORIAL_BATTLE_SURROUNDER);
				attacker->battleAvoidRate -= 10 * surround_enemies;
			}

			/**
			 * If unit have been completely surrounded,
			 * unit may cause def-5 additionally.
			 */
			if (surround_enemies == 3)
				attacker->battleDefense -= 5;
		}
	}
}

void PreBattleCalcSilencerRate(struct BattleUnit* attacker, struct BattleUnit* defender)
{
	attacker->battleSilencerRate += gpSilencerBonus[UNIT_CLASS_ID(&attacker->unit)];

	if (UNIT_CATTRIBUTES(&defender->unit) & CA_BOSS)
		attacker->battleSilencerRate -= 25;
}

void PreBattleCalcPad(struct BattleUnit* attacker, struct BattleUnit* defender) {}

LYN_REPLACE_CHECK(ComputeBattleUnitStats);
void ComputeBattleUnitStats(struct BattleUnit* attacker, struct BattleUnit* defender)
{
	const PreBattleCalcFunc* it;

	LTRACEF("Trace pre-battle: attacker=%p", attacker);

	for (it = gpPreBattleCalcFuncs; *it; it++) {
		LTRACEF("Trace attack: %d", attacker->battleAttack);
		(*it)(attacker, defender);
	}

	LTRACE("Trace done");
}
