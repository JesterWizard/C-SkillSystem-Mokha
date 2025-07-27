#include "jester_headers/GambitCore.h"

extern struct ProcCmd CONST_DATA sProcScr_MoveLimitView[];

u8 Gambit_UpperMenu_Usability(const struct MenuItemDef * def, int param){
	if( gActiveUnit->state & 0x40 )
		return 3;
	MakeTargetListFor_SubGambitMenu(gActiveUnit,0x2);
	if( 0 == gTargetArraySize )
		return 3;
	return 1;
}


u8 Gambit_UpperMenu_Effect(struct MenuProc* procMU, struct MenuItemProc* def){
	ResetIconGraphics();
	LoadIconPalettes(0x4);
	StartMenu(GambitSelectMenu, (ProcPtr)def);
	return ME_CLEAR_GFX | ME_PLAY_BEEP | ME_END | ME_DISABLE;
}


int Gambit_UpperMenu_Hover(struct MenuProc* procMU, struct MenuItemProc* def){
	BmMapFill(gBmMapMovement,-1);
	BmMapFill(gBmMapRange,0);
	FillRangeMapForHover(gActiveUnit,2);
	
	Proc* proc = Proc_Find(sProcScr_MoveLimitView);
	if( proc )
		*((u16*)proc+0x4A/2) = 0x24; 	//Load Red&Green(need new-MapDisplay)

	DisplayMoveRangeGraphics(0x24);
	return 0;
	
}


int Gambit_UpperMenu_Unhover(struct MenuProc* procMU, struct MenuItemProc* def){
	BmMapFill(gBmMapMovement,-1);
	BmMapFill(gBmMapRange,0);
	
	DisplayMoveRangeGraphics(0x3);
	//HideMoveRangeGraphicsWrapper();
	return 0;
}








// Sub Menu
int GambitSelectMenu_Usability(const MenuCommandDefinition* def, int param){
	return 1;
}


int GambitSelectMenu_Effect(struct MenuProc* procMU, struct MenuItemProc* procCMD){
	gActionData.itemSlotIndex = 0;
	ClearBg0Bg1();
	MakeTargetListFor_SubGambitMenu(gActiveUnit,0x2);
	StartTargetSelection(pGambit_TargetSelection_Func);
	return ME_END_FACE0 | ME_PLAY_BEEP | ME_END | ME_DISABLE;;
}


int GambitSelectMenu_Hover(struct MenuProc* procMU, struct MenuItemProc* procCMD){
	BmMapFill(gBmMapMovement,-1);
	BmMapFill(gBmMapRange,0);
	FillRangeMapForHover(gActiveUnit,2);
	
	Proc* proc = Proc_Find(sProcScr_MoveLimitView);
	if( proc )
		*((u16*)proc+0x4A/2) = 0x24; 	//Load Red&Green(need new-MapDisplay)

	DisplayMoveRangeGraphics(0x24);
	return 0;
}


int GambitSelectMenu_Unhover(struct MenuProc* procMU, struct MenuItemProc* procCMD){
	BmMapFill(gBmMapRange,0);
	FillRangeMapForHover(gActiveUnit,2);
	
	Proc* proc = Proc_Find(sProcScr_MoveLimitView);
	if( proc )
		*((u16*)proc+0x4A/2) = 0x24; 	//Load Red&Green(need new-MapDisplay)
	
	DisplayMoveRangeGraphics(0x3);
	
	if( !(0x4 & procMU->state) )
		HideMoveRangeGraphicsWrapper(procMU, procCMD);
	return 0;
}



