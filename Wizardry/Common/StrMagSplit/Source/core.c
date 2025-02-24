#include "common-chax.h"
#include "strmag.h"

bool IsMagicAttack(struct BattleUnit *bu)
{
	return !!((IA_MAGICDAMAGE | IA_MAGIC) & bu->weaponAttributes);
}

int GetUnitMaxMagic(struct Unit *unit)
{
	int status = gpMagicJInfos[UNIT_CLASS_ID(unit)].cap;

	if (status <= 0)
		return 20;

	return status;
}

int GetUnitBaseMagic(struct Unit *unit)
{
	return gpMagicPInfos[UNIT_CHAR_ID(unit)].base + gpMagicJInfos[UNIT_CLASS_ID(unit)].base;
}

int GetUnitBasicMagGrowth(struct Unit *unit)
{
	return gpMagicPInfos[UNIT_CHAR_ID(unit)].growth;
}

<<<<<<< HEAD
int GetUnitJobBasedBasicMagGrowth(struct Unit *unit)
{
	return gpMagicJInfos[UNIT_CHAR_ID(unit)].growth;
}

=======
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc
int GetClassChgMagicBonus(u8 jid)
{
	return gpMagicJInfos[jid].bonus;
}

int GetUnitMagic(struct Unit *unit)
{
	return MagGetter(unit);
<<<<<<< HEAD
}
=======
}
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc
