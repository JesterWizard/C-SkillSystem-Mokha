#include "common-chax.h"
#include "weapon-range.h"
#include "debuff.h"
#include "strmag.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "jester_headers/custom-functions.h"

LYN_REPLACE_CHECK(TryAddUnitToHealTargetList);
void TryAddUnitToHealTargetList(struct Unit* unit) {

    if (!AreUnitsAllied(gSubjectUnit->index, unit->index)) {

#if defined(SID_Saint) && (COMMON_SKILL_VALID(SID_Saint))
        if (!SkillTester(gSubjectUnit, SID_Saint))
             return;
#else 
        return;
#endif

    }

#if defined(SID_AidRefusal) && (COMMON_SKILL_VALID(SID_AidRefusal))
    if (SkillTester(unit, SID_AidRefusal))
        return;
#endif

    if (unit->state & US_RESCUED) {
        return;
    }

#if defined(SID_CursedHeal) && (COMMON_SKILL_VALID(SID_CursedHeal))
    if (!SkillTester(gSubjectUnit, SID_CursedHeal))
        if (GetUnitCurrentHp(unit) == GetUnitMaxHp(unit)) {
            return;
        }  
#else 
    if (GetUnitCurrentHp(unit) == GetUnitMaxHp(unit)) {
        return;
    }
#endif

    AddTarget(unit->xPos, unit->yPos, unit->index, 0);

    return;
}

LYN_REPLACE_CHECK(MakeTargetListForAdjacentHeal);
void MakeTargetListForAdjacentHeal(struct Unit *unit)
{
	gSubjectUnit = unit;
	InitTargets(unit->xPos, unit->yPos);

	BmMapFill(gBmMapRange, 0);
	AddMapForItem(unit, ITEM_STAFF_HEAL);
	ForEachUnit(TryAddUnitToHealTargetList, gBmMapRange, 0);

#if defined(SID_SelfHealing) && (COMMON_SKILL_VALID(SID_SelfHealing))
    if (SkillTester(unit, SID_SelfHealing))
         AddTarget(unit->xPos, unit->yPos, unit->index, 0);
#endif
}

LYN_REPLACE_CHECK(MakeTargetListForRangedHeal);
void MakeTargetListForRangedHeal(struct Unit *unit)
{
	gSubjectUnit = unit;
	InitTargets(unit->xPos, unit->yPos);

	BmMapFill(gBmMapRange, 0);
	AddMapForItem(unit, ITEM_STAFF_PHYSIC);
	ForEachUnit(TryAddUnitToHealTargetList, gBmMapRange, 0);

#if defined(SID_SelfHealing) && (COMMON_SKILL_VALID(SID_SelfHealing))
    if (SkillTester(unit, SID_SelfHealing))
         AddTarget(unit->xPos, unit->yPos, unit->index, 0);
#endif
}

LYN_REPLACE_CHECK(MakeTargetListForRestore);
void MakeTargetListForRestore(struct Unit *unit)
{
	gSubjectUnit = unit;
	InitTargets(unit->xPos, unit->yPos);

	BmMapFill(gBmMapRange, 0);
	AddMapForItem(unit, ITEM_STAFF_RESTORE);
	ForEachUnit(TryAddUnitToRestoreTargetList, gBmMapRange, 0);
}

LYN_REPLACE_CHECK(MakeTargetListForBarrier);
void MakeTargetListForBarrier(struct Unit *unit)
{
	gSubjectUnit = unit;
	InitTargets(unit->xPos, unit->yPos);

	BmMapFill(gBmMapRange, 0);
	AddMapForItem(unit, ITEM_STAFF_BARRIER);
	ForEachUnit(TryAddUnitToBarrierTargetList, gBmMapRange, 0);
}

LYN_REPLACE_CHECK(MakeTargetListForRescueStaff);
void MakeTargetListForRescueStaff(struct Unit *unit)
{
	gSubjectUnit = unit;
	InitTargets(unit->xPos, unit->yPos);

	BmMapFill(gBmMapRange, 0);
	AddMapForItem(unit, ITEM_STAFF_RESCUE);
	ForEachUnit(TryAddUnitToRescueStaffTargetList, gBmMapRange, 0);
}

