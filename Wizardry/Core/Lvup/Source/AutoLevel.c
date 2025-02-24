#include "common-chax.h"
#include "strmag.h"
#include "lvup.h"

LYN_REPLACE_CHECK(UnitAutolevelCore);
void UnitAutolevelCore(struct Unit *unit, u8 classId, int levelCount)
{
	if (levelCount) {
<<<<<<< HEAD
		unit->maxHP += GetAutoleveledStatIncrease(GetUnitJobBasedHpGrowth(unit),  levelCount);
		unit->pow   += GetAutoleveledStatIncrease(GetUnitJobBasedPowGrowth(unit), levelCount);
		unit->skl   += GetAutoleveledStatIncrease(GetUnitJobBasedSklGrowth(unit), levelCount);
		unit->spd   += GetAutoleveledStatIncrease(GetUnitJobBasedSpdGrowth(unit), levelCount);
		unit->def   += GetAutoleveledStatIncrease(GetUnitJobBasedDefGrowth(unit), levelCount);
		unit->res   += GetAutoleveledStatIncrease(GetUnitJobBasedResGrowth(unit), levelCount);
		unit->lck   += GetAutoleveledStatIncrease(GetUnitJobBasedLckGrowth(unit), levelCount);

		/* Hook here */
		UNIT_MAG(unit) += GetAutoleveledStatIncrease(GetUnitJobBasedMagGrowth(unit), levelCount);
=======
		unit->maxHP += GetAutoleveledStatIncrease(GetUnitHpGrowth(unit),  levelCount);
		unit->pow   += GetAutoleveledStatIncrease(GetUnitPowGrowth(unit), levelCount);
		unit->skl   += GetAutoleveledStatIncrease(GetUnitSklGrowth(unit), levelCount);
		unit->spd   += GetAutoleveledStatIncrease(GetUnitSpdGrowth(unit), levelCount);
		unit->def   += GetAutoleveledStatIncrease(GetUnitDefGrowth(unit), levelCount);
		unit->res   += GetAutoleveledStatIncrease(GetUnitResGrowth(unit), levelCount);
		unit->lck   += GetAutoleveledStatIncrease(GetUnitLckGrowth(unit), levelCount);

		/* Hook here */
		UNIT_MAG(unit) += GetAutoleveledStatIncrease(GetUnitMagGrowth(unit), levelCount);
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc
	}
}

LYN_REPLACE_CHECK(UnitAutolevelPenalty);
void UnitAutolevelPenalty(struct Unit *unit, u8 classId, int levelCount)
{
	int level = unit->level;

	if (levelCount && level > unit->pCharacterData->baseLevel) {
		levelCount = level - levelCount;

		unit->maxHP = unit->pCharacterData->baseHP  + unit->pClassData->baseHP;
		unit->pow   = unit->pCharacterData->basePow + unit->pClassData->basePow;
		unit->skl   = unit->pCharacterData->baseSkl + unit->pClassData->baseSkl;
		unit->spd   = unit->pCharacterData->baseSpd + unit->pClassData->baseSpd;
		unit->def   = unit->pCharacterData->baseDef + unit->pClassData->baseDef;
		unit->res   = unit->pCharacterData->baseRes + unit->pClassData->baseRes;
		unit->lck   = unit->pCharacterData->baseLck;

		/* Hook here */
		UNIT_MAG(unit) = GetUnitBaseMagic(unit);

		if (levelCount > unit->pCharacterData->baseLevel) {
			unit->level = levelCount;
			UnitAutolevel(unit);
			unit->level = level;
		}
	}
<<<<<<< HEAD
}
=======
}
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc
