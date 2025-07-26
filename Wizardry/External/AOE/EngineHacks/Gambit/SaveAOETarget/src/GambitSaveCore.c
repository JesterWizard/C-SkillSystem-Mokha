#include "../../GambitCore.h"

void SaveTarget_PostGambitTargetSelection(void){
	s8 size = (s8)gTargetArraySize;
	if( size>0x40 )
		size = 0x40;
	gpCommonFlagSaveSu[0] = size;
	
	for(u8 i=0; i<size; i++)
		gpCommonFlagSaveSu[i+1] = gTargetArray[i].unitIndex;
}


void ClearTarget_CommonFlagSaveSu(void){
	for(u8 i=0; i<0x41; i++)
		gpCommonFlagSaveSu[i] = 0;
}


void MsuSave_CommonFlag(void* dest, unsigned size){
	WriteAndVerifySramFast( (const u8*)gpCommonFlagSaveSu, dest, size);
	return;
}


void MsuLoad_CommonFlag(const void* src, unsigned size){
	(*ReadSramFast)(src, gpCommonFlagSaveSu, size);
	return;
}
