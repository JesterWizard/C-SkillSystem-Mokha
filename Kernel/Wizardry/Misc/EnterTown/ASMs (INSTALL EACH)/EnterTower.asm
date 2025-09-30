.macro blh to, reg=r3
    ldr \reg, =\to
    mov lr, \reg
    .short 0xF800
.endm

.thumb
.align

.global EnterTower
.type EnterTower, %function

EnterTower:
push {lr}
MOV r2, #0x36
ldr r0,=#0x03005266
strb r2,[r0]
MOV r2, #0x4F
ldr r0,=#0x03005268
strb r2,[r0]
ldr r2,=EnterTower
LDR r0,=#0x08A3D748
blh 0x08002E9C
MOV r1, #0xE
blh 0x08002F24
mov r0,#0x17
pop {r1}
bx r1
nop
