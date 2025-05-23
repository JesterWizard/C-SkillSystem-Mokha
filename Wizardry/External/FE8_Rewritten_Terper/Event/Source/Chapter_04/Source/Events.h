static const EventScr EventScr_Beginning[] = {
    LOAD_WAIT(CH4_START_CHAPTER)
    SOUN(SFX_BIRDS_CHIRPING)
    HIGHLIGHT_CHARACTER(CHARACTER_EIRIKA, 60)
    TEXT_BG(0x23, Chapter_04_Scene_01_Convo_01)
    FADE_FROM_BLACK(16)
    CLEAN
    LOAD_WAIT_PERSIST(CH4_ENEMY_UNITS)
    FADE_TO_BLACK(16)
    MUSC(BGM_TENSION)
    HIGHLIGHT_CHARACTER(CHARACTER_EIRIKA, 60)
    TEXT(Chapter_04_Scene_02_Convo_01)
    FADE_FROM_BLACK(16)
    MOVE_CAMERA_TO_POSITION_CENTER(0, 10)
    FADE_TO_BLACK(16)
    REDUCE_VOLUME
    HIGHLIGHT_COORDINATES(1, 11, 60)
    SET_BACKGROUND(0x2)
    TEXTSTART
    TEXTSHOW(Chapter_04_Scene_03_Convo_01)
    TEXTEND
    RESTORE_VOLUME
    FADE_FROM_BLACK(16)
    CLEAN
    MOVE_CAMERA_TO_POSITION_CENTER(0, 0)
    REMA
    FADE_TO_BLACK(16)
    LOAD_WAIT(CH4_ARTUR)
    HIGHLIGHT_CHARACTER(CHARACTER_ARTUR, 60)
    TEXT(Chapter_04_Scene_04_Convo_01)
    MOVE_POSITION_WAIT(24, 11, 6, 9, 3)
    HIGHLIGHT_CHARACTER(CHARACTER_ARTUR, 60)
    TEXT(Chapter_04_Scene_04_Convo_02)
    // Make enemy the active unit again
    MOVE_POSITION_WAIT(24, 9, 3, 9, 3)
    // Battle scene
    StartBattle
    NormalDamage(1, 0) // Artur attacks
    NormalDamage(0, 0) // Enemy attacks 
    CriticalHit(1, 20) // Artur criticals for 20 damage
    EndAttack
    FIGHT(CHARACTER_MONSTER_B8, CHARACTER_ARTUR, 0, false)
    DISA(CHARACTER_MONSTER_B8)
    ERASE(CHARACTER_MONSTER_B8)
    HIGHLIGHT_CHARACTER(CHARACTER_EIRIKA, 60)
    TEXT(Chapter_04_Scene_04_Convo_03)
    LOAD_WAIT_PERSIST(CH4_PLAYER_UNITS)
    PREP_ALT // Prep screen
    MOVE_CAMERA_TO_POSITION_CENTER(0, 0)
    FADE_TO_BLACK(16)
    MUSC(BGM_DISTANT_ROADS)
    HIGHLIGHT_CHARACTER(CHARACTER_ARTUR, 60)
    TEXT(Chapter_04_Scene_05_Convo_01)
    NOFADE
    ENDA
};

