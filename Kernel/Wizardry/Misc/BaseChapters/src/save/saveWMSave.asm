@ Save base chapter availability flag. Hooked at 0xA70DC.
@ Vanilla uses the first 64 bits of the WMSaveStruct to save two bits of info for 29 world map nodes. We repurpose the last unused six bits to store the base chapter availability flag for six nodes. We also use the first 23 of the last 24 bits of the WMSave struct to store the base chapter availability flags for the remaining 23 world map nodes. These last 24 bits seem to have originally been used for padding.
@ r4: pointer to WMDataStruct.
@ sp: pointer to copy of WMSaveStruct, loaded from save data.
@
@ Details on both WM structs:
@ https://github.com/StanHash/DOC/blob/de69d185c39c3d654f0b07784b20ea0309ca7fbc/WMSave.txt
.thumb

mov   r0, sp
push  {r4-r6, r14}
mov   r5, r0          @ r5: pointer to copy of WMSaveStruct, loaded from save data.

@ Save first six flags.
ldrb  r3, [r5, #0x7]
mov   r1, #0x3
and   r3, r1          @ Clear flags first.
mov   r1, #0x4
mov   r2, r4
add   r2, #0x30
mov   r6, #0x2
Loop1:
  ldr   r0, [r2]
  and   r0, r1
  lsr   r0, #0x2
  lsl   r0, r6
  orr   r3, r0
  add   r2, #0x4
  add   r6, #0x1
  cmp   r6, #0x8
  blt   Loop1
strb  r3, [r5, #0x7]

@ Save remaining 23 flags.
ldr   r3, [r5, #0x20]
ldr   r1, =0x800000FF
and   r3, r1          @ Clear flags first.
mov   r1, #0x4
Loop2:
  ldr   r0, [r2]
  and   r0, r1
  lsr   r0, #0x2
  lsl   r0, r6
  orr   r3, r0
  add   r2, #0x4
  add   r6, #0x1
  cmp   r6, #0x1F
  blt   Loop2
str   r3, [r5, #0x20]

@ Vanilla code, overwritten by hook.
mov   r0, r4
add   r0, #0xCC
ldrb  r1, [r0]
mov   r0, r5
add   r0, #0x20
strb  r1, [r0]
mov   r1, r5
ldrh  r0, [r4, #0x2]
strb  r0, [r1, #0x1A]
ldrh  r0, [r4, #0x4]
strb  r0, [r1, #0x1B]

pop   {r4-r6}
pop   {r0}
bx    r0
