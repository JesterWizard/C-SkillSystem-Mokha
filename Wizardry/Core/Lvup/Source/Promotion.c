#include "common-chax.h"
#include "strmag.h"
#include "lvup.h"
#include "bwl.h"
#include "skill-system.h"

STATIC_DECLAR void ApplyUnitPromotionVanilla(struct Unit * unit, u8 classId, u8 bonusCaps)
{
    const struct ClassData* promotedClass = GetClassData(classId);

    int i;

    unit->maxHP = (unit->maxHP + promotedClass->promotionHp > KUNIT_MHP_MAX(unit)) ? KUNIT_MHP_MAX(unit) : unit->maxHP + promotedClass->promotionHp;
    unit->curHP = (unit->curHP + promotedClass->promotionHp > KUNIT_MHP_MAX(unit)) ? KUNIT_MHP_MAX(unit) : unit->curHP + promotedClass->promotionHp;

    unit->pow += promotedClass->promotionPow;
    unit->skl += promotedClass->promotionSkl;
    unit->spd += promotedClass->promotionSpd;
    unit->def += promotedClass->promotionDef;
    unit->res += promotedClass->promotionRes;

    if (bonusCaps == 0)
    {
        if (unit->pow > promotedClass->maxPow)
            unit->pow = promotedClass->maxPow;

        if (unit->skl > promotedClass->maxSkl)
            unit->skl = promotedClass->maxSkl;

        if (unit->spd > promotedClass->maxSpd)
            unit->spd = promotedClass->maxSpd;

        if (unit->def > promotedClass->maxDef)
            unit->def = promotedClass->maxDef;

        if (unit->res > promotedClass->maxRes)
            unit->res = promotedClass->maxRes;
    }

    // Remove base class' base wexp from unit wexp
    for (i = 0; i < 8; ++i)
        unit->ranks[i] -= unit->pClassData->baseRanks[i];

    // Update unit class
    unit->pClassData = promotedClass;

    // Add promoted class' base wexp to unit wexp
    for (i = 0; i < 8; ++i)
    {
        int wexp = unit->ranks[i];

        wexp += unit->pClassData->baseRanks[i];

        if (wexp > WPN_EXP_S)
            wexp = WPN_EXP_S;

        unit->ranks[i] = wexp;
    }

    unit->level = 1;
    unit->exp   = 0;
}

void GenerateBattleUnitStatGainsComparativelyVanilla(struct BattleUnit * bu, struct Unit * unit)
{

    int bonusCaps = 0;

#if defined(SID_LimitBreaker) && (COMMON_SKILL_VALID(SID_LimitBreaker))
        if (SkillTester(unit, SID_LimitBreaker))
            bonusCaps = 5;
#endif

#if defined(SID_LimitBreakerPlus) && (COMMON_SKILL_VALID(SID_LimitBreakerPlus))
        if (SkillTester(unit, SID_LimitBreakerPlus))
            bonusCaps = 10;
#endif

    bu->changeHP  = (bu->unit.maxHP + bonusCaps) - unit->maxHP;
    bu->changePow = (bu->unit.pow + bonusCaps)   - unit->pow;
    bu->changeSkl = (bu->unit.skl + bonusCaps)   - unit->skl;
    bu->changeSpd = (bu->unit.spd + bonusCaps)   - unit->spd;
    bu->changeDef = (bu->unit.def + bonusCaps)   - unit->def;
    bu->changeRes = (bu->unit.res + bonusCaps)   - unit->res;
    bu->changeLck = (bu->unit.lck + bonusCaps)   - unit->lck;
    BU_CHG_MAG(bu) = (UNIT_MAG(&bu->unit) + bonusCaps) - UNIT_MAG(unit);

    if (bu->unit.conBonus != unit->conBonus)
        bu->changeCon = bu->unit.conBonus - unit->conBonus;
    else
    {
        bu->changeCon = 0;
        bu->unit.conBonus = unit->conBonus;
    }
}

LYN_REPLACE_CHECK(ApplyUnitPromotion);
void ApplyUnitPromotion(struct Unit * unit, u8 jid)
{
    int bonusCaps = 0;

#if defined(SID_LimitBreaker) && (COMMON_SKILL_VALID(SID_LimitBreaker))
    if (SkillTester(unit, SID_LimitBreaker))
        bonusCaps = 5;
#endif

#if defined(SID_LimitBreakerPlus) && (COMMON_SKILL_VALID(SID_LimitBreakerPlus))
    if (SkillTester(unit, SID_LimitBreakerPlus))
        bonusCaps = 10;
#endif

    NewBwlRecordHiddenLevel(unit);
    ApplyUnitPromotionVanilla(unit, jid, bonusCaps);
    TryAddSkillPromotion(unit, jid);

    /* Hooks */
    UNIT_MAG(unit) += GetClassChgMagicBonus(jid);

    if (bonusCaps == 0)
    {
        if (UNIT_MAG(unit) > GetUnitMaxMagic(unit))
            UNIT_MAG(unit) = GetUnitMaxMagic(unit);
    }
}

LYN_REPLACE_CHECK(ApplyUnitDefaultPromotion);
void ApplyUnitDefaultPromotion(struct Unit * unit)
{
    ApplyUnitPromotion(unit, GetClassData(unit->pClassData->promotion)->number);
}

LYN_REPLACE_CHECK(GenerateBattleUnitStatGainsComparatively);
void GenerateBattleUnitStatGainsComparatively(struct BattleUnit * bu, struct Unit * unit)
{
    GenerateBattleUnitStatGainsComparativelyVanilla(bu, unit);
}
