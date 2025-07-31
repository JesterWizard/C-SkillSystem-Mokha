#include "common-chax.h"
#include "prep-skill.h"
#include "kernel-lib.h"
#include "uichapterstatus.h"
#include "jester_headers/soundtrack-ids.h"
#include "jester_headers/procs.h"
#include "constants/texts.h"
#include "uiconfig.h"
#include "prepscreen.h"

#define PREP_MENU_VISIBLE_COUNT 5
#define PREP_MENU_MAX_COUNT 9

void PrepScreenMenu_Augury(struct ProcAtMenu* proc) {
    proc->state = 5;
    CallSomeSoundMaybe(SONG_BONDS, 0x100, 0x100, 0x20, 0);
    Proc_Goto(proc, 0xA);
}

void PrepScreenMenu_BEXP(struct ProcAtMenu* proc) {
    PlaySoundEffect(SONG_SE_SYS_WINDOW_SELECT1);
    proc->state = 6;
    Proc_Goto(proc, 0x5);
}

LYN_REPLACE_CHECK(PrepScreenMenu_OnCheckMap);
void PrepScreenMenu_OnCheckMap(struct ProcAtMenu* proc) {
    PlaySoundEffect(SONG_SE_SYS_WINDOW_SELECT1);
    proc->state = 7;
    Proc_Goto(proc, 0x5);
}

void PrepScreenMenu_BaseConversations(struct ProcAtMenu* proc) {
    PlaySoundEffect(SONG_SE_SYS_WINDOW_SELECT1);
    proc->state = 9;
    Proc_Goto(proc, 0x5);
}

void PrepScreenMenu_OnEquip(struct ProcAtMenu * proc)
{
    // if (gpKernelDesigerConfig->equip_skill_en == false)
    // {
    //     /* Check map */
    //     Proc_Goto(proc, 0x5);
    //     return;
    // }

    proc->state = 8;
    Proc_Goto(proc, 0xA);
};

LYN_REPLACE_CHECK(PrepAtMenu_OnInit);
void PrepAtMenu_OnInit(struct ProcAtMenu * proc)
{
    PrepSetLatestCharId(0);
    proc->xDiff = 0;
    *((u16*)&proc->yDiff) = 0;    /* ? */

    if (CheckInLinkArena())
        proc->max_counter = 5;
    else
        proc->max_counter = GetChapterAllyUnitCount();

    proc->unk_30 = 0;
    proc->unk_31 = 0;
    proc->unk_32 = 0;
    proc->state = 0;
    proc->do_help = 0;
    proc->end_prep = 0;
    proc->cur_cmd = 0;
    proc->hand_pos = 0;
}

LYN_REPLACE_CHECK(PrepMenu_OnInit);
void PrepMenu_OnInit(struct ProcPrepMenu_Scroll * proc)
{
    int i;
    for (i = 0; i < 8; i++)
        proc->cmds[i] = 0;

    proc->cur_index = 0;
    proc->max_index = 0;

    ResetSysHandCursor(proc);
    DisplaySysHandCursorTextShadow(0x600, 1);

    proc->on_PressB = 0;
    proc->on_PressStart = 0;
    proc->on_End = 0;
    proc->do_help = 0;

    proc->firstVisibleIndex = 0;

    // Place this AFTER you create your menu proc and set positions.
    StartMenuScrollBar(proc); // 'proc' is your menu's parent Proc
    // Set scroll bar position; adjust x/y as needed for your menu frame
    PutMenuScrollBarAt(3, 58);
    // Optionally decompress graphics if not already done:
    InitMenuScrollBarImg(0x200, 2);
}

