
.thumb

.include "../CommonDefinitions.inc"

MMBCurrentMPGetter:

	.global	MMBCurrentMPGetter
	.type	MMBCurrentMPGetter, %function

	@ 1. Get unit pointer for the attacker
	@ 2. Get BWL entry 0xC for that unit

	ldr		r2, =WindowBuffer

	lsl		r0, r0, #0x01
	lsl		r1, r1, #0x06

	add		r0, r1, r0
	add		r0, r0, r2

	ldr		r1, =0x2120
	strh	r1, [r0]
	add		r1, #0x01
	strh	r1, [r0, #0x02]
	mov		r1, #0x00
	strh	r1, [r0, #0x04]
	strh	r1, [r0, #0x06]
	strh	r1, [r0, #0x0A]
	strh	r1, [r0, #0x0C]
	ldr		r1, =0x213E
	strh	r1, [r0, #0x08]

	ldr		r0, =BattleBufAttacker
	ldr 	r0, [r0, #0] 		@ Load class data of unit
	ldrb 	r0, [r0, #4] 		@ Load character index
	ldr 	r1, =BWL_GetEntry
	push 	{lr}
	mov 	lr, r1
	bllr
	pop		{r3}
	mov 	lr, r3
	cmp 	r0, #0
	beq 	NoBWLData
	add 	r0, #0xC
	ldrb 	r0, [r0]
	b 		End

	NoBWLData:
	mov 	r0, #0

	End:
	bx		lr

.ltorg
