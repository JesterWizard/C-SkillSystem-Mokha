#include "common-chax.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "battle-system.h"
#include "unit-expa.h"
#include "eventinfo.h"
#include "bmmenu.h"
#include "eventscript.h"
#include "EAstdlib.h"
#include "worldmap.h"
#include "constants/texts.h"

typedef struct {
    /* 00 */ struct Font font;
    /* 18 */ struct Text th[18];
} PrepItemSuppyText;
extern const int sExpaConvoyItemAmount;

//! FE8U = 0x08098620
LYN_REPLACE_CHECK(PrepItemScreen_SetupGfx);
void PrepItemScreen_SetupGfx(struct PrepItemScreenProc * proc)
{
    int i;

    struct FaceVramEntry faceConfig[4] = {
        { 0x5800, 6, },
        { 0x6800, 7, },
        { 0x0000, 0, },
        { 0x0000, 0, },
    };

    SetupBackgrounds(gBgConfig_ItemUseScreen);

    gLCDControlBuffer.dispcnt.mode = 0;

    SetupFaceGfxData(faceConfig);

    SetDispEnable(0, 0, 0, 0, 0);

    SetupDebugFontForOBJ(0x06017800, 0);

    gLCDControlBuffer.bg0cnt.priority = 0;
    gLCDControlBuffer.bg1cnt.priority = 2;
    gLCDControlBuffer.bg2cnt.priority = 1;
    gLCDControlBuffer.bg3cnt.priority = 3;

    ResetText();
    ResetIconGraphics_();
    LoadIconPalettes(4);
    LoadUiFrameGraphics();
    ApplyPalette(gUiFramePaletteD, 2);
    LoadObjUIGfx();

    MakePrepUnitList();
    proc->hoverUnitIdx = UnitGetIndexInPrepList(PrepGetLatestCharId());

    ResetSysHandCursor(proc);
    StartParallelWorker(sub_809A274, proc);
    StartUiCursorHand(proc);

    SetPrimaryHBlankHandler(NULL);

    BG_Fill(BG_GetMapBuffer(0), 0);
    BG_Fill(BG_GetMapBuffer(1), 0);
    BG_Fill(BG_GetMapBuffer(2), 0);

    gPaletteBuffer[PAL_BACKDROP_OFFSET] = 0;
    EnablePaletteSync();

    for (i = 0; i < 15; i++) {
        InitText(gPrepItemTexts + i, 5);
    }

    for (i = 0; i < 5; i++) {
        InitText(&gPrepItemTexts[15 + i], 7);
        InitText(&gPrepItemTexts[20 + i], 7);
    }

    InitTextDb(&gPrepItemTexts[25], 8);
    InitTextDb(&gPrepItemTexts[26], 8);
    InitText(&gPrepItemTexts[27], 8);
    InitText(&gPrepItemTexts[30], 7);
    InitText(&gPrepItemTexts[31], 5);

#ifdef CONFIG_EXTENDED_HELPBOXES
    LoadHelpBoxGfx((void *)0x06012000, -1);    
#else
    LoadHelpBoxGfx((void *)0x06014000, -1);
#endif


    SetBlendBackdropA(0);
    SetBlendBackdropB(0);

    SetBlendTargetA(0, 0, 0, 0, 0);
    SetBlendTargetB(0, 0, 0, 0, 0);

    gLCDControlBuffer.wincnt.wout_enableBlend = 1;

    SetWinEnable(1, 0, 0);
    SetWin0Box(0, 4, 240, 68);
    SetWin0Layers(1, 1, 1, 1, 1);
    SetWOutLayers(1, 1, 0, 1, 1);

    BG_SetPosition(0, 4, -4);
    BG_SetPosition(1, 0, 4);
    BG_SetPosition(2, -40, (proc->unk_34 - 4) & 0xff);
    BG_EnableSyncByMask(7);

    ApplyUnitSpritePalettes();
    CpuFastFill(0, PAL_OBJ(0x0B), 0x20);

    ForceSyncUnitSpriteSheet();

    Decompress(Img_PrepTextShadow, (void *)0x06013E00);
    UiCursorHand_80ACA4C(0, 0, 0, 0xd0, 0x3c);
    DisplaySysHandCursorTextShadow(0x600, 1);

    RestartMuralBackground();

    if (proc->selectedUnitIdx != 0xff) {
        SetUiCursorHandConfig(
            0,
            ((proc->selectedUnitIdx % 3) * 64) + 24,
            ((proc->selectedUnitIdx / 3) * 16) + 4 - proc->unk_34,
            2
        );
        UpdatePrepItemScreenFace(0, GetUnitFromPrepList(proc->selectedUnitIdx), 60, 76, 0x0503);
    }

    StartMenuScrollBarExt(proc, 224, 11, 0x200, 4);

    UpdateMenuScrollBarConfig(6, proc->unk_34, ((PrepGetUnitAmount() - 1) / 3) + 1, 4);
    sub_8097668();
    PrepUpdateSMS();
    SetBlendConfig(1, 8, 8, 0);
    SetBlendTargetA(0, 1, 0, 0, 0);
    SetBlendTargetB(0, 1, 0, 0, 0);

    SetPrimaryHBlankHandler(PrepItemScreen_OnHBlank);

    StartDrawPrepFundsSprite(168, 133, 11, proc);

    StartDrawPrepUnitInfoBgSprites(proc);

    return;
}

