#include "common-chax.h"
#include "item-sys.h"
#include "strmag.h"
#include "status-getter.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "debuff.h"
#include "jester_headers/custom-functions.h"

#define LOCAL_TRACE 0

STATIC_DECLAR bool IER_CheckIList(int iid, const u8 *list)
{
	int i;

	for (i = 0; list[i] != ITEM_NONE; i++)
		if (iid == list[i])
			return true;

	return false;
}

/**
 * Desc
 */
// LYN__REPLACE_CHECK(GetItemCantUseMsgid);

/**
 * Promotion list
 */
LYN_REPLACE_CHECK(CanUnitUsePromotionItem);
bool CanUnitUsePromotionItem(struct Unit *unit, int item)
{
	int iid = ITEM_INDEX(item);
	const struct IER_PromoConfig *it = *pr_gpIER_PromotionItemTable;

	for (;; it++) {
		if (it->item == ITEM_NONE || it->item == 0xFFFF)
			break;

		if (it->job_list == NULL)
			break;

		LTRACEF("item=0x%02X 0x%02X, job=0x%02X", it->item, iid, it->job_list[0]);

		if (it->item == iid) {
			int i;
			int jid = UNIT_CLASS_ID(unit);

			for (i = 0; ; i++) {
				int it_jid = it->job_list[i];

				if (it_jid == CLASS_NONE)
					return false;

				if (jid == it_jid) {
					if (it->extra_check)
						return it->extra_check(unit, item);

					return true;
				}
			}
			return false;
		}
	}
	return false;
}

// PlayerPhase_PrepareAction

/**
 * Heal
 */
LYN_REPLACE_CHECK(GetUnitItemHealAmount);
int GetUnitItemHealAmount(struct Unit *unit, int item)
{
	int result = 0;

#ifdef CONFIG_IER_EN
	result = GetItemMight(item) + IER_BYTE(item);

	if (result == 0) {
		switch (GetItemData(ITEM_INDEX(item))->useEffectId) {
		case IER_STAFF_HEAL:
		case IER_STAFF_PHYSIC:
		case IER_STAFF_FORTIFY:
		case IER_VULNERARY:
		case IER_VULNERARY_2:
			result = 10;
			break;

		case IER_STAFF_MEND:
			result = 20;
			break;

		case IER_STAFF_RECOVER:
		case IER_ELIXIR:
			result = 80;
			break;

		default:
			break;
		}
	}
#else
	switch (GetItemIndex(item)) {
	case ITEM_STAFF_HEAL:
	case ITEM_STAFF_PHYSIC:
	case ITEM_STAFF_FORTIFY:
	case ITEM_VULNERARY:
	case ITEM_VULNERARY_2:
		result = 10;
		break;

	case ITEM_STAFF_MEND:
		result = 20;
		break;

	case ITEM_STAFF_RECOVER:
	case ITEM_ELIXIR:
		result = 80;
		break;
	} // switch (GetItemIndex(item))
#endif // IER_EN

	if (GetItemAttributes(item) & IA_STAFF)
		result += MagGetter(unit);

	if (result > 80)
		result = 80;

	return result;
}

/**
 * Key
 */
LYN_REPLACE_CHECK(AiGetChestUnlockItemSlot);
bool AiGetChestUnlockItemSlot(u8 *out)
{
	int i;

	*out = 0;

	if (GetUnitItemCount(gActiveUnit) == UNIT_ITEM_COUNT) {
		gActiveUnit->aiFlags |= AI_UNIT_FLAG_3;
		return false;
	}

	for (i = 0; i < UNIT_ITEM_COUNT; i++) {
		u16 item = gActiveUnit->items[i];

		if (item == 0)
			return false;

		*out = i;

#if CHAX
		switch (GetItemData(ITEM_INDEX(item))->useEffectId) {
		case IER_LOCKPICK:
			if (UNIT_CATTRIBUTES(gActiveUnit) & CA_STEAL)
				return true;

			break;

		case IER_CHESTKEY:
		case IER_DOORKEY:
			return true;

		default:
			break;
		}
#else
		if (GetItemIndex(item) == ITEM_CHESTKEY)
			return true;

		if (GetItemIndex(item) == ITEM_LOCKPICK) {
			if (UNIT_CATTRIBUTES(gActiveUnit) & CA_STEAL)
				return true;
		}
#endif
	}

	return false;
}

