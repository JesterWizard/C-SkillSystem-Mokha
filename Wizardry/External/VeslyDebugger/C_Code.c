// #include "C_Code.h" // headers 
// #include "debug-kit.h"
#include "constants/gfx.h"
#include "mu.h"
#include "../../../include/Configs/configs.h"
#include "../../../include/skill-system.h"
#include "../../../include/common-chax.h"
#include "../../../include/debuff.h"
#include "../../../include/bwl.h"

#define PACKED __attribute__((packed))

extern char * GetSkillNameStr(const u16 sid);

#define PUREFUNC __attribute__((pure))
int Mod(int a, int b) PUREFUNC;

#define xTilesAmount 15
#define favTilesAmount 15
#define tmpSize 15

typedef struct {
    /* 00 */ PROC_HEADER;
    s16 tileID; 
    u16 lastTileHovered; 
    s8 editing; 
    u8 actionID; 
    s8 id; // used by our custom menus 
    s8 digit;
    u8 godMode;
    u8 autoplay; 
    u8 page; 
    s8 mainID; // by the main debugger menu 
    u16 lastFlag; 
    int gold; 
    struct Unit* unit; 
    s16 tmp[tmpSize];
} DebuggerProc;

typedef struct { 
    /* 00 */ PROC_HEADER;
    int id; 
} CheatCodeKeyListenerProc;

const u16 KonamiCodeSequence[] = { DPAD_UP, DPAD_UP, DPAD_DOWN, DPAD_DOWN, DPAD_LEFT, DPAD_RIGHT, DPAD_LEFT, DPAD_RIGHT, B_BUTTON, A_BUTTON, 0, 0 }; 
extern int DebuggerTurnedOff_Flag; 
extern int KeyComboToDisableFlag; 
extern int KonamiCodeEnabled; 

void ToggleFlag(int flag) { 
    if (CheckFlag(flag)) { ClearFlag(flag); } 
    else { SetFlag(flag); } 
} 

void CheckKeysForCheatCode(CheatCodeKeyListenerProc* proc) { 
    int keys = gKeyStatusPtr->newKeys; 
    if (!keys) { return; } 

    if (KonamiCodeEnabled) { 
        if (KonamiCodeSequence[proc->id] & keys) { proc->id++; } 
        else {  if (keys & DPAD_UP) { proc->id = 2; } 
                else { proc->id = 0; } 
        }
        if (!KonamiCodeSequence[proc->id]) { ToggleFlag(DebuggerTurnedOff_Flag); proc->id = 0; } 
    }
    keys |= gKeyStatusPtr->heldKeys; 
    if (KeyComboToDisableFlag) { 
        if ((keys&KEYS_MASK) == KeyComboToDisableFlag) { 
            ToggleFlag(DebuggerTurnedOff_Flag); 
        } 
    } 
    
} 

const struct ProcCmd CheatCodeKeyListenerCmd[] =
{
	PROC_NAME("CheatCodeKeyListenerProc"), 
    PROC_YIELD,
    PROC_REPEAT(CheckKeysForCheatCode), 
    PROC_END, 
}; 


int StartKeyListenerProc(void) { 
    int keys = gKeyStatusPtr->newKeys; 
    if (!keys) { return 0; } 
    CheatCodeKeyListenerProc* proc = Proc_Find(CheatCodeKeyListenerCmd); 
    if (proc) { 
        return 0; 
    } 
    proc = Proc_Start(CheatCodeKeyListenerCmd, PROC_TREE_3); 
    proc->id = 0; 
    return true; 
} 

int Mod(int a, int b)
{
    return a % b;
}

void CopyProcVariables(DebuggerProc* dst, DebuggerProc* src) { 
    dst->tileID = src->tileID; 
    dst->mainID = src->mainID; 
    dst->lastTileHovered = src->lastTileHovered; 
    dst->editing = src->editing; 
    dst->actionID = src->actionID; 
    dst->id = src->id; 
    dst->digit = src->digit; 
    dst->godMode = src->godMode; 
    dst->page = src->page; 
    dst->lastFlag = src->lastFlag; 
    dst->gold = src->gold; 
    dst->autoplay = src->autoplay;
    for (int i = 0; i < tmpSize; ++i) { 
        dst->tmp[i] = src->tmp[i]; 
    }
    dst->unit = src->unit; 
} 

void SetBootType(int id) { 
    struct GlobalSaveInfo info;
    ReadGlobalSaveInfo(&info);
    info.charKnownFlags[0x10] = id; 
    WriteGlobalSaveInfoNoChecksum(&info);
} 

int GetBootType(void) { 
    struct GlobalSaveInfo info;
    ReadGlobalSaveInfo(&info);
    return info.charKnownFlags[0x10]; 
} 

//LYN_REPLACE_CHECK(EventCallGameOverExt);
void EventCallGameOverExt(ProcPtr proc)
{
    Proc_StartBlocking(ProcScr_BmGameOver, proc);
    SetBootType(4); // title screen after game over 
}

#define LGAMECTRL_EXEC_BM_EXT 6  // Directly goto bmmap 
LYN_REPLACE_CHECK(GameControl_CallEraseSaveEventWithKeyCombo);
void GameControl_CallEraseSaveEventWithKeyCombo(ProcPtr proc)
{
    if (gKeyStatusPtr->heldKeys == (L_BUTTON | DPAD_RIGHT | SELECT_BUTTON)) { 
        Proc_Goto(proc, LGAMECTRL_ERASE_SAVE); 
    } 
    else { 
        int var = GetBootType(); 
        switch (var) { 
            case 1: { GmDataInit(); Proc_Goto(proc, LGAMECTRL_EXEC_BM); break; } 
            case 2: { GmDataInit(); Proc_Goto(proc, LGAMECTRL_EXEC_BM_EXT); break; } // Directly goto bmmap
            case 3: { if (IsValidSuspendSave(SAVE_ID_SUSPEND)) { 
                ReadSuspendSave(3);
                //SetNextGameActionId(GAME_ACTION_4);
                Proc_Goto(proc, 8); break; } }  // Resume ch
            default: 
        } 
         
        
    } 
    
    // 8 = resume 
    // 
}


extern int NumberOfPages; 
void RestartDebuggerMenu(DebuggerProc* proc); 
int RestartNow(DebuggerProc* proc); // goto restart label 
void LoopDebuggerProc(DebuggerProc* proc);
void PickupUnitIdle(DebuggerProc* proc); 
void SetupUnitFunc(void); 
int PromoAction(DebuggerProc* proc);
int ArenaAction(DebuggerProc* proc); 
int LevelupAction(DebuggerProc* proc); 
int UnitActionFunc(DebuggerProc* proc); 
void CallPlayerPhase_FinishAction(DebuggerProc* proc);
void ClearActiveUnitStuff(DebuggerProc* proc); 
void PlayerPhase_FinishActionNoCanto(ProcPtr proc);
void CallPlayerPhase_FinishAction(DebuggerProc* proc);
int PlayerPhase_PrepareActionBasic(DebuggerProc* proc);
void PlayerPhase_ApplyUnitMovementWithoutMenu(DebuggerProc* proc);
void EditMapIdle(DebuggerProc* proc); 
void StartPlayerPhaseTerrainWindow(); 
void ChooseTileInit(DebuggerProc* proc);
void ChooseTileIdle(DebuggerProc* proc);
void RenderTilesetRowOnBg2(DebuggerProc* proc);
void DisplayTilesetTile(DebuggerProc* proc, u16* bg, int xTileMap, int yTileMap, int xBmMap, int yBmMap);
void EditMapInit(DebuggerProc* proc);
void InitProc(DebuggerProc* proc);
void EditStatsInit(DebuggerProc* proc);
void EditStatsIdle(DebuggerProc* proc);
void EditItemsInit(DebuggerProc* proc);
void EditItemsIdle(DebuggerProc* proc);
void EditMiscInit(DebuggerProc* proc);
void EditMiscIdle(DebuggerProc* proc);
void RedrawItemMenu(DebuggerProc* proc);
void LoadUnitsIdle(DebuggerProc* proc);
void RedrawLoadMenu(DebuggerProc* proc); 
void LoadUnitsInit(DebuggerProc* proc);
void PutNumberHex(u16 *tm, int color, int number);
void StateInit(DebuggerProc* proc);
void StateIdle(DebuggerProc* proc);
void RedrawStateMenu(DebuggerProc* proc);
void ChStateInit(DebuggerProc* proc);
void ChStateIdle(DebuggerProc* proc);
void EditWExpInit(DebuggerProc* proc);
void EditWExpIdle(DebuggerProc* proc);
void EditSkillsInit(DebuggerProc* proc);
void EditSkillsIdle(DebuggerProc* proc);
void EditSupportsInit(DebuggerProc* proc);
void EditSupportsIdle(DebuggerProc* proc);
u8 CanActiveUnitPromote(void);

#define InitProcLabel 0
#define RestartLabel 1
#define PostActionLabel 2 // ClassChgMenuSelOnPressB 80CDC15 has Proc_Goto(proc, 2) in it, so we make this post action label 2 
#define UnitActionLabel 3 
#define PickupUnitLabel 4
#define ChooseTileLabel 5
#define EditMapLabel 6
#define EditTerrainLabel 7
#define EditTrapLabel 8
#define EditStatsLabel 9
#define EditItemsLabel 10
#define EditMiscLabel 11
#define LoadUnitsLabel 12
#define LevelupLabel 13
#define StateLabel 14
#define ChStateLabel 15
#define WExpLabel 16
#define SkillsLabel 17
#define SupportLabel 18
#define LoopLabel 19
#define ListLabel 20
#define EndLabel 99 

#define ActionID_Promo 1 
#define ActionID_Arena 2 
#define ActionID_Levelup 3 

const struct ProcCmd DebuggerProcCmdIdler[] =
{
    PROC_NAME("DebuggerProcIdler"), 
    PROC_YIELD,
    PROC_REPEAT(LoopDebuggerProc), 
    PROC_END, 
}; 
void SaveProcVarsToIdler(DebuggerProc* proc) { 
    DebuggerProc* procIdler = Proc_Find(DebuggerProcCmdIdler); 
    CopyProcVariables(procIdler, proc); 
    Proc_End(proc); 
} 

const struct ProcCmd DebuggerProcCmd[] =
{
	PROC_NAME("DebuggerProcName"), 
    PROC_YIELD,
    PROC_LABEL(InitProcLabel), 
    //PROC_CALL(InitProc), 
    PROC_LABEL(RestartLabel), // Menu 
    
    PROC_CALL(EndPlayerPhaseSideWindows), 
    PROC_SLEEP(1),
    PROC_WHILE(DoesBMXFADEExist),
    PROC_CALL(SetAllUnitNotBackSprite),
    PROC_CALL(RefreshUnitSprites),
    PROC_CALL(RestartDebuggerMenu), 
    PROC_LABEL(LoopLabel), // Loop indefinitely 
	PROC_REPEAT(LoopDebuggerProc), 
    
    PROC_LABEL(UnitActionLabel), 
    PROC_CALL(PlayerPhase_ApplyUnitMovementWithoutMenu), 
    PROC_WHILE_EXISTS(gProcScr_CamMove),
    PROC_CALL_2(PlayerPhase_PrepareActionBasic), 
    PROC_SLEEP(1),
    PROC_CALL_2(UnitActionFunc),
    
    PROC_LABEL(PostActionLabel), // after action 
    PROC_CALL_2(HandlePostActionTraps),
    PROC_CALL_2(RunPotentialWaitEvents),
    PROC_CALL_2(EnsureCameraOntoActiveUnitPosition),
    PROC_CALL(CallPlayerPhase_FinishAction),
    PROC_GOTO(EndLabel), 
    
    PROC_LABEL(PickupUnitLabel), // Pickup 
    PROC_CALL(StartPlayerPhaseTerrainWindow),
    PROC_CALL(ResetUnitSpriteHover),
    PROC_REPEAT(PickupUnitIdle), 
    PROC_GOTO(EndLabel), 
    
    PROC_LABEL(ChooseTileLabel), // Tile select 
    PROC_CALL(ChooseTileInit), 
    PROC_REPEAT(ChooseTileIdle), 
    
    PROC_LABEL(EditMapLabel), // Map
    PROC_CALL(EditMapInit), 
    PROC_REPEAT(EditMapIdle), 
    PROC_GOTO(EndLabel), 
    
    PROC_LABEL(EditStatsLabel), // Stats 
    PROC_CALL(EditStatsInit), 
    PROC_REPEAT(EditStatsIdle), 
    PROC_GOTO(EndLabel), 

    PROC_LABEL(EditItemsLabel), // Items 
    PROC_CALL(EditItemsInit), 
    PROC_REPEAT(EditItemsIdle), 
    PROC_GOTO(EndLabel), 
    
    PROC_LABEL(EditMiscLabel), // Class etc  
    PROC_CALL(EditMiscInit), 
    PROC_REPEAT(EditMiscIdle), 
    PROC_GOTO(EndLabel), 
    
    PROC_LABEL(LoadUnitsLabel), // Units
    PROC_CALL(LoadUnitsInit), 
    PROC_REPEAT(LoadUnitsIdle), 
    PROC_GOTO(EndLabel), 
    
    PROC_LABEL(LevelupLabel), // Levelup
    //PROC_CALL(LockGame), 
    PROC_SLEEP(5),
    PROC_WHILE(BattleEventEngineExists),
    //PROC_CALL(MapAnmiProc_DisplayDeathFade),
    //PROC_WHILE_EXISTS(ProcScr_MuDeathFade),
    PROC_CALL(DeleteBattleAnimInfoThing),
    PROC_SLEEP(0x1),
    //PROC_CALL(MapAnimProc_DisplayItemStealingPopup),
    //PROC_YIELD,
    PROC_CALL(MapAnim_DisplayExpBar),
    PROC_YIELD,
    //PROC_CALL(DisplayWRankUpPopup),
    //PROC_YIELD,
    PROC_CALL(MapAnim_MoveCameraOntoSubject),
    PROC_SLEEP(0x2),
    //PROC_CALL(UnlockGame),
    PROC_CALL(UpdateActorFromBattle), 
    PROC_CALL(MapAnim_Cleanup),
    PROC_GOTO(RestartLabel), 
    
    PROC_LABEL(StateLabel), // Unit state
    PROC_CALL(StateInit), 
    PROC_REPEAT(StateIdle), 
    PROC_GOTO(EndLabel), 
    
    PROC_LABEL(ChStateLabel), // Ch state
    PROC_CALL(ChStateInit), 
    PROC_REPEAT(ChStateIdle), 
    PROC_GOTO(EndLabel), 

    PROC_LABEL(WExpLabel), // Wexp
    PROC_CALL(EditWExpInit), 
    PROC_REPEAT(EditWExpIdle), 
    PROC_GOTO(EndLabel), 

    PROC_LABEL(SkillsLabel), // Skills
    PROC_CALL(EditSkillsInit),
    PROC_REPEAT(EditSkillsIdle), 
    PROC_GOTO(EndLabel), 

    PROC_LABEL(SupportLabel), // Supports
    PROC_CALL(EditSupportsInit),
    PROC_REPEAT(EditSupportsIdle),
    PROC_GOTO(EndLabel),
    
    PROC_LABEL(EndLabel), 
    PROC_CALL(ClearActiveUnitStuff),
    PROC_CALL(SaveProcVarsToIdler),
    PROC_END,
};

int ShouldAIControlRemainingUnits(void)
{
    DebuggerProc * proc;
    proc = Proc_Find(DebuggerProcCmdIdler);
    if (!proc)
    {
        return false;
    }
    if (proc->autoplay)
    {
        return true;
    }
    return false;
}

// extern void AiPhaseBerserkInit(struct Proc* proc);

// LYN_REPLACE_CHECK(AiPhaseBerserkInit);
// void AiPhaseBerserkInit(struct Proc * proc)
// {
//     int i;

//     gAiState.flags = AI_FLAG_BERSERKED;
//     if (ShouldAIControlRemainingUnits())
//     {
//         gAiState.flags = AI_FLAG_0; // do not attack allies
//     }
//     gAiState.unk7E = -1;

//     for (i = 0; i < 8; ++i)
//         gAiState.unk86[i] = 0; // cmd_result

//     gAiState.specialItemFlags = gAiItemConfigTable[gPlaySt.chapterIndex];

//     UpdateAllPhaseHealingAIStatus();
//     SetupUnitInventoryAIFlags();

//     Proc_StartBlocking(gProcScr_BerserkCpOrder, proc);
// }

// extern void CpOrderBerserkInit(ProcPtr proc);

// LYN_REPLACE_CHECK(CpOrderBerserkInit);
// void CpOrderBerserkInit(ProcPtr proc)
// {
//     int i, aiNum = 0;

//     u32 faction = gPlaySt.faction;
//     int AIControl = ShouldAIControlRemainingUnits();

//     int factionUnitCountLut[3] = { 62, 20, 50 }; // TODO: named constant for those

//     for (i = 0; i < factionUnitCountLut[faction >> 6]; ++i)
//     {
//         struct Unit * unit = GetUnit(faction + i + 1);

//         if (!unit->pCharacterData)
//             continue;

//         if (!AIControl) // all units act this way, even if not berserked
//         {
//             if (unit->statusIndex != UNIT_STATUS_BERSERK)
//             {
//                 continue;
//             }
//         }

//         if (unit->state & (US_HIDDEN | US_UNSELECTABLE | US_DEAD | US_RESCUED | US_HAS_MOVED_AI))
//             continue;

//         gAiState.units[aiNum++] = faction + i + 1;
//     }

//     if (aiNum != 0)
//     {
//         gAiState.units[aiNum] = 0;
//         gAiState.unitIt = gAiState.units;

//         AiDecideMainFunc = AiDecideMain;

//         Proc_StartBlocking(gProcScr_CpDecide, proc);
//     }
// }

//const char* UnitStats
#define NumberOfOptions 9 
#define NumberOfItems 5
#define START_X 19
#define Y_HAND 2
#define NUMBER_X 17
typedef const struct {
  u32 x;
  u32 y;
} LocationTable;
static LocationTable CursorLocationTable[] = {
  //{(NUMBER_X*8) - (0 * 8) - 4, Y_HAND*8},
  {(START_X*8) - (1 * 8) + 4, Y_HAND*8},
  {(START_X*8) - (2 * 8) + 4, Y_HAND*8},
  {(START_X*8) - (3 * 8) + 4, Y_HAND*8},
  {(START_X*8) - (4 * 8) + 4, Y_HAND*8},
  {(START_X*8) - (5 * 8) + 4, Y_HAND*8},
  {(START_X*8) - (6 * 8) + 4, Y_HAND*8}, 
  {(START_X*8) - (7 * 8) + 4, Y_HAND*8}, 
  {(START_X*8) - (8 * 8) + 4, Y_HAND*8}, 
};



#define NumberOfState 32
#define StateWidth 7 
static LocationTable StateCursorLocationTable[] = {
  //{(NUMBER_X*8) - (0 * 8) - 4, Y_HAND*8},
  {8, 16}, {8, 32}, {8, 48}, {8, 64}, {8, 80}, {8, 96}, {8, 112}, {8, 128}, 
  {8+(8*StateWidth), 16}, {8+(8*StateWidth), 32}, {8+(8*StateWidth), 48}, {8+(8*StateWidth), 64}, {8+(8*StateWidth), 80}, {8+(8*StateWidth), 96}, {8+(8*StateWidth), 112}, {8+(8*StateWidth), 128}, 
  {8+(16*StateWidth), 16}, {8+(16*StateWidth), 32}, {8+(16*StateWidth), 48}, {8+(16*StateWidth), 64}, {8+(16*StateWidth), 80}, {8+(16*StateWidth), 96}, {8+(16*StateWidth), 112}, {8+(16*StateWidth), 128}, 
  {8+(24*StateWidth), 16}, {8+(24*StateWidth), 32}, {8+(24*StateWidth), 48}, {8+(24*StateWidth), 64}, {8+(24*StateWidth), 80}, {8+(24*StateWidth), 96}, {8+(24*StateWidth), 112}, {8+(24*StateWidth), 128}, 
};

static const int DigitDecimalTable[] = { 
1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000
}; 

static const int DigitHexTable[] = { 
0x1, 0x10, 0x100, 0x1000, 0x10000, 0x100000, 0x1000000, 0x10000000, 0x7fffffff
}; 

static const int* pDigitTable[2] = { DigitDecimalTable, DigitHexTable }; 

static int GetMaxDigits(int number, int type) { 
	int result = 1; 
    while (number > pDigitTable[type][result]) { result++; } 
	if (result > 9) { result = 9; } 
	return result; 
} 