static const EventScr EventScr_Ending[] = {
    MUSC(BGM_VICTORY)
/* If Lute wasn't recruited, then have her join here */
    CHECK_EVENTID(EVFLAG_TMP(0x8))
	SVAL(EVT_SLOT_0, 0x1)
	BEQ(0x1, EVT_SLOT_C, EVT_SLOT_0)
    GOTO(0x2)
LABEL(0x1)
    TEXT_BG_HIDE_MAP(0x2, Chapter_04_Scene_06_Convo_01)
    LOAD_WAIT_PERSIST(CH4_LUTE)
    GOTO(0x2)
LABEL(0x2)
    MUSC(BGM_INTO_THE_SHADOWS_OF_VICTORY)
    TEXT_BG_HIDE_MAP(0x18, Chapter_04_Scene_07_Convo_01)
    TEXT_BG_HIDE_MAP(0x18, Chapter_04_Scene_08_Convo_01)
    FADE_FROM_BLACK(16)
    MUSC(BGM_NULL)
    CLEAN
    MOVE_CAMERA_TO_POSITION_CENTER(7, 7)
    CLEAR_ALL_UNITS
    LOAD_WAIT(CH4_EIRIKA_SETH)
    FADE_TO_BLACK(16)
    LOAD_WAIT(CH4_LARACHEL_APPEARS_2)
    MUSC(BGM_LAUGHTER)
    HIGHLIGHT_CHARACTER(CHARACTER_LARACHEL, 60)
    TEXT_BG_HIDE_MAP(0x18, Chapter_04_Scene_09_Convo_01)
    CHECK_IF_TRAINEE_OR_LEVEL_CAPPED(CHARACTER_ROSS, 10, CLASS_JOURNEYMAN)
    BEQ(0x3, EVT_SLOT_C, EVT_SLOT_0)
    SET_BACKGROUND(0x19)
    TEXTSTART
    TEXTSHOW(Chapter_04_Scene_10_Convo_01)
    TEXTEND
    REMA
#if defined(SID_Aptitude) && (COMMON_SKILL_VALID(SID_Aptitude))
    GIVE_SKILL_SCROLL_TO(SID_Aptitude, CHARACTER_ROSS)
#endif
    GOTO(0x3)
LABEL(0x3)
    SET_BACKGROUND(BLACK_BACKGROUND)
    REMA
    NEXT_CHAPTER_WITH_MAP(0x6) // Chapter 5 - The Empire's Reach
    ENDA
};

/**
 * Misc events
 */

static const EventListScr EventScr_CH4_BONEWALKERS_APPEAR[] = {
    CHECK_TURNS //Store current turn count in slot C
    SVAL(EVT_SLOT_7, 2)
    BNE(0x0, EVT_SLOT_C, EVT_SLOT_7)
    STAL(15)
    LOAD_WAIT_PERSIST(CH4_ENEMY_REINFORCEMENTS_BONEWALKERS)
    GOTO(0x1)

LABEL(0x0)
    CHECK_EVENTID_
    SADD(EVT_SLOT_2, EVT_SLOT_C, EVT_SLOT_0)
    ENUF_SLOT2
    GOTO(0x1)

LABEL(0x1)
    NOFADE
    ENDA
};

static const EventListScr EventScr_CH4_REVENANTS_APPEAR[] = {
    CHECK_TURNS //Store current turn count in slot C
    SVAL(EVT_SLOT_7, 3)
    BNE(0x0, EVT_SLOT_C, EVT_SLOT_7)
    STAL(15)
    LOAD_WAIT_PERSIST(CH4_ENEMY_REINFORCEMENTS_REVENANTS)
    GOTO(0x1)

LABEL(0x0)
    CHECK_EVENTID_
    SADD(EVT_SLOT_2, EVT_SLOT_C, EVT_SLOT_0)
    ENUF_SLOT2
    GOTO(0x1)

LABEL(0x1)
    NOFADE
    ENDA
};

static const EventListScr EventScr_CH4_LARACHEL_APPEARS[] = {
    CHECK_TURNS //Store current turn count in slot C
    SVAL(EVT_SLOT_7, 2)
    BNE(0x0, EVT_SLOT_C, EVT_SLOT_7)
    CAMERA(7, 0)
    STAL(15)
    LOAD_WAIT_PERSIST(CH4_LARACHEL_APPEARS)
    MUSC(BGM_LAUGHTER)
    HIGHLIGHT_CHARACTER(CHARACTER_LARACHEL, 60)
    TEXT(Chapter_04_LARACHEL)
    MOVE(24, CHARACTER_LARACHEL, 15, 2)
    MOVE(24, CHARACTER_DOZLA, 15, 1)
    MOVE_WAIT(24, CHARACTER_RENNAC, 15, 1)
    CLEN // Remove NPCs
    GOTO(0x1)

LABEL(0x0)
    CHECK_EVENTID_
    SADD(EVT_SLOT_2, EVT_SLOT_C, EVT_SLOT_0)
    ENUF_SLOT2
    GOTO(0x1)

LABEL(0x1)
    NOFADE
    ENDA
};

