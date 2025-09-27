@ Returns 1 if chapter is base chapter, 0 otherwise.
@ Args:
@   r0:   chapterID.
.thumb

mov   r2, r0
mov   r0, #0x0
ldr   r1, =WLC_baseChapterIDStart
lsl   r1, #0x5
lsr   r1, #0x5
sub   r2, r1
mov   r1, #0x1D         @ There are 0x1D world map nodes.
cmp   r2, r1
bhs   Return
  mov   r0, #0x1
Return:
bx    r14