int GetMostSignificantDigit(int val, int type) { 
	int result = 0; 
    while (val >= pDigitTable[type][result+1]) { result++; } 
	if (result > 9) { result = 9; } 
	return result; 
} 


#define StatWidth 4
void RedrawUnitStatsMenu(DebuggerProc* proc); 

void FixCursorOverflow(void) { 
    int x = gBmSt.playerCursor.x; 
    int y = gBmSt.playerCursor.y; 
    if (x < 0) { gBmSt.playerCursor.x = 0; gActiveUnitMoveOrigin.x = 0; } 
    if (y < 0) { gBmSt.playerCursor.y = 0; gActiveUnitMoveOrigin.y = 0; } 
    if (x >= gBmMapSize.x) { 
        x = gBmMapSize.x - 1;
        gBmSt.playerCursor.x = x; 
        gActiveUnitMoveOrigin.x = x; 
        gActiveUnit->xPos = x; 
    } 
    if (y >= gBmMapSize.y) { 
        y = gBmMapSize.y - 1; 
        gBmSt.playerCursor.y = y; 
        gActiveUnitMoveOrigin.x = y; 
        gActiveUnit->yPos = y; 
    } 

} 

void SomeMenuInit(DebuggerProc* proc) { 
    ResetTextFont();
    SetTextFontGlyphs(0);
//		ChapterStatus_SetupFont((void*)proc);

    BG_Fill(gBG0TilemapBuffer, 0);
    BG_EnableSyncByMask(BG0_SYNC_BIT);
    ResetTextFont();
    SetTextFontGlyphs(0);
    SetTextFont(0);
    ClearBg0Bg1(); 
    ResetText();
}

void EditStatsInit(DebuggerProc* proc) { 
    SomeMenuInit(proc); 
    struct Unit* unit = proc->unit; 
    proc->tmp[0] = unit->maxHP; 
    proc->tmp[1] = unit->curHP; 
    proc->tmp[2] = unit->pow; 
    proc->tmp[3] = unit->skl; 
    proc->tmp[4] = unit->spd; 
    proc->tmp[5] = unit->def; 
    proc->tmp[6] = unit->res; 
    proc->tmp[7] = unit->lck; 
    proc->tmp[8] = unit->_u47; 
    
    
    
    int x = NUMBER_X - StatWidth - 1; 
    int y = Y_HAND - 2; 
    int w = StatWidth + (START_X - NUMBER_X) + 3; 
    int h = (NumberOfOptions * 2) + 2; 
    
    DrawUiFrame(
        BG_GetMapBuffer(1), // back BG
        x, y, w, h,
        TILEREF(0, 0), 0); // style as 0 ? 

    //ClearUiFrame(
    //    BG_GetMapBuffer(1), // front BG 
    //    x, y, w, h);
    
    struct Text* th = gStatScreen.text;
    
    for (int i = 0; i < 15; ++i) { 
        InitText(&th[i], StatWidth);
    } 
    int c = 0; 
    Text_DrawString(&th[c], "Max HP"); c++; 
    Text_DrawString(&th[c], GetStringFromIndex(0x4E9)); c++; 
    Text_DrawString(&th[c], GetStringFromIndex(0x4FE)); c++; 
    Text_DrawString(&th[c], GetStringFromIndex(0x4EC)); c++; 
    Text_DrawString(&th[c], GetStringFromIndex(0x4ED)); c++; 
    Text_DrawString(&th[c], GetStringFromIndex(0x4EF)); c++; 
    Text_DrawString(&th[c], GetStringFromIndex(0x4F0)); c++; 
    Text_DrawString(&th[c], GetStringFromIndex(0x4EE)); c++; 
    Text_DrawString(&th[c], GetStringFromIndex(0x4FF)); c++; 
    
    RedrawUnitStatsMenu(proc);
}

void RedrawUnitStatsMenu(DebuggerProc* proc) { 
	TileMap_FillRect(gBG0TilemapBuffer + TILEMAP_INDEX(NUMBER_X-2, Y_HAND), 9, 2 * NumberOfOptions, 0);
	BG_EnableSyncByMask(BG0_SYNC_BIT);
    //ResetText();
    struct Text* th = gStatScreen.text;
    int x = NUMBER_X - StatWidth; 
    for (int i = 0; i < NumberOfOptions; ++i) { 
        PutText(&th[i], gBG0TilemapBuffer + TILEMAP_INDEX(x, (Y_HAND-1) + (i*2)));
    } 


    for (int i = 0; i < NumberOfOptions; ++i) { 
        PutNumber(gBG0TilemapBuffer + TILEMAP_INDEX(START_X, Y_HAND - 1 + (i*2)), TEXT_COLOR_SYSTEM_GOLD, proc->tmp[i]); 
    } 

	BG_EnableSyncByMask(BG0_SYNC_BIT);

}


static const u16 sSprite_VertHand[] = {
    1,
    0x0002, 0x4000, 0x0006
};
static const u8 sHandVOffsetLookup[] = {
    0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 2, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 3, 3, 2, 2, 2, 1, 1, 1, 1,
};
extern int sPrevHandClockFrame; 
extern struct Vec2 sPrevHandScreenPosition; 
static void DisplayVertUiHand(int x, int y)
{
    if ((int)(GetGameClock() - 1) == sPrevHandClockFrame)
    {
        x = (x + sPrevHandScreenPosition.x) >> 1;
        y = (y + sPrevHandScreenPosition.y) >> 1;
    }

    sPrevHandScreenPosition.x = x;
    sPrevHandScreenPosition.y = y;
    sPrevHandClockFrame = GetGameClock();

    y += (sHandVOffsetLookup[Mod(GetGameClock(), ARRAY_COUNT(sHandVOffsetLookup))] - 14);
    PutSprite(2, x, y, sSprite_VertHand, 0);
}

const s8 StatCapLookup[] = { 
    255, 255, 63, 63, 63, 63, 63, 63, 63,
}; 

void SaveStats(DebuggerProc* proc) { 
    struct Unit* unit = proc->unit; 
    //int hpDiff = proc->tmp[0] - unit->maxHP; 
    unit->maxHP = proc->tmp[0]; 
    //if (hpDiff) { unit->curHP += hpDiff; } 
    unit->curHP = proc->tmp[1]; 
    unit->pow = proc->tmp[2]; 
    unit->skl = proc->tmp[3]; 
    unit->spd = proc->tmp[4]; 
    unit->def = proc->tmp[5]; 
    unit->res = proc->tmp[6]; 
    unit->lck = proc->tmp[7]; 
    unit->_u47 = proc->tmp[8];  // Mag
} 

void SaveItems(DebuggerProc* proc) { 

    struct Unit* unit = proc->unit; 
    for (int i = 0; i < NumberOfItems; ++i) { 
        unit->items[i] = proc->tmp[i]; 
    }
    
    UnitRemoveInvalidItems(unit);


} 

extern struct KeyStatusBuffer sKeyStatusBuffer;
void EditStatsIdle(DebuggerProc* proc) { 
    
	//DisplayVertUiHand(CursorLocationTable[proc->digit].x, CursorLocationTable[proc->digit].y); // 6 is the tile of the downwards hand 	
	u16 keys = sKeyStatusBuffer.repeatedKeys; 
    if (keys & B_BUTTON) { //press B to not save stats 
        Proc_Goto(proc, RestartLabel);
        m4aSongNumStart(0x6B); 
    };
    if ((keys & START_BUTTON)||(keys & A_BUTTON)) { //press A or Start to update stats and continue 
        SaveStats(proc); 
        Proc_Goto(proc, RestartLabel);
        m4aSongNumStart(0x6B); 
    };
    if (proc->editing) { 
        DisplayVertUiHand(CursorLocationTable[proc->digit].x, (Y_HAND - 1 + (proc->id * 2)) * 8); 
        int max;	
        if (proc->id == 0 || proc->id == 1)
            max = 255;
        else
            max = StatCapLookup[proc->id]; 
        int min = 0; 
        int max_digits = GetMaxDigits(max, 0); 
        
        if (keys & DPAD_RIGHT) {
          if (proc->digit > 0) { proc->digit--; }
          else { proc->digit = max_digits - 1; proc->editing = false; } 
          RedrawUnitStatsMenu(proc);
        }
        if (keys & DPAD_LEFT) {
          if (proc->digit < (max_digits-1)) { proc->digit++; }
          else { proc->digit = 0; proc->editing = false; } 
          RedrawUnitStatsMenu(proc);
        }
        
        if (keys & DPAD_UP) {
            if (proc->tmp[proc->id] == max) { proc->tmp[proc->id] = min; } 
            else { 
                proc->tmp[proc->id] += DigitDecimalTable[proc->digit]; 
                if (proc->tmp[proc->id] > max) { proc->tmp[proc->id] = max; } 
            } 
            RedrawUnitStatsMenu(proc); 
        }
        if (keys & DPAD_DOWN) {
            
            if (proc->tmp[proc->id] == min) { proc->tmp[proc->id] = max; } 
            else { 
                proc->tmp[proc->id] -= DigitDecimalTable[proc->digit]; 
                if (proc->tmp[proc->id] < min) { proc->tmp[proc->id] = min; } 
            } 
            
            RedrawUnitStatsMenu(proc); 
        }
    }
    else { 
        DisplayUiHand(CursorLocationTable[0].x - ((StatWidth + 2) * 8), (Y_HAND - 1 + (proc->id * 2)) * 8);
        if (keys & DPAD_RIGHT) {
            proc->digit = 1; 
          proc->editing = true; 
        }
        if (keys & DPAD_LEFT) {
          proc->digit = 0; 
          proc->editing = true; 
        }
        
        if (keys & DPAD_UP) {
            proc->id--; 
            if (proc->id < 0) { proc->id = NumberOfOptions - 1; } 
            RedrawUnitStatsMenu(proc); 
        }
        if (keys & DPAD_DOWN) {
            proc->id++; 
            if (proc->id >= NumberOfOptions) { proc->id = 0; } 
            
            RedrawUnitStatsMenu(proc); 
        }
    } 
} 

#define WExpWidth 11
#define WExpOptions 8 
void RedrawUnitWExpMenu(DebuggerProc* proc); 
void EditWExpInit(DebuggerProc* proc) { 
    SomeMenuInit(proc); 
    struct Unit* unit = proc->unit; 
    for (int i = 0; i < WExpOptions; ++i) { 
        proc->tmp[i] = unit->ranks[i]; 
    } 
    
    
    int x = NUMBER_X - WExpWidth - 1; 
    int y = Y_HAND - 1; 
    int w = WExpWidth + (START_X - NUMBER_X) + 3; 
    int h = (WExpOptions * 2) + 2; 
    
    DrawUiFrame(
        BG_GetMapBuffer(1), // back BG
        x, y, w, h,
        TILEREF(0, 0), 0); // style as 0 ? 
    BG_EnableSyncByMask(BG1_SYNC_BIT); 
    
    struct Text* th = gStatScreen.text;
    
    for (int i = 0; i < 15; ++i) { 
        InitText(&th[i], WExpWidth);
    } 
    for (int i = 0; i < WExpOptions; ++i) { 
        x = Text_GetCursor(&th[i]); x++; Text_SetCursor(&th[i], x); 
        Text_DrawString(&th[i], GetStringFromIndex(0x505 + i));
    } 
    RedrawUnitWExpMenu(proc);
}

void DebuggerDisplayWeaponExp(int num, int x, int y, int wtype, int wexp)
{
    int progress, progressMax, color;

    //int wexp = gStatScreen.unit->ranks[wtype];

    // Display weapon type icon
    DrawIcon(gBG0TilemapBuffer + TILEMAP_INDEX(x, y),
        0x70 + wtype, // TODO: icon id definitions
        TILEREF(0, 2));
    
    x += 4; 

    color = wexp >= WPN_EXP_S
        ? TEXT_COLOR_SYSTEM_GREEN
        : TEXT_COLOR_SYSTEM_BLUE;

    // Display rank letter
    PutSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(x + 4, y),
        color,
        GetDisplayRankStringFromExp(wexp));

    GetWeaponExpProgressState(wexp, &progress, &progressMax);

    DrawStatBarGfx(0x180 + num*6, 5,
        //gUiTmScratchC + TILEMAP_INDEX(x + 2, y + 1), TILEREF(0, STATSCREEN_BGPAL_6),
        gBG1TilemapBuffer + TILEMAP_INDEX(x + 2, y + 1), TILEREF(0, 1),
        0x22, (progress*34)/(progressMax-1), 0);
}

void RedrawUnitWExpMenu(DebuggerProc* proc) { 
	TileMap_FillRect(gBG0TilemapBuffer + TILEMAP_INDEX(NUMBER_X-2, Y_HAND), 9, 2 * WExpOptions, 0);
	BG_EnableSyncByMask(BG0_SYNC_BIT|BG1_SYNC_BIT);
    //ResetText();
    int c = 0; 
    struct Text* th = gStatScreen.text;

    
    c = 0; 
    int x = (NUMBER_X - WExpWidth) + 2; 
    PutText(&th[c], gBG0TilemapBuffer + TILEMAP_INDEX(x, Y_HAND + (c*2))); c++; 
    PutText(&th[c], gBG0TilemapBuffer + TILEMAP_INDEX(x, Y_HAND + (c*2))); c++; 
    PutText(&th[c], gBG0TilemapBuffer + TILEMAP_INDEX(x, Y_HAND + (c*2))); c++; 
    PutText(&th[c], gBG0TilemapBuffer + TILEMAP_INDEX(x, Y_HAND + (c*2))); c++; 
    PutText(&th[c], gBG0TilemapBuffer + TILEMAP_INDEX(x, Y_HAND + (c*2))); c++; 
    PutText(&th[c], gBG0TilemapBuffer + TILEMAP_INDEX(x, Y_HAND + (c*2))); c++; 
    PutText(&th[c], gBG0TilemapBuffer + TILEMAP_INDEX(x, Y_HAND + (c*2))); c++; 
    PutText(&th[c], gBG0TilemapBuffer + TILEMAP_INDEX(x, Y_HAND + (c*2))); c++; 


    for (int i = 0; i < WExpOptions; ++i) { 
        DebuggerDisplayWeaponExp(i, x-2, Y_HAND + (i*2), i, proc->tmp[i]); // first i is bar ID, second i is wep type ID 
        PutNumber(gBG0TilemapBuffer + TILEMAP_INDEX(START_X, Y_HAND + (i*2)), TEXT_COLOR_SYSTEM_GOLD, proc->tmp[i]); 
    } 
    
    
    SetBlendTargetA(0, 0, 1, 0, 0);
    //SetBlendTargetB(0, 0, 0, 0, 1);
	BG_EnableSyncByMask(BG0_SYNC_BIT|BG1_SYNC_BIT);

}


void SaveWExp(DebuggerProc* proc) { 
    struct Unit* unit = proc->unit; 
    for (int i = 0; i < WExpOptions; ++i) { 
    unit->ranks[i] = proc->tmp[i]; 
    } 
} 

void ClearTilesetRow(DebuggerProc* proc); 
void EditWExpIdle(DebuggerProc* proc) { 
    
	//DisplayVertUiHand(CursorLocationTable[proc->digit].x, CursorLocationTable[proc->digit].y); // 6 is the tile of the downwards hand 	
	u16 keys = sKeyStatusBuffer.repeatedKeys; 
    if (keys & B_BUTTON) { //press B to not save WExp 
        ClearTilesetRow(proc); 
        Proc_Goto(proc, RestartLabel);
        m4aSongNumStart(0x6B); 
    };
    if ((keys & START_BUTTON)||(keys & A_BUTTON)) { //press A or Start to update WExp and continue 
        SaveWExp(proc); 
        ClearTilesetRow(proc); 
        Proc_Goto(proc, RestartLabel);
        m4aSongNumStart(0x6B); 
    };
    if (proc->editing) { 
        DisplayVertUiHand(CursorLocationTable[proc->digit].x, (Y_HAND + (proc->id * 2)) * 8); 	
        int max = 251; 
        int min = 0; 
        int max_digits = GetMaxDigits(max, 0); 
        
        if (keys & DPAD_RIGHT) {
          if (proc->digit > 0) { proc->digit--; }
          else { proc->digit = max_digits - 1; proc->editing = false; } 
          RedrawUnitWExpMenu(proc);
        }
        if (keys & DPAD_LEFT) {
          if (proc->digit < (max_digits-1)) { proc->digit++; }
          else { proc->digit = 0; proc->editing = false; } 
          RedrawUnitWExpMenu(proc);
        }
        
        if (keys & DPAD_UP) {
            if (proc->tmp[proc->id] == max) { proc->tmp[proc->id] = min; } 
            else { 
                proc->tmp[proc->id] += DigitDecimalTable[proc->digit]; 
                if (proc->tmp[proc->id] > max) { proc->tmp[proc->id] = max; } 
            } 
            RedrawUnitWExpMenu(proc); 
        }
        if (keys & DPAD_DOWN) {
            
            if (proc->tmp[proc->id] == min) { proc->tmp[proc->id] = max; } 
            else { 
                proc->tmp[proc->id] -= DigitDecimalTable[proc->digit]; 
                if (proc->tmp[proc->id] < min) { proc->tmp[proc->id] = min; } 
            } 
            
            RedrawUnitWExpMenu(proc); 
        }
    }
    else { 
        DisplayUiHand(CursorLocationTable[0].x - ((WExpWidth + 2) * 8), (Y_HAND + (proc->id * 2)) * 8);
        if (keys & DPAD_RIGHT) {
            proc->digit = 1; 
          proc->editing = true; 
        }
        if (keys & DPAD_LEFT) {
          proc->digit = 0; 
          proc->editing = true; 
        }
        
        if (keys & DPAD_UP) {
            proc->id--; 
            if (proc->id < 0) { proc->id = WExpOptions - 1; } 
            RedrawUnitWExpMenu(proc); 
        }
        if (keys & DPAD_DOWN) {
            proc->id++; 
            if (proc->id >= WExpOptions) { proc->id = 0; } 
            
            RedrawUnitWExpMenu(proc); 
        }
    } 
} 

#define SkillsWidth 12

#ifdef CONFIG_TURN_ON_ALL_SKILLS
    #define SkillsLearnable 5
#else
    #define SkillsLearnable 7
#endif

void RedrawUnitSkillsMenu(DebuggerProc* proc);
void EditSkillsInit(DebuggerProc* proc) { 
    SomeMenuInit(proc); 
    LoadIconPalettes(4);
    struct Unit* unit = proc->unit; 

#ifdef CONFIG_TURN_ON_ALL_SKILLS
    u64 buffer = 0;
    for (int i = 0; i < 7; ++i)
        buffer |= ((u64)unit->supports[i]) << (8 * i);

    for (int i = 0; i < SkillsLearnable; ++i)
        proc->tmp[i] = (buffer >> (i * 10)) & 0x3FF;
#else
    for (int i = 0; i < SkillsLearnable; ++i)
        proc->tmp[i] = unit->supports[i];
#endif

    
    int x = NUMBER_X - SkillsWidth + 1; 
    int y = Y_HAND - 1; 
    int w = SkillsWidth + (START_X - NUMBER_X) + 3; 
    int h = (SkillsLearnable * 2) + 2; 
    
    DrawUiFrame(
        BG_GetMapBuffer(1), // back BG
        x, y, w, h,
        TILEREF(0, 0), 0); // style as 0 ? 

    //ClearUiFrame(
    //    BG_GetMapBuffer(1), // front BG 
    //    x, y, w, h);
    
    struct Text* th = gStatScreen.text;
    
    for (int i = 0; i < 15; ++i) { 
        InitText(&th[i], SkillsWidth+2);
        Text_DrawString(&th[i], ""); 
    } 
    
    int uid; 
    for (int i = 0; i < SkillsLearnable; ++i) { 
        uid = unit->supports[i]; 
        if (uid) { 
        Text_DrawString(&th[i], GetSkillNameStr(uid));
        } 
    } 
    RedrawUnitSkillsMenu(proc);
}

enum icon_sheet_idx {
    ICON_SHEET_VANILLA,
    ICON_SHEET_AFFIN,
    ICON_SHEET_MOUNT,
    ICON_SHEET_WTYPE,
    ICON_SHEET_COMBATART,
    ICON_SHEET_SKILL0,
    ICON_SHEET_SKILL1,
    ICON_SHEET_SKILL2,
    ICON_SHEET_SKILL3,

    ICON_SHEET_AMT = 16
};

