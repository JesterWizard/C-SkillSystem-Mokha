@ Load base chapter availability flag. Hooked at 0xA716C.
@ Vanilla uses the first 64 bits of the WMSaveStruct to save two bits of info for 29 world map nodes. We repurpose the last unused six bits to store the base chapter availability flag for six nodes. We also use the first 23 of the last 24 bits of the WMSave struct to store the base chapter availability flags for the remaining 23 world map nodes. These last 24 bits seem to have originally been used for padding.
@ r4: pointer to WMDataStruct.
@ sp: pointer to copy of WMSaveStruct, loaded from save data.
@
@ Details on both WM structs:
@ https://github.com/StanHash/DOC/blob/de69d185c39c3d654f0b07784b20ea0309ca7fbc/WMSave.txt
.thumb

mov   r0, sp
push  {r4-r7, r14}
mov   r5, r0          @ r5: pointer to copy of WMSaveStruct, loaded from save data.

@ Load first six flags.
ldrb  r0, [r5, #0x7]
mov   r1, #0x4
mov   r2, r4
add   r2, #0x30
mov   r6, #0x0
Loop1:
  mov   r7, r0
  and   r7, r1
  ldr   r3, [r2, r6]
  orr   r3, r7
  str   r3, [r2, r6]
  lsr   r0, #0x1
  add   r6, #0x4
  cmp   r6, #0x18
  blt   Loop1

@ Load remaining 23 flags.
ldr   r0, [r5, #0x20]
lsr   r0, #0x6
Loop2:
  mov   r7, r0
  and   r7, r1
  ldr   r3, [r2, r6]
  orr   r3, r7
  str   r3, [r2, r6]
  lsr   r0, #0x1
  add   r6, #0x4
  cmp   r6, #0x74
  blt   Loop2

@ Vanilla code, overwritten by hook.
mov   r0, r5
add   r0, #0x20
ldrb  r1, [r0]
mov   r0, r4
add   r0, #0xCC
strb  r1, [r0]
mov   r0, r5

pop   {r4-r7}
pop   {r1}
bx    r1
