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
	.file	"FreeMU_Event.c"
	.text
	.align	1
	.global	pFMU_RunMiscBasedEvents
	.arch armv4t
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	pFMU_RunMiscBasedEvents, %function
pFMU_RunMiscBasedEvents:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	ldr	r3, .L2
	@ sp needed
	ldr	r3, [r3]
	ldrb	r1, [r3, #17]
	ldrb	r0, [r3, #16]
	ldr	r3, .L2+4
	bl	.L4
	pop	{r4}
	pop	{r0}
	bx	r0
.L3:
	.align	2
.L2:
	.word	gActiveUnit
	.word	RunMiscBasedEvents
	.size	pFMU_RunMiscBasedEvents, .-pFMU_RunMiscBasedEvents
	.align	1
	.global	FMUmisc_RunMapEvents
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	FMUmisc_RunMapEvents, %function
FMUmisc_RunMapEvents:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	movs	r5, #16
	movs	r4, #17
	ldr	r3, .L41
	str	r0, [sp, #4]
	ldr	r3, [r3]
	ldrsb	r5, [r3, r5]
	ldrsb	r4, [r3, r4]
	movs	r0, r5
	movs	r1, r4
	ldr	r3, .L41+4
	bl	.L4
	movs	r3, r4
	ldr	r2, .L41+8
	ldrh	r6, [r2]
	ands	r3, r5
	movs	r1, #1
	lsrs	r3, r3, #31
	cmp	r5, r6
	bgt	.L8
	movs	r1, #0
.L8:
	tst	r3, r1
	beq	.L24
	ldrh	r2, [r2, #2]
	movs	r3, #1
	cmp	r4, r2
	bgt	.L9
	movs	r3, #0
.L9:
	lsls	r3, r3, #24
	bne	.L6
.L24:
	ldr	r7, .L41+12
	ldr	r2, [r7]
	lsls	r3, r4, #2
	ldr	r3, [r3, r2]
	ldrb	r3, [r3, r5]
	mov	ip, r3
	ldr	r2, .L41+16
.L10:
	ldrb	r3, [r2]
	cmp	r3, #0
	bne	.L13
	ldr	r0, [sp, #4]
	adds	r0, r0, #52
	ldrb	r3, [r0]
	cmp	r3, #0
	bne	.L14
	subs	r5, r5, #1
.L38:
	lsls	r5, r5, #24
	asrs	r5, r5, #24
.L15:
	movs	r1, r4
	ldr	r3, .L41+4
	movs	r0, r5
	bl	.L4
	movs	r3, r4
	ldr	r2, .L41+8
	ldrh	r6, [r2]
	ands	r3, r5
	movs	r1, #1
	lsrs	r3, r3, #31
	cmp	r5, r6
	bgt	.L19
	movs	r1, #0
.L19:
	tst	r3, r1
	beq	.L25
	ldrh	r2, [r2, #2]
	movs	r3, #1
	cmp	r4, r2
	bgt	.L20
	movs	r3, #0
.L20:
	lsls	r3, r3, #24
	bne	.L6
.L25:
	ldr	r3, [r7]
	lsls	r2, r4, #2
	ldr	r3, [r2, r3]
	ldr	r2, .L41+20
	ldrb	r1, [r3, r5]
.L21:
	ldrb	r3, [r2]
	cmp	r3, #0
	bne	.L23
.L6:
	movs	r6, #0
.L11:
	@ sp needed
	movs	r0, r6
	pop	{r1, r2, r3, r4, r5, r6, r7}
	pop	{r1}
	bx	r1
.L13:
	cmp	r3, r0
	bne	.L12
	ldrb	r3, [r2, #1]
	rsbs	r1, r3, #0
	adcs	r1, r1, r3
	mov	r6, ip
	subs	r3, r3, r6
	rsbs	r6, r3, #0
	adcs	r3, r3, r6
	orrs	r1, r3
	subs	r6, r1, #0
	beq	.L12
.L40:
	movs	r1, r4
	movs	r0, r5
	ldr	r3, .L41+24
	bl	.L4
	b	.L11
.L12:
	adds	r2, r2, #2
	b	.L10
.L14:
	cmp	r3, #1
	bne	.L16
	adds	r5, r5, #1
	b	.L38
.L16:
	lsls	r4, r4, #24
	lsrs	r4, r4, #24
	cmp	r3, #2
	bne	.L17
	adds	r4, r4, #1
.L39:
	lsls	r4, r4, #24
	asrs	r4, r4, #24
	b	.L15
.L17:
	subs	r4, r4, #1
	b	.L39
.L23:
	cmp	r0, r3
	bne	.L22
	ldrb	r3, [r2, #1]
	rsbs	r6, r3, #0
	adcs	r6, r6, r3
	subs	r3, r1, r3
	rsbs	r7, r3, #0
	adcs	r3, r3, r7
	orrs	r6, r3
	bne	.L40
.L22:
	adds	r2, r2, #2
	b	.L21
.L42:
	.align	2
.L41:
	.word	gActiveUnit
	.word	GetLocationEventCommandAt
	.word	gMapSize
	.word	gMapTerrain
	.word	HookListFMU_LocationBasedEvent
	.word	HookListFMU_LocationBasedEventDoor
	.word	RunLocationEvents
	.size	FMUmisc_RunMapEvents, .-FMUmisc_RunMapEvents
	.align	1
	.global	FMU_RunTrapASMC
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	FMU_RunTrapASMC, %function
FMU_RunTrapASMC:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	movs	r6, #16
	movs	r7, #17
	movs	r5, r0
	ldr	r3, .L51
	ldr	r3, [r3]
	ldr	r4, .L51+4
	ldrsb	r6, [r3, r6]
	ldrsb	r7, [r3, r7]
.L44:
	ldrb	r0, [r4]
	cmp	r0, #0
	beq	.L46
	ldr	r3, [r4, #4]
	cmp	r3, #0
	beq	.L45
	ldr	r3, .L51+8
	movs	r1, r7
	movs	r0, r6
	bl	.L4
	ldrb	r3, [r4]
	ldrb	r2, [r0, #2]
	cmp	r2, r3
	bne	.L45
	movs	r0, r5
	ldr	r3, [r4, #4]
	bl	.L4
	movs	r0, #1
.L46:
	@ sp needed
	pop	{r3, r4, r5, r6, r7}
	pop	{r1}
	bx	r1
.L45:
	adds	r4, r4, #8
	b	.L44
.L52:
	.align	2
.L51:
	.word	gActiveUnit
	.word	HookListFMU_TrapList_OnPressA
	.word	GetTrapAt
	.size	FMU_RunTrapASMC, .-FMU_RunTrapASMC
	.align	1
	.global	FMU_RunTrapASMC_Auto
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	FMU_RunTrapASMC_Auto, %function
FMU_RunTrapASMC_Auto:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	movs	r6, #16
	movs	r7, #17
	movs	r5, r0
	ldr	r3, .L61
	ldr	r3, [r3]
	ldr	r4, .L61+4
	ldrsb	r6, [r3, r6]
	ldrsb	r7, [r3, r7]
.L54:
	ldrb	r0, [r4]
	cmp	r0, #0
	beq	.L56
	ldr	r3, [r4, #4]
	cmp	r3, #0
	beq	.L55
	ldr	r3, .L61+8
	movs	r1, r7
	movs	r0, r6
	bl	.L4
	ldrb	r3, [r4]
	ldrb	r2, [r0, #2]
	cmp	r2, r3
	bne	.L55
	movs	r0, r5
	ldr	r3, [r4, #4]
	bl	.L4
	movs	r0, #1
.L56:
	@ sp needed
	pop	{r3, r4, r5, r6, r7}
	pop	{r1}
	bx	r1
.L55:
	adds	r4, r4, #8
	b	.L54
.L62:
	.align	2
.L61:
	.word	gActiveUnit
	.word	HookListFMU_TrapList_Auto
	.word	GetTrapAt
	.size	FMU_RunTrapASMC_Auto, .-FMU_RunTrapASMC_Auto
	.align	1
	.global	pFMU_RunLocBasedAsmcAuto
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	pFMU_RunLocBasedAsmcAuto, %function
pFMU_RunLocBasedAsmcAuto:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	movs	r6, r0
	movs	r1, #0
	movs	r2, #16
	ldr	r5, .L78
	ldr	r3, [r5]
	adds	r6, r6, #44
	ldrsb	r1, [r6, r1]
	ldrsb	r2, [r3, r2]
	movs	r4, r0
	cmp	r1, r2
	bne	.L64
	movs	r2, r0
	adds	r2, r2, #46
	ldrb	r2, [r2]
	ldrb	r3, [r3, #17]
	lsls	r2, r2, #24
	lsls	r3, r3, #24
	asrs	r2, r2, #24
	asrs	r3, r3, #24
	cmp	r2, r3
	beq	.L63
.L64:
	movs	r0, r4
	bl	FMU_RunTrapASMC_Auto
	ldr	r3, [r5]
	cmp	r0, #0
	beq	.L66
.L77:
	ldrb	r2, [r3, #16]
	strb	r2, [r6]
	movs	r2, r4
	ldrb	r3, [r3, #17]
	adds	r2, r2, #46
	strb	r3, [r2]
	movs	r1, #14
	movs	r0, r4
	ldr	r3, .L78+4
	bl	.L4
.L63:
	@ sp needed
	pop	{r4, r5, r6}
	pop	{r0}
	bx	r0
.L66:
	ldrb	r1, [r3, #17]
	ldrb	r0, [r3, #16]
	ldr	r3, .L78+8
	bl	.L4
	cmp	r0, #0
	beq	.L63
	ldr	r3, [r5]
	b	.L77
.L79:
	.align	2
.L78:
	.word	gActiveUnit
	.word	ProcGoto
	.word	RunMiscBasedEvents
	.size	pFMU_RunLocBasedAsmcAuto, .-pFMU_RunLocBasedAsmcAuto
	.align	1
	.global	FMUmisc_RunTalkEvents
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	FMUmisc_RunTalkEvents, %function
FMUmisc_RunTalkEvents:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	ldr	r3, .L104
	movs	r1, r0
	adds	r0, r0, #52
	ldr	r3, [r3]
	ldrb	r0, [r0]
	ldrb	r2, [r3, #16]
	ldrb	r3, [r3, #17]
	cmp	r0, #0
	bne	.L81
	subs	r2, r2, #1
.L102:
	lsls	r2, r2, #24
	lsrs	r2, r2, #24
.L82:
	lsls	r2, r2, #24
	lsls	r3, r3, #24
	asrs	r4, r2, #24
	asrs	r0, r3, #24
	ands	r3, r2
	ldr	r2, .L104+4
	ldrh	r6, [r2]
	movs	r5, #1
	lsrs	r3, r3, #31
	cmp	r4, r6
	bgt	.L86
	movs	r5, #0
.L86:
	tst	r3, r5
	beq	.L85
	ldrh	r2, [r2, #2]
	movs	r3, #1
	cmp	r0, r2
	bgt	.L87
	movs	r3, #0
.L87:
	lsls	r3, r3, #24
	beq	.L85
.L89:
	movs	r0, #0
.L88:
	@ sp needed
	pop	{r4, r5, r6}
	pop	{r1}
	bx	r1
.L81:
	cmp	r0, #1
	bne	.L83
	adds	r2, r2, #1
	b	.L102
.L83:
	cmp	r0, #2
	bne	.L84
	adds	r3, r3, #1
.L103:
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	b	.L82
.L84:
	subs	r3, r3, #1
	b	.L103
.L85:
	ldr	r3, .L104+8
	ldr	r3, [r3]
	lsls	r0, r0, #2
	ldr	r3, [r0, r3]
	ldrb	r0, [r3, r4]
	cmp	r0, #0
	beq	.L89
	ldr	r3, [r1, #48]
	ldr	r3, [r3]
	ldrb	r4, [r3, #4]
	ldr	r3, .L104+12
	bl	.L4
	ldr	r3, [r0]
	ldrb	r5, [r3, #4]
	movs	r0, r4
	movs	r1, r5
	ldr	r3, .L104+16
	bl	.L4
	cmp	r0, #0
	beq	.L89
	movs	r0, r4
	movs	r1, r5
	ldr	r3, .L104+20
	bl	.L4
	movs	r0, #1
	b	.L88
.L105:
	.align	2
.L104:
	.word	gActiveUnit
	.word	gMapSize
	.word	gMapUnit
	.word	GetUnit
	.word	134758249
	.word	134758321
	.size	FMUmisc_RunTalkEvents, .-FMUmisc_RunTalkEvents
	.ident	"GCC: (devkitARM release 55) 10.2.0"
	.code 16
	.align	1
.L4:
	bx	r3