extern struct ProcCmd sProc_Menu[];
LYN_REPLACE_CHECK(PrepMenu_CtrlLoop);
void PrepMenu_CtrlLoop(struct ProcPrepMenu_Scroll *proc)
{
    struct ProcPrepMenuItem* cmd;
    int index = proc->cur_index;
    int xPos = (proc->xPos + 1) * 8 + 4;
    int yPos = (proc->yPos + 1) * 8 + proc->cur_index * 16;

    int visibleY = (proc->yPos + 1) * 8 + (proc->cur_index - proc->firstVisibleIndex) * 16;

    ShowSysHandCursor(xPos, visibleY, 0x6, 0x400);
    struct MenuProc* proc2 = Proc_Find(sProc_Menu);
    GetMenuCursorPosition(proc2, &xPos, &yPos);
    ApplyMenuCursorVScroll(proc2, &xPos, &yPos);

    cmd = proc->cmds[proc->cur_index];

    if (proc->do_help) {
        if ((R_BUTTON | B_BUTTON) & gKeyStatusPtr->newKeys) {
            CloseHelpBox();
            proc->do_help = 0;
            return;
        }
    } else {
        if (R_BUTTON & gKeyStatusPtr->newKeys) {
            if (cmd->msg_rtext) {
                StartHelpBox(xPos, yPos, cmd->msg_rtext);
                proc->do_help = 1;
            }
            return;
        }

        if (A_BUTTON & gKeyStatusPtr->newKeys) {
            if ((1 & cmd->color) || (NULL == cmd->effect)) {
                PlaySoundEffect(SONG_6C);
                return;
            } else {
                Proc_Goto(proc, 0x0);
                cmd->effect(proc->proc_parent);
                PlaySoundEffect(SONG_SE_SYS_WINDOW_SELECT1);
                return;
            }
        }

        if (B_BUTTON & gKeyStatusPtr->newKeys) {
            if (proc->on_PressB) {
                if (proc->on_PressB(proc->proc_parent)) {
                    Proc_Goto(proc, 0x0);
                    PlaySoundEffect(SONG_SE_SYS_WINDOW_CANSEL1);
                    return;
                } else {
                    PlaySoundEffect(SONG_6C);
                    return;
                }
            }
            return;
        }

        if (START_BUTTON & gKeyStatusPtr->newKeys) {
            if (proc->on_PressStart) {
                if (proc->on_PressStart(proc->proc_parent)) {
                    PlaySoundEffect(SONG_SE_SYS_WINDOW_SELECT1);
                    Proc_Goto(proc, 0x0);
                    return;
                } else {
                    PlaySoundEffect(SONG_6C);
                    return;
                }
            }
            return;
        }
    }

    if (DPAD_UP & gKeyStatusPtr->repeatedKeys) {
        if (proc->cur_index)
            proc->cur_index = proc->cur_index - 1;
        // else if (DPAD_UP & gKeyStatusPtr->newKeys) // Allows looking of cursor when at top
        //     proc->cur_index = proc->max_index - 1;
    }

    if (DPAD_DOWN & gKeyStatusPtr->repeatedKeys) {
        if (proc->cur_index < (proc->max_index - 1))
            proc->cur_index = proc->cur_index + 1;
        // else if (DPAD_DOWN & gKeyStatusPtr->newKeys) // Allows looping of cursor when at bottom
        //     proc->cur_index = 0;
    }

    SetPrepScreenMenuPosition(1, 6);

    // After updating proc->cur_index
    if (proc->cur_index < proc->firstVisibleIndex) {
        proc->firstVisibleIndex = proc->cur_index;
    }
    if (proc->cur_index >= proc->firstVisibleIndex + PREP_MENU_VISIBLE_COUNT - 1) {
        proc->firstVisibleIndex = proc->cur_index - PREP_MENU_VISIBLE_COUNT + 1;
    }

    UpdateMenuScrollBarConfig(
        (u8)proc->max_index,                       // Height of scrollbar (should be set to max)
        (u16)proc->firstVisibleIndex*16,           // currentSegment: which is topmost
        (u16)proc->max_index,                      // totalRows: total number of items - should be actual max
        (u8)PREP_MENU_VISIBLE_COUNT                // visibleRows: window size
    );

    if (index != proc->cur_index) {
        PlaySoundEffect(SONG_SE_SYS_CURSOR_UD1);

        // Add this to handle scrolling
        struct MenuProc* proc2 = Proc_Find(sProc_Menu);
        int xPos, yPos;
        GetMenuCursorPosition(proc2, &xPos, &yPos);
        ApplyMenuCursorVScroll(proc2, &xPos, &yPos);

        if (proc->do_help) {
            StartHelpBox((proc->xPos + 1) * 8 + 4,
                         (proc->yPos + 1) * 8 + proc->cur_index * 16,
                         (cmd = proc->cmds[proc->cur_index])->msg_rtext);
        }
    }
}

