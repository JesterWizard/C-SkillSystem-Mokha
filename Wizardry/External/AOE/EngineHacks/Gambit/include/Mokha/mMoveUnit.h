#ifndef HEARDERMOKHA_MU
#define HEARDERMOKHA_MU

extern u8* gUnitMoveBuffer;	// 0x2033EFC

//extern u8 MuCtr_StartDefinedMove() //0x8079D75
extern void MuCtr_StartMoveTowards(struct Unit *, s8, s8, u8, u16);
//extern u8 MuCtr_InitDefinedMove() //0x8079E79
//extern u8 MuCtr_Exists() //0x8079F85


#endif // HEARDERMOKHA_MU