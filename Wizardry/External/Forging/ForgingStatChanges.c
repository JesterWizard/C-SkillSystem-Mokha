// int GetItemForgeCount(int item) {
// 	struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
// 	if(limits.maxCount == 0) {
// 		return 0;
// 	}
// 	return ITEM_USES(item);
// }

// int GetItemForgeCost(int item) {
// 	struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
// 	int count = GetItemForgeCount(item);
// 	if(count >= limits.maxCount) {
// 		return 0;
// 	}
// 	return (count + 1) * limits.baseCost;
// }

// bool IsItemForgeable(int item) {
// 	struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
// 	//int count = GetItemForgeCount(item);
	
// 	if(GetItemForgeCount(item) >= limits.maxCount) {
// 		return false;
// 	}
	
// 	if(GetItemForgeCost(item) > (int)GetPartyGoldAmount()) {
// 		return false;
// 	}
	
// 	return true;
// }

// int GetItemMight(int item) {
// 	int result = GetItemData(ITEM_INDEX(item))->might;
// 	struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
// 	struct ForgeBonuses bonuses = gForgeBonuses[limits.forgeIndex];
		
// 	int forgeMt = (GetItemForgeCount(item) * bonuses.mtBonus) / 6;
		
// 	result += forgeMt;
//     return result;
// }

// int GetItemHit(int item) {
// 	int result = GetItemData(ITEM_INDEX(item))->hit;
// 	struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
// 	struct ForgeBonuses bonuses = gForgeBonuses[limits.forgeIndex];
		
// 	int forgeHit = (GetItemForgeCount(item) * bonuses.hitBonus) / 6;
		
// 	result += forgeHit;		
//     return result;
// }

// int GetItemCrit(int item) {
// 	int result = GetItemData(ITEM_INDEX(item))->crit;

// 	struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
// 	struct ForgeBonuses bonuses = gForgeBonuses[limits.forgeIndex];
		
// 	int forgeCrit = (GetItemForgeCount(item) * bonuses.critBonus) / 6;
		
// 	result += forgeCrit;
//     return result;
// }

// int GetItemWeight(int item) {
// 	int result = GetItemData(ITEM_INDEX(item))->weight;
// 	struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
// 	struct ForgeBonuses bonuses = gForgeBonuses[limits.forgeIndex];
		
// 	int forgeWt = (GetItemForgeCount(item) * bonuses.wtBonus) / 6;
		
// 	result += forgeWt;
//     return result;
// }

// u16 GetItemAfterUse(int item) {
//     if (GetItemAttributes(item) & IA_UNBREAKABLE)
//         return item; // unbreakable items don't loose uses!
	
// 	struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
// 	if(limits.maxCount) {
// 		return item; // items that have a nonzero forge count don't lose uses
// 	}

//     item -= (1 << 8); // lose one use

//     if (item < (1 << 8))
//         return 0; // return no item if uses < 0

//     return item; // return used item
// }

// int MakeNewItem(int item) {
//     int uses = GetItemMaxUses(item);

//     if (GetItemAttributes(item) & IA_UNBREAKABLE)
//         uses = 0;
	
// 	struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
// 	if(limits.maxCount) {
// 		uses = 0;
// 	}

//     return (uses << 8) + GetItemIndex(item);
// }