#define SKILL_ICON(sid)   ((ICON_SHEET_SKILL0 << 8) + (sid))

void RedrawUnitSkillsMenu(DebuggerProc* proc) { 
    TileMap_FillRect(gBG0TilemapBuffer + TILEMAP_INDEX(NUMBER_X-2, Y_HAND), 9, 2 * SkillsLearnable, 0);
    BG_EnableSyncByMask(BG0_SYNC_BIT);
    ResetIconGraphics(); // Add this to reset icon state

    struct Text* th = gStatScreen.text;
    int x = NUMBER_X - SkillsWidth + 2;

    // Clear and redraw all skill name texts
    for (int i = 0; i < SkillsLearnable; ++i) {
        ClearText(&th[i]);
        // Only draw name if skill ID is non-zero
        if (proc->tmp[i]) {
            // Add extra space at start of text for icon
            Text_SetCursor(&th[i], 2);
            Text_DrawString(&th[i], GetSkillNameStr(proc->tmp[i]));
        }
        PutText(&th[i], gBG0TilemapBuffer + TILEMAP_INDEX(x+2, Y_HAND + (i*2)));

        // Draw skill IDs
        PutNumber(gBG0TilemapBuffer + TILEMAP_INDEX(START_X+2, Y_HAND + (i*2)), 
        TEXT_COLOR_SYSTEM_GOLD, proc->tmp[i]);

        // Draw skill icon
        DrawIcon(TILEMAP_LOCATED(gBG0TilemapBuffer, x, Y_HAND + (i*2)), SKILL_ICON(proc->tmp[i]), 0x4000);
    }

    BG_EnableSyncByMask(BG0_SYNC_BIT);
}

void SaveSkills(DebuggerProc* proc) { 
    struct Unit* unit = proc->unit; 

#ifdef CONFIG_TURN_ON_ALL_SKILLS
    // Compose the 5 skill IDs into a bit buffer
    u64 bitbuf = 0;
    for (int i = 0; i < SkillsLearnable; ++i) {
        bitbuf |= ((u64)(proc->tmp[i] & 0x3FF)) << (i * 10); // store 10 bits per skill
    }

    // Write the packed buffer into unit->supports[7]
    for (int i = 0; i < 7; ++i) {
        unit->supports[i] = (bitbuf >> (i * 8)) & 0xFF;
    }
#else
    for (int i = 0; i < SkillsLearnable; ++i) { 
        unit->supports[i] = proc->tmp[i]; 
    } 
#endif

    ResetSkillLists();
} 

#ifdef CONFIG_TURN_ON_ALL_SKILLS
    const int max = 0x3FF;
#else
    const int max = 0xFF;
#endif

struct ProcHelpBoxIntroString {
    /* 00 */ PROC_HEADER;

    /* 29 */ u8 _pad[0x54-0x29];
    /* 54 */ char* string; 

    /* 58 */ int item;
    /* 5C */ int msg;
    /* 60 */ int unk_60;
    /* 64 */ s16 pretext_lines; /* lines for  prefix */
};

extern void HelpBoxSetupstringLines(struct ProcHelpBoxIntro* proc); 
extern void HelpBoxDrawstring(struct ProcHelpBoxIntro* proc); 
void HelpBoxIntroDrawTextsString(struct ProcHelpBoxIntroString * proc); 

struct ProcCmd const ProcScr_HelpBoxIntroString[] = {
    PROC_SLEEP(6),

    PROC_REPEAT(HelpBoxSetupstringLines),
    PROC_REPEAT(HelpBoxDrawstring),

    PROC_CALL(HelpBoxIntroDrawTextsString),

    PROC_END,
};

// extern signed char sMsgString[0x1000];
// extern struct MsgBuffer sMsgString;

extern int sActiveMsg; 
void LoadStringIntoBuffer(char* a) {      
    sActiveMsg = 0; 
    signed char messageString[0x1000];
    for (int i = 0; i < 50; ++i) { 
        messageString[i] = a[i];
        if (!a[i]) { 
        break; }  
    } 
    SetMsgTerminator(messageString);
} 

void StartHelpBoxTextInitWithString(int item, int msgId, char* string)
{
    struct ProcHelpBoxIntroString * proc = Proc_Start(ProcScr_HelpBoxIntroString, PROC_TREE_3);

    proc->item = item;
    proc->msg = msgId;
    proc->string = string; 
}

