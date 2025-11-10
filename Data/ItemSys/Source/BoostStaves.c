#include "common-chax.h"
#include "item-sys.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "strmag.h"

//! FE8U = 0x08034B48
LYN_REPLACE_CHECK(DrawUnitResChangeText);
void DrawUnitResChangeText(struct Text* text, struct Unit* unit, int bonus) {
    ClearText(text);

    char * statName = "Null";
    int statNumber = 0;

    int itemId = GetItemIndex(gActiveUnit->items[0]);

    switch (itemId)
    {
#ifdef CONFIG_ITEM_INDEX_FORCE_STAFF
    case CONFIG_ITEM_INDEX_FORCE_STAFF:
        statName = "Str";
        statNumber = GetUnitPower(unit);
        break;
#endif
#ifdef CONFIG_ITEM_INDEX_TEMPEST_STAFF
    case CONFIG_ITEM_INDEX_TEMPEST_STAFF:
        statName = "Mag";
        statNumber = GetUnitMagic(unit);
        break;
#endif
#ifdef CONFIG_ITEM_INDEX_ACUITY_STAFF
    case CONFIG_ITEM_INDEX_ACUITY_STAFF:
        statName = "Skl";
        statNumber = GetUnitSkill(unit);
        break;
#endif
#ifdef CONFIG_ITEM_INDEX_SPRINT_STAFF
    case CONFIG_ITEM_INDEX_SPRINT_STAFF:
        statName = "Spd";
        statNumber = GetUnitSpeed(unit);
        break;
#endif
#ifdef CONFIG_ITEM_INDEX_FORTUNE_STAFF
    case CONFIG_ITEM_INDEX_FORTUNE_STAFF:
        statName = "Lck";
        statNumber = GetUnitLuck(unit);
        break;
#endif
#ifdef CONFIG_ITEM_INDEX_IRON_STAFF
    case CONFIG_ITEM_INDEX_IRON_STAFF:
        statName = "Def";
        statNumber = GetUnitDefense(unit);
        break;
#endif
    case ITEM_STAFF_BARRIER:
        statName = "Res";
        statNumber = GetUnitResistance(unit);
        break;

#ifdef CONFIG_ITEM_INDEX_OMNI_STAFF
    case CONFIG_ITEM_INDEX_OMNI_STAFF:
        statName = "Omni";
        statNumber = 0;
        break;
#endif

    default:
        break;
    }

    Text_InsertDrawString(text, 0, 3, statName);
    Text_InsertDrawNumberOrBlank(text, 32, 2, statNumber);
    Text_InsertDrawNumberOrBlank(text, 56, 2, statNumber + bonus);

    Text_InsertDrawString(text, 40, 3, " - ");

    return;
}

//! FE8U = 0x080350A4
LYN_REPLACE_CHECK(RefreshUnitResChangeInfoWindow);
void RefreshUnitResChangeInfoWindow(struct Unit* unit) {
    int y = 0;
    int x = GetUnitInfoWindowX(unit, 10);

    struct UnitInfoWindowProc* proc = UnitInfoWindow_DrawBase(0, unit, x, y, 10, 1);

    DrawUnitResChangeText(proc->lines + 0, unit, 7 - unit->barrierDuration);
    PutText(proc->lines + 0, gBG0TilemapBuffer + TILEMAP_INDEX(x + 1, y + 3));

    return;
}