#include "common-chax.h"
#include "item-sys.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "bmitem.h"
#include "jester_headers/Forging.h"

#ifdef CONFIG_FORGING
    int GetItemForgeCount(int item) {
        struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
        if(limits.maxCount == 0) {
            return 0;
        }
        return ITEM_USES(item);
    }

    int GetItemForgeCost(int item) {
        struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
        int count = GetItemForgeCount(item);
        if(count >= limits.maxCount) {
            return 0;
        }
        return (count + 1) * limits.baseCost;
    }

    bool IsItemForgeable(int item) {
        struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
        //int count = GetItemForgeCount(item);
        
        if(GetItemForgeCount(item) >= limits.maxCount) {
            return false;
        }
        
        if(GetItemForgeCost(item) > (int)GetPartyGoldAmount()) {
            return false;
        }
        
        return true;
    }

    const struct MenuItemDef gForgeMenuItems[] = {
        {"", 0, 0, 0, 0, ForgeMenuItemUsability, DrawForgeMenuItem, ForgeMenuOnSelect, 0, ForgeMenuSwitchIn, 0},
        {"", 0, 0, 0, 0, ForgeMenuItemUsability, DrawForgeMenuItem, ForgeMenuOnSelect, 0, ForgeMenuSwitchIn, 0},
        {"", 0, 0, 0, 0, ForgeMenuItemUsability, DrawForgeMenuItem, ForgeMenuOnSelect, 0, ForgeMenuSwitchIn, 0},
        {"", 0, 0, 0, 0, ForgeMenuItemUsability, DrawForgeMenuItem, ForgeMenuOnSelect, 0, ForgeMenuSwitchIn, 0},
        {"", 0, 0, 0, 0, ForgeMenuItemUsability, DrawForgeMenuItem, ForgeMenuOnSelect, 0, ForgeMenuSwitchIn, 0},
        MenuItemsEnd
    };

    const struct MenuDef gForgeMenuDef = {
        {13, 1, 17, 0},
        0,
        gForgeMenuItems,
        ForgeMenuInit,
        0, 0,
        MenuCancelSelect,
        0, 0
    };

    void ForgeMenuInit(struct MenuProc* proc) {
        ResetText();

        StartFace(0, GetUnitPortraitId(gActiveUnit), 0x38, 0x18, 2);
        DrawUiFrame(gBG1TilemapBuffer, 1, 13, 28, 6, 0, 1);
        
        PutSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(9, 14), TEXT_COLOR_SYSTEM_GOLD, TEXT_SPECIAL_G);
        PutNumberOrBlank(gBG0TilemapBuffer + TILEMAP_INDEX(8, 14), TEXT_COLOR_SYSTEM_BLUE, GetPartyGoldAmount());
        
        struct Text *texts = gPrepItemTexts;
        
        InitText(&texts[0], 8);
        InitText(&texts[1], 11);
        InitText(&texts[2], 8);
        InitText(&texts[3], 11);
        InitText(&texts[4], 6);
        
        for(int i = 0; i < 5; i++) {
            
            ClearText(&texts[i]);
        }
    }

    int ForgeMenuSwitchIn(struct MenuProc* menu, struct MenuItemProc* menuItem) {
        int item = gActiveUnit->items[menuItem->itemNumber];
        struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
        //struct ForgeBonuses bonuses = gForgeBonuses[GetItemIndex(item)];
        struct Text *texts = gPrepItemTexts;
        for(int i = 0; i < 5; i++) {
            ClearText(&texts[i]);
        }

        PutDrawText(&texts[0], TILEMAP_LOCATED(gBG0TilemapBuffer, 11, 14), TEXT_COLOR_SYSTEM_GOLD, 0, 0, GetStringFromIndex(0x0503));
        PutDrawText(&texts[1], TILEMAP_LOCATED(gBG0TilemapBuffer, 19, 14), TEXT_COLOR_SYSTEM_GOLD, 0, 0, GetStringFromIndex(0x04F4));
        PutDrawText(&texts[2], TILEMAP_LOCATED(gBG0TilemapBuffer, 11, 16), TEXT_COLOR_SYSTEM_GOLD, 0, 0, GetStringFromIndex(0x0502));
        PutDrawText(&texts[3], TILEMAP_LOCATED(gBG0TilemapBuffer, 19, 16), TEXT_COLOR_SYSTEM_GOLD, 0, 0, GetStringFromIndex(0x0501));
        PutDrawText(&texts[4], TILEMAP_LOCATED(gBG0TilemapBuffer, 3, 16),  TEXT_COLOR_SYSTEM_GOLD, 0, 0, GetStringFromIndex(0x0001));
        
        PutSpecialChar(TILEMAP_LOCATED(gBG0TilemapBuffer, 15, 14), TEXT_COLOR_SYSTEM_WHITE, TEXT_SPECIAL_ARROW);
        PutSpecialChar(TILEMAP_LOCATED(gBG0TilemapBuffer, 24, 14), TEXT_COLOR_SYSTEM_WHITE, TEXT_SPECIAL_ARROW);
        PutSpecialChar(TILEMAP_LOCATED(gBG0TilemapBuffer, 15, 16), TEXT_COLOR_SYSTEM_WHITE, TEXT_SPECIAL_ARROW);
        PutSpecialChar(TILEMAP_LOCATED(gBG0TilemapBuffer, 24, 16), TEXT_COLOR_SYSTEM_WHITE, TEXT_SPECIAL_ARROW);
        PutSpecialChar(TILEMAP_LOCATED(gBG0TilemapBuffer, 2, 16),  TEXT_COLOR_SYSTEM_WHITE, TEXT_SPECIAL_DASH);
        
        if(limits.maxCount && (GetItemForgeCount(item) < limits.maxCount)) {

            Text_InsertDrawNumberOrBlank(&texts[0], 0x16, TEXT_COLOR_SYSTEM_BLUE, GetItemMight(item));
            Text_InsertDrawNumberOrBlank(&texts[1], 0x1E, TEXT_COLOR_SYSTEM_BLUE, GetItemHit(item));
            Text_InsertDrawNumberOrBlank(&texts[2], 0x16, TEXT_COLOR_SYSTEM_BLUE, GetItemWeight(item));
            Text_InsertDrawNumberOrBlank(&texts[3], 0x1E, TEXT_COLOR_SYSTEM_BLUE, GetItemCrit(item));
            Text_InsertDrawNumberOrBlank(&texts[4], 0x28, TEXT_COLOR_SYSTEM_BLUE, GetItemForgeCost(item));

            int forgedItem = item + (1 << 8);
            Text_InsertDrawNumberOrBlank(&texts[0], 0x32, TEXT_COLOR_SYSTEM_GREEN, GetItemMight(forgedItem));
            Text_InsertDrawNumberOrBlank(&texts[1], 0x42, TEXT_COLOR_SYSTEM_GREEN, GetItemHit(forgedItem));
            Text_InsertDrawNumberOrBlank(&texts[2], 0x32, TEXT_COLOR_SYSTEM_GREEN, GetItemWeight(forgedItem));
            Text_InsertDrawNumberOrBlank(&texts[3], 0x42, TEXT_COLOR_SYSTEM_GREEN, GetItemCrit(forgedItem));
        }
        // If the item is at max forge count or cannot be forged, then show their text in green
        else {
            Text_InsertDrawNumberOrBlank(&texts[0], 0x16, TEXT_COLOR_SYSTEM_GREEN, GetItemMight(item));
            Text_InsertDrawNumberOrBlank(&texts[1], 0x1E, TEXT_COLOR_SYSTEM_GREEN, GetItemHit(item));
            Text_InsertDrawNumberOrBlank(&texts[2], 0x16, TEXT_COLOR_SYSTEM_GREEN, GetItemWeight(item));
            Text_InsertDrawNumberOrBlank(&texts[3], 0x1E, TEXT_COLOR_SYSTEM_GREEN, GetItemCrit(item));
            Text_InsertDrawNumberOrBlank(&texts[4], 0x28, TEXT_COLOR_SYSTEM_GREEN, GetItemForgeCost(item));
        }

        return 0;
    }

    u8 ForgeMenuItemUsability(const struct MenuItemDef*, int number) {
        int item = gActiveUnit->items[number];
        
        if(item == 0) {
            return MENU_NOTSHOWN;
        }
        
        if(!(IsItemForgeable(item))) {
            return MENU_DISABLED;
        }
        
        return MENU_ENABLED;
    }

    int DrawForgeMenuItem(struct MenuProc* menu, struct MenuItemProc* menuItem) {
        int item = gActiveUnit->items[menuItem->itemNumber];
        
        DrawItemForgeMenuLine(&menuItem->text, item, gBG0TilemapBuffer + TILEMAP_INDEX(menuItem->xTile, menuItem->yTile));
        
        BG_EnableSyncByMask(BG0_SYNC_BIT);

        return 0;
    }

    void DrawItemForgeMenuLine(struct Text* text, int item, u16* mapOut) {
        int count = GetItemForgeCount(item);
        struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
        bool isForgeable = IsItemForgeable(item);
        
        Text_SetParams(text, 0, (isForgeable ? TEXT_COLOR_SYSTEM_WHITE : TEXT_COLOR_SYSTEM_GRAY));
        Text_DrawString(text, GetItemName(item));
        PutText(text, mapOut + 2);
        
        int color;
        if (limits.maxCount == 0) {
            color = TEXT_COLOR_SYSTEM_BLUE;
            PutNumberOrBlank(mapOut + 11, color, GetItemUses(item));
            
            PutSpecialChar(mapOut + 12, TEXT_COLOR_SYSTEM_WHITE, TEXT_SPECIAL_SLASH);
            
            PutNumberOrBlank(mapOut + 14, color, GetItemMaxUses(item));		
        }
        else {
            color = TEXT_COLOR_SYSTEM_GOLD;
            if (count >= limits.maxCount) {
                PutSpecialChar(mapOut + 13, color, TEXT_SPECIAL_PLUS);
                PutNumberOrBlank(mapOut + 14, color, count);				
            }
            else {
                PutSpecialChar(mapOut + 10, color, TEXT_SPECIAL_PLUS);
                PutNumberOrBlank(mapOut + 11, color, count);
                
                PutSpecialChar(mapOut + 12, TEXT_COLOR_SYSTEM_WHITE, TEXT_SPECIAL_ARROW);
                
                PutSpecialChar(mapOut + 13, color, TEXT_SPECIAL_PLUS);
                PutNumberOrBlank(mapOut + 14, color, count + 1);				
            }
        }
        DrawIcon(mapOut, GetItemIconId(item), 0x4000);
    }

    u8 ForgeMenuOnSelect(struct MenuProc* menu, struct MenuItemProc* menuItem) {
        int item = gActiveUnit->items[menuItem->itemNumber];
        struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
        if(IsItemForgeable(item)) {
            gActiveUnit->items[menuItem->itemNumber] += (1 << 8);
            gPlaySt.partyGoldAmount -= GetItemForgeCost(item);
            return MENU_ACT_CLEAR | MENU_ACT_SND6A | MENU_ACT_END | MENU_ACT_SKIPCURSOR;
        }

        if(limits.maxCount == 0) {
            MenuFrozenHelpBox(menu, ItemCannotBeForgedRText);
        }

        else if(GetItemForgeCount(item) >= limits.maxCount) {
            MenuFrozenHelpBox(menu, ItemAtMaxForgeCountRText);
        }
        
        else {
            MenuFrozenHelpBox(menu, NotEnoughGoldToForgeItemRText);
        }
        
        return MENU_ACT_SND6B;
    }

    void CallForgeMenuASMC(struct EventEngineProc* proc) {
        ClearBg0Bg1();
        SetDispEnable(1, 1, 1, 1, 1);
        SetTextFont(0);
        InitSystemTextFont();
        LoadUiFrameGraphics();
        ResetIconGraphics_();
        
        StartMenu(&gForgeMenuDef, proc);
    }
