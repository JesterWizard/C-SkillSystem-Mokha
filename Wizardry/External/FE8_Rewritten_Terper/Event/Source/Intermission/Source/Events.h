/**
 * Event list
 */
static const EventScr EventScr_NEW_JOURNEY_TOWER_OF_VALNI_3[] = {
    CAMERA2(11, 4)
    MUSS(0x2e)
    STAL(33)
    LOAD1(0x1, UnitDef_088B7A54)
    ENUN
    FADU(16)
    SVAL(EVT_SLOT_2, 0x53)
    // CALL(EventScr_CallBreakStone)
    CUMO_CHAR(CHARACTER_CAELLACH)
    STAL(60)
    CURE
    TEXTSTART
    TEXTSHOW(Intermission_In_Scene_Tower_Of_Valni_01)
    TEXTEND
    REMA
    MOVE(0x2, CHARACTER_FRELIAN, 10, 4)
    ENUN
    MOVE(0x10, CHARACTER_CAELLACH, 11, 4)
    ENUN
    // CUMO_CHAR(CHARACTER_FRELIAN)
    // STAL(60)
    // CURE
    TEXTSTART
    TEXTSHOW(Intermission_In_Scene_Tower_Of_Valni_02)
    TEXTEND
    REMA
    SVAL(EVT_SLOT_1, 0x3)
    SET_HP(CHARACTER_FRELIAN)
    SVAL(EVT_SLOT_D, 0x0)
    SVAL(EVT_SLOT_1, 0x10000)
    SENQUEUE1
    FIGHT_MAP(CHARACTER_CAELLACH, CHARACTER_FRELIAN, 0, 0)
    DISA(CHARACTER_FRELIAN)
    MOVE_1STEP(0x0, CHARACTER_SELENA, FACING_DOWN)
    ENUN
    CUMO_CHAR(CHARACTER_SELENA)
    STAL(60)
    CURE
    TEXTSTART
    TEXTSHOW(Intermission_In_Scene_Tower_Of_Valni_03)
    TEXTEND
    REMA
    MURE(0x4)
    FADI(4)
    CLEA
    CLEE
    CLEN
    ENDA
};

static const EventScr EventScr_NEW_JOURNEY_TOWER_OF_VALNI_2[] = {
    LOAD1(0x1, UnitDef_088B7964)
    ENUN
    SVAL(EVT_SLOT_B, 0x14000b)
    MOVE_1STEP(0x0, CHAR_EVT_POSITION_AT_SLOTB, FACING_DOWN)
    ENUN
    SVAL(EVT_SLOT_D, 0x0)
    SVAL(EVT_SLOT_1, 0x0)
    SENQUEUE1
    SVAL(EVT_SLOT_B, 0x15000b)
    FIGHT_MAP(CHARACTER_CAELLACH, CHAR_EVT_ACTIVE_UNIT, 0, 0)
    DISA(CHAR_EVT_POSITION_AT_SLOTB)
    SVAL(EVT_SLOT_B, 0x140009)
    MOVE(0x0, CHAR_EVT_POSITION_AT_SLOTB, 9, 22)
    SVAL(EVT_SLOT_B, 0x14000d)
    MOVE(0x10, CHAR_EVT_POSITION_AT_SLOTB, 13, 21)
    LOAD1(0x1, UnitDef_088B798C)
    ENUN
    SVAL(EVT_SLOT_D, 0x0)
    SVAL(EVT_SLOT_1, 0x501)
    SENQUEUE1
    SVAL(EVT_SLOT_1, 0x0)
    SENQUEUE1
    SVAL(EVT_SLOT_B, 0x160009)
    FIGHT_MAP(0x80, CHAR_EVT_ACTIVE_UNIT, 0, 0)
    DISA(CHAR_EVT_POSITION_AT_SLOTB)
    SVAL(EVT_SLOT_D, 0x0)
    SVAL(EVT_SLOT_1, 0x10000)
    SENQUEUE1
    SVAL(EVT_SLOT_B, 0x15000d)
    FIGHT_MAP(0x81, CHAR_EVT_ACTIVE_UNIT, 0, 0)
    DISA(CHAR_EVT_POSITION_AT_SLOTB)
    MOVE(0x0, 0x80, 10, 20)
    MOVE(0x0, 0x81, 13, 18)
    SVAL(EVT_SLOT_B, 0x130010)
    MOVE(0x0, CHAR_EVT_POSITION_AT_SLOTB, 19, 14)
    LOAD1(0x1, UnitDef_088B79F0)
    SVAL(EVT_SLOT_B, 0x130006)
    MOVE(0x0, CHAR_EVT_POSITION_AT_SLOTB, 7, 24)
    LOAD1(0x1, UnitDef_088B79C8)
    ENUN
    SVAL(EVT_SLOT_D, 0x0)
    SVAL(EVT_SLOT_1, 0x0)
    SENQUEUE1
    SVAL(EVT_SLOT_1, 0x1)
    SENQUEUE1
    SVAL(EVT_SLOT_1, 0x0)
    SENQUEUE1
    SVAL(EVT_SLOT_B, 0x180007)
    FIGHT_MAP(0x82, CHAR_EVT_ACTIVE_UNIT, 0, 0)
    DISA(CHAR_EVT_POSITION_AT_SLOTB)
    MOVE(0x0, 0x82, 5, 24)
    MOVE(0x10, CHARACTER_CAELLACH, 9, 17)
    STAL2(30)
    FADI(16)
    ENUN
    CLEA
    CLEE
    CLEN
    ENDA
};