void HelpBoxIntroDrawTextsString(struct ProcHelpBoxIntroString * proc)
{
    struct HelpBoxScrollProc * otherProc;
    int textSpeed;

    SetTextFont(&gHelpBoxSt.font);

    SetTextFontGlyphs(1);

    Text_SetColor(&gHelpBoxSt.text[0], 6);
    Text_SetColor(&gHelpBoxSt.text[1], 6);
    Text_SetColor(&gHelpBoxSt.text[2], 6);

    SetTextFont(0);

    Proc_EndEach(gProcScr_HelpBoxTextScroll);

    otherProc = Proc_Start(gProcScr_HelpBoxTextScroll, PROC_TREE_3);
    otherProc->font = &gHelpBoxSt.font;

    otherProc->texts[0] = &gHelpBoxSt.text[0];
    otherProc->texts[1] = &gHelpBoxSt.text[1];
    otherProc->texts[2] = &gHelpBoxSt.text[2];

    otherProc->pretext_lines = proc->pretext_lines;

    //GetStringFromIndex(proc->msg);
    LoadStringIntoBuffer(proc->string); 

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

void ApplyHelpBoxContentSizeString(struct HelpBoxProc* proc, int width, int height, char* string)
{
    width = 0xF0 & (width + 15); // align to 16 pixel multiple

    switch (GetHelpBoxItemInfoKind(proc->item))
    {

    case 1: // weapon
        if (width < 0x90)
            width = 0x90;

        if (GetStringTextLen(string) > 8)
            height += 0x20;
        else
            height += 0x10;

        break;
    
    case 2: // staff
        if (width < 0x60)
            width = 0x60;

        height += 0x10;

        break;

    case 3: // save stuff
        width = 0x80;
        height += 0x10;

        break;

    } // switch (GetHelpBoxItemInfoKind(proc->item))

    proc->wBoxFinal = width;
    proc->hBoxFinal = height;
}


static void StartHelpBoxString(int x, int y, char* string)
{
    sMutableHbi.adjUp    = NULL;
    sMutableHbi.adjDown  = NULL;
    sMutableHbi.adjLeft  = NULL;
    sMutableHbi.adjRight = NULL;

    sMutableHbi.xDisplay = x;
    sMutableHbi.yDisplay = y;
    sMutableHbi.mid      = 0x505; // default text ID 

    sMutableHbi.redirect = NULL;
    sMutableHbi.populate = NULL;

    sHbOrigin.x = 0;
    sHbOrigin.y = 0;
    
    const struct HelpBoxInfo* info = &sMutableHbi; 
    struct HelpBoxProc* proc;
    int wContent, hContent;
    LoadStringIntoBuffer(string); 

    proc = (void*) Proc_Find(gProcScr_HelpBox);

    if (!proc)
    {
        proc = (void*) Proc_Start(gProcScr_HelpBox, PROC_TREE_3);

        proc->unk52 = false;

        SetHelpBoxInitPosition(proc, info->xDisplay, info->yDisplay);
        ResetHelpBoxInitSize(proc);
    }
    else
    {
        proc->xBoxInit = proc->xBox;
        proc->yBoxInit = proc->yBox;

        proc->wBoxInit = proc->wBox;
        proc->hBoxInit = proc->hBox;
    }

    proc->info = info;

    proc->timer    = 0;
    proc->timerMax = 12;

    proc->item = 0;
    proc->mid = info->mid;

    if (proc->info->populate)
        proc->info->populate(proc);

    SetTextFontGlyphs(1);
    GetStringTextBox(string, &wContent, &hContent);
    SetTextFontGlyphs(0);

    ApplyHelpBoxContentSizeString(proc, wContent, hContent, string);
    ApplyHelpBoxPosition(proc, info->xDisplay, info->yDisplay);

    ClearHelpBoxText();
    StartHelpBoxTextInitWithString(proc->item, proc->mid, string);

    sLastHbi = info;
}

void EditSkillsIdle(DebuggerProc* proc) { 
    
	//DisplayVertUiHand(CursorLocationTable[proc->digit].x, CursorLocationTable[proc->digit].y); // 6 is the tile of the downwards hand 	
	u16 keys = sKeyStatusBuffer.repeatedKeys; 
    if (keys & B_BUTTON) {
        // Otherwise, close the Supports/Skills window
        struct HelpBoxProc* proc_helpbox = (void*) Proc_Find(gProcScr_HelpBox);
        if (proc_helpbox)
            CloseHelpBox();
        else
        {
            Proc_Goto(proc, RestartLabel);
            m4aSongNumStart(0x6B);
        }
    }
    if ((keys & START_BUTTON)||(keys & A_BUTTON)) { // press A or Start to update Supports and continue
        CloseHelpBox(); 
        SaveSkills(proc); 
        Proc_Goto(proc, RestartLabel);
        m4aSongNumStart(0x6B); 
    };
    if (proc->editing) { 
        DisplayVertUiHand(CursorLocationTable[proc->digit].x+16, (Y_HAND + (proc->id * 2)) * 8); 	

        int min = 0; 
        int max_digits = GetMaxDigits(max, 0); 
        
        if (keys & DPAD_RIGHT) {
          if (proc->digit > 0) { proc->digit--; }
          else { proc->digit = max_digits - 1; proc->editing = false; } 
          RedrawUnitSkillsMenu(proc);
        }
        if (keys & DPAD_LEFT) {
          if (proc->digit < (max_digits-1)) { proc->digit++; }
          else { proc->digit = 0; proc->editing = false; } 
          RedrawUnitSkillsMenu(proc);
        }
        
        if (keys & DPAD_UP) {
            if (proc->tmp[proc->id] == max) { proc->tmp[proc->id] = min; } 
            else { 
                proc->tmp[proc->id] += DigitDecimalTable[proc->digit];
                
                if (proc->tmp[proc->id] > max) { proc->tmp[proc->id] = max; } 
            } 
            RedrawUnitSkillsMenu(proc); 

            if (Proc_Find(gProcScr_HelpBox)) {
                const u16 skillId = proc->tmp[proc->id];
                u16 msg = GetSkillDescMsg(skillId);
                char * skillDesc = GetStringFromIndex(msg); // Clear any leftover tile data     
                StartHelpBoxString(11 * 8, 10 * 8, skillDesc);
            }
        }
        if (keys & DPAD_DOWN) {
            
            if (proc->tmp[proc->id] == min) { proc->tmp[proc->id] = max; } 
            else { 
                proc->tmp[proc->id] -= DigitDecimalTable[proc->digit]; 
                if (proc->tmp[proc->id] < min) { proc->tmp[proc->id] = min; } 
            } 
            
            RedrawUnitSkillsMenu(proc); 

            if (Proc_Find(gProcScr_HelpBox)) {
                const u16 skillId = proc->tmp[proc->id];
                u16 msg = GetSkillDescMsg(skillId);
                char * skillDesc = GetStringFromIndex(msg); // Clear any leftover tile data     
                StartHelpBoxString(11 * 8, 10 * 8, skillDesc);
            }
        }

        if (keys & R_BUTTON) {
            const u16 skillId = proc->tmp[proc->id];
            u16 msg = GetSkillDescMsg(skillId);
            char * skillDesc = GetStringFromIndex(msg); // Clear any leftover tile data     
            LoadHelpBoxGfx(NULL, -1); // TODO: default constants?
            StartHelpBoxString(11 * 8, 10 * 8, skillDesc);
        }
    }
    else { 
        DisplayUiHand(CursorLocationTable[0].x - ((SkillsWidth + 2) * 7), (Y_HAND + (proc->id * 2)) * 8);
        if (keys & DPAD_RIGHT) {
            proc->digit = 1; 
          proc->editing = true; 
        }
        if (keys & DPAD_LEFT) {
          proc->digit = 0; 
          proc->editing = true; 
        }
        
        if (keys & DPAD_UP) {
            proc->id--; 
            if (proc->id < 0) { proc->id = SkillsLearnable - 1; } 
            RedrawUnitSkillsMenu(proc);

            if (Proc_Find(gProcScr_HelpBox)) {
                const u16 skillId = proc->tmp[proc->id];
                u16 msg = GetSkillDescMsg(skillId);
                char * skillDesc = GetStringFromIndex(msg); // Clear any leftover tile data     
                StartHelpBoxString(11 * 8, 10 * 8, skillDesc);
            }

        }
        if (keys & DPAD_DOWN) {
            proc->id++;
            if (proc->id >= SkillsLearnable) { proc->id = 0; }    
            RedrawUnitSkillsMenu(proc); 

            if (Proc_Find(gProcScr_HelpBox)) {
                const u16 skillId = proc->tmp[proc->id];
                u16 msg = GetSkillDescMsg(skillId);
                char * skillDesc = GetStringFromIndex(msg); // Clear any leftover tile data     
                StartHelpBoxString(11 * 8, 10 * 8, skillDesc);
            }
            
        }

        if (keys & R_BUTTON) {
            const u16 skillId = proc->tmp[proc->id];
            u16 msg = GetSkillDescMsg(skillId);
            char * skillDesc = GetStringFromIndex(msg); // Clear any leftover tile data     
            LoadHelpBoxGfx(NULL, -1); // TODO: default constants?
            StartHelpBoxString(11 * 8, 10 * 8, skillDesc);
        }
    } 
} 

#define SupportsWidth 10
#define SupportsOptions 7  // Number of possible supports per unit
void RedrawUnitSupportsMenu(DebuggerProc* proc);
void EditSupportsInit(DebuggerProc* proc) {
    SomeMenuInit(proc);
    struct Unit* unit = proc->unit;
    struct NewBwl* bwl = GetNewBwl(UNIT_CHAR_ID(unit));

    // Initialize tmp array with current support values
    if (bwl) {
        for (int i = 0; i < SupportsOptions; ++i) {
            proc->tmp[i] = bwl->supports[i];
        }
    } else {
        for (int i = 0; i < SupportsOptions; ++i) {
            proc->tmp[i] = 0;
        }
    }
    
    int x = NUMBER_X - SupportsWidth - 1;
    int y = Y_HAND - 1;
    int w = SupportsWidth + (START_X - NUMBER_X) + 3;
    int h = (SupportsOptions * 2) + 2;
    
    DrawUiFrame(
        BG_GetMapBuffer(1),
        x, y, w, h,
        TILEREF(0, 0), 0);

    struct Text* th = gStatScreen.text;
    
    for (int i = 0; i < 15; ++i) {
        InitText(&th[i], SupportsWidth);
        Text_DrawString(&th[i], "");
    }

    if (unit->pCharacterData->pSupportData)
    {
        int uid;
        for (int i = 0; i < SupportsOptions; ++i)
        {
            uid = unit->pCharacterData->pSupportData->characters[i];
            if (uid)
            {
                Text_DrawString(&th[i], GetStringFromIndex(GetCharacterData(uid)->nameTextId));
            }
        }
    }
    RedrawUnitSupportsMenu(proc);
}

void RedrawUnitSupportsMenu(DebuggerProc* proc) {
    TileMap_FillRect(gBG0TilemapBuffer + TILEMAP_INDEX(NUMBER_X-2, Y_HAND), 9, 2 * SupportsOptions, 0);
    BG_EnableSyncByMask(BG0_SYNC_BIT);

    struct Text* th = gStatScreen.text;
    struct Unit* unit = proc->unit;
    int x = NUMBER_X - SupportsWidth;

    // Clear and redraw all support name texts
    for (int i = 0; i < SupportsOptions; ++i) {
        ClearText(&th[i]);
        if (unit->pCharacterData->pSupportData) {
            int uid = unit->pCharacterData->pSupportData->characters[i];
            if (uid) {
                Text_DrawString(&th[i], GetStringFromIndex(GetCharacterData(uid)->nameTextId));
            }
        }
        PutText(&th[i], gBG0TilemapBuffer + TILEMAP_INDEX(x, Y_HAND + (i*2)));
        
        // Draw support values
        PutNumber(gBG0TilemapBuffer + TILEMAP_INDEX(START_X, Y_HAND + (i*2)), 
                TEXT_COLOR_SYSTEM_GOLD, proc->tmp[i]);
    }

    BG_EnableSyncByMask(BG0_SYNC_BIT);
}

void SaveSupports(DebuggerProc* proc) {
    struct Unit* unit = proc->unit;
    struct NewBwl* bwl = GetNewBwl(UNIT_CHAR_ID(unit));
    if (!bwl)
        return;
        
    // Save the support values back to BWL
    for (int i = 0; i < SupportsOptions; ++i) {
        if (unit->pCharacterData->pSupportData && 
            unit->pCharacterData->pSupportData->characters[i]) {
            bwl->supports[i] = proc->tmp[i];
        }
    }
}

void EditSupportsIdle(DebuggerProc* proc) {
    u16 keys = sKeyStatusBuffer.repeatedKeys;

    if (keys & B_BUTTON) {
        Proc_Goto(proc, RestartLabel);
        m4aSongNumStart(0x6B);
        return;
    }

    if ((keys & START_BUTTON)||(keys & A_BUTTON)) {
        SaveSupports(proc);
        Proc_Goto(proc, RestartLabel);
        m4aSongNumStart(0x6B);
        return; 
    }

    if (proc->editing) { 
        DisplayVertUiHand(CursorLocationTable[proc->digit].x, (Y_HAND + (proc->id * 2)) * 8); 	
        int max = 255; 
        int min = 0; 
        int max_digits = GetMaxDigits(max, 0); 
        
        if (keys & DPAD_RIGHT) {
          if (proc->digit > 0) { proc->digit--; }
          else { proc->digit = max_digits - 1; proc->editing = false; } 
          RedrawUnitSupportsMenu(proc);
        }
        if (keys & DPAD_LEFT) {
          if (proc->digit < (max_digits-1)) { proc->digit++; }
          else { proc->digit = 0; proc->editing = false; } 
          RedrawUnitSupportsMenu(proc);
        }
        
        if (keys & DPAD_UP) {
            if (proc->tmp[proc->id] == max) { proc->tmp[proc->id] = min; } 
            else { 
                proc->tmp[proc->id] += DigitDecimalTable[proc->digit]; 
                if (proc->tmp[proc->id] > max) { proc->tmp[proc->id] = max; } 
            } 
            RedrawUnitSupportsMenu(proc); 
        }
        if (keys & DPAD_DOWN) {
            
            if (proc->tmp[proc->id] == min) { proc->tmp[proc->id] = max; } 
            else { 
                proc->tmp[proc->id] -= DigitDecimalTable[proc->digit]; 
                if (proc->tmp[proc->id] < min) { proc->tmp[proc->id] = min; } 
            } 
            
            RedrawUnitSupportsMenu(proc); 
        }
    }
    else { 
        DisplayUiHand(CursorLocationTable[0].x - ((SupportsWidth + 2) * 8), (Y_HAND + (proc->id * 2)) * 8);
        if (keys & DPAD_RIGHT) {
            proc->digit = 1; 
          proc->editing = true; 
        }
        if (keys & DPAD_LEFT) {
          proc->digit = 0; 
          proc->editing = true; 
        }
        
        if (keys & DPAD_UP) {
            proc->id--; 
            if (proc->id < 0) { proc->id = SupportsOptions - 1; } 
            RedrawUnitSupportsMenu(proc); 
        }
        if (keys & DPAD_DOWN) {
            proc->id++; 
            if (proc->id >= SupportsOptions) { proc->id = 0; } 
            
            RedrawUnitSupportsMenu(proc); 
        }
    };
}

u8 EditSupportsNow(struct MenuProc* menu, struct MenuItemProc* menuItem) {
    DebuggerProc* proc = Proc_Find(DebuggerProcCmd);
    Proc_Goto(proc, SupportLabel);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}




static const char states[32][16] = { 
"Acting",
"Acted",
"Dead",
"Undeployed",
"Rescuing",
"Rescued",
"Cantoed",
"Under roof",
"Spotted",
"Concealed",
"AI decided",
"In ballista",
"Drop item",
"Afa's drops",
"Solo anim1",
"Solo anim2",
"Escaped",
"Arena 1",
"Arena 2",
"Super arena",
"Unk 25",
"Benched",
"Scene unit",
"Portrait+1",
"Shake",
"Can't deploy",
"Departed",
"4th palette",
"Unk 35",
"Unk 36",
"Capture",
"Unk 38",
}; 


void StateInit(DebuggerProc* proc) { 
    SomeMenuInit(proc); 
    struct Unit* unit = proc->unit; 
    proc->tmp[0] = unit->state; 
    proc->tmp[1] = unit->state >> 16; 
    
    
    int x = 1; 
    int y = 1; 
    int w = 29; //StatWidth + (START_X - NUMBER_X) + 3; 
    int h = 18; //(NumberOfOptions * 2) + 2; 
    
    DrawUiFrame(
        BG_GetMapBuffer(1), // back BG
        x, y, w, h,
        TILEREF(0, 0), 0); // style as 0 ? 

    //ClearUiFrame(
    //    BG_GetMapBuffer(1), // front BG 
    //    x, y, w, h);
    
    struct Text* th = gStatScreen.text;
    
    for (int i = 0; i < NumberOfState; ++i) { 
        InitText(&th[i], StateWidth);
        Text_DrawString(&th[i], states[i]);
    } 
    StartGreenText(proc);  
    RedrawStateMenu(proc);
}



void RedrawStateMenu(DebuggerProc* proc) { 
	TileMap_FillRect(gBG0TilemapBuffer + TILEMAP_INDEX(NUMBER_X-2, Y_HAND), 9, 2 * NumberOfOptions, 0);
	//BG_EnableSyncByMask(BG0_SYNC_BIT);
    //ResetText();
    int c = 0; 
    struct Text* th = gStatScreen.text;
    
    u32 state = proc->tmp[0] | (proc->tmp[1] << 16); 
    
    for (int i = 0; i < NumberOfState; ++i) { 
        c = state & (1 << i); 
        if (c) { c = TEXT_COLOR_SYSTEM_GOLD; } 
        
        if (Text_GetColor(&th[i]) != c) {
            ClearText(&th[i]);
            Text_SetColor(&th[i], c); 
            Text_DrawString(&th[i], states[i]);
        }
    } 
    c = 0;
    int x = 2; 
    int y = 2; 
    for (int i = 0; i < 8; ++i) { 
        PutText(&th[c], gBG0TilemapBuffer + TILEMAP_INDEX(x, y + (i*2))); c++; 
    } 
    x += StateWidth; 
    for (int i = 0; i < 8; ++i) { 
        PutText(&th[c], gBG0TilemapBuffer + TILEMAP_INDEX(x, y + (i*2))); c++; 
    } 
    x += StateWidth; 
    for (int i = 0; i < 8; ++i) { 
        PutText(&th[c], gBG0TilemapBuffer + TILEMAP_INDEX(x, y + (i*2))); c++; 
    } 
    x += StateWidth; 
    for (int i = 0; i < 8; ++i) { 
        PutText(&th[c], gBG0TilemapBuffer + TILEMAP_INDEX(x, y + (i*2))); c++; 
    } 


	BG_EnableSyncByMask(BG0_SYNC_BIT);

}
void SaveState(DebuggerProc* proc) { 
    u32 state = proc->tmp[0] | (proc->tmp[1] << 16); 
    proc->unit->state = state;
} 

void StateIdle(DebuggerProc* proc) { 
	u16 keys = sKeyStatusBuffer.repeatedKeys; 
    if ((keys & START_BUTTON)||(keys & B_BUTTON)) { //press B or Start to update state and continue 
         
        Proc_Goto(proc, RestartLabel);
        m4aSongNumStart(0x6B); 
    }
    u32 id = proc->id;
    if ((keys & A_BUTTON)) { //press B or Start to update state and continue 
        u32 state = proc->tmp[0] | (proc->tmp[1] << 16); 
        state ^= (1 << id);
        //state = (state & (1 << id)) | ~(state & (1 << id));
        
        proc->tmp[0] = state & 0xffff; 
        proc->tmp[1] = state >> 16; 
        SaveState(proc);
        RedrawStateMenu(proc);
    }
    
    DisplayUiHand(StateCursorLocationTable[id].x, StateCursorLocationTable[id].y); 	
    
    if (keys & DPAD_RIGHT) {
      id += 8;
    }
    if (keys & DPAD_LEFT) {
      id -= 8;
    }
    if (keys & DPAD_UP) {
        if (!(id % 8)) { id += 8; }
        id--;
    }
    if (keys & DPAD_DOWN) {
        
        id++;
        if (!(id % 8)) { id -= 8; }
        
    }
    
    if (id != (u32)proc->id) { 
        id %= 32; 
        proc->id = id; 
        RedrawStateMenu(proc);
    } 
    
    

}


#define ItemNameWidth 8
void EditItemsInit(DebuggerProc* proc) { 
    SomeMenuInit(proc); 
    LoadIconPalettes(4);
    struct Unit* unit = proc->unit; 
    for (int i = 0; i < NumberOfItems; ++i) { 
        proc->tmp[i] = unit->items[i]; 
    }
    
    int x = NUMBER_X - ItemNameWidth - 3; 
    int y = Y_HAND - 1; 
    int w = ItemNameWidth + (START_X - NUMBER_X) + 8; 
    int h = (NumberOfItems * 2) + 2; 
    
    DrawUiFrame(
        BG_GetMapBuffer(1), // back BG
        x, y, w, h,
        TILEREF(0, 0), 0); // style as 0 ? 

    struct Text* th = gStatScreen.text;
    
    for (int i = 0; i < NumberOfItems; ++i) { 
        InitText(&th[i], ItemNameWidth);
    } 


    RedrawItemMenu(proc);
}

void RedrawItemMenu(DebuggerProc* proc) { 
	//TileMap_FillRect(gBG0TilemapBuffer + TILEMAP_INDEX(NUMBER_X-2, Y_HAND), 9, 2 * NumberOfItems, 0);
    BG_Fill(gBG0TilemapBuffer, 0); 
	BG_EnableSyncByMask(BG0_SYNC_BIT);
    ResetIconGraphics();
    //ResetText();
    const struct ItemData* itemData[5]; 
    struct Text* th = gStatScreen.text;
    for (int i = 0; i < NumberOfItems; ++i) { 
        itemData[i] = GetItemData(proc->tmp[i] & 0xFF); 
    } 
    for (int i = 0; i < NumberOfItems; ++i) { 
        ClearText(&th[i]); 
        if (proc->tmp[i]) { 
            Text_DrawString(&th[i], GetStringFromIndex(itemData[i]->nameTextId));
        } 
    } 
    
    int x = NUMBER_X - (ItemNameWidth); 
    for (int i = 0; i < NumberOfItems; ++i) { 
        if (proc->tmp[i]) { 
            PutText(&th[i], gBG0TilemapBuffer + TILEMAP_INDEX(x, Y_HAND + (i*2))); 
        }
    } 
    int n = 0; 
    for (int i = 0; i < NumberOfItems; ++i) { // item id 
        if (proc->tmp[i]) { n = itemData[i]->number; } else { n = 0; } 
        PutNumberHex(gBG0TilemapBuffer + TILEMAP_INDEX(START_X, Y_HAND + (i*2)), TEXT_COLOR_SYSTEM_GOLD, n); 
    } 
    
    for (int i = 0; i < NumberOfItems; ++i) { // uses 
        if (proc->tmp[i]) { n = (proc->tmp[i] & 0xFF00) >> 8; } else { n = 0; } 
        PutNumber(gBG0TilemapBuffer + TILEMAP_INDEX(START_X + 3, Y_HAND + (i*2)), TEXT_COLOR_SYSTEM_GOLD, n); 
    } 
    
    int icon; 
    for (int i = 0; i < NumberOfItems; ++i) { 
        icon = GetItemIconId(proc->tmp[i]);
        if (icon >= 0) { 
            if (proc->tmp[i]) { 
            DrawIcon(TILEMAP_LOCATED(gBG0TilemapBuffer, x-2, Y_HAND + (i*2)), icon, 0x4000);
            }
        }
    }

	BG_EnableSyncByMask(BG0_SYNC_BIT);

}

static int GetMaxItems(void); 
void EditItemsIdle(DebuggerProc* proc) { 
	//DisplayVertUiHand(CursorLocationTable[proc->digit].x, CursorLocationTable[proc->digit].y); // 6 is the tile of the downwards hand 	
	u16 keys = sKeyStatusBuffer.repeatedKeys; 
    if (keys & B_BUTTON) { //press B to not save stats 
        Proc_Goto(proc, RestartLabel);
        m4aSongNumStart(0x6B); 
    };
    if ((keys & START_BUTTON)||(keys & A_BUTTON)) { //press A or Start to update stats and continue 
        SaveItems(proc); 
        Proc_Goto(proc, RestartLabel);
        m4aSongNumStart(0x6B); 
    };
    if (proc->editing) { 
        if (proc->editing == 1) { 
            DisplayVertUiHand(CursorLocationTable[proc->digit].x, (Y_HAND + (proc->id * 2)) * 8); 	
            int max = GetMaxItems(); 
            int min = 0; 
            int max_digits = GetMaxDigits(max, 1); 
            int val = 0; 
            
            if (keys & DPAD_RIGHT) {
              if (proc->digit > 0) { proc->digit--; }
              else { proc->digit = max_digits - 1; proc->editing = 2; proc->digit = 1; } 
              RedrawItemMenu(proc);
            }
            if (keys & DPAD_LEFT) {
              if (proc->digit < (max_digits-1)) { proc->digit++; }
              else { proc->digit = 0; proc->editing = false; } 
              RedrawItemMenu(proc);
            }
            
            if (keys & DPAD_UP) {
                if ((proc->tmp[proc->id] & 0xFF) == max) { proc->tmp[proc->id] = min | (proc->tmp[proc->id] & 0xFF00); } 
                else { 
                    proc->tmp[proc->id] += pDigitTable[1][proc->digit]; 
                    if ((proc->tmp[proc->id] & 0xFF) > max) { proc->tmp[proc->id] = max | (proc->tmp[proc->id] & 0xFF00); } 
                } 
                proc->tmp[proc->id] = MakeNewItem(proc->tmp[proc->id] & 0xFF); 
                RedrawItemMenu(proc); 
            }
            if (keys & DPAD_DOWN) {
                if ((proc->tmp[proc->id] & 0xFF) == min) { proc->tmp[proc->id] = max | (proc->tmp[proc->id] & 0xFF00); } 
                else { 
                    val = (proc->tmp[proc->id] & 0xFF) - pDigitTable[1][proc->digit]; 
                    if (val < min) { proc->tmp[proc->id] = min | (proc->tmp[proc->id] & 0xFF00); } 
                    else { proc->tmp[proc->id] = val | (proc->tmp[proc->id] & 0xFF00); } 
                } 
                proc->tmp[proc->id] = MakeNewItem(proc->tmp[proc->id] & 0xFF); 
                RedrawItemMenu(proc); 
            }
        }
        else { 
            DisplayVertUiHand(CursorLocationTable[proc->digit].x + (3 * 8), (Y_HAND + (proc->id * 2)) * 8); 	
            int max = 255 << 10; // skill scrolls
            int min = 0 << 8; 
            int max_digits = GetMaxDigits(max >> 8, 0); 
            
            if (keys & DPAD_RIGHT) {
              if (proc->digit > 0) { proc->digit--; }
              else { proc->digit = max_digits - 1; proc->editing = false; } 
              RedrawItemMenu(proc);
            }
            if (keys & DPAD_LEFT) {
              if (proc->digit < (max_digits-1)) { proc->digit++; }
              else { proc->digit = 0; proc->editing = 1; proc->digit = 0; } 
              RedrawItemMenu(proc);
            }
            
            if (keys & DPAD_UP) {
                if ((proc->tmp[proc->id] & 0xFF00) == max) { proc->tmp[proc->id] = min | (proc->tmp[proc->id] & 0xFF); } 
                else { 
                    proc->tmp[proc->id] += DigitDecimalTable[proc->digit] << 8; 
                    if ((proc->tmp[proc->id] & 0xFF00) > max) { proc->tmp[proc->id] = max | (proc->tmp[proc->id] & 0xFF); } 
                } 
                RedrawItemMenu(proc); 
            }
            if (keys & DPAD_DOWN) {
                
                if ((proc->tmp[proc->id] & 0xFF00) == min) { proc->tmp[proc->id] = max | (proc->tmp[proc->id] & 0xFF); } 
                else { 
                    proc->tmp[proc->id] -= DigitDecimalTable[proc->digit] << 8; 
                    if ((proc->tmp[proc->id] & 0xFF00) < min) { proc->tmp[proc->id] = min | (proc->tmp[proc->id] & 0xFF); } 
                } 
                
                RedrawItemMenu(proc); 
            }
        }
    }
    else { 
        DisplayUiHand(CursorLocationTable[0].x - ((ItemNameWidth + 4) * 8), (Y_HAND + (proc->id * 2)) * 8);
        if (keys & DPAD_RIGHT) {
            proc->digit = 1; 
          proc->editing = true; 
        }
        if (keys & DPAD_LEFT) {
          proc->digit = 0; 
          proc->editing = 2; 
        }
        
        if (keys & DPAD_UP) {
            proc->id--; 
            if (proc->id < 0) { proc->id = NumberOfItems - 1; } 
            RedrawItemMenu(proc); 
        }
        if (keys & DPAD_DOWN) {
            proc->id++; 
            if (proc->id >= NumberOfItems) { proc->id = 0; } 
            
            RedrawItemMenu(proc); 
        }
    } 
} 


#define NumberOfChState 8 
#define ChStateNameWidth 11 

void GotoChapter(int id) { 
    SetNextChapterId(id); 
    gPlaySt.save_menu_type = 2;
    SetNextGameActionId(GAME_ACTION_USR_SKIPPED);

    DeleteAll6CWaitMusicRelated();
    Sound_FadeOutBGM(4);
    SetTextFont(NULL);
    InitSystemTextFont();
    LoadUiFrameGraphics();
    ReadGameSaveCoreGfx();
    UnpackChapterMapPalette();
    //ChangeUnitSpritePalette(proc->mapSpritePalIdOverride);
    EndAllMus(); 
    EndBMapMain();
    //memset((u8*)(gEventCallQueue), 0, 0x80);
} 

void LomaChapter(int id) { 
    gPlaySt.chapterIndex = id; 
    RestartBattleMap();
    int x = 0; 
    int y = 0; 
    gBmSt.camera.x = GetCameraCenteredX(x * 16);
    gBmSt.camera.y = GetCameraCenteredY(y * 16);

    RefreshEntityBmMaps();
    RenderBmMap();
    RefreshUnitSprites();
    RefreshBMapGraphics();

    //ChangeUnitSpritePalette(proc->mapSpritePalIdOverride);

    BG_Fill(gBG0TilemapBuffer, 0);
    BG_Fill(gBG1TilemapBuffer, 0);

    BG_EnableSyncByMask(BG0_SYNC_BIT);
    BG_EnableSyncByMask(BG1_SYNC_BIT);
    FixCursorOverflow(); 
}
 
extern void WfxInit(void); 
void SaveChState(DebuggerProc* proc) { 
    
    gPlaySt.partyGoldAmount = proc->gold; 
    gPlaySt.chapterTurnNumber = proc->tmp[5]; 
    if (gPlaySt.chapterWeatherId != proc->tmp[1]) { 
        gPlaySt.chapterWeatherId = 0; 
        WfxInit(); //WfxNone_Init(); 
        SetWeather(proc->tmp[1]); 
        InitBmBgLayers();
        UnpackChapterMapGraphics(gPlaySt.chapterIndex);
        RenderBmMap();
        RefreshUnitSprites();
        
    } 
    
    if (gPlaySt.chapterVisionRange != proc->tmp[2]) { // fix? 
        gPlaySt.chapterVisionRange = proc->tmp[2];
        RefreshEntityBmMaps();
        RefreshUnitSprites();
        RenderBmMap();
    }
    if (proc->id == 3) { // mnc 2
        GotoChapter(proc->tmp[3]); 
        Proc_End(proc); 
        return; 

    } 
    if (proc->id == 4) { 
        LomaChapter(proc->tmp[4]); // loma 
    } 
    
    proc->lastFlag = proc->tmp[6]; 
    if (proc->id == 7) { // save & restart ch 
        GotoChapter(gPlaySt.chapterIndex); 
        Proc_End(proc); 
        return; 
    } 
    
    DebuggerProc* procIdler = Proc_Find(DebuggerProcCmdIdler); 
    procIdler->lastFlag = proc->lastFlag; 
    
}  

static const char chStates[][24] = { 
"Gold",
"Weather",
"Fog",
"Jump to ch.",
"Loma to ch.",
"Turn",
"Flags",
"Save & restart ch."
// clear ch 
// Preparations 
}; 

static const char weatherStates[][16] = { 
"Clear",
"Snowy",
"Blizzard",
"Night",
"Rainy",
"Volcano",
"Sandstorm",
"Cloudy",
}; 


void RedrawChStateMenu(DebuggerProc* proc); 
void ChStateInit(DebuggerProc* proc) { 
    SomeMenuInit(proc); 
    LoadIconPalettes(4);
    //struct Unit* unit = proc->unit; 
    for (int i = 0; i < NumberOfChState; ++i) { 
        proc->tmp[i] = 0; 
    }
    proc->gold = gPlaySt.partyGoldAmount; // gold can be bigger than u16
    proc->tmp[1] = gPlaySt.chapterWeatherId; 
    proc->tmp[2] = gPlaySt.chapterVisionRange; 
    proc->tmp[3] = gPlaySt.chapterIndex; 
    proc->tmp[4] = gPlaySt.chapterIndex; 
    proc->tmp[5] = gPlaySt.chapterTurnNumber; 
    proc->tmp[6] = proc->lastFlag; 
    proc->tmp[7] = proc->lastFlag; // unused 
    
    
    int x = 2; 
    int y = Y_HAND - 1; 
    int w = ChStateNameWidth + (START_X - NUMBER_X) + 10; 
    int h = (NumberOfChState * 2) + 2; 
    
    DrawUiFrame(
        BG_GetMapBuffer(1), // back BG
        x, y, w, h,
        TILEREF(0, 0), 0); // style as 0 ? 

    struct Text* th = gStatScreen.text;
    int i = 0; 
    for (i = 0; i < (NumberOfChState); ++i) { 
        InitText(&th[i], ChStateNameWidth);
    } 
    InitText(&th[i], ChStateNameWidth); i++; 
    InitText(&th[i], ChStateNameWidth+4); i++; 
    InitText(&th[i], ChStateNameWidth+4); i++; 
    
    RedrawChStateMenu(proc);
    //StartGreenText(proc); 
}

//"Gold",
//"Weather",
//"Fog",
//"Jump to ch.", // hex 
//"Loma to ch.", //hex 
//"Turn",
//"Flags", //hex 
//"Save & restart" //n/a 
static const s8 chStateHexOrDecimal[] = { 0, 0, 0, 1, 1, 0, 1, -1 }; 
static const int chStateMax[] = { 999999, 7, 4, 0x4E, 0x4E, 999, 0x12C, 0 }; 
static const int chStateMin[] = { 0,      0, 0, 0,    0,    0,     0,     0}; 

int GetChStateMax(int id) { 
    return chStateMax[id]; 
} 
int GetChStateMin(int id) { 
    return chStateMin[id]; 
} 

void RedrawChStateMenu(DebuggerProc* proc) { 
	//TileMap_FillRect(gBG0TilemapBuffer + TILEMAP_INDEX(NUMBER_X-2, Y_HAND), 9, 2 * NumberOfMisc, 0);
    BG_Fill(gBG0TilemapBuffer, 0); 
	BG_EnableSyncByMask(BG0_SYNC_BIT);
    ResetIconGraphics();
    //ResetText();
    //struct Unit* unit = proc->unit; 
    struct Text* th = gStatScreen.text;
    int i = 0; 
    for (i = 0; i < (NumberOfChState); ++i) { 
        ClearText(&th[i]); 
        Text_DrawString(&th[i], chStates[i]);  
    } 
    ClearText(&th[i]); Text_DrawString(&th[i], weatherStates[proc->tmp[1]]); i++; 
    ClearText(&th[i]); Text_DrawString(&th[i], GetStringFromIndex(GetROMChapterStruct(proc->tmp[3])->chapTitleTextId)); i++; 
    ClearText(&th[i]); Text_DrawString(&th[i], GetStringFromIndex(GetROMChapterStruct(proc->tmp[4])->chapTitleTextId)); i++; 

    int x = 3; 
    for (i = 0; i < NumberOfChState; ++i) { 
        PutText(&th[i], gBG0TilemapBuffer + TILEMAP_INDEX(x, Y_HAND + (i*2))); 
    } 
    
    x += ChStateNameWidth-4; 
    PutText(&th[i], gBG0TilemapBuffer + TILEMAP_INDEX(x, Y_HAND + (2))); i++;  // weather type 
    PutText(&th[i], gBG0TilemapBuffer + TILEMAP_INDEX(x, Y_HAND + (6))); i++;  // ch 
    PutText(&th[i], gBG0TilemapBuffer + TILEMAP_INDEX(x, Y_HAND + (8))); i++;  // ch
    
    PutNumber(gBG0TilemapBuffer + TILEMAP_INDEX(START_X + 4, Y_HAND), TEXT_COLOR_SYSTEM_GOLD, proc->gold); 
    
    int hexOrDec = 0; 
    int col = TEXT_COLOR_SYSTEM_WHITE; 
    for (i = 1; i < NumberOfChState; ++i) { 
        col = TEXT_COLOR_SYSTEM_WHITE;
        if (i == 6) { // flags 
            col = CheckFlag(proc->tmp[i]); 
            if (col) { col = TEXT_COLOR_SYSTEM_GOLD; 
            }
            else { col = TEXT_COLOR_SYSTEM_WHITE; 
            } 
        } 
        hexOrDec = chStateHexOrDecimal[i]; 
        if (hexOrDec < 0) { continue; } 
        if (hexOrDec) { 
            PutNumberHex(gBG0TilemapBuffer + TILEMAP_INDEX(START_X + 4, Y_HAND + (i*2)), col, proc->tmp[i]); 
        } 
        else { 
        PutNumber(gBG0TilemapBuffer + TILEMAP_INDEX(START_X + 4, Y_HAND + (i*2)), col, proc->tmp[i]); 
        }
        
    } 

	BG_EnableSyncByMask(BG0_SYNC_BIT);

}


void ChStateIdle(DebuggerProc* proc) { 
	//DisplayVertUiHand(CursorLocationTable[proc->digit].x, CursorLocationTable[proc->digit].y); // 6 is the tile of the downwards hand 	
	u16 keys = sKeyStatusBuffer.repeatedKeys; 
    if (keys & B_BUTTON) { //press B to not save ch state 
        Proc_Goto(proc, RestartLabel);
        m4aSongNumStart(0x6B); 
    };
    if ((keys & START_BUTTON)||(keys & A_BUTTON)) { //press A or Start to update ch state and continue 
        SaveChState(proc);
        if (proc->id != 6) { 
        Proc_Goto(proc, RestartLabel);
        m4aSongNumStart(0x6B);
        }
        else { // flags 
            int flag = proc->tmp[6]; 
            if (CheckFlag(flag)) { ClearFlag(flag); } 
            else { SetFlag(flag); } 
            RedrawChStateMenu(proc);
        } 
    };
    int id = proc->id; 
    int type = chStateHexOrDecimal[id]; 
    int val = proc->tmp[id];
    if (!id) { val = proc->gold; } 
    
    if (proc->editing && (type >= 0)) { 
        DisplayVertUiHand(CursorLocationTable[proc->digit].x + 32, (Y_HAND + (id * 2)) * 8); 	
        int max = GetChStateMax(id); 
        int min = GetChStateMin(id); 
        int max_digits = GetMaxDigits(max, type); 
        
        
        if (keys & DPAD_RIGHT) {
          if (proc->digit > 0) { proc->digit--; }
          else { proc->digit = max_digits - 1; proc->editing = false; } 
          RedrawChStateMenu(proc);
        }
        if (keys & DPAD_LEFT) {
          if (proc->digit < (max_digits-1)) { proc->digit++; }
          else { proc->digit = 0; proc->editing = false; } 
          RedrawChStateMenu(proc);
        }
        
        if (keys & DPAD_UP) {
            if (!id) { // gold 
                if ((proc->gold) == max) { proc->gold = min; } 
                else { 
                    proc->gold += pDigitTable[type][proc->digit]; 
                    if ((proc->gold) > max) { proc->gold = max; } 
                } 
            } 
            else { 
                if ((proc->tmp[id]) == max) { proc->tmp[id] = min; } 
                else { 
                    proc->tmp[id] += pDigitTable[type][proc->digit]; 
                    if ((proc->tmp[id]) > max) { proc->tmp[id] = max; } 
                } 
            } 
            RedrawChStateMenu(proc); 
        }
        if (keys & DPAD_DOWN) {
            if (!id) { // gold 
                if ((proc->gold) == min) { proc->gold = max; } 
                else { 
                    val = (proc->gold) - pDigitTable[type][proc->digit]; 
                    if (val < min) { proc->gold = min; } 
                    else { proc->gold = val; } 
                } 
            } 
            else { // not gold 
                if ((proc->tmp[id]) == min) { proc->tmp[id] = max; } 
                else { 
                    val = (proc->tmp[id]) - pDigitTable[type][proc->digit]; 
                    if (val < min) { proc->tmp[id] = min; } 
                    else { proc->tmp[id] = val; } 
                } 
            } 
            RedrawChStateMenu(proc); 
        }
    }
    else { 
        DisplayUiHand(CursorLocationTable[0].x - ((ChStateNameWidth + 5) * 8), (Y_HAND + (id * 2)) * 8);
        if (keys & DPAD_RIGHT) {
            proc->digit = GetMostSignificantDigit(val, type); 
            proc->editing = true; 
        }
        if (keys & DPAD_LEFT) {
            proc->digit = 0; 
            proc->editing = true; 
        }
        
        if (keys & DPAD_UP) {
            proc->id--; 
            if (proc->id < 0) { proc->id = NumberOfChState - 1; } 
            RedrawChStateMenu(proc); 
        }
        if (keys & DPAD_DOWN) {
            proc->id++; 
            if (proc->id >= NumberOfChState) { proc->id = 0; } 
            
            RedrawChStateMenu(proc); 
        }
    } 
} 


#define NumberOfMisc 7 
#define MiscNameWidth 8 
 
void SaveMisc(DebuggerProc* proc) { 

    struct Unit* unit = proc->unit; 
    unit->pCharacterData = GetCharacterData(proc->tmp[0]); 
    unit->pClassData = GetClassData(proc->tmp[1]); 
    unit->level = proc->tmp[2]; 
    unit->exp = proc->tmp[3] & 0xFF; 
    unit->conBonus = proc->tmp[4]; 
    unit->movBonus = proc->tmp[5]; 
    if (UNIT_MOV(unit) > 15) { unit->movBonus = 15 - UNIT_MOV_BASE(unit); } 
    SetUnitStatusIndex(unit, proc->tmp[6] & 0x3F);
    if (unit->statusIndex) { SetUnitStatusDuration(unit, 3); }

}  

void RedrawMiscMenu(DebuggerProc* proc); 
void EditMiscInit(DebuggerProc* proc) { 
    SomeMenuInit(proc); 
    LoadIconPalettes(4);
    struct Unit* unit = proc->unit; 
    for (int i = 0; i < NumberOfMisc; ++i) { 
        proc->tmp[i] = 0; 
    }
    proc->tmp[0] = unit->pCharacterData->number; 
    proc->tmp[1] = unit->pClassData->number; 
    proc->tmp[2] = unit->level; 
    proc->tmp[3] = unit->exp; 
    proc->tmp[4] = unit->conBonus; 
    proc->tmp[5] = unit->movBonus; 
    proc->tmp[6] = (GetUnitStatusIndex(unit) & 0x3F);
    
    
    int x = NUMBER_X - MiscNameWidth - 1; 
    int y = Y_HAND - 1; 
    int w = MiscNameWidth + (START_X - NUMBER_X) + 3; 
    int h = (NumberOfMisc * 2) + 2; 
    
    DrawUiFrame(
        BG_GetMapBuffer(1), // back BG
        x, y, w, h,
        TILEREF(0, 0), 0); // style as 0 ? 

    struct Text* th = gStatScreen.text;
    
    for (int i = 0; i < NumberOfMisc; ++i) { 
        InitText(&th[i], MiscNameWidth);
    } 


    RedrawMiscMenu(proc);
}




extern int sStatusNameTextIdLookup[];
void RedrawMiscMenu(DebuggerProc* proc) { 
	//TileMap_FillRect(gBG0TilemapBuffer + TILEMAP_INDEX(NUMBER_X-2, Y_HAND), 9, 2 * NumberOfMisc, 0);
    BG_Fill(gBG0TilemapBuffer, 0); 
	BG_EnableSyncByMask(BG0_SYNC_BIT);
    ResetIconGraphics();
    //ResetText();
    //struct Unit* unit = proc->unit; 
    struct Text* th = gStatScreen.text;
    int i = 0; 
    for (i = 0; i < NumberOfMisc; ++i) { 
        ClearText(&th[i]); 
    } 
    
    
    i = 0; 
    
    Text_DrawString(&th[i], GetStringFromIndex(GetCharacterData(proc->tmp[0])->nameTextId)); i++; 
    Text_DrawString(&th[i], GetStringFromIndex(GetClassData(proc->tmp[1])->nameTextId)); i++; 
    Text_DrawString(&th[i], "Level"); i++; 
    Text_DrawString(&th[i], "Exp"); i++; 
    Text_DrawString(&th[i], "Bonus Con"); i++; 
    Text_DrawString(&th[i], "Bonus Mov"); i++; 
    if (!proc->tmp[6]) { 
        Text_DrawString(&th[i], "Status"); i++; 
    } 
    else { 
        
        Text_DrawString(&th[i], GetStringFromIndex(gpDebuffInfos[proc->tmp[6] & 0x3F].name)); i++;
    } 
    
    int x = NUMBER_X - (MiscNameWidth); 
    for (i = 0; i < NumberOfMisc; ++i) { 
        PutText(&th[i], gBG0TilemapBuffer + TILEMAP_INDEX(x, Y_HAND + (i*2))); 
    } 
    for (i = 0; i < NumberOfMisc; ++i) { 
        //
        if (i < 2) { 
        PutNumberHex(gBG0TilemapBuffer + TILEMAP_INDEX(START_X, Y_HAND + (i*2)), TEXT_COLOR_SYSTEM_GOLD, proc->tmp[i]); 
        } 
        else { 
        PutNumber(gBG0TilemapBuffer + TILEMAP_INDEX(START_X, Y_HAND + (i*2)), TEXT_COLOR_SYSTEM_GOLD, proc->tmp[i]); 
        }
        
    } 
    
    //for (i = 0; i < NumberOfMisc; ++i) { // uses 
    //    if (proc->tmp[i]) { n = (proc->tmp[i] & 0xFF00) >> 8; } else { n = 0; } 
    //    PutNumber(gBG0TilemapBuffer + TILEMAP_INDEX(START_X + 3, Y_HAND + (i*2)), TEXT_COLOR_SYSTEM_GOLD, n); 
    //} 


	BG_EnableSyncByMask(BG0_SYNC_BIT);

}

int GetMiscMin(int id) { 
    int result = 0; 
    switch (id) { 
        case 0: { result = 1; break; } // unitID
        case 1: { result = 1; break; } // classID 
        case 2: { result = 1; break; } // level
        case 3: { result = 0; break; } // exp  -1 ? 
        case 4: { result = 0; break; } // + con 
        case 5: { result = 0; break; } // + mov  
        case 6: { result = 0; break; } // status  
        default: 
    } 
    return result; 
} 

static int GetMaxItems(void) {  
	const struct ItemData* table = GetItemData(1); 
    int c = 255; 
	for (int i = 1; i < 255; i++) { 
		if (table->number != i) { table--; break; } 
		table++; 
	} 
    c = table->number;
	if (c > 255) { c = 255; } 
	if (c < 1) { c = 1; } 
	return table->number; 
} 

static int GetMaxClasses(void) { 
	const struct ClassData* table = GetClassData(1); 
	int c = 255; 
	for (int i = 1; i <= c; i++) { 
		if (table->number != i) { table--; break; } 
		table++; 
	} 
	c = table->number;
	if (c > 255) { c = 255; } 
	if (c < 1) { c = 1; } 
	return c; 
} 

int GetMiscMax(int id) { 
    int result = 0; 
    switch (id) { 
        case 0: { result = 255; break; } // unitID
        case 1: { result = GetMaxClasses(); break; } // classID 
        case 2: { result = 255; break; } // level
        case 3: { result = 100; break; } // exp  
        case 4: { result = 15; break; } // + con 
        case 5: { result = 15; break; } // + mov  
        case 6: { result = 63; break; } // status  
        default: 
    } 
    return result; 
} 


void EditMiscIdle(DebuggerProc* proc) { 
	//DisplayVertUiHand(CursorLocationTable[proc->digit].x, CursorLocationTable[proc->digit].y); // 6 is the tile of the downwards hand 	
	u16 keys = sKeyStatusBuffer.repeatedKeys; 
    if (keys & B_BUTTON) { //press B to not save stats 
        Proc_Goto(proc, RestartLabel);
        m4aSongNumStart(0x6B); 
    };
    if ((keys & START_BUTTON)||(keys & A_BUTTON)) { //press A or Start to update stats and continue 
        SaveMisc(proc); 
        Proc_Goto(proc, RestartLabel);
        m4aSongNumStart(0x6B); 
    };
    if (proc->editing) { 
        DisplayVertUiHand(CursorLocationTable[proc->digit].x, (Y_HAND + (proc->id * 2)) * 8); 	
        int max = GetMiscMax(proc->id); 
        int min = GetMiscMin(proc->id); 
        int type = (proc->id < 2); 
        int max_digits = GetMaxDigits(max, type); 
        int val = 0; 
        
        if (keys & DPAD_RIGHT) {
          if (proc->digit > 0) { proc->digit--; }
          else { proc->digit = max_digits - 1; proc->editing = false; } 
          RedrawMiscMenu(proc);
        }
        if (keys & DPAD_LEFT) {
          if (proc->digit < (max_digits-1)) { proc->digit++; }
          else { proc->digit = 0; proc->editing = false; } 
          RedrawMiscMenu(proc);
        }
        
        if (keys & DPAD_UP) {
            if ((proc->tmp[proc->id]) == max) { proc->tmp[proc->id] = min; } 
            else { 
                proc->tmp[proc->id] += pDigitTable[type][proc->digit]; 
                if ((proc->tmp[proc->id]) > max) { proc->tmp[proc->id] = max; } 
            } 
            //proc->tmp[proc->id] = GetPrevMisc(proc->tmp[proc->id], proc->id, min, max); 
            RedrawMiscMenu(proc); 
        }
        if (keys & DPAD_DOWN) {
            if ((proc->tmp[proc->id]) == min) { proc->tmp[proc->id] = max; } 
            else { 
                val = (proc->tmp[proc->id]) - pDigitTable[type][proc->digit]; 
                if (val < min) { proc->tmp[proc->id] = min; } 
                else { proc->tmp[proc->id] = val; } 
            } 
            //proc->tmp[proc->id] = GetNextMisc(proc->tmp[proc->id], proc->id, min, max); 
            RedrawMiscMenu(proc); 
        }
    }
    else { 
        DisplayUiHand(CursorLocationTable[0].x - ((MiscNameWidth + 2) * 8), (Y_HAND + (proc->id * 2)) * 8);
        if (keys & DPAD_RIGHT) {
            proc->digit = 1; 
          proc->editing = true; 
        }
        if (keys & DPAD_LEFT) {
          proc->digit = 0; 
          proc->editing = true; 
        }
        
        if (keys & DPAD_UP) {
            proc->id--; 
            if (proc->id < 0) { proc->id = NumberOfMisc - 1; } 
            RedrawMiscMenu(proc); 
        }
        if (keys & DPAD_DOWN) {
            proc->id++; 
            if (proc->id >= NumberOfMisc) { proc->id = 0; } 
            
            RedrawMiscMenu(proc); 
        }
    } 
} 


#define NumberOfLoad 6 
#define LoadNameWidth 12 


extern struct Unit* LoadUnit(const struct UnitDefinition* uDef); // 17788 17598
extern void ClearUnit(struct Unit* unit); // 17508 17394
static void InitUnitDef(struct UnitDefinition* uDef, struct Unit* unit, const struct CharacterData* data) { 

    uDef->charIndex = data->number;
    //uDef->classIndex = data->defaultClass;
    uDef->classIndex = 0;
    uDef->leaderCharIndex = unit->supports[UNIT_SUPPORT_MAX_COUNT-1];
    uDef->autolevel = true; 
	uDef->allegiance = UNIT_FACTION(unit)>>6; 
    uDef->level = data->baseLevel; 
    if (!uDef->level) { uDef->level = 1; } 
    uDef->xPosition = unit->xPos; 
    uDef->yPosition = unit->yPos; 
	uDef->genMonster = false; 
	uDef->itemDrop = (unit->state & US_DROP_ITEM) != 0; 
	uDef->sumFlag = 0; 
	uDef->unk_05_7 = 0; 
	uDef->extraData = 0; 
	uDef->redaCount = 0; 
	uDef->redas = NULL; 
	uDef->items[0] = unit->items[0]; 
	uDef->items[1] = unit->items[1]; 
	uDef->items[2] = unit->items[2]; 
	uDef->items[3] = unit->items[3]; 
	uDef->ai[0] = unit->ai1;
	uDef->ai[1] = unit->ai2;
	uDef->ai[2] = unit->ai_config & 0xFF;
	uDef->ai[3] = (unit->ai_config>>8);
} 

static void ReinitUnitDef(struct UnitDefinition* uDef, struct Unit* unit) { 

    uDef->charIndex = unit->pCharacterData->number;
    uDef->classIndex = unit->pCharacterData->defaultClass;
    uDef->leaderCharIndex = unit->supports[UNIT_SUPPORT_MAX_COUNT-1];
    uDef->autolevel = true; 
	uDef->allegiance = UNIT_FACTION(unit)>>6; 
    uDef->level = unit->pCharacterData->baseLevel; 
    uDef->xPosition = unit->xPos; 
    uDef->yPosition = unit->yPos; 
	uDef->genMonster = false; 
	uDef->itemDrop = (unit->state & US_DROP_ITEM) != 0; 
	uDef->sumFlag = 0; 
	uDef->unk_05_7 = 0; 
	uDef->extraData = 0; 
	uDef->redaCount = 0; 
	uDef->redas = NULL; 
	uDef->items[0] = unit->items[0]; 
	uDef->items[1] = unit->items[1]; 
	uDef->items[2] = unit->items[2]; 
	uDef->items[3] = unit->items[3]; 
	uDef->ai[0] = unit->ai1;
	uDef->ai[1] = unit->ai2;
	uDef->ai[2] = unit->ai_config & 0xFF;
	uDef->ai[3] = (unit->ai_config>>8);
} 

#define SinglePlayer 0 
#define SingleNPC 1 
#define SingleEnemy 2 
#define PlayerUnits 3 
#define BossUnits 4 
#define ExistingUnits 5

int FindNextBoss(int c) { 
    const struct CharacterData* data; 
    for (; c < 256; ++c) { 
        data = GetCharacterData(c); 
        if (data->attributes & CA_BOSS) { return c; } 
    } 
    return 0; 
} 

const u8 BasicWeaponsByType[] = { ITEM_SWORD_IRON,  ITEM_LANCE_IRON, ITEM_AXE_IRON, ITEM_BOW_IRON, ITEM_STAFF_HEAL, ITEM_ANIMA_FIRE, ITEM_LIGHT_LIGHTNING, ITEM_DARK_FLUX, ITEM_MONSTER_ROTTEN_CLAW, ITEM_LOCKPICK, ITEM_ELIXIR, ITEM_VULNERARY}; 
static void SilentTryAddItem(struct Unit* unit, int itemType) { 
    for (int i = 0; i < 5; ++i) { 
        if (!unit->items[i]) { 
            unit->items[i] = MakeNewItem(BasicWeaponsByType[itemType]); 
            break; 
        } 
    } 
} 

void GrantWeapons(struct Unit* unit) { 
    if (unit->items[0]) { return; } 
    
    for (int i = 0; i < 8; ++i) { 
        if (unit->ranks[i]) { 
            SilentTryAddItem(unit, i); 
        } 
    } 
    if (UNIT_CATTRIBUTES(unit) & CA_LOCK_3) { 
        SilentTryAddItem(unit, 8); 
    } 
    if (UNIT_CATTRIBUTES(unit) & CA_THIEF) { 
        SilentTryAddItem(unit, 9); 
    } 
    if (UNIT_FACTION(unit) == FACTION_RED) { return; } 
    if (UNIT_CATTRIBUTES(unit) & CA_PROMOTED) { 
        SilentTryAddItem(unit, 10); // elixir 
    } 
    else { 
    SilentTryAddItem(unit, 11); // vuln 
    } 
} 

inline s8 CanUnitCrossTerrain2(struct Unit* unit, int terrain) {
    const s8* lookup = GetUnitMovementCost(unit);
    return (lookup[terrain] > 0) ? TRUE : FALSE;
}

void FindNearestTile(struct Unit* unit) { 
    if (unit->state & (US_DEAD | US_NOT_DEPLOYED | US_BIT16)) { return; }  
    int xOut = -1; 
    int yOut = -1;
    int iy, ix, minDistance = 9999;

    unit->xPos = gActiveUnit->xPos; 
    unit->yPos = gActiveUnit->yPos; 
    // Fill the movement map
    GenerateExtendedMovementMap(unit->xPos, unit->yPos, TerrainTable_MovCost_FlyNormal);

    // Put the active unit on the unit map (kinda, just marking its spot)
    // // gBmMapUnit[gActiveUnit->yPos][gActiveUnit->xPos] = 0xFF;

    // Remove the actor unit from the unit map (why?)
    // // gBmMapUnit[unit->yPos][unit->xPos] = 0;

    for (iy = gBmMapSize.y - 1; iy >= 0; --iy) {
        for (ix = gBmMapSize.x - 1; ix >= 0; --ix) {
            int distance;

            if (gBmMapMovement[iy][ix] > MAP_MOVEMENT_MAX)
                continue;

            if (gBmMapUnit[iy][ix] != 0)
                continue;

            if (gBmMapHidden[iy][ix] & HIDDEN_BIT_UNIT)
                continue;

            if (!CanUnitCrossTerrain2(unit, gBmMapTerrain[iy][ix]))
                continue;

            distance = RECT_DISTANCE(ix, iy, unit->xPos, unit->yPos);

            if (minDistance >= distance) {
                minDistance = distance;

                xOut = ix;
                yOut = iy;
            }
        }
    }

    // Remove the active unit from the unit map again
    // gBmMapUnit[gActiveUnit->yPos][gActiveUnit->xPos] = 0;
    if (xOut >= 0) { unit->xPos = xOut; } 
    else { unit->xPos = 0; } 
    if (yOut >= 0) { unit->yPos = yOut; } 
    else { unit->yPos = 0; } 
    gBmMapUnit[unit->yPos][unit->xPos] = unit->index; 

} 

void LoadAllUnits(int type, int uid) { 
	struct UnitDefinition uDef; 
	struct Unit* unit; 

	int i = 1; 
	int end = 0xC0; 
    u32 attr = 0; 
	int c = 1; // char id 
    int c_end = 0xFD; // last BWL is 0x45  
	if (type == BossUnits) { i = 0x80; attr = CA_BOSS; c_end = 0xFD; } 
	if (type == PlayerUnits) { end = 0x80; c_end = 50; } 
	if (type == SinglePlayer) { c = uid; c_end = uid + 1; } 
	if (type == SingleNPC) { c = uid; c_end = uid + 1; i = 0x40; } 
	if (type == SingleEnemy) { c = uid; c_end = uid + 1; i = 0x80; } 
    
    
    int deployedPlayers = 0; 
    int deployedNPCs = 0; 
    int deployedEnemies = 0; 

    u32 charIDsToIgnore[8] = { 0, 0, 0, 0, 0, 0, 0, 0 }; 
    int i_copy = i; 
    
    int tmp = 0; 
    if (type != ExistingUnits) { 
        for (; i < end; ++i) { 
            unit = GetUnit(i);
            if (!UNIT_IS_VALID(unit)) { continue; } 
            tmp = (UNIT_FACTION(unit) >> 6) + 1; 
            deployedPlayers += tmp & 1; 
            deployedNPCs += (tmp & 2) >> 1; 
            deployedEnemies += tmp >> 2; 
            
            tmp = unit->pCharacterData->number; 
            
            
            charIDsToIgnore[tmp >> 5] |= 1 << (tmp & 0x1F); // make 8 bitfields of unitIDs to ignore 
            // 8 words * 32 bits = 256 characters  
        }   
    }
    i = i_copy; 
    
	for (; i < end; ++i) { 
        if (attr) { c = FindNextBoss(c); if (!c) { break; } } 
        if (c >= c_end) { break; } 
        if (charIDsToIgnore[c >> 5] & (1 << (c & 0x1F))) { c++; continue; } 
		unit = GetUnit(i); 
        if (!unit) { continue; } 
		if (!(unit->pCharacterData) && (type == ExistingUnits)) { continue; } 
		if ((unit->pCharacterData) && (type != ExistingUnits)) { continue; } 
        if (type == ExistingUnits) { c = unit->pCharacterData->number; } 
		u32 state = unit->state; 
        tmp = (UNIT_FACTION(unit) >> 6) + 1; 
        deployedPlayers += tmp & 1; 
        deployedNPCs += (tmp & 2) >> 1; 
        deployedEnemies += tmp >> 2; 
        switch (tmp) { 
            case 1: { if (deployedPlayers > 50) { state |= US_NOT_DEPLOYED; continue; } break; } 
            case 2: { if (deployedNPCs > 20) { state |= US_NOT_DEPLOYED; continue; } break; } 
            case 3: { if (deployedEnemies > 50) { state |= US_NOT_DEPLOYED; continue; } break; } 
            default: 
        } 
        
        if (type == ExistingUnits) { 
            ReinitUnitDef(&uDef, unit); 
            ClearUnit(unit);
        } 
        else { 
            ClearUnit(unit);
            InitUnitDef(&uDef, unit, GetCharacterData(c)); 
        } 
		LoadUnit(&uDef); 
        GrantWeapons(unit); 
		unit->state = state; 
        FindNearestTile(unit); 
        c++; 
	} 
} 

void SaveLoadUnits(DebuggerProc* proc) { 
    int id = proc->id; 
    LoadAllUnits(id, proc->tmp[id]);


}  

int GetLoadMax(int val) { 
    return 0xFF; 
} 
int GetLoadMin(int val) { 
    return 0x1; 
} 


void RedrawLoadMenu(DebuggerProc* proc); 
void LoadUnitsInit(DebuggerProc* proc) { 
    SomeMenuInit(proc); 
    LoadIconPalettes(4);
    //struct Unit* unit = proc->unit; 
    for (int i = 0; i < NumberOfLoad; ++i) { 
        proc->tmp[i] = 0; 
    }
    
    proc->tmp[0] = 1; // Eirika default 
    proc->tmp[1] = 0xCC; // Messenger
    proc->tmp[2] = 0x68; // O'Neil 
    // proc->tmp[0] = unit->pCharacterData->number; 
    // proc->tmp[1] = unit->pClassData->number; 
    // proc->tmp[2] = unit->level; 
    // proc->tmp[3] = unit->exp; 
    // proc->tmp[4] = unit->conBonus; 
    // proc->tmp[5] = unit->movBonus; 
    // proc->tmp[6] = unit->statusIndex; 
    
    
    int x = NUMBER_X - LoadNameWidth - 1; 
    int y = Y_HAND - 1; 
    int w = LoadNameWidth + (START_X - NUMBER_X) + 3; 
    int h = (NumberOfLoad * 2) + 2; 
    
    DrawUiFrame(
        BG_GetMapBuffer(1), // back BG
        x, y, w, h,
        TILEREF(0, 0), 0); // style as 0 ? 

    struct Text* th = gStatScreen.text;
    
    for (int i = 0; i <= NumberOfLoad+3; ++i) { 
        InitText(&th[i], LoadNameWidth);
    } 


    RedrawLoadMenu(proc);
}
  
extern int sStatusNameTextIdLookup[];
void RedrawLoadMenu(DebuggerProc* proc) { 
	//TileMap_FillRect(gBG0TilemapBuffer + TILEMAP_INDEX(NUMBER_X-2, Y_HAND), 9, 2 * NumberOfLoad, 0);
    BG_Fill(gBG0TilemapBuffer, 0); 
	BG_EnableSyncByMask(BG0_SYNC_BIT);
    ResetIconGraphics();
    //ResetText();
    //struct Unit* unit = proc->unit; 
    struct Text* th = gStatScreen.text;
    int i = 0; 
    for (i = 0; i <= NumberOfLoad+3; ++i) { 
        ClearText(&th[i]); 
    } 
    
    
    i = 0; 
    
    Text_DrawString(&th[i], "Load Player"); i++; 
    Text_DrawString(&th[i], "Load NPC"); i++; 
    Text_DrawString(&th[i], "Load Enemy"); i++; 
    Text_DrawString(&th[i], "Load all players"); i++; 
    Text_DrawString(&th[i], "Load all bosses"); i++; 
    Text_DrawString(&th[i], "Reload units"); i++; 
    //Text_DrawString(&th[i], "Preparations menu"); i++; 
    Text_DrawString(&th[i], GetStringFromIndex(GetCharacterData(proc->tmp[0])->nameTextId)); i++; 
    Text_DrawString(&th[i], GetStringFromIndex(GetCharacterData(proc->tmp[1])->nameTextId)); i++; 
    Text_DrawString(&th[i], GetStringFromIndex(GetCharacterData(proc->tmp[2])->nameTextId)); i++; 
    
    int x = NUMBER_X - (LoadNameWidth); 
    for (i = 0; i < NumberOfLoad; ++i) { 
        PutText(&th[i], gBG0TilemapBuffer + TILEMAP_INDEX(x, Y_HAND + (i*2))); 
    } 
    PutText(&th[i], gBG0TilemapBuffer + TILEMAP_INDEX(x + 7, Y_HAND)); i++; 
    PutText(&th[i], gBG0TilemapBuffer + TILEMAP_INDEX(x + 7, Y_HAND+2)); i++; 
    PutText(&th[i], gBG0TilemapBuffer + TILEMAP_INDEX(x + 7, Y_HAND+4)); i++; 
    for (i = 0; i < 3; ++i) { 
        //PutNumber(gBG0TilemapBuffer + TILEMAP_INDEX(START_X, Y_HAND + (i*2)), TEXT_COLOR_SYSTEM_GOLD, proc->tmp[i]); 
        PutNumberHex(gBG0TilemapBuffer + TILEMAP_INDEX(START_X, Y_HAND + (i*2)), TEXT_COLOR_SYSTEM_GOLD, proc->tmp[i]); 
    } 
    
    //for (i = 0; i < NumberOfLoad; ++i) { // uses 
    //    if (proc->tmp[i]) { n = (proc->tmp[i] & 0xFF00) >> 8; } else { n = 0; } 
    //    PutNumber(gBG0TilemapBuffer + TILEMAP_INDEX(START_X + 3, Y_HAND + (i*2)), TEXT_COLOR_SYSTEM_GOLD, n); 
    //} 


	BG_EnableSyncByMask(BG0_SYNC_BIT);

}

void LoadUnitsIdle(DebuggerProc* proc) { 
	//DisplayVertUiHand(CursorLocationTable[proc->digit].x, CursorLocationTable[proc->digit].y); // 6 is the tile of the downwards hand 	
	u16 keys = sKeyStatusBuffer.repeatedKeys; 
    if (keys & B_BUTTON) { //press B to not save stats 
        Proc_Goto(proc, RestartLabel);
        m4aSongNumStart(0x6B); 
    };
    if ((keys & START_BUTTON)||(keys & A_BUTTON)) { //press A or Start to update stats and continue 
        SaveLoadUnits(proc); 
        Proc_Goto(proc, RestartLabel);
        m4aSongNumStart(0x6B); 
    };
    if (proc->editing) { 
        DisplayVertUiHand(CursorLocationTable[proc->digit].x, (Y_HAND + (proc->id * 2)) * 8); 	
        int max = GetLoadMax(proc->id); 
        int min = GetLoadMin(proc->id); 
        int max_digits = GetMaxDigits(max, 1); 
        int val = 0; 
        
        if (keys & DPAD_RIGHT) {
          if (proc->digit > 0) { proc->digit--; }
          else { proc->digit = max_digits - 1; proc->editing = false; } 
          RedrawLoadMenu(proc);
        }
        if (keys & DPAD_LEFT) {
          if (proc->digit < (max_digits-1)) { proc->digit++; }
          else { proc->digit = 0; proc->editing = false; } 
          RedrawLoadMenu(proc);
        }
        
        if (keys & DPAD_UP) {
            if ((proc->tmp[proc->id]) == max) { proc->tmp[proc->id] = min; } 
            else { 
                proc->tmp[proc->id] += DigitHexTable[proc->digit]; 
                if ((proc->tmp[proc->id]) > max) { proc->tmp[proc->id] = max; } 
            } 
            //proc->tmp[proc->id] = GetPrevLoad(proc->tmp[proc->id], proc->id, min, max); 
            RedrawLoadMenu(proc); 
        }
        if (keys & DPAD_DOWN) {
            if ((proc->tmp[proc->id]) == min) { proc->tmp[proc->id] = max; } 
            else { 
                val = (proc->tmp[proc->id]) - DigitHexTable[proc->digit]; 
                if (val < min) { proc->tmp[proc->id] = min; } 
                else { proc->tmp[proc->id] = val; } 
            } 
            //proc->tmp[proc->id] = GetNextLoad(proc->tmp[proc->id], proc->id, min, max); 
            RedrawLoadMenu(proc); 
        }
    }
    else { 
        DisplayUiHand(CursorLocationTable[0].x - ((LoadNameWidth + 2) * 8), (Y_HAND + (proc->id * 2)) * 8);
        if (keys & DPAD_RIGHT) {
            proc->digit = 1; 
            proc->editing = true; 
            if (proc->id > 2) { proc->id = 2; } 
        }
        if (keys & DPAD_LEFT) {
            proc->digit = 0; 
            proc->editing = true; 
            if (proc->id > 2) { proc->id = 2; } 
        }
        
        if (keys & DPAD_UP) {
            proc->id--; 
            if (proc->id < 0) { proc->id = NumberOfLoad - 1; } 
            RedrawLoadMenu(proc); 
        }
        if (keys & DPAD_DOWN) {
            proc->id++; 
            if (proc->id >= NumberOfLoad) { proc->id = 0; } 
            
            RedrawLoadMenu(proc); 
        }
    } 
} 

void ChooseTileInit(DebuggerProc* proc) { // if need to load gfx 
    EndPlayerPhaseSideWindows(); 
    int lastTile = proc->lastTileHovered;
    for (int i = 0; i < xTilesAmount; ++i) { 
        proc->tmp[i] = (lastTile + i) & 0x3FF; 
    } 
    RenderTilesetRowOnBg2(proc); 
}

void OffsetTileset(DebuggerProc* proc, int amount) { 
    int newVal = 0; 
    if (amount < 0) { 
        for (int i = 0; i < xTilesAmount; ++i) { 
            newVal = proc->tmp[i] & 0x3FF; 
            proc->tmp[i] = (newVal - ABS(amount)) & 0x3FF; 
        } 
    
    } 
    else { 
        for (int i = 0; i < xTilesAmount; ++i) { 
            newVal = proc->tmp[i] & 0x3FF; 
            proc->tmp[i] = (newVal + amount) & 0x3FF; 
        } 
    } 
    proc->lastTileHovered = proc->tmp[0]; 
    RenderTilesetRowOnBg2(proc);
}

void ClearTilesetRow(DebuggerProc* proc) { 
    SetBackgroundTileDataOffset(2, 0);
    SetBlendTargetA(0, 1, 0, 0, 0);
    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG0_SYNC_BIT | BG1_SYNC_BIT | BG2_SYNC_BIT);
} 

