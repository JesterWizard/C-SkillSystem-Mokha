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