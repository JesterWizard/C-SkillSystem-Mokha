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

static const struct REDA REDAs_NULL[] = {
    { } 
};

/**
 * Ally unit and REDA definitions
 */

static const struct UnitDefinition CH9_PLAYER_TEMP[] = {
    UNIT_ENTRY(CHARACTER_EIRIKA, CLASS_EIRIKA_LORD, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 8, 2, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_SWORD_RAPIER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 9, 2, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_STAFF_MEND, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_GILLIAM, CLASS_ARMOR_KNIGHT, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 9, 4, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_SWORD_IRON, ITEM_LOCKPICK),
    UNIT_ENTRY(CHARACTER_FRANZ, CLASS_CAVALIER, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 0, 11, 2, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_MOULDER, CLASS_PRIEST, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 10, 3, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_KYLE, CLASS_CAVALIER, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 10, 1, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_SWORD_IRON, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_GARCIA, CLASS_FIGHTER, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 11, 4, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_BOW_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_JOSHUA, CLASS_MYRMIDON, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 4, 6, 2, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_SWORD_KILLER, ITEM_VULNERARY),
    {},
};

static const struct UnitDefinition CH9_INNES_VANESSA_SYRENE[] = {
    UNIT_ENTRY(CHARACTER_INNES, CLASS_SNIPER, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 16, 3, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_SWORD_RAPIER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_SYRENE, CLASS_PEGASUS_KNIGHT, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 16, 4, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_STAFF_MEND, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_VANESSA, CLASS_PEGASUS_KNIGHT, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 17, 3, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_SWORD_IRON, ITEM_LOCKPICK),
    {}
};

static const struct UnitDefinition CH9_VANESSA[] = {
    UNIT_ENTRY(CHARACTER_VANESSA, CLASS_PEGASUS_KNIGHT, FACTION_ID_BLUE,  NO_ITEM_DROP, AUTOLEVEL, 6, 17, 3, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_LANCE_KILLER, ITEM_VULNERARY),
    {}
};

static const struct REDA REDAs_LARACHEL[] = {
    { .x = 18, .y = 2, .b = -1, .delayFrames = 0, },
};
static const struct REDA REDAs_DOZLA[] = {
    { .x = 17, .y = 1, .b = CHARACTER_LARACHEL, .delayFrames = 0, },
};
static const struct REDA REDAs_RENNAC[] = {
    { .x = 19, .y = 1, .b = CHARACTER_LARACHEL, .delayFrames = 0, },
};
static const struct UnitDefinition CH9_EIRIKA_AND_UNITS[] = {
    UNIT_ENTRY(CHARACTER_LARACHEL, CLASS_TROUBADOUR, FACTION_ID_GREEN,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 18, 0, 1, REDAs_LARACHEL, 0, 0, 0, 0, ITEM_SWORD_RAPIER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_DOZLA, CLASS_BERSERKER, FACTION_ID_GREEN,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 17, 0, 1, REDAs_DOZLA, 0, 0, 0, 0, ITEM_STAFF_MEND, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_RENNAC, CLASS_ROGUE, FACTION_ID_GREEN,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 19, 0, 1, REDAs_RENNAC, 0, 0, 0, 0, ITEM_SWORD_IRON, ITEM_LOCKPICK),
    UNIT_ENTRY(CHARACTER_EIRIKA, CLASS_EIRIKA_LORD, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 0, 13, 3, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_VANESSA, CLASS_PEGASUS_KNIGHT, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 17, 3, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_VULNERARY),
    {},
};

static const struct UnitDefinition CH9_PLAYER_UNITS[] = {
    UNIT_ENTRY(CHARACTER_EIRIKA, CLASS_EIRIKA_LORD, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 20, 4, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_SWORD_RAPIER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 19, 1, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_STAFF_MEND, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 19, 5, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_STAFF_MEND, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 19, 3, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_STAFF_MEND, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 21, 1, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_STAFF_MEND, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 20, 2, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_STAFF_MEND, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 21, 6, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_STAFF_MEND, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 20, 5, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_STAFF_MEND, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 21, 3, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_STAFF_MEND, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 20, 7, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_STAFF_MEND, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_SETH, CLASS_PALADIN, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 18, 6, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_STAFF_MEND, ITEM_VULNERARY),
    {},
};

