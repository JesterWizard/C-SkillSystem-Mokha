#ifndef GUARD_CONSTANTS_CHARACTERS_H
#define GUARD_CONSTANTS_CHARACTERS_H

enum {
    CHARACTER_NONE         = 0x00,

    // Player Characters
    CHARACTER_EIRIKA       = 0x01,
    CHARACTER_SETH         = 0x02,
    CHARACTER_GILLIAM      = 0x03,
    CHARACTER_FRANZ        = 0x04,
    CHARACTER_MOULDER      = 0x05,
    CHARACTER_VANESSA      = 0x06,
    CHARACTER_ROSS         = 0x07,
    CHARACTER_NEIMI        = 0x08,
    CHARACTER_COLM         = 0x09,
    CHARACTER_GARCIA       = 0x0A,
    CHARACTER_INNES        = 0x0B,
    CHARACTER_LUTE         = 0x0C,
    CHARACTER_NATASHA      = 0x0D,
    CHARACTER_CORMAG       = 0x0E,
    CHARACTER_EPHRAIM      = 0x0F,
    CHARACTER_FORDE        = 0x10,
    CHARACTER_KYLE         = 0x11,
    CHARACTER_AMELIA       = 0x12,
    CHARACTER_ARTUR        = 0x13,
    CHARACTER_GERIK        = 0x14,
    CHARACTER_TETHYS       = 0x15,
    CHARACTER_MARISA       = 0x16,
    CHARACTER_SALEH        = 0x17,
    CHARACTER_EWAN         = 0x18,
    CHARACTER_LARACHEL     = 0x19,
    CHARACTER_DOZLA        = 0x1A,
    CHARACTER_ENEMY_1B     = 0x1B,
    CHARACTER_RENNAC       = 0x1C,
    CHARACTER_DUESSEL      = 0x1D,
    CHARACTER_MYRRH        = 0x1E,
    CHARACTER_KNOLL        = 0x1F,
    CHARACTER_JOSHUA       = 0x20,
    CHARACTER_SYRENE       = 0x21,
    CHARACTER_TANA         = 0x22,
    CHARACTER_LYON_CC      = 0x23,
    CHARACTER_ORSON_CC     = 0x24,
    CHARACTER_GLEN_CC      = 0x25,
    CHARACTER_SELENA_CC    = 0x26,
    CHARACTER_VALTER_CC    = 0x27,
    CHARACTER_RIEV_CC      = 0x28,
    CHARACTER_CAELLACH_CC  = 0x29,
    CHARACTER_FADO_CC      = 0x2A,
    CHARACTER_ISMAIRE_CC   = 0x2B,
    CHARACTER_HAYDEN_CC    = 0x2C,

    // Enemies
    CHARACTER_ENEMY_2D     = 0x2D,
    CHARACTER_ENEMY_2E     = 0x2E,
    CHARACTER_ENEMY_2F     = 0x2F,
    CHARACTER_ENEMY_30     = 0x30,
    CHARACTER_ENEMY_31     = 0x31,
    CHARACTER_ENEMY_32     = 0x32,
    CHARACTER_ENEMY_33     = 0x33,
    CHARACTER_ENEMY_34     = 0x34,
    CHARACTER_ENEMY_35     = 0x35,
    CHARACTER_ENEMY_36     = 0x36,
    CHARACTER_ENEMY_37     = 0x37,
    CHARACTER_ENEMY_38     = 0x38,
    CHARACTER_ENEMY_39     = 0x39,
    CHARACTER_ENEMY_3A     = 0x3A,
    
    // Summoned Characters
    CHARACTER_SUMMON_LYON  = 0x3B,
    CHARACTER_SUMMON_PLUS  = 0x3C,
    CHARACTER_SUMMON_KNOLL = 0x3E,
    CHARACTER_SUMMON_EWAN  = 0x3F,

