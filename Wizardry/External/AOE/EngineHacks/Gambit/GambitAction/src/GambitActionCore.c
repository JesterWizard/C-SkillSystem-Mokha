#include "../../GambitCore.h"
static inline u8 GetGambitDmgReal(struct Unit* TargetUnit, u8 MaxDmg);

int GambitAction(Proc* proc){
	ProcGamAction* newProc = Proc_StartBlocking(gpProc_GambitAction, proc);
	newProc->Counter = gpCommonFlagSaveSu[0];
	newProc->gpCurUnitID = &gpCommonFlagSaveSu[1];
	return 0;
}


void cProc_AttackPriority(ProcGamAction* proc){
	struct Unit * TargetUnit = GetUnit(gActionData.targetIndex);
	MakeGambitAnime(proc,TargetUnit);
	MakeGambitEffect(proc,TargetUnit);

	if( 1 == proc->Counter )
		Proc_Goto((Proc*)proc, 0x2);
	return;
}


void cProc_AttackNext(ProcGamAction* proc){	
	u8 TargetID= *(proc->gpCurUnitID);
	
	if( TargetID != gActionData.targetIndex )
	{
		struct Unit * TargetUnit = GetUnit(TargetID);
		MakeGambitAnime(proc,TargetUnit);
		MakeGambitEffect(proc,TargetUnit);
	}	
	return;
}


void cProc_SetNextGambitTarget(ProcGamAction* proc){
	u8 iCounter = --proc->Counter;
	if(0==iCounter)
		Proc_Goto((Proc*)proc, 0x2);
	
	proc->gpCurUnitID++;
	return;
}





void MakeGambitAnime(ProcGamAction* proc, struct Unit* TargetUnit){
	
	struct Unit* SubjectUnit = GetUnit(gActionData.subjectIndex);
	
	//配置BattleStruct
	SetupSubjectBattleUnitForStaff(SubjectUnit, gActionData.itemSlotIndex);
	SetupTargetBattleUnitForStaff(TargetUnit);
	
	//GambitEffect
	u8 u8Dmg = GetGambitDmgReal(TargetUnit,0xA);
	// TargetUnit->curHP -= u8Dmg;
	
	//配置动画DIY内容
	*(0x3+(u8*)gpCurrentRound) = u8Dmg;
	gBattleActor.weaponBefore = 0x074;
	
	//避免 BattleApplyItemEffect->BattleApplyItemExpGains 加经验
	gBattleActor.weaponAttributes = 0x0;
	gBattleActor.weaponType= 0x0;
	
	//开启动画
	BattleApplyItemEffect((Proc*)proc);
	BeginBattleAnimations();
	return;
}



void MakeGambitEffect(ProcGamAction* proc, struct Unit* TargetUnit){
	u8 u8Dmg = GetGambitDmgReal(TargetUnit,0xA);
	TargetUnit->curHP -= u8Dmg;
	return;
}


static inline u8 GetGambitDmgReal(struct Unit* TargetUnit, u8 MaxDmg){
	u8 UnitCurHp=GetUnitCurrentHp(TargetUnit);
	if( UnitCurHp > MaxDmg)
		return MaxDmg;
	else
		return (UnitCurHp-1);
}