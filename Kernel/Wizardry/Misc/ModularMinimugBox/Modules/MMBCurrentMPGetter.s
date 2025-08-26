
.thumb

.include "../CommonDefinitions.inc"

MMBCurrentMPGetter:

	.global	MMBCurrentMPGetter
	.type	MMBCurrentMPGetter, %function

	@ 1. Get unit pointer for the attacker
	@ 2. Get BWL entry UnitCurrentMP for that unit

	ldr		r0, =BattleBufAttacker
	ldr 	r0, [r0, #0] 		@ Load class data of unit
	ldrb 	r0, [r0, #4] 		@ Load character index
	ldr 	r1, =BWL_GetEntry
	mov 	lr, r1
	bllr
	mov 	lr, r3
	cmp 	r0, #0
	beq 	NoBWLData
	add 	r0, #UnitCurrentMP
	ldrb 	r0, [r0]
	b 		End

	NoBWLData:
	mov 	r0, #0

	End:
	bx		lr

.ltorg
