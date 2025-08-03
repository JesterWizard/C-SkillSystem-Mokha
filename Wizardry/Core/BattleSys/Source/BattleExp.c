#include "common-chax.h"
#include "skill-system.h"
#include "battle-system.h"
#include "bwl.h"
#include "constants/skills.h"
#include "strmag.h"
#include "combat-art.h"
#include "constants/combat-arts.h"
#ifdef CONFIG_MODULAR_STAFF_EXP
    inline static int StaffEXP(int weapon)
    {
        int exp = 0;

        switch (weapon)
        {
        case ITEM_STAFF_HEAL:
        case ITEM_STAFF_TORCH:
            exp = 15;
            break;
    #ifdef CONFIG_ITEM_INDEX_FORCE_STAFF
        case CONFIG_ITEM_INDEX_FORCE_STAFF:
    #endif
    #ifdef CONFIG_ITEM_INDEX_TEMPEST_STAFF
        case CONFIG_ITEM_INDEX_TEMPEST_STAFF:
    #endif
    #ifdef CONFIG_ITEM_INDEX_ACUITY_STAFF
        case CONFIG_ITEM_INDEX_ACUITY_STAFF:
    #endif
    #ifdef CONFIG_ITEM_INDEX_SPRINT_STAFF
        case CONFIG_ITEM_INDEX_SPRINT_STAFF:
    #endif
    #ifdef CONFIG_ITEM_INDEX_FORTUNE_STAFF
        case CONFIG_ITEM_INDEX_FORTUNE_STAFF:
    #endif
    #ifdef CONFIG_ITEM_INDEX_IRON_STAFF
        case CONFIG_ITEM_INDEX_IRON_STAFF:
    #endif
        case ITEM_STAFF_BARRIER:
            exp = 17;
            break;
    #ifdef CONFIG_ITEM_INDEX_MINE_STAFF
        case CONFIG_ITEM_INDEX_MINE_STAFF:
    #endif
        case ITEM_STAFF_MEND:
            exp = 20;
            break;
        case ITEM_STAFF_UNLOCK:
        case ITEM_STAFF_RESTORE:
        case ITEM_STAFF_RECOVER:
            exp = 25;
            break;
        case ITEM_STAFF_BERSERK:
        case ITEM_STAFF_SLEEP:
        case ITEM_STAFF_SILENCE:
        case ITEM_STAFF_PHYSIC:
    #ifdef CONFIG_ITEM_INDEX_RUNE_STAFF
        case CONFIG_ITEM_INDEX_RUNE_STAFF:
    #endif
    #ifdef CONFIG_ITEM_INDEX_SLOW_STAFF
        case CONFIG_ITEM_INDEX_SLOW_STAFF:
    #endif
    #ifdef CONFIG_ITEM_INDEX_FORGE_STAFF
        case CONFIG_ITEM_INDEX_FORGE_STAFF:
    #endif
            exp = 30;
            break;
        case ITEM_STAFF_RESCUE:
            exp = 50;
            break;
        case ITEM_STAFF_WARP:
            exp = 60;
            break;
        case ITEM_STAFF_REPAIR:
        case ITEM_STAFF_FORTIFY:
            exp = 70;
            break;
        case ITEM_STAFF_LATONA:
            exp = 100;
            break;
        
        }
        return exp;
    }
#endif

LYN_REPLACE_CHECK(GetUnitExpLevel);
int GetUnitExpLevel(struct Unit *unit)
{
    int base, bonus;
    base = unit->level;

    if (CheckHasBwl(UNIT_CHAR_ID(unit)))
        bonus = GetUnitHiddenLevel(unit);
    else
        bonus = gpClassPreLoadHiddenLevel[UNIT_CLASS_ID(unit)];

    return base + bonus;
}