//! FE8U = 0x0809B86C
LYN_REPLACE_CHECK(PrepItemTrade_Init);
void PrepItemTrade_Init(struct PrepMenuTradeProc * proc)
{
    const char* str;
    int i;

    struct FaceVramEntry faceConfig[4] = {
        { 0x5800, 6 },
        { 0x6800, 7 },
        { 0x0000, 0 },
        { 0x0000, 0 }
    };

    // TODO: Seems to be necessary to match - there is a "ldrh" here...
    SetupBackgrounds((void*)(u32)*gBgConfig_ItemUseScreen);

    SetupFaceGfxData(faceConfig);

    gLCDControlBuffer.bg0cnt.priority = 1;
    gLCDControlBuffer.bg1cnt.priority = 2;
    gLCDControlBuffer.bg2cnt.priority = 0;
    gLCDControlBuffer.bg3cnt.priority = 3;

    BG_Fill(BG_GetMapBuffer(0), 0);
    BG_Fill(BG_GetMapBuffer(1), 0);
    BG_Fill(BG_GetMapBuffer(2), 0);

    ResetText();
    ResetIconGraphics_();
    LoadUiFrameGraphics();
    LoadObjUIGfx();

    BG_SetPosition(0, 0, 0);
    BG_SetPosition(1, 0, 0);
    BG_SetPosition(2, 0, 0);

#ifdef CONFIG_EXTENDED_HELPBOXES
    LoadHelpBoxGfx((void*)0x06012000, -1);
#else
    LoadHelpBoxGfx((void*)0x06014000, -1);
#endif

    LoadIconPalettes(4);

    RestartMuralBackground();

    for (i = 0; i < 5; i++) {
        InitTextDb(gPrepItemTexts + 15 + i, 7);
        InitTextDb(gPrepItemTexts + 20 + i, 7);
    }

    proc->selectedItemSlot = 0xff;

    if (proc->unk_40 != -1) {
        proc->cursorItemSlot = proc->unk_40 + 8;
    } else {
        if (GetUnitItemCount(proc->units[0]) == 0) {
            proc->cursorItemSlot = 8;
        } else {
            proc->cursorItemSlot = 0;
        }
    }

    proc->helpBoxItemSlot = 0xff;

    StartFace2(0, GetUnitPortraitId(proc->units[0]), 64, -4, (FACE_DISP_KIND(FACE_96x80_FLIPPED) | FACE_DISP_HLAYER(FACE_HLAYER_3)));
    StartFace2(1, GetUnitPortraitId(proc->units[1]), 174, -4, (FACE_DISP_KIND(FACE_96x80) | FACE_DISP_HLAYER(FACE_HLAYER_3)));

    DrawUiFrame2(1, 8, 14, 12, 0);
    DrawUiFrame2(15, 8, 14, 12, 0);

    BG_EnableSyncByMask(7);

    str = GetStringFromIndex(proc->units[0]->pCharacterData->nameTextId);
    PutDrawText(0, gBG0TilemapBuffer, 0, ((48 - GetStringTextLen(str)) / 2), 6, str);

    str = GetStringFromIndex(proc->units[1]->pCharacterData->nameTextId);
    PutDrawText(0, gBG0TilemapBuffer + 0x18, 0, ((48 - GetStringTextLen(str)) / 2), 6, str);

    DrawPrepScreenItems(gBG0TilemapBuffer + 0x122, gPrepItemTexts + 15, proc->units[0], 0);
    DrawPrepScreenItems(gBG0TilemapBuffer + 0x130, gPrepItemTexts + 20, proc->units[1], 0);

    StartUiCursorHand(proc);

    ResetSysHandCursor(proc);
    DisplaySysHandCursorTextShadow(0x600, 1);
    ShowSysHandCursor(
        (proc->cursorItemSlot >> 3) * 0x70 + 0x10,
        (proc->cursorItemSlot & 7) * 0x10 + 0x48,
        0xb,
        0x800
    );

    StartHelpPromptSprite(200, 0x93, 2, proc);

    StartSysBrownBox(0xd, 0xe00, 0xf, 0xc00, 0x400, proc);

    EnableSysBrownBox(0, -0x28, -1, 1);
    EnableSysBrownBox(1, 0xb8, -1, 0);

    SetBlendConfig(1, 0xe, 4, 0);

    SetBlendTargetA(0, 0, 0, 0, 0);
    SetBlendTargetB(0, 0, 0, 1, 0);

    return;
}

