// void DrawItemMenuLine(struct Text* text, int item, s8 isUsable, u16* mapOut) {
//     Text_SetParams(text, 0, (isUsable ? TEXT_COLOR_SYSTEM_WHITE : TEXT_COLOR_SYSTEM_GRAY));
//     Text_DrawString(text, GetItemName(item));
//     PutText(text, mapOut + 2);
	
// 	struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
// 	if(GetItemForgeCount(item) && limits.maxCount) {
// 		PutSpecialChar(mapOut + 10, isUsable ? TEXT_COLOR_SYSTEM_GOLD : TEXT_COLOR_SYSTEM_GRAY, TEXT_SPECIAL_PLUS);
// 		PutNumberOrBlank(mapOut + 11, isUsable ? TEXT_COLOR_SYSTEM_GOLD : TEXT_COLOR_SYSTEM_GRAY, GetItemForgeCount(item));
// 	}
// 	else if(limits.maxCount == 0) {
// 		PutNumberOrBlank(mapOut + 11, isUsable ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY, GetItemUses(item));		
// 	}
//     DrawIcon(mapOut, GetItemIconId(item), 0x4000);
// }

// void DrawItemMenuLineLong(struct Text* text, int item, s8 isUsable, u16* mapOut) {
//     Text_SetParams(text, 0, (isUsable ? TEXT_COLOR_SYSTEM_WHITE : TEXT_COLOR_SYSTEM_GRAY));
//     Text_DrawString(text, GetItemName(item));
//     PutText(text, mapOut + 2);
	
// 	struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
// 	if(GetItemForgeCount(item) && limits.maxCount) {
// 		PutSpecialChar(mapOut + 12, isUsable ? TEXT_COLOR_SYSTEM_GOLD : TEXT_COLOR_SYSTEM_GRAY, TEXT_SPECIAL_PLUS);
// 		PutNumberOrBlank(mapOut + 13, isUsable ? TEXT_COLOR_SYSTEM_GOLD : TEXT_COLOR_SYSTEM_GRAY, GetItemForgeCount(item));
// 	}
// 	else if(limits.maxCount == 0) {
// 		PutNumberOrBlank(mapOut + 10, isUsable ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY, GetItemUses(item));
// 		PutNumberOrBlank(mapOut + 13, isUsable ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY, GetItemMaxUses(item));
// 		PutSpecialChar(mapOut + 11, isUsable ? TEXT_COLOR_SYSTEM_WHITE : TEXT_COLOR_SYSTEM_GRAY, TEXT_SPECIAL_SLASH);		
// 	}
//     DrawIcon(mapOut, GetItemIconId(item), 0x4000);
// }

// void DrawItemMenuLineNoColor(struct Text* text, int item, u16* mapOut) {
//     Text_SetCursor(text, 0);
//     Text_DrawString(text, GetItemName(item));
//     PutText(text, mapOut + 2);
	
// 	struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
// 	if(GetItemForgeCount(item) && limits.maxCount) {
// 		PutSpecialChar(mapOut + 10, Text_GetColor(text), TEXT_SPECIAL_PLUS);
// 		PutNumberOrBlank(mapOut + 11, Text_GetColor(text), GetItemForgeCount(item));
// 	}
// 	else if(limits.maxCount == 0) {
// 		PutNumberOrBlank(mapOut + 11, Text_GetColor(text), GetItemUses(item));		
// 	}
//     DrawIcon(mapOut, GetItemIconId(item), 0x4000);
// }

// void DrawItemStatScreenLine(struct Text* text, int item, int nameColor, u16* mapOut) {
//     int color;
//     ClearText(text);
//     color = nameColor;
//     Text_SetColor(text, color);
//     Text_DrawString(text, GetItemName(item));
	