LYN_REPLACE_CHECK(PrepMenu_ShowFrozenHand);
void PrepMenu_ShowFrozenHand(struct ProcPrepMenu_Scroll *proc)
{
    DisplayFrozenUiHand((proc->xPos + 1) * 8 + 4,
                        (proc->yPos + 1) * 8 + proc->cur_index * 16);
}

LYN_REPLACE_CHECK(PrepMenu_ShowActiveHand);
void PrepMenu_ShowActiveHand(struct ProcPrepMenu_Scroll *proc)
{    
    ShowSysHandCursor((proc->xPos + 1) * 8 + 4,
                             (proc->yPos + 1) * 8 + proc->cur_index * 16,
                             6, 0x400);
}

LYN_REPLACE_CHECK(PrepMenu_OnEnd);
void PrepMenu_OnEnd(struct ProcPrepMenu_Scroll *proc)
{
    if (proc->on_End)
        proc->on_End(proc->proc_parent);

    EndMenuScrollBar();
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
       InitText(&gPrepMainMenuTexts[i + 5], 0xE); // Initialize text space for menu section descriptions
    for (i = 0; i < 4; i++)
        InitText(&gPrepMainMenuTexts[i + 1], 0x8); // NOT Menu section titles
    InitText(&gPrepMainMenuTexts[0], 0xA);

    /* "Preparations" */
    Decompress(gUnknown_08A1A4C8, (void*)0x6014800);
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
        /* Chapter objective tile background */
        Decompress(gUnknown_08A1B658, gGenericBuffer);
        CallARM_FillTileRect(TILEMAP_LOCATED(gBG1TilemapBuffer, 0x10, 2), gGenericBuffer, 0x1000);
        /* Options titles and descriptions tile background */
        Decompress(gUnknown_08A1B698, gGenericBuffer);
        CallARM_FillTileRect(TILEMAP_LOCATED(gBG1TilemapBuffer, 1, 6), gGenericBuffer, 0x1000);
    }

    Prep_DrawChapterGoal(0x5800, 0xB);
    NewSysBlackBoxHandler(proc);
    SysBlackBoxSetGfx(0x6800);
    proc->unk_35 = GetActivePrepMenuItemIndex();

    switch ((proc->unk_35)) {
        case 0:
            ParsePrepMenuDescTexts(MSG_PREP_SCREEN_DESC_UNITS);
            break;
        case 1:
            ParsePrepMenuDescTexts(MSG_PREP_SCREEN_DESC_ITEMS);
            break;
        case 2:
            ParsePrepMenuDescTexts(MSG_PREP_SCREEN_DESC_SAVE);
            break;
        case 4:
            ParsePrepMenuDescTexts(MSG_PREP_SCREEN_DESC_SUPPORT);
            break;
        case 5:
            ParsePrepMenuDescTexts(MSG_PREP_SCREEN_DESC_AUGURY);
            break;
        case 6:
            ParsePrepMenuDescTexts(MSG_PREP_SCREEN_DESC_BEXP);
            break;
        case 7:
            ParsePrepMenuDescTexts(MSG_PREP_SCREEN_DESC_START_MAP);
            break;
        case 8:
            ParsePrepMenuDescTexts(MSG_PREP_SCREEN_DESC_SKILLS);
            break;
        case 9:
            ParsePrepMenuDescTexts(MSG_PREP_SCREEN_DESC_BASE_CONVERSATIONS);
            break;
        default:
            break;
    }
    
    DrawPrepMenuDescTexts();
}

LYN_REPLACE_CHECK(sub_8095C00);
void sub_8095C00(int msg, ProcPtr parent)
{
    struct ProcPrepMenuDesc * proc;

    proc = Proc_Find(ProcScr_PrepMenuDescHandler);
    if (proc)
        Proc_End(proc);
    
    proc = Proc_Start(ProcScr_PrepMenuDescHandler, parent);
    //proc->msg = msg;

    /* So this is actually what replaces the prep menu descriptions when moving the cursor up and down */
    switch (GetActivePrepMenuItemIndex()) {
        case 0:
            proc->msg = MSG_PREP_SCREEN_DESC_UNITS;
            break;
        case 1:
            proc->msg = MSG_PREP_SCREEN_DESC_ITEMS;
            break;
        case 2:
            proc->msg = MSG_PREP_SCREEN_DESC_SAVE;
            break;
        case 4:
            proc->msg = MSG_PREP_SCREEN_DESC_SUPPORT;
            break;
        case 5:
            proc->msg = MSG_PREP_SCREEN_DESC_AUGURY;
            break;
        case 6:
            proc->msg = MSG_PREP_SCREEN_DESC_BEXP;
            break;
        case 7:
            proc->msg = MSG_PREP_SCREEN_DESC_START_MAP;
            break;
        case 8:
            proc->msg = MSG_PREP_SCREEN_DESC_SKILLS;
            break;
        case 9:
            proc->msg = MSG_PREP_SCREEN_DESC_BASE_CONVERSATIONS;
            break;
        default:
            break;
    }
}

