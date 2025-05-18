    .INCLUDE "macro.inc"
    .SYNTAX UNIFIED
    .section .rodata

.global _SkillTester
_SkillTester:
    .4byte ARM_SkillTester

    .arm
    .section .rodata

.global _ARM_SkillTester_CopyStart
_ARM_SkillTester_CopyStart:
    push {r4, r5, lr}

     @ UNIT_IS_VALID
    cmp r0, #0
    beq .Lend_false
    ldr r2, [r0]
    cmp r2, #0
    beq .Lend_false

    mov r5, r0

    /**
     * Here we may not judge:
     * 1. COMMON_SKILL_VALID(sid)
     * 2. SKILL_INDEX_LIST(sid) valid
     */
    mov r2, r1
    // lsr r3, r1, #8          // r3 = sid_list
    mov r3, #0                 // Force return a 0 so that the jump table always uses the generic skill tester
    add pc, pc, r3, lsl #2

.4byte 0

    /**
     * 1. Only skill index range from 0x01~0xFE can be found in RAM list
     * 2. person/job list aims to support all 0x400 skills
     * 3. item list is limited to judge only skill index 0x300~0x400 on consideration of performance issue 
     */

.Ltesters:
    b _SkillTester_Generic
    b _SkillTester_COMMON
    b _SkillTester_COMMON
    b _SkillTester_IInfo

.Lend_true:
    mov r0, #1
    pop {r4, r5, lr}
    bx lr

.Lend_false:
    mov r0, #0
    pop {r4, r5, lr}
    bx lr

@ Used for 1021 skills in RAM
_SkillTester_Generic:
    push    {r4-r11, lr}         @ Save callee-saved + extra scratch registers

    add     r1, r0, #0x32        @ r1 = &unit->supports[0]

    ldrb    r4, [r1, #0]         @ Load all 7 bytes
    ldrb    r5, [r1, #1]
    ldrb    r6, [r1, #2]
    ldrb    r7, [r1, #3]
    ldrb    r8, [r1, #4]
    ldrb    r9, [r1, #5]
    ldrb    r10, [r1, #6]

    @ Build buffer: r11 = lower 32 bits, r12 = upper bits
    orr     r11, r4, r5, lsl #8
    orr     r11, r11, r6, lsl #16
    orr     r11, r11, r7, lsl #24

    orr     r12, r8, r9, lsl #8
    orr     r12, r12, r10, lsl #16

    mov     r3, #0              @ r3 = index (0 to 4)

.Lskill_loop:
    mov     r1, r3
    mov r0, #10      @ or any scratch register you’re using
    mul r1, r1, r0   @ r1 = r1 * 10


    cmp     r1, #32
    blt     .Lread_low

    @ Offset in r1 >= 32 → read from r12
    sub     r1, r1, #32
    mov     r0, r12
    lsr     r0, r0, r1
    b       .Lmask_compare

.Lread_low:
    mov     r0, r11
    lsr     r0, r0, r1

.Lmask_compare:
    mov     r1, #0xFF
    orr     r1, r1, #0x300     @ r1 = 0x3FF
    and     r0, r0, r1
    cmp     r0, r2
    beq     .Lfound


    add     r3, r3, #1
    cmp     r3, #5
    blt     .Lskill_loop

    b       .Lnot_found

.Lfound:
    pop     {r4-r11, lr}
    b       .Lend_true

.Lnot_found:
    pop     {r4-r11, lr}
    b       .Lend_false
@ end of code used for 1021 skills in RAM

_SkillTester_COMMON:
_SkillTester_PInfo:
    ldr r4, .LgpConstSkillTable_Person
    ldr r0, [r5]
    // adr lr, .Lend_false
    adr lr, _SkillTester_JInfo
    ldrb r0, [r0, #4]

.L_Table:
    ldr r1, [r4]
    add r1, r1, r0, lsl #2
    ldrh r0, [r1], #2
    cmp r2, r0
    beq .Lend_true
    ldrh r0, [r1]
    cmp r2, r0
    beq .Lend_true
    mov pc, lr

_SkillTester_JInfo:
    ldr r4, .LgpConstSkillTable_Job
    ldr r0, [r5, #4]
    ldrb r0, [r0, #4]
    adr lr, .Lend_false
    b .L_Table

_SkillTester_IInfo:
    add r3, r5, #0x1E
    ldr r4, .LgpConstSkillTable_Item
    ldrb r0, [r3], #2
    bl .L_Table
    ldrb r0, [r3], #2
    bl .L_Table
    ldrb r0, [r3], #2
    bl .L_Table
    ldrb r0, [r3], #2
    bl .L_Table
    ldrb r0, [r3], #2
    // bl .L_Table

    adr lr, _SkillTester_COMMON
    // adr lr, .Lend_false
    b .L_Table

.LgpConstSkillTable_Person:
    .4byte gpConstSkillTable_Person
.LgpConstSkillTable_Job:
    .4byte gpConstSkillTable_Job
.LgpConstSkillTable_Item:
    .4byte gpConstSkillTable_Item

.LgBattleActor:
    .4byte gBattleActor

.global _ARM_SkillTester_CopyEnd
_ARM_SkillTester_CopyEnd:
