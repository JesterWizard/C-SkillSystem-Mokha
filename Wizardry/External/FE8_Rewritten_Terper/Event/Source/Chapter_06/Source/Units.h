/**
 * Players
 */
static const struct UnitDefinition CH6_PLAYER_UNITS[] = {
    UNIT_ENTRY(CHARACTER_EIRIKA, CLASS_EIRIKA_LORD, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 6, 17, 1, REDAs_EIRIKA_1, 0, 0, 0, 0, ITEM_SWORD_RAPIER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_NATASHA, CLASS_CLERIC, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 5, 18, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_STAFF_MEND, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_COLM, CLASS_THIEF, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 5, 16, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_SWORD_IRON, ITEM_LOCKPICK),
    UNIT_ENTRY(CHARACTER_TANA, CLASS_PEGASUS_KNIGHT, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 0, 4, 19, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_GILLIAM, CLASS_ARMOR_KNIGHT, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 3, 16, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_FRANZ, CLASS_CAVALIER, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 6, 19, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_SWORD_IRON, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_NEIMI, CLASS_ARCHER_F, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 7, 18, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_BOW_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 4, 17, 1, REDAs_SETH_1, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_LANCE_SILVER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_JOSHUA, CLASS_MYRMIDON, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 4, 3, 18, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_SWORD_KILLER, ITEM_VULNERARY),
    {},
};

static const struct UnitDefinition CH6_EIRIKA_SETH[] = {
    UNIT_ENTRY(CHARACTER_EIRIKA, CLASS_EIRIKA_LORD, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 5, 2, 0, 4, REDAs_EIRIKA_ARRIVES, 0, 0, 0, 0, ITEM_SWORD_RAPIER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 2, 0, 5, REDAs_SETH_ARRIVES, 0, 0, 0, 0, ITEM_SWORD_STEEL, ITEM_LANCE_SILVER),
    {},
};

/**
 * Enemies
 */
static const struct UnitDefinition CH6_ENEMY_UNITS[] = {
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 14, 9, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 10, 0, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_LANCE_JAVELIN, ITEM_LANCE_IRON), 
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_FIGHTER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 16, 9, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_AXE_POISON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_MAGE, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 18, 5, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_ANIMA_THUNDER),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 14, 11, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_MERCENARY, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 0, 0, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_SWORD_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_ARMOR_KNIGHT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 16, 12, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_ARMOR_KNIGHT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 16, 13, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_PRIEST, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 18, 9, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_STAFF_MEND),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_SHAMAN, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 18, 4, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_DARK_FLUX),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_FIGHTER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 15, 15, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_AXE_STEEL), 
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_MERCENARY, FACTION_ID_RED, ITEM_DROP, AUTOLEVEL, 6, 15, 10, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_BLADE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_CAVALIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 8, 1, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_BLADE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_CAVALIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 7, 1, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_SWORD_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_CAVALIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 11, 0, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_TROUBADOUR, FACTION_ID_RED, ITEM_DROP, AUTOLEVEL, 6, 10, 0, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_STAFF_MEND, ITEM_ELIXIR),
    UNIT_ENTRY(CHARACTER_MONSTER_B0, CLASS_BAEL, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 1, 27, 4, 0, REDAs_NULL, 0, 0, 20, 0, ITEM_MONSTER_POISON_CLAW),
    UNIT_ENTRY(CHARACTER_NOVALA, CLASS_SHAMAN, FACTION_ID_RED, NO_ITEM_DROP, NO_AUTOLEVEL, 10, 20, 5, 0, REDAs_NULL, 3, 3, 0, 0, ITEM_DARK_FLUX),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_ARCHER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 12, 4, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_BOW_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 14, 6, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_LANCE_IRON), 
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 20, 7, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_ARMOR_KNIGHT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 21, 13, 0, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_LANCE_HORSESLAYER, ITEM_LANCE_JAVELIN),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 19, 13, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_FIGHTER, FACTION_ID_RED, ITEM_DROP, AUTOLEVEL, 5, 13, 6, 0, REDAs_NULL, 0, 3, 0, 0, ITEM_AXE_IRON, ITEM_AXE_HALBERD),
    {}
};

static const struct UnitDefinition CH6_TURN_4_REINFORCEMENTS[] = {
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_CAVALIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 7, 0, 11, 1, REDAs_CAVALIER_1, 0, 0, 1, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_CAVALIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 0, 9, 1, REDAs_CAVALIER_2, 0, 0, 1, 0, ITEM_SWORD_IRON, ITEM_LANCE_JAVELIN), 
    UNIT_ENTRY(CHARACTER_SOLDIER_80, CLASS_CAVALIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 6, 0, 13, 1, REDAs_CAVALIER_3, 0, 0, 1, 0, ITEM_LANCE_JAVELIN, ITEM_LANCE_IRON), 
    {}
};

/**
 * Green units
 */
static const struct UnitDefinition CH6_RENAIS_CITIZENS[] = {
    UNIT_ENTRY(CHARACTER_CITIZEN_YOUNG_GIRL, 0x6E, FACTION_ID_GREEN, NO_ITEM_DROP, AUTOLEVEL, 1, 26, 12, 0, REDAs_NULL, 0, 3, 0, 0),
    UNIT_ENTRY(CHARACTER_CITIZEN_YOUNG_MAN, 0x71, FACTION_ID_GREEN, NO_ITEM_DROP, AUTOLEVEL, 1, 25, 13, 0, REDAs_NULL, 0, 3, 0, 0), 
    UNIT_ENTRY(CHARACTER_CITIZEN_TEEN_GIRL, 0x72, FACTION_ID_GREEN, NO_ITEM_DROP, AUTOLEVEL, 1, 25, 12, 0, REDAs_NULL, 0, 3, 0, 0),
    {}
};