STATIC_DECLAR int KernelModifyBattleUnitExp(int base, struct BattleUnit *actor, struct BattleUnit *target)
{
    int status = base;
    u8 cid = GetCombatArtInForce(&actor->unit);

    switch (cid) {
    
    case CID_Practice:
        status = (status * 3 / 2);
        break;
    };

#if defined(SID_Blossom) && (COMMON_SKILL_VALID(SID_Blossom))
    if (BattleSkillTester(actor, SID_Blossom))
        status = status / 2;
#endif

#if defined(SID_Paragon) && (COMMON_SKILL_VALID(SID_Paragon))
    if (BattleSkillTester(actor, SID_Paragon))
        status = status * 2;
#endif

#if defined(SID_Mentorship) && (COMMON_SKILL_VALID(SID_Mentorship))
    if (BattleSkillTester(actor, SID_Mentorship))
        status = status + Div(status * SKILL_EFF0(SID_Mentorship), 100);
    else
    {
        for (int i = 0; i < ARRAY_COUNT_RANGE2x2; i++)
        {
            int _x = actor->unit.xPos + gVecs_2x2[i].x;
            int _y = actor->unit.yPos + gVecs_2x2[i].y;

            struct Unit *unit_ally = GetUnitAtPosition(_x, _y);

            if (!UNIT_IS_VALID(unit_ally))
                continue;

            if (unit_ally->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                continue;

            if (!AreUnitsAllied(actor->unit.index, unit_ally->index))
                continue;

            if (SkillTester(unit_ally, SID_Mentorship))
            {
                status = status * Div(status * SKILL_EFF0(SID_Mentorship), 100);;
                break;
            }
        }
    }
#endif

#if defined(SID_StaffParagon) && (COMMON_SKILL_VALID(SID_StaffParagon))
    int bonusEXP = 0;
    int unitMagicStat = GetUnitMagic(GetUnit(actor->unit.index));
    int itemHealAmount = GetUnitItemHealAmount(GetUnit(actor->unit.index), GetUnit(actor->unit.index)->items[gActionData.itemSlotIndex]);
    int hpChange = gBattleTarget.unit.curHP - gBattleTarget.hpInitial;

    if (BattleSkillTester(actor, SID_StaffParagon))
        bonusEXP = (unitMagicStat + itemHealAmount) - hpChange;

    /* Halve the exp gain if promoted */
    if (UNIT_CATTRIBUTES(&actor->unit) & CA_PROMOTED)
        bonusEXP /= 2;

    /* The final score shoud be 1/5 of excess healing */
    bonusEXP /= 5;

    status += bonusEXP;
#endif

#if defined(SID_Prodigy) && (COMMON_SKILL_VALID(SID_Prodigy))
    if (BattleSkillTester(actor, SID_Prodigy))
    {
        LIMIT_AREA(status, 1, 254);
    }
    else
    {
        LIMIT_AREA(status, 1, 100);
    }
#else
        LIMIT_AREA(status, 1, 100);
#endif

#if defined(SID_Pulse) && (COMMON_SKILL_VALID(SID_Pulse))
    if (BattleSkillTester(actor, SID_Pulse))
    {
        if (status < 10)
            status = 10;
    }
#endif

    /* Check last */
#if defined(SID_VoidCurse) && (COMMON_SKILL_VALID(SID_VoidCurse))
    if (BattleSkillTester(target, SID_VoidCurse))
        status = 0;
#endif

    return status;
}

LYN_REPLACE_CHECK(GetBattleUnitExpGain);
int GetBattleUnitExpGain(struct BattleUnit *actor, struct BattleUnit *target)
{
    int result;

    if (!CanBattleUnitGainLevels(actor) || (actor->unit.curHP == 0) || UNIT_CATTRIBUTES(&target->unit) & CA_NEGATE_LETHALITY)
        return 0;

    if (!actor->nonZeroDamage)
        return 1;

    result = GetUnitRoundExp(&actor->unit, &target->unit);
    result += GetUnitKillExpBonus(&actor->unit, &target->unit);

    result = KernelModifyBattleUnitExp(result, actor, target);

#if !CHAX
    ModifyUnitSpecialExp(&actor->unit, &target->unit, &result);
#endif

    return result;
}

LYN_REPLACE_CHECK(BattleApplyMiscActionExpGains);
void BattleApplyMiscActionExpGains(void)
{
    int exp;

    if (UNIT_FACTION(&gBattleActor.unit) != FACTION_BLUE)
        return;

    if (!CanBattleUnitGainLevels(&gBattleActor))
        return;

    if (gPlaySt.chapterStateBits & PLAY_FLAG_EXTRA_MAP)
        return;

    exp = 10;
    exp = KernelModifyBattleUnitExp(exp, &gBattleActor, &gBattleTarget);

    gBattleActor.expGain = exp;
    gBattleActor.unit.exp += exp;

    CheckBattleUnitLevelUp(&gBattleActor);
}

LYN_REPLACE_CHECK(GetBattleUnitStaffExp);
int GetBattleUnitStaffExp(struct BattleUnit *bu)
{
    int result;

    if (!CanBattleUnitGainLevels(bu))
        return 0;

    if (gBattleHitArrayRe->attributes & BATTLE_HIT_ATTR_MISS)
        return 1;

#ifdef CONFIG_MODULAR_STAFF_EXP
    result = StaffEXP(ITEM_INDEX(bu->weapon));
#else
    result = 10 + GetItemCostPerUse(bu->weapon) / 20;
#endif

    if (UNIT_CATTRIBUTES(&bu->unit) & CA_PROMOTED)
        result = result / 2;

    result = KernelModifyBattleUnitExp(
        result,
        bu,
        bu == &gBattleActor
            ? &gBattleTarget
            : &gBattleActor);

#if defined(SID_Prodigy) && (COMMON_SKILL_VALID(SID_Prodigy))
    if (BattleSkillTester(bu, SID_Prodigy))
    {
        if (result > 254)
            result = 254;
    }
    else 
    {
        if (result > 100)
            result = 100;      
    }
#else
    if (result > 100)
        result = 100;
#endif

    return result;
}

LYN_REPLACE_CHECK(BattleApplyExpGains);
void BattleApplyExpGains(void) {
    if ((UNIT_FACTION(&gBattleActor.unit) != FACTION_BLUE) || (UNIT_FACTION(&gBattleTarget.unit) != FACTION_BLUE)) {
        if (!(gPlaySt.chapterStateBits & PLAY_FLAG_EXTRA_MAP)) {
            gBattleActor.expGain  = GetBattleUnitExpGain(&gBattleActor, &gBattleTarget);
            gBattleTarget.expGain = GetBattleUnitExpGain(&gBattleTarget, &gBattleActor);

            gBattleActor.unit.exp  += gBattleActor.expGain;
            gBattleTarget.unit.exp += gBattleTarget.expGain;

            CheckBattleUnitLevelUp(&gBattleActor);
            CheckBattleUnitLevelUp(&gBattleTarget);
        }
    }
}