static const struct UnitDefinition CH9_EIRIKA_EPHRAIM_PIRATE[] = {
    UNIT_ENTRY(CHARACTER_EIRIKA, CLASS_EIRIKA_LORD, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 21, 7, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_SWORD_RAPIER, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_EPHRAIM, CLASS_EPHRAIM_LORD, FACTION_ID_BLUE,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 22, 7, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_STAFF_MEND, ITEM_VULNERARY),
    UNIT_ENTRY(CHARACTER_BANDIT_CH5, CLASS_FIGHTER, FACTION_ID_RED,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 22, 8, 0, REDAs_NULL, 0, 0, 0, 0, ITEM_STAFF_MEND, ITEM_VULNERARY),
    {}
};

static const struct REDA REDAs_FRELIAN_MESSENGER[] = {
    { .x = 21, .y = 4, .b = -1, .delayFrames = 0, },
    { .x = 21, .y = 6, .b = -1, .delayFrames = 0, },
};
static const struct UnitDefinition CH9_FRELIAN_MESSENGER[] = {
    UNIT_ENTRY(CHARACTER_MESSENGER, CLASS_PEGASUS_KNIGHT, FACTION_ID_GREEN,  NO_ITEM_DROP, NO_AUTOLEVEL, 1, 12, 4, 2, REDAs_FRELIAN_MESSENGER, 0, 0, 0, 0, ITEM_LANCE_IRON, ITEM_LANCE_KILLER, ITEM_VULNERARY),
    {}
};

// /**
//  * Enemy unit and REDA definitions
//  */

static const struct UnitDefinition CH9_GRADO_THRONEROOM[] = {
    UNIT_ENTRY(CHARACTER_VIGARDE, CLASS_GENERAL, FACTION_ID_RED,  NO_ITEM_DROP, AUTOLEVEL, 1, 14, 2, 0, REDAs_NULL, 0, 0, 0xC, 0, ITEM_LANCE_IRON), 
    UNIT_ENTRY(CHARACTER_LYON, CLASS_NECROMANCER, FACTION_ID_GREEN, NO_ITEM_DROP, AUTOLEVEL, 1, 15, 3, 0, REDAs_NULL, 0, 0, 0xC, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_GLEN, CLASS_WYVERN_LORD, FACTION_ID_RED,  NO_ITEM_DROP, AUTOLEVEL, 1, 13, 5, 0, REDAs_NULL, 0, 0, 0xC, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_VALTER, CLASS_WYVERN_KNIGHT, FACTION_ID_RED,  NO_ITEM_DROP, AUTOLEVEL, 1, 15, 5, 0, REDAs_NULL, 0, 0, 0xC, 0, ITEM_LANCE_IRON), 
    UNIT_ENTRY(CHARACTER_RIEV, CLASS_BISHOP, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 1, 17, 4, 0, REDAs_NULL, 0, 0, 0xC, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_DUESSEL, CLASS_GREAT_KNIGHT, FACTION_ID_RED,  NO_ITEM_DROP, AUTOLEVEL, 1, 11, 4, 0, REDAs_NULL, 0, 0, 0xC, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_SELENA, CLASS_MAGE_KNIGHT_F, FACTION_ID_RED,  NO_ITEM_DROP, AUTOLEVEL, 1, 11, 5, 0, REDAs_NULL, 0, 0, 0xC, 0, ITEM_LANCE_IRON), 
    {}
};

static const struct REDA REDAs_BOSS[] = {
    { .x = 13, .y = 3, .b = -1, .delayFrames = 0, },
};
static const struct UnitDefinition CH9_BOSS[] = {
    UNIT_ENTRY(CHARACTER_BINKS, CLASS_WARRIOR, FACTION_ID_RED, NO_ITEM_DROP, NO_AUTOLEVEL, 1, 6, 3, 1, REDAs_BOSS, 3, 3, 9, 14, ITEM_LANCE_JAVELIN), 
    {}
};