    // Boss/Unique Enemy Characters
    CHARACTER_LYON         = 0x40,
    CHARACTER_MORVA        = 0x41,
    CHARACTER_ORSON_CH5X   = 0x42,
    CHARACTER_VALTER       = 0x43,
    CHARACTER_SELENA       = 0x44,
    CHARACTER_VALTER_PROLOGUE = 0x45,
    CHARACTER_BREGUET      = 0x46,
    CHARACTER_BONE         = 0x47,
    CHARACTER_BAZBA        = 0x48,
    CHARACTER_ENTOMBED_CH4 = 0x49,
    CHARACTER_SAAR         = 0x4A,
    CHARACTER_NOVALA       = 0x4B,
    CHARACTER_MURRAY       = 0x4C,
    CHARACTER_TIRADO       = 0x4D,
    CHARACTER_BINKS        = 0x4E,
    CHARACTER_PABLO        = 0x4F,
    CHARACTER_MAELDUIN_CHUnk = 0x50, // TODO: which chapter?
    CHARACTER_AIAS         = 0x51,
    CHARACTER_CARLYLE      = 0x52,
    CHARACTER_CAELLACH     = 0x53,
    CHARACTER_PABLO_2      = 0x54, // TODO: which chapter?
    CHARACTER_ENEMY_55     = 0x55,
    CHARACTER_GORGON_CHUnk = 0x56, // TODO: which chapter?
    CHARACTER_RIEV         = 0x57,
    CHARACTER_ENEMY_58     = 0x58,
    CHARACTER_ENEMY_59     = 0x59,
    CHARACTER_GHEB         = 0x5A,
    CHARACTER_BERAN        = 0x5B,
    CHARACTER_CYCLOPS_CHUnk = 0x5C,
    CHARACTER_WIGHT_CHUnk  = 0x5D,
    CHARACTER_DEATHGOYLE_CHUnk = 0x5E,
    CHARACTER_ENEMY_60     = 0x60,
    CHARACTER_ENEMY_61     = 0x61,
    CHARACTER_ENEMY_62     = 0x62,
    CHARACTER_ENEMY_63     = 0x63,
    CHARACTER_ENEMY_64     = 0x64,
    CHARACTER_ENEMY_65     = 0x65,
    CHARACTER_BANDIT_CH5   = 0x66,
    CHARACTER_ONEILL       = 0x68,
    CHARACTER_GLEN         = 0x69,
    CHARACTER_ZONTA        = 0x6A,
    CHARACTER_VIGARDE      = 0x6B,
    CHARACTER_LYON_FINAL   = 0x6C,
    CHARACTER_ORSON        = 0x6D,

    CHARACTER_ENEMY_6E     = 0x6E,
    CHARACTER_ENEMY_6F     = 0x6F,
    CHARACTER_ENEMY_70     = 0x70,
    CHARACTER_ENEMY_71     = 0x71,
    CHARACTER_ENEMY_72     = 0x72,
    CHARACTER_ENEMY_73     = 0x73,
    CHARACTER_ENEMY_74     = 0x74,
    CHARACTER_ENEMY_75     = 0x75,
    CHARACTER_ENEMY_76     = 0x76,
    CHARACTER_ENEMY_77     = 0x77,
    CHARACTER_ENEMY_78     = 0x78,
    CHARACTER_ENEMY_79     = 0x79,
    CHARACTER_ENEMY_7A     = 0x7A,
    CHARACTER_ENEMY_7B     = 0x7B,
    CHARACTER_ENEMY_7C     = 0x7C,
    CHARACTER_ENEMY_7D     = 0x7D,
    CHARACTER_ENEMY_7E     = 0x7E,

