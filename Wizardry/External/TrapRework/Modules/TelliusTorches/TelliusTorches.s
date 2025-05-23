.thumb
.align

.global TelliusTorchInitialization
.type TelliusTorchInitialization, %function

.global GetAdjacentTelliusTorch
.type GetAdjacentTelliusTorch, %function

.global GetAdjacentTelliusTorchAt
.type GetAdjacentTelliusTorchAt, %function

.global TelliusTorchUsability
.type TelliusTorchUsability, %function

.global TelliusTorchEffect
.type TelliusTorchEffect, %function

.global TelliusTorchMapSpriteFunc
.type TelliusTorchMapSpriteFunc, %function


.macro blh to, reg=r3
  ldr \reg, =\to
  mov lr, \reg
  .short 0xf800
.endm


.equ RefreshTerrainMap,0x8019A65


.equ SpawnTrap,0x802E2B8 @r0 = x coord, r1 = y coord, r2 = trap ID
.equ Init_ReturnPoint,0x8037901

.equ gEventSlot, 0x30004B8

TelliusTorchInitialization:

@r5 = pointer to trap data in events
ldrb r0,[r5,#1] @x coord
ldrb r1,[r5,#2] @y coord
ldrb r2,[r5] @trap ID
blh SpawnTrap @returns pointer to trap data in RAM

@give it our vision range data
ldrb r1,[r5,#3] @initial vision range
strb r1,[r0,#3] 
ldrb r1,[r5,#4] @set vision range
strb r1,[r0,#4]

blh RefreshTerrainMap

ldr r3,=Init_ReturnPoint
bx r3

.ltorg
.align


.equ GetTrapAt,0x802e1f0



GetAdjacentTelliusTorch: @r0 = unit we're checking for adjacency to
push {r14}
ldrb r1,[r0,#0x11] @ Unit Y.
ldrb r0,[r0,#0x10] @ Unit X.
bl GetAdjacentTelliusTorchAt
pop {r1}
bx r1
.ltorg

GetAdjacentTelliusTorchAt: @ r0 = X, r1 = Y.
push {r5,r6,lr}
mov r5,r0
mov r6,r1

CheckLeft:
mov r0,r5
sub r0,#1
mov r1,r6
blh GetTrapAt
cmp r0,#0x00
beq CheckAbove
	ldrb r1,[r0,#2]
	ldr r2,=TelliusTorchTrapIDLink
	ldrb r2,[r2]
	cmp r1,r2
	beq RetTrap
CheckAbove:
mov r0,r5
mov r1,r6
sub r1,#1
blh GetTrapAt
cmp r0,#0x00
beq CheckRight
	ldrb r1,[r0,#2]
	ldr r2,=TelliusTorchTrapIDLink
	ldrb r2,[r2]
	cmp r1,r2
	beq RetTrap
CheckRight:
mov r0,r5
add r0,#1
mov r1,r6
blh GetTrapAt
cmp r0,#0x00
beq CheckBelow
	ldrb r1,[r0,#2]
	ldr r2,=TelliusTorchTrapIDLink
	ldrb r2,[r2]
	cmp r1,r2
	beq RetTrap
CheckBelow:
mov r0,r5
mov r1,r6
add r1,#1
blh GetTrapAt

ldrb r1,[r0,#2]
ldr r2,=TelliusTorchTrapIDLink
ldrb r2,[r2]
cmp r1,r2
beq RetTrap

mov r0,#0

RetTrap:
pop {r5,r6}
pop {r1}
bx r1

.ltorg
.align



TelliusTorchUsability:
push {r4,r14}
ldr r4,=#0x3004E50
ldr r0,[r4]
bl GetAdjacentTelliusTorch
cmp r0,#0
beq Usability_RetFalse

@can't use if cantoing
ldr r0,[r4]
ldr r0,[r0,#0xC]
mov r1,#0x40
and r0,r1
cmp r0,#0
beq Usability_RetTrue

Usability_RetFalse:
mov r0,#3
b Usability_GoBack

Usability_RetTrue:
mov r0,#1

Usability_GoBack:
pop {r4}
pop {r1}
bx r1

.ltorg
.align



TelliusTorchEffect:
push {r4,r14}

@get the trap we're altering
ldr r0,=#0x3004E50
ldr r0,[r0]
bl GetAdjacentTelliusTorch
@r0 = location of trap data

@toggle the state of +3 between 0 or the contents of +4
ldrb r1,[r0,#3]
cmp r1,#0
bne Eff_TurnOff
ldrb r1,[r0,#4]
strb r1,[r0,#3]
b Eff_ActionSet

Eff_TurnOff:
mov r1,#0
strb r1,[r0,#3]

Eff_ActionSet:

@set an action that can be canto'd from
ldr r0,=#0x203A958
add r0,#0x11
mov r1,#0x10 @visit
strb r1,[r0]


mov r0,#0x94		@play beep sound & end menu on next frame & clear menu graphics
pop {r4}
pop {r1}
bx r1

.ltorg
.align




TelliusTorchMapSpriteFunc:
push {r4,r14}
mov r4,r0 @r4 = trap data ptr

ldrb r1,[r4,#3]
cmp r1,#0
bne MapSpriteFunc_TorchIsOn

ldr r0,=TelliusTorchOffMapSpriteIDLink
ldrb r0,[r0]
b MapSpriteFunc_GoBack

MapSpriteFunc_TorchIsOn:
blh TrapRework_NewUpdateAllLightRunes
ldr r0,=TelliusTorchOnMapSpriteIDLink
ldrb r0,[r0]

MapSpriteFunc_GoBack:
pop {r4}
pop {r1}
bx r1

.ltorg
.align

.global ToggleTelliusTorchAt
.type ToggleTelliusTorchAt, %function
ToggleTelliusTorchAt: @ Memory slot 0x1 = coordinates.
push { lr }
ldr r0, =gEventSlot
ldr r0, [ r0, #0x04 ] @ Coordinate word. Lower short has x, higher short has y.
lsr r1, r0, #0x10 @ Get y short in r1.
lsl r0, r0, #0x10
lsr r0, r0, #0x10 @ Clear the upper y short from the x coordinate.
blh GetTrapAt @ Get the pointer to the trap at these coordinates.
cmp r0, #0x00
beq EndSetTelliusTorchAt
	@ Toggle the state of +3 between 0 or the contents of +4.
	ldrb r1, [ r0, #0x03 ]
	cmp r1, #0x0
	bne ToggleTurnOff
		ldrb r1, [ r0, #0x04 ]
		strb r1, [ r0, #0x03 ]
		b EndSetTelliusTorchAt
	ToggleTurnOff:
		mov r1, #0x0
		strb r1, [ r0, #0x03 ]
	blh RefreshTerrainMap
EndSetTelliusTorchAt:
pop { r0 }
bx r0
.ltorg