static const EventListScr EventScr_CH4_GARCIA_APPEARS[] = {
    CHECK_TURNS //Store current turn count in slot C
    SVAL(EVT_SLOT_7, 3)
    BNE(0x0, EVT_SLOT_C, EVT_SLOT_7)

    CHECK_ALIVE(CHARACTER_ROSS)
    BEQ(0x1, EVT_SLOT_C, EVT_SLOT_0)
    CHECK_DEPLOYED(CHARACTER_ROSS)
    BEQ(0x1, EVT_SLOT_C, EVT_SLOT_0)

    STAL(15)
    MOVE_CAMERA_TO_UNIT(CHARACTER_ROSS)
    HIGHLIGHT_CHARACTER(CHARACTER_ROSS, 60)
    TEXT(Chapter_04_ROSS)
    GOTO(0x1)

LABEL(0x0)
    CHECK_EVENTID_
    SADD(EVT_SLOT_2, EVT_SLOT_C, EVT_SLOT_0)
    ENUF_SLOT2
    GOTO(0x2)

LABEL(0x1)
    LOAD_WAIT_PERSIST(CH4_GARCIA_APPEARS)
    MUSC(BGM_RAID)
    MOVE_CAMERA_TO_UNIT(CHARACTER_GARCIA)
    HIGHLIGHT_CHARACTER(CHARACTER_GARCIA, 60)
    TEXT(Chapter_04_GARCIA)
    GOTO(0x2)

LABEL(0x2)
    NOFADE
    ENDA
};

static const EventListScr EventScr_Talk_ROSS_GARCIA[] = {
    CHANGE_MUSIC_SAVE_PREVIOUS_MUSIC(BGM_COMRADES)
    STAL(33)
    TEXT(Chapter_04_ROSS_GARCIA)
    RESTORE_PREVIOUS_MUSIC
    CHANGE_TO_BLUE(CHARACTER_GARCIA)
    NOFADE
    ENDA
};

static const EventListScr EventListScr_VILLAGE_1_TALK[] = {
    HOUSE_EVENT_NO_END(0x2, Chapter_04_VILLAGE_OLD_MAN)
    CALL(EventScr_RemoveBGIfNeeded)
    GIVE_ITEM_TO(ITEM_AXE_IRON, CHARACTER_EVT_ACTIVE)
    NOFADE
    ENDA
};

static const EventListScr EventListScr_VILLAGE_2_TALK[] = {
    MUSC(BGM_COMRADES)
    STAL(33)
    CHECK_ACTIVE
    SVAL(EVT_SLOT_1, CHARACTER_ARTUR)
    BEQ(0x0, EVT_SLOT_C, EVT_SLOT_1)
    SVAL(EVT_SLOT_1, CHARACTER_EIRIKA)
    BEQ(0x1, EVT_SLOT_C, EVT_SLOT_1) 
    GOTO(0x2)

LABEL(0x0)
    HOUSE_EVENT_NO_END(0x2, Chapter_04_VILLAGE_LUTE_ARTUR)
#if defined(SID_Paragon) && (COMMON_SKILL_VALID(SID_Paragon))
    GIVE_SKILL_SCROLL_TO(SID_Paragon, CHARACTER_ARTUR)
#endif
    GOTO(0x3)

LABEL(0x1)
    HOUSE_EVENT_NO_END(0x2, Chapter_04_VILLAGE_LUTE_EIRIKA)
    GOTO(0x3)

LABEL(0x2)
    HOUSE_EVENT_NO_END(0x2, Chapter_04_VILLAGE_LUTE_GENERIC)
    GOTO(0x3)

LABEL(0x3)
    CALL(EventScr_RemoveBGIfNeeded)
    LOAD_WAIT_PERSIST(CH4_LUTE)
    NOFADE
    ENDA
};


/**
 * Event list
 */

static const EventListScr EventListScr_Turn[] = {
    AFEV(EVFLAG_TMP(7), EventScr_CH4_LARACHEL_APPEARS, 0)
    AFEV(EVFLAG_TMP(8), EventScr_CH4_GARCIA_APPEARS, 0)
    AFEV(EVFLAG_TMP(9), EventScr_CH4_BONEWALKERS_APPEAR, 0)
    AFEV(EVFLAG_TMP(10), EventScr_CH4_REVENANTS_APPEAR, 0)
    END_MAIN
};

static const EventListScr EventListScr_Character[] = {
    CharacterEventBothWays(EVFLAG_TMP(11), EventScr_Talk_ROSS_GARCIA, CHARACTER_ROSS, CHARACTER_GARCIA)
    END_MAIN
};

static const EventListScr EventListScr_Location[] = {
    VILLAGE(EVFLAG_TMP(12), EventListScr_VILLAGE_1_TALK, 8, 2)
    VILLAGE(EVFLAG_TMP(13), EventListScr_VILLAGE_2_TALK, 1, 11)
    END_MAIN
};

static const EventListScr EventListScr_Misc[] = {
    DEFEAT_ALL(EventScr_Ending)
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