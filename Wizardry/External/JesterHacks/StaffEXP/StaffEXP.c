#include "common-chax.h"

u8 StaffEXP(int weapon)
{
    int exp = 0;

    switch (weapon)
    {
    case ITEM_STAFF_HEAL:
    case ITEM_STAFF_TORCH:
        exp = 15;
        break;
#ifdef CONFIG_ITEM_INDEX_FORCE_STAFF
    case CONFIG_ITEM_INDEX_FORCE_STAFF:
#endif
#ifdef CONFIG_ITEM_INDEX_TEMPEST_STAFF
    case CONFIG_ITEM_INDEX_TEMPEST_STAFF:
#endif
#ifdef CONFIG_ITEM_INDEX_ACUITY_STAFF
    case CONFIG_ITEM_INDEX_ACUITY_STAFF:
#endif
#ifdef CONFIG_ITEM_INDEX_SPRINT_STAFF
    case CONFIG_ITEM_INDEX_SPRINT_STAFF:
#endif
#ifdef CONFIG_ITEM_INDEX_FORTUNE_STAFF
    case CONFIG_ITEM_INDEX_FORTUNE_STAFF:
#endif
#ifdef CONFIG_ITEM_INDEX_IRON_STAFF
    case CONFIG_ITEM_INDEX_IRON_STAFF:
#endif
    case ITEM_STAFF_BARRIER:
        exp = 17;
        break;
// #ifdef CONFIG_ITEM_INDEX_MINE_STAFF
//     case CONFIG_ITEM_INDEX_MINE_STAFF:
// #endif
    case ITEM_STAFF_MEND:
        exp = 20;
        break;
    case ITEM_STAFF_UNLOCK:
    case ITEM_STAFF_RESTORE:
    case ITEM_STAFF_RECOVER:
        exp = 25;
        break;
    case ITEM_STAFF_BERSERK:
    case ITEM_STAFF_SLEEP:
    case ITEM_STAFF_SILENCE:
    case ITEM_STAFF_PHYSIC:
#ifdef CONFIG_ITEM_INDEX_RUNE_STAFF
    case CONFIG_ITEM_INDEX_RUNE_STAFF:
#endif
#ifdef CONFIG_ITEM_INDEX_SLOW_STAFF
    case CONFIG_ITEM_INDEX_SLOW_STAFF:
#endif
// #ifdef CONFIG_ITEM_INDEX_FORGE_STAFF
//     case CONFIG_ITEM_INDEX_FORGE_STAFF:
// #endif
        exp = 30;
        break;
    case ITEM_STAFF_RESCUE:
        exp = 50;
        break;
    case ITEM_STAFF_WARP:
        exp = 60;
        break;
    case ITEM_STAFF_REPAIR:
    case ITEM_STAFF_FORTIFY:
        exp = 70;
        break;
    case ITEM_STAFF_LATONA:
        exp = 100;
        break;
    
    }
    return exp;
}