#include "common-chax.h"
#include "status-getter.h"
#include "skill-system.h"
#include "battle-system.h"
#include "item-sys.h"
#include "constants/skills.h"

<<<<<<< HEAD
typedef int (*HealAmountGetterFunc_t)(int old, struct Unit *actor, struct Unit *target);
extern HealAmountGetterFunc_t const *const gpHealAmountGetters;

#if (defined(SID_WeaponHeal) && (COMMON_SKILL_VALID(SID_WeaponHeal)))
static int find_item_slot(struct Unit *unit, int item)
{
	int i;
	int iid = ITEM_INDEX(item);
	for (i = 0; i < UNIT_ITEM_COUNT; i++)
		if (ITEM_INDEX(unit->items[i]) == iid)
			return i;
	return -1;
}
#endif

static int HealAmountGetter(int base, struct Unit *actor, struct Unit *target)
{
    const HealAmountGetterFunc_t *it;
    int status = base;
=======
static int HealAmountGetter(int base, struct Unit *actor, struct Unit *target)
{
	const HealAmountGetterFunc_t *it;
	int status = base;
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc

	for (it = gpHealAmountGetters; *it; it++)
		status = (*it)(status, actor, target);

	LIMIT_AREA(status, 0, 80);
	return status;
}

LYN_REPLACE_CHECK(GetUnitItemHealAmount);
int GetUnitItemHealAmount(struct Unit *unit, int item)
{
	int result = 0;

<<<<<<< HEAD
    switch (GetItemIndex(item))
    {
    case ITEM_STAFF_HEAL:
    case ITEM_STAFF_PHYSIC:
    case ITEM_STAFF_FORTIFY:
    case ITEM_VULNERARY:
    case ITEM_VULNERARY_2:
        result = 10;
        break;
=======
	switch (GetItemIndex(item)) {
	case ITEM_STAFF_HEAL:
	case ITEM_STAFF_PHYSIC:
	case ITEM_STAFF_FORTIFY:
	case ITEM_VULNERARY:
	case ITEM_VULNERARY_2:
		result = 10;
		break;
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc

	case ITEM_STAFF_MEND:
		result = 20;
		break;

	case ITEM_STAFF_RECOVER:
	case ITEM_ELIXIR:
		result = 80;
		break;

	} // switch (GetItemIndex(item))

	if (GetItemAttributes(item) & IA_STAFF)
		result += MagGetter(unit);

	if (result > 80)
		result = 80;

	return result;
}

LYN_REPLACE_CHECK(ExecStandardHeal);
void ExecStandardHeal(ProcPtr proc)
{
	int amount;

<<<<<<< HEAD
    struct Unit *unit_act = GetUnit(gActionData.subjectIndex);
    struct Unit *unit_tar = GetUnit(gActionData.targetIndex);
=======
	struct Unit *unit_act = GetUnit(gActionData.subjectIndex);
	struct Unit *unit_tar = GetUnit(gActionData.targetIndex);
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc

	BattleInitItemEffect(unit_act, gActionData.itemSlotIndex);

<<<<<<< HEAD
    amount = GetUnitItemHealAmount(
        unit_act,
        unit_act->items[gActionData.itemSlotIndex]);
=======
	BattleInitItemEffectTarget(unit_tar);

	amount = GetUnitItemHealAmount(
		unit_act,
		GetItemFromSlot(unit_act, gActionData.itemSlotIndex)
	);
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc

#if CHAX
	amount = HealAmountGetter(amount, unit_act, unit_tar);
#endif

<<<<<<< HEAD
    BattleInitItemEffectTarget(unit_tar);

    AddUnitHp(unit_tar, amount);
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

#if (defined(SID_LiveToServe) && (COMMON_SKILL_VALID(SID_LiveToServe)))
    if (SkillTester(unit_act, SID_LiveToServe))
    {
        //BattleInitItemEffectTarget(unit_act);
        AddUnitHp(unit_act, amount);
        gBattleHitIterator->hpChange = gBattleActor.unit.curHP - GetUnitCurrentHp(unit_act);
        gBattleActor.unit.curHP = GetUnitCurrentHp(unit_act);
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

    BattleApplyItemEffect(proc);
    BeginBattleAnimations();
=======
	AddUnitHp(unit_tar, amount);
	gBattleHitIterator->hpChange = gBattleTarget.unit.curHP - GetUnitCurrentHp(unit_tar);
	gBattleTarget.unit.curHP = GetUnitCurrentHp(unit_tar);

	BattleApplyItemEffect(proc);
	BeginBattleAnimations();
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc
}

LYN_REPLACE_CHECK(ExecFortify);
void ExecFortify(ProcPtr proc)
{
	int i;
	int amount;
	int targetCount;

<<<<<<< HEAD
    struct Unit *unit_act = GetUnit(gActionData.subjectIndex);
    struct Unit *unit_tar = GetUnit(gActionData.targetIndex);

    BattleInitItemEffect(unit_act,
                         gActionData.itemSlotIndex);
=======
	struct Unit *unit_act = GetUnit(gActionData.subjectIndex);
	struct Unit *unit_tar = GetUnit(gActionData.targetIndex);

	BattleInitItemEffect(unit_act, gActionData.itemSlotIndex);
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc

	BattleInitItemEffectTarget(GetUnitFromCharId(GetPlayerLeaderUnitId()));
	MakeTargetListForRangedHeal(unit_act);

<<<<<<< HEAD
    amount = GetUnitItemHealAmount(
        unit_act,
        unit_act->items[gActionData.itemSlotIndex]);
=======
	amount = GetUnitItemHealAmount(
		unit_act,
		GetItemFromSlot(unit_act, gActionData.itemSlotIndex)
	);
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc

	targetCount = GetSelectTargetCount();

	for (i = 0; i < targetCount; i++) {
#if CHAX
		int amound_real = HealAmountGetter(amount, unit_act, unit_tar);

		AddUnitHp(GetUnit(GetTarget(i)->uid), amound_real);
#else
		AddUnitHp(GetUnit(GetTarget(i)->uid), amount);
#endif
	}

	BattleApplyItemEffect(proc);
	BeginBattleAnimations();
}

LYN_REPLACE_CHECK(BattleInitItemEffectTarget);
void BattleInitItemEffectTarget(struct Unit *unit)
{
    InitBattleUnit(&gBattleTarget, unit);

    SetBattleUnitTerrainBonusesAuto(&gBattleTarget);
    ComputeBattleUnitBaseDefense(&gBattleTarget);
    ComputeBattleUnitSupportBonuses(&gBattleTarget, NULL);

    gBattleTarget.battleAttack = 0xFF;
    gBattleTarget.battleEffectiveHitRate = 0xFF;
    gBattleTarget.battleEffectiveCritRate = 0xFF;

    gBattleTarget.weaponBefore = 0;

    BattleUnitTargetSetEquippedWeapon(&gBattleTarget);

    gBattleActor.hasItemEffectTarget = TRUE;
}