//! FE8U = 0x0808E3D4
LYN_REPLACE_CHECK(ChapterStatus_ShowAllLayers);
void ChapterStatus_ShowAllLayers(void)
{
    SetDispEnable(1, 1, 1, 1, 1);
    return;
}

LYN_REPLACE_CHECK(SetPrepScreenMenuPosition);
void SetPrepScreenMenuPosition(int x, int y)
{
    int i;
    struct ProcPrepMenu_Scroll *proc;
    struct ProcPrepMenuItem *cmd;
    proc = Proc_Find(ProcScr_PrepMenu);

    if (proc) {
        proc->xPos = x;
        proc->yPos = y;

        if (proc->max_index > 1) {
            // Loop through the *visible slots* on the screen (0 to PREP_MENU_VISIBLE_COUNT - 1)
            for (i = 0; i < PREP_MENU_VISIBLE_COUNT; i++) {
                // Calculate the actual index of the menu item that should be in this visible slot
                int actualItemIndex = proc->firstVisibleIndex + i;

                // Check if this actual item index is within the bounds of existing menu items
                if (actualItemIndex < proc->max_index) {
                    cmd = proc->cmds[actualItemIndex]; // Get the menu item data
                    ClearText(&cmd->text); // Clear previous text for this item
                    PutDrawText( // Draw the text for the menu item
                        &cmd->text,
                        TILEMAP_LOCATED(gBG0TilemapBuffer, x + 2, y + 2 * i + 1), // Y position based on visible slot 'i'
                        1 & cmd->color,
                        0, 0,
                        GetStringFromIndex(cmd->msg)
                    );
                } else {
                    // If no item exists for this visible slot (e.g., end of menu reached),
                    // clear the corresponding tilemap area to remove any old text.
                    // Assuming 0x2 is an empty tile. Adjust as per your tilemap structure.
                    TileMap_FillRect(TILEMAP_LOCATED(gBG0TilemapBuffer, x + 2, y + 2 * i + 1), 0x2, 0x1, 0); //
                }
            }
        }
        BG_EnableSyncByMask(0x1);
    }
}

