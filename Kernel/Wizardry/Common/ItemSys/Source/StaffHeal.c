#include "common-chax.h"
#include "status-getter.h"
#include "skill-system.h"
#include "battle-system.h"
#include "item-sys.h"
#include "kernel-lib.h"
#include "constants/skills.h"
#include "strmag.h"
#include "debuff.h"
#include "bmunit.h"
#include "lvup.h"
#include "jester_headers/custom-functions.h"

typedef int (*HealAmountGetterFunc_t)(int old, struct Unit *actor, struct Unit *target);
extern HealAmountGetterFunc_t const *const gpHealAmountGetters;

static int find_item_slot(struct Unit *unit, int item)
{
	int i;
	int iid = ITEM_INDEX(item);
	for (i = 0; i < UNIT_ITEM_COUNT; i++)
		if (ITEM_INDEX(unit->items[i]) == iid)
			return i;
	return -1;
}

int HealAmountGetter(int base, struct Unit *actor, struct Unit *target)
{
	const HealAmountGetterFunc_t *it;
	int status = base;

	for (it = gpHealAmountGetters; *it; it++)
		status = (*it)(status, actor, target);

	LIMIT_AREA(status, 0, 80);
	return status;
}

LYN_REPLACE_CHECK(ExecStandardHeal);
void ExecStandardHeal(ProcPtr proc)
{
	int amount;

	struct Unit *unit_act = GetUnit(gActionData.subjectIndex);
	struct Unit *unit_tar = GetUnit(gActionData.targetIndex);

	BattleInitItemEffect(unit_act, gActionData.itemSlotIndex);

	BattleInitItemEffectTarget(unit_tar);

/* Having crashing issues with the switch case for making this work on staves only */
#if defined(SID_WhiteMage) && (COMMON_SKILL_VALID(SID_WhiteMage))
    if (SkillTester(unit_act, SID_WhiteMage))
    {
        amount += amount / 2;
        // switch (unit_act->items[gActionData.itemSlotIndex])
        // {
        // case ITEM_STAFF_HEAL:
        // case ITEM_STAFF_MEND:
        // case ITEM_STAFF_PHYSIC:
        // case ITEM_STAFF_FORTIFY:
        //     amount += amount / 2;
        //     break;
        // }
    }
#endif

	amount = GetUnitItemHealAmount(
		unit_act,
		GetItemFromSlot(unit_act, gActionData.itemSlotIndex)
	);

#if CHAX
	amount = HealAmountGetter(amount, unit_act, unit_tar);
#endif

	/* Judge on actor range 3x3 */
	for (int i = 0; i < ARRAY_COUNT_RANGE3x3; i++) {
		int _x = unit_act->xPos + gVecs_3x3[i].x;
		int _y = unit_act->yPos + gVecs_3x3[i].y;

		struct Unit *unit_target = GetUnitAtPosition(_x, _y);

		if (!UNIT_IS_VALID(unit_target))
			continue;

		if (AreUnitsAllied(unit_act->index, unit_target->index))
			continue;
		
#if (defined(SID_Blight) && COMMON_SKILL_VALID(SID_Blight))
			if (SkillTester(unit_target, SID_Blight)) {
				amount *= -1;
			}
#endif
		
		break;
	}

/* Don't let the target be killed */
#if defined(SID_CursedHeal) && (COMMON_SKILL_VALID(SID_CursedHeal))
    if (SkillTester(unit_act, SID_CursedHeal))
        SetUnitHp(unit_tar, (GetUnitCurrentHp(unit_tar) - amount) > 0 ? (GetUnitCurrentHp(unit_tar) - amount) : 1);
    else 
        AddUnitHp(unit_tar, amount);
#else 
    AddUnitHp(unit_tar, amount);
#endif

#if defined(SID_ExplosiveHeal) && (COMMON_SKILL_VALID(SID_ExplosiveHeal))
    if (SkillTester(unit_act, SID_ExplosiveHeal))
    {
        for (int i = 0; i < ARRAY_COUNT_RANGE1x1; i++)
        {
            int _x = gActiveUnit->xPos + gVecs_1x1[i].x;
            int _y = gActiveUnit->yPos + gVecs_1x1[i].y;
    
            struct Unit * unit_adjacent = GetUnitAtPosition(_x, _y);

            if (!UNIT_IS_VALID(unit_adjacent))
                continue;
    
            if (unit_adjacent->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                continue;
    
            if (AreUnitsAllied(gActiveUnit->index, unit_adjacent->index) && GetUnit(unit_adjacent->index) != unit_tar)
            {
                int amount_real = HealAmountGetter(amount, unit_act, unit_adjacent);
                AddUnitHp(unit_adjacent, amount_real);
            }
        }
    }
#endif

#if (defined(SID_Zenkai) && (COMMON_SKILL_VALID(SID_Zenkai)))
    if (SkillTester(unit_tar, SID_Zenkai))
    {
        // If the unit start at 30% or less HP and then got healed to full, give them bonus EXP
        if(gBattleTarget.unit.curHP <= (Div(GetUnitMaxHp(unit_tar), 10) * 3) && GetUnitCurrentHp(unit_tar) == GetUnitMaxHp(unit_tar))
        {
            if (unit_tar->level < UNIT_LEVEL_MAX_RE)
            {
                unit_tar->exp += SKILL_EFF0(SID_Zenkai);
                
                if (unit_tar->exp >= 100)
                {
                    // This is a bit of a hack, but it works for now.
                    unit_tar->pow += GetStatIncrease_NEW(GetUnitPowGrowth(unit_tar), unit_tar->exp);
                    UNIT_MAG(unit_tar) += GetStatIncrease_NEW(GetUnitMagGrowth(unit_tar), unit_tar->exp);
                    unit_tar->skl += GetStatIncrease_NEW(GetUnitSklGrowth(unit_tar), unit_tar->exp);
                    unit_tar->spd += GetStatIncrease_NEW(GetUnitSpdGrowth(unit_tar), unit_tar->exp);
                    unit_tar->def += GetStatIncrease_NEW(GetUnitDefGrowth(unit_tar), unit_tar->exp);
                    unit_tar->res += GetStatIncrease_NEW(GetUnitResGrowth(unit_tar), unit_tar->exp);
                    unit_tar->lck += GetStatIncrease_NEW(GetUnitLckGrowth(unit_tar), unit_tar->exp);
                    unit_tar->maxHP += GetStatIncrease_NEW(GetUnitHpGrowth(unit_tar), unit_tar->exp);

                    unit_tar->level++;
                    if (unit_tar->level > 20)
                        unit_tar->exp = UNIT_EXP_DISABLED;
                    else
                        unit_tar->exp -= 100;
                }
            }
        }
    }
#endif

	gBattleHitIterator->hpChange = gBattleTarget.unit.curHP - GetUnitCurrentHp(unit_tar);
	gBattleTarget.unit.curHP = GetUnitCurrentHp(unit_tar);

#if (defined(SID_SelfHealing) && (COMMON_SKILL_VALID(SID_SelfHealing)))
    if (SkillTester(unit_act, SID_SelfHealing))
    {
        if (gActiveUnit == unit_tar)
        {
            AddUnitHp(unit_act, amount);
            gBattleHitIterator->hpChange = gBattleActor.unit.curHP - GetUnitCurrentHp(unit_act);
            gBattleActor.unit.curHP = GetUnitCurrentHp(unit_act);
        }
    }
#endif

#if (defined(SID_WeaponHeal) && (COMMON_SKILL_VALID(SID_WeaponHeal)))
    if (SkillTester(unit_act, SID_WeaponHeal) && Roll1RN(SKILL_EFF0(SID_WeaponHeal)))
    {
        int slot = 0;
        int weapon = gBattleTarget.weaponBefore;

        if (weapon != ITEM_NONE)
        {
            if (GetItemUses(weapon) < GetItemMaxUses(weapon))
            {
                slot = find_item_slot(&gBattleTarget.unit, weapon);
				unit_tar->items[slot] = (weapon += (1 << 8));
            }
        }
    }
#endif

#if (defined(SID_FortifyingStaff) && (COMMON_SKILL_VALID(SID_FortifyingStaff)))
    if (SkillTester(unit_act, SID_FortifyingStaff) && (unit_tar->statusIndex == UNIT_STATUS_NONE))
            unit_tar->statusIndex = UNIT_STATUS_DEFENSE;
#endif

#if (defined(SID_LiveToServe) && (COMMON_SKILL_VALID(SID_LiveToServe)))
    if (SkillTester(unit_act, SID_LiveToServe))
    {
        AddUnitHp(unit_act, amount);
        gBattleHitIterator->hpChange = gBattleActor.unit.curHP - GetUnitCurrentHp(unit_act);
        gBattleActor.unit.curHP = GetUnitCurrentHp(unit_act);
        gActionData.unk08 = SID_LiveToServe;
    }
#endif


	BattleApplyItemEffect(proc);
	BeginBattleAnimations();
}

LYN_REPLACE_CHECK(ExecFortify);
void ExecFortify(ProcPtr proc)
{
	int i;
	int amount;
	int targetCount;

	struct Unit *unit_act = GetUnit(gActionData.subjectIndex);
    FORCE_DECLARE struct Unit *unit_tar = GetUnit(gActionData.targetIndex);

	BattleInitItemEffect(unit_act, gActionData.itemSlotIndex);

	BattleInitItemEffectTarget(GetUnitFromCharId(GetPlayerLeaderUnitId()));
	MakeTargetListForRangedHeal(unit_act);

	amount = GetUnitItemHealAmount(
		unit_act,
		GetItemFromSlot(unit_act, gActionData.itemSlotIndex)
	);

/* Having crashing issues with the switch case for making this work on staves only */
#if defined(SID_WhiteMage) && (COMMON_SKILL_VALID(SID_WhiteMage))
    if (SkillTester(unit_act, SID_WhiteMage))
    {
        amount += amount / 2;
        // switch (unit_act->items[gActionData.itemSlotIndex])
        // {
        // case ITEM_STAFF_HEAL:
        // case ITEM_STAFF_MEND:
        // case ITEM_STAFF_PHYSIC:
        // case ITEM_STAFF_FORTIFY:
        //     amount += amount / 2;
        //     break;
        // }
    }
#endif

	targetCount = GetSelectTargetCount();

	for (i = 0; i < targetCount; i++) {
#if CHAX
		struct Unit *unit_tar = GetUnit(GetTarget(i)->uid);
		int amound_real = HealAmountGetter(amount, unit_act, unit_tar);

		AddUnitHp(unit_tar, amound_real);
#else
		AddUnitHp(GetUnit(GetTarget(i)->uid), amount);
#endif
	}

	BattleApplyItemEffect(proc);
	BeginBattleAnimations();
}

LYN_REPLACE_CHECK(ExecStatusStaff);
void ExecStatusStaff(ProcPtr proc) {
    int accuracy = 0;

    BattleInitItemEffect(GetUnit(gActionData.subjectIndex),
        gActionData.itemSlotIndex);

    BattleInitItemEffectTarget(GetUnit(gActionData.targetIndex));

#if defined(SID_NinthCircle) && (COMMON_SKILL_VALID(SID_NinthCircle))
    if (SkillTester(gActiveUnit, SID_NinthCircle))
    {
        MakeTargetListForRangedStatusStaves(gActiveUnit);
        int targetCount = GetSelectTargetCount();

        for (int i = 0; i < targetCount; i++)
        {
            struct Unit* target = GetUnit(GetTarget(i)->uid);

        #ifdef CONFIG_CUSTOM_STAFF_ACCURACY
            accuracy = CONFIG_CUSTOM_STAFF_ACCURACY;
        #else
            accuracy = GetOffensiveStaffAccuracy(
                GetUnit(gActionData.subjectIndex),
                GetUnit(gActionData.targetIndex)
            );
        #endif
            gBattleActor.battleEffectiveHitRate = accuracy;

            if (Roll1RN(accuracy)) {
                switch (GetItemIndex(gBattleActor.weaponBefore)) {
                case ITEM_STAFF_BERSERK:
                    SetUnitStatus(target, UNIT_STATUS_BERSERK);
                    break;
                case ITEM_STAFF_SILENCE:
                    SetUnitStatus(target, UNIT_STATUS_SILENCED);
                    break;
                case ITEM_STAFF_SLEEP:
                    SetUnitStatus(target, UNIT_STATUS_SLEEP);
                    break;
                }
            }
        }
    }
    else
    {
        if (!Roll1RN(accuracy)) {
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_MISS;
        } else {
            switch (GetItemIndex(gBattleActor.weaponBefore)) {
                case ITEM_STAFF_BERSERK:
                    gBattleTarget.statusOut = UNIT_STATUS_BERSERK;
                    break;
                case ITEM_STAFF_SILENCE:
                    gBattleTarget.statusOut = UNIT_STATUS_SILENCED;
                    break;
                case ITEM_STAFF_SLEEP:
                    gBattleTarget.statusOut = UNIT_STATUS_SLEEP;
                    break;
                case ITEM_MONSTER_STONE:
                    switch (gPlaySt.faction) {
                        case FACTION_BLUE:
                            if (UNIT_FACTION(&gBattleTarget.unit) == FACTION_BLUE) {
                                gBattleTarget.statusOut = UNIT_STATUS_13;
                            } else {
                                gBattleTarget.statusOut = UNIT_STATUS_PETRIFY;
                            }
                            break;
                        case FACTION_RED:
                            if (UNIT_FACTION(&gBattleTarget.unit) == FACTION_RED) {
                                gBattleTarget.statusOut = UNIT_STATUS_13;
                            } else {
                                gBattleTarget.statusOut = UNIT_STATUS_PETRIFY;
                            }
                            break;
                        case FACTION_GREEN:
                            if (UNIT_FACTION(&gBattleTarget.unit) == FACTION_GREEN) {
                                gBattleTarget.statusOut = UNIT_STATUS_13;
                            } else {
                                gBattleTarget.statusOut = UNIT_STATUS_PETRIFY;
                            }
                            break;
                    }
                    break;
            }
        }
    }
#else
    if (!Roll1RN(accuracy)) {
        gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_MISS;
    } else {
        switch (GetItemIndex(gBattleActor.weaponBefore)) {
            case ITEM_STAFF_BERSERK:
                gBattleTarget.statusOut = UNIT_STATUS_BERSERK;
                break;
            case ITEM_STAFF_SILENCE:
                gBattleTarget.statusOut = UNIT_STATUS_SILENCED;
                break;
            case ITEM_STAFF_SLEEP:
                gBattleTarget.statusOut = UNIT_STATUS_SLEEP;
                break;
            case ITEM_MONSTER_STONE:
                switch (gPlaySt.faction) {
                    case FACTION_BLUE:
                        if (UNIT_FACTION(&gBattleTarget.unit) == FACTION_BLUE) {
                            gBattleTarget.statusOut = UNIT_STATUS_13;
                        } else {
                            gBattleTarget.statusOut = UNIT_STATUS_PETRIFY;
                        }
                        break;
                    case FACTION_RED:
                        if (UNIT_FACTION(&gBattleTarget.unit) == FACTION_RED) {
                            gBattleTarget.statusOut = UNIT_STATUS_13;
                        } else {
                            gBattleTarget.statusOut = UNIT_STATUS_PETRIFY;
                        }
                        break;
                    case FACTION_GREEN:
                        if (UNIT_FACTION(&gBattleTarget.unit) == FACTION_GREEN) {
                            gBattleTarget.statusOut = UNIT_STATUS_13;
                        } else {
                            gBattleTarget.statusOut = UNIT_STATUS_PETRIFY;
                        }
                        break;
                }
                break;
        }
    }
#endif
    BattleApplyItemEffect(proc);
    BeginBattleAnimations();

    return;
}