//! FE8U = 0x0809D914
LYN_REPLACE_CHECK(sub_809D914);
void sub_809D914(struct PrepItemSupplyProc * proc)
{

    gLCDControlBuffer.dispcnt.mode = 0;
    SetupBackgrounds(NULL);

    BG_Fill(BG_GetMapBuffer(0), 0);
    BG_Fill(BG_GetMapBuffer(1), 0);
    BG_Fill(BG_GetMapBuffer(2), 0);

    gLCDControlBuffer.bg0cnt.priority = 1;
    gLCDControlBuffer.bg1cnt.priority = 2;
    gLCDControlBuffer.bg2cnt.priority = 0;
    gLCDControlBuffer.bg3cnt.priority = 3;

    ResetFaces();
    ResetText();
    ResetIconGraphics_();
    LoadUiFrameGraphics();
    LoadObjUIGfx();

    BG_SetPosition(0, 0, 0);
    BG_SetPosition(1, 0, 0);
    BG_SetPosition(2, 0, proc->yOffsetPerPage[proc->currentPage] - 0x28);

#ifdef CONFIG_EXTENDED_HELPBOXES
    LoadHelpBoxGfx((void*)0x06012000, -1);
#else
    LoadHelpBoxGfx((void*)0x06013000, -1);
#endif

    LoadIconPalettes(4);
    RestartMuralBackground();
    BG_EnableSyncByMask(8);

    gLCDControlBuffer.dispcnt.bg0_on = 0;
    gLCDControlBuffer.dispcnt.bg1_on = 0;
    gLCDControlBuffer.dispcnt.bg2_on = 0;
    gLCDControlBuffer.dispcnt.bg3_on = 0;
    gLCDControlBuffer.dispcnt.obj_on = 0;

    return;
}

