#include "common-chax.h"
#include "strmag.h"
#include "lvup.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "kernel-lib.h"

STATIC_DECLAR void CheckBattleUnitStatCapsVanilla(struct Unit *unit, struct BattleUnit *bu)
{
<<<<<<< HEAD
    int limitBreaker = 0;

#if defined(SID_LimitBreaker) && (COMMON_SKILL_VALID(SID_LimitBreaker))
    if (SkillTester(unit, SID_LimitBreaker))
        limitBreaker = SKILL_EFF0(SID_LimitBreaker);
#endif

#if defined(SID_LimitBreakerPlus) && (COMMON_SKILL_VALID(SID_LimitBreakerPlus))
    if (SkillTester(unit, SID_LimitBreakerPlus))
        limitBreaker = SKILL_EFF0(SID_LimitBreakerPlus);
#endif

#if defined(SID_OgreBody) && (COMMON_SKILL_VALID(SID_OgreBody))
    if (SkillTester(unit, SID_OgreBody))
    {
        if (unit->maxHP > SKILL_EFF0(SID_OgreBody))
            unit->maxHP = SKILL_EFF0(SID_OgreBody);
    }
    else
    {
        if (unit->maxHP > UNIT_MHP_MAX(unit) + limitBreaker)
            unit->maxHP = UNIT_MHP_MAX(unit) + limitBreaker;
    }
#endif

#ifdef CONFIG_UNLOCK_ALLY_MHP_LIMIT
    if (unit->maxHP > KUNIT_MHP_MAX(unit) + limitBreaker)
        unit->maxHP = KUNIT_MHP_MAX(unit) + limitBreaker;
#else
    if (unit->maxHP > UNIT_MHP_MAX(unit) + limitBreaker)
        unit->maxHP = UNIT_MHP_MAX(unit) + limitBreaker;
#endif

    if ((unit->pow + bu->changePow) > UNIT_POW_MAX(unit) + limitBreaker)
        bu->changePow = (UNIT_POW_MAX(unit)  + limitBreaker) - unit->pow;

    if ((unit->skl + bu->changeSkl) > UNIT_SKL_MAX(unit) + limitBreaker)
        bu->changeSkl = (UNIT_SKL_MAX(unit) + limitBreaker) - unit->skl;

    if ((unit->spd + bu->changeSpd) > UNIT_SPD_MAX(unit) + limitBreaker)
        bu->changeSpd = (UNIT_SPD_MAX(unit) + limitBreaker) - unit->spd;

    if ((unit->def + bu->changeDef) > UNIT_DEF_MAX(unit) + limitBreaker)
        bu->changeDef = (UNIT_DEF_MAX(unit) + limitBreaker) - unit->def;

    if ((unit->res + bu->changeRes) > UNIT_RES_MAX(unit) + limitBreaker)
        bu->changeRes = (UNIT_RES_MAX(unit) + limitBreaker) - unit->res;

    if ((unit->lck + bu->changeLck) > UNIT_LCK_MAX(unit) + limitBreaker)
        bu->changeLck = (UNIT_LCK_MAX(unit) + limitBreaker) - unit->lck;
=======
	if ((unit->maxHP + bu->changeHP) > GetUnitMaxStatusHp(unit))
		bu->changeHP = GetUnitMaxStatusHp(unit) - unit->maxHP;

	if ((unit->pow + bu->changePow) > GetUnitMaxStatusPow(unit))
		bu->changePow = GetUnitMaxStatusPow(unit) - unit->pow;

	if ((unit->skl + bu->changeSkl) > GetUnitMaxStatusSkl(unit))
		bu->changeSkl = GetUnitMaxStatusSkl(unit) - unit->skl;

	if ((unit->spd + bu->changeSpd) > GetUnitMaxStatusSpd(unit))
		bu->changeSpd = GetUnitMaxStatusSpd(unit) - unit->spd;

	if ((unit->def + bu->changeDef) > GetUnitMaxStatusDef(unit))
		bu->changeDef = GetUnitMaxStatusDef(unit) - unit->def;

	if ((unit->res + bu->changeRes) > GetUnitMaxStatusRes(unit))
		bu->changeRes = GetUnitMaxStatusRes(unit) - unit->res;

	if ((unit->lck + bu->changeLck) > GetUnitMaxStatusLck(unit))
		bu->changeLck = GetUnitMaxStatusLck(unit) - unit->lck;
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc
}