// 	struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
// 	if(GetItemForgeCount(item) && limits.maxCount) {
// 		color = (nameColor == TEXT_COLOR_SYSTEM_GRAY) ? TEXT_COLOR_SYSTEM_GRAY : TEXT_COLOR_SYSTEM_GOLD;
// 		PutSpecialChar(mapOut + 13, color, TEXT_SPECIAL_PLUS);
// 		PutNumberOrBlank(mapOut + 14, color, GetItemForgeCount(item));
// 	}
// 	else if(limits.maxCount == 0) {
// 		color = (nameColor == TEXT_COLOR_SYSTEM_GRAY) ? TEXT_COLOR_SYSTEM_GRAY : TEXT_COLOR_SYSTEM_WHITE;
// 		PutSpecialChar(mapOut + 12, color, TEXT_SPECIAL_SLASH);

// 		color = (nameColor != TEXT_COLOR_SYSTEM_GRAY) ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY;
// 		PutNumberOrBlank(mapOut + 11, color, GetItemUses(item));
// 		PutNumberOrBlank(mapOut + 14, color, GetItemMaxUses(item));
// 	}
// 	PutText(text, mapOut + 2);
//     DrawIcon(mapOut, GetItemIconId(item), 0x4000);
// }

// void RefreshUnitInventoryInfoWindow(struct Unit* unit) {
//     int i;
//     int xPos;
//     int itemCount;

//     struct UnitInfoWindowProc* proc;

//     itemCount = GetUnitItemCount(unit);

//     xPos = GetUnitInfoWindowX(unit, 0xd);

//     proc = UnitInfoWindow_DrawBase(0, unit, xPos, 0, 0xd, itemCount != 0 ? itemCount : 1);

//     if (itemCount == 0) {
//         int offset;

//         ClearText(proc->lines + 0);
//         Text_InsertDrawString(proc->lines + 0, 0, 1, GetStringFromIndex(0x5a8)); // TODO: msgid "Nothing[.]"

//         offset = TILEMAP_INDEX(xPos+3, 0+3);
//         PutText(proc->lines + 0, gBG0TilemapBuffer + offset);

//         return;
//     }

//     for (i = 0; i < itemCount; i++) {
//         int yPos = 0 + i * 2 + 3;

//         int item = unit->items[i];

//         ClearText(proc->lines + i);
//         Text_DrawString(proc->lines + i, GetItemName(item));

//         PutText(proc->lines + i, gBG0TilemapBuffer + TILEMAP_INDEX(xPos+3, yPos));
// 		struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
// 		if(GetItemForgeCount(item) && limits.maxCount) {
// 			PutSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 10, yPos), TEXT_COLOR_SYSTEM_GOLD, TEXT_SPECIAL_PLUS);
// 			PutNumberOrBlank(gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 11, yPos), TEXT_COLOR_SYSTEM_GOLD, GetItemForgeCount(item));
// 		}
// 		else if(limits.maxCount == 0) {
// 			PutNumberOrBlank(gBG0TilemapBuffer + TILEMAP_INDEX(xPos+11, yPos), 2, GetItemUses(item));			
// 		}
//         DrawIcon(gBG0TilemapBuffer + TILEMAP_INDEX(xPos+1, yPos), GetItemIconId(item), 0x4000);
//     }
//     return;
// }

// void RefreshUnitStealInventoryInfoWindow(struct Unit* unit) {
//     int i;
//     int itemCount;
//     int xPos;
//     struct UnitInfoWindowProc* proc;

//     itemCount = GetUnitItemCount(unit);

//     xPos = GetUnitInfoWindowX(unit, 0xd);

//     proc = UnitInfoWindow_DrawBase(0, unit, xPos, 0, 0xd, itemCount);

//     for (i = 0; i < itemCount; i++) {
//         int yPos = 0 + i * 2 + 3;

//         int item = unit->items[i];
//         s8 stealable = IsItemStealable(item);

//         ClearText(proc->lines + i);

//         Text_SetColor(proc->lines + i, stealable ? 0 : 1);
//         Text_DrawString(proc->lines + i, GetItemName(item));