#endif

LYN_REPLACE_CHECK(GetItemData);
const struct ItemData * GetItemData(int itemIndex) 
{
    return gItemData_New + itemIndex;
}

LYN_REPLACE_CHECK(GetItemNameWithArticle);
char * GetItemNameWithArticle(int item, s8 capitalize)
{
    return GetItemName(item);
}

LYN_REPLACE_CHECK(GetItemName);
char * GetItemName(int item)
{
    char * result;

#if CHAX
    if (IsDuraItem(item))
        return GetDuraItemName(item);
#endif

    result = GetStringFromIndex(GetItemData(ITEM_INDEX(item))->nameTextId);
    result = StrInsertTact();
    return result;
}

LYN_REPLACE_CHECK(GetItemDescId);
int GetItemDescId(int item)
{
#if CHAX
    if (IsDuraItem(item))
        return GetDuraItemDescId(item);
#endif

    return GetItemData(ITEM_INDEX(item))->descTextId;
}

LYN_REPLACE_CHECK(GetItemUseDescId);
int GetItemUseDescId(int item)
{
#if CHAX
    if (IsDuraItem(item))
        return GetDuraItemUseDescId(item);
#endif

    return GetItemData(ITEM_INDEX(item))->useDescTextId;
}

LYN_REPLACE_CHECK(GetItemUses);
int GetItemUses(int item)
{
    if (GetItemAttributes(item) & IA_UNBREAKABLE)
        return 0xFF;

#if CHAX
    if (IsDuraItem(item))
        return 1;
#endif

    return ITEM_USES(item);
}

