#include "common-chax.h"
#include "stat-screen.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "savemenu.h"
#include "uichapterstatus.h"
#include "unitlistscreen.h"
#include "savemenu.h"

LYN_REPLACE_CHECK(StartStatScreenHelp);
void StartStatScreenHelp(int pageid, struct Proc * proc)
{
    LoadHelpBoxGfx(NULL, -1); // default

    if (!gStatScreen.help)
    {
        switch (pageid) {
        case STATSCREEN_PAGE_0:
            StartUnitScreenHelp(pageid, proc);
            break;

        case STATSCREEN_PAGE_1:
            gStatScreen.help = &gHelpInfo_Ss1Item0;
            break;

        case STATSCREEN_PAGE_2:
            StartSkillScreenHelp(pageid, proc);
            break;

        case 3:
            gStatScreen.help = RTextPageSupport;
            break;
        } // switch (pageid)
    }
    StartMovingHelpBox(gStatScreen.help, proc);
}

LYN_REPLACE_CHECK(DisplayPage);
void DisplayPage(int pageid)
{
    typedef void(* func_type)(void);
    extern const func_type gStatScreenDrawPages[];

    CpuFastFill(0, gUiTmScratchA, sizeof(gUiTmScratchA));
    CpuFastFill(0, gUiTmScratchC, sizeof(gUiTmScratchC));

    gStatScreenDrawPages[pageid]();
}

void HbPopuplate_Page3Skill(struct HelpBoxProc * proc)
{
    struct SkillList * list = GetUnitSkillList(gStatScreen.unit);
    proc->mid = GetSkillDescMsg(list->sid[proc->info->mid]);
}

void HbRedirect_Page3Skill(struct HelpBoxProc * proc)
{
    if (proc->info->mid < GetUnitSkillList(gStatScreen.unit)->amt)
        return;

    switch (proc->moveKey) {
    case DPAD_DOWN:
        TryRelocateHbDown(proc);
        break;

    case DPAD_UP:
        TryRelocateHbUp(proc);
        break;

    case DPAD_LEFT:
        TryRelocateHbLeft(proc);
        break;

    case DPAD_RIGHT:
    default:
        TryRelocateHbRight(proc);
        break;
    } // switch
}

LYN_REPLACE_CHECK(LoadHelpBoxGfx);
void LoadHelpBoxGfx(void * vram, int palId)
{
// Repoint the vram used for the stat screen help box
#ifdef CONFIG_EXTENDED_HELPBOXES
    if (vram == NULL) {
        if (Proc_Find(gProcScr_StatScreen))
            vram = (void *)0x06012000;
        else
            vram = (void *)0x06013000;
    }
#else
    if (vram == NULL) {
        vram = (void *)0x06013000;
    }
#endif

    if (palId < 0) {
        palId = 5;
    }

    palId = (palId & 0xF) + 0x10;

    Decompress(gGfx_HelpTextBox, vram + 0x360);
    Decompress(gGfx_HelpTextBox2, vram + 0x760);
    Decompress(gGfx_HelpTextBox3, vram + 0xb60);
    Decompress(gGfx_HelpTextBox4, vram + 0xf60);
    Decompress(gGfx_HelpTextBox5, vram + 0x1360);

    InitSpriteTextFont(&gHelpBoxSt.font, vram, palId);

    InitSpriteText(&gHelpBoxSt.text[0]);
    InitSpriteText(&gHelpBoxSt.text[1]);
    InitSpriteText(&gHelpBoxSt.text[2]);

    /* Only provide the extra text box tiles if we're not in the save menu or chapter status screens */
#ifdef CONFIG_EXTENDED_HELPBOXES

    const struct ProcCmd * procExceptionsList[9] = 
    {
        ProcScr_SaveMenu,
        gProcScr_SaveMenuPostChapter,
        gProcScr_ChapterStatusScreen,
        gProcScr_DrawUnitInfoBgSprites,
        ProcScr_bmview,
        ProcScr_UnitListScreen_Field,
        ProcScr_UnitListScreen_PrepMenu,
        ProcScr_UnitListScreen_SoloAnim,
        ProcScr_UnitListScreen_WorldMap,
        // PrepScreenProc_MapIdle,
    };

    bool procFound = false;
    
    for (int i = 0; i < (int)ARRAY_COUNT(procExceptionsList); i++)
    {
        if (Proc_Find(procExceptionsList[i]))
        {
            procFound = true;
            break;
        }
    }

    if (gActionData.unk08 == SID_SummonPlus && !procFound)
        procFound = true;

    if (!procFound)
    {
        InitSpriteText(&gHelpBoxSt.text[3]);
        InitSpriteText(&gHelpBoxSt.text[4]);
    }
#endif

    SetTextFont(0);

    ApplyPalette(Pal_HelpBox, palId);

    gHelpBoxSt.oam2_base = (((u32)vram << 0x11) >> 0x16) + (palId & 0xF) * 0x1000;
}

