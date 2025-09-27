	.cpu arm7tdmi
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 4
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"FreeMU_Extra.c"
	.text
	.align	1
	.global	FMU_CanUnitBeOnPos
	.arch armv4t
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	FMU_CanUnitBeOnPos, %function
FMU_CanUnitBeOnPos:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	movs	r4, r0
	movs	r0, #0
	cmp	r1, r0
	blt	.L2
	cmp	r2, r0
	blt	.L2
	ldr	r3, .L9
	ldrh	r5, [r3]
	cmp	r1, r5
	bge	.L2
	ldrh	r3, [r3, #2]
	cmp	r2, r3
	bge	.L2
	ldr	r3, .L9+4
	ldr	r3, [r3]
	lsls	r2, r2, #2
	ldr	r3, [r3, r2]
	ldrb	r3, [r3, r1]
	cmp	r3, r0
	bne	.L2
	ldr	r0, .L9+8
	ldr	r0, [r0]
	ldr	r0, [r0, r2]
	ldrb	r5, [r0, r1]
	movs	r0, r3
	lsls	r3, r5, #31
	bmi	.L2
	ldr	r3, .L9+12
	ldr	r3, [r3]
	ldr	r3, [r3, r2]
	movs	r0, r4
	ldrb	r1, [r3, r1]
	ldr	r3, .L9+16
	bl	.L11
	subs	r3, r0, #1
	sbcs	r0, r0, r3
.L2:
	@ sp needed
	pop	{r4, r5, r6}
	pop	{r1}
	bx	r1
.L10:
	.align	2
.L9:
	.word	gMapSize
	.word	gMapUnit
	.word	gMapHidden
	.word	gMapTerrain
	.word	CanUnitCrossTerrain
	.size	FMU_CanUnitBeOnPos, .-FMU_CanUnitBeOnPos
	.align	1
	.global	EnableFreeMovementASMC
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	EnableFreeMovementASMC, %function
EnableFreeMovementASMC:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L13
	@ sp needed
	ldr	r2, [r3]
	movs	r3, #1
	ldrb	r1, [r2]
	orrs	r3, r1
	strb	r3, [r2]
	bx	lr
.L14:
	.align	2
.L13:
	.word	FreeMoveFlag
	.size	EnableFreeMovementASMC, .-EnableFreeMovementASMC
	.align	1
	.global	DisableFreeMovementASMC
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	DisableFreeMovementASMC, %function
DisableFreeMovementASMC:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L16
	@ sp needed
	ldr	r2, [r3]
	ldrb	r3, [r2]
	lsrs	r3, r3, #1
	lsls	r3, r3, #1
	strb	r3, [r2]
	bx	lr
.L17:
	.align	2
.L16:
	.word	FreeMoveFlag
	.size	DisableFreeMovementASMC, .-DisableFreeMovementASMC
	.align	1
	.global	GetFreeMovementState
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	GetFreeMovementState, %function
GetFreeMovementState:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, lr}
	movs	r0, #1
	ldr	r3, .L21
	ldr	r3, [r3]
	str	r3, [sp, #4]
	ldrb	r3, [r3]
	tst	r3, r0
	bne	.L19
	ldr	r3, .L21+4
	ldrb	r0, [r3, #14]
	ldr	r3, .L21+8
	bl	.L11
	subs	r3, r0, #1
	sbcs	r0, r0, r3
.L19:
	add	sp, sp, #12
	@ sp needed
	pop	{r1}
	bx	r1
.L22:
	.align	2
.L21:
	.word	FreeMoveFlag
	.word	gChapterData
	.word	WLC_isBaseChapter
	.size	GetFreeMovementState, .-GetFreeMovementState
	.align	1
	.global	End6CInternal_FreeMU
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	End6CInternal_FreeMU, %function
End6CInternal_FreeMU:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	movs	r4, r0
	@ sp needed
	bl	DisableFreeMovementASMC
	movs	r1, #15
	movs	r0, r4
	ldr	r3, .L24
	bl	.L11
	pop	{r4}
	pop	{r0}
	bx	r0
.L25:
	.align	2
.L24:
	.word	ProcGoto
	.size	End6CInternal_FreeMU, .-End6CInternal_FreeMU
	.align	1
	.global	ChangeControlledUnitASMC
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	ChangeControlledUnitASMC, %function
ChangeControlledUnitASMC:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L27
	push	{r4, lr}
	movs	r4, r0
	@ sp needed
	ldr	r0, [r3, #4]
	lsls	r0, r0, #16
	ldr	r3, .L27+4
	lsrs	r0, r0, #16
	bl	.L11
	movs	r2, #17
	movs	r1, #16
	str	r0, [r4, #48]
	ldr	r3, .L27+8
	ldrsb	r2, [r0, r2]
	ldrsb	r1, [r0, r1]
	movs	r0, #0
	bl	.L11
	pop	{r4}
	pop	{r0}
	bx	r0
.L28:
	.align	2
.L27:
	.word	gEventSlot
	.word	GetUnitStructFromEventParameter
	.word	EnsureCameraOntoPosition
	.size	ChangeControlledUnitASMC, .-ChangeControlledUnitASMC
	.align	1
	.global	NewPlayerPhaseEvaluationFunc
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	NewPlayerPhaseEvaluationFunc, %function
NewPlayerPhaseEvaluationFunc:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	movs	r4, r0
	bl	GetFreeMovementState
	movs	r1, r4
	ldr	r3, .L32
	cmp	r0, #0
	beq	.L30
	ldr	r0, .L32+4
	bl	.L11
.L31:
	@ sp needed
	movs	r0, r4
	ldr	r3, .L32+8
	bl	.L11
	pop	{r4}
	pop	{r0}
	bx	r0
.L30:
	ldr	r0, .L32+12
	bl	.L11
	movs	r1, #7
	ldr	r3, .L32+16
	bl	.L11
	b	.L31
.L33:
	.align	2
.L32:
	.word	ProcStartBlocking
	.word	FreeMovementControlProc
	.word	BreakProcLoop
	.word	gProc_PlayerPhase
	.word	ProcGoto
	.size	NewPlayerPhaseEvaluationFunc, .-NewPlayerPhaseEvaluationFunc
	.align	1
	.global	NewMakePhaseControllerFunc
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	NewMakePhaseControllerFunc, %function
NewMakePhaseControllerFunc:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	movs	r4, r0
	bl	GetFreeMovementState
	ldr	r3, .L39
	cmp	r0, #0
	bne	.L35
	ldr	r3, .L39+4
	ldrb	r2, [r3, #15]
	ldr	r3, .L39+8
	cmp	r2, #0
	beq	.L35
	ldr	r3, .L39+12
.L35:
	@ sp needed
	movs	r1, r4
	movs	r0, r3
	ldr	r3, .L39+16
	bl	.L11
	movs	r0, r4
	ldr	r3, .L39+20
	bl	.L11
	pop	{r4}
	pop	{r0}
	bx	r0
.L40:
	.align	2
.L39:
	.word	FreeMovementControlProc
	.word	gChapterData
	.word	gProc_PlayerPhase
	.word	gProc_CpPhase
	.word	ProcStartBlocking
	.word	BreakProcLoop
	.size	NewMakePhaseControllerFunc, .-NewMakePhaseControllerFunc
	.align	1
	.global	pFMU_InitFMUnit
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	pFMU_InitFMUnit, %function
pFMU_InitFMUnit:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	ldr	r3, .L42
	@ sp needed
	movs	r4, r0
	ldrb	r0, [r3, #18]
	ldr	r3, .L42+4
	bl	.L11
	movs	r3, #2
	str	r0, [r4, #48]
	adds	r4, r4, #52
	strb	r3, [r4]
	pop	{r4}
	pop	{r0}
	bx	r0
.L43:
	.align	2
.L42:
	.word	gGMData
	.word	GetUnitByCharId
	.size	pFMU_InitFMUnit, .-pFMU_InitFMUnit
	.align	1
	.global	pFMU_OnInit
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	pFMU_OnInit, %function
pFMU_OnInit:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0, #48]
	cmp	r3, #0
	bne	.L45
	ldr	r3, .L48
	str	r3, [r0, #48]
.L45:
	ldr	r3, [r0, #48]
	lsls	r3, r3, #14
	bmi	.L46
	ldr	r3, .L48
	str	r3, [r0, #48]
.L46:
	ldr	r3, [r0, #48]
	lsls	r3, r3, #6
	bmi	.L47
	ldr	r3, .L48
	str	r3, [r0, #48]
.L47:
	@ sp needed
	ldr	r2, [r0, #48]
	ldr	r3, .L48+4
	str	r2, [r3]
	bx	lr
.L49:
	.align	2
.L48:
	.word	gUnitArrayBlue
	.word	gActiveUnit
	.size	pFMU_OnInit, .-pFMU_OnInit
	.align	1
	.global	pFMU_InitTimer
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	pFMU_InitTimer, %function
pFMU_InitTimer:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r3, #0
	@ sp needed
	adds	r0, r0, #41
	strb	r3, [r0]
	bx	lr
	.size	pFMU_InitTimer, .-pFMU_InitTimer
	.align	1
	.global	pFMU_CorrectCameraPosition
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	pFMU_CorrectCameraPosition, %function
pFMU_CorrectCameraPosition:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	movs	r2, #17
	movs	r1, #16
	push	{r4, lr}
	ldr	r3, .L52
	@ sp needed
	ldr	r3, [r3]
	ldrsb	r2, [r3, r2]
	ldrsb	r1, [r3, r1]
	ldr	r3, .L52+4
	bl	.L11
	pop	{r4}
	pop	{r0}
	bx	r0
.L53:
	.align	2
.L52:
	.word	gActiveUnit
	.word	EnsureCameraOntoPosition
	.size	pFMU_CorrectCameraPosition, .-pFMU_CorrectCameraPosition
	.align	1
	.global	FMU_ChkKeyForMUExtra
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	FMU_ChkKeyForMUExtra, %function
FMU_ChkKeyForMUExtra:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L61
	ldrh	r3, [r3, #4]
	movs	r2, #16
	movs	r1, r3
	movs	r0, #1
	ands	r1, r2
	tst	r3, r2
	bne	.L55
	movs	r0, r1
	lsls	r1, r3, #26
	bmi	.L55
	movs	r0, #3
	lsls	r1, r3, #25
	bmi	.L55
	movs	r0, r2
	lsls	r3, r3, #24
	bpl	.L55
	subs	r0, r0, #14
.L55:
	@ sp needed
	bx	lr
.L62:
	.align	2
.L61:
	.word	gKeyState
	.size	FMU_ChkKeyForMUExtra, .-FMU_ChkKeyForMUExtra
	.ident	"GCC: (devkitARM release 55) 10.2.0"
	.code 16
	.align	1
.L11:
	bx	r3