// bg0 text, bg1 menu bgs, bg2 blank, bg3 map 
void ChooseTileIdle(DebuggerProc* proc) { 
    int x = 7; 
    int y = 9; 
    u16 keys = gKeyStatusPtr->newKeys | gKeyStatusPtr->repeatedKeys; 
    if (keys & A_BUTTON) {
        proc->tileID = proc->tmp[7]; 
        Proc_Goto(proc, EditMapLabel); 
    }
    if (keys & B_BUTTON) {
        gActionData.xMove = gActiveUnitMoveOrigin.x; 
        gActionData.yMove = gActiveUnitMoveOrigin.y; 
        PlayerPhase_ApplyUnitMovementWithoutMenu(proc); 
        ClearActiveUnitStuff(proc); 
        ClearTilesetRow(proc); 
        PlaySoundEffect(0x6B);
        Proc_Goto(proc, RestartLabel); 
    }
    if (keys & DPAD_LEFT) {
        OffsetTileset(proc, -1); 
    }
    if (keys & DPAD_RIGHT) {
        OffsetTileset(proc, 1); 
    }
    if (keys & DPAD_UP) {
        OffsetTileset(proc, -16); 
    }
    if (keys & DPAD_DOWN) {
        OffsetTileset(proc, 16); 
    }

    PutMapCursor(x << 4, y << 4, 0);


}

