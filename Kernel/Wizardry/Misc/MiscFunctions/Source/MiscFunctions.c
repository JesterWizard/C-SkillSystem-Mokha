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
#include "action-expa.h"
#include "jester_headers/event-call.h"


typedef struct {
    /* 00 */ struct Font font;
    /* 18 */ struct Text th[18];
} PrepItemSuppyText;
extern const int sExpaConvoyItemAmount;

extern const u16 * Events_WM_Beginning[];
extern const u16 * Events_WM_ChapterIntro[];

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

//! FE8U = 0x0801D084
LYN_REPLACE_CHECK(PlayerPhase_PrepareAction);
s8 PlayerPhase_PrepareAction(ProcPtr proc)
{
    s8 cameraReturn;
    int item;

    cameraReturn = EnsureCameraOntoPosition(
        proc, GetUnit(gActionData.subjectIndex)->xPos, GetUnit(gActionData.subjectIndex)->yPos);
    cameraReturn ^= 1;

#ifdef CONFIG_NO_WAIT_AFTER_TRADING
        gBmSt.taken_action = 0;
#endif

    switch (gActionData.unitActionType)
    {
        case 0:
            /**
             * If character has use some action: such as trade with somebody,
             * then he may get chance to take another action but cannot change to another.
             * If player want to control other character, the current unit may caught wait action.
             */
            if (gBmSt.taken_action != 0)
            {
                gActionData.unitActionType = UNIT_ACTION_FORCE_WAIT;
                break;
            }

            PlayerPhase_BackToMove(proc);

            return 1;

        case UNIT_ACTION_TRADED:
            gBmSt.taken_action |= BM_TAKEN_ACTION_TRADE;
            PlayerPhase_CancelAction(proc);

            return 1;

        case UNIT_ACTION_TRADED_SUPPLY:
            gBmSt.taken_action |= BM_TAKEN_ACTION_SUPPLY;
            PlayerPhase_CancelAction(proc);

            return 1;

        case UNIT_ACTION_TAKE:
        case UNIT_ACTION_GIVE:
            gBmSt.taken_action |= BM_TAKEN_ACTION_TAKE;
            PlayerPhase_CancelAction(proc);

            return 1;

        case UNIT_ACTION_RIDE_BALLISTA:
        case UNIT_ACTION_EXIT_BALLISTA:
            gBmSt.taken_action |= BM_TAKEN_ACTION_BALLISTA;
            PlayerPhase_CancelAction(proc);

            return 1;

        case UNIT_ACTION_TRADED_1D:
            PlayerPhase_CancelAction(proc);

            return 1;
    }

    item = GetItemIndex(GetUnit(gActionData.subjectIndex)->items[gActionData.itemSlotIndex]);

    gBattleActor.hasItemEffectTarget = 0;

    switch (item)
    {
        case ITEM_HEAVENSEAL:
        case ITEM_HEROCREST:
        case ITEM_KNIGHTCREST:
        case ITEM_ORIONSBOLT:
        case ITEM_ELYSIANWHIP:
        case ITEM_GUIDINGRING:
        case ITEM_MASTERSEAL:
        case ITEM_OCEANSEAL:
        case ITEM_LUNARBRACE:
        case ITEM_SOLARBRACE:
        case ITEM_UNK_C1:
            return cameraReturn;
    }

    if ((gActionData.unitActionType != UNIT_ACTION_WAIT) && !gBmSt.just_resumed)
    {
        gActionData.suspendPointType = SUSPEND_POINT_DURINGACTION;
        WriteSuspendSave(SAVE_ID_SUSPEND);
    }

    return cameraReturn;
}

