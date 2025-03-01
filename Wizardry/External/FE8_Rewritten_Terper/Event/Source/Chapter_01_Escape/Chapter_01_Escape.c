#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "jester_headers/macros.h"
#include "jester_headers/soundtrack-ids.h"
#include "jester_headers/maps.h"
#include "jester_headers/flags.h"
#include "jester_headers/miscellaenous.h"
#include "EAstdlib.h"

/**
 * Ally unit and REDA definitions
 */
static const struct REDA REDAs_SETH[] = { 
    { .x = 13, .y = 8, .b = 0x0000, .delayFrames = 0, },
};

static const struct REDA REDAs_EIRIKA[] = {
    { .x = 12, .y = 9, .b = 0x0000, .delayFrames = 0, },
};

static const struct REDA REDAs_FRANZ[] = { 
    { .x = 5, .y = 1, .b = 0x0000, .delayFrames = 0, },
};

static const struct REDA REDAs_GILLIAM[] = {
    { .x = 6, .y = 1, .b = 0x0000, .delayFrames = 0, },
};

static const struct UnitDefinition CH1_EIRIKA_SETH[] = {
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE, NO_ITEM_DROP, NO_AUTOLEVEL, 1, 14, 9, 1, REDAs_SETH, 0, 0, 0, 0, ITEM_SWORD_STEEL, ITEM_LANCE_SILVER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_EIRIKA, CLASS_EIRIKA_LORD, FACTION_ID_BLUE, NO_ITEM_DROP, NO_AUTOLEVEL, 1, 14, 9, 1, REDAs_EIRIKA, 0, 0, 0, 0, ITEM_SWORD_RAPIER, ITEM_VULNERARY),
    {},
};

static const struct UnitDefinition CH1_REINFORCEMENTS_ALLIES[] = {
    UNIT_ENTRY(CHARACTER_FRANZ, CLASS_CAVALIER, FACTION_ID_BLUE, NO_ITEM_DROP, NO_AUTOLEVEL, 1, 5, 0, 1, REDAs_FRANZ, 0, 0, 0, 0, ITEM_SWORD_IRON, ITEM_LANCE_IRON, ITEM_VULNERARY, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_GILLIAM, CLASS_ARMOR_KNIGHT, FACTION_ID_BLUE, NO_ITEM_DROP, NO_AUTOLEVEL, 1, 6, 0, 1, REDAs_GILLIAM, 0, 0, 0, 0, ITEM_LANCE_IRON),
    {},
};

/**
 * Enemy unit and REDA definitions
 */

static const struct REDA REDAs_BREGUET_ENEMY[] = {
    { .x = 2, .y = 5, .b = -1, .delayFrames = 16,}
};
static const struct REDA REDAs_SOLDIER1_ENEMY[] = { 
    { .x = 1, .y = 6, .b = 0xFEFF, } 
};
static const struct REDA REDAs_SOLDIER2_ENEMY[] = { 
    { .x = 3, .y = 8, .b = 0xFEFF, },
    { .x = 3, .y = 6, .b = 0xFEFF, } 
};
static const struct REDA REDAs_SOLDIER3_ENEMY[] = { 
    { .x = 2, .y = 7, .b = 0xFEFF, } 
};
static const struct REDA REDAs_FIGHTER1_ENEMY[] = {
    { .x = 1, .y = 8,  .b = -1, .a = CHARACTER_BREGUET }
};
static const struct REDA REDAs_FIGHTER2_ENEMY[] = {
    { .x = 3, .y = 8, .b = -1, .a = CHARACTER_BREGUET }
};
static const struct REDA REDAs_FIGHTER3_ENEMY[] = {
    { .x = 2, .y = 9, .b = -1, .a = CHARACTER_BREGUET} 
};
static const struct UnitDefinition CH1_BREGUET_FORCES[] = {
    UNIT_ENTRY(CHARACTER_BREGUET, CLASS_ARMOR_KNIGHT, FACTION_ID_RED, NO_ITEM_DROP, NO_AUTOLEVEL, 4, 2, 9, 1, REDAs_BREGUET_ENEMY,  3, 3, 9, 20, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 1, 9, 1, REDAs_SOLDIER1_ENEMY, 0, 3, 9, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 2, 9, 2, REDAs_SOLDIER2_ENEMY, 0, 3, 9, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 3, 2, 9, 1, REDAs_SOLDIER3_ENEMY, 3, 12, 1, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_FIGHTER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 1, 9, 1, REDAs_FIGHTER1_ENEMY, 0, 0, 1, 0, ITEM_AXE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_FIGHTER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 2, 9, 1, REDAs_FIGHTER2_ENEMY, 0, 0, 1, 0, ITEM_AXE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_FIGHTER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 2, 9, 1, REDAs_FIGHTER3_ENEMY, 0, 12, 1, 0, ITEM_AXE_IRON),
    {}
};