LYN_REPLACE_CHECK(AiTryHealSelf);
bool AiTryHealSelf(void)
{
	int i;

	for (i = 0; i < UNIT_ITEM_COUNT; i++) {
		u16 item = gActiveUnit->items[i];

		if (item == 0)
			return 0;

		switch (GetItemData(ITEM_INDEX(item))->useEffectId) {
		case IER_VULNERARY:
		case IER_ELIXIR:
			if (!(gAiState.flags & AI_FLAG_STAY) && !(gActiveUnit->ai_config & AI_UNIT_CONFIG_FLAG_STAY)) {
				/**
				 * If unit can move around (rather than stick on position)
				 * he may try escape to a safe place then heal itself.
				 */
				struct Vec2 position;

				if (AiFindSafestReachableLocation(gActiveUnit, &position) == true) {
					AiSetDecision(position.x, position.y, AI_ACTION_USEITEM, 0, i, 0, 0);
					return true;
				}
			} else {
				AiSetDecision(gActiveUnit->xPos, gActiveUnit->yPos, AI_ACTION_USEITEM, 0, i, 0, 0);
				return true;
			}
			break;

		default:
			break;
		}
	}
	return false;
}

STATIC_DECLAR int GetStatBoosterText(struct Unit *unit, int item)
{
	int iid = ITEM_INDEX(item);
	const struct IER_PrepStatBoosterMsg *list = *pr_IER_StatBoosterTextTable;

	for (; list->item != ITEM_NONE; list++) {
		if (list->item != iid)
			continue;

		if (list->msg_getter)
			return list->msg_getter(unit, item);
		else
			return list->msg;
	}
	return 0;
}

LYN_REPLACE_CHECK(ApplyStatBoostItem);
int ApplyStatBoostItem(struct Unit *unit, int slot)
{
	int item = unit->items[slot];
	FORCE_DECLARE const struct ItemData *iinfo = GetItemData(ITEM_INDEX(item));
	const struct ItemStatBonuses *statBonuses = GetItemStatBonuses(item);
	int msg = GetStatBoosterText(unit, item);

#ifdef CONFIG_IER_EN
	if (iinfo->useEffectId == IER_METISSTOME) {
		unit->state |= US_GROWTH_BOOST;
		UnitUpdateUsedItem(unit, slot);
		return msg;
	}
#else
	if (GetItemIndex(item) == ITEM_METISSTOME) {
		unit->state |= US_GROWTH_BOOST;
		UnitUpdateUsedItem(unit, slot);
		return 0x1D; /* Maturity increased */
	}
#endif

#if (defined(SID_ShrewdPotential) && COMMON_SKILL_VALID(SID_ShrewdPotential))
	if (SkillTester(unit, SID_ShrewdPotential)) {
		if (statBonuses->hpBonus > 0) {
			unit->maxHP += SKILL_EFF0(SID_ShrewdPotential);
			unit->curHP += SKILL_EFF0(SID_ShrewdPotential);
		}
		if (statBonuses->powBonus > 0)
			unit->pow += SKILL_EFF0(SID_ShrewdPotential);
		if (ITEM_MAG_BONUS(statBonuses) > 0)
			UNIT_MAG(unit) += SKILL_EFF0(SID_ShrewdPotential);
		if (statBonuses->sklBonus > 0)
			unit->skl += SKILL_EFF0(SID_ShrewdPotential);
		if (statBonuses->spdBonus > 0)
			unit->spd += SKILL_EFF0(SID_ShrewdPotential);
		if (statBonuses->lckBonus > 0)
			unit->lck += SKILL_EFF0(SID_ShrewdPotential);
		if (statBonuses->defBonus > 0)
			unit->def += SKILL_EFF0(SID_ShrewdPotential);
		if (statBonuses->resBonus > 0)
			unit->res += SKILL_EFF0(SID_ShrewdPotential);
		if (statBonuses->conBonus > 0)
			unit->conBonus += SKILL_EFF0(SID_ShrewdPotential);
		if (statBonuses->movBonus > 0)
			unit->movBonus += SKILL_EFF0(SID_ShrewdPotential);
	}
#endif

	unit->maxHP += statBonuses->hpBonus;
	unit->curHP += statBonuses->hpBonus;
	unit->pow += statBonuses->powBonus;
	unit->skl += statBonuses->sklBonus;
	unit->spd += statBonuses->spdBonus;
	unit->def += statBonuses->defBonus;
	unit->res += statBonuses->resBonus;
	unit->lck += statBonuses->lckBonus;
	unit->movBonus += statBonuses->movBonus;
	unit->conBonus += statBonuses->conBonus;

	UNIT_MAG(unit) += ITEM_MAG_BONUS(statBonuses);

	UnitCheckStatCaps(unit);
	UnitUpdateUsedItem(unit, slot);

#ifdef CONFIG_IER_EN
	return msg;
#else
	if (statBonuses->hpBonus > 0)
		return 0x1C;
	else if (statBonuses->powBonus > 0)
		return 0x13;
	else if (ITEM_MAG_BONUS(statBonuses) > 0)
		return 0x14;
	else if (statBonuses->sklBonus > 0)
		return 0x15;
	else if (statBonuses->spdBonus > 0)
		return 0x16;
	else if (statBonuses->lckBonus > 0)
		return 0x17;
	else if (statBonuses->defBonus > 0)
		return 0x18;
	else if (statBonuses->resBonus > 0)
		return 0x19;
	else if (statBonuses->movBonus > 0)
		return 0x1A;
	else if (statBonuses->conBonus > 0)
		return 0x1B;

	return 0;
#endif
}

