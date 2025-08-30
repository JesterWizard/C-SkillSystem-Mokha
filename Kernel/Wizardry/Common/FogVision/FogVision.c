#include "common-chax.h"

LYN_REPLACE_CHECK(GetUnitFogViewRange);
int GetUnitFogViewRange(struct Unit* unit) {

    int result = 0;

#ifdef CONFIG_MODULAR_FOG_UNIT_SIGHT
    switch (unit->pClassData->number)
    {
    case CLASS_EIRIKA_LORD:
    case CLASS_EIRIKA_MASTER_LORD:
    case CLASS_EPHRAIM_LORD:
    case CLASS_EPHRAIM_MASTER_LORD:
        result = 1;
        break;
    case CLASS_PALADIN:
    case CLASS_PALADIN_F:
        result = 2;
        break;

    default:
        result = 1;
        break;
    }
#else
    int result = gPlaySt.chapterVisionRange;
#endif

    if (UNIT_CATTRIBUTES(unit) & CA_THIEF)
        result += 5;

    return result + unit->torchDuration;
}