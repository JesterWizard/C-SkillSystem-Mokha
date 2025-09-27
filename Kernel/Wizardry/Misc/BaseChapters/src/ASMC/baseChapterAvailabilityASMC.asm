@ This ASMC can be used to control which WMnodes have a base chapter available.
@ Memory slot 1: WM node.
@ Memory slot 2: Setting:
@   - 0: Make base chapter unavailable.
@   - 1: Make base chapter available.
@   - 2: Make base chapter unavailable if available or vice versa.
.thumb

ldr   r0, =gEventSlot+0x4
add   r1, r0, #0x4
ldr   r0, [r0]
ldr   r1, [r1]
lsl   r0, #0x2
ldr   r2, =gGMData+0x30
add   r2, r0
ldr   r0, [r2]
mov   r3, #0x4

cmp   r1, #0x0
bne   L1
  orr   r0, r3        @ Setting 0.
  b     L3
L1:
cmp   r1, #0x1
bne   L2
  mvn   r3, r3
  and   r0, r3        @ Setting 1.
  b     L3
L2:
cmp   r1, #0x2
bne   L3
  eor   r0, r3        @ Setting 2.
L3:
str   r0, [r2]
bx    r14