LYN_REPLACE_CHECK(GetItemMaxUses);
int GetItemMaxUses(int item)
{
    if (GetItemAttributes(item) & IA_UNBREAKABLE)
        return 0xFF;

#if CHAX
    if (IsDuraItem(item))
        return 1;
#endif

    return GetItemData(ITEM_INDEX(item))->maxUses;
}

LYN_REPLACE_CHECK(GetItemIconId);
int GetItemIconId(int item)
{
    if (!item)
        return -1;

#if CHAX
    if (IsDuraItem(item))
        return GetDuraItemIconId(item);
#endif

    return GetItemData(ITEM_INDEX(item))->iconId;
}

LYN_REPLACE_CHECK(GetItemHit);
int GetItemHit(int item) {
	int result = GetItemData(ITEM_INDEX(item))->hit;

#ifdef CONFIG_FORGING
	struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
	struct ForgeBonuses bonuses = gForgeBonuses[limits.forgeIndex];
		
	int forgeHit = (GetItemForgeCount(item) * bonuses.hitBonus) / 6;
		
	result += forgeHit;		
#endif

    return result;
}

LYN_REPLACE_CHECK(GetItemWeight);
int GetItemWeight(int item) {
	int result = GetItemData(ITEM_INDEX(item))->weight;

#ifdef CONFIG_FORGING
	struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
	struct ForgeBonuses bonuses = gForgeBonuses[limits.forgeIndex];
		
	int forgeWt = (GetItemForgeCount(item) * bonuses.wtBonus) / 6;
		
	result += forgeWt;
#endif

    return result;
}

