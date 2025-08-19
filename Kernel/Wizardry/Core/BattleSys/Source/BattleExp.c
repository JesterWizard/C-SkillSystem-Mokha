#include "common-chax.h"
#include "skill-system.h"
#include "battle-system.h"
#include "bwl.h"
#include "constants/skills.h"

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

#if defined(SID_Blossom) && (COMMON_SKILL_VALID(SID_Blossom))
	if (BattleFastSkillTester(actor, SID_Blossom))
		status = status / 2;
#endif

#if defined(SID_Paragon) && (COMMON_SKILL_VALID(SID_Paragon))
	if (BattleFastSkillTester(actor, SID_Paragon))
		status = status * 2;
#endif

	/* Check last */
#if defined(SID_VoidCurse) && (COMMON_SKILL_VALID(SID_VoidCurse))
	if (BattleFastSkillTester(target, SID_VoidCurse))
		status = 0;
#endif

	LIMIT_AREA(status, 0, 100);
	if (base > 0 && status <= 0)
		status = 1;

	return status;
}

int GetBattleUnitExpGainRework(struct BattleUnit *actor, struct BattleUnit *target)
{
	int result = GetBattleUnitExpGain(actor, target);

	result = KernelModifyBattleUnitExp(result, actor, target);
	ModifyUnitSpecialExp(&actor->unit, &target->unit, &result);

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

#if CHAX
	ResetPopupSkillStack();
#endif

	CheckBattleUnitLevelUp(&gBattleActor);
}

int GetBattleUnitStaffExpRework(struct BattleUnit *bu)
{
	int result = 0;
	
#ifdef CONFIG_MODULAR_STAFF_EXP
    result = StaffEXP(ITEM_INDEX(bu->weapon));
#else
    result = GetBattleUnitStaffExp(bu);
#endif

	result = KernelModifyBattleUnitExp(
		result,
		bu,
		bu == &gBattleActor
			? &gBattleTarget
			: &gBattleActor);

	if (result > 100)
		result = 100;

	return result;
}

LYN_REPLACE_CHECK(BattleApplyItemExpGains);
void BattleApplyItemExpGains(void)
{
	if (gPlaySt.chapterStateBits & PLAY_FLAG_EXTRA_MAP)
		return;

#if CHAX
	ResetPopupSkillStack();
#endif

	if (gBattleActor.weaponAttributes & IA_STAFF) {
		if (UNIT_FACTION(&gBattleActor.unit) == FACTION_BLUE)
			gBattleActor.wexpMultiplier++;

		gBattleActor.expGain = GetBattleUnitStaffExpRework(&gBattleActor);
		gBattleActor.unit.exp += gBattleActor.expGain;

		CheckBattleUnitLevelUp(&gBattleActor);
	} else if ((gBattleActor.weaponType == ITYPE_12) && (gBattleActor.unit.exp != UNIT_EXP_DISABLED)) {
		gBattleActor.expGain = 20;
		gBattleActor.unit.exp += 20;

		CheckBattleUnitLevelUp(&gBattleActor);
	}
}

LYN_REPLACE_CHECK(BattleApplyExpGains);
void BattleApplyExpGains(void)
{
	if ((UNIT_FACTION(&gBattleActor.unit) != FACTION_BLUE) || (UNIT_FACTION(&gBattleTarget.unit) != FACTION_BLUE)) {
		if (!(gPlaySt.chapterStateBits & PLAY_FLAG_EXTRA_MAP)) {
			gBattleActor.expGain  = GetBattleUnitExpGainRework(&gBattleActor, &gBattleTarget);
			gBattleTarget.expGain = GetBattleUnitExpGainRework(&gBattleTarget, &gBattleActor);

			gBattleActor.unit.exp  += gBattleActor.expGain;
			gBattleTarget.unit.exp += gBattleTarget.expGain;

#if CHAX
			ResetPopupSkillStack();
#endif

			CheckBattleUnitLevelUp(&gBattleActor);
			CheckBattleUnitLevelUp(&gBattleTarget);
		}
	}
}