#ifdef CONFIG_FE8_REWRITE
LYN_REPLACE_CHECK(WorldMap_CallBeginningEvent);
//! FE8U = 0x080BA334
void WorldMap_CallBeginningEvent(struct WorldMapMainProc * proc)
{
    int chIndex;
    int node_next;

    Sound_FadeOutBGM(4);

    if ((gGMData.state.bits.monster_merged) || (gPlaySt.chapterStateBits & PLAY_FLAG_POSTGAME))
    {
        sub_80BA008(proc->timer);
    }
    else
    {
        int loc = gGMData.units[0].location;

        gGMData.current_node = loc;
        node_next = WMLoc_GetNextLocId(loc);

        if (node_next > -1)
        {
            chIndex = WMLoc_GetChapterId(node_next);

            gPlaySt.chapterIndex = chIndex;
            

            if (Events_WM_Beginning[GetROMChapterStruct(chIndex)->gmapEventId] == NULL)
                return;

            ResetGmStoryNode();
            proc->gm_icon->merge_next_node = false;

            /**
             * JESTER - I've resorted to hooking into the WM call function to directly load the
             * WM events I want based on the supplied eventSCR. It's an unfortunate bit of hardcoding
             * I'm looking to remove, but it frees me from having to rely on the list in ASM in vanilla.
             */
            int eventID = GetROMChapterStruct(chIndex)->gmapEventId;

            // NoCashGBAPrintf("SET event id is: %d", eventID);
            
            switch (eventID) {
            case 55:
                CallEvent((const u16 *)EventScrWM_Ch1_ENDING, 0);
                break;
            case 1:
                CallEvent((const u16 *)EventScrWM_Prologue_SET_NODE, 0);
                break;
            case 2:
                break;
            case 3:
                CallEvent((const u16 *)EventScrWM_Ch2_SET_NODE, 0);
                break;
            case 4:
                CallEvent((const u16 *)EventScrWM_Ch3_SET_NODE, 0);
                break;
            case 5:
                CallEvent((const u16 *)EventScrWM_Ch4_SET_NODE, 0);
                break;
            case 6:
                break;
            case 7:
                CallEvent((const u16 *)EventScrWM_Ch5_SET_NODE, 0);
                break;
            case 8:
                CallEvent((const u16 *)EventScrWM_Ch6_SET_NODE, 0);
                break;
            case 9:
                CallEvent((const u16 *)EventScrWM_Ch7_SET_NODE, 0);
                break;
            case 10:
                CallEvent((const u16 *)EventScrWM_Ch8_SET_NODE, 0);
                break;
            case 11:
                CallEvent((const u16 *)EventScrWM_Ch9_SET_NODE, 0);
                break;
            case 12:
                CallEvent((const u16 *)EventScrWM_Ch10_SET_NODE, 0);
                break;
            default: 
                CallEvent(Events_WM_Beginning[eventID], 0);
                break;
            }
        }
    }

    StartWMFaceCtrl(proc);
    StartGmapMuEntry(NULL);
}

LYN_REPLACE_CHECK(CallChapterWMIntroEvents);
//! FE8U = 0x080BA3D4
void CallChapterWMIntroEvents(ProcPtr proc)
{
    if (Events_WM_ChapterIntro[GetROMChapterStruct(gPlaySt.chapterIndex)->gmapEventId] != NULL)
    {
         /**
        * JESTER - I've resorted to hooking into the WM call function to directly load the
        * WM events I want based on the supplied eventSCR. It's an unfortunate bit of hardcoding
        * I'm looking to remove, but it frees me from having to rely on the list in ASM in vanilla.
        */
        int eventID = GetROMChapterStruct(gPlaySt.chapterIndex)->gmapEventId;

        // NoCashGBAPrintf("TRAVEL event id is: %d", eventID);
            
        switch (eventID) {
        case 55:
            break;
        case 1:
            CallEvent((const u16 *)EventScrWM_Prologue_TRAVEL_TO_NODE, 0);
            break;
        case 2:
            break;
        case 3:
            CallEvent((const u16 *)EventScrWM_Ch2_TRAVEL_TO_NODE, 0);
            break;
        case 4:
            CallEvent((const u16 *)EventScrWM_Ch3_TRAVEL_TO_NODE, 0);
            break;
        case 5:
            CallEvent((const u16 *)EventScrWM_Ch4_TRAVEL_TO_NODE, 0);
            break;
        case 6:
            break;
        case 7:
            CallEvent((const u16 *)EventScrWM_Ch5_TRAVEL_TO_NODE, 0);
            break;
        case 8:
            CallEvent((const u16 *)EventScrWM_Ch6_TRAVEL_TO_NODE, 0);
            break;
        case 9:
            CallEvent((const u16 *)EventScrWM_Ch7_TRAVEL_TO_NODE, 0);
            break;
        case 10:
            CallEvent((const u16 *)EventScrWM_Ch8_TRAVEL_TO_NODE, 0);
            break;
        case 11:
            CallEvent((const u16 *)EventScrWM_Ch9_TRAVEL_TO_NODE, 0);
            break;
        case 12:
            CallEvent((const u16 *)EventScrWM_Ch10_TRAVEL_TO_NODE, 0);
            break;
        default: 
            CallEvent(Events_WM_ChapterIntro[eventID], 0);
            break;
        }
        StartWMFaceCtrl(proc);
        StartGmapMuEntry(NULL);
    }
}