LYN_REPLACE_CHECK(GetItemCrit);
int GetItemCrit(int item) {
	int result = GetItemData(ITEM_INDEX(item))->crit;

#ifdef CONFIG_FORGING
	struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
	struct ForgeBonuses bonuses = gForgeBonuses[limits.forgeIndex];
		
	int forgeCrit = (GetItemForgeCount(item) * bonuses.critBonus) / 6;
		
	result += forgeCrit;
#endif

    return result;
}

LYN_REPLACE_CHECK(GetItemCost);
int GetItemCost(int item)
{
    if (GetItemAttributes(item) & IA_UNBREAKABLE)
        return GetItemData(ITEM_INDEX(item))->costPerUse;
    else
        return GetItemData(ITEM_INDEX(item))->costPerUse * GetItemUses(item);
}

LYN_REPLACE_CHECK(GetItemMaxCost);
int GetItemMaxCost(int item)
{
    return GetItemData(ITEM_INDEX(item))->costPerUse * GetItemMaxUses(item);
}

LYN_REPLACE_CHECK(GetItemMight);
int GetItemMight(int item) {
	int result = GetItemData(ITEM_INDEX(item))->might;

#ifdef CONFIG_FORGING
	struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
	struct ForgeBonuses bonuses = gForgeBonuses[limits.forgeIndex];
		
	int forgeMt = (GetItemForgeCount(item) * bonuses.mtBonus) / 6;
		
	result += forgeMt;
#endif

    return result;
}