LYN_REPLACE_CHECK(InitPrepScreenMainMenu);
void InitPrepScreenMainMenu(struct ProcAtMenu* proc)
{
    StartPrepScreenMenu(proc);

    if (!CheckInLinkArena()) 
    {
        SetPrepScreenMenuItem(PREP_MAINMENU_UNIT, PrepScreenMenu_OnPickUnits, TEXT_COLOR_SYSTEM_WHITE, MSG_PREP_SCREEN_TITLE_UNITS, 0);
        SetPrepScreenMenuItem(PREP_MAINMENU_ITEM, PrepScreenMenu_OnItems, TEXT_COLOR_SYSTEM_WHITE, MSG_PREP_SCREEN_TITLE_ITEMS, 0);

        AtMenu_AddPrepScreenSupportMenuItem(proc);

#ifdef CONFIG_SKILL_SYSTEM_RELATED_FUNCTIONALITY
        SetPrepScreenMenuItem(PREP_MAINMENU_SKILLS, PrepScreenMenu_OnEquip, TEXT_COLOR_SYSTEM_WHITE, MSG_PREP_SCREEN_TITLE_SKILLS, 0);
#else
        if (CanPrepScreenCheckMap())
            SetPrepScreenMenuItem(PREP_MAINMENU_CHECKMAP, PrepScreenMenu_OnCheckMap, TEXT_COLOR_SYSTEM_WHITE, MSG_PREP_SCREEN_TITLE_CHECK_MAP, 0);
        else 
            SetPrepScreenMenuItem(PREP_MAINMENU_CHECKMAP, PrepScreenMenu_OnCheckMap, TEXT_COLOR_SYSTEM_GRAY, MSG_PREP_SCREEN_TITLE_CHECK_MAP, 0);
#endif

        if (CanPrepScreenSave())
            SetPrepScreenMenuItem(PREP_MAINMENU_SAVE, PrepScreenMenu_OnSave, TEXT_COLOR_SYSTEM_WHITE, MSG_PREP_SCREEN_TITLE_SAVE, 0);
        else
            SetPrepScreenMenuItem(PREP_MAINMENU_SAVE, PrepScreenMenu_OnSave, TEXT_COLOR_SYSTEM_GRAY, MSG_PREP_SCREEN_TITLE_SAVE, 0);

        SetPrepScreenMenuItem(PREP_MAINMENU_AUGURY, PrepScreenMenu_Augury, TEXT_COLOR_SYSTEM_WHITE, MSG_PREP_SCREEN_TITLE_AUGURY, 0);
        SetPrepScreenMenuItem(PREP_MAINMENU_CHECKMAP, PrepScreenMenu_OnCheckMap, TEXT_COLOR_SYSTEM_WHITE, MSG_PREP_SCREEN_TITLE_CHECK_MAP, 0);
        SetPrepScreenMenuItem(PREP_MAINMENU_BONUX_EXP, PrepScreenMenu_BEXP, TEXT_COLOR_SYSTEM_WHITE, MSG_PREP_SCREEN_TITLE_BEXP, 0);
        //SetPrepScreenMenuItem(PREP_MAINMENU_BASE_CONVERSATIONS, PrepScreenMenu_BaseConversations, TEXT_COLOR_SYSTEM_WHITE, MSG_PREP_SCREEN_TITLE_BASE_CONVERSATIONS, 0);

    } 
    else 
    {
        SetPrepScreenMenuItem(PREP_MAINMENU_UNIT, PrepScreenMenu_OnPickUnits, TEXT_COLOR_SYSTEM_WHITE, MSG_PREP_SCREEN_TITLE_UNITS, 0);
        SetPrepScreenMenuItem(PREP_MAINMENU_ITEM, PrepScreenMenu_OnItems, TEXT_COLOR_SYSTEM_WHITE, MSG_PREP_SCREEN_TITLE_ITEMS, 0);
        SetPrepScreenMenuItem(PREP_MAINMENU_UNK3, sub_8095284, TEXT_COLOR_SYSTEM_WHITE, 0x75C, 0);
    }

    SetPrepScreenMenuOnBPress(PrepScreenMenu_OnBPress);
    SetPrepScreenMenuOnStartPress(PrepScreenMenu_OnStartPress);

    TileMap_FillRect(gBG0TilemapBuffer, 0xC, 0x13, 0);
    TileMap_FillRect(gBG1TilemapBuffer, 0xC, 0x13, 0);

    if (CheckInLinkArena())
        SetPrepScreenMenuPosition(1, 5);
    else
        SetPrepScreenMenuPosition(1, 6);

    SetPrepScreenMenuSelectedItem(proc->cur_cmd);

}

/* This handles where each menu option should go when selected */
LYN_REPLACE_CHECK(AtMenu_StartSubmenu);
void AtMenu_StartSubmenu(struct ProcAtMenu * proc)
{
    sub_8095C2C(proc);

    switch (proc->state) {

    case 1: /* Pick Units */
        Proc_StartBlocking(ProcScr_PrepUnitScreen, proc);
        break;

    case 2: /* Items */
        StartPrepItemScreen(proc);
        break;

    case 3: /* Save */
        StartPrepSaveScreen(proc);
        break;

    case 4: /* Support */
        StartFortuneSubMenu(2, proc);
        break;

#ifdef CONFIG_INSTALL_PREPS_AUGURY
    case 5: /* Augury */
        Proc_StartBlocking(PREEXT_Procs_Augury, proc);
        break;
#endif

    case 6: /* Bonus EXP */
        break;

    case 7: /* Chapter Status */
        StartChapterStatusScreen_FromPrep(proc);
        break;

#ifdef CONFIG_SKILL_SYSTEM_RELATED_FUNCTIONALITY
    case 8: /* Skills */
        StartPrepEquipScreen(proc); 
        break;
#endif

    case 9: /* Base Conversations */
        break;

    default:
        break;
    }

    Proc_Break(proc);
};