//         PutText(proc->lines + i, gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 3, yPos));
// 		struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
// 		if(GetItemForgeCount(item) && limits.maxCount) {
// 			PutSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 10, yPos), stealable ? TEXT_COLOR_SYSTEM_GOLD : TEXT_COLOR_SYSTEM_GRAY, TEXT_SPECIAL_PLUS);
// 			PutNumberOrBlank(gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 11, yPos), stealable ? TEXT_COLOR_SYSTEM_GOLD : TEXT_COLOR_SYSTEM_GRAY, GetItemForgeCount(item));
// 		}
// 		else if(limits.maxCount == 0) {
// 			PutNumberOrBlank(gBG0TilemapBuffer + TILEMAP_INDEX(xPos+11, yPos), stealable ? 2 : 1, GetItemUses(item));			
// 		}
//         DrawIcon(gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 1, yPos), GetItemIconId(item), 0x4000);
//     }

//     return;
// }

////! FE8U = 0x08034E5C
// void RefreshHammerneUnitInfoWindow(struct Unit* unit) {
//     int i;
//     int color;
//     int xPos;
//     int itemCount;
//     struct UnitInfoWindowProc* proc;

//     itemCount = GetUnitItemCount(unit);

//     xPos = GetUnitInfoWindowX(unit, 0x10);

//     proc = UnitInfoWindow_DrawBase(0, unit, xPos, 0, 0x10, itemCount);

//     for (i = 0; i < itemCount; i++) {
//         int yPos = 0 + i * 2 + 3;

//         int item = unit->items[i];

//         color = IsItemHammernable(item) ? 0 : 1;

//         ClearText(proc->lines + i);

//         Text_SetColor(proc->lines + i, color);
//         Text_DrawString(proc->lines + i, GetItemName(item));

//         PutText(proc->lines + i, gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 3, yPos));
		
// 		struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
// 		if(GetItemForgeCount(item) && limits.maxCount) {
// 			PutSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 13, yPos), IsItemHammernable(item) ? TEXT_COLOR_SYSTEM_GOLD : TEXT_COLOR_SYSTEM_GRAY, TEXT_SPECIAL_PLUS);
// 			PutNumberOrBlank(gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 14, yPos), IsItemHammernable(item) ? TEXT_COLOR_SYSTEM_GOLD : TEXT_COLOR_SYSTEM_GRAY, GetItemForgeCount(item));
// 		}
// 		else if(limits.maxCount == 0) {
// 			PutSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 12, yPos), color, TEXT_SPECIAL_SLASH);

// 			color = IsItemHammernable(item) ? 2 : 1;

// 			PutNumberOrBlank(gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 11, yPos), color, GetItemUses(item));
// 			PutNumberOrBlank(gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 14, yPos), color, GetItemMaxUses(item));	
// 		}
// 		DrawIcon(gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 1, yPos), GetItemIconId(item), 0x4000);
//     }

//     BG_EnableSyncByMask(BG0_SYNC_BIT | BG1_SYNC_BIT);

//     return;
// }

// void DrawPrepScreenItems(u16 * tm, struct Text* th, struct Unit* unit, u8 checkPrepUsability) {
//     s8 isUsable;
//     int i;
//     int itemCount;

//     TileMap_FillRect(tm, 11, 9, 0);

//     itemCount = GetUnitItemCount(unit);

//     for (i = 0; i < itemCount; i++) {
//         int item = unit->items[i];

//         if (checkPrepUsability != 0) {
//             isUsable = CanUnitUseItemPrepScreen(unit, item);
//         } else {
//             isUsable = IsItemDisplayUsable(unit, item);
//         }

//         ClearText(th);
//         PutDrawText(
//             th,
//             tm + i * 0x40 + 2,
//             !isUsable ? TEXT_COLOR_SYSTEM_GRAY : TEXT_COLOR_SYSTEM_WHITE,
//             0,
//             0,
//             GetItemName(item)
//         );
		
// 		struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
// 		if(GetItemForgeCount(item) && limits.maxCount) {
// 			PutSpecialChar(tm + i * 0x40 + 0xA, isUsable ? TEXT_COLOR_SYSTEM_GOLD : TEXT_COLOR_SYSTEM_GRAY, TEXT_SPECIAL_PLUS);
// 			PutNumberOrBlank(tm + i * 0x40 + 0xB, isUsable ? TEXT_COLOR_SYSTEM_GOLD : TEXT_COLOR_SYSTEM_GRAY, GetItemForgeCount(item));
// 		}
// 		else if(limits.maxCount == 0) {
// 			PutNumberOrBlank(tm + i * 0x40 + 0xB, isUsable ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY, GetItemUses(item));
// 		}
//         DrawIcon(tm + i * 0x40, GetItemIconId(item), 0x4000);