LYN_REPLACE_CHECK(DoUseBarrierStaff);
void DoUseBarrierStaff(struct Unit* unit)
{
    MakeTargetListForBarrier(unit);

    char * str = "NULL";

    int itemId = GetItemIndex(unit->items[0]);

    switch (itemId)
    {
#ifdef CONFIG_ITEM_INDEX_FORCE_STAFF
    case CONFIG_ITEM_INDEX_FORCE_STAFF:
        str = "Select which character's strength to bolster";
        break;
#endif
#ifdef CONFIG_ITEM_INDEX_TEMPEST_STAFF
    case CONFIG_ITEM_INDEX_TEMPEST_STAFF:
        str = "Select which character's magic to bolster";
        break;
#endif
#ifdef CONFIG_ITEM_INDEX_ACUITY_STAFF
    case CONFIG_ITEM_INDEX_ACUITY_STAFF:
        str = "Select which character's skill to bolster";
        break;
#endif
#ifdef CONFIG_ITEM_INDEX_SPRINT_STAFF
    case CONFIG_ITEM_INDEX_SPRINT_STAFF:
        str = "Select which character's speed to bolster";
        break;
#endif
#ifdef CONFIG_ITEM_INDEX_FORTUNE_STAFF
    case CONFIG_ITEM_INDEX_FORTUNE_STAFF:
        str = "Select which character's luck to bolster";
        break;
#endif
#ifdef CONFIG_ITEM_INDEX_IRON_STAFF
    case CONFIG_ITEM_INDEX_IRON_STAFF:
        str = "Select which character's defense to bolster";
        break;
#endif
    case ITEM_STAFF_BARRIER:
        str = "Select which character's resistance to bolster";
        break;

    default:
        break;
    }

    BmMapFill(gBmMapMovement, -1);

    StartSubtitleHelp(NewTargetSelection(&gSelectInfo_Barrier), str);
}

