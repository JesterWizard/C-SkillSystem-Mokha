#include "common-chax.h"
#include "hooks.h"
#include "stat-screen.h"
#include "skill-system.h"
#include "constants/texts.h"
#include "bwl.h"

#define PAGE4_PINFO_MAX 8

static void WhiteMagicList(void)
{

    PutDrawText(
        &gStatScreen.text[15],
        gUiTmScratchA + TILEMAP_INDEX(0x1, 0x1),
        TEXT_COLOR_SYSTEM_WHITE,
        0, 0,
        "White Magic");

    /* 1st */
    DrawIcon(gUiTmScratchA + TILEMAP_INDEX(0x1, 0x3), GetItemIconId(ITEM_STAFF_HEAL), 0x4000);
    PutDrawText(
        &gStatScreen.text[16],
        gUiTmScratchA + TILEMAP_INDEX(0x3, 0x3),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetItemName(ITEM_STAFF_HEAL));

    /* 2nd */
    DrawIcon(gUiTmScratchA + TILEMAP_INDEX(0x1, 0x5), GetItemIconId(ITEM_STAFF_MEND), 0x4000);
    PutDrawText(
        &gStatScreen.text[17],
        gUiTmScratchA + TILEMAP_INDEX(0x3, 0x5),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetItemName(ITEM_STAFF_MEND));

    /* 3rd */
    DrawIcon(gUiTmScratchA + TILEMAP_INDEX(0x1, 0x7), GetItemIconId(ITEM_STAFF_RECOVER), 0x4000);
    PutDrawText(
        &gStatScreen.text[18],
        gUiTmScratchA + TILEMAP_INDEX(0x3, 0x7),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetItemName(ITEM_STAFF_RECOVER));
};

static void BlackMagicList(void)
{
    PutDrawText(
        &gStatScreen.text[29],
        gUiTmScratchA + TILEMAP_INDEX(0x9, 0x1),
        TEXT_COLOR_SYSTEM_WHITE,
        0, 0,
        "Black Magic");

    /* 1st */
    DrawIcon(gUiTmScratchA + TILEMAP_INDEX(0x9, 0x3), GetItemIconId(ITEM_ANIMA_FIRE), 0x4000);
    PutDrawText(
        &gStatScreen.text[30],
        gUiTmScratchA + TILEMAP_INDEX(0xB, 0x3),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetItemName(ITEM_ANIMA_FIRE));

    /* 2st */
    DrawIcon(gUiTmScratchA + TILEMAP_INDEX(0x9, 0x5), GetItemIconId(ITEM_ANIMA_THUNDER), 0x4000);
    PutDrawText(
        &gStatScreen.text[31],
        gUiTmScratchA + TILEMAP_INDEX(0xB, 0x5),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetItemName(ITEM_ANIMA_THUNDER));

    /* 3rd */
    DrawIcon(gUiTmScratchA + TILEMAP_INDEX(0x9, 0x7), GetItemIconId(ITEM_ANIMA_FIMBULVETR), 0x4000);
    PutDrawText(
        &gStatScreen.text[32],
        gUiTmScratchA + TILEMAP_INDEX(0xB, 0x7),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetItemName(ITEM_ANIMA_FIMBULVETR));
};

static void DrawPage5MagicList(void)
{
    // struct Unit * unit = gStatScreen.unit;

    WhiteMagicList();
    BlackMagicList();
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
