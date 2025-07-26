#include "include/gbafe.h"

extern const MenuDef GambitSelectMenu[];

// Menu
u8 Gambit_UpperMenu_Usability(const struct MenuItemDef * def, int param);
u8 Gambit_UpperMenu_Effect(struct MenuProc*, struct MenuItemProc * def);
int Gambit_UpperMenu_Hover(struct MenuProc*, struct MenuItemProc * def);
int Gambit_UpperMenu_Unhover(struct MenuProc*, struct MenuItemProc * def);

int GambitSelectMenu_Usability(const MenuCommandDefinition*, int param);
int GambitSelectMenu_Effect(struct MenuProc*, struct MenuItemProc * procCMD);
int GambitSelectMenu_Hover(struct MenuProc*, struct MenuItemProc * procCMD);
int GambitSelectMenu_Unhover(struct MenuProc*, struct MenuItemProc * procCMD);




// Target Select
extern const struct TargetSelectionDefinition pGambit_TargetSelection_Func[];
void MakeTargetListFor_SubGambitMenu(struct Unit * unit, u8 range);
void MakeTargetListFor_AfterSelectAPressed(u8 x, u8 y, u8 GambitIndex);
void ForEachUnitInRange_ByRangeMovement(void(*)(struct Unit * unit));

// void _TargetSelect_Gambit1_Constructor(struct TargetSelectionProc*);
// void _TargetSelect_Gambit2_Destructor(struct TargetSelectionProc*);
// void _TargetSelect_Gambit4_TargetChange(struct TargetSelectionProc*, struct TargetEntry*);
// int _TargetSelect_Gambit5_APressed(struct TargetSelectionProc*, struct TargetEntry*);
// int _TargetSelect_Gambit6_BPressed(struct TargetSelectionProc*, struct TargetEntry*);
// int _TargetSelect_Gambit7_RPressed(struct TargetSelectionProc*, struct TargetEntry*);



// Effect Map
void FillRangeMapForHover(struct Unit * unit, u8 range);
void FillAOEEffectMay_OnChangeTarget(u8 x, u8 y, u8 GambitIndex);

typedef void (*Gambit_AOEMap)(u8 x, u8 y, u8 direction);
extern Gambit_AOEMap GambitEffectMap_DrawMapRoutineTable[];
void GambitResetMaps(void);





// Action
typedef struct ProcGamAction ProcGamAction;
struct ProcGamAction{
	/* 0x00 */ PROC_FIELDS;
	/* 0x29 */ s16 Counter;
	/* 0x2C */ u8* gpCurUnitID; 
};

extern u8 Gambit_ActionIndex;
extern const ProcCmd gpProc_GambitAction[];

int GambitAction(struct Proc*);
void cProc_AttackPriority(ProcGamAction* proc);
void cProc_AttackNext(ProcGamAction* proc);
void cProc_SetNextGambitTarget(ProcGamAction* proc);

void MakeGambitAnime(ProcGamAction* proc, struct Unit * TargetUnit);
void MakeGambitEffect(ProcGamAction* proc, struct Unit * TargetUnit);




// Expanded Save
extern u8* gpCommonFlagSaveSu;
void SaveTarget_PostGambitTargetSelection(void);
void ClearTarget_CommonFlagSaveSu(void);

void MsuSave_CommonFlag(void* dest, unsigned size);
void MsuLoad_CommonFlag(const void* src, unsigned size);