LYN_REPLACE_CHECK(DrawItemMenuLine);
void DrawItemMenuLine(struct Text * text, int item, s8 isUsable, u16 * mapOut)
{
    Text_SetParams(text, 0, (isUsable ? TEXT_COLOR_SYSTEM_WHITE : TEXT_COLOR_SYSTEM_GRAY));
    Text_DrawString(text, GetItemName(item));

    PutText(text, mapOut + 2);

#ifndef CONFIG_INFINITE_DURABILITY
    if (!IsDuraItem(item))
    {
        PutNumberOrBlank(mapOut + 11, isUsable ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY, GetItemUses(item));
    }
#endif

#ifdef CONFIG_FORGING
	struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
	if(GetItemForgeCount(item) && limits.maxCount) {
		PutSpecialChar(mapOut + 10, isUsable ? TEXT_COLOR_SYSTEM_GOLD : TEXT_COLOR_SYSTEM_GRAY, TEXT_SPECIAL_PLUS);
		PutNumberOrBlank(mapOut + 11, isUsable ? TEXT_COLOR_SYSTEM_GOLD : TEXT_COLOR_SYSTEM_GRAY, GetItemForgeCount(item));
	}
	else if(limits.maxCount == 0) {
		PutNumberOrBlank(mapOut + 11, isUsable ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY, GetItemUses(item));		
	}
#endif

    DrawIcon(mapOut, GetItemIconId(item), 0x4000);
}

LYN_REPLACE_CHECK(DrawItemMenuLineLong);
void DrawItemMenuLineLong(struct Text * text, int item, s8 isUsable, u16 * mapOut)
{
    Text_SetParams(text, 0, (isUsable ? TEXT_COLOR_SYSTEM_WHITE : TEXT_COLOR_SYSTEM_GRAY));
    Text_DrawString(text, GetItemName(item));

    PutText(text, mapOut + 2);

#ifndef CONFIG_INFINITE_DURABILITY
    if (!IsDuraItem(item))
    {
        PutNumberOrBlank(mapOut + 10, isUsable ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY, GetItemUses(item));
        PutNumberOrBlank(mapOut + 13, isUsable ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY, GetItemMaxUses(item));
        PutSpecialChar(mapOut + 11, isUsable ? TEXT_COLOR_SYSTEM_WHITE : TEXT_COLOR_SYSTEM_GRAY, TEXT_SPECIAL_SLASH);
    }
#endif

#ifdef CONFIG_FORGING
	struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
	if(GetItemForgeCount(item) && limits.maxCount) {
		PutSpecialChar(mapOut + 12, isUsable ? TEXT_COLOR_SYSTEM_GOLD : TEXT_COLOR_SYSTEM_GRAY, TEXT_SPECIAL_PLUS);
		PutNumberOrBlank(mapOut + 13, isUsable ? TEXT_COLOR_SYSTEM_GOLD : TEXT_COLOR_SYSTEM_GRAY, GetItemForgeCount(item));
	}
	else if(limits.maxCount == 0) {
		PutNumberOrBlank(mapOut + 10, isUsable ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY, GetItemUses(item));
		PutNumberOrBlank(mapOut + 13, isUsable ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY, GetItemMaxUses(item));
		PutSpecialChar(mapOut + 11, isUsable ? TEXT_COLOR_SYSTEM_WHITE : TEXT_COLOR_SYSTEM_GRAY, TEXT_SPECIAL_SLASH);		
	}
#endif

    DrawIcon(mapOut, GetItemIconId(item), 0x4000);
}