static const EventScr EventScr_NEW_JOURNEY_TOWER_OF_VALNI_1[] = {
    LOMA(TOWER_OF_VALNI_08)
    MOVE_CAMERA_TO_POSITION(12, 20)
    LOAD_WAIT(INTERMISSION_FRELIAN_FORCES)
    FADE_TO_BLACK(16)
    BROWNBOXTEXT(0x66A, 2, 2)
    CALL(EventScr_NEW_JOURNEY_TOWER_OF_VALNI_2)
    CALL(EventScr_NEW_JOURNEY_TOWER_OF_VALNI_3)
    ENDA
};

static const EventScr EventScr_NEW_JOURNEY_CASTLE_FRELIA[] = {
    MUSC(BGM_VICTORY)
    SET_BACKGROUND(0xA)
    TEXT_NO_REMA(Intermission_In_Scene_Frelia_Castle_01)
    FADE_FROM_BLACK(3)
    REMA
    SET_BACKGROUND(0xA)
    TEXT(Intermission_In_Scene_Frelia_Castle_02)
    SET_BACKGROUND(0xE)
    TEXT(Intermission_In_Scene_Frelia_Castle_03)
    MUSC(MUTE)
    STAL(30)
    MUSC(BGM_SOLVE_THE_RIDDLE)
    SET_BACKGROUND(0xE)
    TEXT(Intermission_In_Scene_Frelia_Castle_04)
    TEXT(Intermission_In_Scene_Frelia_Castle_05)
    TEXTCONT
    TEXTEND
    FADE_FROM_BLACK(16)
    REMA
    CALL(EventScr_NEW_JOURNEY_TOWER_OF_VALNI_1)
    REMOVEPORTRAITS
    SET_BACKGROUND(0xE)
    FADE_TO_BLACK(4)
    TEXT(Intermission_In_Scene_Frelia_Castle_06)
    SET_BACKGROUND(0xE)
    TEXT(Intermission_In_Scene_Frelia_Castle_07)
    MUSC(MUTE)
    GIVE_GOLD(10000)
    SET_BACKGROUND(0xA)
    TEXT(Intermission_In_Scene_Frelia_Castle_08)
    FADE_FROM_BLACK(16)
    // ASMC(SetEirikaMode)
    NEXT_CHAPTER_WITH_MAP(CHAPTER_09)
    ENDB
};

/**
 * Main events
 */
static const EventScr EventScr_Beginning[] = {
    CHECK_EVENTID(136) // Flag 0x88
    BEQ(0x1, EVT_SLOT_C, EVT_SLOT_0)
    CALL(EventScr_NEW_JOURNEY_CASTLE_FRELIA)
    GOTO(0x2)

LABEL(0x1)
    MUSC(BGM_VICTORY)
    SET_BACKGROUND(0xE)
    TEXTSTART
    TEXTSHOW(Chapter_01_Scene_11_Convo_01)
    TEXTEND
    SET_BACKGROUND(0x35)
    REMA
    MUSIC_FADEOUT(MUTE)
    TEXTCONT
    TEXTEND
    MUSC(BGM_INTO_THE_SHADOWS_OF_VICTORY)
    TEXTCONT
    FADE_FROM_BLACK(8)
    TEXTEND
    FADE_TO_BLACK(8)
    SET_BACKGROUND(0xE)
    MUSC(BGM_SORROW)
    TEXTSTART
    TEXTSHOW(Chapter_01_Scene_11_Convo_02)
    TEXTEND
    SET_BACKGROUND(0x35)
    MUSIC_FADEOUT(MUTE)
    FADE_FROM_BLACK(8)
    MUSC(BGM_INTO_THE_SHADOWS_OF_VICTORY)
    FADE_TO_BLACK(8)
    SET_BACKGROUND(0xE)
    TEXTSTART
    TEXTSHOW(Chapter_01_Scene_11_Convo_03)
    TEXTEND
    REMA
    GIVE_GOLD(5000)
#if defined(SID_Supply) && (COMMON_SKILL_VALID(SID_Supply))
    GIVE_SKILL_SCROLL_TO(SID_Supply, CHARACTER_EIRIKA)
#endif
    REMA
    FADE_FROM_BLACK(16)
    LOAD_WAIT_PERSIST(CH1_MOULDER_TANA)
    NEXT_CHAPTER_WITH_MAP(CHAPTER_02)

LABEL(0x2)
    ENDA
};

static const EventScr EventScr_Ending[] = {
    ENDA
};

static const EventListScr EventListScr_Turn[] = {
    END_MAIN
};

static const EventListScr EventListScr_Character[] = {
    END_MAIN
};

static const EventListScr EventListScr_Location[] = {
    END_MAIN
};

static const EventListScr EventListScr_Misc[] = {
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