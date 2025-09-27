@ Executed when new menu option is selected.
@ Moves gProc_WorldMap to our stuff.
.thumb

push  {r14}
ldr   r0, =gProc_WorldMap
ldr   r3, =ProcFind
bl    GOTO_R3
cmp   r0, #0x0
beq   Return

  ldr   r1, =WLC_procLoadBaseChapter
  str   r1, [r0, #0x4]                  @ Move proc to our function.

Return:
mov   r0, #0x17                         @ Unk, end menu, play beep sound, clear menu graphics.
pop   {r1}
bx    r1
GOTO_R3:
bx  r3
