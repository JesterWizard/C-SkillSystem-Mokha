.macro AddMapINLINE x, y
	add		r0, r4, #\x
	add		r1, r5, #\y
	mov		r2, #0
	mov		r3, #1
	push	{r4}
	_blh4	MapAddInRange			@ (r0=xPos, r1= yPos, r2=Range, r3=Value)
	pop		{r4}
.endm