//         th++;
//     }

//     return;
// }

// void sub_809D300(struct Text * textBase, u16 * tm, int yLines, struct Unit * unit)
// {
//     int i;

//     TileMap_FillRect(tm, 12, 31, 0);

//     if (gUnknown_02012F56 == 0) {
//         ClearText(textBase);
//         Text_InsertDrawString(textBase, 0, 1, GetStringFromIndex(0x5a8)); // TODO: msgid "Nothing[.]"
//         PutText(textBase, tm + 3);
//         return;
//     }

//     for (i = yLines; (i < yLines + 7) && (i < gUnknown_02012F56); i++) {
//         struct Text* th = textBase + (i & 7);
//         int item = gPrepScreenItemList[i].item;
//         int unusable = !IsItemDisplayUsable(unit, item);

//         ClearText(th);

//         Text_InsertDrawString(
//             th,
//             0,
//             unusable,
//             GetItemName(item)
//         );

//         DrawIcon(tm + TILEMAP_INDEX(1, i*2 & 0x1f), GetItemIconId(item), 0x4000);

//         PutText(th, tm + TILEMAP_INDEX(3, i*2 & 0x1f));
		
// 		struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
// 		if(GetItemForgeCount(item) && limits.maxCount) {
// 			PutSpecialChar(tm + TILEMAP_INDEX(11, i*2 & 0x1f), !unusable ? TEXT_COLOR_SYSTEM_GOLD : TEXT_COLOR_SYSTEM_GRAY, TEXT_SPECIAL_PLUS);
// 			PutNumberOrBlank(tm + TILEMAP_INDEX(12, i*2 & 0x1f), !unusable ? TEXT_COLOR_SYSTEM_GOLD : TEXT_COLOR_SYSTEM_GRAY, GetItemForgeCount(item));
// 		}
// 		else if(limits.maxCount == 0) {
// 		    PutNumberOrBlank(tm + TILEMAP_INDEX(12, i*2 & 0x1f), !unusable ? 2 : 1, GetItemUses(item));	
// 		}
//     }

//     return;
// }

// void sub_809D47C(struct Text * textBase, u16 * tm, int yLines, struct Unit * unit)
// {
//     if (gUnknown_02012F56 > yLines) {
//         int y = (yLines * 2) & 0x1f;
//         struct Text* th = textBase + (yLines & 7);
//         int item = gPrepScreenItemList[yLines].item;
//         int unusable = !IsItemDisplayUsable(unit, item);

//         int offset = TILEMAP_INDEX(0, y);
//         TileMap_FillRect(tm + offset, 12, 1, 0);

//         ClearText(th);
//         Text_InsertDrawString(th, 0, unusable, GetItemName(item));
//         DrawIcon(tm + offset + 1, GetItemIconId(item), 0x4000);
//         PutText(th, tm + offset + 3);
		
// 		struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
// 		if(GetItemForgeCount(item) && limits.maxCount) {
// 			PutSpecialChar(tm + offset + 11, !unusable ? TEXT_COLOR_SYSTEM_GOLD : TEXT_COLOR_SYSTEM_GRAY, TEXT_SPECIAL_PLUS);
// 			PutNumberOrBlank(tm + offset + 12, !unusable ? TEXT_COLOR_SYSTEM_GOLD : TEXT_COLOR_SYSTEM_GRAY,  GetItemForgeCount(item));
// 		}
// 		else if(limits.maxCount == 0) {
// 			PutNumberOrBlank(tm + offset + 12, !unusable ? 2 : 1,  GetItemUses(item));
// 		}
//     }
// }

// void PrepUnit_DrawUnitItems(struct Unit *unit)
// {
//     int i, cnt;

//     ResetIconGraphics_();
//     TileMap_FillRect(TILEMAP_LOCATED(gBG0TilemapBuffer, 1, 5), 0xB, 0xA, 0);