LYN_REPLACE_CHECK(MakeTargetListForSilence);
void MakeTargetListForSilence(struct Unit *unit)
{
	gSubjectUnit = unit;
	InitTargets(unit->xPos, unit->yPos);

	BmMapFill(gBmMapRange, 0);
	AddMapForItem(unit, ITEM_STAFF_SILENCE);
	ForEachUnit(TryAddUnitToSilenceTargetList, gBmMapRange, 0);
}

LYN_REPLACE_CHECK(MakeTargetListForSleep);
void MakeTargetListForSleep(struct Unit *unit)
{
	gSubjectUnit = unit;
	InitTargets(unit->xPos, unit->yPos);

	BmMapFill(gBmMapRange, 0);
	AddMapForItem(unit, ITEM_STAFF_SLEEP);
	ForEachUnit(TryAddUnitToSleepTargetList, gBmMapRange, 0);
}

LYN_REPLACE_CHECK(MakeTargetListForBerserk);
void MakeTargetListForBerserk(struct Unit *unit)
{
	gSubjectUnit = unit;
	InitTargets(unit->xPos, unit->yPos);

	BmMapFill(gBmMapRange, 0);
	AddMapForItem(unit, ITEM_STAFF_BERSERK);
	ForEachUnit(TryAddUnitToBerserkTargetList, gBmMapRange, 0);
}

LYN_REPLACE_CHECK(MakeTargetListForWarp);
void MakeTargetListForWarp(struct Unit *unit)
{
	gSubjectUnit = unit;
	InitTargets(unit->xPos, unit->yPos);

	BmMapFill(gBmMapRange, 0);
	AddMapForItem(unit, ITEM_STAFF_WARP);
	ForEachUnit(TryAddUnitToWarpTargetList, gBmMapRange, 0);
}

LYN_REPLACE_CHECK(MakeTargetListForHammerne);
void MakeTargetListForHammerne(struct Unit *unit)
{
	gSubjectUnit = unit;
	InitTargets(unit->xPos, unit->yPos);

	BmMapFill(gBmMapRange, 0);
	AddMapForItem(unit, ITEM_STAFF_REPAIR);
	ForEachUnit(TryAddUnitToHammerneTargetList, gBmMapRange, 0);
}

void TryAddUnitToSlowTargetList(struct Unit* unit) 
{
    if (AreUnitsAllied(gSubjectUnit->index, unit->index)) {
        return;
    }

    if (unit->statusIndex != UNIT_STATUS_NONE) {
        return;
    }

    AddTarget(unit->xPos, unit->yPos, unit->index, 0);

    return;
}

void MakeTargetListForSlow(struct Unit *unit)
{
	int x = unit->xPos;
    int y = unit->yPos;
	gSubjectUnit = unit;
	InitTargets(x, y);

	BmMapFill(gBmMapRange, 0);
#ifdef CONFIG_ITEM_INDEX_SLOW_STAFF
	AddMapForItem(unit, CONFIG_ITEM_INDEX_SLOW_STAFF);
#endif
	ForEachAdjacentUnit(x, y, TryAddUnitToSlowTargetList);
}

void TryAddUnitToForgeTargetList(struct Unit* unit) 
{	
    if (AreUnitsAllied(gSubjectUnit->index, unit->index)) {
        return;
    }

	if (gSubjectUnit->items[4] != 0) {
		return;
	}

    if (unit->res >= GetUnitMagic(gSubjectUnit)) {
        return;
    }

	for (int i = 0; i < UNIT_ITEM_COUNT; i++)
	{
		if (unit->items[i] == 0)
			return;
		
		if (GetItemAttributes(unit->items[i]) == IA_WEAPON)
			break;
	}

    AddTarget(unit->xPos, unit->yPos, unit->index, 0);

    return;
}

void MakeTargetListForForge(struct Unit *unit)
{
	int x = unit->xPos;
    int y = unit->yPos;
	gSubjectUnit = unit;
	InitTargets(x, y);

	BmMapFill(gBmMapRange, 0);
#ifdef CONFIG_ITEM_INDEX_FORGE_STAFF
	AddMapForItem(unit, CONFIG_ITEM_INDEX_FORGE_STAFF);
#endif
	ForEachUnitInMagBy2Range(TryAddUnitToForgeTargetList);
}