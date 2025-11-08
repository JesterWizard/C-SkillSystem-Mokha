
.thumb

.include "../CommonDefinitions.inc"
.include "../../../../../fe8-kernel-dev.h"

MMBDrawMP:

	.global	MMBDrawMP
	.type	MMBDrawMP, %function

	.set MMBMPXCoordinate,	EALiterals + 0
	.set MMBMPYCoordinate,	EALiterals + 4
	.set MMBHeight,			EALiterals + 8

	@ Inputs:
	@ r0: pointer to proc state
	@ r1: pointer to unit in RAM

	push	{r4-r7, lr}

	mov		r4, r0
	mov		r5, r1

	@ Get tilemap position

	ldr		r0, MMBMPXCoordinate
	ldr		r1, MMBMPYCoordinate

	ldr		r2, =WindowBuffer

	lsl		r0, r0, #0x01
	lsl		r1, r1, #0x06

	add		r0, r1, r0
	add		r0, r0, r2

	ldr		r1, =0x213E
	strh	r1, [r0, #0x08]

	@ Check if we're on a unit to avoid
	@ bad number drawing

	mov		r0, r4
	add		r0, #UnitFlag

	ldrb	r0, [r0]
	cmp		r0, #0x00
	bne		End

	mov		r0, r5
	cmp		r0, #0x00
	beq		End

	@ Get positions for numbers

	ldr		r6, MMBMPXCoordinate
	ldr		r7, MMBMPYCoordinate

	lsl		r6, r6, #0x03 @ mult by 8
	lsl		r7, r7, #0x03

	add		r6, #24 @ past the HP label

	@ check for lower window

	mov		r0, r4
	add		r0, #WindowPosType
	ldrb	r0, [r0]
	lsl		r0, r0, #0x03
	ldr		r1, =WindowSideTable
	add		r0, r1, r0
	mov		r1, #0x03
	ldsb	r0, [r0, r1] @ -1 top 1 bottom
	cmp		r0, #0x00
	blt		SkipBottom

	ldr		r0, MMBHeight
	mov		r1, #20
	sub		r1, r1, r0

	lsl		r1, r1, #0x03
	add		r7, r7, r1

SkipBottom:

	@ @ Get MP numbers

	@ @ BWL - Current MP
	@ @ 1. Get unit pointer for the attacker
	@ @ 2. Get BWL entry 0xC for that unit
	@ @ This is the old system that requires a specific BWL entry

	@ mov 	r0, r5
	@ ldr 	r0, [r0, #0] 		@ Load class data of unit
	@ ldrb 	r0, [r0, #4] 		@ Load character index
	@ ldr 	r1, =BWL_GetEntry
	@ push 	{lr}
	@ mov 	lr, r1
	@ bllr
	@ pop		{r3}
	@ mov 	lr, r3
	@ cmp 	r0, #0
	@ beq 	NoBWLData
	@ add 	r0, #UnitCurrentMP
	@ ldrb 	r0, [r0]
	@ b 		CheckLimit

	@ NoBWLData:
	@ mov 	r0, #0

	@ CheckLimit:
	@ cmp		r0, #254 // JESTER - Originally 99, but expanded as I have made HP go up to 254
	@ ble		SkipDashedCurrentMP

	@ @ MP too high, display --

	mov		r0, #0xFF

	@ This is the new system that uses a custom MP getter derived from a custom fe8-kernel-dev.h header file
	mov		r2, r0
	mov		r0, r6
	mov		r1, r7

	mov		r0, r5
	ldr		r1, =GetUnitCurrentMP
	mov		lr, r1
	bllr

	cmp		r0, #254 // JESTER - Originally 99, but expanded as I have made HP go up to 254
	ble		SkipDashedCurrentMP

	@ HP too high, display --

	mov		r0, #0xFF

SkipDashedCurrentMP:

	mov		r2, r0
	mov		r0, r6
	mov		r1, r7

	ldr		r3, =MMBDrawUnsignedNumber
	mov		lr, r3

	bllr

	add		r6, r6, #25

	@ @ BWL - Max MP
	@ @ 1. Get unit pointer for the attacker
	@ @ 2. Get BWL entry 0xD for that unit
	@ @ This is the old system that requires a specific BWL entry

	@ mov 	r0, r5
	@ ldr 	r0, [r0, #0] 		@ Load class data of unit
	@ ldrb 	r0, [r0, #4] 		@ Load character index
	@ ldr 	r1, =BWL_GetEntry
	@ mov 	lr, r1
	@ bllr
	@ mov 	lr, r3
	@ cmp 	r0, #0
	@ beq 	NoBWLData2
	@ add 	r0, #UnitMaxMP
	@ ldrb 	r0, [r0]
	@ b 		CheckLimit2

	@ NoBWLData2:
	@ mov 	r0, #0

	@ CheckLimit2:
	@ cmp		r0, #254 // JESTER - Originally 99, but expanded as I have made HP go up to 254
	@ ble		SkipDashedMaxMP

	@ @ HP too high, display --

	@ mov		r0, #0xFF

	@ This is the new system that uses a custom MP getter derived from a custom fe8-kernel-dev.h header file
	mov		r2, r0
	mov		r0, r6
	mov		r1, r7

	mov		r0, r5
	ldr		r1, =GetUnitMaxMP
	mov		lr, r1
	bllr

	cmp		r0, #254 // JESTER - Originally 99, but expanded as I have made HP go up to 254
	ble		SkipDashedMaxMP

	@ HP too high, display --

	mov		r0, #0xFF

SkipDashedMaxMP:

	mov		r2, r0
	mov		r0, r6
	mov		r1, r7

	ldr		r3, =MMBDrawUnsignedNumber
	mov		lr, r3

	bllr

End:

	pop		{r4-r7}
	pop		{r0}
	bx		r0

.ltorg

EALiterals:
	@ MMBHPXCoordinate
	@ MMBHPYCoordinate
	@ MMBHeight