static const struct REDA REDAs_SOLDIER_1[] = {
    { .x = 13, .y = 2, .b = -1, .delayFrames = 0, },
};
static const struct REDA REDAs_SOLDIER_2[] = {
    { .x = 11, .y = 3, .b = -1, .delayFrames = 0, },
};
static const struct REDA REDAs_MERCENARY_1[] = {
    { .x = 9, .y = 4, .b = -1, .delayFrames = 0, },
};
static const struct REDA REDAs_ARCHER_1[] = {
    { .x = 9, .y = 2, .b = -1, .delayFrames = 0, },
};
static const struct REDA REDAs_PIRATE_1[] = {
    { .x = 12, .y = 7, .b = -1, .delayFrames = 0, },
};
static const struct UnitDefinition CH9_ENEMY_UNITS[] = {
    UNIT_ENTRY(CHARACTER_BINKS, CLASS_WARRIOR, FACTION_ID_RED, NO_ITEM_DROP, NO_AUTOLEVEL, 5, 21, 21, 0, REDAs_NULL, 3, 3, 9, 0, ITEM_AXE_STEEL, ITEM_BOW_STEEL),
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_MERCENARY, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 9, 19, 22, 0, REDAs_NULL, 0, 3, 9, 0, ITEM_SWORD_STEEL), 
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_MERCENARY, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 9, 19, 20, 0, REDAs_NULL, 0, 3, 9, 0, ITEM_SWORD_STEEL),
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_MERCENARY, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 9, 18, 21, 0, REDAs_NULL, 0, 3, 9, 0, ITEM_SWORD_STEEL),
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 8, 10, 20, 0, REDAs_NULL, 0, 3, 9, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 8, 6, 2, 1, REDAs_SOLDIER_1, 0, 3, 9, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 8, 6, 5, 1, REDAs_SOLDIER_2, 0, 3, 9, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_SOLDIER, FACTION_ID_RED, ITEM_DROP, AUTOLEVEL, 8, 10, 21, 0, REDAs_NULL, 0, 3, 9, 0, ITEM_LANCE_AXEREAVER),
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_SNIPER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 2, 4, 3, 0, REDAs_NULL, 0, 3, 9, 0, ITEM_BOW_STEEL),
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_MERCENARY, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 9, 6, 6, 1, REDAs_MERCENARY_1, 0, 3, 9, 0, ITEM_SWORD_IRON),
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_MERCENARY, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 9, 17, 21, 0, REDAs_NULL, 0, 0, 9, 0, ITEM_SWORD_STEEL), 
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_MERCENARY, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 9, 4, 6, 0, REDAs_NULL, 0, 0, 9, 0, ITEM_SWORD_IRON),
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_MERCENARY, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 9, 6, 7, 0, REDAs_NULL, 0, 0, 9, 0, ITEM_SWORD_IRON),
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_MERCENARY, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 9, 14, 18, 0, REDAs_NULL, 0, 0, 9, 0, ITEM_SWORD_IRON),
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_MYRMIDON, FACTION_ID_RED, ITEM_DROP, AUTOLEVEL, 9, 3, 9, 0, REDAs_NULL, 0, 3, 9, 0, ITEM_SWORD_KILLER),
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_FIGHTER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 9, 8, 20, 0, REDAs_NULL, 0, 3, 9, 0, ITEM_AXE_IRON),
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_FIGHTER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 9, 8, 21, 0, REDAs_NULL, 0, 3, 9, 0, ITEM_AXE_HANDAXE),
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_ARCHER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 8, 6, 2, 1, REDAs_ARCHER_1, 0, 3, 9, 0, ITEM_BOW_IRON),
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_ARCHER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 8, 17, 16, 0, REDAs_NULL, 0, 3, 9, 0, ITEM_BOW_IRON),
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_ARCHER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 8, 15, 14, 0, REDAs_NULL, 0, 3, 9, 0, ITEM_BOW_IRON), 
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_SHAMAN, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 8, 15, 19, 0, REDAs_NULL, 0, 3, 9, 0, ITEM_DARK_FLUX),
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_PIRATE, FACTION_ID_RED, ITEM_DROP, AUTOLEVEL, 9, 19, 11, 0, REDAs_NULL, 0, 11, 9, 0, ITEM_AXE_IRON, ITEM_MASTERSEAL),
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_PIRATE, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 9, 12, 10, 1, REDAs_PIRATE_1, 0, 4, 9, 0, ITEM_AXE_HANDAXE),
    {}
};

static const struct UnitDefinition CH9_TURN_1_EWAN[] = {
    UNIT_ENTRY(CHARACTER_EWAN, CLASS_PUPIL, FACTION_ID_BLUE, NO_ITEM_DROP, AUTOLEVEL, 8, 19, 0, 0, REDAs_NULL, 0, 4, 9, 0, ITEM_ANIMA_FIRE, ITEM_VULNERARY, ITEM_BOOSTER_CON),
    {}
};


