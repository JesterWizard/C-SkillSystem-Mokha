#include "common-chax.h"
#include "stat-screen.h"
#include "skill-system.h"
#include "combat-art.h"
#include "icon-rework.h"
#include "constants/texts.h"
#include "jester_headers/gaiden-magic.h"

#define WHITE_MAGIC_STATSCREEN_INDEX 15
#define BLACK_MAGIC_STATSCREEN_INDEX 29

static void DrawWhiteMagicList(void)
{
    PutDrawText(
        &gStatScreen.text[WHITE_MAGIC_STATSCREEN_INDEX],
        gUiTmScratchA + TILEMAP_INDEX(0x1, 0x1),
        TEXT_COLOR_SYSTEM_WHITE,
        0, 0,
        "White Magic");

    for (int i = 0; i < 5; i++)
    {
        if (sWhiteMagicPList[UNIT_CHAR_ID(gStatScreen.unit)].list[i] == 0)
            return;
        
        DrawIcon(gUiTmScratchA + TILEMAP_INDEX(0x1, 0x3 + (i * 2)), GetItemIconId(sWhiteMagicPList[UNIT_CHAR_ID(gStatScreen.unit)].list[i]), 0x4000);
#ifdef CONFIG_AUTO_NARROW_FONT
        PutDrawText(
            &gStatScreen.text[WHITE_MAGIC_STATSCREEN_INDEX + (i + 1)],
            gUiTmScratchA + TILEMAP_INDEX(0x3, 0x3 +  (i * 2)),
            TEXT_COLOR_SYSTEM_GOLD,
            0, 0,
            Utf8ToNarrowFonts((char *)GetItemName(sWhiteMagicPList[UNIT_CHAR_ID(gStatScreen.unit)].list[i])));
#else 
        PutDrawText(
            &gStatScreen.text[WHITE_MAGIC_STATSCREEN_INDEX + (i + 1)],
            gUiTmScratchA + TILEMAP_INDEX(0x3, 0x3 +  (i * 2)),
            TEXT_COLOR_SYSTEM_GOLD,
            0, 0,
            GetItemName(sWhiteMagicPList[UNIT_CHAR_ID(gStatScreen.unit)].list[i]));
#endif
    }
};

static void DrawBlackMagicList(void)
{
    PutDrawText(
        &gStatScreen.text[BLACK_MAGIC_STATSCREEN_INDEX],
        gUiTmScratchA + TILEMAP_INDEX(0x9, 0x1),
        TEXT_COLOR_SYSTEM_WHITE,
        0, 0,
        "Black Magic");

    for (int i = 0; i < 5; i++)
    {
        if (sBlackMagicPList[UNIT_CHAR_ID(gStatScreen.unit)].list[i] == 0)
            return;
        
        DrawIcon(gUiTmScratchA + TILEMAP_INDEX(0x9, 0x3 + (i * 2)), GetItemIconId(sBlackMagicPList[UNIT_CHAR_ID(gStatScreen.unit)].list[i]), 0x4000);
#ifdef CONFIG_AUTO_NARROW_FONT
        PutDrawText(
            &gStatScreen.text[BLACK_MAGIC_STATSCREEN_INDEX + (i + 1)],
            gUiTmScratchA + TILEMAP_INDEX(0xB, 0x3 +  (i * 2)),
            TEXT_COLOR_SYSTEM_GOLD,
            0, 0,
            Utf8ToNarrowFonts((char *)GetItemName(sBlackMagicPList[UNIT_CHAR_ID(gStatScreen.unit)].list[i])));
#else 
        PutDrawText(
            &gStatScreen.text[BLACK_MAGIC_STATSCREEN_INDEX + (i + 1)],
            gUiTmScratchA + TILEMAP_INDEX(0xB, 0x3 +  (i * 2)),
            TEXT_COLOR_SYSTEM_GOLD,
            0, 0,
            GetItemName(sBlackMagicPList[UNIT_CHAR_ID(gStatScreen.unit)].list[i]));
#endif
   }
};

void DrawGaidenMagicPage(void)
{
    int i;
    for (i = 15; i < 20; i++)
        ClearText(&gStatScreen.text[i]);

    for (i = 29; i < 34; i++)
        ClearText(&gStatScreen.text[i]);

    DrawWhiteMagicList();
    DrawBlackMagicList();
}