#include "../../GambitCore.h"

void FillRangeMapForHover(struct Unit * unit, u8 range){
	MapAddInRange(unit->xPos, unit->yPos, range, 1);
	MapAddInRange(unit->xPos, unit->yPos, 0, -1);
	return;
}


void FillAOEEffectMay_OnChangeTarget(u8 x, u8 y, u8 GambitIndex){
	u8 uDirec = GetFacingDirection(gActiveUnit->xPos, gActiveUnit->yPos, x, y);
	GambitEffectMap_DrawMapRoutineTable[0](x,y,uDirec);
	return;
}


void GambitResetMaps(void){
	BmMapFill(gBmMapMovement,-1);
	BmMapFill(gBmMapOther,-1);
	BmMapFill(gBmMapRange,0);
	gWorkingBmMap = gBmMapMovement;
	DisplayMoveRangeGraphics(0x3);
	// HideMoveRangeGraphicsWrapper();
	return;
}