static const struct UnitDefinition CH9_TURN_2_REINFORCEMENTS[] = {
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_PIRATE, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 9, 5, 0, 0, REDAs_NULL, 3, 4, 9, 0, ITEM_AXE_IRON),
    {}
};

static const struct UnitDefinition CH9_TURN_3_REINFORCEMENTS_1[] = {
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_MERCENARY, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 9, 7, 0, 0, REDAs_NULL, 0, 0, 9, 0, ITEM_SWORD_IRON),
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_MAGE, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 8, 4, 0, 0, REDAs_NULL, 0, 0, 9, 0, ITEM_ANIMA_FIRE),
    {}
};

static const struct UnitDefinition CH9_TURN_3_REINFORCEMENTS_2[] = {
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_ARCHER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 8, 14, 23, 0, REDAs_NULL, 0, 0, 9, 0, ITEM_BOW_IRON),
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_ARCHER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 8, 17, 23, 0, REDAs_NULL, 0, 0, 9, 0, ITEM_BOW_IRON),
    {}
};

static const struct REDA REDAs_SOLDIER_RE_1[] = {
    { .x = 2, .y = 19, .b = -1, .delayFrames = 0, },
};
static const struct REDA REDAs_SOLDIER_RE_2[] = {
    { .x = 1, .y = 20, .b = -1, .delayFrames = 0, },
};
static const struct REDA REDAs_SOLDIER_RE_3[] = {
    { .x = 3, .y = 20, .b = -1, .delayFrames = 0, },
};
static const struct REDA REDAs_AMELIA[] = {
    { .x = 2, .y = 21, .b = -1, .delayFrames = 0, },
};

static const struct UnitDefinition CH9_TURN_3_REINFORCEMENTS_3[] = {
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 9, 2, 23, 1, REDAs_SOLDIER_RE_1, 0, 0, 9, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 9, 1, 23, 1, REDAs_SOLDIER_RE_2, 0, 0, 9, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 9, 3, 23, 1, REDAs_SOLDIER_RE_3, 0, 0, 9, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_AMELIA, CLASS_RECRUIT, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 9, 2, 23, 1, REDAs_AMELIA, 0, 3, 10, 0, ITEM_LANCE_SLIM, ITEM_BOOSTER_SPD),
    {}
};

static const struct UnitDefinition CH9_TURN_4_REINFORCEMENTS_1[] = {
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_SOLDIER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 9, 7, 0, 0, REDAs_NULL, 0, 0, 9, 0, ITEM_LANCE_IRON),
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_MERCENARY, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 8, 4, 0, 0, REDAs_NULL, 0, 0, 9, 0, ITEM_SWORD_IRON),
    {}
};

static const struct UnitDefinition CH9_TURN_4_REINFORCEMENTS_2[] = {
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_FIGHTER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 8, 14, 23, 0, REDAs_NULL, 0, 0, 9, 0, ITEM_AXE_IRON),
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_FIGHTER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 8, 17, 23, 0, REDAs_NULL, 0, 0, 9, 0, ITEM_AXE_STEEL),
    {}
};

static const struct UnitDefinition CH9_TURN_5_REINFORCEMENTS_1[] = {
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_FIGHTER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 9, 7, 0, 0, REDAs_NULL, 0, 0, 9, 0, ITEM_AXE_IRON, ITEM_AXE_HANDAXE),
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_FIGHTER, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 8, 4, 0, 0, REDAs_NULL, 0, 0, 9, 0, ITEM_AXE_STEEL),
    {}
};

static const struct UnitDefinition CH9_TURN_5_REINFORCEMENTS_2[] = {
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_MAGE, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 8, 14, 23, 0, REDAs_NULL, 0, 0, 9, 0, ITEM_ANIMA_FIRE),
    UNIT_ENTRY(CHARACTER_MERCENARY_A7, CLASS_MAGE, FACTION_ID_RED, NO_ITEM_DROP, AUTOLEVEL, 8, 17, 23, 0, REDAs_NULL, 0, 0, 9, 0, ITEM_ANIMA_FIRE),
    {}
};


/**
 * Green unit and REDA definitions
 */

/**
 * ASM Events
 */

