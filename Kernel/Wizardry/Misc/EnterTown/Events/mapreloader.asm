
.thumb
.align

.global chapterchecker
.type chapterchecker, %function


chapterchecker:
MOV r1, #0x7F
AND r0 ,r1
SUB r1, #0x81
AND r0 ,r1
ldr r3,=#0x03005266
ldrb r3,[r3]
cmp r3,#0x0
beq ender
mov r0,#0x0
ldr r3,=#0x03005266
strb r0,[r3]
mov r0,#0x36

ender:
ldr r3,=#0x080B8A70|1
bx r3