LYN_REPLACE_CHECK(DrawItemMenuLineNoColor);
void DrawItemMenuLineNoColor(struct Text * text, int item, u16 * mapOut)
{
    Text_SetCursor(text, 0);
    Text_DrawString(text, GetItemName(item));

    PutText(text, mapOut + 2);

#ifndef CONFIG_INFINITE_DURABILITY
    if (!IsDuraItem(item))
    {
        PutNumberOrBlank(mapOut + 11, Text_GetColor(text), GetItemUses(item));
    }
#endif

#ifdef CONFIG_FORGING
	struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
	if(GetItemForgeCount(item) && limits.maxCount) {
		PutSpecialChar(mapOut + 10, Text_GetColor(text), TEXT_SPECIAL_PLUS);
		PutNumberOrBlank(mapOut + 11, Text_GetColor(text), GetItemForgeCount(item));
	}
	else if(limits.maxCount == 0) {
		PutNumberOrBlank(mapOut + 11, Text_GetColor(text), GetItemUses(item));		
	}
#endif
    
    DrawIcon(mapOut, GetItemIconId(item), 0x4000);
}

LYN_REPLACE_CHECK(DrawItemStatScreenLine);
void DrawItemStatScreenLine(struct Text * text, int item, int nameColor, u16 * mapOut)
{
    int color;

    ClearText(text);

    color = nameColor;
    Text_SetColor(text, color);

    Text_DrawString(text, GetItemName(item));

#ifndef CONFIG_INFINITE_DURABILITY
    if (!IsDuraItem(item))
    {
        color = (nameColor == TEXT_COLOR_SYSTEM_GRAY) ? TEXT_COLOR_SYSTEM_GRAY : TEXT_COLOR_SYSTEM_WHITE;
        PutSpecialChar(mapOut + 12, color, TEXT_SPECIAL_SLASH);

        color = (nameColor != TEXT_COLOR_SYSTEM_GRAY) ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY;
        PutNumberOrBlank(mapOut + 11, color, GetItemUses(item));
        PutNumberOrBlank(mapOut + 14, color, GetItemMaxUses(item));
    }
#endif

#ifdef CONFIG_FORGING
	struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
	if(GetItemForgeCount(item) && limits.maxCount) {
		color = (nameColor == TEXT_COLOR_SYSTEM_GRAY) ? TEXT_COLOR_SYSTEM_GRAY : TEXT_COLOR_SYSTEM_GOLD;
		PutSpecialChar(mapOut + 13, color, TEXT_SPECIAL_PLUS);
		PutNumberOrBlank(mapOut + 14, color, GetItemForgeCount(item));
	}
	else if(limits.maxCount == 0) {
		color = (nameColor == TEXT_COLOR_SYSTEM_GRAY) ? TEXT_COLOR_SYSTEM_GRAY : TEXT_COLOR_SYSTEM_WHITE;
		PutSpecialChar(mapOut + 12, color, TEXT_SPECIAL_SLASH);

		color = (nameColor != TEXT_COLOR_SYSTEM_GRAY) ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY;
		PutNumberOrBlank(mapOut + 11, color, GetItemUses(item));
		PutNumberOrBlank(mapOut + 14, color, GetItemMaxUses(item));
	}
#endif

    PutText(text, mapOut + 2);

    DrawIcon(mapOut, GetItemIconId(item), 0x4000);
}

LYN_REPLACE_CHECK(GetItemAfterUse);
u16 GetItemAfterUse(int item)
{
    if (GetItemAttributes(item) & IA_UNBREAKABLE)
        return item;

#if CHAX
    if (IsDuraItem(item))
        return 0;
#endif

#ifdef CONFIG_FORGING
    struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
    if(limits.maxCount) {
        return item; // items that have a nonzero forge count don't lose uses
    }
#endif

    item -= (1 << 8);
    if (item < (1 << 8))
        return 0;

    return item;
}

LYN_REPLACE_CHECK(MakeNewItem);
int MakeNewItem(int item) {
    int uses = GetItemMaxUses(item);

    if (GetItemAttributes(item) & IA_UNBREAKABLE)
        uses = 0;
	
#ifdef CONFIG_FORGING
	struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
	if(limits.maxCount) {
		uses = 0;
	}
#endif

    return (uses << 8) + GetItemIndex(item);
}

