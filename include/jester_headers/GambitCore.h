// #include "../../Tools/FE-CLib-Mokha/include/gbafe.h"
#include "../common-chax.h"

typedef struct MenuDef MenuDef;
typedef struct MenuCommandDefinition MenuCommandDefinition;

typedef struct MenuProc MenuProc;
typedef struct MenuCommandProc MenuCommandProc;

struct MenuCommandDefinition {
	/* 00 */ const char* rawName;

	/* 04 */ u16 nameId, helpId;
	/* 08 */ u8 colorId, _u09;

	/* 0C */ int(*isAvailable)(const MenuCommandDefinition*, int);

	/* 10 */ void(*onDraw)(MenuProc*, MenuCommandProc*);

	/* 14 */ int(*onEffect)(MenuProc*, MenuCommandProc*);
	/* 18 */ int(*onIdle)(MenuProc*, MenuCommandProc*);

	/* 1C */ void(*onSwitchIn)(MenuProc*, MenuCommandProc*);
	/* 20 */ void(*onSwitchOut)(MenuProc*, MenuCommandProc*);
};

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


struct TargetSelectionProc;

typedef struct TargetEntry TargetEntry;
typedef struct TargetSelectionDefinition TargetSelectionDefinition;
typedef struct TargetSelectionProc TargetSelectionProc;

struct TargetEntry {
	/* 00 */ u8 x, y;
	/* 02 */ u8 unitIndex;
	/* 03 */ u8 trapIndex;

	/* 04 */ TargetEntry* next;
	/* 08 */ TargetEntry* prev;
};

struct TargetSelectionDefinition {
	/* 00 */ void(*onInit)(struct TargetSelectionProc*);
	/* 04 */ void(*onEnd)(struct TargetSelectionProc*);

	/* 08 */ void(*onInitTarget)(struct TargetSelectionProc*, struct TargetEntry*);

	/* 0C */ void(*onSwitchIn)(struct TargetSelectionProc*, struct TargetEntry*);
	/* 10 */ void(*onSwitchOut)(struct TargetSelectionProc*, struct TargetEntry*);

	/* 14 */ int(*onAPress)(struct TargetSelectionProc*, struct TargetEntry*);
	/* 18 */ int(*onBPress)(struct TargetSelectionProc*, struct TargetEntry*);
	/* 1C */ int(*onRPress)(struct TargetSelectionProc*, struct TargetEntry*);
};

struct TargetSelectionProc {
	PROC_HEADER;

	/* 2C */ const TargetSelectionDefinition* pDefinition;
	/* 30 */ TargetEntry* pCurrentEntry;
	
	/* 34 */ u8 stateBits;

	/* 38 */ int(*onAPressOverride)(TargetSelectionProc*, TargetEntry*);
};

enum _TargetSelectionEffect {
	TSE_NONE = 0x00,

	TSE_DISABLE = 0x01, // (for one frame, probably useful for blocking)
	TSE_END = 0x02,
	TSE_PLAY_BEEP = 0x04,
	TSE_PLAY_BOOP = 0x08,
	TSE_CLEAR_GFX = 0x10,
	TSE_END_FACE0 = 0x20
};


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

typedef struct Proc Proc;
typedef struct Proc ProcState;

typedef struct ProcCmd ProcCode;
typedef struct ProcCmd ProcCmd;

typedef struct FMUProc FMUProc;
typedef bool (*ButtonFunc) (struct FMUProc*);

#define PROC_FIELDS \
	ProcCmd* codeStart; /* start of instructions */ \
	ProcCmd* codeNext; /* next instruction */ \
	void (*onEnd)(Proc*); /* called on proc end */ \
	void (*onCycle)(Proc*); /* called on proc cycle */ \
	const char* name; /* name (debug) */ \
	Proc* parent; /* parent proc */ \
	Proc* child; /* first child proc */ \
	Proc* previous; /* previous proc (younger sibling) */ \
	Proc* next; /* next proc (older sibling) */ \
	u16 sleepTime; /* time left to sleep */ \
	u8 mark; /* mark */ \
	u8 statebits; /* state */ \
	u8 lockCount; /* lock count*/


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

