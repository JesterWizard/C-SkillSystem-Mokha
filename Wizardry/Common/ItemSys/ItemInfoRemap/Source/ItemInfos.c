#include "common-chax.h"
#include "item-sys.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "bmitem.h"

LYN_REPLACE_CHECK(GetItemData);
const struct ItemData * GetItemData(int itemIndex) {
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

    item -= (1 << 8);
    if (item < (1 << 8))
        return 0;

    return item;
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