//! FE8U = 0x0808A00C
LYN_REPLACE_CHECK(HelpBoxIntroDrawTexts);
void HelpBoxIntroDrawTexts(struct ProcHelpBoxIntro * proc)
{
    struct HelpBoxScrollProc * otherProc;
    int textSpeed;

    SetTextFont(&gHelpBoxSt.font);

    SetTextFontGlyphs(1);

    Text_SetColor(&gHelpBoxSt.text[0], 6);
    Text_SetColor(&gHelpBoxSt.text[1], 6);
    Text_SetColor(&gHelpBoxSt.text[2], 6);

#ifdef CONFIG_EXTENDED_HELPBOXES
    Text_SetColor(&gHelpBoxSt.text[3], 6);
    Text_SetColor(&gHelpBoxSt.text[4], 6);
#endif

    SetTextFont(0);

    Proc_EndEach(gProcScr_HelpBoxTextScroll);

    otherProc = Proc_Start(gProcScr_HelpBoxTextScroll, PROC_TREE_3);
    otherProc->font = &gHelpBoxSt.font;

    otherProc->texts[0] = &gHelpBoxSt.text[0];
    otherProc->texts[1] = &gHelpBoxSt.text[1];
    otherProc->texts[2] = &gHelpBoxSt.text[2];

#ifdef CONFIG_EXTENDED_HELPBOXES
    otherProc->texts[3] = &gHelpBoxSt.text[3];
    otherProc->texts[4] = &gHelpBoxSt.text[4];
#endif

    otherProc->pretext_lines = proc->pretext_lines;

    // GetStringFromIndex writes to sMsgString.buffer1, which is then used by StringInsertSpecialPrefixByCtrl a couple lines later
    GetStringFromIndex(proc->msg);

    otherProc->string = StringInsertSpecialPrefixByCtrl();
    otherProc->chars_per_step = 1;
    otherProc->step = 0;

    textSpeed = gPlaySt.config.textSpeed;
    switch (gPlaySt.config.textSpeed) {
    case 0: /* default speed */
        otherProc->speed = 2;
        break;

    case 1: /* slow */
        otherProc->speed = textSpeed;
        break;

    case 2: /* fast */
        otherProc->speed = 1;
        otherProc->chars_per_step = textSpeed;
        break;

    case 3: /* draw all at once */
        otherProc->speed = 0;
        otherProc->chars_per_step = 0x7f;
        break;
    }
}

//! FE8U = 0x080898C4
LYN_REPLACE_CHECK(sub_80898C4);
void sub_80898C4(void* vram, int palId) {

// Repoint the vram used for the stat screen help box
#ifdef CONFIG_EXTENDED_HELPBOXES
    if (vram == NULL) {
        vram = (void *)0x06012000;
    }
#else
    if (vram == NULL) {
        vram = (void *)0x06013000;
    }
#endif


    if (palId < 0) {
        palId = 5;
    }

    palId = (palId & 0xf) + 0x10;

    Decompress(gGfx_HelpTextBox, vram + 0x360);
    Decompress(gGfx_HelpTextBox2, vram + 0x760);
    Decompress(gGfx_HelpTextBox3, vram + 0xb60);
    Decompress(gGfx_HelpTextBox4, vram + 0xf60);
    Decompress(gGfx_HelpTextBox5, vram + 0x1360);

    InitSpriteTextFont(&gHelpBoxSt.font, vram, palId);

    InitSpriteText(&gHelpBoxSt.text[0]);
    InitSpriteText(&gHelpBoxSt.text[1]);
    
#ifdef CONFIG_EXTENDED_HELPBOXES
    InitSpriteText(&gHelpBoxSt.text[2]);
    InitSpriteText(&gHelpBoxSt.text[3]);
    InitSpriteText(&gHelpBoxSt.text[4]);
#endif

    gHelpBoxSt.text[2].tile_width = 0;

    SetTextFont(0);

    ApplyPalette(Pal_HelpBox, palId);

    gHelpBoxSt.oam2_base = (((u32)vram << 0x11) >> 0x16) + (palId & 0xF) * 0x1000;

    return;
}