//! FE8U = 0x0809ED8C
LYN_REPLACE_CHECK(PrepItemList_InitGfx);
void PrepItemList_InitGfx(struct PrepItemListProc * proc)
{
    int i;
    const char* str;

    PrepItemSuppyText PrepItemSuppyTexts;

    gLCDControlBuffer.dispcnt.mode = 0;

    SetupBackgrounds(NULL);

    BG_Fill(BG_GetMapBuffer(0), 0);
    BG_Fill(BG_GetMapBuffer(1), 0);
    BG_Fill(BG_GetMapBuffer(2), 0);

    gLCDControlBuffer.bg0cnt.priority = 1;
    gLCDControlBuffer.bg1cnt.priority = 2;
    gLCDControlBuffer.bg2cnt.priority = 0;
    gLCDControlBuffer.bg3cnt.priority = 3;

    ResetFaces();
    ResetText();
    ResetIconGraphics_();
    LoadUiFrameGraphics();
    LoadObjUIGfx();

    BG_SetPosition(0, 0, 0);
    BG_SetPosition(1, 0, 0);
    BG_SetPosition(2, 0, proc->yOffsetPerPage[proc->currentPage] - 40);

#ifdef CONFIG_EXTENDED_HELPBOXES
    LoadHelpBoxGfx((void*)0x06013000, -1);
#else
    LoadHelpBoxGfx((void*)0x06012000, -1);
#endif

    LoadIconPalettes(4);

    RestartMuralBackground();

    PutImg_PrepItemUseUnk(0x5000, 5);

    Decompress(gUnknown_08A1B9EC, gGenericBuffer);
    CallARM_FillTileRect(gBG1TilemapBuffer, gGenericBuffer, 0x1000);

    BG_EnableSyncByMask(7);

    StartUiCursorHand(proc);

    ResetSysHandCursor(proc);
    DisplaySysHandCursorTextShadow(0x600, 1);

    gLCDControlBuffer.dispcnt.win0_on = 1;
    gLCDControlBuffer.dispcnt.win1_on = 0;
    gLCDControlBuffer.dispcnt.objWin_on = 0;

    gLCDControlBuffer.win0_left = 128;
    gLCDControlBuffer.win0_top = 40;
    gLCDControlBuffer.win0_right = 224;
    gLCDControlBuffer.win0_bottom = 152;

    gLCDControlBuffer.wincnt.win0_enableBg0 = 1;
    gLCDControlBuffer.wincnt.win0_enableBg1 = 1;
    gLCDControlBuffer.wincnt.win0_enableBg2 = 1;
    gLCDControlBuffer.wincnt.win0_enableBg3 = 1;
    gLCDControlBuffer.wincnt.win0_enableObj = 1;

    gLCDControlBuffer.wincnt.wout_enableBg0 = 1;
    gLCDControlBuffer.wincnt.wout_enableBg1 = 1;
    gLCDControlBuffer.wincnt.wout_enableBg2 = 0;
    gLCDControlBuffer.wincnt.wout_enableBg3 = 1;
    gLCDControlBuffer.wincnt.wout_enableObj = 1;

    StartGreenText(proc);

    StartHelpPromptSprite(195, 147, 9, proc);

    InitText(PrepItemSuppyTexts.th + 0, 6);
    InitText(PrepItemSuppyTexts.th + 1, 5);

    InitText(PrepItemSuppyTexts.th + 15, 4);

    for (i = 0; i < UNIT_ITEM_COUNT; i++) {
        InitText(PrepItemSuppyTexts.th + 2 + i, 7);
    }

    for (i = 0; i < 8; i++) {
        InitTextDb(PrepItemSuppyTexts.th + 7 + i, 7);
    }

    StoreConvoyWeaponIconGraphics(0x4000, 6);

    sub_809D8D4(gBG0TilemapBuffer + 0x6F, 0x4000, 6);

    Decompress(gUnknown_08A19CCC, (void*)0x06015000);
    ApplyPalette(Pal_SpinningArrow, 0x14);

    StartMenuScrollBarExt(proc, 225, 47, 0x5800, 9);
    sub_8097668();
    SomethingPrepListRelated(proc->unit, proc->currentPage, 3);
    sub_809F5F4(proc);

    sub_809D300(
        PrepItemSuppyTexts.th + 7,
        gBG2TilemapBuffer + 0xF,
        (proc->yOffsetPerPage[proc->currentPage]) >> 4,
        proc->unit
    );

    BG_EnableSyncByMask(4);

    DrawPrepScreenItems(gBG0TilemapBuffer + 0x6F + 0xb3, PrepItemSuppyTexts.th + 2, proc->unit, 0);
    sub_809EBF0();

    StartUiSpinningArrows(proc);
    LoadUiSpinningArrowGfx(0, 0x280, 2);
    SetUiSpinningArrowPositions(0x78, 0x18, 0xe9, 0x18);
    SetUiSpinningArrowConfig(3);

    StartParallelWorker(List_PutHighlightedCategorySprites, proc);

    StartFace2(0, GetUnitPortraitId(proc->unit), 64, -4, FACE_DISP_KIND(FACE_96x80_FLIPPED) | FACE_DISP_HLAYER(FACE_HLAYER_3));

    str = GetStringFromIndex(proc->unit->pCharacterData->nameTextId);

    StartSysBrownBox(0xd, 0xe00, 0xf, 0xc00, 0x400, proc);

    EnableSysBrownBox(0, -40, -1, 1);
    EnableSysBrownBox(1, 0x98, 6, 2);

    SetBlendConfig(1, 0xe, 4, 0);
    SetBlendTargetA(0, 0, 0, 0, 0);
    SetBlendTargetB(0, 0, 0, 1, 0);

    PutDrawText(PrepItemSuppyTexts.th, gBG0TilemapBuffer, 0, GetStringTextCenteredPos(48, str), 0, str);

    PrepItemList_DrawCurrentOwnerText(proc);

    return;
}

