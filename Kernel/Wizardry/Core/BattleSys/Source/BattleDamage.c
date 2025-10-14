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
#include "playst-expa.h"
#include "jester_headers/custom-functions.h"
#include "bwl.h"

static const u8 flyingClasses[8] = 
{
    CLASS_WYVERN_KNIGHT,
    CLASS_WYVERN_LORD,
    CLASS_WYVERN_RIDER,
    CLASS_DRACO_ZOMBIE,
    CLASS_GARGOYLE,
    CLASS_DEATHGOYLE,
    CLASS_PEGASUS_KNIGHT,
    CLASS_FALCON_KNIGHT
};

#define FLYING_CLASS_COUNT (sizeof(flyingClasses) / sizeof(flyingClasses[0]))

static bool isFlyingClass(int classId);
static bool isFlyingClass(int classId) {
    for (unsigned i = 0; i < FLYING_CLASS_COUNT; i++) {
        if (flyingClasses[i] == classId) {
            return true;
        }
    }
    return false;
}


#define LOCAL_TRACE 0

extern s8 Vanilla_CanUnitCrossTerrain(struct Unit * unit, int terrain);
extern bool Generic_CanUnitBeOnPos(struct Unit * unit, s8 x, s8 y, int x2, int y2);

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

	int base_damage, crit_correction, result;
	struct BaseDmg* base_dmg = GetBaseDmg(attacker);
	int max_damage = GetMaxDamage(attacker, defender);
    FORCE_DECLARE bool barricadePlus_activated;
    FORCE_DECLARE int roll12_ID = 15; // Set it to the maximum value for its bitfield, so it won't be accidentally triggered
    FORCE_DECLARE bool tintedLensPlus = false;

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

#if defined(SID_Roll12) && (COMMON_SKILL_VALID(SID_Roll12))
    if (BattleFastSkillTester(attacker, SID_Roll12))
        roll12_ID = NextRN_N(9);
#endif

	gDmg.crit_atk = false;
	gDmg.correction = 0;
	gDmg.crit_correction = gpKernelBattleDesignerConfig->crit_correction;

	gDmg.increase = base_dmg->increase;
	gDmg.decrease = base_dmg->decrease;
	gDmg.real_damage = base_dmg->real_damage;

	/**
	 * Roll critical and silencer attack
	 */

#if (defined(SID_Chipper) && (COMMON_SKILL_VALID(SID_Chipper)))
	if (BattleFastSkillTester(attacker, SID_Chipper))
		SetPartyGoldAmount(GetPartyGoldAmount() + SKILL_EFF0(SID_Chipper));