    CHARACTER_SOLDIER_7F   = 0x7F,
    CHARACTER_SOLDIER_80   = 0x80,
    CHARACTER_SOLDIER_81   = 0x81,
    CHARACTER_SOLIDER_82   = 0x82,
    CHARACTER_SOLDIER_83   = 0x83,
    CHARACTER_SOLDIER_84   = 0x84,
    CHARACTER_SOLDIER_85   = 0x85,
    CHARACTER_SOLDIER_86   = 0x86,
    CHARACTER_SOLDIER_87   = 0x87,
    CHARACTER_SOLDIER_88   = 0x88,
    CHARACTER_SOLDIER_89   = 0x89,
    CHARACTER_SOLDIER_8A   = 0x8A,
    CHARACTER_SOLDIER_8B   = 0x8B,
    CHARACTER_SOLDIER_8C   = 0x8C,

    CHARACTER_REMNANT_8D   = 0x8D,
    CHARACTER_CUTTHROAT_8E = 0x8E,
    CHAARCTER_REMNANT_8F   = 0x8F,
    CHAARCTER_REMNANT_90   = 0x90,
    CHAARCTER_REMNANT_91   = 0x91,

    CHARACTER_ENEMY_92     = 0x92,
    CHARACTER_ENEMY_93     = 0x93,
    CHARACTER_ENEMY_94     = 0x94,
    CHARACTER_ENEMY_95     = 0x95,
    CHARACTER_ENEMY_96     = 0x96,
    CHARACTER_ENEMY_97     = 0x97,
    CHARACTER_ENEMY_98     = 0x98,
    CHARACTER_ENEMY_99     = 0x99,
    CHARACTER_ENEMY_9A     = 0x9A,
    CHARACTER_ENEMY_9B     = 0x9B,
    CHARACTER_ENEMY_9C     = 0x9C,
    CHARACTER_ENEMY_9D     = 0x9D,
    CHARACTER_ENEMY_9E     = 0x9E,
    CHARACTER_ENEMY_9F     = 0x9F,
    CHARACTER_ENEMY_A0     = 0xA0,
    CHARACTER_ENEMY_A1     = 0xA1,
    CHARACTER_ENEMY_A2     = 0xA2,
    CHARACTER_ENEMY_A3     = 0xA3,
    CHARACTER_ENEMY_A4     = 0xA4,
    CHARACTER_ENEMY_A5     = 0xA5,
    CHARACTER_ENEMY_A6     = 0xA6,

    CHARACTER_MERCENARY_A7 = 0xA7,

    // NEED TO FINISH THE REST OF THIS AT SOME POINT
    CHARACTER_MONSTER_AA   = 0xAA,
    CHARACTER_MONSTER_AC   = 0xAC,
    CHARACTER_MONSTER_B0   = 0xB0,
    CHARACTER_MONSTER_B7   = 0xB7,
    CHARACTER_MONSTER_B8   = 0xB8,
    CHARACTER_MONSTER_BA   = 0xBA,
    CHARACTER_MONSTER_BD   = 0xBD,
    CHARACTER_FOMORTIIS    = 0xBE,

    CHARACTER_FRELIAN      = 0xC0,
    CHARACTER_FADO         = 0xC5,

    CHARACTER_HAYDEN       = 0xC7,
    CHARACTER_MANSEL       = 0xC8,
    CHARACTER_KLIMT        = 0xC9,
    CHARACTER_DARA         = 0xCA,
    CHARACTER_ISMAIRE      = 0xCB,
    CHARACTER_MESSENGER    = 0xCC,

    CHARACTER_CITIZEN_YOUNG_GIRL   = 0xF9,
    CHARACTER_CITIZEN_YOUNG_MAN    = 0xFA,
    CHARACTER_CITIZEN_TEEN_GIRL    = 0xFB,

    CHARACTER_CITIZEN      = 0xFC,
    CHARACTER_ARENA_OPPONENT = 0xFD,
    CHARACTER_WALL         = 0xFE,
    CHARACTER_SNAG         = 0xFF,
};

enum event_autoload_pid_idx {
    CHARACTER_EVT_LEADER = 0,
    CHARACTER_EVT_ACTIVE = -1,
    CHARACTER_EVT_SLOTB = -2,
    CHARACTER_EVT_SLOT2 = -3,
};

#endif // GUARD_CONSTANTS_CHARACTERS_H