LYN_REPLACE_CHECK(Event97_WmInitNextStoryNode);
//! FE8U = 0x0800C2DC
u8 Event97_WmInitNextStoryNode(struct EventEngineProc * proc)
{
    // struct WorldMapMainProc * worldMapProc;

    int nodeId = WMLoc_GetNextLocId(gGMData.current_node);

    // NoCashGBAPrintf("Next node ID is: %d", nodeId);

    if (nodeId < 0)
    {
        return EVC_ADVANCE_CONTINUE;
    }

    if (EVENT_IS_SKIPPING(proc))
    {
        ResetGmStoryNode();
        gGMData.nodes[nodeId].state |= 1;
        gGMData.nodes[nodeId].state |= 2;

        GM_ICON->nodeId = nodeId;
        GM_ICON->merge_next_node = true;
    }
    else
    {
        if (!(gGMData.nodes[nodeId].state & 1))
        {
            StartGmBaseEntry(nodeId, 0, NULL);
            ResetGmStoryNode();
            gGMData.nodes[nodeId].state |= 2;
        }
    }

    return EVC_ADVANCE_CONTINUE;
};

LYN_REPLACE_CHECK(Event3E_PrepScreenCall);
//! FE8U = 0x08010968
u8 Event3E_PrepScreenCall(struct EventEngineProc * proc)
{
    HideAllUnits();
    ClearFlag(0x84);
    Proc_StartBlocking(gProcScr_SALLYCURSOR, proc);

    return EVC_ADVANCE_YIELD;
}
#endif

#ifdef CONFIG_QUALITY_OF_LIFE_EPILOGUE_FADE

    //! FE8U = 0x080B723C
    LYN_REPLACE_CHECK(PairedEndingBattleDisp_InitBlend);
    void PairedEndingBattleDisp_InitBlend(struct EndingBattleDisplayProc * proc)
    {
        proc->timer = 0;

        SetBlendAlpha(0x10, 0);
        SetBlendTargetA(0, 0, 0, 0, 0);
        SetBlendTargetB(0, 0, 1, 0, 0);

        return;
    }

    //! FE8U = 0x080B7274
    LYN_REPLACE_CHECK(PairedEndingBattleDisp_Loop_Blend);
    void PairedEndingBattleDisp_Loop_Blend(struct EndingBattleDisplayProc * proc)
    {
        int bldAmt = proc->timer >> 2;

        proc->timer++;

        SetBlendAlpha(0x10 - bldAmt, bldAmt);

        if (bldAmt == 8)
        {
            Proc_Break(proc);
        }

        return;
    }

    static const struct ProcCmd gProcScr_EndingBattleDisplay_Solo_NEW[] =
    {
        PROC_YIELD,

        PROC_CALL(SoloEndingBattleDisp_Init),
        PROC_REPEAT(SoloEndingBattleDisp_Loop),
        
        PROC_SLEEP(16),

        PROC_CALL(PairedEndingBattleDisp_InitBlend),
        PROC_REPEAT(PairedEndingBattleDisp_Loop_Blend),

        PROC_END,
    };

    LYN_REPLACE_CHECK(StartSoloEndingBattleDisplay);
    void StartSoloEndingBattleDisplay(struct CharacterEndingEnt * endingEnt, struct Unit * unit, struct CharacterEndingProc * parent)
    {
        struct EndingBattleDisplayProc * proc = Proc_StartBlocking(gProcScr_EndingBattleDisplay_Solo_NEW, parent);

        proc->units[0] = unit;
        proc->units[1] = NULL;

        proc->pCharacterEnding = endingEnt;

        return;
    }

