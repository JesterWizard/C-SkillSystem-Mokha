#include "common-chax.h"
#include "kernel-lib.h"
#include "constants/skills.h"
#include "mu.h"
#include "debuff.h"
#include "bwl.h"
#include "playst-expa.h"
#include "jester_headers/class-pairs.h"

typedef int (*UnitRefrainFunc_t)(struct Unit *unit);
// extern const UnitRefrainFunc_t gUnitRefrainHooks[];
extern UnitRefrainFunc_t const *const gpUnitRefrainHooks;

LYN_REPLACE_CHECK(ResetAllPlayerUnitState);
void ResetAllPlayerUnitState(void)
{
	FOR_UNITS_VALID_FACTION(FACTION_BLUE, unit, {
		SetUnitHp(unit, GetUnitMaxHp(unit));
		SetUnitStatus(unit, US_NONE);
		unit->torchDuration = 0;
		unit->barrierDuration = 0;
		unit->state &=
			US_DEAD | US_NOT_DEPLOYED | US_GROWTH_BOOST |
			US_SOLOANIM_1 | US_SOLOANIM_2 | US_BIT16 |
			US_BIT20 | US_BIT21 | US_BIT22 |
			US_BIT25 | US_BIT26;

		unit->xPos = -1;

		unit->state |= US_HIDDEN;
		unit->rescue = 0;

#if CHAX
		const UnitRefrainFunc_t *it;
		for (it = gpUnitRefrainHooks; *it; it++)
			(*it)(unit);
#endif
	})

	RefreshEntityBmMaps();
	EndAllMus();
}

LYN_REPLACE_CHECK(ChapterChangeUnitCleanup);
void ChapterChangeUnitCleanup(void)
{
	// Clear phantoms
	FOR_UNITS_VALID_FACTION(FACTION_BLUE, unit, {
		if (UNIT_IS_PHANTOM(unit))
			ClearUnit(unit);
	})

	// Clear all non player units (green & red units)
	FOR_UNITS_VALID_ALL(unit, {
		if (UNIT_FACTION(unit) != FACTION_BLUE)
			ClearUnit(unit);
	})

	// Reset player unit "temporary" states (HP, status, some state flags, etc)
	FOR_UNITS_VALID_FACTION(FACTION_BLUE, unit, {
		u32 pid = UNIT_CHAR_ID(unit);
 		struct NewBwl * bwl = GetNewBwl(pid);

		SetUnitHp(unit, GetUnitMaxHp(unit));
		SetUnitStatus(unit, UNIT_STATUS_NONE);

		unit->torchDuration = 0;
		unit->barrierDuration = 0;

		if (unit->state & US_NOT_DEPLOYED)
			unit->state = unit->state | US_BIT21;
		else
			unit->state = unit->state &~ US_BIT21;

		unit->state &= (
			US_DEAD | US_GROWTH_BOOST | US_SOLOANIM_1 | US_SOLOANIM_2 |
			US_BIT16 | US_BIT20 | US_BIT21 | US_BIT25 | US_BIT26
		);

		if (UNIT_CATTRIBUTES(unit) & CA_SUPPLY)
			unit->state = unit->state &~ US_DEAD;

		unit->state |= US_HIDDEN | US_NOT_DEPLOYED;

		unit->rescue = 0;
		unit->supportBits = 0;

#if CHAX
		const UnitRefrainFunc_t *it;
		for (it = gpUnitRefrainHooks; *it; it++)
			(*it)(unit);
#endif

#ifdef CONFIG_LAGUZ_BARS
        for (int i = 0; i < laguzListSize; i++)
        {
            if (unit->pClassData->number == laguzPairs[i][1])
            {
                unit->pClassData = GetClassData(laguzPairs[i][0]);
                ClearUnitStatDebuff(unit, UNIT_STAT_BUFF_LAGUZ);
                ClearUnitStatDebuff(unit, UNIT_STAT_BUFF_LAGUZ_HALFSHIFT);
                break;
            }
        }
#endif

#if defined(CONFIG_AUTO_REPAIR_WEAPONS)
		for (int i = 0; i < 5; i++)
			// if(GetItemIndex(unit->items[i]) == ITEM_SWORD_RAPIER)
			unit->items[i] = MakeNewItem(unit->items[i]);
#endif

#if defined(CONFIG_RESET_BWL_STATS_EACH_CHAPTER)
        if (bwl != NULL)
        {
            bwl->battleAmt = 0;
            bwl->winAmt = 0;
            bwl->lossAmt = 0;
        }
#endif

#ifdef CONFIG_MP_SYSTEM
		if (bwl != NULL)
			bwl->currentMP = 0;
#endif
	})

	gPlaySt.chapterStateBits = gPlaySt.chapterStateBits &~ PLAY_FLAG_PREPSCREEN;
}