extern u16 sTilesetConfig[]; 
void RenderTilesetRowOnBg2(DebuggerProc* proc) {
    int ix, iy;
    //RegisterBlankTile(0x400); 
    //BG_Fill(gBG0TilemapBuffer, 0);
    //BG_Fill(gBG1TilemapBuffer, 0);
    //SetBackgroundTileDataOffset(2, 0);
    //BG_Fill(gBG2TilemapBuffer, 0);
    //
    //BG_EnableSyncByMask(BG0_SYNC_BIT | BG1_SYNC_BIT | BG2_SYNC_BIT);
    
    RenderBmMapOnBg2(); 
    
    SetBackgroundTileDataOffset(2, 0x8000);

    gBmSt.mapRenderOrigin.x = gBmSt.camera.x >> 4;
    gBmSt.mapRenderOrigin.y = gBmSt.camera.y >> 4;

    for (iy = (10 - 1); iy >= 9; --iy) // 9 so only bottom row 
        for (ix = (15 - 1); ix >= 0; --ix)
            DisplayTilesetTile(proc, gBG2TilemapBuffer, ix, iy,
                (short) gBmSt.mapRenderOrigin.x + ix, (short) gBmSt.mapRenderOrigin.y + iy);

    BG_EnableSyncByMask(1 << 2);
    BG_SetPosition(2, 0, 0);
}

void DisplayTilesetTile(DebuggerProc* proc, u16* bg, int xTileMap, int yTileMap, int xBmMap, int yBmMap) {
    u16* out = bg + yTileMap * 0x40 + xTileMap * 2; // TODO: BG_LOCATED_TILE?
    //u16* tile = sTilesetConfig + gBmMapBaseTiles[yBmMap][xBmMap];
    
    u16* tile = sTilesetConfig + (proc->tmp[xTileMap] << 2);

    // TODO: palette id constants
    u16 base = gBmMapFog[yBmMap][xBmMap] ? (6 << 12) : (11 << 12);

    out[0x00 + 0] = base + *tile++;
    out[0x00 + 1] = base + *tile++;
    out[0x20 + 0] = base + *tile++;
    out[0x20 + 1] = base + *tile++;
}

void EditMapInit(DebuggerProc* proc) { 
    ClearTilesetRow(proc); 
    StartPlayerPhaseTerrainWindow(); 
} 



void FixAndHandlePlayerCursorMovement(void) { 
    FixCursorOverflow(); 
    HandlePlayerCursorMovement(); 
} 

extern const struct ProcCmd gProcScr_TerrainDisplay[]; 
void EditMapIdle(DebuggerProc* proc) { 
    FixAndHandlePlayerCursorMovement();
    int x = gBmSt.playerCursor.x; 
    int y = gBmSt.playerCursor.y; 
    if (gKeyStatusPtr->newKeys & A_BUTTON) { // see https://github.com/FireEmblemUniverse/fireemblem8u/blob/a608c6c4b6bc0cdf15f14292c99657cae73f6bdb/src/bmmap.c#L271
        gBmMapBaseTiles[y][x] = proc->tileID << 2;
        RefreshTerrainBmMap();
        UpdateRoofedUnits();
        RenderBmMap();
        ProcPtr terrainDispProc = Proc_Find(gProcScr_TerrainDisplay); 
        Proc_Goto(terrainDispProc, 0); // new terrain 
        //PlaySoundEffect(0x6A);
        return; 
    } 
    
    if (gKeyStatusPtr->newKeys & B_BUTTON) { 
        PlaySoundEffect(0x6A);
        Proc_Goto(proc, ChooseTileLabel); 
        return; 
    } 
    if (gKeyStatusPtr->newKeys & (R_BUTTON | START_BUTTON)) {
        PlaySoundEffect(0x6A);
        Proc_Goto(proc, ChooseTileLabel); 
        return; 
    }
    PutMapCursor(gBmSt.playerCursorDisplay.x, gBmSt.playerCursorDisplay.y, IsUnitSpriteHoverEnabledAt(x, y) ? 3 : 0);
}