LYN_REPLACE_CHECK(TradeMenu_InitUnitNameDisplay);
void TradeMenu_InitUnitNameDisplay(struct TradeMenuProc * proc)
{
    char* str;
    int xStart;

    // TODO: constants
#ifdef CONFIG_EXTENDED_HELPBOXES
    StartSysBrownBox(6, 0x5800, 0x08, 0x800, 0x400, (struct Proc *) (proc));
#else
    StartSysBrownBox(6, 0x4800, 0x08, 0x800, 0x400, (struct Proc *) (proc));
#endif

    EnableSysBrownBox(0, -40, -1, 1);
    EnableSysBrownBox(1, 184, -1, 0);

    // TODO: special effect constants
    SetBlendConfig(1, 12, 6, 0);

    // TODO: name functions
    SetBlendTargetA(FALSE, FALSE, FALSE, FALSE, FALSE);
    SetBlendTargetB(TRUE,  TRUE,  TRUE,  TRUE,  TRUE);

    // TODO: text color constants

    str = GetStringFromIndex(UNIT_NAME_ID(proc->units[0]));
    xStart = ((8 * UNIT_PANEL_WIDTH) - GetStringTextLen(str)) / 2;

    PutDrawText(NULL, gBG0TilemapBuffer + TILEMAP_INDEX(0, 0), 0, xStart, UNIT_PANEL_WIDTH, str);

    str = GetStringFromIndex(UNIT_NAME_ID(proc->units[1]));
    xStart = ((8 * UNIT_PANEL_WIDTH) - GetStringTextLen(str)) / 2;

    PutDrawText(NULL, gBG0TilemapBuffer + TILEMAP_INDEX(24, 0), 0, xStart, UNIT_PANEL_WIDTH, str);

    BG_EnableSyncByMask(BG0_SYNC_BIT);
}

//! FE8U = 0x08032728
LYN_REPLACE_CHECK(KillUnitOnCombatDeath);
void KillUnitOnCombatDeath(struct Unit* unitA, struct Unit* unitB) {
    if (GetUnitCurrentHp(unitA) != 0) {
        return;
    }

#ifdef CONFIG_SEND_INVENTORY_ON_DEATH
    if (UNIT_FACTION(unitA) == FACTION_BLUE)
    {
        for (int i = 0; i < 5; i++)
        {
            if (unitA->items[i] == 0)
                break;
            
            if (GetConvoyItemCount() < sExpaConvoyItemAmount)
                AddItemToConvoy(unitA->items[i]);
        }
    }
#endif

    PidStatsRecordDefeatInfo(unitA->pCharacterData->number, unitB->pCharacterData->number, DEFEAT_CAUSE_COMBAT);

    UnitKill(unitA);

    return;
}

//! FE8U = 0x08032750
LYN_REPLACE_CHECK(KillUnitOnArenaDeathMaybe);
void KillUnitOnArenaDeathMaybe(struct Unit* unit) {
    if (GetUnitCurrentHp(unit) != 0) {
        return;
    }

#ifdef CONFIG_SEND_INVENTORY_ON_DEATH
    if (UNIT_FACTION(unit) == FACTION_BLUE)
    {
        for (int i = 0; i < 5; i++)
        {
            if (unit->items[i] == 0)
                break;
            
            if (GetConvoyItemCount() < sExpaConvoyItemAmount)
                AddItemToConvoy(unit->items[i]);
        }
    }
#endif

    UnitKill(unit);

    PidStatsRecordDefeatInfo(unit->pCharacterData->number, 0, DEFEAT_CAUSE_ARENA);

    return;
}