#endif

#ifdef CONFIG_QUALITY_OF_LIFE_UNIT_NAME_DROP

    struct PopupInstruction const NewItemDropPopup[] = {
        POPUP_SOUND(0x5A),
        POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
        POPUP_UNIT_NAME,
        POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
        POPUP_MSG(MSG_GOT_ITEM),             /* got */
        POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
        POPUP_ITEM_STR,
        POPUP_SPACE(1),
        POPUP_ITEM_ICON,
        POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
        POPUP_SPACE(1),
        POPUP_MSG(0x022),                   /* .[.] */
        POPUP_END
    };

    struct PopupInstruction const NewItemPilferedPopup[] = {
        POPUP_SOUND(0x5A),
        POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
        POPUP_UNIT_NAME,
        POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
        POPUP_MSG(MSG_PILFERED_ITEM),       /* pilfered */
        POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
        POPUP_ITEM_STR,
        POPUP_SPACE(1),
        POPUP_ITEM_ICON,
        POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
        POPUP_SPACE(1),
        POPUP_MSG(0x022),                   /* .[.] */
        POPUP_END
    };

    struct PopupInstruction const NewItemStolePopup[] = {
        POPUP_SOUND(0x5C),
        POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
        POPUP_UNIT_NAME,
        POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
        POPUP_MSG(MSG_STOLE_ITEM),          /* Stole */
        POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
        POPUP_ITEM_STR,
        POPUP_SPACE(1),
        POPUP_ITEM_ICON,
        POPUP_SPACE(1),
        POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
        POPUP_MSG(0x022),                   /* .[.] */
        POPUP_END
    };

    struct PopupInstruction const NewGoldGotPopup[] = {
        POPUP_SOUND(0x5A),
        POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
        POPUP_UNIT_NAME,
        POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
        POPUP_MSG(MSG_GOT_ITEM),               /* got */
        POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
        POPUP_NUM,
        POPUP_SPACE(3),
        POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
        POPUP_MSG(MSG_GOLD),                   /* gold.[.] */
        POPUP_END
    };

    struct PopupInstruction const NewGoldStolenPopup[] = {
        POPUP_SOUND(0x5C),
        POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
        POPUP_UNIT_NAME,
        POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
        POPUP_MSG(MSG_STOLE_ITEM),              /* Stole */
        POPUP_COLOR(TEXT_COLOR_SYSTEM_BLUE),
        POPUP_NUM,
        POPUP_SPACE(3),
        POPUP_COLOR(TEXT_COLOR_SYSTEM_WHITE),
        POPUP_MSG(MSG_GOLD),                   /* gold. */
        POPUP_END
    };

    LYN_REPLACE_CHECK(NewPopup_ItemGot_unused);
    void NewPopup_ItemGot_unused(struct Unit* unit, u16 item, ProcPtr parent)
    {
        SetPopupItem(item);
        SetPopupUnit(unit);

        if (FACTION_BLUE == UNIT_FACTION(unit))
            NewPopup_Simple(NewItemDropPopup, 0x60, 0x0, parent);
        else
            NewPopup_Simple(NewItemPilferedPopup, 0x60, 0x0, parent);
    }

    LYN_REPLACE_CHECK(NewPopup_GoldGot);
    void NewPopup_GoldGot(ProcPtr parent, struct Unit *unit, int value)
    {
        SetPopupNumber(value);
        SetPopupUnit(unit);

        if (FACTION_BLUE == UNIT_FACTION(unit)) 
        {
            value += GetPartyGoldAmount();
            SetPartyGoldAmount(value);
            NewPopup_Simple(NewGoldGotPopup, 0x60, 0x0, parent);
        } 
        else
            NewPopup_Simple(NewGoldStolenPopup, 0x60, 0x0, parent);
    }

    LYN_REPLACE_CHECK(NewPopup_ItemStealing);
    void NewPopup_ItemStealing(u16 item, ProcPtr parent)
    {
        SetPopupItem(item);
        SetPopupUnit(gActiveUnit);

        NewPopup_Simple(NewItemStolePopup, 0x60, 0x0, parent);
    }