static const struct REDA REDAs_FIGHTER4_ENEMY[] = {
    { .x = 13, .y = 8, .b = -1, .a = CHARACTER_BREGUET }
};
static const struct REDA REDAs_SOLDIER4_ENEMY[] = {
    { .x = 13, .y = 9, .b = -1, .a = CHARACTER_BREGUET }
};
static const struct REDA REDAs_FIGHTER5_ENEMY[] = {
    { .x = 12, .y = 9, .b = -1, .a = CHARACTER_BREGUET} 
};
static const struct UnitDefinition CH1_REINFORCEMENTS_ENEMIES[] = {
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_FIGHTER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 1, 14, 9, 1, REDAs_FIGHTER4_ENEMY, 0, 0, 9, 0, ITEM_AXE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_83, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 14, 9, 1, REDAs_SOLDIER4_ENEMY, 0, 0, 9, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_FIGHTER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 1, 14, 9, 1, REDAs_FIGHTER5_ENEMY, 0, 0, 9, 0, ITEM_AXE_IRON),
    {},
};

/**
 * Green unit and REDA definitions
 */

static const struct REDA REDAs_CAVALIER_GREEN[] = {
    { .x = 2, .y = 3, .b = -1},
    { .x = 0, .y = 3, .b = -1},
    { .x = 0, .y = 0, .b = -1}
};
static const struct REDA REDAs_MERCENARY_GREEN[] = {
    { .x = 2, .y = 2, .b = 0x0000} 
};