//     cnt = GetUnitItemCount(unit);
//     for (i = 0; i < cnt; i++) {
//         int item = unit->items[i];

//         DrawIcon(
//             TILEMAP_LOCATED(gBG0TilemapBuffer, 1, 5 + 2 * i),
//             GetItemIconId(item),
//             TILEREF(0, BGPAL_ICONS)
//         );

//         ClearText(&gPrepUnitTexts[i + 0xE]);

//         PutDrawText(
//             &gPrepUnitTexts[i + 0xE],
//             TILEMAP_LOCATED( gBG0TilemapBuffer, 3, 5 + 2 * i),
//             IsItemDisplayUsable(unit, item)
//                 ? TEXT_COLOR_SYSTEM_WHITE
//                 : TEXT_COLOR_SYSTEM_GRAY,
//             0, 0, GetItemName(item)
//         );
		
// 		struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
// 		if(GetItemForgeCount(item) && limits.maxCount) {
// 			PutSpecialChar(TILEMAP_LOCATED(gBG0TilemapBuffer, 10, 5 + 2 * i), IsItemDisplayUsable(unit, item) ? TEXT_COLOR_SYSTEM_GOLD : TEXT_COLOR_SYSTEM_GRAY, TEXT_SPECIAL_PLUS);
// 			PutNumberOrBlank(TILEMAP_LOCATED(gBG0TilemapBuffer, 11, 5 + 2 * i), IsItemDisplayUsable(unit, item) ? TEXT_COLOR_SYSTEM_GOLD : TEXT_COLOR_SYSTEM_GRAY, GetItemForgeCount(item));
// 		}
// 		else if(limits.maxCount == 0) {
// 			PutNumberOrBlank(
// 				TILEMAP_LOCATED(gBG0TilemapBuffer, 11, 5 + 2 * i),
// 				IsItemDisplayUsable(unit, item)
// 					? TEXT_COLOR_SYSTEM_BLUE
// 					: TEXT_COLOR_SYSTEM_GRAY,
// 				GetItemUses(item)
// 			);			
// 		}
//     }
//     BG_EnableSyncByMask(BG0_SYNC_BIT);
// }

// void sub_8099F7C(struct Text* th, u16 * tm, struct Unit* unit, u16 flags) {
//     int itemCount;
//     int i;

//     TileMap_FillRect(tm, 12, 20, 0);

//     if ((flags & 2) != 0) {
//         ResetIconGraphics();
//     }

//     if (!unit) {
//         return;
//     }

//     itemCount = GetUnitItemCount(unit);

//     for (i = 0; i < itemCount; th++, i++) {
//         u16 item = unit->items[i];

//         int isUnusable = ((flags & 4) != 0)
//             ? !CanUnitUseItemPrepScreen(unit, item)
//             : !IsItemDisplayUsable(unit, item);

//         if ((flags & 1) == 0) {
//             ClearText(th);
//             Text_SetColor(th, isUnusable);
//             Text_SetCursor(th, 0);
//             Text_DrawString(th, GetItemName(item));
//         }

//         DrawIcon(tm + i * 0x40, GetItemIconId(item), 0x4000);

//         PutText(th, tm + 2 + i * 0x40);
// 		struct ForgeLimits limits = gForgeLimits[GetItemIndex(item)];
// 		if(GetItemForgeCount(item) && limits.maxCount) {
// 			PutSpecialChar(tm + 10 + i * 0x40, !isUnusable ? TEXT_COLOR_SYSTEM_GOLD : TEXT_COLOR_SYSTEM_GRAY, TEXT_SPECIAL_PLUS);
// 			PutNumberOrBlank(tm + 11 + i * 0x40, !isUnusable ? TEXT_COLOR_SYSTEM_GOLD : TEXT_COLOR_SYSTEM_GRAY, GetItemForgeCount(item));
// 		}
// 		else if(limits.maxCount == 0) {
// 			PutNumberOrBlank(tm + 11 + i * 0x40, !isUnusable ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY, GetItemUses(item));
// 		}
//     }

//     return;
// }