static const EventScr EventScr_SET_GRADO_CASTLE[] = {
    CHECK_EVBIT(8)
    FADE_FROM_BLACK(16)
    CLEAR_ALL_UNITS
    LOMA(OUTSIDE_GRADO_CASTLE)
    MOVE_CAMERA_TO_POSITION(0, 0)
    FADE_TO_BLACK(16)
    BROWNBOXTEXT(LOCATION_Grado_Keep, 2, 2)
    HIGHLIGHT_COORDINATES(10, 4, 60)
    FADE_FROM_BLACK(16)
    LOMA(INSIDE_GRADO_CASTLE)
    MOVE_CAMERA_TO_POSITION_CENTER(14, 4)
    ENDA
};

static const EventScr EventScr_INSIDE_GRADO_CASTLE[] = {
    SADD(EVT_SLOT_7, EVT_SLOT_2, EVT_SLOT_0)
    SADD(EVT_SLOT_8, EVT_SLOT_3, EVT_SLOT_0)
    SADD(EVT_SLOT_9, EVT_SLOT_4, EVT_SLOT_0)
    SVAL(EVT_SLOT_2, 0x2002)
    CALL(EventScr_SET_GRADO_CASTLE)
    TILECHANGE(0x0)
    LOAD_WAIT(CH9_GRADO_THRONEROOM)
    FADE_TO_BLACK(16)
    HIGHLIGHT_CHARACTER(CHARACTER_VIGARDE, 60)
    SADD(EVT_SLOT_2, EVT_SLOT_7, EVT_SLOT_0)
    TEXT(Chapter_09_Pre_Scene_01_Convo_01)
    MOVE(16, CHARACTER_GLEN, 13, 10)
    MOVE(16, CHARACTER_VALTER, 15, 10)
    MOVE_TWICE_WAIT(16, CHARACTER_RIEV, 17, 5, 15, 5)
    DISA(CHARACTER_GLEN)
    DISA(CHARACTER_VALTER)
    HIGHLIGHT_CHARACTER(CHARACTER_VIGARDE, 60)
    SADD(EVT_SLOT_2, EVT_SLOT_8, EVT_SLOT_0)
    TEXT(Chapter_09_Pre_Scene_01_Convo_02)
    MOVE(16, CHARACTER_RIEV, 15, 10)
    MOVE(16, CHARACTER_SELENA, 15, 5)
    MOVE_TWICE_WAIT(16, CHARACTER_DUESSEL, 11, 5, 13, 5)
    DISA(CHARACTER_CAELLACH)
    DISA(CHARACTER_RIEV)
    HIGHLIGHT_CHARACTER(CHARACTER_VIGARDE, 60)
    SADD(EVT_SLOT_2, EVT_SLOT_9, EVT_SLOT_0)
    TEXT(Chapter_09_Pre_Scene_01_Convo_03)
    MOVE(16, CHARACTER_DUESSEL, 13, 10)
    MOVE(16, CHARACTER_SELENA, 15, 10)
    STAL(60)
    FADE_FROM_BLACK(16)
    ENUN
    CLEAR_ALL_UNITS
    ENDA
};

/**
 * Main events
 */