static const struct UnitDefinition CH1_FRELIAN_FORCES[] = {
    UNIT_ENTRY(0xC1, CLASS_CAVALIER, FACTION_ID_GREEN, NO_ITEM_DROP, NO_AUTOLEVEL, 1, 2, 2, 3, REDAs_CAVALIER_GREEN,  0, 0, 0, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(0xC0, CLASS_MERCENARY, FACTION_ID_GREEN, NO_ITEM_DROP, AUTOLEVEL, 1, 2, 2, 1, REDAs_MERCENARY_GREEN, 0, 0, 0, 0, ITEM_SWORD_IRON),
    {}
};

/**
 * Main events
 */

static const EventScr EventScr_Beginning[] = {
    MUSC(BGM_TENSION)
    LOAD_WAIT(CH1_BREGUET_FORCES)
    STAL(60)
    HIGHLIGHT_COORDINATES(2, 2, 60)
    TEXT_BG(0x26, Chapter_01_Scene_01_Convo_01)
    LOAD_WAIT(CH1_FRELIAN_FORCES)
    ERASE(0xC1)
    HIGHLIGHT_CHARACTER(CHARACTER_BREGUET, 60)
    TEXT_BG(0x1C, Chapter_01_Scene_02_Convo_01)
    MOVE_WAIT(0, CHARACTER_BREGUET, 2, 3)
    /* Force set battle-quotes flag to make the following script-battle not to show battle quote */
    ENUT(EVFLAG_BATTLE_QUOTES)

    StartBattle
    CriticalHit(0, 20)
    NormalDamage(1, 0)
    EndAttack
    FIGHT(CHARACTER_BREGUET, 0xC0, 0, 0)

    /* Clear battle Quotes flag */
    ENUF(EVFLAG_BATTLE_QUOTES)

    KILL(0xC0)
    ERASE(0xC0)
    HIGHLIGHT_CHARACTER(CHARACTER_BREGUET, 60)
    TEXT(Chapter_01_Scene_02_Convo_02)
    MOVE_WAIT(0, CHARACTER_BREGUET, 2, 2)
    MOVE_POSITION_WAIT(24, 1, 6, 1, 3)
    MOVE_POSITION_WAIT(24, 3, 6, 3, 3)
    MOVE_POSITION_WAIT(24, 1, 8, 9, 5)
    MOVE_POSITION_WAIT(24, 2, 7, 8, 3)
    MOVE_POSITION_WAIT(24, 3, 8, 4, 7)
    MOVE_POSITION_WAIT(24, 2, 9, 2, 8)
    STAL(60)
    HIGHLIGHT_COORDINATES(2, 2, 60)
    TEXT_BG(0x26, Chapter_01_Scene_03_Convo_01)
    LOAD_WAIT_PERSIST(CH1_EIRIKA_SETH)
    SET_UNIT_HP(CHARACTER_SETH, 10)
    HIGHLIGHT_CHARACTER(CHARACTER_EIRIKA, 60)
    TEXT(Chapter_01_Scene_04_Convo_01)

    /**
     * Temporary flag(11) is used for triggering event: EventScr_Ch1_Turn_EnemyReinforceArrive,
     * this flag will be unset by event: EventScr_Ch1_Misc_Area
     */
    ENUT(EVFLAG_TMP(11))

    NOFADE
    ENDA
};

static const EventScr EventScr_Ending[] = {
    MUSC(BGM_VICTORY)
    TEXT_BG(0x26, Chapter_01_Scene_10_Convo_01)
    REMA
    FADE_FROM_BLACK(16)
    NEXT_CHAPTER_WITH_MAP(0x38)
    ENDA
};

/**
 * Misc events
 */
static const EventListScr EventScr_Talk_FRANZ_SETH[] = {
    ConvoEvent(Chapter_01_Scene_06_Convo_01)
};

static const EventListScr EventScr_Talk_FRANZ_EIRIKA[] = {
    ConvoEvent(Chapter_01_Scene_07_Convo_01)
};

static const EventListScr EventScr_FRANZ_RETURNS[] = {
    CHECK_TURNS //Store current turn count in slot C
    SVAL(EVT_SLOT_7, 2)
    BNE(0x0, EVT_SLOT_C, EVT_SLOT_7)
    MUSC(BGM_COMRADES)
    LOAD_WAIT_PERSIST(CH1_REINFORCEMENTS_ALLIES)
    HIGHLIGHT_CHARACTER(CHARACTER_FRANZ, 60)
    TEXT(Chapter_01_Scene_05_Convo_01)
    GOTO(0x1)

LABEL(0x0)
    CHECK_EVENTID_
    SADD(EVT_SLOT_2, EVT_SLOT_C, EVT_SLOT_0)
    ENUF_SLOT2

LABEL(0x1)
    NOFADE
    ENDA
};

static const EventListScr EventScr_ENEMY_REINFORCEMENTS[] = {
    CHECK_TURNS //Store current turn count in slot C
    SVAL(EVT_SLOT_7, 3)
    BNE(0x0, EVT_SLOT_C, EVT_SLOT_7)
    REDUCE_VOLUME
    LOAD_WAIT_PERSIST(CH1_REINFORCEMENTS_ENEMIES)
    HIGHLIGHT_CHARACTER(CHARACTER_SOLDIER_83, 60)
    TEXT(Chapter_01_Scene_12_Convo_01)
    RESTORE_VOLUME
    GOTO(0x1)

LABEL(0x0)
    CHECK_EVENTID_
    SADD(EVT_SLOT_2, EVT_SLOT_C, EVT_SLOT_0)
    ENUF_SLOT2

LABEL(0x1)
    NOFADE
    ENDA
};

static const EventListScr EventListScr_HOUSE_TALK_1[] = {
    HouseEvent(Chapter_01_Scene_08_Convo_01, 0x0)
    END_MAIN
};

static const EventListScr EventListScr_HOUSE_TALK_2[] = {
    HouseEvent(Chapter_01_Scene_09_Convo_01, 0x0)
    END_MAIN
};

/**
 * Event list
 */
static const EventListScr EventListScr_Turn[] = {
    AFEV(EVFLAG_TMP(7), EventScr_FRANZ_RETURNS, 0)
    AFEV(EVFLAG_TMP(8), EventScr_ENEMY_REINFORCEMENTS, 0)
    END_MAIN
};

static const EventListScr EventListScr_Character[] = {
    CharacterEventBothWays(EVFLAG_TMP(9), EventScr_Talk_FRANZ_SETH, CHARACTER_FRANZ, CHARACTER_SETH)
    CharacterEventBothWays(EVFLAG_TMP(10), EventScr_Talk_FRANZ_EIRIKA, CHARACTER_FRANZ, CHARACTER_EIRIKA)
    END_MAIN
};

// static const EventListScr EventListScr_ESCAPE_TILE_1[] = {
//     ESCAPE_LOGIC(14, 9)
// };

static const EventListScr EventListScr_Location[] = {
    HOUSE(EVFLAG_TMP(11), EventListScr_HOUSE_TALK_1, 10, 4)
    HOUSE(EVFLAG_TMP(12), EventListScr_HOUSE_TALK_2, 13, 6)
    SEIZE(2, 2)
    // ESCAPE_TILE(EVFLAG_TMP(10), EventListScr_ESCAPE_TILE_1, 14, 9)
    END_MAIN
};

static const EventListScr EventListScr_Misc[] = {
    CAUSE_GAME_OVER_IF_LORD_DIES
    END_MAIN
};

static const EventListScr EventListScr_SelectUnit[] = {
    END_MAIN
};

static const EventListScr EventListScr_SelectDestination[] = {
    END_MAIN
};

static const EventListScr EventListScr_UnitMove[] = {
    END_MAIN
};

static void const * const EventListScr_Tutorial[] = {
    NULL
};

static const u8 TrapData_ThisEvent[] = {
    TRAP_NONE
};

static const u8 TrapData_ThisEventHard[] = {
    TRAP_NONE
};

const struct ChapterEventGroup Chapter01Event = {
    .turnBasedEvents               = EventListScr_Turn,
    .characterBasedEvents          = EventListScr_Character,
    .locationBasedEvents           = EventListScr_Location,
    .miscBasedEvents               = EventListScr_Misc,
    .specialEventsWhenUnitSelected = EventListScr_SelectUnit,
    .specialEventsWhenDestSelected = EventListScr_SelectDestination,
    .specialEventsAfterUnitMoved   = EventListScr_UnitMove,
    .tutorialEvents                = EventListScr_Tutorial,

    .traps            = TrapData_ThisEvent,
    .extraTrapsInHard = TrapData_ThisEventHard,

    .playerUnitsChoice1InEncounter = NULL,
    .playerUnitsChoice2InEncounter = NULL,
    .playerUnitsChoice3InEncounter = NULL,

    .enemyUnitsChoice1InEncounter = NULL,
    .enemyUnitsChoice2InEncounter = NULL,
    .enemyUnitsChoice3InEncounter = NULL,

    .beginningSceneEvents = EventScr_Beginning,
    .endingSceneEvents    = EventScr_Ending,
};
