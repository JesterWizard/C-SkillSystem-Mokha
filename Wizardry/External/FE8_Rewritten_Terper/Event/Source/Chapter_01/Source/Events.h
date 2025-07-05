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

   // ASMC(SetGameOptions_Chapter1)

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
    BNE(0x0, EVT_SLOT_C, EVT_SLOT_7) // Don't do anything if it isn't turn 2
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
    BNE(0x0, EVT_SLOT_C, EVT_SLOT_7) // Don't do anything if it isn't turn 3
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

// Use 0 for the flag if you want the event to be always available
static const EventListScr EventListScr_Location[] = {
    HOUSE(0, EventListScr_HOUSE_TALK_1, 10, 4)
    HOUSE(0, EventListScr_HOUSE_TALK_2, 13, 6)
    SEIZE(2, 2)
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