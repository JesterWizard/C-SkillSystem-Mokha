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

#ifdef CONFIG_FREE_MOVEMENT
    extern void EnableFreeMovementASMC();
    extern void DisableFreeMovementASMC();
#endif

#ifdef CONFIG_LIGHTS_OUT_GAME

    enum {
        DIMENSIONS_3x3 = 0,
        DIMENSIONS_3x4 = 1,
        DIMENSIONS_4x4 = 2,
        DIMENSIONS_4x5 = 3,

        ICON_COUNT_2 = 2,
        ICON_COUNT_3 = 3,
        
        CAN_SKIP = 0,
        CANT_SKIP = 1,

        REWARD_TIER_1 = 0,
        REWARD_TIER_2 = 1,
        REWARD_TIER_3 = 2,

        FLAG_0 = 0,
        FLAG_1 = 1,
        FLAG_2 = 2,
        FLAG_3 = 3,
        FLAG_4 = 4,
        FLAG_5 = 5,
        FLAG_6 = 6,
        FLAG_7 = 7,
        FLAG_8 = 8,
        FLAG_9 = 9,
        FLAG_10 = 10,
        FLAG_11 = 11,
        FLAG_12 = 12,
        FLAG_13 = 13,
        FLAG_14 = 14,
        FLAG_15 = 15
    };

    extern void PuzzleEvent();
    extern void PuzzleEvent2();
#endif

void PrepItemUseScroll_OnInit(struct ProcPrepItemUseJunaFruit * proc);
void PrepItemUseScroll_OnEnd(struct ProcPrepItemUseJunaFruit * proc);
void PrepItemUseScroll_OnDraw(struct ProcPrepItemUseJunaFruit * proc, int item, int x, int y);
extern const struct ProcCmd ProcScr_PrepItemUseScroll[];

extern const struct ProcCmd ProcScr_PrepItemUseArmsScroll[];
bool CanUnitUseArmsScroll(struct Unit * unit);
void ItemUseAction_ArmsScroll(struct Unit * unit);
void IER_PrepEffect_ArmsScroll(struct ProcPrepItemUse * proc, u16 item);
bool IER_PrepUsability_ArmsScroll(struct Unit *unit, int item);
void PrepItemUseArmsScroll_OnInit(struct ProcPrepItemUseJunaFruit * proc);
void PrepItemUseArmsScroll_OnEnd(struct ProcPrepItemUseJunaFruit * proc);
void PrepItemUseArmsScroll_OnDraw(struct ProcPrepItemUseJunaFruit * proc, int item, int x, int y);

int GetHighestWeaponRank(struct Unit * unit);