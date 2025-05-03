u8 RefugeEffect(struct MenuProc* menu, struct MenuItemProc* menuItem);
u8 RefugeUsability(const struct MenuItemDef* def, int number);
void RefreshUnitTakeRefugeInfoWindows(ProcPtr);
// void RefreshUnitRefugeInfoWindows(struct Unit* unit);
void RefugeSelection_OnConstruction(ProcPtr proc);
u8 RefugeSelection_OnChange(ProcPtr proc, struct SelectTarget* target);
u8 RefugeSelection_OnSelect(ProcPtr proc, struct SelectTarget* target);
u8 RefugeSelection_OnHelp(ProcPtr proc, struct SelectTarget * target);
s8 CanUnitRefuge(struct Unit* actor, struct Unit* target);
void TryAddUnitToRefugeTargetList(struct Unit* unit);
void MakeRefugeTargetList(struct Unit* unit);

extern const struct UnitInfoWindowProc * sRescueUnitInfoWindows[2];