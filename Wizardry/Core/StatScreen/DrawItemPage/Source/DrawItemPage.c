#include <common-chax.h>
#include <list-verify.h>
#include <battle-system.h>
#include <stat-screen.h>
#include <shield.h>

void ResetItemPageLists(void)
{
	memset(&gItemPageList, 0, sizeof(gItemPageList));
}

static void UpdateItemPageListExt(struct Unit *unit, struct ItemPageList *list)
{
	int i, item, cnt = 0;

	/**
	 * Unit items
	 */
	for (i = 0; i < UNIT_ITEM_COUNT; i++) {
		struct ItemPageEnt *ent;

		item = unit->items[i];
		if (item == ITEM_NONE)
			break;

		ent = &list->ent[cnt++];
		if (cnt >= CHAX_ITEM_PAGE_AMT)
			return;

		ent->item = item;
		ent->slot = i;

		if ((unit->state & US_DROP_ITEM) && ((GetUnitItemCount(unit) - 1) == i))
			ent->color = TEXT_COLOR_SYSTEM_GREEN;
		else
			ent->color = IsItemDisplayUsable(unit, item)
					   ? TEXT_COLOR_SYSTEM_WHITE
					   : TEXT_COLOR_SYSTEM_GRAY;
	}
}

void UpdateItemPageList(struct Unit *unit, struct ItemPageList *list)
{
	ResetItemPageLists();
	UpdateItemPageListExt(unit, list);
	WriteUnitList(unit, &list->header);
}

struct ItemPageList *GetUnitItemPageList(struct Unit *unit)
{
	struct ItemPageList *list = &gItemPageList;

	if (!JudgeUnitList(unit, &list->header))
		UpdateItemPageList(unit, list);

	return list;
}

static void DrawItemLineDefault(const struct ItemPageEnt *ent, int line)
{
	DrawItemStatScreenLine(
		&gStatScreen.text[STATSCREEN_TEXT_ITEM0 + line],
		ent->item,
		ent->color,
		gUiTmScratchA + TILEMAP_INDEX(1, 1 + line * 2)
	);
}

static void DrawItemEquipLine(int slot)
{
	int i, line;
	struct Unit *unit = gStatScreen.unit;
	struct ItemPageList *list = GetUnitItemPageList(unit);

	line = -1;

	for (i = 0; i < CHAX_ITEM_PAGE_AMT; i++) {
		if (list->ent[i].slot == slot) {
			line = i;
			break;
		}
	}

	if (line >= 0) {
		PutSpecialChar(
			gUiTmScratchA + TILEMAP_INDEX(16, 1 + line * 2),
			TEXT_COLOR_SYSTEM_WHITE, TEXT_SPECIAL_35);

		CallARM_FillTileRect(
			gUiTmScratchC + TILEMAP_INDEX(1, 2 + line * 2),
			gpTSA_ItemEquipLine, TILEREF(0x40, STATSCREEN_BGPAL_3));
	}
}

static void DrawItemPageSubfix(int slot)
{
	int i, weapon;
	const char *str;
	struct Unit *unit = gStatScreen.unit;

	Decompress(gpTSA_ItemPageSubfix, gGenericBuffer);
	CallARM_FillTileRect(
		gUiTmScratchC + TILEMAP_INDEX(1, 11),
		gGenericBuffer, TILEREF(0x40, STATSCREEN_BGPAL_3));

	DisplayTexts(gpPage1TextInfo);

	weapon = GetItemFromSlot(unit, slot);

	PutNumberOrBlank(
		gUiTmScratchA + TILEMAP_INDEX(8,  13),
		TEXT_COLOR_SYSTEM_BLUE, gBattleActor.battleAttack);

	PutNumberOrBlank(
		gUiTmScratchA + TILEMAP_INDEX(8,  15),
		TEXT_COLOR_SYSTEM_BLUE, gBattleActor.battleHitRate);

	PutNumberOrBlank(
		gUiTmScratchA + TILEMAP_INDEX(15, 13),
		TEXT_COLOR_SYSTEM_BLUE, gBattleActor.battleCritRate);

	PutNumberOrBlank(
		gUiTmScratchA + TILEMAP_INDEX(15, 15),
		TEXT_COLOR_SYSTEM_BLUE, gBattleActor.battleAvoidRate);

	str = GetItemDisplayRangeString(weapon);
	Text_InsertDrawString(
		&gStatScreen.text[STATSCREEN_TEXT_BSRANGE],
		55 - GetStringTextLen(str),
		TEXT_COLOR_SYSTEM_BLUE, str);

	for (i = 0; i < 8; ++i) {
		gUiTmScratchA[TILEMAP_INDEX(1 + i, 11)] = TILEREF(0x60 + i, STATSCREEN_BGPAL_7);
		gUiTmScratchA[TILEMAP_INDEX(1 + i, 12)] = TILEREF(0x68 + i, STATSCREEN_BGPAL_7);
	}
}

LYN_REPLACE_CHECK(DisplayPage1);
void DisplayPage1(void)
{
	int i, slot;
	struct Unit *unit = gStatScreen.unit;
	struct ItemPageList *list = GetUnitItemPageList(unit);

	for (i = 0; i < CHAX_ITEM_PAGE_AMT; i++) {
		struct ItemPageEnt *ent = &list->ent[i];

		if (ent->item == ITEM_NONE)
			break;

		switch (ent->slot) {
		case 0 ... 4:
			DrawItemLineDefault(ent, i);
			break;

		default:
			break;
		}
	}

	slot = GetUnitEquippedWeaponSlot(unit);
	DrawItemEquipLine(slot);
	DrawItemPageSubfix(slot);

#ifdef CONFIG_INSTALL_KERNEL_SHIELD
	if (gpKernelDesigerConfig->shield_en)
		DrawItemPage_ShieldEquipLine();
#endif
}

/**
 * Helpbox
 */
LYN_REPLACE_CHECK(HbRedirect_SSItem);
void HbRedirect_SSItem(struct HelpBoxProc* proc)
{
    if (!gStatScreen.unit->items[0])
        TryRelocateHbLeft(proc);

    if (!gStatScreen.unit->items[proc->info->mid])
    {
        if (proc->moveKey == 0 || proc->moveKey == DPAD_RIGHT || proc->moveKey == DPAD_UP)
            TryRelocateHbUp(proc);
        else if (proc->moveKey == DPAD_DOWN)
            TryRelocateHbDown(proc);
    }
}

LYN_REPLACE_CHECK(HbPopulate_SSItem);
void HbPopulate_SSItem(struct HelpBoxProc *proc)
{
    int item = gStatScreen.unit->items[proc->info->mid];

    proc->item = item;
    proc->mid  = GetItemDescId(item);
}
