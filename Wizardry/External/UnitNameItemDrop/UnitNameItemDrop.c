#include "common-chax.h"
#include "constants/texts.h"

struct PopupInstruction const NewItemDropPopup[] = {
    POPUP_SOUND(0x5A),
	POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
	POPUP_UNIT_NAME,
    POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
    POPUP_MSG(MSG_GOT_ITEM),             /* got */
    POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
    POPUP_ITEM_STR,
    POPUP_SPACE(1),
    POPUP_ITEM_ICON,
    POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
    POPUP_SPACE(1),
    POPUP_MSG(0x022),                   /* .[.] */
    POPUP_END
};

struct PopupInstruction const NewItemPilferedPopup[] = {
    POPUP_SOUND(0x5A),
	POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
	POPUP_UNIT_NAME,
    POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
    POPUP_MSG(MSG_PILFERED_ITEM),       /* pilfered */
    POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
    POPUP_ITEM_STR,
    POPUP_SPACE(1),
    POPUP_ITEM_ICON,
    POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
    POPUP_SPACE(1),
    POPUP_MSG(0x022),                   /* .[.] */
    POPUP_END
};

struct PopupInstruction const NewItemStolePopup[] = {
    POPUP_SOUND(0x5C),
	POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
	POPUP_UNIT_NAME,
	POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
    POPUP_MSG(MSG_STOLE_ITEM),          /* Stole */
	POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
    POPUP_ITEM_STR,
    POPUP_SPACE(1),
    POPUP_ITEM_ICON,
    POPUP_SPACE(1),
    POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
    POPUP_MSG(0x022),                   /* .[.] */
    POPUP_END
};

struct PopupInstruction const NewGoldGotPopup[] = {
    POPUP_SOUND(0x5A),
	POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
	POPUP_UNIT_NAME,
    POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
    POPUP_MSG(MSG_GOT_ITEM),               /* got */
    POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
    POPUP_NUM,
    POPUP_SPACE(3),
    POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
    POPUP_MSG(MSG_GOLD),                   /* gold.[.] */
    POPUP_END
};

struct PopupInstruction const NewGoldStolenPopup[] = {
    POPUP_SOUND(0x5C),
	POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
	POPUP_UNIT_NAME,
	POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
    POPUP_MSG(MSG_STOLE_ITEM),              /* Stole */
    POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
    POPUP_NUM,
    POPUP_SPACE(3),
    POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
    POPUP_MSG(MSG_GOLD),                   /* gold. */
    POPUP_END
};

LYN_REPLACE_CHECK(NewPopup_ItemGot_unused);
void NewPopup_ItemGot_unused(struct Unit* unit, u16 item, ProcPtr parent)
{
    SetPopupItem(item);
	SetPopupUnit(unit);

    if (FACTION_BLUE == UNIT_FACTION(unit))
        NewPopup_Simple(NewItemDropPopup, 0x60, 0x0, parent);
    else
        NewPopup_Simple(NewItemPilferedPopup, 0x60, 0x0, parent);
}


/* I'm unable to use these two functions as any amends in their current state, event a debug statement causes a bad jump error I can't solve */
// LYN_REPLACE_CHECK(NewPopup_GoldGot);
// void NewPopup_GoldGot(ProcPtr parent, struct Unit *unit, int value)
// {
//     // SetPopupNumber(value);
// 	// SetPopupUnit(unit);

//     if (FACTION_BLUE == UNIT_FACTION(unit)) 
//     {
//       //  value += GetPartyGoldAmount();
//        // SetPartyGoldAmount(value);
//        //NewPopup_Simple(NewGoldGotPopup, 0x60, 0x0, parent);
//     } 
//    // else
//    //     NewPopup_Simple(NewGoldStolenPopup, 0x60, 0x0, parent);
// }

// LYN_REPLACE_CHECK(NewPopup_ItemStealing);
// void NewPopup_ItemStealing(u16 item, ProcPtr parent)
// {
//     // SetPopupItem(item);
// 	// SetPopupUnit(gActiveUnit);

//     // NewPopup_Simple(NewItemStolePopup, 0x60, 0x0, parent);
// }