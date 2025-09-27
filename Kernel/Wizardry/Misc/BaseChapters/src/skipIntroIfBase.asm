@ Skip chapter intro if chapter is base chapter.
@ Replaces 0x1550C.
.thumb

push  {r4, r14}
mov   r4, r0            @ Proc.

@ Vanilla
ldr   r0, =gChapterData
ldrb  r0, [r0, #0xE]
cmp   r0, #0x38         @ Castle Frelia skips chapter intro.
beq   Return
cmp   r0, #0x6          @ This chapter skips chapter intro if flag 0x88 is set.
bne   L1
  mov   r0, #0x88
  ldr   r3, =CheckEventId
  bl    GOTO_R3
  cmp   r0, #0x0
  bne   Return
  b     runIntro

@ Check if base chapter.
L1:
bl    WLC_isBaseChapter
cmp   r0, #0x0
bne   Return
  runIntro:
  ldr   r0, =gProc_ChapterIntroFx
  mov   r1, r4
  ldr   r3, =ProcStartBlocking
  bl    GOTO_R3

Return:
pop   {r4}
pop   {r0}
bx    r0
GOTO_R3:
bx    r3
