#include "../../GambitCore.h"
void MakeTargetListFor_SubGambitMenu(struct Unit* unit,u8 range){
	InitTargets(unit->xPos,unit->yPos);
	BmMapFill(gBmMapRange,0);
	MapAddInBoundedRange(unit->xPos,unit->yPos,0,range);
	ForEachUnitInRange(AddUnitToTargetListIfNotAllied);
	
	return;
}

void MakeTargetListFor_AfterSelectAPressed(u8 x, u8 y, u8 GambitIndex){
	InitTargets(x,y);
	gWorkingBmMap = gBmMapMovement;
	ForEachUnitInRange_ByRangeMovement(AddUnitToTargetListIfNotAllied);
	return;
}

void ForEachUnitInRange_ByRangeMovement(void(func)(struct Unit*)){
	if( (gBmMapSize.x < 0) | (gBmMapSize.y < 0) )
		return;
	
	for( u8 y=0; y<gBmMapSize.y; y++ )
		for( u8 x=0; x<gBmMapSize.x; x++ )
		{
			// if( -1 == gBmMapMovement[y][x] )
			// 	continue;
			if( gBmMapUnit[y][x] )
				func( GetUnit(gBmMapUnit[y][x]) );
		}
	return;
}

struct ProcCmd CONST_DATA sProcScr_MoveLimitViewChange[] =
{
    PROC_NAME("MLVCHC"),
    PROC_MARK(PROC_MARK_DISP),

    PROC_CALL(MoveLimitViewChange_OnInit),
    PROC_REPEAT(MoveLimitViewChange_OnLoop),

    PROC_END,
};

struct ProcCmd CONST_DATA sProcScr_MoveLimitView[] =
{
    PROC_NAME("E_MOVELIMITVIEW"),
    PROC_MARK(PROC_MARK_DISP),

    PROC_SET_END_CB(MoveLimitView_OnEnd),

    PROC_START_CHILD(sProcScr_MoveLimitViewChange),

    PROC_CALL(MoveLimitView_OnInit),
    PROC_REPEAT(MoveLimitView_OnLoop),

    PROC_END,
};


// Funcs
void _TargetSelect_Gambit4_TargetChange(struct TargetSelectionProc* proc, struct TargetEntry* entry){
		
	FillAOEEffectMay_OnChangeTarget(entry->x,entry->y,0);
	BmMapFill(gBmMapRange,0);
	FillRangeMapForHover(gActiveUnit,2);
		
	Proc* pproc = Proc_Find(sProcScr_MoveLimitView);
	if( pproc )
		*((u16*)pproc+0x4A/2) = 0x24; 	//Load Red&Green(need new-MapDisplay)
	DisplayMoveRangeGraphics(0x3);
	
	TargetSelect_ChangeTarget(proc,entry);
	return;
}


int _TargetSelect_Gambit5_APressed(struct TargetSelectionProc* proc, struct TargetEntry* entry){
	gActionData.unitActionType = Gambit_ActionIndex;
	gActionData.targetIndex = entry->unitIndex;
	
	Proc_EndEach(gProcScr_BKSEL);
	MakeTargetListFor_AfterSelectAPressed(entry->x,entry->y,0);
	
	SaveTarget_PostGambitTargetSelection();
	return TSE_CLEAR_GFX | TSE_PLAY_BEEP | TSE_END | TSE_DISABLE;
}


int _TargetSelect_Gambit6_BPressed(struct TargetSelectionProc* proc, struct TargetEntry* entry){
	ResetIconGraphics();
	LoadIconPalettes(0x4);
	StartMenu(GambitSelectMenu, (ProcPtr)proc);
	return TSE_CLEAR_GFX | TSE_PLAY_BEEP | TSE_END | TSE_DISABLE;
}