#endif

LYN_REPLACE_CHECK(UnitDrop);
void UnitDrop(struct Unit * actor, int xTarget, int yTarget)
{
    struct Unit * target = GetUnit(actor->rescue);

    actor->state = actor->state & ~(US_RESCUING | US_RESCUED);
    target->state = target->state & ~(US_RESCUING | US_RESCUED | US_HIDDEN);

/* Let rescued units move after the rescuer dies */
#ifdef CONFIG_DEATH_DANCE
    if (UNIT_FACTION(target) == gPlaySt.faction && actor->curHP != 0)
        target->state |= US_UNSELECTABLE; // TODO: US_GRAYED    
#else
    if (UNIT_FACTION(target) == gPlaySt.faction)
        target->state |= US_UNSELECTABLE; // TODO: US_GRAYED
#endif

    actor->rescue = 0;
    target->rescue = 0;

    target->xPos = xTarget;
    target->yPos = yTarget;

    /* If we've captured an enemy, dropping them will kill them immediately */
    if (target->curHP == 0)
    {
        PidStatsRecordDefeatInfo(target->pCharacterData->number, actor->pCharacterData->number, DEFEAT_CAUSE_COMBAT);
        UnitKill(target);
    }
}

LYN_REPLACE_CHECK(TradeMenu_InitItemDisplay);
void TradeMenu_InitItemDisplay(struct TradeMenuProc * proc)
{
    DrawUiFrame2(1,  8, 14, 12, 0);
    DrawUiFrame2(15, 8, 14, 12, 0);

    ResetTextFont();

    ResetIconGraphics();
    LoadIconPalettes(4); // TODO: palette id constant

    TradeMenu_InitItemText(proc);
    TradeMenu_RefreshItemText(proc);

#ifdef CONFIG_QUALITY_OF_LIFE_AI_TRADE_FIX
    bool noPortraitUnit_1 = false;
    bool noPortraitUnit_2 = false;

    if (proc->units[0]->pCharacterData->portraitId == 0)
        noPortraitUnit_1 = true;

    if (proc->units[1]->pCharacterData->portraitId == 0)
        noPortraitUnit_2 = true;

    if (!noPortraitUnit_1)
        StartFace(0, GetUnitPortraitId(proc->units[0]), 64,  -4, 3);
    if (!noPortraitUnit_2)
        StartFace(1, GetUnitPortraitId(proc->units[1]), 176, -4, 2);

#else
    // TODO: face display type (arg 5) constants
    StartFace(0, GetUnitPortraitId(proc->units[0]), 64,  -4, 3);
    StartFace(1, GetUnitPortraitId(proc->units[1]), 176, -4, 2);
#endif

    SetFaceBlinkControlById(0, 5);
    SetFaceBlinkControlById(1, 5);

    BG_EnableSyncByMask(BG0_SYNC_BIT | BG1_SYNC_BIT);
}