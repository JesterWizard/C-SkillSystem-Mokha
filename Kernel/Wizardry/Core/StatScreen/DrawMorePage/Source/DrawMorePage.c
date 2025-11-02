#include "common-chax.h"

extern u16 const *const *const gpSprites_PageNameRework;
extern u16 const *const gpPageNameChrOffsetLutRe;
extern u16 const *const gpPageNamePaletteRe;

LYN_REPLACE_CHECK(DisplayPageNameSprite);
void DisplayPageNameSprite(int pageid)
{
	int colorid;

	PutSprite(4,
		111 + gStatScreen.xDispOff, 1 + gStatScreen.yDispOff,
		sSprite_PageNameBack, TILEREF(0x293, 4) + 0xC00);

    /* Display stat screen title */
	PutSprite(4,
		114 + gStatScreen.xDispOff, 0 + gStatScreen.yDispOff,
		gpSprites_PageNameRework[pageid],
		TILEREF(0x240 + gpPageNameChrOffsetLutRe[pageid], 3) + 0xC00);

	colorid = (GetGameClock()/4) % 16;

	CpuCopy16(
		gpPageNamePaletteRe + colorid,
		PAL_OBJ(3) + 0xE,
		sizeof(u16));

	EnablePaletteSync();
}

struct TextInitInfo const sSSMasterTextInitInfo_NEW[] =
{
    { gStatScreen.text + STATSCREEN_TEXT_CHARANAME,  7  }, // 0
    { gStatScreen.text + STATSCREEN_TEXT_CLASSNAME,  8  }, // 1
    { gStatScreen.text + STATSCREEN_TEXT_UNUSUED,    16 }, // 2
    { gStatScreen.text + STATSCREEN_TEXT_POWLABEL,   3  }, // 3
    { gStatScreen.text + STATSCREEN_TEXT_SKLLABEL,   3  }, // 4
    { gStatScreen.text + STATSCREEN_TEXT_SPDLABEL,   3  }, // 5
    { gStatScreen.text + STATSCREEN_TEXT_LCKLABEL,   3  }, // 6
    { gStatScreen.text + STATSCREEN_TEXT_DEFLABEL,   3  }, // 7
    { gStatScreen.text + STATSCREEN_TEXT_RESLABEL,   3  }, // 8
    { gStatScreen.text + STATSCREEN_TEXT_MOVLABEL,   3  }, // 9
    { gStatScreen.text + STATSCREEN_TEXT_CONLABEL,   3  }, // 10
    { gStatScreen.text + STATSCREEN_TEXT_AIDLABEL,   3  }, // 11
    { gStatScreen.text + STATSCREEN_TEXT_RESCUENAME, 9  }, // 12
    { gStatScreen.text + STATSCREEN_TEXT_AFFINLABEL, 7  }, // 13 
    { gStatScreen.text + STATSCREEN_TEXT_STATUS,     9  }, // 14
    { gStatScreen.text + STATSCREEN_TEXT_ITEM0,      7  }, // 15 
    { gStatScreen.text + STATSCREEN_TEXT_ITEM1,      7  }, // 16
    { gStatScreen.text + STATSCREEN_TEXT_ITEM2,      7  }, // 17
    { gStatScreen.text + STATSCREEN_TEXT_ITEM3,      7  }, // 18
    { gStatScreen.text + STATSCREEN_TEXT_ITEM4,      7  }, // 19
    { gStatScreen.text + STATSCREEN_TEXT_BSRANGE,    7  }, // 20
    { gStatScreen.text + STATSCREEN_TEXT_BSATKLABEL, 3  }, // 21
    { gStatScreen.text + STATSCREEN_TEXT_BSHITLABEL, 3  }, // 22
    { gStatScreen.text + STATSCREEN_TEXT_BSCRTLABEL, 3  }, // 23
    { gStatScreen.text + STATSCREEN_TEXT_BSAVOLABEL, 4  }, // 24
    { gStatScreen.text + STATSCREEN_TEXT_WEXP0,      2  }, // 25
    { gStatScreen.text + STATSCREEN_TEXT_WEXP1,      2  }, // 26
    { gStatScreen.text + STATSCREEN_TEXT_WEXP2,      2  }, // 27
    { gStatScreen.text + STATSCREEN_TEXT_WEXP3,      2  }, // 28
    { gStatScreen.text + STATSCREEN_TEXT_SUPPORT0,   7  }, // 29
    { gStatScreen.text + STATSCREEN_TEXT_SUPPORT1,   7  }, // 30
    { gStatScreen.text + STATSCREEN_TEXT_SUPPORT2,   7  }, // 31
    { gStatScreen.text + STATSCREEN_TEXT_SUPPORT3,   7  }, // 32
    { gStatScreen.text + STATSCREEN_TEXT_SUPPORT4,   7  }, // 33
    { gStatScreen.text + STATSCREEN_TEXT_BWL,        16 }, // 34