static const EventScr EventScr_Beginning[] = {
    MUSC(BGM_SOLVE_THE_RIDDLE)
    SVAL(EVT_SLOT_2, 0xA16)
    SVAL(EVT_SLOT_3, 0xA17)
    SVAL(EVT_SLOT_4, 0xA18)
    CALL(EventScr_INSIDE_GRADO_CASTLE)
    LOMA(0x45)
    MOVE_CAMERA_TO_POSITION(0, 0)
    SVAL(EVT_SLOT_1, 1)
    SET_STATE(CHARACTER_EIRIKA)
    LOAD_WAIT(CH9_PLAYER_TEMP)
    FADE_TO_BLACK(16)
    HIGHLIGHT_CHARACTER(CHARACTER_JOSHUA, 60)
    MUSC(BGM_ADVANCE)
    SET_BACKGROUND(0x1C)
    TEXT(Chapter_09_Pre_Scene_02_Convo_01)
    FADE_FROM_BLACK(16)
    CLEAR_ALL_UNITS
    LOMA(CHAPTER_09)
    MOVE_CAMERA_TO_POSITION(18, 4)
    LOAD_WAIT(CH9_INNES_VANESSA_SYRENE)
    FADE_TO_BLACK(16)
    HIGHLIGHT_CHARACTER(CHARACTER_INNES, 60)
    SET_BACKGROUND(0x7)
    TEXT_NO_REMA(Chapter_09_Pre_Scene_03_Convo_01)
    REDUCE_VOLUME
    TEXTCONT
    TEXTEND
    REMA
    FADE_FROM_BLACK(16)
    CLEAR_ALL_UNITS
    CLEAN
    LOAD_WAIT(CH9_EIRIKA_AND_UNITS)
    FADE_TO_BLACK(16)
    HIGHLIGHT_CHARACTER(CHARACTER_LARACHEL, 60)
    MUSC(BGM_LAUGHTER)
    STAL(33)
    SET_BACKGROUND(0x7)
    TEXT_NO_REMA(Chapter_09_Pre_Scene_04_Convo_01)
    MURE(4)
    TEXTCONT
    TEXTEND
    REMA
    FADE_FROM_BLACK(16)
    CLEAN // Clear background
    FADE_TO_BLACK(16)
    MOVE(0, CHARACTER_LARACHEL, 6, 2)
    STAL(16)
    MOVE(0, CHARACTER_DOZLA, 6, 2)
    MOVE(0, CHARACTER_RENNAC, 6, 2)
    ENUN
    CLEN // Clear NPC units
    MOVE(0, CHARACTER_EIRIKA, 16, 3)
    TEXT(Chapter_09_Pre_Scene_05_Convo_01)
    MUSC(MUTE)
    MOVE_CAMERA_TO_POSITION_CENTER(14, 4)
    LOAD_WAIT(CH9_BOSS)
    HIGHLIGHT_CHARACTER(CHARACTER_BINKS, 60)
    MUSC(BGM_RAID)
    TEXT_BG(0x7, Chapter_09_Pre_Scene_06_Convo_01)
    LOAD_WAIT_PERSIST(CH9_ENEMY_UNITS)
    SVAL(EVT_SLOT_1, 1)
    LOAD_WAIT_PERSIST(CH9_VANESSA)
    FADE_FROM_BLACK(16)
    DISA(CHARACTER_BINKS)
    CLEA // Clear Player units
    PREP
    ENDA
};

static const EventScr EventScr_Ending[] = {
    MUSC(BGM_INTO_THE_SHADOWS_OF_VICTORY)
    FADE_FROM_BLACK(16)
    CLEAR_ALL_UNITS
    LOAD_WAIT(CH9_EIRIKA_EPHRAIM_PIRATE)
    LOMA(CHAPTER_09)
    MOVE_CAMERA_TO_POSITION(21, 7)
    FADE_TO_BLACK(16)
    HIGHLIGHT_CHARACTER(CHARACTER_EIRIKA, 60)
    SET_BACKGROUND(0x7)
    TEXT(Chapter_09_Post_Scene_01_Convo_01)
    FADE_FROM_BLACK(16)
    CLEAR_ALL_UNITS
    CLEAN
    LOAD_WAIT(CH9_EIRIKA_EPHRAIM_PIRATE)
    LOMA(CHAPTER_09)
    MOVE_CAMERA_TO_POSITION(21, 7)
    FADE_TO_BLACK(16)
    HIGHLIGHT_CHARACTER(CHARACTER_BANDIT_CH5, 60)
    TEXT_BG(0x7, Chapter_09_Post_Scene_01_Convo_02)
    MUSC(MUTE)
    MOVE_TWICE_WAIT(0, CHARACTER_BANDIT_CH5, 22, 6, 20, -1)
    DISA(CHARACTER_BANDIT_CH5)
    HIGHLIGHT_CHARACTER(CHARACTER_EIRIKA, 60)
    MUSC(BGM_INDIGNATION)
    TEXT_BG(0x7, Chapter_09_Post_Scene_01_Convo_03)
    LOAD_WAIT(CH9_FRELIAN_MESSENGER)
    HIGHLIGHT_CHARACTER(CHARACTER_MESSENGER, 60)
    SET_BACKGROUND(0x7)
    MUSC(BGM_RAID)
    CHECK_EVENTID(0x8)
    TEXT(Chapter_09_Post_Scene_02_Convo_01)
    SET_BACKGROUND(0x7)
    TEXT(Chapter_09_Post_Scene_02_Convo_02)
    GIVE_ITEM_TO(ITEM_BOOSTER_HP, CHARACTER_TANA)
    NEXT_CHAPTER_WITH_MAP(CHAPTER_10)
    ENDA
};

