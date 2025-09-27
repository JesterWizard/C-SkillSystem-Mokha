@ Replaces 0x8009DE1, CallActualSaveMenu.
@ Does a few extra things to ensure GAMECTRL's
@ current chapter data doesn't contain baseChapterID.
.thumb

push  {r4, r14}
mov   r4, r0                @ GAMECTRL procstate.

ldr   r0, =gChapterData
ldrb  r0, [r0, #0xE]
cmp   r0, #0x38             @ Castle Frelia chapter.
beq   Return
  bl    WLC_isBaseChapter
  cmp   r0, #0x0
  beq   CallSaveMenu
    @ Base chapter, GAMECTRL+0x2A and chData+0xE contain wrong chapterID, repair.
    ldr   r0, =gGMData
    ldr   r3, =0x80BD225    @ Gets next chapterID based on WorldMap struct.
    bl    GOTO_R3
    ldr   r1, =gChapterData
    strb  r0, [r1, #0xE]
    mov   r1, #0x2A
    strb  r0, [r4, r1]
    b     Return

CallSaveMenu:
mov   r0, r4
ldr   r3, =Start_savemenu2
bl    GOTO_R3

Return:
pop   {r4}
pop   {r0}
bx    r0
GOTO_R3:
bx    r3