    { }, // end
};

LYN_REPLACE_CHECK(InitTexts);
void InitTexts(void)
{
    InitTextInitInfo(sSSMasterTextInitInfo_NEW);
}

LYN_REPLACE_CHECK(StatScreen_Display);
void StatScreen_Display(struct Proc* proc)
{
    /* JESTER - 3 is the original page amount, the +1 is for the supports page */
    int pageAmt = 3 + 1;

#ifdef CONFIG_MP_SYSTEM
    pageAmt += 1;
#endif

#ifdef CONFIG_STAT_PAGE_PERSONAL_INFO
    pageAmt += 1;
#endif

    int fid = GetUnitPortraitId(gStatScreen.unit);

    if (gStatScreen.unit->state & US_BIT23)
        fid++;

    // Set page amount (in FE6, this was dependant on whether this is ally or enemy)
    gStatScreen.pageAmt = pageAmt;

    // Init text and icons
    ResetText();
    ResetIconGraphics_();

    InitTexts();

    // Display portrait
    PutFace80x72(proc, gBG2TilemapBuffer + TILEMAP_INDEX(1, 1), fid,
        0x4E0, STATSCREEN_BGPAL_FACE);

    if (GetPortraitData(fid)->img)
        ApplyPalette(gUnknown_08A01EE4, STATSCREEN_BGPAL_2);
    else
        ApplyPalette(gUnknown_08A01F04, STATSCREEN_BGPAL_2);

    // Display Map Sprite
    EndAllMus();
    gStatScreen.mu = StartUiMu(gStatScreen.unit, 80, 138);

    // Draw left panel labels and info
    DisplayLeftPanel();

    // Draw page content
    DisplayPage(gStatScreen.page);

    TileMap_CopyRect(gUiTmScratchA, gBG0TilemapBuffer + TILEMAP_INDEX(12, 2), 18, 18);
    TileMap_CopyRect(gUiTmScratchC, gBG2TilemapBuffer + TILEMAP_INDEX(12, 2), 18, 18);

    BG_EnableSyncByMask(BG0_SYNC_BIT | BG1_SYNC_BIT | BG2_SYNC_BIT);
}

enum
{
    // Magical constants

    // Neutral left arrow position
    PAGENUM_LEFTARROW_X = 103,
    PAGENUM_LEFTARROW_Y = 3,

    // Neutral right arrow position
    PAGENUM_RIGHTARROW_X = 217,
    PAGENUM_RIGHTARROW_Y = 3,

    // initial arrow offset on select
    PAGENUM_SELECT_XOFF = 6,

    // arrow animation speeds
    PAGENUM_ANIMSPEED = 4,
    PAGENUM_SELECT_ANIMSPEED = 31,

    PAGENUM_DISPLAY_X = 215,
    PAGENUM_DISPLAY_Y = 17,

    // name animation scaling time
    PAGENAME_SCALE_TIME = 6,
};

LYN_REPLACE_CHECK(PageNumCtrl_UpdatePageNum);
void PageNumCtrl_UpdatePageNum(struct StatScreenPageNameProc* proc)
{
    int chr = 0x289;

    /* JESTER - A little something to account for page numbers greater than the standard 5 */
    int shift = 0;

    if (gStatScreen.pageAmt > 5)
        shift = gStatScreen.pageAmt - 5;

    // page amt
    PutSprite(2,
        gStatScreen.xDispOff + PAGENUM_DISPLAY_X + 13,
        gStatScreen.yDispOff + PAGENUM_DISPLAY_Y,
        gObject_8x8, TILEREF(chr, STATSCREEN_OBJPAL_4) + OAM2_LAYER(3) + gStatScreen.pageAmt - shift);

    // '/'
    PutSprite(2,
        gStatScreen.xDispOff + PAGENUM_DISPLAY_X + 7,
        gStatScreen.yDispOff + PAGENUM_DISPLAY_Y,
        gObject_8x8, TILEREF(chr, STATSCREEN_OBJPAL_4) + OAM2_LAYER(3) - shift);

    // page num
    PutSprite(2,
        gStatScreen.xDispOff + PAGENUM_DISPLAY_X,
        gStatScreen.yDispOff + PAGENUM_DISPLAY_Y,
        gObject_8x8, TILEREF(chr, STATSCREEN_OBJPAL_4) + OAM2_LAYER(3) + gStatScreen.page + 1 - shift);
}