/*
** This line may complain about the same function being hooked in Vesly's debugger
** but as far as I can tell, the debugger doesn't use the shop, so it's fine.
*/
LYN_REPLACE_CHECK(StartShopScreen);
void StartShopScreen(struct Unit * unit, const u16 * inventory, u8 shopType, ProcPtr parent)
{
    struct ProcShop * proc;
    const u16 * shopItems;
    int i;

    EndPlayerPhaseSideWindows();

    if (parent)
        proc = Proc_StartBlocking(gProcScr_Shop, parent);
    else
        proc = Proc_Start(gProcScr_Shop, PROC_TREE_3);

    proc->shopType = shopType;
    proc->unit = unit;

    shopItems = gDefaultShopInventory;
    
    /*
    ** If the custom inventory passed in as a parameter isn't 0,
    ** then set shopItems to that inventory array
    */
    if (inventory != 0)
        shopItems = inventory;

#if CHAX
    for (i = 0; i < SHOP_ITEMS_MAX_AMT; i++)
    {
        u16 _item = *shopItems++;

        // Check if it's a skill scroll
        if (IsDuraItem(_item))
            proc->shopItems[i] = _item;
        else
            proc->shopItems[i] = MakeNewItem(_item);
    }
#else
    for (i = 0; i <= SHOP_ITEMS_MAX_AMT; i++)
        proc->shopItems[i] = MakeNewItem(*shopItems++);
#endif

    UpdateShopItemCounts(proc);
}

LYN_REPLACE_CHECK(RefreshUnitInventoryInfoWindow);
void RefreshUnitInventoryInfoWindow(struct Unit* unit) {
    int i;
    int xPos;
    int itemCount;

    struct UnitInfoWindowProc* proc;

    itemCount = GetUnitItemCount(unit);

    xPos = GetUnitInfoWindowX(unit, 0xd);

    proc = UnitInfoWindow_DrawBase(0, unit, xPos, 0, 0xd, itemCount != 0 ? itemCount : 1);

    if (itemCount == 0) {
        int offset;

        ClearText(proc->lines + 0);
        Text_InsertDrawString(proc->lines + 0, 0, 1, GetStringFromIndex(0x5a8)); // TODO: msgid "Nothing[.]"

        offset = TILEMAP_INDEX(xPos+3, 0+3);
        PutText(proc->lines + 0, gBG0TilemapBuffer + offset);

        return;
    }

    for (i = 0; i < itemCount; i++) {
        int yPos = 0 + i * 2 + 3;

        int item = unit->items[i];

        ClearText(proc->lines + i);
        Text_DrawString(proc->lines + i, GetItemName(item));

        PutText(proc->lines + i, gBG0TilemapBuffer + TILEMAP_INDEX(xPos+3, yPos));

#ifdef CONFIG_FORGING
		struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
		if(GetItemForgeCount(item) && limits.maxCount) {
			PutSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 10, yPos), TEXT_COLOR_SYSTEM_GOLD, TEXT_SPECIAL_PLUS);
			PutNumberOrBlank(gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 11, yPos), TEXT_COLOR_SYSTEM_GOLD, GetItemForgeCount(item));
		}
		else if(limits.maxCount == 0) {
			PutNumberOrBlank(gBG0TilemapBuffer + TILEMAP_INDEX(xPos+11, yPos), 2, GetItemUses(item));			
		}
#endif
        DrawIcon(gBG0TilemapBuffer + TILEMAP_INDEX(xPos+1, yPos), GetItemIconId(item), 0x4000);
    }
    return;
}
LYN_REPLACE_CHECK(RefreshHammerneUnitInfoWindow);
void RefreshHammerneUnitInfoWindow(struct Unit* unit) {
    int i;
    int color;
    int xPos;
    int itemCount;
    struct UnitInfoWindowProc* proc;

    itemCount = GetUnitItemCount(unit);

    xPos = GetUnitInfoWindowX(unit, 0x10);

    proc = UnitInfoWindow_DrawBase(0, unit, xPos, 0, 0x10, itemCount);

    for (i = 0; i < itemCount; i++) {
        int yPos = 0 + i * 2 + 3;

        int item = unit->items[i];

        color = IsItemHammernable(item) ? 0 : 1;

        ClearText(proc->lines + i);

        Text_SetColor(proc->lines + i, color);
        Text_DrawString(proc->lines + i, GetItemName(item));

        PutText(proc->lines + i, gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 3, yPos));
		
#ifdef CONFIG_FORGING        
		struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
		if(GetItemForgeCount(item) && limits.maxCount) {
			PutSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 13, yPos), IsItemHammernable(item) ? TEXT_COLOR_SYSTEM_GOLD : TEXT_COLOR_SYSTEM_GRAY, TEXT_SPECIAL_PLUS);
			PutNumberOrBlank(gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 14, yPos), IsItemHammernable(item) ? TEXT_COLOR_SYSTEM_GOLD : TEXT_COLOR_SYSTEM_GRAY, GetItemForgeCount(item));
		}
		else if(limits.maxCount == 0) {
			PutSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 12, yPos), color, TEXT_SPECIAL_SLASH);

			color = IsItemHammernable(item) ? 2 : 1;

			PutNumberOrBlank(gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 11, yPos), color, GetItemUses(item));
			PutNumberOrBlank(gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 14, yPos), color, GetItemMaxUses(item));	
		}
