struct ChapterStatusProc {
    /* 00 */ PROC_HEADER;

    /* 29 */ u8 restoreStateOnExit;
    /* 2A */ u8 focusUnitOnExit;
    /* 2B */ u8 timesCompleted;
    /* 2C */ u8 numObjectiveTextLines;
    /* 2D */ u8 unk_2d;
    /* 2E */ u8 unitIndex;
    /* 2F */ u8 unk_2f;
    /* 30 */ u8 numAllyUnits;
    /* 31 */ u8 numEnemyUnits;

    /* 34 */ struct Unit* units[2];

    /* 3C */ u16 unk_3c;
    /* 3E */ u8 helpTextActive;
    /* 3F */ u8 unk_3f;
    /* 40 */ u8 unk_40;

    /* 41 */ u8 _pad[0x64-0x41];

    /* 64 */ u16 unk_64;
};

struct StatusScreenSt
{
    /* 00 */ struct Text th;
    /* 08 */ struct Font font;
};

void StartChapterStatusHelpBox(ProcPtr proc);
struct Unit * GetFirstValidBlueUnit_Unused(void);
struct Unit * GetEnemyBossUnit(void);
int CountEnemyBossUnits(void);
int CountUnitsByFaction(int faction);
void UpdateStatusFactionSelectorGlow(struct ChapterStatusProc * proc);
const char * SplitObjectiveTextOnNewline(const char * str);
void UpdateUnitSpritePal(bool isHidden);
void ChapterStatus_Init(struct ChapterStatusProc * proc);
void DrawChapterStatusTextForUnit(struct Unit * unit);
void ChapterStatus_ShowAllLayers(void);
void ChapterStatus_SetupFont(ProcPtr proc);
void DrawChapterStatusStatValues(void);
void ChapterStatus_DrawText(struct ChapterStatusProc * proc);
void ChapterStatus_LoopKeyHandler(struct ChapterStatusProc * proc);
void ChapterStatus_OnEnd(struct ChapterStatusProc * proc);
void ChapterStatus_FocusLeaderUnit(struct ChapterStatusProc * proc);
void StartChapterStatusScreen(ProcPtr parent);
void StartChapterStatusScreen_FromPrep(ProcPtr parent);
void StatusScreenSpriteDraw_Init(struct ChapterStatusProc * proc);
void StatusScreenSpriteDraw_Loop(struct ChapterStatusProc * proc);

extern struct ProcCmd gProcScr_ChapterStatusScreen[];
extern struct ProcCmd const ProcScr_ChapterStatusScreen_FromPrep[];
extern struct ProcCmd ProcScr_StatusScreenSpriteDraw[];