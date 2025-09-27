@ Skips phase switch graphics if chapter is base chapter.
@ Blocking child originally created at 0x859A290 in E_BMAPMAIN,
@ Instead we change that proc command with a call to this function.
.thumb

push  {r4, r14}
mov   r4, r0

ldr   r0, =gChapterData
ldrb  r0, [r0, #0xE]
bl    WLC_isBaseChapter
cmp   r0, #0x0
bne   Return
  ldr   r0, =gProc_PhaseSwitchGfx
  mov   r1, r4
  ldr   r3, =ProcStartBlocking
  bl    GOTO_R3

Return:
mov   r0, #0x0      @ Return 0 to ensure E_BMAPMAIN yields, like it originally does.
pop   {r4}
pop   {r1}
bx    r1
GOTO_R3:
bx    r3
