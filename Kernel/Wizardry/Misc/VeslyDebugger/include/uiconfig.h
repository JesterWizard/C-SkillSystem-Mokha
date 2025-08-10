#ifndef GUARD_UI_CONFIG_H
#define GUARD_UI_CONFIG_H

struct Selector
{
    /* 00 */ u16 helpTextId;
    /* 02 */ u16 optionTextId;
    /* 04 */ u8 xPos;
    /* 05 */ u8 unk_05;
    STRUCT_PAD(0x06, 0x08);
};

struct GameOption
{
    /* 00 */ u16 msgId;
    /* 04 */ struct Selector selectors[4];
    /* 24 */ u8 icon;
    /* 28 */ bool (*func)(ProcPtr);
};

struct ConfigScreen
{
    /* 00 */ STRUCT_PAD(0x00, 0x2A);
    /* 2A */ s16 selectedOptionIdx;
    /* 2C */ s16 headOptionIdx; // option index at the top of the menu
    /* 2E */ u16 bg1YOffset;
    /* 30 */ STRUCT_PAD(0x30, 0x32);
    /* 32 */ s16 unk_32; // unused in FE8; appears to be leftover from FE7
    /* 34 */ s16 maxOption;
    /* 36 */ STRUCT_PAD(0x36, 0x37);
    /* 37 */ s8 source;
    /* 38 */ struct Text optionTexts[6];

    /* 68 */ struct Text text_68; // initialized but unused
    /* 70 */ struct Text valueTexts[6];

    /* A0 */ struct Text text_a0; // initialized but unused
    /* A8 */ struct Text optionHelpText;
};

struct ConfigProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x30);

    /* 30 */ s16 moving;
    /* 32 */ STRUCT_PAD(0x32, 0x36);
    /* 36 */ u8 loadSoloAnimScreen;
};

enum {
    GAME_OPTION_ANIMATION      =  0,
    GAME_OPTION_TERRAIN        =  1,
    GAME_OPTION_UNIT           =  2,
    GAME_OPTION_AUTOCURSOR     =  3,
    GAME_OPTION_TEXT_SPEED     =  4,
    GAME_OPTION_GAME_SPEED     =  5,
    GAME_OPTION_MUSIC          =  6,
    GAME_OPTION_SOUND_EFFECTS  =  7,
    GAME_OPTION_WINDOW_COLOR   =  8,
    GAME_OPTION_CPU_LEVEL      =  9,
    GAME_OPTION_COMBAT         = 10,
    GAME_OPTION_SUBTITLE_HELP  = 11,
    GAME_OPTION_AUTOEND_TURNS  = 12,
    GAME_OPTION_UNIT_COLOR     = 13,
    GAME_OPTION_OBJECTIVE      = 14,
    GAME_OPTION_CONTROLLER     = 15,
    GAME_OPTION_RANK_DISPLAY   = 16,
};

// ??? GetSelectedGameOption(???);
// ??? GetSelectedOptionValue(???);
// ??? DrawGameOptionIcon(???);
// ??? DrawGameOptionHelpText(???);
// ??? DrawGameOptionText(???);
// ??? DrawOptionValueTexts(???);
// ??? ConfigSprites_Init(???);
// ??? DrawConfigUiSprites(???);
// ??? Config_Init(???);
// ??? WindowColorOptionChangeHandler(???);
// ??? MusicOptionChangeHandler(???);
// ??? GenericOptionChangeHandler(???);
// ??? GetGameOption(???);
// ??? SetGameOption(???);
// ??? sub_80B2188(???);
// ??? Config_Loop_KeyHandler(???);
// ??? sub_80B2464(???);
// ??? sub_80B24AC(???);
// ??? sub_80B24C0(???);


// extern ??? gConfigUiState
// extern ??? gGameOptionsUiOrder
// extern ??? gSprite_ConfigurationUiHeader
// extern ??? gGameOptions
// extern ??? gProcScr_RedrawConfigHelpText
// extern ??? gProcScr_DrawConfigUiSprites
extern struct ProcCmd CONST_DATA gProcScr_Config1[];
extern struct ProcCmd CONST_DATA gProcScr_Config2[];
extern struct ProcCmd CONST_DATA gProcScr_Config3[];

#endif // GUARD_UI_CONFIG_H