STATIC_DECLAR void UnitCheckStatCapsVanilla(struct Unit *unit)
{
<<<<<<< HEAD
    int limitBreaker = 0;

#if defined(SID_LimitBreaker) && (COMMON_SKILL_VALID(SID_LimitBreaker))
    if (SkillTester(unit, SID_LimitBreaker))
        limitBreaker = SKILL_EFF0(SID_LimitBreaker);
#endif

#if defined(SID_LimitBreakerPlus) && (COMMON_SKILL_VALID(SID_LimitBreakerPlus))
    if (SkillTester(unit, SID_LimitBreakerPlus))
        limitBreaker = SKILL_EFF0(SID_LimitBreakerPlus);
#endif

#if defined(SID_OgreBody) && (COMMON_SKILL_VALID(SID_OgreBody))
    if (SkillTester(unit, SID_OgreBody))
    {
        if (unit->maxHP > SKILL_EFF0(SID_OgreBody))
            unit->maxHP = SKILL_EFF0(SID_OgreBody);
    }
    else
    {
        if (unit->maxHP > UNIT_MHP_MAX(unit) + limitBreaker)
            unit->maxHP = UNIT_MHP_MAX(unit) + limitBreaker;
    }
#endif

#ifdef CONFIG_UNLOCK_ALLY_MHP_LIMIT
    if (unit->maxHP > KUNIT_MHP_MAX(unit) + limitBreaker)
        unit->maxHP = KUNIT_MHP_MAX(unit) + limitBreaker;
#else
    if (unit->maxHP > UNIT_MHP_MAX(unit) + limitBreaker)
        unit->maxHP = UNIT_MHP_MAX(unit) + limitBreaker;
#endif

    if (unit->pow > UNIT_POW_MAX(unit) + limitBreaker)
        unit->pow = UNIT_POW_MAX(unit) + limitBreaker;

    if (unit->skl > UNIT_SKL_MAX(unit) + limitBreaker)
        unit->skl = UNIT_SKL_MAX(unit) + limitBreaker;

    if (unit->spd > UNIT_SPD_MAX(unit) + limitBreaker)
        unit->spd = UNIT_SPD_MAX(unit) + limitBreaker;

    if (unit->def > UNIT_DEF_MAX(unit) + limitBreaker)
        unit->def = UNIT_DEF_MAX(unit) + limitBreaker;

    if (unit->res > UNIT_RES_MAX(unit) + limitBreaker)
        unit->res = UNIT_RES_MAX(unit) + limitBreaker;

    if (unit->lck > UNIT_LCK_MAX(unit) + limitBreaker)
        unit->lck = UNIT_LCK_MAX(unit) + limitBreaker;
=======
	if (unit->maxHP > GetUnitMaxStatusHp(unit))
		unit->maxHP = GetUnitMaxStatusHp(unit);

	if (unit->pow > GetUnitMaxStatusPow(unit))
		unit->pow = GetUnitMaxStatusPow(unit);

	if (unit->skl > GetUnitMaxStatusSkl(unit))
		unit->skl = GetUnitMaxStatusSkl(unit);

	if (unit->spd > GetUnitMaxStatusSpd(unit))
		unit->spd = GetUnitMaxStatusSpd(unit);

	if (unit->def > GetUnitMaxStatusDef(unit))
		unit->def = GetUnitMaxStatusDef(unit);

	if (unit->res > GetUnitMaxStatusRes(unit))
		unit->res = GetUnitMaxStatusRes(unit);

	if (unit->lck > GetUnitMaxStatusLck(unit))
		unit->lck = GetUnitMaxStatusLck(unit);
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc

	if (unit->conBonus > (GetUnitMaxStatusCon(unit) - UNIT_CON_BASE(unit)))
		unit->conBonus = (GetUnitMaxStatusCon(unit) - UNIT_CON_BASE(unit));

	if (unit->movBonus > (GetUnitMaxStatusMov(unit) - UNIT_MOV_BASE(unit)))
		unit->movBonus = (GetUnitMaxStatusMov(unit) - UNIT_MOV_BASE(unit));
}

LYN_REPLACE_CHECK(CheckBattleUnitStatCaps);
void CheckBattleUnitStatCaps(struct Unit *unit, struct BattleUnit *bu)
{
	CheckBattleUnitStatCapsVanilla(unit, bu);

<<<<<<< HEAD
    int limitBreaker = 0;

#if defined(SID_LimitBreaker) && (COMMON_SKILL_VALID(SID_LimitBreaker))
    if (SkillTester(unit, SID_LimitBreaker))
        limitBreaker = SKILL_EFF0(SID_LimitBreaker);
#endif

#if defined(SID_LimitBreakerPlus) && (COMMON_SKILL_VALID(SID_LimitBreakerPlus))
    if (SkillTester(unit, SID_LimitBreakerPlus))
        limitBreaker = SKILL_EFF0(SID_LimitBreakerPlus);
#endif

    /* Hooks */
    if ((UNIT_MAG(unit) + BU_CHG_MAG(bu)) > GetUnitMaxMagic(unit) + limitBreaker)
        BU_CHG_MAG(bu) = (GetUnitMaxMagic(unit) + limitBreaker) - UNIT_MAG(unit);
=======
	/* Hooks */
	if ((UNIT_MAG(unit) + BU_CHG_MAG(bu)) > GetUnitMaxStatusMag(unit))
		BU_CHG_MAG(bu) = GetUnitMaxStatusMag(unit) - UNIT_MAG(unit);
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc
}

#if 0
LYN_UNUSED_REPLACE_CHECK(UnitCheckStatCaps);
void UnitCheckStatCaps(struct Unit *unit)
#else
/* External hook to save spaces */
void _UnitCheckStatCaps(struct Unit *unit)
#endif
{
	UnitCheckStatCapsVanilla(unit);

<<<<<<< HEAD
    int limitBreaker = 0;

#if defined(SID_LimitBreaker) && (COMMON_SKILL_VALID(SID_LimitBreaker))
    if (SkillTester(unit, SID_LimitBreaker))
        limitBreaker = SKILL_EFF0(SID_LimitBreaker);
#endif

#if defined(SID_LimitBreakerPlus) && (COMMON_SKILL_VALID(SID_LimitBreakerPlus))
    if (SkillTester(unit, SID_LimitBreakerPlus))
        limitBreaker = SKILL_EFF0(SID_LimitBreakerPlus);
#endif

    /* Hooks */
    if (UNIT_MAG(unit) > GetUnitMaxMagic(unit) + limitBreaker)
        UNIT_MAG(unit) = GetUnitMaxMagic(unit) + limitBreaker;
=======
	/* Hooks */
	if (UNIT_MAG(unit) > GetUnitMaxStatusMag(unit))
		UNIT_MAG(unit) = GetUnitMaxStatusMag(unit);
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc
}
