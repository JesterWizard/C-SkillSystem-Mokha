static const EventScr EventScr_Beginning[] = {
    LOAD_WAIT(Renais_Castle_Allies) // Load units and move them if possible
    LOAD_MAP_XY(17, 12, CHAPTER_16) // Load coordinates and the map for this chapter
    FADE_TO_BLACK(16) // Fade in from black in 1 second
    MUSC(BGM_RAID) // Set BGM to RAID
    BROWNBOXTEXT(BB_Renais_Castle, 2, 2) // Set location text pop up
    LOAD_WAIT(Panicked_Soldier_Allies) // Load units and move them if possible
    CAMERA(17, 2) // Set camera to these coordinates
    HIGHLIGHT_CHARACTER(CHARACTER_EPHRAIM, 60) // Set flashing cursor on unit for a number of frames
    TEXT(Chapter_00_Scene_01_Convo_01) // Play this dialogue
    MOVE_WAIT(0, CHARACTER_EPHRAIM, 13, 11) // Move unit to chosen coordinates at given speed then wait
    ERASE(CHARACTER_EPHRAIM) // Erase given unit
    MOVE_1STEP_WAIT(0, CHARACTER_EIRIKA, MOVE_LEFT) // Move unit 1 step in given direction and wait
    HIGHLIGHT_CHARACTER(CHARACTER_EIRIKA, 60) // Set flashing cursor on unit for a number of frames
    TEXT(Chapter_00_Scene_01_Convo_02)
    MOVEONTO_WAIT(0, CHARACTER_SETH, CHARACTER_EIRIKA) // Move unit to another unit and wait    
    ERASE(CHARACTER_EIRIKA) // Erase given unit
    HIGHLIGHT_CHARACTER(CHARACTER_SETH, 60) // Set flashing cursor on unit for a number of frames
    TEXT(Chapter_00_Scene_01_Convo_03)
    MOVE(0, CHARACTER_SETH, 13, 11) // Move unit to chosen coordinates at given speed then wait
    MOVE_TWICE_WAIT(0, CHARACTER_FRANZ, 12, 4, 12, 11) // Move unit twice and wait
    ERASE(CHARACTER_SETH) // Erase given unit
    ERASE(CHARACTER_FRANZ) // Erase given unit
    MOVE(0, CHARACTER_MOULDER, 11, 4)
    MOVE(0, CHARACTER_VANESSA, 15, 4)
    ENUN // Wait for units to finish moving before continuing
    MOVE_1STEP(0, CHARACTER_MOULDER, MOVE_RIGHT)
    MOVE_1STEP(0, CHARACTER_VANESSA, MOVE_LEFT)
    ENUN // Wait for units to finish moving before continuing
    LOAD_WAIT(Renais_Castle_Enemy1) // Load units and move them if possible
    LOAD_WAIT(Renais_Castle_Enemy2) // Load units and move them if possible
    LOAD_WAIT(Renais_Castle_Enemy3) // Load units and move them if possible
    HIGHLIGHT_CHARACTER(CHARACTER_FADO, 60) // Set flashing cursor on unit for a number of frames
    TEXT(Chapter_00_Scene_01_Convo_04)
    FADE_FROM_BLACK(2) // Fade out to black in 8 seconds
    REMA // Remove the current map
    SKIP_POINT
    CLEAR_ALL_UNITS

    // Escape Castle Renais
    LOAD_MAP_XY(0, 0, OUTSIDE_RENAIS)
    FADE_TO_BLACK(16)
    LOAD_WAIT(Renais_Runaways_Allies)
    HIGHLIGHT_CHARACTER(CHARACTER_SETH, 60)
    TEXT_BG(0x1D, Chapter_00_Scene_02_Convo_01)
    MOVE_THRICE_WAIT(0, CHARACTER_FRANZ, 4, 4, 4, 2, 0, 2)
    ERASE(CHARACTER_FRANZ)
    HIGHLIGHT_CHARACTER(CHARACTER_SETH, 60)
    TEXT_BG(0x1D, Chapter_00_Scene_02_Convo_02)
    LOAD_WAIT(Renais_Outside_Castle_Enemy)
    MOVE_1STEP_WAIT(0, CHARACTER_SETH, MOVE_RIGHT)
    MOVE_1STEP_WAIT(0, CHARACTER_EIRIKA, MOVE_LEFT)
    HIGHLIGHT_CHARACTER(CHARACTER_VALTER_PROLOGUE, 60)
    TEXT_BG(0x1D, Chapter_00_Scene_02_Convo_03)
    MOVE_1STEP_WAIT(0, CHARACTER_VALTER_PROLOGUE, MOVE_LEFT)

    // Battle scene
    StartBattle
    NormalDamage(0, 0)
    NormalDamage(1, 0)
    EndAttack
    FIGHT(CHARACTER_SETH, CHARACTER_VALTER_PROLOGUE, 0, false)
    // End battle
    
    /* Escape Valter */
    HIGHLIGHT_CHARACTER(CHARACTER_SETH, 60)
    TEXT(Chapter_00_Scene_02_Convo_04)
    MOVE_1STEP_WAIT(0, CHARACTER_SETH, MOVE_LEFT)
    ERASE(CHARACTER_EIRIKA)
    MOVE_THRICE_WAIT(0, CHARACTER_SETH, 4, 4, 4, 2, 0, 2)
    ERASE(CHARACTER_SETH)
    HIGHLIGHT_CHARACTER(CHARACTER_VALTER_PROLOGUE, 60)
    TEXT(Chapter_00_Scene_02_Convo_05)
    FADE_FROM_BLACK(16)
    REMA
    SKIP_POINT
    CLEAR_ALL_UNITS

    /* Highway map with O'Neil and company */
    LOAD_MAP_XY(0, 0, PROLOGUE)
    FADE_TO_BLACK(16)
    LOAD_WAIT_PERSIST(Eirika_Seth_Allies)
    SET_UNIT_HP(CHARACTER_SETH, 10)
    HIGHLIGHT_CHARACTER(CHARACTER_EIRIKA, 60)
    REDUCE_VOLUME
    TEXT_BG(0x1D, Chapter_00_Scene_03_Convo_01)
    MOVE_WAIT(0, CHARACTER_SETH, 4, 4)
    HIGHLIGHT_CHARACTER(CHARACTER_SETH, 60)
    TEXT(Chapter_00_Scene_03_Convo_02)
    MOVE_WAIT(0, CHARACTER_EIRIKA, 4, 5)
    HIGHLIGHT_CHARACTER(CHARACTER_SETH, 60)
    TEXT(Chapter_00_Scene_03_Convo_03)
    GIVE_ITEM_TO(ITEM_SWORD_RAPIER, CHARACTER_EIRIKA)
    LOAD_WAIT(Grado_Reinforcements_Enemy)
    HIGHLIGHT_CHARACTER(CHARACTER_ONEILL, 60)
    TEXT(Chapter_00_Scene_03_Convo_04)
    GIVE_SKILL_SCROLL_TO(SID_GoddessDance, CHARACTER_EIRIKA)
    // UseArrowTrap(0, 1, 2)
    ASMC(SetGameOptions_Chapter0)
    NOFADE
    ENDA
};

