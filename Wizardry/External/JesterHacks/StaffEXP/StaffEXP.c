#include "common-chax.h"

u8 StaffEXP(int weapon)
{
    int exp = 0;

    switch (weapon)
    {
    case ITEM_STAFF_HEAL:
    case ITEM_STAFF_TORCH:
        exp = 15;
    case CONFIG_ITEM_INDEX_FORCE_STAFF:
    case CONFIG_ITEM_INDEX_TEMPEST_STAFF:
    case CONFIG_ITEM_INDEX_ACUITY_STAFF:
    case CONFIG_ITEM_INDEX_SPRINT_STAFF:
    case CONFIG_ITEM_INDEX_FORTUNE_STAFF:
    case CONFIG_ITEM_INDEX_IRON_STAFF:
    case ITEM_STAFF_BARRIER:
        exp = 17;
        break;
    case ITEM_STAFF_MEND:
        exp = 20;
    case ITEM_STAFF_UNLOCK:
    case ITEM_STAFF_RESTORE:
    case ITEM_STAFF_RECOVER:
        exp = 25;
    case ITEM_STAFF_BERSERK:
    case ITEM_STAFF_SLEEP:
    case ITEM_STAFF_SILENCE:
    case ITEM_STAFF_PHYSIC:
        exp = 30;
    case ITEM_STAFF_RESCUE:
        exp = 50;
    case ITEM_STAFF_WARP:
        exp = 60;
    case ITEM_STAFF_RESTORE:
    case ITEM_STAFF_FORTIFY:
        exp = 70;
    case ITEM_STAFF_LATONA:
        exp = 100;
    
    }
    return exp;
}