LYN_REPLACE_CHECK(ExecBarrierStaff);
void ExecBarrierStaff(ProcPtr proc) {

    struct Unit *unit_act = GetUnit(gActionData.subjectIndex);
    struct Unit *unit_tar = GetUnit(gActionData.targetIndex);

    BattleInitItemEffect(unit_act, gActionData.itemSlotIndex);

    BattleInitItemEffectTarget(unit_tar);

    int itemId = GetItemIndex(unit_act->items[0]);

    switch (itemId)
    {
#ifdef CONFIG_ITEM_INDEX_FORCE_STAFF
    case CONFIG_ITEM_INDEX_FORCE_STAFF:
        unit_tar->boostType = 0;
        break;
#endif
#ifdef CONFIG_ITEM_INDEX_TEMPEST_STAFF
    case CONFIG_ITEM_INDEX_TEMPEST_STAFF:
        unit_tar->boostType = 1;
        break;
#endif
#ifdef CONFIG_ITEM_INDEX_ACUITY_STAFF
    case CONFIG_ITEM_INDEX_ACUITY_STAFF:
        unit_tar->boostType = 2;
        break;
#endif
#ifdef CONFIG_ITEM_INDEX_SPRINT_STAFF
    case CONFIG_ITEM_INDEX_SPRINT_STAFF:
        unit_tar->boostType = 3;
        break;
#endif
#ifdef CONFIG_ITEM_INDEX_FORTUNE_STAFF
    case CONFIG_ITEM_INDEX_FORTUNE_STAFF:
        unit_tar->boostType = 4;
        break;
#endif
#ifdef CONFIG_ITEM_INDEX_IRON_STAFF
    case CONFIG_ITEM_INDEX_IRON_STAFF:
        unit_tar->boostType = 5;
        break;
#endif
    case ITEM_STAFF_BARRIER:
        unit_tar->boostType = 6;
        break;
    
    default:
        break;
    }

    unit_tar->barrierDuration = 7;

#if defined(SID_ExplosiveBuff) && (COMMON_SKILL_VALID(SID_ExplosiveBuff))
    if (SkillTester(unit_act, SID_ExplosiveBuff))
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
                unit_adjacent->barrierDuration = 7;
        }
    }
#endif

    BattleApplyItemEffect(proc);
    BeginBattleAnimations();

    return;
}


LYN_REPLACE_CHECK(ExecMine);
void ExecMine(ProcPtr proc) {
    BattleInitItemEffect(GetUnit(gActionData.subjectIndex),
        gActionData.itemSlotIndex);

    AddTrap(gActionData.xOther, gActionData.yOther, TRAP_MINE, 0);

    StartMineAnim(proc, gActionData.xOther, gActionData.yOther);

    BattleApplyItemEffect(proc);
    BeginBattleAnimations(); // I need this for the EXP bar but it's causing a softlock on the prologue, still grants exp and level ups though

    gBattleTarget.statusOut = -1;

    return;
}

LYN_REPLACE_CHECK(ExecLightRune);
void ExecLightRune(ProcPtr proc) {
    BattleInitItemEffect(GetUnit(gActionData.subjectIndex),
        gActionData.itemSlotIndex);

    AddLightRune(gActionData.xOther, gActionData.yOther);

    BattleApplyItemEffect(proc);

#ifdef CONFIG_ITEM_INDEX_RUNE_STAFF
    int x = gActionData.xOther * 0x10 - gBmSt.camera.x - 0x18;
    int y = gActionData.yOther * 0x10 - gBmSt.camera.y - 0x28;

    BG_SetPosition(0, -x, -y);
    BeginBattleAnimations(); // This way we can gain EXP for using a light rune staff
#else
    StartLightRuneAnim(proc, gActionData.xOther, gActionData.yOther);
#endif

    gBattleTarget.statusOut = -1;

    return;
}

void ExecCustomStaves(ProcPtr proc) {
    struct Unit * unit_act = GetUnit(gActionData.subjectIndex);
    struct Unit * unit_tar = GetUnit(gActionData.targetIndex);

    BattleInitItemEffect(unit_act, gActionData.itemSlotIndex);

    BattleInitItemEffectTarget(unit_tar);

    BattleApplyItemEffect(proc);

    int itemId = GetItemIndex(unit_act->items[0]);

    switch (itemId)
    {   
#ifdef CONFIG_ITEM_INDEX_SLOW_STAFF
    case CONFIG_ITEM_INDEX_SLOW_STAFF:
        SetUnitStatus(unit_tar, NEW_UNIT_STATUS_SLOW);
        break;
#endif
#ifdef CONFIG_ITEM_INDEX_FORGE_STAFF
    case CONFIG_ITEM_INDEX_FORGE_STAFF:
        SetUnitStatus(unit_tar, NEW_UNIT_STATUS_SLOW);
        break;
#endif
    default:
        break;
    }

    BeginBattleAnimations();
    
    return;
}