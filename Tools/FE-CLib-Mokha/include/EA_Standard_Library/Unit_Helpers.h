#pragma once

#define FlashCursor(pid, time) \
    CUMO_CHAR(pid) \
    STAL(time) \
    CURE

#define START_BATTLE                           SVAL(EVT_SLOT_D, 0)
#define NORMAL_DAMAGE(combatantNumber,damage)  SVAL(EVT_SLOT_1, combatantNumber + damage * 0x100) SAVETOQUEUE
#define CRITICAL_HIT_(combatantNumber,damage)  SVAL(EVT_SLOT_1, combatantNumber + damage * 0x100 + 0x00010000) SAVETOQUEUE
#define MISSED_ATTACK(combatantNumber,damage)  SVAL(EVT_SLOT_1, combatantNumber + damage * 0x100 + 0x00020000) SAVETOQUEUE
#define SILENCER(combatantNumber,damage)       SVAL(EVT_SLOT_1, combatantNumber + damage * 0x100 + 0x00080000) SAVETOQUEUE
#define SURE_SHOT(combatantNumber,damage)      SVAL(EVT_SLOT_1, combatantNumber + damage * 0x100 + 0x00400000) SAVETOQUEUE
#define POISON(combatantNumber,damage)         SVAL(EVT_SLOT_1, combatantNumber + damage * 0x100 + 0x00400000) SAVETOQUEUE
#define DEVIL_REVERSAL(combatantNumber,damage) SVAL(EVT_SLOT_1, combatantNumber + damage * 0x100 + 0x00800000) SAVETOQUEUE
#define PIERCE(combatantNumber,damage)         SVAL(EVT_SLOT_1, combatantNumber + damage * 0x100 + 0xC0000000) SAVETOQUEUE
#define END_ATTACK                             SVAL(EVT_SLOT_1, 0xFFFFFFFF) SAVETOQUEUE

/* Mokha's macro */
#define ATTACKER 1
#define DEFENDER 0
