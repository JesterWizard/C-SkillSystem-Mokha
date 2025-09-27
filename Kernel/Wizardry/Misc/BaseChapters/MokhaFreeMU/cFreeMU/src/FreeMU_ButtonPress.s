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
	.file	"FreeMU_ButtonPress.c"
	.text
	.align	1
	.global	FMU_OnButton_StartMenu
	.arch armv4t
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	FMU_OnButton_StartMenu, %function
FMU_OnButton_StartMenu:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	movs	r2, #31
	movs	r1, #0
	ldr	r0, .L2
	ldrb	r3, [r0, #1]
	ands	r3, r2
	push	{r4, lr}
	strb	r3, [r0, #1]
	@ sp needed
	adds	r0, r0, #60
	ldrb	r3, [r0]
	adds	r2, r2, #32
	ands	r3, r2
	strb	r3, [r0]
	movs	r2, r1
	movs	r3, r1
	ldr	r4, .L2+4
	ldr	r0, .L2+8
	bl	.L4
	movs	r0, #1
	pop	{r4}
	pop	{r1}
	bx	r1
.L3:
	.align	2
.L2:
	.word	gLCDIOBuffer
	.word	StartMenuAdjusted
	.word	FreeMovementLMenu
	.size	FMU_OnButton_StartMenu, .-FMU_OnButton_StartMenu
	.align	1
	.global	FMU_OnButton_EndFreeMove
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	FMU_OnButton_EndFreeMove, %function
FMU_OnButton_EndFreeMove:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	ldr	r3, .L6
	@ sp needed
	bl	.L8
	movs	r0, #1
	pop	{r4}
	pop	{r1}
	bx	r1
.L7:
	.align	2
.L6:
	.word	End6CInternal_FreeMU
	.size	FMU_OnButton_EndFreeMove, .-FMU_OnButton_EndFreeMove
	.align	1
	.global	FMU_OnButton_ChangeUnit
	.syntax unified
	.code	16
	.thumb_func
	.fpu softvfp
	.type	FMU_OnButton_ChangeUnit, %function
FMU_OnButton_ChangeUnit:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L24
	push	{r4, r5, r6, lr}
	movs	r4, r0
	ldrh	r0, [r3]
	ldr	r3, .L24+4
	bl	.L8
	movs	r3, #0
	cmp	r0, r3
	beq	.L10
	ldr	r3, [r4, #48]
	ldrb	r0, [r3, #11]
	adds	r0, r0, #1
	lsls	r0, r0, #24
	ldr	r5, .L24+8
	lsrs	r0, r0, #24
	bl	.L26
.L11:
	movs	r3, #16
	ldrsb	r3, [r0, r3]
	adds	r3, r3, #1
	beq	.L12
	ldr	r3, [r0]
	cmp	r3, #0
	bne	.L14
.L13:
	movs	r0, #1
	bl	.L26
.L14:
	ldr	r3, .L24+12
	str	r0, [r3]
	movs	r3, #1
	str	r0, [r4, #48]
.L10:
	@ sp needed
	movs	r0, r3
	pop	{r4, r5, r6}
	pop	{r1}
	bx	r1
.L12:
	ldrb	r0, [r0, #11]
	adds	r0, r0, #1
	lsls	r0, r0, #24
	lsrs	r0, r0, #24
	bl	.L26
	cmp	r0, #0
	beq	.L13
	ldr	r3, [r0]
	cmp	r3, #0
	bne	.L11
	b	.L13
.L25:
	.align	2
.L24:
	.word	ChangeControlledUnitFlag
	.word	CheckEventId
	.word	GetUnit
	.word	gActiveUnit
	.size	FMU_OnButton_ChangeUnit, .-FMU_OnButton_ChangeUnit
	.ident	"GCC: (devkitARM release 55) 10.2.0"
	.code 16
	.align	1
.L8:
	bx	r3
.L4:
	bx	r4
.L26:
	bx	r5
