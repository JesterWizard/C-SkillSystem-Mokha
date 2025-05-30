.thumb

@ This file contains any LTF hacks that doesn't involve simply replacing pointers to functions
@ Aka complex hooks; Aka the hard part

.global LTFHook_InDisableBattleMapGfx

LTFHook_InDisableBattleMapGfx:
	@ Hook with jumpToHack at FE8U:08030198

	@ known state:
	@   r0 = 0
	@   r1 = gGameState

	ldr  r1, =gPaletteBuffer
	strh r0, [r1] @ gPaletteBuffer[0] = 0;

	@ implicit @ arg r0 = func ptr

	@ ldr r3, =SetSecondaryHBlankCallback
	ldr r3, =SetSecondaryHBlankHandler
	bl  BXR3

	ldr r3, =EnablePaletteSync
	bl  BXR3

	mov r0, #1 @ arg r0 = Mark

	@ ldr r3, =ProcHaltEachMarked
	ldr r3, =Proc_BlockEachMarked
	bl  BXR3

	ldr r3, =LTF_DisablePredictionDisplay
	bl  BXR3

	pop {r3}
BXR3:
	bx r3

.global LTFHook_InRestoreBattleMapGfx

LTFHook_InRestoreBattleMapGfx:
	@ Hook with jumpToHack at FE8U:080301D0

	mov r0, #1

	@ ldr r3, =ProcResumeEachMarked
	ldr r3, =Proc_UnblockEachMarked	
	bl  BXR3

	@ ldr r0, =gProc_VBlankHandler
	ldr r0, =sProc_BMVSync

	@ ldr r3, =ProcFind
	ldr r3, =Proc_Find
	bl  BXR3

	cmp r0, #0
	beq LTFHook_InRestoreBattleMapGfx.end

	@ ldr r3, =EndProc
	ldr r3, =Proc_End
	bl  BXR3

	@ ldr r3, =StartGameVBlankProc
	ldr r3, =BMapVSync_Start
	bl  BXR3

	ldr r3, =LTF_EnablePredictionDisplay
	bl  BXR3

LTFHook_InRestoreBattleMapGfx.end:
	pop {r0}
	bx  r0
