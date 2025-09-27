#include "include/gbafe.h"

extern const MenuDefinition GambitSelectMenu[];


// Menu
int Gambit_UpperMenu_Usability(const MenuCommandDefinition*, int);
int Gambit_UpperMenu_Effect(MenuProc*, MenuCommandProc*);
void Gambit_UpperMenu_Hover(MenuProc*, MenuCommandProc*);
void Gambit_UpperMenu_Unhover(MenuProc*, MenuCommandProc*);

int GambitSelectMenu_Usability(const MenuCommandDefinition*, int);
int GambitSelectMenu_Effect(MenuProc*, MenuCommandProc*);
void GambitSelectMenu_Hover(MenuProc*, MenuCommandProc*);
void GambitSelectMenu_Unhover(MenuProc*, MenuCommandProc*);




// Target Select
extern const struct TargetSelectionDefinition pGambit_TargetSelection_Func[];
void MakeTargetListFor_SubGambitMenu(Unit* unit,u8 range);
void MakeTargetListFor_AfterSelectAPressed(u8 x, u8 y, u8 GambitIndex);
void ForEachUnitInRange_ByRangeMovement(void(*)(struct Unit*));

void _TargetSelect_Gambit1_Constructor(struct TargetSelectionProc*);
void _TargetSelect_Gambit2_Destructor(struct TargetSelectionProc*);
void _TargetSelect_Gambit4_TargetChange(struct TargetSelectionProc*, struct TargetEntry*);
int _TargetSelect_Gambit5_APressed(struct TargetSelectionProc*, struct TargetEntry*);
int _TargetSelect_Gambit6_BPressed(struct TargetSelectionProc*, struct TargetEntry*);
int _TargetSelect_Gambit7_RPressed(struct TargetSelectionProc*, struct TargetEntry*);



// Effect Map
void FillRangeMapForHover(Unit* unit,u8 range);
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
extern const ProcInstruction gpProc_GambitAction[];

int GambitAction(struct Proc*);
void cProc_AttackPriority(ProcGamAction* proc);
void cProc_AttackNext(ProcGamAction* proc);
void cProc_SetNextGambitTarget(ProcGamAction* proc);

void MakeGambitAnime(ProcGamAction* proc, Unit* TargetUnit);
void MakeGambitEffect(ProcGamAction* proc, Unit* TargetUnit);




// Expanded Save
extern u8* gpCommonFlagSaveSu;
void SaveTarget_PostGambitTargetSelection(void);
void ClearTarget_CommonFlagSaveSu(void);

void MsuSave_CommonFlag(void* dest, unsigned size);
void MsuLoad_CommonFlag(const void* src, unsigned size);