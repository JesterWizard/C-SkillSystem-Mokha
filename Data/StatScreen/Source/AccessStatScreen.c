#include "common-chax.h"
#include "jester_headers/custom-arrays.h"

const int statScreenDenyClasses[] = {
    CLASS_GORGONEGG, 
    CLASS_GORGONEGG2, 
    CLASS_FIGHTER
};

const int sizeOfDenyClasses = sizeof(statScreenDenyClasses) / sizeof(statScreenDenyClasses[0]);

//! FE8U = 0x0801C928
LYN_REPLACE_CHECK(CanShowUnitStatScreen);
bool CanShowUnitStatScreen(struct Unit * unit)
{
#ifdef CONFIG_DENY_STAT_SCREEN
	for (int i = 0; i < sizeOfDenyClasses; i++)
		if (UNIT_CLASS_ID(unit) == statScreenDenyClasses[i])
			return false;
#endif

/* If the unit is in fog, deny access to their stat screen */
#ifdef CONFIG_MULTIPLE_FOG_STAGES
    if (!gBmMapFog[unit->yPos][unit->xPos])
        return false;
#endif

    if (UNIT_IS_GORGON_EGG(unit))
        return false;

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
                continue;
        }
#endif

/* If the unit is in fog, deny access to their stat screen */
#ifdef CONFIG_MULTIPLE_FOG_STAGES
        if (!gBmMapFog[unit->yPos][unit->xPos])
            continue;
#endif

        if (UNIT_IS_GORGON_EGG(unit))
            continue;

        return unit;
    }
}