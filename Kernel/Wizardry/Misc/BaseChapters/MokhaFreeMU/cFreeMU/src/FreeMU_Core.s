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
	.file	"FreeMU_Core.c"
	.text
	.align	1
	.global	pFMU_MainLoop
	.arch armv4t
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	pFMU_MainLoop, %function
pFMU_MainLoop:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	ldr	r3, .L3
	movs	r4, r0
	bl	.L5
	cmp	r0, #0
	bne	.L1
	movs	r1, #1
	movs	r0, r4
	ldr	r3, .L3+4
	bl	.L5
.L1:
	@ sp needed
	pop	{r4}
	pop	{r0}
	bx	r0
.L4:
	.align	2
.L3:
	.word	MU_Exists
	.word	ProcGoto
	.size	pFMU_MainLoop, .-pFMU_MainLoop
	.align	1
	.global	pFMU_HanleContinueMove
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	pFMU_HanleContinueMove, %function
pFMU_HanleContinueMove:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	@ sp needed
	bx	lr
	.size	pFMU_HanleContinueMove, .-pFMU_HanleContinueMove
	.global	__aeabi_idivmod
	.align	1
	.global	pFMU_UpdateSMS
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	pFMU_UpdateSMS, %function
pFMU_UpdateSMS:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	movs	r4, #32
	ldr	r3, [r0, #48]
	ldr	r2, [r3, #60]
	ldrh	r1, [r2, #8]
	lsls	r1, r1, #22
	lsrs	r1, r1, #22
	sub	sp, sp, #20
	subs	r1, r1, #128
	ldr	r3, [r3, #4]
	str	r1, [sp]
	ldrb	r7, [r3, #6]
	ldr	r3, .L21
	lsls	r2, r7, #3
	adds	r3, r3, r2
	ldrh	r6, [r3, #2]
	movs	r5, r0
	str	r4, [sp, #4]
	cmp	r6, #1
	bhi	.L8
	cmp	r6, #0
	bne	.L15
	subs	r4, r4, #16
	str	r4, [sp, #4]
.L8:
	ldr	r3, .L21+4
	bl	.L5
	ldr	r3, .L21+8
	movs	r1, #72
	bl	.L5
	ldr	r3, .L21+12
	lsls	r2, r7, #2
	ldr	r0, [r2, r3]
	str	r1, [sp, #8]
	cmp	r0, #0
	beq	.L7
	adds	r5, r5, #52
	ldrb	r2, [r5]
	ldr	r1, .L21+16
	ldr	r3, .L21+20
	cmp	r2, #2
	bne	.L10
	ldr	r2, .L21
	lsls	r7, r7, #3
	adds	r7, r2, r7
	ldr	r0, [r7, #4]
	bl	.L5
	movs	r0, #0
.L11:
	adds	r3, r6, #7
	lsls	r0, r0, r3
	movs	r3, r0
	movs	r0, #3
	muls	r0, r3
	movs	r3, #128
	lsls	r6, r6, #2
	lsls	r3, r3, r6
	adds	r6, r0, r3
	adds	r3, r3, r6
	str	r3, [sp, #12]
	ldr	r3, [sp]
	lsls	r3, r3, #5
	str	r3, [sp]
	ldr	r3, [sp, #4]
	ldr	r2, .L21+24
	lsrs	r5, r3, #3
	ldr	r3, [sp]
	adds	r1, r3, r2
	ldr	r3, .L21+16
	lsrs	r4, r4, #3
	ldr	r7, .L21+28
	adds	r0, r0, r3
	movs	r2, r4
	movs	r3, r5
	bl	.L23
	ldr	r3, [sp]
	ldr	r2, .L21+32
	adds	r1, r3, r2
	ldr	r3, .L21+16
	movs	r2, r4
	adds	r0, r6, r3
	movs	r3, r5
	bl	.L23
	ldr	r2, .L21+16
	mov	ip, r2
	ldr	r3, [sp]
	ldr	r6, .L21+36
	adds	r1, r3, r6
	ldr	r3, [sp, #12]
	add	r3, r3, ip
	movs	r0, r3
	movs	r2, r4
	movs	r3, r5
	bl	.L23
	ldr	r2, [sp, #8]
	ldr	r3, .L21+40
	cmp	r2, #30
	bgt	.L12
	movs	r2, #128
	ldr	r1, .L21+44
	ldr	r0, .L21+24
	lsls	r2, r2, #6
.L20:
	bl	.L5
.L7:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7}
	pop	{r0}
	bx	r0
.L15:
	movs	r3, #32
	movs	r4, #16
	str	r3, [sp, #4]
	b	.L8
.L10:
	bl	.L5
	ldrb	r3, [r5]
	movs	r0, #2
	cmp	r3, #3
	beq	.L11
	movs	r0, r3
	b	.L11
.L12:
	movs	r2, #128
	ldr	r1, [sp, #8]
	subs	r1, r1, #35
	lsls	r2, r2, #6
	cmp	r1, #31
	bls	.L13
	ldr	r1, .L21+44
	ldr	r0, .L21+32
	b	.L20
.L13:
	movs	r0, r6
	ldr	r1, .L21+44
	b	.L20
.L22:
	.align	2
.L21:
	.word	gStandingMapSpriteData
	.word	GetGameClock
	.word	__aeabi_idivmod
	.word	FMU_idleSMSGfxTable
	.word	gGenericBuffer
	.word	Decompress
	.word	gSMSGfxBuffer_Frame1
	.word	CopyTileGfxForObj
	.word	gSMSGfxBuffer_Frame2
	.word	gSMSGfxBuffer_Frame3
	.word	RegisterTileGraphics
	.word	100732928
	.size	pFMU_UpdateSMS, .-pFMU_UpdateSMS
	.align	1
	.global	pFMUCtr_OnEnd
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	pFMUCtr_OnEnd, %function
pFMUCtr_OnEnd:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	ldr	r3, .L31
	movs	r5, r0
	ldr	r0, .L31+4
	bl	.L5
	ldr	r3, .L31+8
	movs	r4, r0
	movs	r0, r5
	bl	.L5
	cmp	r4, #0
	beq	.L24
	ldr	r3, .L31+12
	bl	.L5
	cmp	r0, #0
	beq	.L24
	movs	r0, r4
	bl	pFMU_UpdateSMS
.L24:
	@ sp needed
	pop	{r4, r5, r6}
	pop	{r0}
	bx	r0
.L32:
	.align	2
.L31:
	.word	ProcFind
	.word	FreeMovementControlProc
	.word	MuCtr_OnEnd
	.word	GetFreeMovementState
	.size	pFMUCtr_OnEnd, .-pFMUCtr_OnEnd
	.align	1
	.global	pFMU_MoveUnit
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	pFMU_MoveUnit, %function
pFMU_MoveUnit:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	movs	r6, r0
	movs	r1, r6
	movs	r5, #16
	movs	r4, #17
	ldr	r7, .L62
	adds	r1, r1, #52
	ldr	r0, [r7]
	ldrb	r2, [r1]
	ldr	r3, .L62+4
	sub	sp, sp, #28
	ldrh	r3, [r3, #4]
	ldrsb	r5, [r0, r5]
	ldrsb	r4, [r0, r4]
	str	r2, [sp, #16]
	movs	r2, #240
	tst	r3, r2
	beq	.L34
	subs	r2, r2, #224
	mov	ip, r2
	ands	r2, r3
	str	r2, [sp, #12]
	mov	r2, ip
	tst	r3, r2
	beq	.L35
	movs	r3, #1
	adds	r5, r5, #1
	lsls	r5, r5, #24
	asrs	r5, r5, #24
.L61:
	strb	r3, [r1]
.L34:
	lsls	r3, r5, #24
	str	r3, [sp, #20]
	movs	r3, r4
	ands	r3, r5
	lsrs	r3, r3, #31
	str	r3, [sp, #12]
	ldr	r3, .L62+8
	ldrh	r2, [r3]
	movs	r3, #1
	cmp	r5, r2
	bgt	.L40
	movs	r3, #0
.L40:
	ldr	r2, [sp, #12]
	tst	r2, r3
	beq	.L47
	ldr	r3, .L62+8
	ldrh	r2, [r3, #2]
	movs	r3, #1
	cmp	r4, r2
	bgt	.L41
	movs	r3, #0
.L41:
	lsls	r3, r3, #24
	bne	.L38
.L47:
	movs	r3, r6
	adds	r3, r3, #45
	strb	r5, [r3]
	strb	r4, [r3, #2]
.L38:
	movs	r2, r4
	movs	r1, r5
	ldr	r3, .L62+12
	bl	.L5
	cmp	r0, #0
	beq	.L42
	ldr	r3, .L62+8
	ldrh	r1, [r3]
	movs	r2, #1
	cmp	r5, r1
	bgt	.L45
	movs	r2, #0
.L45:
	ldr	r1, [sp, #12]
	tst	r1, r2
	beq	.L48
	ldrh	r2, [r3, #2]
	movs	r3, #1
	cmp	r4, r2
	bgt	.L46
	movs	r3, #0
.L46:
	lsls	r3, r3, #24
	bne	.L43
.L48:
	ldr	r3, [sp, #20]
	lsrs	r1, r3, #24
	movs	r3, #0
	lsls	r2, r4, #24
	str	r3, [sp]
	ldr	r0, [r7]
	ldr	r4, .L62+16
	lsrs	r2, r2, #24
	adds	r3, r3, #16
	bl	.L64
.L43:
	movs	r1, #2
	movs	r0, r6
	ldr	r3, .L62+20
	bl	.L5
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7}
	pop	{r0}
	bx	r0
.L35:
	lsls	r2, r3, #26
	bpl	.L36
	subs	r5, r5, #1
	lsls	r5, r5, #24
	ldr	r3, [sp, #12]
	asrs	r5, r5, #24
	b	.L61
.L36:
	lsls	r2, r3, #25
	bpl	.L37
	subs	r4, r4, #1
	lsls	r4, r4, #24
	movs	r3, #3
	asrs	r4, r4, #24
	b	.L61
.L37:
	lsls	r3, r3, #24
	bpl	.L34
	adds	r4, r4, #1
	lsls	r4, r4, #24
	movs	r3, #2
	asrs	r4, r4, #24
	b	.L61
.L42:
	movs	r3, r6
	adds	r3, r3, #52
	ldrb	r3, [r3]
	ldr	r2, [sp, #16]
	cmp	r3, r2
	beq	.L43
	movs	r0, r6
	bl	pFMU_UpdateSMS
	b	.L43
.L63:
	.align	2
.L62:
	.word	gActiveUnit
	.word	gKeyState
	.word	gMapSize
	.word	FMU_CanUnitBeOnPos
	.word	MuCtr_StartMoveTowards
	.word	ProcGoto
	.size	pFMU_MoveUnit, .-pFMU_MoveUnit
	.align	1
	.global	pFMU_HandleKeyMisc
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	pFMU_HandleKeyMisc, %function
pFMU_HandleKeyMisc:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	ldr	r3, .L76
	ldrh	r3, [r3, #4]
	movs	r1, #4
	lsls	r2, r3, #31
	bmi	.L75
	movs	r1, #5
	lsls	r2, r3, #30
	bmi	.L75
	movs	r1, #6
	lsls	r2, r3, #22
	bmi	.L75
	movs	r1, #7
	lsls	r2, r3, #23
	bmi	.L75
	movs	r1, #8
	lsls	r2, r3, #29
	bmi	.L75
	lsls	r3, r3, #28
	bpl	.L65
	movs	r1, #9
.L75:
	ldr	r3, .L76+4
	bl	.L5
.L65:
	@ sp needed
	pop	{r4}
	pop	{r0}
	bx	r0
.L77:
	.align	2
.L76:
	.word	gKeyState
	.word	ProcGoto
	.size	pFMU_HandleKeyMisc, .-pFMU_HandleKeyMisc
	.align	1
	.global	pFMU_HandleSave
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	pFMU_HandleSave, %function
pFMU_HandleSave:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	movs	r4, r0
	adds	r4, r4, #41
	ldrb	r3, [r4]
	ldr	r2, .L80
	adds	r3, r3, #1
	lsls	r3, r3, #24
	ldrb	r2, [r2]
	lsrs	r3, r3, #24
	strb	r3, [r4]
	cmp	r2, r3
	bcs	.L78
	ldr	r3, .L80+4
	movs	r1, #14
	bl	.L5
	movs	r3, #0
	strb	r3, [r4]
.L78:
	@ sp needed
	pop	{r4}
	pop	{r0}
	bx	r0
.L81:
	.align	2
.L80:
	.word	TimerDelay
	.word	ProcGoto
	.size	pFMU_HandleSave, .-pFMU_HandleSave
	.align	1
	.global	pFMU_PressA
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	pFMU_PressA, %function
pFMU_PressA:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	ldr	r3, .L88
	ldrh	r3, [r3, #4]
	movs	r5, r0
	ldr	r4, .L88+4
	lsls	r3, r3, #31
	bmi	.L83
.L82:
	@ sp needed
	pop	{r4, r5, r6}
	pop	{r0}
	bx	r0
.L85:
	movs	r0, r5
	adds	r4, r4, #4
	bl	.L5
	cmp	r0, #0
	bne	.L82
.L83:
	ldr	r3, [r4]
	cmp	r3, #0
	bne	.L85
	b	.L82
.L89:
	.align	2
.L88:
	.word	gKeyState
	.word	FMU_FunctionList_OnPressA
	.size	pFMU_PressA, .-pFMU_PressA
	.align	1
	.global	pFMU_PressB
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	pFMU_PressB, %function
pFMU_PressB:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	ldr	r3, .L96
	ldrh	r3, [r3, #4]
	movs	r5, r0
	ldr	r4, .L96+4
	lsls	r3, r3, #30
	bmi	.L91
.L90:
	@ sp needed
	pop	{r4, r5, r6}
	pop	{r0}
	bx	r0
.L93:
	movs	r0, r5
	adds	r4, r4, #4
	bl	.L5
	cmp	r0, #0
	bne	.L90
.L91:
	ldr	r3, [r4]
	cmp	r3, #0
	bne	.L93
	b	.L90
.L97:
	.align	2
.L96:
	.word	gKeyState
	.word	FMU_FunctionList_OnPressB
	.size	pFMU_PressB, .-pFMU_PressB
	.align	1
	.global	pFMU_PressL
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	pFMU_PressL, %function
pFMU_PressL:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	ldr	r3, .L104
	ldrh	r3, [r3, #4]
	movs	r5, r0
	ldr	r4, .L104+4
	lsls	r3, r3, #22
	bmi	.L99
.L98:
	@ sp needed
	pop	{r4, r5, r6}
	pop	{r0}
	bx	r0
.L101:
	movs	r0, r5
	adds	r4, r4, #4
	bl	.L5
	cmp	r0, #0
	bne	.L98
.L99:
	ldr	r3, [r4]
	cmp	r3, #0
	bne	.L101
	b	.L98
.L105:
	.align	2
.L104:
	.word	gKeyState
	.word	FMU_FunctionList_OnPressL
	.size	pFMU_PressL, .-pFMU_PressL
	.align	1
	.global	pFMU_PressR
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	pFMU_PressR, %function
pFMU_PressR:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	ldr	r3, .L112
	ldrh	r3, [r3, #4]
	movs	r5, r0
	ldr	r4, .L112+4
	lsls	r3, r3, #23
	bmi	.L107
.L106:
	@ sp needed
	pop	{r4, r5, r6}
	pop	{r0}
	bx	r0
.L109:
	movs	r0, r5
	adds	r4, r4, #4
	bl	.L5
	cmp	r0, #0
	bne	.L106
.L107:
	ldr	r3, [r4]
	cmp	r3, #0
	bne	.L109
	b	.L106
.L113:
	.align	2
.L112:
	.word	gKeyState
	.word	FMU_FunctionList_OnPressR
	.size	pFMU_PressR, .-pFMU_PressR
	.align	1
	.global	pFMU_PressStart
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	pFMU_PressStart, %function
pFMU_PressStart:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	ldr	r3, .L120
	ldrh	r3, [r3, #4]
	movs	r5, r0
	ldr	r4, .L120+4
	lsls	r3, r3, #28
	bmi	.L115
.L114:
	@ sp needed
	pop	{r4, r5, r6}
	pop	{r0}
	bx	r0
.L117:
	movs	r0, r5
	adds	r4, r4, #4
	bl	.L5
	cmp	r0, #0
	bne	.L114
.L115:
	ldr	r3, [r4]
	cmp	r3, #0
	bne	.L117
	b	.L114
.L121:
	.align	2
.L120:
	.word	gKeyState
	.word	FMU_FunctionList_OnPressStart
	.size	pFMU_PressStart, .-pFMU_PressStart
	.align	1
	.global	pFMU_PressSelect
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	pFMU_PressSelect, %function
pFMU_PressSelect:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	ldr	r3, .L128
	ldrh	r3, [r3, #4]
	movs	r5, r0
	ldr	r4, .L128+4
	lsls	r3, r3, #29
	bmi	.L123
.L122:
	@ sp needed
	pop	{r4, r5, r6}
	pop	{r0}
	bx	r0
.L125:
	movs	r0, r5
	adds	r4, r4, #4
	bl	.L5
	cmp	r0, #0
	bne	.L122
.L123:
	ldr	r3, [r4]
	cmp	r3, #0
	bne	.L125
	b	.L122
.L129:
	.align	2
.L128:
	.word	gKeyState
	.word	FMU_FunctionList_OnPressSelect
	.size	pFMU_PressSelect, .-pFMU_PressSelect
	.ident	"GCC: (devkitARM release 55) 10.2.0"
	.code 16
	.align	1
.L5:
	bx	r3
.L64:
	bx	r4
.L23:
	bx	r7