/**
 * Misc events
 */

static const EventListScr EventScr_CH9_TURN_1_REINFORCEMENTS[] = {
    CHECK_TURNS //Store current turn count in slot C
    SVAL(EVT_SLOT_7, 1)
    BNE(0x0, EVT_SLOT_C, EVT_SLOT_7)
    MUSC(BGM_SHADOW_OF_THE_ENEMY)
    MOVE_CAMERA_TO_POSITION(19, 0)
    // WM_WAITFORCAM
    LOAD_WAIT_PERSIST(CH9_TURN_1_EWAN)
    HIGHLIGHT_CHARACTER(CHARACTER_EWAN, 60)
    TEXT(Chapter_09_In_Scene_Ewan)
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

static const EventListScr EventScr_CH9_TURN_2_REINFORCEMENTS[] = {
    CHECK_TURNS //Store current turn count in slot C
    SVAL(EVT_SLOT_7, 2)
    BNE(0x0, EVT_SLOT_C, EVT_SLOT_7)
    MOVE_CAMERA_TO_POSITION(5, 0)
    LOAD_WAIT_PERSIST(CH9_TURN_2_REINFORCEMENTS)
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

static const EventListScr EventScr_CH9_TURN_3_REINFORCEMENTS[] = {
    CHECK_TURNS //Store current turn count in slot C
    SVAL(EVT_SLOT_7, 3)
    BNE(0x0, EVT_SLOT_C, EVT_SLOT_7)
    MOVE_CAMERA_TO_POSITION(6, 2)
    LOAD_WAIT_PERSIST(CH9_TURN_3_REINFORCEMENTS_1)
    STAL(30)
    MOVE_CAMERA_TO_POSITION(15, 21)
    LOAD_WAIT_PERSIST(CH9_TURN_3_REINFORCEMENTS_2)
    STAL(30)
    MOVE_CAMERA_TO_POSITION(2, 20)
    LOAD_WAIT_PERSIST(CH9_TURN_3_REINFORCEMENTS_3)
    TEXT(Chapter_09_In_Scene_Ameila_Arrives)
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

static const EventListScr EventScr_CH9_TURN_4_REINFORCEMENTS[] = {
    CHECK_TURNS //Store current turn count in slot C
    SVAL(EVT_SLOT_7, 4)
    BNE(0x0, EVT_SLOT_C, EVT_SLOT_7)
    MOVE_CAMERA_TO_POSITION(6, 2)
    LOAD_WAIT_PERSIST(CH9_TURN_4_REINFORCEMENTS_1)
    STAL(30)
    MOVE_CAMERA_TO_POSITION(15, 21)
    LOAD_WAIT_PERSIST(CH9_TURN_4_REINFORCEMENTS_2)
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

static const EventListScr EventScr_CH9_TURN_5_REINFORCEMENTS[] = {
    CHECK_TURNS //Store current turn count in slot C
    SVAL(EVT_SLOT_7, 5)
    BNE(0x0, EVT_SLOT_C, EVT_SLOT_7)
    MOVE_CAMERA_TO_POSITION(6, 2)
    LOAD_WAIT_PERSIST(CH9_TURN_5_REINFORCEMENTS_1)
    STAL(30)
    MOVE_CAMERA_TO_POSITION(15, 21)
    LOAD_WAIT_PERSIST(CH9_TURN_5_REINFORCEMENTS_2)
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

static const EventListScr EventListScr_HOUSE_1_TALK[] = {
    MUSC(BGM_LAUGHTER)
    STAL(33)
    HOUSE_EVENT_NO_END(0x0, Chapter_09_In_Scene_House_01)
    CALL(EventScr_RemoveBGIfNeeded) // This is vital, the game crashes without it for this event
    MURE(2)
    GIVE_ITEM_TO(ITEM_BOOSTER_DEF, CHARACTER_EVT_ACTIVE)
    NOFADE
    ENDA
};
static const EventListScr EventListScr_HOUSE_2_TALK[] = {
    HOUSE_EVENT_NO_END(0x0, Chapter_09_In_Scene_House_02)
    CALL(EventScr_RemoveBGIfNeeded) // This is vital, the game crashes without it for this event
    // GIVE_ITEM_TO(SID_HedgeFund, CHARACTER_EVT_ACTIVE)
    NOFADE
    ENDA
};

static const EventListScr EventScr_Talk_EIRIKA_AMELIA[] = {
    MUSC(BGM_COMRADES)
    STAL(33)
    TEXT(Chapter_09_In_Scene_Ameila_Eirika)
    CHANGE_TO_BLUE(CHARACTER_AMELIA)
    NOFADE
    ENDA
};
static const EventListScr EventScr_Talk_FRANZ_AMELIA[] = {
    MUSC(BGM_COMRADES)
    STAL(33)
    TEXT(Chapter_09_In_Scene_Ameila_Franz)
    CHANGE_TO_BLUE(CHARACTER_AMELIA)
    NOFADE
    ENDA
};

static const u16 EventListScr_CH9_ARMORY[] = {
    ITEM_SWORD_IRON,
    ITEM_SWORD_STEEL,
    ITEM_BLADE_STEEL,
    ITEM_LANCE_IRON,
    ITEM_LANCE_STEEL,
    ITEM_LANCE_JAVELIN,
    ITEM_AXE_IRON,
    ITEM_AXE_STEEL,
    ITEM_AXE_HANDAXE,
    ITEM_BOW_IRON,
    ITEM_BOW_STEEL,
    ITEM_NONE,
};

static const u16 EventListScr_CH9_VENDOR[] = {
    ITEM_ANIMA_FIRE,
    ITEM_ANIMA_THUNDER,
    ITEM_LIGHT_LIGHTNING,
    ITEM_LIGHT_SHINE,
    ITEM_STAFF_HEAL,
    ITEM_STAFF_MEND,
    ITEM_STAFF_UNLOCK,
    ITEM_VULNERARY,
    ITEM_ANTITOXIN,
    ITEM_TORCH,
    ITEM_NONE,
};


/**
 * Event list
 */

static const EventListScr EventListScr_Turn[] = {
    AFEV(EVFLAG_TMP(7), EventScr_CH9_TURN_1_REINFORCEMENTS, 0)
    AFEV(EVFLAG_TMP(8), EventScr_CH9_TURN_2_REINFORCEMENTS, 0)
    AFEV(EVFLAG_TMP(9), EventScr_CH9_TURN_3_REINFORCEMENTS, 0)
    AFEV(EVFLAG_TMP(10), EventScr_CH9_TURN_4_REINFORCEMENTS, 0)
    AFEV(EVFLAG_TMP(11), EventScr_CH9_TURN_5_REINFORCEMENTS, 0)
    END_MAIN
};

static const EventListScr EventListScr_Character[] = {
    CharacterEventBothWays(EVFLAG_TMP(12), EventScr_Talk_EIRIKA_AMELIA, CHARACTER_EIRIKA, CHARACTER_AMELIA)
    CharacterEventBothWays(EVFLAG_TMP(13), EventScr_Talk_FRANZ_AMELIA, CHARACTER_FRANZ, CHARACTER_AMELIA)
    END_MAIN
};

static const EventListScr EventListScr_Location[] = {
    ARMORY(EventListScr_CH9_ARMORY, 1, 5)
    VENDOR(EventListScr_CH9_VENDOR, 1, 2)
    VILLAGE(EVFLAG_TMP(14), EventListScr_HOUSE_1_TALK, 1, 11)
    VILLAGE(EVFLAG_TMP(15), EventListScr_HOUSE_2_TALK, 11, 15)
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

static const u8 TrapData_ThisEvent[] = {
    TRAP_NONE
};

static const u8 TrapData_ThisEventHard[] = {
    TRAP_NONE
};

const struct ChapterEventGroup Chapter09Event = {
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

    .playerUnitsInNormal = CH9_PLAYER_UNITS,
    .playerUnitsInHard   = CH9_PLAYER_UNITS,

    .playerUnitsChoice1InEncounter = NULL,
    .playerUnitsChoice2InEncounter = NULL,
    .playerUnitsChoice3InEncounter = NULL,

    .enemyUnitsChoice1InEncounter = NULL,
    .enemyUnitsChoice2InEncounter = NULL,
    .enemyUnitsChoice3InEncounter = NULL,

    .beginningSceneEvents = EventScr_Beginning,
    .endingSceneEvents    = EventScr_Ending,
};