void PickupUnitIdle(DebuggerProc* proc) { 
    FixAndHandlePlayerCursorMovement(); 
    if (gKeyStatusPtr->newKeys & A_BUTTON) { 
        gActionData.xMove = gBmSt.playerCursor.x; 
        gActionData.yMove = gBmSt.playerCursor.y; 
        gActiveUnitMoveOrigin.x = gBmSt.playerCursor.x;
        gActiveUnitMoveOrigin.y = gBmSt.playerCursor.y;
        PlayerPhase_ApplyUnitMovementWithoutMenu(proc); 
        ClearActiveUnitStuff(proc); 
        PlaySoundEffect(0x6B);
        Proc_Goto(proc, RestartLabel);
        return; 
    } 
    
    if (gKeyStatusPtr->newKeys & B_BUTTON) { 
        gActionData.xMove = gActiveUnitMoveOrigin.x; 
        gActionData.yMove = gActiveUnitMoveOrigin.y; 
        PlayerPhase_ApplyUnitMovementWithoutMenu(proc); 
        ClearActiveUnitStuff(proc); 
        PlaySoundEffect(0x6B);
        Proc_Goto(proc, RestartLabel); 
        return; 
    } 
    PutMapCursor(
        gBmSt.playerCursorDisplay.x, gBmSt.playerCursorDisplay.y,
        IsUnitSpriteHoverEnabledAt(gBmSt.playerCursor.x, gBmSt.playerCursor.y) ? 3 : 0);
}

void ClearActiveUnitStuff(DebuggerProc* proc) { 
    EndAllMus(); 
    if (gActiveUnit) { 
        //if (UNIT_FACTION(gActiveUnit) == gPlaySt.faction) { // if turn of the actor, refresh 
        //EndAllMus();
        gActiveUnit->state &= ~(US_HIDDEN|US_UNSELECTABLE|US_CANTOING); 
        //} 
    } 

    EnsureCameraOntoPosition(proc, gActiveUnitMoveOrigin.x, gActiveUnitMoveOrigin.y);
    SetCursorMapPosition(gActiveUnitMoveOrigin.x, gActiveUnitMoveOrigin.y);
    gBmSt.gameStateBits &= ~BM_FLAG_3;

    HideMoveRangeGraphics();

    RefreshEntityBmMaps();
    RefreshUnitSprites();
    RenderBmMap();

    //PlaySoundEffect(0x6B);

    //Proc_Goto(proc, 9);
} 

void PlayerPhase_ApplyUnitMovementWithoutMenu(DebuggerProc* proc) { 
    gActiveUnit->xPos = gActionData.xMove;
    gActiveUnit->yPos = gActionData.yMove;
    UnitFinalizeMovement(gActiveUnit); 
    ResetTextFont();
}

int PlayerPhase_PrepareActionBasic(DebuggerProc* proc) { 
    s8 cameraReturn;
    SetupUnitFunc(); 

    cameraReturn = EnsureCameraOntoPosition(
        proc, GetUnit(gActionData.subjectIndex)->xPos, GetUnit(gActionData.subjectIndex)->yPos);
    cameraReturn ^= 1;
    //if ((gActionData.unitActionType != UNIT_ACTION_WAIT) && !gBmSt.just_resumed)
    //{
    //    gActionData.suspendPointType = SUSPEND_POINT_DURINGACTION;
    //    WriteSuspendSave(SAVE_ID_SUSPEND);
    //}

    return cameraReturn;

} 

void CallPlayerPhase_FinishAction(DebuggerProc* proc) { 
    PlayerPhase_FinishActionNoCanto(proc); 
    ProcPtr playerPhaseProc = Proc_Find(gProcScr_PlayerPhase); 
    Proc_Goto(playerPhaseProc, 0); 
} 

int UnitActionFunc(DebuggerProc* proc) { 
    switch (proc->actionID) { 
        case ActionID_Promo: { 
            PromoAction(proc); 
        break; } 
        case ActionID_Arena: { 
            ArenaAction(proc); 
        break; } 
        case ActionID_Levelup: { 
            LevelupAction(proc); 
        break; } 
        
        default: 
    } 
    proc->actionID = 0; 
    return 0; 
} 

int PromoAction(DebuggerProc* proc) { 
    StartBmPromotion(proc);
    Proc_Goto(proc, PostActionLabel); 
    return 0; 
} 
int ArenaAction(DebuggerProc* proc) { 
    StartArenaScreen();
    Proc_Goto(proc, PostActionLabel); 
    return 0; 
} 
// extern const struct ProcCmd sProcScr_BattleAnimSimpleLock[]; 
int LevelupAction(DebuggerProc* proc) { 
    
    gActiveUnit->exp = 99; 
    InitBattleUnit(&gBattleActor, gActiveUnit);
    //if (UNIT_FACTION(&gBattleActor.unit) != FACTION_BLUE)
        //return;

    if (CanBattleUnitGainLevels(&gBattleActor)) { // see BattleApplyMiscAction 
        if (!(gPlaySt.chapterStateBits & PLAY_FLAG_EXTRA_MAP)) { 

            gBattleActor.expGain = 1;
            gBattleActor.unit.exp += 1;

            CheckBattleUnitLevelUp(&gBattleActor);
             
            //Proc_StartBlocking(sProcScr_BattleAnimSimpleLock, proc);
            EndAllMus();
            ResetText(); 
            
            gBattleActor.weaponBefore = 1; // see BeginMapAnimForSummon

            gManimSt.hp_changing = 0;
            gManimSt.mapAnimKind = 0;
            gManimSt.actorCount = 1;

            gManimSt.subjectActorId = 0;
            gManimSt.targetActorId = 1;

            SetupMapBattleAnim(&gBattleActor, &gBattleTarget, gBattleHitArray);
            //Proc_Start(ProcScr_MapAnimSummon, PROC_TREE_3);
            Proc_Goto(proc, LevelupLabel); 
            return 0; 
        }
    }
    Proc_Goto(proc, PostActionLabel); 
    
    
    return 0; 
} 