#else
        PutSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 12, yPos), color, TEXT_SPECIAL_SLASH);

		color = IsItemHammernable(item) ? 2 : 1;

		PutNumberOrBlank(gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 11, yPos), color, GetItemUses(item));
		PutNumberOrBlank(gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 14, yPos), color, GetItemMaxUses(item));	
#endif
		DrawIcon(gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 1, yPos), GetItemIconId(item), 0x4000);
    }

    BG_EnableSyncByMask(BG0_SYNC_BIT | BG1_SYNC_BIT);

    return;
}


LYN_REPLACE_CHECK(PrepUnit_DrawUnitItems);
void PrepUnit_DrawUnitItems(struct Unit *unit)
{
    int i, cnt;

    ResetIconGraphics_();
    TileMap_FillRect(TILEMAP_LOCATED(gBG0TilemapBuffer, 1, 5), 0xB, 0xA, 0);

    cnt = GetUnitItemCount(unit);
    for (i = 0; i < cnt; i++) {
        int item = unit->items[i];

        DrawIcon(
            TILEMAP_LOCATED(gBG0TilemapBuffer, 1, 5 + 2 * i),
            GetItemIconId(item),
            TILEREF(0, BGPAL_ICONS)
        );

        ClearText(&gPrepUnitTexts[i + 0xE]);

        PutDrawText(
            &gPrepUnitTexts[i + 0xE],
            TILEMAP_LOCATED( gBG0TilemapBuffer, 3, 5 + 2 * i),
            IsItemDisplayUsable(unit, item)
                ? TEXT_COLOR_SYSTEM_WHITE
                : TEXT_COLOR_SYSTEM_GRAY,
            0, 0, GetItemName(item)
        );

#ifdef CONFIG_FORGING
        struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
		if(GetItemForgeCount(item) && limits.maxCount) {
			PutSpecialChar(TILEMAP_LOCATED(gBG0TilemapBuffer, 10, 5 + 2 * i), IsItemDisplayUsable(unit, item) ? TEXT_COLOR_SYSTEM_GOLD : TEXT_COLOR_SYSTEM_GRAY, TEXT_SPECIAL_PLUS);
			PutNumberOrBlank(TILEMAP_LOCATED(gBG0TilemapBuffer, 11, 5 + 2 * i), IsItemDisplayUsable(unit, item) ? TEXT_COLOR_SYSTEM_GOLD : TEXT_COLOR_SYSTEM_GRAY, GetItemForgeCount(item));
		}
		else if(limits.maxCount == 0) {
			PutNumberOrBlank(
				TILEMAP_LOCATED(gBG0TilemapBuffer, 11, 5 + 2 * i),
				IsItemDisplayUsable(unit, item)
					? TEXT_COLOR_SYSTEM_BLUE
					: TEXT_COLOR_SYSTEM_GRAY,
				GetItemUses(item)
			);			
		}
#else
        PutNumberOrBlank(
            TILEMAP_LOCATED(gBG0TilemapBuffer, 11, 5 + 2 * i),
            IsItemDisplayUsable(unit, item)
                ? TEXT_COLOR_SYSTEM_BLUE
                : TEXT_COLOR_SYSTEM_GRAY,
            GetItemUses(item)
        );
#endif

    }

    BG_EnableSyncByMask(BG0_SYNC_BIT);
}