#endif
	 
	if (BattleRoll1RN(gBattleStats.critRate, false)) {
		gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_CRIT;
		gDmg.crit_atk = true;

#if defined(SID_AngerPoint) && (COMMON_SKILL_VALID(SID_AngerPoint))
		if (BattleFastSkillTester(defender, SID_AngerPoint) && !gBattleActorGlobalFlag.skill_activated_angerpoint)
			gBattleActorGlobalFlag.skill_activated_angerpoint = true;
#endif

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

#if (defined(SID_Swarm) && (COMMON_SKILL_VALID(SID_Swarm)))
    if (BattleFastSkillTester(attacker, SID_Swarm))
    {
        if (gBattleStats.range == 1)
        {
            struct Unit * unit = GetUnit(defender->unit.index);
            int x = unit->xPos;
            int x2 = attacker->unit.xPos;
            int y = unit->yPos;
            int y2 = attacker->unit.yPos;
            bool noUnit = false;

            // if the target can be on any adjacent position, do nothing
            if (Generic_CanUnitBeOnPos(unit, x + 1, y, x2, y2))
            {
                noUnit = true;
            }
            if (noUnit || Generic_CanUnitBeOnPos(unit, x - 1, y, x2, y2))
            {
                noUnit = true;
            }
            if (noUnit || Generic_CanUnitBeOnPos(unit, x, y + 1, x2, y2))
            {
                noUnit = true;
            }
            if (noUnit || Generic_CanUnitBeOnPos(unit, x, y - 1, x2, y2))
            {
                noUnit = true;
            }

            if (!noUnit)
            {
                int dmg = attacker->battleAttack - defender->battleDefense;
                if (dmg < 0)
                    dmg = 0;
                int addDmg = Div(dmg * SKILL_EFF0(SID_Swarm), 100);
                attacker->battleAttack += addDmg;
            }
        }
    }
#endif

#if defined(SID_RampUp) && (COMMON_SKILL_VALID(SID_RampUp))
    if (BattleFastSkillTester(attacker, SID_RampUp) && gDmg.crit_atk)
        base_damage*= 2;
#endif

	if (base_damage <= 0)
		base_damage = 0;

	/* JESTER - This is an early return for 0 battle damage that I'm turning off for SID_TintedLens */
	// if (base_damage <= 0 && gDmg.real_damage <= 0) {
	// 	/* If no damage taken, directly end the damage calculation */
	// 	gDmg.result = 0;
	// 	return 0;
	// }

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

#if (defined(SID_Bide) && (COMMON_SKILL_VALID(SID_Bide)))
    if (BattleFastSkillTester(attacker, SID_Bide) && gActionData.unk08 == SID_Bide)
        result = (attacker->unit.curHP - 1) * 2;
#endif

#if (defined(SID_TintedLensPlus) && (COMMON_SKILL_VALID(SID_TintedLensPlus)))
    if (BattleFastSkillTester(attacker, SID_TintedLensPlus))
    {
        if (result < 6)
        {
            tintedLensPlus = true;
            result = 6;
        }
    }
#endif

#if (defined(SID_TintedLens) && (COMMON_SKILL_VALID(SID_TintedLens)))
    if ((BattleFastSkillTester(attacker, SID_TintedLens) || BattleFastSkillTester(defender, SID_TintedLens)) && !tintedLensPlus)
    {
        if (result < 6)
			result = 6;
    }
#endif

#if (defined(SID_Decadon) && (COMMON_SKILL_VALID(SID_Decadon)))
    if (BattleFastSkillTester(attacker, SID_Decadon))
        result = 10;
#endif

#if (defined(SID_LethalityPlus) && (COMMON_SKILL_VALID(SID_LethalityPlus)))
    if (BattleFastSkillTester(attacker, SID_LethalityPlus))
        if (gActionData.unk08 == SID_LethalityPlus && !PlayStExpa_CheckBit(PLAYSTEXPA_BIT_LethalityPlus_Used))
        {
            PlayStExpa_SetBit(PLAYSTEXPA_BIT_LethalityPlus_Used);
            gActionData.unk08 = 0;
            result = defender->unit.curHP;
        }
#endif

#if (defined(SID_GrimReaper) && (COMMON_SKILL_VALID(SID_GrimReaper)))
    if (BattleFastSkillTester(attacker, SID_GrimReaper))
        if (gActionData.unk08 == SID_GrimReaper && CheckBitUES(GetUnit(attacker->unit.index), UES_BIT_GRIM_REAPER_SKILL_USED))
        {
            ClearBitUES(GetUnit(attacker->unit.index), UES_BIT_GRIM_REAPER_SKILL_USED);
            gActionData.unk08 = 0;
            result = defender->unit.curHP;
            
            if (UNIT_CATTRIBUTES(&defender->unit) && CA_BOSS)
                GetUnit(attacker->unit.index)->maxHP -= 5;
            else
                GetUnit(attacker->unit.index)->maxHP -= 1;
        }
#endif

#if (defined(SID_Levitate) && (COMMON_SKILL_VALID(SID_Levitate)))
        if (BattleFastSkillTester(defender, SID_Levitate))
        {
            if (IsMagicAttack(attacker) || isFlyingClass(UNIT_CLASS_ID(GetUnit(attacker->unit.index))))
                result = result;
            else
                result = 0;
        }
#endif

#if (defined(SID_Equalizer) && (COMMON_SKILL_VALID(SID_Equalizer)))
    if (BattleFastSkillTester(attacker, SID_Equalizer) || BattleFastSkillTester(defender, SID_Equalizer))
        result = ((defender->battleAttack - attacker->battleDefense) + (attacker->battleAttack - defender->battleDefense)) / 2;
#endif

#if (defined(SID_Osmose) && (COMMON_SKILL_VALID(SID_Osmose)))
    if (BattleFastSkillTester(attacker, SID_Osmose))
    {
        struct NewBwl * bwl = GetNewBwl(UNIT_CHAR_ID(GetUnit(attacker->unit.index)));
        bwl->currentMP += result/4;
        bwl->currentMP = (bwl->currentMP > bwl->maxMP) ? bwl->maxMP : bwl->currentMP; 
    }
#endif

	LTRACEF("[round %d] dmg=%d: max=%d, base=%d (atk=%d, def=%d, cor=%d), inc=%d, crt=%d, dec=%d, real=%d",
		GetBattleHitRound(gBattleHitIterator), result, max_damage, base_damage,
		gBattleStats.attack, gBattleStats.defense, gDmg.correction, gDmg.increase, crit_correction, gDmg.decrease, gDmg.real_damage);

	if (result > max_damage)
		result = max_damage;

	gDmg.result = result;
	return result;
}