u8 StartPromotionNow(struct MenuProc * menu, struct MenuItemProc * menuItem) {
    //SetupUnitFunc(); 
    if (CanActiveUnitPromote() != 1) { return MENU_ACT_SKIPCURSOR | MENU_ACT_SND6B; } 
	DebuggerProc* proc; 
	proc = Proc_Find(DebuggerProcCmd); 
    proc->actionID = ActionID_Promo; 
    Proc_Goto(proc, UnitActionLabel); 
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
} 
u8 StartArenaNow(struct MenuProc * menu, struct MenuItemProc * menuItem) {
    //SetupUnitFunc(); 
	DebuggerProc* proc; 
	proc = Proc_Find(DebuggerProcCmd); 
    proc->actionID = ActionID_Arena; 
    Proc_Goto(proc, UnitActionLabel); // 0xb7 
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}
u8 LevelupNow(struct MenuProc * menu, struct MenuItemProc * menuItem) {
    //SetupUnitFunc(); 
	DebuggerProc* proc; 
	proc = Proc_Find(DebuggerProcCmd); 
    proc->actionID = ActionID_Levelup; 
    Proc_Goto(proc, UnitActionLabel); // 0xb7 
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}
u8 ChStateNow(struct MenuProc * menu, struct MenuItemProc * menuItem) {
	DebuggerProc* proc; 
	proc = Proc_Find(DebuggerProcCmd); 
    Proc_Goto(proc, ChStateLabel); 
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

u8 StartGodmodeNow(struct MenuProc * menu, struct MenuItemProc * menuItem) {
    //SetupUnitFunc(); 
	DebuggerProc* proc; 
	proc = Proc_Find(DebuggerProcCmd); 
    proc->actionID = 0; 
    Proc_Goto(proc, RestartLabel); // 0xb7 
    DebuggerProc* procIdler = Proc_Find(DebuggerProcCmdIdler); 
    if (procIdler->godMode) { 
        procIdler->godMode = false; 
        proc->godMode = false; 
    } 
    else { 
        procIdler->godMode = true; 
        proc->godMode = true; 
    } 
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

u8 ToggleBootNow(struct MenuProc * menu, struct MenuItemProc * menuItem) {
    //SetupUnitFunc(); 
	DebuggerProc* proc; 
	proc = Proc_Find(DebuggerProcCmd); 
    proc->actionID = 0; 
    Proc_Goto(proc, RestartLabel); // 0xb7 
    int boot = GetBootType(); 
    boot++; 
    boot %= 4;
    SetBootType(boot); 
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

u8 ControlAiNow(struct MenuProc * menu, struct MenuItemProc * menuItem) {
    //SetupUnitFunc(); 
	DebuggerProc* proc; 
	proc = Proc_Find(DebuggerProcCmd); 
    proc->actionID = 0; 
    Proc_Goto(proc, RestartLabel); // 0xb7 
    //DebuggerProc* procIdler = Proc_Find(DebuggerProcCmdIdler); 
    if (gPlaySt.config.debugControlRed) { 
        gPlaySt.config.debugControlRed = 0; 
    } 
    else { 
        gPlaySt.config.debugControlRed = 2; 
    } 
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

u8 PageIncrementNow(struct MenuProc * menu, struct MenuItemProc * menuItem) {
    //SetupUnitFunc(); 
	DebuggerProc* proc; 
	proc = Proc_Find(DebuggerProcCmd); 
    proc->actionID = 0; 
    Proc_Goto(proc, RestartLabel); // 0xb7 
    DebuggerProc* procIdler = Proc_Find(DebuggerProcCmdIdler); 
    proc->page++; 
    if (proc->page > (NumberOfPages-1)) { 
        proc->page = 0; 
    } 
    procIdler->page = proc->page; 
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}


// void ComputeBattleUnitEffectiveStats(struct BattleUnit* attacker, struct BattleUnit* defender) {
//     ComputeBattleUnitEffectiveHitRate(attacker, defender);
//     ComputeBattleUnitEffectiveCritRate(attacker, defender);
//     ComputeBattleUnitSilencerRate(attacker, defender);
//     ComputeBattleUnitSpecialWeaponStats(attacker, defender);
// 	DebuggerProc* proc; 
// 	proc = Proc_Find(DebuggerProcCmdIdler); 
//     if (!proc) { return; } 
//     #define MaxStat 99 
//     if (proc->godMode) { 
//         struct BattleUnit* bunitA = attacker; 
//         struct BattleUnit* bunitB = defender; 
//         if (UNIT_FACTION(&attacker->unit) == FACTION_RED) { 
//             bunitA = defender; bunitB = attacker; 
//         } 
//         bunitA->battleAttack = bunitB->unit.maxHP;
//         bunitA->battleDefense = MaxStat;
//         bunitA->battleSpeed = MaxStat;
//         bunitA->battleHitRate = MaxStat*2;
//         bunitA->battleAvoidRate = MaxStat;
//         bunitA->battleEffectiveHitRate = 100;
//         bunitA->battleCritRate = MaxStat*2;
//         bunitA->battleDodgeRate = 100;
//         bunitA->battleEffectiveCritRate = 100;

//         bunitB->hpInitial = 1; 
//         bunitB->battleAttack = 0;
//         bunitB->battleDefense = 0;
//         bunitB->battleSpeed = 0;
//         bunitB->battleHitRate = 0;
//         bunitB->battleAvoidRate = 0;
//         bunitB->battleEffectiveHitRate = 0;
//         bunitB->battleCritRate = 0;
//         bunitB->battleDodgeRate = 0;
//         bunitB->battleEffectiveCritRate = 0;
//     }
    
// }

u8 PickupUnitNow(struct MenuProc * menu, struct MenuItemProc * menuItem) {
    //SetupUnitFunc(); 
	DebuggerProc* proc; 
	proc = Proc_Find(DebuggerProcCmd); 
    Proc_Goto(proc, PickupUnitLabel);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

u8 EditMapNow(struct MenuProc * menu, struct MenuItemProc * menuItem) {
    //SetupUnitFunc(); 
	DebuggerProc* proc; 
	proc = Proc_Find(DebuggerProcCmd); 
    Proc_Goto(proc, ChooseTileLabel);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

u8 EditStatsNow(struct MenuProc * menu, struct MenuItemProc * menuItem) {
	DebuggerProc* proc; 
	proc = Proc_Find(DebuggerProcCmd); 
    Proc_Goto(proc, EditStatsLabel);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}
u8 EditItemsNow(struct MenuProc * menu, struct MenuItemProc * menuItem) {
	DebuggerProc* proc; 
	proc = Proc_Find(DebuggerProcCmd); 
    Proc_Goto(proc, EditItemsLabel);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}
u8 EditMiscNow(struct MenuProc * menu, struct MenuItemProc * menuItem) {
	DebuggerProc* proc; 
	proc = Proc_Find(DebuggerProcCmd); 
    Proc_Goto(proc, EditMiscLabel);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

u8 LoadUnitsNow(struct MenuProc * menu, struct MenuItemProc * menuItem) {
	DebuggerProc* proc; 
	proc = Proc_Find(DebuggerProcCmd); 
    Proc_Goto(proc, LoadUnitsLabel);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}
u8 EditStateNow(struct MenuProc * menu, struct MenuItemProc * menuItem) {
	DebuggerProc* proc; 
	proc = Proc_Find(DebuggerProcCmd); 
    Proc_Goto(proc, StateLabel);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}
u8 EditWExpNow(struct MenuProc * menu, struct MenuItemProc * menuItem) {
	DebuggerProc* proc; 
	proc = Proc_Find(DebuggerProcCmd); 
    Proc_Goto(proc, WExpLabel);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}
u8 EditSkillsNow(struct MenuProc * menu, struct MenuItemProc * menuItem) {
	DebuggerProc* proc; 
	proc = Proc_Find(DebuggerProcCmd); 
    Proc_Goto(proc, SkillsLabel);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

u8 AiControlRemainingUnitsNow(struct MenuProc * menu, struct MenuItemProc * menuItem)
{
    DebuggerProc * proc;
 
    proc = Proc_Find(DebuggerProcCmd);
 
    proc->actionID = 0;

    Proc_Goto(proc, RestartLabel); // 0xb7
 
    DebuggerProc * procIdler = Proc_Find(DebuggerProcCmdIdler);
 
    if (procIdler->autoplay)
    {
        procIdler->autoplay = false;
        proc->autoplay = false;
    }

    else
    {
        procIdler->autoplay = true;
        proc->autoplay = true;
    }

    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

int ShouldStartDebugger(void) { 
    if (CheckFlag(DebuggerTurnedOff_Flag)) { return false; } 
    return true; 
} 


void SetupUnitFunc(void) { 
    gBattleActor.weaponBefore = gBattleTarget.weaponBefore = GetUnit(gActionData.subjectIndex)->items[gActionData.itemSlotIndex];

    gBattleActor.weapon = gBattleTarget.weapon = GetUnitEquippedWeapon(GetUnit(gActionData.subjectIndex));
    gBattleActor.hasItemEffectTarget = 0;
    gBattleTarget.statusOut = -1;
    gActionData.unitActionType = 1; 
    UnitBeginAction(gActiveUnit); 
}

extern u8* gPromoJidLut;
extern int GetPromoTable(int classNumber, int aOrB);
u8 CanActiveUnitPromote(void) { 
    if (UNIT_FACTION(gActiveUnit) != gPlaySt.faction) { return 2; } 
    u8* promoTable = gPromoJidLut; 
    int classNumber = gActiveUnit->pClassData->number; 
    if ((!promoTable[classNumber * 2]) && (!promoTable[(classNumber * 2) + 1])) { // gPromoJidLut[classNumber][0]; 
        return 2; // greyed out 
    } 
            
    return 1; 
} 
u8 CanActiveUnitPromoteMenu(const struct MenuItemDef* def, int number) { 
    return CanActiveUnitPromote(); 
} 


u8 CallArenaIsUnitAllowed(const struct MenuItemDef* def, int number) { 
    return ArenaIsUnitAllowed(gActiveUnit); 
} 

u8 CallEndEventNow(struct MenuProc * menu, struct MenuItemProc * menuItem) {
    //SetupUnitFunc(); 
	DebuggerProc* proc; 
	proc = Proc_Find(DebuggerProcCmd); 
    Proc_Goto(proc, EndLabel);
    CallEndEvent(); 
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}



extern const struct MenuItemDef gDebuggerMenuItems[]; 
extern const struct MenuItemDef gDebuggerMenuItemsPage2[]; 
extern const struct MenuItemDef gDebuggerMenuItemsPage3[]; 
extern char* gDebuggerMenuText[]; 

extern const struct MenuItemDef* ggDebuggerMenuItems[]; 

int CountDebuggerMenuItems(int page) { 
    int result = 0; 
    for (int i = 0; i < page; ++i) { 
        for (int c = 0; c < 255; ++c) { 
            if (!ggDebuggerMenuItems[i][c].name) { break; } 
            result++; 
        } 
    } 
    return result + page; // avoid the word 0 terminator offset  
} 

char* GetDebuggerMenuText(DebuggerProc* procIdler, int index) { 
    //index += procIdler->page * NumberOfOptions; 
    index += CountDebuggerMenuItems(procIdler->page); 
    return gDebuggerMenuText[index * 2]; 
} 
char* GetDebuggerMenuDesc(DebuggerProc* procIdler, int index) { 
    index += CountDebuggerMenuItems(procIdler->page); 
    return gDebuggerMenuText[(index * 2) + 1]; 
} 

int DebuggerMenuItemDraw(struct MenuProc * menu, struct MenuItemProc * menuItem) { 
    if (menuItem->availability == MENU_DISABLED) {
        Text_SetColor(&menuItem->text, 1);
    }
    DebuggerProc* procIdler = Proc_Find(DebuggerProcCmdIdler); 
    
    Text_DrawString(&menuItem->text, GetDebuggerMenuText(procIdler, menuItem->itemNumber));
    PutText(&menuItem->text, BG_GetMapBuffer(menu->frontBg) + TILEMAP_INDEX(menuItem->xTile, menuItem->yTile));
    return 0;
} 
int GodmodeDrawText(struct MenuProc * menu, struct MenuItemProc * menuItem) { 
    if (menuItem->availability == MENU_DISABLED) {
        Text_SetColor(&menuItem->text, 1);
    }
    DebuggerProc* procIdler = Proc_Find(DebuggerProcCmdIdler); 
    if (procIdler->godMode) { 
        Text_DrawString(&menuItem->text, " Godmode on");
    } 
    else { 
        Text_DrawString(&menuItem->text, GetDebuggerMenuText(procIdler, menuItem->itemNumber));
    } 
    PutText(&menuItem->text, BG_GetMapBuffer(menu->frontBg) + TILEMAP_INDEX(menuItem->xTile, menuItem->yTile));
    return 0;
} 

int BootmodeDrawText(struct MenuProc * menu, struct MenuItemProc * menuItem) { 
    if (menuItem->availability == MENU_DISABLED) {
        Text_SetColor(&menuItem->text, 1);
    }
    int boot = GetBootType();

    if (boot == 1) { 
        Text_DrawString(&menuItem->text, " Restart");
    } 
    else if (boot == 2) { 
        Text_DrawString(&menuItem->text, " Restart2");
    } 
    else if (boot == 3) { 
        Text_DrawString(&menuItem->text, " Resume");
    } 
    else { 
        Text_DrawString(&menuItem->text, " Boot title");
    } 
    PutText(&menuItem->text, BG_GetMapBuffer(menu->frontBg) + TILEMAP_INDEX(menuItem->xTile, menuItem->yTile));
    return 0;
} 


int ControlAiDrawText(struct MenuProc * menu, struct MenuItemProc * menuItem) { 
    if (menuItem->availability == MENU_DISABLED) {
        Text_SetColor(&menuItem->text, 1);
    }
    //DebuggerProc* procIdler = Proc_Find(DebuggerProcCmdIdler); 
    if (gPlaySt.config.debugControlRed) { 
        Text_DrawString(&menuItem->text, " AI is off");
    } 
    else { 
        DebuggerProc* procIdler = Proc_Find(DebuggerProcCmdIdler); 
        Text_DrawString(&menuItem->text, GetDebuggerMenuText(procIdler, menuItem->itemNumber));
    } 
    PutText(&menuItem->text, BG_GetMapBuffer(menu->frontBg) + TILEMAP_INDEX(menuItem->xTile, menuItem->yTile));
    return 0;
} 

int AiControlRemainingUnitsDrawText(struct MenuProc * menu, struct MenuItemProc * menuItem)
{
    if (menuItem->availability == MENU_DISABLED)
    {
        Text_SetColor(&menuItem->text, 1);
    }

    DebuggerProc * procIdler = Proc_Find(DebuggerProcCmdIdler);

    if (procIdler->autoplay)
    {
        Text_DrawString(&menuItem->text, " Autoplay on");
    }
    else
    {
        Text_DrawString(&menuItem->text, GetDebuggerMenuText(procIdler, menuItem->itemNumber));
    }

    PutText(&menuItem->text, BG_GetMapBuffer(menu->frontBg) + TILEMAP_INDEX(menuItem->xTile, menuItem->yTile));

    return 0;
}

void PageMenuItemDrawSprites(struct MenuProc* menu) { 
	DebuggerProc* proc; 
	proc = Proc_Find(DebuggerProcCmd); 
    int chr = 0x289;
    int x = (menu->menuItems[menu->itemCount - 1]->xTile * 8) + 6 + (8*3); 
    int y = (menu->menuItems[menu->itemCount - 1]->yTile * 8) + 4; 

    
    PutSprite(0, x, y, gObject_8x8, TILEREF(chr, 0) + OAM2_LAYER(0) + proc->page + 1); x+=8; 
    PutSprite(0, x, y, gObject_8x8, TILEREF(chr, 0) + OAM2_LAYER(0)); x+=8; 
    PutSprite(0, x, y, gObject_8x8, TILEREF(chr, 0) + OAM2_LAYER(0) + NumberOfPages); x+=8; 

} 


int PageMenuItemDraw(struct MenuProc * menu, struct MenuItemProc * menuItem) { 
    if (menuItem->availability == MENU_DISABLED) {
        Text_SetColor(&menuItem->text, 1);
    }
    DebuggerProc* procIdler = Proc_Find(DebuggerProcCmdIdler); 
    Text_DrawString(&menuItem->text, GetDebuggerMenuText(procIdler, menuItem->itemNumber));
    PutText(&menuItem->text, BG_GetMapBuffer(menu->frontBg) + TILEMAP_INDEX(menuItem->xTile, menuItem->yTile));
    //PageMenuItemDrawSprites(menuItem); 
    return 0; 
}

struct Unit* GetNextUnit(int deployId, int allegiance) { 
    struct Unit* unit;
    //deployId++;
    for (int i = deployId+1; i < ((allegiance & 0xC0) + 0x40); ++i)
    {
        unit = GetUnit(i); 
        if (UNIT_IS_VALID(unit))
        {
            return unit;
        }
    }
    for (int i = allegiance; i < deployId; ++i)
    {
        unit = GetUnit(i); 
        if (UNIT_IS_VALID(unit))
        {
            return unit;
        }
    }
    return NULL; 
} 

struct Unit* GetPrevUnit(int deployId, int allegiance) { 
    struct Unit* unit;
    //deployId--;
    //if (!deployId) { deployId = ((allegiance & 0xC0) + 0x3F); } 
    for (int i = deployId-1; i >= allegiance; --i) // should loop back to itself I guess 
    {
        unit = GetUnit(i); 
        if (UNIT_IS_VALID(unit))
        {
            return unit;
        }
    }
    for (int i = ((allegiance & 0xC0) + 0x3F); i > deployId; --i) // should loop back to itself I guess 
    {
        unit = GetUnit(i); 
        if (UNIT_IS_VALID(unit))
        {
            return unit;
        }
    }
    return NULL; 
} 

void SwapToPreviousUnit(DebuggerProc* proc) { 
    struct Unit* unit = proc->unit; 
    int deployId = unit->index & 0xFF; 
    int allegiance = UNIT_FACTION(unit); // 0x00, 0x40, or 0x80 
    if (!allegiance) { allegiance = 1; } // start GetUnit(i) at 1, not 0. 
    unit = GetPrevUnit(deployId, allegiance); 
    if (unit) { proc->unit = unit; } 
}
void SwapToNextUnit(DebuggerProc* proc) { 
    struct Unit* unit = proc->unit; 
    int deployId = unit->index & 0xFF; 
    int allegiance = UNIT_FACTION(unit); 
    unit = GetNextUnit(deployId, allegiance); 
    if (unit) { proc->unit = unit; } 
}

u8 PageIdler(struct MenuProc* menu, struct MenuItemProc* command) { 
    u16 keys = gKeyStatusPtr->repeatedKeys; 
    PageMenuItemDrawSprites(menu); 
    if (!keys) { return MENU_ITEM_NONE; } 
	DebuggerProc* proc = Proc_Find(DebuggerProcCmd); 
    DebuggerProc* procIdler = Proc_Find(DebuggerProcCmdIdler); 
    proc->mainID = menu->itemCurrent; 
    procIdler->mainID = menu->itemCurrent; 
    int page = proc->page; 
    
    if (keys & L_BUTTON) { 
        SwapToPreviousUnit(proc); 
        gActiveUnitMoveOrigin.x = proc->unit->xPos; 
        gActiveUnitMoveOrigin.y = proc->unit->yPos; 
        Proc_Goto(proc, RestartLabel); 
        return MENU_ACT_SKIPCURSOR | MENU_ACT_CLEAR | MENU_ACT_END | MENU_ACT_SND6A;
    } 
    if (keys & R_BUTTON) { 
        SwapToNextUnit(proc);
        gActiveUnitMoveOrigin.x = proc->unit->xPos; 
        gActiveUnitMoveOrigin.y = proc->unit->yPos;         
        Proc_Goto(proc, RestartLabel); 
        return MENU_ACT_SKIPCURSOR | MENU_ACT_CLEAR | MENU_ACT_END | MENU_ACT_SND6A;
    } 
    
    if (keys & DPAD_LEFT) { 
        page--; 
    }
    if (keys & DPAD_RIGHT) { 
        page++; 
    } 
    if (proc->page != page) { 
        if (page < 0) { page = NumberOfPages-1; } 
        if (page >= NumberOfPages) { page = 0; } 
        proc->page = page; 
        procIdler->page = page; 
        Proc_Goto(proc, RestartLabel); 
        return MENU_ACT_SKIPCURSOR | MENU_ACT_CLEAR | MENU_ACT_END | MENU_ACT_SND6A;
    } 
    return MENU_ITEM_NONE;
    
    

} 

u8 MenuCancelSelectResumePlayerPhase(struct MenuProc* menu, struct MenuItemProc* item)
{
	DebuggerProc* proc; 
	proc = Proc_Find(DebuggerProcCmd); 
    Proc_Goto(proc, EndLabel); 
    return MENU_ACT_SKIPCURSOR | MENU_ACT_CLEAR | MENU_ACT_END | MENU_ACT_SND6B;
}

u8 DebuggerHelpBox(struct MenuProc* menu, struct MenuItemProc* item); 
const struct MenuDef gDebuggerMenuDef = {
    {1, 0, 9, 0}, // { s8 x, y, w, h; };
    0,
    gDebuggerMenuItems,
    0, 0, 0,
    MenuCancelSelectResumePlayerPhase,
    MenuAutoHelpBoxSelect,
    DebuggerHelpBox
};


const struct MenuDef gDebuggerMenuDefPage2 = {
    {1, 0, 9, 0}, // { s8 x, y, w, h; };
    0,
    gDebuggerMenuItemsPage2,
    0, 0, 0,
    MenuCancelSelectResumePlayerPhase,
    MenuAutoHelpBoxSelect,
    DebuggerHelpBox
};
const struct MenuDef gDebuggerMenuDefPage3 = {
    {1, 0, 9, 0}, // { s8 x, y, w, h; };
    0,
    gDebuggerMenuItemsPage3,
    0, 0, 0,
    MenuCancelSelectResumePlayerPhase,
    MenuAutoHelpBoxSelect,
    DebuggerHelpBox
};


void UnitBeginActionInit(struct Unit* unit) {
    gActiveUnit = unit;
    gActiveUnitId = unit->index;

    gActiveUnitMoveOrigin.x = unit->xPos;
    gActiveUnitMoveOrigin.y = unit->yPos;
    gActionData.xMove = unit->xPos; 
    gActionData.yMove = unit->yPos; 

    gActionData.subjectIndex = unit->index;
    gActionData.itemSlotIndex = -1; 
    gActionData.unitActionType = 0;
    gActionData.moveCount = 0;

    gBmSt.taken_action = 0;
    gBmSt.unk3F = 0xFF;

    sub_802C334(); // zeroes out a few bits of unknown ram 

    //gActiveUnit->state |= US_HIDDEN;
    //gBmMapUnit[unit->yPos][unit->xPos] = 0;
}

int RestartNow(DebuggerProc* proc) { 
    Proc_Goto(proc, RestartLabel); 
    return 0; // yield 
}


void StartDebuggerProc(ProcPtr playerPhaseProc) { // based on PlayerPhase_MainIdle
    if (!ShouldStartDebugger()) { return; } 
    struct Unit * unit = GetUnit(gBmMapUnit[gBmSt.playerCursor.y][gBmSt.playerCursor.x]);
    if (!unit) { return; } 
    gActiveUnitMoveOrigin.x = unit->xPos; 
    gActiveUnitMoveOrigin.y = unit->yPos; 
    UnitBeginActionInit(unit); 
    DebuggerProc* procIdler = Proc_Find(DebuggerProcCmdIdler); 
    if (!procIdler) { 
        procIdler = Proc_Start(DebuggerProcCmdIdler, (void*)3);
        InitProc(procIdler); 
    } 
    procIdler->unit = unit; 
    
	DebuggerProc* proc = Proc_Find(DebuggerProcCmd); 
	if (!proc) { 
		//proc = Proc_Start(DebuggerProcCmd, (void*)3);
        //ProcPtr playerPhaseProc = Proc_Find(gProcScr_PlayerPhase); 
		proc = Proc_StartBlocking(DebuggerProcCmd, playerPhaseProc); 
        InitProc(proc); 
        CopyProcVariables(proc, procIdler); 
	} 
    //RestartDebuggerMenu(proc);
    //Proc_Goto(proc, RestartLabel); 
    
}
void MakeMoveunitForAnyActiveUnit(void) {
    if (!MuExists()) {
        StartMu(gActiveUnit);
        HideUnitSprite(gActiveUnit);
    }
    SetAutoMuDefaultFacing();
}

void InitProc(DebuggerProc * proc)
{
    proc->mainID = 0;
    proc->page = 0;
    proc->editing = false;
    proc->actionID = 0;
    proc->godMode = 0;
    proc->autoplay = 0;
    proc->lastFlag = 1;
    proc->tileID = 1;
    proc->id = 0;
    proc->lastTileHovered = 0;
    for (int i = 0; i < tmpSize; ++i)
    {
        proc->tmp[i] = 0;
    }
}

void RestartDebuggerMenu(DebuggerProc* proc) { 
    ClearActiveUnitStuff(proc); // in case we didn't refresh units before restarting 
    struct Unit * unit = proc->unit; //GetUnit(gBmMapUnit[gBmSt.playerCursor.y][gBmSt.playerCursor.x]);
    if (!unit) { Proc_Goto(proc, EndLabel); return; } 
    EndAllMenus();
    ResetText();
    ProcPtr playerPhaseProc = Proc_Find(gProcScr_PlayerPhase); 
    Proc_Goto(playerPhaseProc, 9); // wait for menu? 
    UnitBeginActionInit(unit); 
    proc->actionID = 0; 
    proc->editing = false; 
    proc->actionID = 0; 
    proc->id = 0; 
    for (int i = 0; i < tmpSize; ++i) { 
        proc->tmp[i] = 0; 
    } 
    
    gPlaySt.xCursor = gBmSt.playerCursor.x;
    gPlaySt.yCursor = gBmSt.playerCursor.y;
    //EndAllMus();
    //ShowUnitSprite(unit);
    //UnitSpriteHoverUpdate();
    

    //gBmMapUnit[gActiveUnit->yPos][gActiveUnit->xPos] = 0;
    gActiveUnit->state |= US_HIDDEN;
    HideUnitSprite(gActiveUnit);
    MakeMoveunitForAnyActiveUnit(); 
    
    gBmSt.gameStateBits &= ~(BM_FLAG_0 | BM_FLAG_1);
    gBmSt.gameStateBits &= ~BM_FLAG_3;
    PutMapCursor(
        gBmSt.playerCursorDisplay.x, gBmSt.playerCursorDisplay.y,
        IsUnitSpriteHoverEnabledAt(gBmSt.playerCursor.x, gBmSt.playerCursor.y) ? 3 : 0);
    
    struct MenuProc* menu = NULL; 
    switch (proc->page) { 
        case 0: { menu = StartOrphanMenuAdjusted(&gDebuggerMenuDef, gBmSt.cursorTarget.x - gBmSt.camera.x, 1, 0x15); break; } 
        case 1: { menu = StartOrphanMenuAdjusted(&gDebuggerMenuDefPage2, gBmSt.cursorTarget.x - gBmSt.camera.x, 1, 0x15); break; } 
        case 2: { menu = StartOrphanMenuAdjusted(&gDebuggerMenuDefPage3, gBmSt.cursorTarget.x - gBmSt.camera.x, 1, 0x15); break; } 
        default: 
    }
    if (menu) { 
        menu->itemCurrent = proc->mainID; 
        int count = menu->itemCount - 1; 
        if (menu->itemCurrent >= count) { menu->itemCurrent = count; } 
    } 
    
    //UnpackUiFramePalette(3);
    Decompress(
        gUnknown_08A02274, (void*)(VRAM + 0x10000 + 0x240 * 0x20));

    //RefreshBMapGraphics(); // should not happen on the same frame as starting a menu, or black boxes occur 
    // perhaps they both use the generic buffer 
    
    
} 




void LoopDebuggerProc(DebuggerProc* proc) { 
    return; 
} 




/*
#define A_BUTTON        0x0001
#define B_BUTTON        0x0002
#define SELECT_BUTTON   0x0004
#define START_BUTTON    0x0008
#define DPAD_RIGHT      0x0010
#define DPAD_LEFT       0x0020
#define DPAD_UP         0x0040
#define DPAD_DOWN       0x0080
*/




void PlayerPhase_FinishActionNoCanto(ProcPtr proc)
{
    if (gPlaySt.chapterVisionRange != 0)
    {
        RenderBmMapOnBg2();

        MoveActiveUnit(gActionData.xMove, gActionData.yMove);

        RefreshEntityBmMaps();
        RenderBmMap();

        NewBMXFADE(0);

        RefreshUnitSprites();
    }
    else
    {
        MoveActiveUnit(gActionData.xMove, gActionData.yMove);

        RefreshEntityBmMaps();
        RenderBmMap();
    }

    SetCursorMapPosition(gActiveUnit->xPos, gActiveUnit->yPos);

    gPlaySt.xCursor = gBmSt.playerCursor.x;
    gPlaySt.yCursor = gBmSt.playerCursor.y;

    //if (TryMakeCantoUnit(proc)) // has PROC_GOTO in it 
    //{
    //    HideUnitSprite(gActiveUnit);
    //    return;
    //}

    // if (ShouldCallEndEvent())
    // {
        // EndAllMus();

        // RefreshEntityBmMaps();
        // RenderBmMap();
        // RefreshUnitSprites();

        // MaybeCallEndEvent_();

        // Proc_Goto(proc, 8);

        // return;
    // }

    EndAllMus();

    return;
}

extern void SetBlendConfig(u16 effect, u8 coeffA, u8 coeffB, u8 blendY); 
extern const struct ProcCmd gProcScr_TerrainDisplay[]; 
extern const struct ProcCmd gProcScr_PrepMap_MenuButtonDisplay[]; 
void InitPlayerPhaseTerrainWindow(); 
struct ProcCmd const gProcScr_TerrainWindowMaker[] = {
    PROC_WHILE(DoesBMXFADEExist),

    PROC_CALL(InitPlayerPhaseTerrainWindow),

    PROC_END,
};
void InitPlayerPhaseTerrainWindow() {

    gLCDControlBuffer.dispcnt.win0_on = 0;
    gLCDControlBuffer.dispcnt.win1_on = 0;
    gLCDControlBuffer.dispcnt.objWin_on = 0;

    gLCDControlBuffer.wincnt.wout_enableBg0 = 1;
    gLCDControlBuffer.wincnt.wout_enableBg1 = 1;
    gLCDControlBuffer.wincnt.wout_enableBg2 = 1;
    gLCDControlBuffer.wincnt.wout_enableBg3 = 1;
    gLCDControlBuffer.wincnt.wout_enableObj = 1;
    gLCDControlBuffer.wincnt.wout_enableBlend = 1;

    BG_SetPosition(0, 0, 0);
    BG_SetPosition(1, 0, 0);
    BG_SetPosition(2, 0, 0);

    SetBlendConfig(1, 0xD, 3, 0);

    SetBlendTargetA(0, 1, 0, 0, 0);

    SetBlendBackdropA(0);

    SetBlendTargetB(0, 0, 1, 1, 1);

    Decompress(gGfx_PlayerInterfaceFontTiles, (void*)(VRAM + 0x2000));
    Decompress(gGfx_PlayerInterfaceNumbers, (void*)(VRAM + 0x15C00));

    CpuFastSet((void*)(VRAM + 0x2EA0), (void*)(VRAM + 0x15D40), 8);

    ApplyPalette(gPaletteBuffer, 0x18);

    LoadIconPalette(1, 2);

    ResetTextFont();

    if (gPlaySt.config.disableTerrainDisplay == 0) {
        Proc_Start(gProcScr_TerrainDisplay, PROC_TREE_3);
    }

    return;
}



void StartPlayerPhaseTerrainWindow() {
    Proc_Start(gProcScr_TerrainWindowMaker, PROC_TREE_3);
    return;
}

u8 DebuggerHelpBox(struct MenuProc* menu, struct MenuItemProc* item)
{
    DebuggerProc* procIdler = Proc_Find(DebuggerProcCmdIdler); 
    StartHelpBoxString(item->xTile*8, item->yTile*8, GetDebuggerMenuDesc(procIdler, item->itemNumber));
    return 0; 
}


struct SpecialCharSt {
    s8 color;
    s8 id;
    s16 chr_position;
};
extern struct SpecialCharSt sSpecialCharStList[]; 
extern int AddSpecialChar(struct SpecialCharSt *st, int color, int id); 
extern void DrawSpecialCharGlyph(int a, int b, struct Glyph *glyph); 
extern struct Font *gActiveFont; 
int CustomAddSpecialChar(struct SpecialCharSt *st, int color, int id)
{
    st->color = color;
    st->id = id;
    st->chr_position = gActiveFont->chr_counter++;

    (st + 1)->color = -1;

    DrawSpecialCharGlyph(st->chr_position, color, TextGlyphs_System[id]);
    //DrawSpecialCharGlyph(st->chr_position, color, &gUnknown_0858FC14);

    return st->chr_position;
}

int CustomGetSpecialCharChr(int color, int id)
{
    struct SpecialCharSt *it = sSpecialCharStList;

    while (1) {
        if (it->color < 0)
            return CustomAddSpecialChar(it, color, id);

        if (it->color == color && it->id == id)
            return it->chr_position;

        it++;
    }
}

void CustomPutSpecialChar(u16 * tm, int color, int id)
{
    int chr;

    if (id == TEXT_SPECIAL_NOTHING) {
        tm[0x00] = 0;
        tm[0x20] = 0;

        return;
    }

    chr = CustomGetSpecialCharChr(color, id) * 2 + gActiveFont->tileref;

    tm[0x00] = chr;
    tm[0x20] = chr + 1;
}

void PutNumberHex(u16 *tm, int color, int number)
{
    if (number == 0) {
        PutSpecialChar(tm, color, TEXT_SPECIAL_BIGNUM_0);
        return;
    }

    int tmp; 
    while (number != 0) {
        tmp = number % 16; 
        if (tmp > 9) { 
            tmp -= 10; 

            // This is a temp fix for the capital "C" displaying as a lower case "a". We'll forcibly use lowercase "c" instead
            if (tmp == 2)
            {
                CustomPutSpecialChar(tm, color, 99 );    
            }
            else
                CustomPutSpecialChar(tm, color, 65 + tmp );
            //if (tmp >= 5) { CustomPutSpecialChar(tm, color, TEXT_SPECIAL_S ); } 
            //else { 
            //    PutSpecialChar(tm, color, tmp + TEXT_SPECIAL_A); 
            //} 
        } 
        else { 
            //PutSpecialChar(tm, color, number % 16 + TEXT_SPECIAL_BIGNUM_0);
            CustomPutSpecialChar(tm, color, 48 + tmp );
        } 
        number >>= 4;

        tm--;
    }
}
