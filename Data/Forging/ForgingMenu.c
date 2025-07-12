#include "jester_headers/Forging.h"
#include "constants/texts.h"
#include "common-chax.h"

// const struct MenuItemDef gForgeMenuItems[] = {
// 	{"", 0, 0, 0, 0, ForgeMenuItemUsability, DrawForgeMenuItem, ForgeMenuOnSelect, 0, ForgeMenuSwitchIn, 0},
// 	{"", 0, 0, 0, 0, ForgeMenuItemUsability, DrawForgeMenuItem, ForgeMenuOnSelect, 0, ForgeMenuSwitchIn, 0},
// 	{"", 0, 0, 0, 0, ForgeMenuItemUsability, DrawForgeMenuItem, ForgeMenuOnSelect, 0, ForgeMenuSwitchIn, 0},
// 	{"", 0, 0, 0, 0, ForgeMenuItemUsability, DrawForgeMenuItem, ForgeMenuOnSelect, 0, ForgeMenuSwitchIn, 0},
// 	{"", 0, 0, 0, 0, ForgeMenuItemUsability, DrawForgeMenuItem, ForgeMenuOnSelect, 0, ForgeMenuSwitchIn, 0},
// 	MenuItemsEnd
// };

// const struct MenuDef gForgeMenuDef = {
// 	{13, 1, 17, 0},
// 	0,
// 	gForgeMenuItems,
// 	ForgeMenuInit,
// 	0, 0,
// 	MenuCancelSelect,
// 	0, 0
// };

// void ForgeMenuInit(struct MenuProc* proc) {
// 	NoCashGBAPrint("Init menu");
// 	ResetText();

// 	StartFace(0, GetUnitPortraitId(gActiveUnit), 0x38, 0x18, 2);
// 	DrawUiFrame(gBG1TilemapBuffer, 1, 13, 28, 6, 0, 1);
	
// 	PutSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(9, 14), TEXT_COLOR_SYSTEM_GOLD, TEXT_SPECIAL_G);
// 	PutNumberOrBlank(gBG0TilemapBuffer + TILEMAP_INDEX(8, 14), TEXT_COLOR_SYSTEM_BLUE, GetPartyGoldAmount());
	
// 	struct Text *texts = gPrepItemTexts;
	
// 	InitText(&texts[0], 8);
// 	InitText(&texts[1], 11);
// 	InitText(&texts[2], 8);
// 	InitText(&texts[3], 11);
// 	InitText(&texts[4], 6);
	
// 	for(int i = 0; i < 5; i++) {
		
// 		ClearText(&texts[i]);
// 	}
// }

// int ForgeMenuSwitchIn(struct MenuProc* menu, struct MenuItemProc* menuItem) {
// 	int item = gActiveUnit->items[menuItem->itemNumber];
// 	struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
// 	//struct ForgeBonuses bonuses = gForgeBonuses[GetItemIndex(item)];
// 	struct Text *texts = gPrepItemTexts;
// 	for(int i = 0; i < 5; i++) {
// 		ClearText(&texts[i]);
// 	}

// 	PutDrawText(&texts[0], TILEMAP_LOCATED(gBG0TilemapBuffer, 11, 14), TEXT_COLOR_SYSTEM_GOLD, 0, 0, GetStringFromIndex(0x0503));
// 	PutDrawText(&texts[1], TILEMAP_LOCATED(gBG0TilemapBuffer, 19, 14), TEXT_COLOR_SYSTEM_GOLD, 0, 0, GetStringFromIndex(0x04F4));
// 	PutDrawText(&texts[2], TILEMAP_LOCATED(gBG0TilemapBuffer, 11, 16), TEXT_COLOR_SYSTEM_GOLD, 0, 0, GetStringFromIndex(0x0502));
// 	PutDrawText(&texts[3], TILEMAP_LOCATED(gBG0TilemapBuffer, 19, 16), TEXT_COLOR_SYSTEM_GOLD, 0, 0, GetStringFromIndex(0x0501));
// 	PutDrawText(&texts[4], TILEMAP_LOCATED(gBG0TilemapBuffer, 3, 16),  TEXT_COLOR_SYSTEM_GOLD, 0, 0, GetStringFromIndex(0x0001));
	
// 	Text_InsertDrawNumberOrBlank(&texts[0], 0x16, TEXT_COLOR_SYSTEM_BLUE, GetItemMight(item));
// 	Text_InsertDrawNumberOrBlank(&texts[1], 0x1E, TEXT_COLOR_SYSTEM_BLUE, GetItemHit(item));
// 	Text_InsertDrawNumberOrBlank(&texts[2], 0x16, TEXT_COLOR_SYSTEM_BLUE, GetItemWeight(item));
// 	Text_InsertDrawNumberOrBlank(&texts[3], 0x1E, TEXT_COLOR_SYSTEM_BLUE, GetItemCrit(item));
// 	Text_InsertDrawNumberOrBlank(&texts[4], 0x28, TEXT_COLOR_SYSTEM_BLUE, GetItemForgeCost(item));
	
// 	PutSpecialChar(TILEMAP_LOCATED(gBG0TilemapBuffer, 15, 14), TEXT_COLOR_SYSTEM_WHITE, TEXT_SPECIAL_ARROW);
// 	PutSpecialChar(TILEMAP_LOCATED(gBG0TilemapBuffer, 24, 14), TEXT_COLOR_SYSTEM_WHITE, TEXT_SPECIAL_ARROW);
// 	PutSpecialChar(TILEMAP_LOCATED(gBG0TilemapBuffer, 15, 16), TEXT_COLOR_SYSTEM_WHITE, TEXT_SPECIAL_ARROW);
// 	PutSpecialChar(TILEMAP_LOCATED(gBG0TilemapBuffer, 24, 16), TEXT_COLOR_SYSTEM_WHITE, TEXT_SPECIAL_ARROW);
// 	PutSpecialChar(TILEMAP_LOCATED(gBG0TilemapBuffer, 2, 16),  TEXT_COLOR_SYSTEM_WHITE, TEXT_SPECIAL_DASH);
	