//! FE8U = 0x0808A118
LYN_REPLACE_CHECK(ClearHelpBoxText);
void ClearHelpBoxText(void) {

    SetTextFont(&gHelpBoxSt.font);

    SpriteText_DrawBackground(&gHelpBoxSt.text[0]);
    SpriteText_DrawBackground(&gHelpBoxSt.text[1]);
    SpriteText_DrawBackground(&gHelpBoxSt.text[2]);

#ifdef CONFIG_EXTENDED_HELPBOXES
    SpriteText_DrawBackground(&gHelpBoxSt.text[3]);
    SpriteText_DrawBackground(&gHelpBoxSt.text[4]);
#endif

    Proc_EndEach(gProcScr_HelpBoxTextScroll);
    Proc_EndEach(ProcScr_HelpBoxIntro);

    SetTextFont(0);

    return;
}

LYN_REPLACE_CHECK(AtMenu_Reinitialize);
void AtMenu_Reinitialize(struct ProcAtMenu* proc)
{
    int i;

    SetupBackgrounds(gBgConfig_ItemUseScreen);
    ResetText();
    LoadUiFrameGraphics();
    LoadHelpBoxGfx(NULL, 0xE);
    SetDispEnable(0, 0, 0, 0, 0);
    LoadObjUIGfx();
    ResetUnitSprites();
    
    MakePrepUnitList();
    PrepAutoCapDeployUnits(proc);
    ReorderPlayerUnitsBasedOnDeployment();

    BG_Fill(gBG0TilemapBuffer, 0);
    BG_Fill(gBG1TilemapBuffer, 0);
    BG_Fill(gBG2TilemapBuffer, 0);

    for (i = 0; i < 5; i++)
        InitText(&gPrepMainMenuTexts[i + 5], 0xE);
    for (i = 0; i < 4; i++)
        InitText(&gPrepMainMenuTexts[i + 1], 0x8);
    InitText(&gPrepMainMenuTexts[0], 0xA);

    /* "Preparations" */
#ifdef CONFIG_EXTENDED_HELPBOXES
    Decompress(gUnknown_08A1A4C8, (void*)0x6015800);
#else
    Decompress(gUnknown_08A1A4C8, (void*)0x6014800);
#endif
    /* "Menu", "Start" button */
    Decompress(gUnknown_08A1D510, (void*)0x6016000);
    ApplyPalettes(Pal_SysBrownBox, 0x19, 2);
    
    sub_8095C50(0x7000, 0x6);
    ApplyPalette(gUnknown_08A1D4C8, 0x14);
    EnablePaletteSync();

    gLCDControlBuffer.bg0cnt.priority = 0;
    gLCDControlBuffer.bg1cnt.priority = 2;
    gLCDControlBuffer.bg2cnt.priority = 1;
    gLCDControlBuffer.bg3cnt.priority = 3;

    gLCDControlBuffer.dispcnt.win0_on = 0;
    gLCDControlBuffer.dispcnt.win1_on = 0;
    gLCDControlBuffer.dispcnt.objWin_on  = 0;

    BG_SetPosition(0, 0, 0);
    BG_SetPosition(1, 0, 0);
    BG_SetPosition(2, 0, 0);

    InitPrepScreenMainMenu(proc);
    BG_EnableSyncByMask(0xF);
    SetDefaultColorEffects();

    StartPrepSpecialCharEffect(proc);
    RestartMuralBackground();
    ApplyPalettes(gUiFramePaletteB, 0x2, 3);

    if (CheckInLinkArena()) {
        Decompress(gUnknown_08A1B698, gGenericBuffer);
        CallARM_FillTileRect(TILEMAP_LOCATED(gBG1TilemapBuffer, 1, 5), gGenericBuffer, 0x1000);
    } else {
        Decompress(gUnknown_08A1B658, gGenericBuffer);
        CallARM_FillTileRect(TILEMAP_LOCATED(gBG1TilemapBuffer, 0x10, 2), gGenericBuffer, 0x1000);
        Decompress(gUnknown_08A1B698, gGenericBuffer);
        CallARM_FillTileRect(TILEMAP_LOCATED(gBG1TilemapBuffer, 1, 6), gGenericBuffer, 0x1000);
    }

    Prep_DrawChapterGoal(0x5800, 0xB);
    NewSysBlackBoxHandler(proc);
    SysBlackBoxSetGfx(0x6800);
    proc->unk_35 = GetActivePrepMenuItemIndex();
    ParsePrepMenuDescTexts(sub_8095024());
    DrawPrepMenuDescTexts();
}