static const EventScr EventScr_Ending[] = {
    MUSC(BGM_VICTORY)
    TEXT_BG(0x1D, Chapter_00_Scene_03_Convo_06)
    FADE_FROM_BLACK(16)
    NEXT_CHAPTER_WITHOUT_MAP(CHAPTER_01)
    REMA
    // SET_FLAG(GUIDE_SUSPEND)
    // SET_FLAG(GUIDE_SAVE)
    // SET_FLAG(GUIDE_MOVEMENT_RANGE)
    // SET_FLAG(GUIDE_VIEWING_UNITS)
    // SET_FLAG(GUIDE_GAME_FLOW)
    // SET_FLAG(GUIDE_DEFEAT_A_BOSS)
    // SET_FLAG(GUIDE_ATTACK_RANGE)
    // SET_FLAG(GUIDE_STRATEGIC_BATTLE_INFO)
    // SET_FLAG(GUIDE_DETAILED_BATTLE_INFO)
    // SET_FLAG(GUIDE_LEVELING_UP)
    // SET_FLAG(GUIDE_WEAPON_DURABILITY)
    ENDA
};

static const EventListScr EventListScr_ONeillAttack[] = {
    CHECK_ENEMIES
    SVAL(EVT_SLOT_7, 1)
    BNE(0x0, EVT_SLOT_C, EVT_SLOT_7)
    MUSC(BGM_TRUTH_DESPAIR_AND_HOPE)
    TEXT(Chapter_00_ONeill_Agro)
    /* slot1 saves the (u8)( (AI1 << 8) | AI2 ) */
    SVAL(EVT_SLOT_1, 0x0)
    CHAI(CHARACTER_ONEILL)
    GOTO(0x1)

// Preserves the flag if the condition isn't met
LABEL(0x0)
    CHECK_EVENTID_
    SADD(EVT_SLOT_2, EVT_SLOT_C, EVT_SLOT_0)
    ENUF_SLOT2

LABEL(0x1)
    NOFADE
    ENDA
};

static const EventListScr EventListScr_OneEnemyLeft[] = {
    CHECK_ENEMIES
    SVAL(EVT_SLOT_7, 1)
    BNE(0x0, EVT_SLOT_C, EVT_SLOT_7)
    HIGHLIGHT_CHARACTER(CHARACTER_EIRIKA, 60)
    TEXT(Chapter_00_Scene_03_Convo_05)
    /* this unsets the event ID so the next turn Oneill will agro (see TURN events) */
    ENUF(EVFLAG_TMP(8))
    GOTO(0x1)

// Preserves the flag if the condition isn't met
LABEL(0x0)
    CHECK_EVENTID_
    SADD(EVT_SLOT_2, EVT_SLOT_C, EVT_SLOT_0)
    ENUF_SLOT2

LABEL(0x1)
    NOFADE
    ENDA
};

static const EventListScr EventListScr_Turn[] = {
    TURN(EVFLAG_TMP(8), EventListScr_ONeillAttack, 1, 255, FACTION_RED)
    END_MAIN
};

static const EventListScr EventListScr_Character[] = {
    END_MAIN
};

static const EventListScr EventListScr_Location[] = {
    // ESCAPE(1, 0)
    END_MAIN
};

static const EventListScr EventListScr_Misc[] = {
    DEFEAT_ALL(EventScr_Ending)
    AFEV(EVFLAG_TMP(7), EventListScr_OneEnemyLeft, 0)
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