// 	if(limits.maxCount && (GetItemForgeCount(item) < limits.maxCount)) {
// 		int forgedItem = item + (1 << 8);
// 		Text_InsertDrawNumberOrBlank(&texts[0], 0x32, TEXT_COLOR_SYSTEM_BLUE, GetItemMight(forgedItem));
// 		Text_InsertDrawNumberOrBlank(&texts[1], 0x42, TEXT_COLOR_SYSTEM_BLUE, GetItemHit(forgedItem));
// 		Text_InsertDrawNumberOrBlank(&texts[2], 0x32, TEXT_COLOR_SYSTEM_BLUE, GetItemWeight(forgedItem));
// 		Text_InsertDrawNumberOrBlank(&texts[3], 0x42, TEXT_COLOR_SYSTEM_BLUE, GetItemCrit(forgedItem));
// 	}

// 	return 0;
// }

// u8 ForgeMenuItemUsability(const struct MenuItemDef*, int number) {
// 	int item = gActiveUnit->items[number];
	
// 	if(item == 0) {
// 		return MENU_NOTSHOWN;
// 	}
	
// 	if(!(IsItemForgeable(item))) {
// 		return MENU_DISABLED;
// 	}
	
// 	return MENU_ENABLED;
// }

// int DrawForgeMenuItem(struct MenuProc* menu, struct MenuItemProc* menuItem) {
// 	int item = gActiveUnit->items[menuItem->itemNumber];
	
// 	DrawItemForgeMenuLine(&menuItem->text, item, gBG0TilemapBuffer + TILEMAP_INDEX(menuItem->xTile, menuItem->yTile));
	
// 	BG_EnableSyncByMask(BG0_SYNC_BIT);

// 	return 0;
// }

// void DrawItemForgeMenuLine(struct Text* text, int item, u16* mapOut) {
// 	int count = GetItemForgeCount(item);
// 	struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
// 	bool isForgeable = IsItemForgeable(item);
	
//     Text_SetParams(text, 0, (isForgeable ? TEXT_COLOR_SYSTEM_WHITE : TEXT_COLOR_SYSTEM_GRAY));
//     Text_DrawString(text, GetItemName(item));
//     PutText(text, mapOut + 2);
	
// 	int color;
// 	if (limits.maxCount == 0) {
// 		color = TEXT_COLOR_SYSTEM_BLUE;
// 		PutNumberOrBlank(mapOut + 11, color, GetItemUses(item));
		
// 		PutSpecialChar(mapOut + 12, TEXT_COLOR_SYSTEM_WHITE, TEXT_SPECIAL_SLASH);
		
// 		PutNumberOrBlank(mapOut + 14, color, GetItemMaxUses(item));		
// 	}
// 	else {
// 		color = TEXT_COLOR_SYSTEM_GOLD;
// 		if (count >= limits.maxCount) {
// 			PutSpecialChar(mapOut + 13, color, TEXT_SPECIAL_PLUS);
// 			PutNumberOrBlank(mapOut + 14, color, count);				
// 		}
// 		else {
// 			PutSpecialChar(mapOut + 10, color, TEXT_SPECIAL_PLUS);
// 			PutNumberOrBlank(mapOut + 11, color, count);
			
// 			PutSpecialChar(mapOut + 12, TEXT_COLOR_SYSTEM_WHITE, TEXT_SPECIAL_ARROW);
			
// 			PutSpecialChar(mapOut + 13, color, TEXT_SPECIAL_PLUS);
// 			PutNumberOrBlank(mapOut + 14, color, count + 1);				
// 		}
// 	}
//     DrawIcon(mapOut, GetItemIconId(item), 0x4000);
// }

// u8 ForgeMenuOnSelect(struct MenuProc* menu, struct MenuItemProc* menuItem) {
// 	int item = gActiveUnit->items[menuItem->itemNumber];
// 	struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
// 	if(IsItemForgeable(item)) {
// 		gActiveUnit->items[menuItem->itemNumber] += (1 << 8);
// 		gPlaySt.partyGoldAmount -= GetItemForgeCost(item);
// 		return MENU_ACT_CLEAR | MENU_ACT_SND6A | MENU_ACT_END | MENU_ACT_SKIPCURSOR;
// 	}

// 	if(limits.maxCount == 0) {
// 		MenuFrozenHelpBox(menu, ItemCannotBeForgedRText);
// 	}

// 	else if(GetItemForgeCount(item) >= limits.maxCount) {
// 		MenuFrozenHelpBox(menu, ItemAtMaxForgeCountRText);
// 	}
	
// 	else {
// 		MenuFrozenHelpBox(menu, NotEnoughGoldToForgeItemRText);
// 	}
	
// 	return MENU_ACT_SND6B;
// }

// void CallForgeMenuASMC(struct EventEngineProc* proc) {
//     ClearBg0Bg1();
//     SetDispEnable(1, 1, 1, 1, 1);
//     SetTextFont(0);
//     InitSystemTextFont();
//     LoadUiFrameGraphics();
// 	ResetIconGraphics_();
	
// 	StartMenu(&gForgeMenuDef, proc);
// }