#ifndef FREE_MOVEMENT_MOKHA
#define FREE_MOVEMENT_MOKHA

// #include "common-chax"

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

struct FMUProc {
	PROC_FIELDS;
	/* 29 */	u8 uTimer;
	/* 2A */	u16 Free;
	/* 2C */	s8 xCur;
	/* 2D */	s8 xTo;
	/* 2E */	s8 yCur;
	/* 2F */	s8 yTo;
	/* 30 */	struct Unit* FMUnit;
};

struct FMUTrapDef{
	u8 TrapID;
	ButtonFunc Func;
};
extern struct FMUTrapDef HookListFMU_TrapList_OnPressA[];
extern struct FMUTrapDef HookListFMU_TrapList_Auto[];


struct LocEventDef {
	u8 LocID;
	u8 TrapID;
};


extern struct LocEventDef HookListFMU_LocationBasedEvent[];
extern struct LocEventDef HookListFMU_LocationBasedEventDoor[];
extern ButtonFunc FMU_FunctionList_OnPressA[];
extern ButtonFunc FMU_FunctionList_OnPressB[];
extern ButtonFunc FMU_FunctionList_OnPressR[];
extern ButtonFunc FMU_FunctionList_OnPressL[];
extern ButtonFunc FMU_FunctionList_OnPressSelect[];
extern ButtonFunc FMU_FunctionList_OnPressStart[];

extern const u8 TimerDelay;
//#define FreeMoveFlag iFRAM[0]
extern u8* const FreeMoveFlag;

#define RunCharacterEvents ( (void(*)(u8,u8))(0x8083FB1) )
#define CheckForCharacterEvents ( (u8(*)(u8,u8))(0x8083F69) )
extern const ProcCode FreeMovementControlProc[];
extern const MenuDef FreeMovementLMenu;
extern bool RunMiscBasedEvents(u8,u8);


/*------------- External --------------*/
bool FMU_CanUnitBeOnPos(struct Unit * unit, s8, s8);
void EnableFreeMovementASMC(void);
void DisableFreeMovementASMC(void);
u8 GetFreeMovementState(void);
void End6CInternal_FreeMU(FMUProc* proc);
void ChangeControlledUnitASMC(struct FMUProc*);
void NewPlayerPhaseEvaluationFunc(struct Proc*);
void NewMakePhaseControllerFunc(struct Proc*);
void pFMU_OnInit(struct FMUProc*);
void pFMU_InitTimer(struct FMUProc*);
void pFMU_CorrectCameraPosition(struct FMUProc*);
u8 FMU_ChkKeyForMUExtra(void);


/*------------- Core --------------*/
void pFMU_MainLoop(struct FMUProc*);
void pFMU_HanleContinueMove(struct FMUProc*);
void pFMU_MoveUnit(struct FMUProc*);
void pFMU_HandleKeyMisc(struct FMUProc*);
void pFMU_HandleSave(struct FMUProc*);
void pFMU_PressA(struct FMUProc*);
void pFMU_PressB(struct FMUProc*);
void pFMU_PressL(struct FMUProc*);
void pFMU_PressR(struct FMUProc*);
void pFMU_PressSelect(struct FMUProc*);
void pFMU_PressStart(struct FMUProc*);


/*------------- Events --------------*/
void pFMU_RunMiscBasedEvents(struct FMUProc*);
void pFMU_RunLocBasedAsmcAuto(struct FMUProc*);
bool FMUmisc_RunMapEvents(struct FMUProc*);
bool FMUmisc_RunTalkEvents(struct FMUProc*);
bool FMU_RunTrapASMC(FMUProc*);
bool FMU_RunTrapASMC_Auto(FMUProc*);

/*------------- KeyPress --------------*/
bool FMU_OnButton_StartMenu(FMUProc*);
bool FMU_OnButton_EndFreeMove(FMUProc*);
bool FMU_OnButton_ChangeUnit(FMUProc*);

#endif //FREE_MOVEMENT_MOKHA