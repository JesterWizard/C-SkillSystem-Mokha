#include "common-chax.h"
#include "strmag.h"
#include "lvup.h"
#include "bwl.h"
#include "skill-system.h"

STATIC_DECLAR void ApplyUnitPromotionVanilla(struct Unit * unit, u8 classId)
{
    const struct ClassData* promotedClass = GetClassData(classId);

    int i;

    // Apply stat ups

    // unit->maxHP += promotedClass->promotionHp;

    // if (unit->maxHP > promotedClass->maxHP)
    //     unit->maxHP = promotedClass->maxHP;

    // unit->curHP += promotedClass->promotionHp;

    // if (unit->curHP > GetUnitMaxHp(unit))
    //     unit->curHP = GetUnitMaxHp(unit);

    unit->maxHP = (unit->maxHP + promotedClass->promotionHp > KUNIT_MHP_MAX(unit)) ? KUNIT_MHP_MAX(unit) : unit->maxHP + promotedClass->promotionHp;
    unit->curHP = (unit->curHP + promotedClass->promotionHp > KUNIT_MHP_MAX(unit)) ? KUNIT_MHP_MAX(unit) : unit->curHP + promotedClass->promotionHp;

    unit->pow += promotedClass->promotionPow;

    if (unit->pow > promotedClass->maxPow)
        unit->pow = promotedClass->maxPow;

    unit->skl += promotedClass->promotionSkl;

    if (unit->skl > promotedClass->maxSkl)
        unit->skl = promotedClass->maxSkl;

    unit->spd += promotedClass->promotionSpd;

    if (unit->spd > promotedClass->maxSpd)
        unit->spd = promotedClass->maxSpd;

    unit->def += promotedClass->promotionDef;

    if (unit->def > promotedClass->maxDef)
        unit->def = promotedClass->maxDef;

    unit->res += promotedClass->promotionRes;

    if (unit->res > promotedClass->maxRes)
        unit->res = promotedClass->maxRes;

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
    bu->changeHP  = bu->unit.maxHP - unit->maxHP;
    bu->changePow = bu->unit.pow   - unit->pow;
    bu->changeSkl = bu->unit.skl   - unit->skl;
    bu->changeSpd = bu->unit.spd   - unit->spd;
    bu->changeDef = bu->unit.def   - unit->def;
    bu->changeRes = bu->unit.res   - unit->res;
    bu->changeLck = bu->unit.lck   - unit->lck;

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
    NewBwlRecordHiddenLevel(unit);
    ApplyUnitPromotionVanilla(unit, jid);
    TryAddSkillPromotion(unit, jid);

    /* Hooks */
    UNIT_MAG(unit) += GetClassChgMagicBonus(jid);
    if (UNIT_MAG(unit) > GetUnitMaxMagic(unit))
        UNIT_MAG(unit) = GetUnitMaxMagic(unit);
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

    /* Hooks */
    BU_CHG_MAG(bu) = UNIT_MAG(&bu->unit) - UNIT_MAG(unit);
}
