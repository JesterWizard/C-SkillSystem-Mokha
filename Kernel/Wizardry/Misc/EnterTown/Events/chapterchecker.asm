
.thumb
.align

.global chapterchecker
.type chapterchecker, %function


chapterchecker:
lsl r0,r1,#0x5
ldr r1,=#0x082060B0
add r0,r0,r1
ldr r3,=#0x03005268
ldrb r3,[r3]
cmp r3,#0x0
beq normal
mov r0,r3
mov r1,#0x0
ldr r3,=#0x03005268
strb r1,[r3]
ldr r1,=#0x082060B0
b ender

normal:
ldrb r0,[r0,#0x4]

ender:
ldr r3,=#0x80BB5C8|1
bx r3
