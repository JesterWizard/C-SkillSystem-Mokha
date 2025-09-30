
.thumb
.align

.global ExitSerafew
.type ExitSerafew, %function

ExitSerafew:
push {lr}
ldr r1,=#0x0202BCFE
ldrb r1,[r1]
cmp r1,#0x4F
bne nolamo
mov r0,#0x1
b ender

nolamo:
mov r0,#0x3

ender:
pop {r1}
bx r1
nop
