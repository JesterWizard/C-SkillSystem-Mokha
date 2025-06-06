#ifndef GUARD_PLAYER_INTERFACE_H
#define GUARD_PLAYER_INTERFACE_H

struct PlayerInterfaceProc
{
    /* 00 */ PROC_HEADER;
    /* 2A */ u8 pad[0x2C-0x2A];
    /* 2C */ struct Text texts[2];

    /* 3C */ s8 xBurst;
    /* 3D */ s8 yBurst;
    /* 3E */ s8 wBurst;
    /* 3F */ s8 hBurst;

    /* 40 */ u16 * statusTm;
    /* 44 */ s16 unitClock;
    /* 46 */ s16 xHp;
    /* 48 */ s16 yHp;
    /* 4A */ u8 burstUnitIdPrev;
    /* 4B */ u8 burstUnitId;
    /* 4C */ u8 xCursorPrev;
    /* 4D */ u8 yCursorPrev;
    /* 4E */ u8 xCursor;
    /* 4F */ u8 yCursor;
    /* 50 */ s8 cursorQuadrant;
    /* 51 */ u8 hpCurHi;
    /* 52 */ u8 hpCurLo;
    /* 53 */ u8 hpMaxHi;
    /* 54 */ u8 hpMaxLo;
    /* 55 */ s8 hideContents;
    /* 56 */ s8 isRetracting;
    /* 57 */ s8 windowQuadrant;
    /* 58 */ int showHideClock;
};

struct PlayerInterfaceConfigEntry
{
    /* 00 */ s8 xTerrain, yTerrain;
    /* 02 */ s8 xMinimug, yMinimug;
    /* 04 */ s8 xGoal, yGoal;
    STRUCT_PAD(0x06, 0x08);
};

extern struct PlayerInterfaceConfigEntry sPlayerInterfaceConfigLut[4];

// ??? GetWindowQuadrant(???);
// ??? GetCursorQuadrant(???);
// ??? GetHpBarLeftTile(???);
// ??? GetHpBarMidTiles(???);
// ??? GetHpBarRightTile(???);
// ??? DrawHpBar(???);
// ??? MMB_Loop_SlideIn(???);
// ??? MMB_Loop_SlideOut(???);
// ??? TerrainDisplay_Loop_SlideIn(???);
// ??? TerrainDisplay_Loop_SlideOut(???);
// ??? sub_808C234(???);
// ??? sub_808C288(???);
// ??? GetMinimugFactionPalette(???);
// ??? sub_808C314(???);
// ??? sub_808C33C(???);
// ??? sub_808C360(???);
// ??? MMB_DrawStatusText(???);
// ??? DrawUnitDisplayHpOrStatus(???);
void ApplyUnitMapUiFramePal(int faction, int palId);
void ClearUnitMapUiStatus(struct PlayerInterfaceProc * proc, u16 * buffer, struct Unit * unit);
void PutUnitMapUiStatus(u16 * buffer, struct Unit * unit);
void UnitMapUiUpdate(struct PlayerInterfaceProc * proc, struct Unit * unit);
void DrawUnitMapUi(struct PlayerInterfaceProc * proc, struct Unit * unit);
// ??? sub_808C710(???);
// ??? sub_808C750(???);
// ??? sub_808C8EC(???);
// ??? DrawTerrainDisplayWindow(???);
// ??? TerrainDisplay_Init(???);
// ??? TerrainDisplay_Loop_OnSideChange(???);
// ??? TerrainDisplay_Loop_Display(???);
// ??? MMB_Init(???);
// ??? MMB_Loop_OnSideChange(???);
// ??? MMB_Loop_Display(???);
// ??? MMB_CheckForUnit(???);
// ??? BurstDisplay_Init(???);
// ??? BurstDisplay_Loop_Display(???);
// ??? InitPlayerPhaseInterface(???);
void StartPlayerPhaseSideWindows(void);
void EndPlayerPhaseSideWindows(void);
// ??? sub_808D190(???);
// ??? sub_808D1B4(???);
// ??? sub_808D200(???);
// ??? GoalDisplay_Init(???);
// ??? GoalDisplay_Loop_OnSideChange(???);
// ??? sub_808D514(???);
// ??? GoalDisplay_Loop_SlideIn(???);
// ??? GoalDisplay_Loop_SlideOut(???);
// ??? sub_808D778(???);
// ??? __malloc_unlock_0(???);
// ??? sub_808D780(???);
// ??? GoalDisplay_Loop_Display(???);
// ??? IsAnyPlayerSideWindowRetracting(???);
// ??? MenuButtonDisp_Init(???);
// ??? sub_808D8A0(???);
// ??? sub_808D924(???);
// ??? MenuButtonDisp_UpdateCursorPos(???);
// ??? MenuButtonDisp_Loop_OnSlideIn(???);
// ??? MenuButtonDisp_Loop_Display(???);
// ??? MenuButtonDisp_Loop_OnSlideOut(???);


#endif  // GUARD_PLAYER_INTERFACE_H
