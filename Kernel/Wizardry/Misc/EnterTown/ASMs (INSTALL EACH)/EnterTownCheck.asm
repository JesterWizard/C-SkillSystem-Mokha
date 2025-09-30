
.thumb
.align

.global EnterTownCheck
.type EnterTownCheck, %function

EnterTownCheck:
push {lr}
ldr r1,=#0x03005291
ldrb r1,[r1]
cmp r1,#0x5
bne nolamo
mov r0,#0x1
b ender

nolamo:
mov r0,#0x3

ender:
pop {r1}
bx r1
nop
