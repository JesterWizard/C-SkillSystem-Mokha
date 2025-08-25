#include "common-chax.h"
#include "hooks.h"
#include "stat-screen.h"
#include "skill-system.h"
#include "constants/texts.h"
#include "bwl.h"
#include "gaiden-magic.h"
#include "utf8.h"

#define PAGE4_PINFO_MAX 8
#define WHITE_MAGIC_STARTING_TEXT_FIELD 15
#define BLACK_MAGIC_STARTING_TEXT_FIELD 29

static void WhiteMagicList(struct Unit * unit)
{

    PutDrawText(
        &gStatScreen.text[WHITE_MAGIC_STARTING_TEXT_FIELD],
        gUiTmScratchA + TILEMAP_INDEX(0x1, 0x1),
        TEXT_COLOR_SYSTEM_WHITE,
        0, 0,
        "White Magic");

#ifdef CONFIG_USE_GAIDEN_MAGIC
        struct GaidenMagicList *gmaglist = GetGaidenMagicList(unit);

		for (int i = 0; i < GAIDEN_MAGIC_LIST_LEN; i++) {
			u16 item = gmaglist->wmags[i];

			if (item == 0)
				break;

            DrawIcon(gUiTmScratchA + TILEMAP_INDEX(0x1, 0x3 + (i * 2)), GetItemIconId(item), 0x4000);

            PutDrawText(
                &gStatScreen.text[WHITE_MAGIC_STARTING_TEXT_FIELD + (i + 1)],
                gUiTmScratchA + TILEMAP_INDEX(0x3, 0x3 + (i * 2)),
                TEXT_COLOR_SYSTEM_GOLD,
                0, 0,
                Utf8ToNarrowFonts(GetItemName(item)));
		}
#endif
};

static void BlackMagicList(struct Unit * unit)
{
    PutDrawText(
        &gStatScreen.text[BLACK_MAGIC_STARTING_TEXT_FIELD],
        gUiTmScratchA + TILEMAP_INDEX(0x9, 0x1),
        TEXT_COLOR_SYSTEM_WHITE,
        0, 0,
        "Black Magic");

#ifdef CONFIG_USE_GAIDEN_MAGIC
        struct GaidenMagicList *gmaglist = GetGaidenMagicList(unit);

		for (int i = 0; i < GAIDEN_MAGIC_LIST_LEN; i++) {
			u16 item = gmaglist->bmags[i];

			if (item == 0)
				break;

            DrawIcon(gUiTmScratchA + TILEMAP_INDEX(0x9, 0x3 + (i * 2)), GetItemIconId(item), 0x4000);

            PutDrawText(
                &gStatScreen.text[BLACK_MAGIC_STARTING_TEXT_FIELD + (i +1)],
                gUiTmScratchA + TILEMAP_INDEX(0xB, 0x3 + (i * 2)),
                TEXT_COLOR_SYSTEM_GOLD,
                0, 0,
                Utf8ToNarrowFonts(GetItemName(item)));
		}
#endif
};

void DisplayMP(struct Unit * unit)
{
    struct NewBwl * bwl = GetNewBwl(UNIT_CHAR_ID(unit));
    
    PutDrawText(
        &gStatScreen.text[18],
        gUiTmScratchA + TILEMAP_INDEX(0x5, 0xE),
        TEXT_COLOR_SYSTEM_WHITE,
        0, 0,
        "MP:");

    PutNumber((gUiTmScratchA + TILEMAP_INDEX(0x8, 0xE)), TEXT_COLOR_SYSTEM_BLUE, bwl->currentMP);
    PutSpecialChar((gUiTmScratchA + TILEMAP_INDEX(0x9, 0xE)), TEXT_COLOR_SYSTEM_WHITE, TEXT_SPECIAL_SLASH);
    PutNumber((gUiTmScratchA + TILEMAP_INDEX(0xB, 0xE)), TEXT_COLOR_SYSTEM_BLUE, bwl->maxMP);
};

static void DrawPage5MagicList(void)
{
    struct Unit * unit = gStatScreen.unit;

    WhiteMagicList(unit);
    BlackMagicList(unit);
    DisplayMP(unit);
};

void DrawPage5Rework(void)
{
    int i;
    for (i = 15; i < 20; i++)
        ClearText(&gStatScreen.text[i]);

    for (i = 29; i < 34; i++)
        ClearText(&gStatScreen.text[i]);

    DrawPage5MagicList();
}
