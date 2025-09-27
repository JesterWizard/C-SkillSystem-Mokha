@ Checks whether new menu option is available.
.thumb

@ Grab current WM node.
ldr   r0, =gGMData
ldrb  r1, [r0, #0x11]
lsl   r1, #0x2
add   r1, #0x30
ldr   r0, [r0, r1]

@ Flag
@   - 0: node available.
@   - 1: next destination.
@   - 2: base chapter available (custom).
@ If flag 0 and 2 are on and 1 is off,
@ enable option.
mov   r1, #0x7
and   r1, r0
mov   r0, #0x3
cmp   r1, #0x5
bne   Return
  mov   r0, #0x1

Return:
bx    r14
