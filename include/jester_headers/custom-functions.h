extern void IsTraineeLevelCappedOrPromoted(void);
extern int GetStatIncrease_NEW(int growth, int expGained);
extern void MakeHurtTargetList(int faction);

extern void TryAddUnitToAdjacentEnemyTargetList(struct Unit* unit);
extern void MakeTargetListForAdjacentEnemies(struct Unit* unit);

extern void TryAddUnitToAdjacentEnemyNonBossTargetList(struct Unit* unit);
extern void MakeTargetListForAdjacentNonBossEnemies(struct Unit* unit);

extern void TryAddUnitToAdjacentSameFactionTargetList(struct Unit* unit);
extern void MakeTargetListForAdjacentSameFaction(struct Unit* unit);

extern void TryAddUnitToAdjacentUnitsTargetList(struct Unit* unit);
extern void MakeTargetListForAdjacentUnits(struct Unit* unit);

extern void TryAddUnitToRangedStatusStavesTargetList(struct Unit* unit);
extern void MakeTargetListForRangedStatusStaves(struct Unit* unit);

extern void ExecCustomStaves(ProcPtr proc);
extern void TryAddUnitToSlowTargetList(struct Unit* unit);
extern void MakeTargetListForSlow(struct Unit *unit);

struct SecondaryGoalWindowProc {
    PROC_HEADER;
    struct Text text; // Enough for HP display
    int xCursor, yCursor;
    int xCursorPrev, yCursorPrev;
    int unitIdPrev;
};

extern void DrawSecondaryGoalWindow(struct SecondaryGoalWindowProc *proc);
extern void SecondaryGoalWindow_Init(struct SecondaryGoalWindowProc *proc);
extern void SecondaryGoalWindow_Loop_Display(struct SecondaryGoalWindowProc *proc);
extern void GetGoalWindowPosition(int* x, int* y);
extern const struct ProcCmd gProcScr_SecondaryGoalWindow[];