#include "common-chax.h"
#include "jester_headers/custom-arrays.h"

//! FE8U = 0x0801C928
LYN_REPLACE_CHECK(CanShowUnitStatScreen);
bool CanShowUnitStatScreen(struct Unit * unit)
{
#ifdef CONFIG_DENY_STAT_SCREEN
	for (int i = 0; i < sizeOfDenyClasses; i++)
	{
		if (UNIT_CLASS_ID(unit) == statScreenDenyClasses[i])
			return false;
	}
#else
    if (UNIT_IS_GORGON_EGG(unit))
    {
        return false;
    }
#endif

    return true;
}

LYN_REPLACE_CHECK(FindNextUnit);
struct Unit* FindNextUnit(struct Unit* u, int direction)
{
    int faction = UNIT_FACTION(u);
    int i       = u->index;

    struct Unit* unit;

    while (TRUE)
    {
        i = (i + direction) & 0x3F;
        unit = GetUnit(faction + i);

        if (!UNIT_IS_VALID(unit))
            continue;

        if ((sStatScreenInfo.config & STATSCREEN_CONFIG_NONDEAD) && (unit->state & US_DEAD))
            continue;

        if ((sStatScreenInfo.config & STATSCREEN_CONFIG_NONBENCHED) && (unit->state & US_NOT_DEPLOYED))
            continue;

        if ((sStatScreenInfo.config & STATSCREEN_CONFIG_NONUNK9) && (unit->state & US_BIT9))
            continue;

        if ((sStatScreenInfo.config & STATSCREEN_CONFIG_NONROOFED) && (unit->state & US_UNDER_A_ROOF))
            continue;

        if ((sStatScreenInfo.config & STATSCREEN_CONFIG_NONUNK16) && (unit->state & US_BIT16))
            continue;

        if ((sStatScreenInfo.config & STATSCREEN_CONFIG_NONSUPPLY) && (UNIT_CATTRIBUTES(unit) & CA_SUPPLY))
            continue;

#ifdef CONFIG_DENY_STAT_SCREEN
        for (int i = 0; i < sizeOfDenyClasses; i++)
        {
            if (UNIT_CLASS_ID(unit) == statScreenDenyClasses[i])
                return u;
        }
#else
        if (UNIT_IS_GORGON_EGG(unit))
            continue;
#endif

        return unit;
    }
}