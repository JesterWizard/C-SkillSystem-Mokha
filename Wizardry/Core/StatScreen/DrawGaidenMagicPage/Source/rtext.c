#include "common-chax.h"
#include "stat-screen.h"
#include "constants/texts.h"

static _DECL_INFO RText_Name, RText_Class, RText_Level, RText_Exp, RText_Hp;
static _DECL_INFO RText_Page5_White_Magic_Title, RText_Page5_White_Magic_1, RText_Page5_White_Magic_2, RText_Page5_White_Magic_3, RText_Page5_White_Magic_4, RText_Page5_White_Magic_5;
static _DECL_INFO RText_Page5_Black_Magic_Title, RText_Page5_Black_Magic_1, RText_Page5_Black_Magic_2, RText_Page5_Black_Magic_3, RText_Page5_Black_Magic_4, RText_Page5_Black_Magic_5;

_DECL_INFO * const RTextGaidenMagic = &RText_Page5_White_Magic_Title;

/* White magic */
static _DECL_INFO RText_Page5_White_Magic_Title = {
    &RText_Page5_White_Magic_5, 
    &RText_Page5_White_Magic_1, 
    &RText_Name, 
    &RText_Page5_Black_Magic_Title,
    0x66, 0x18, MSG_STAT_SCREEN_WHITE_MAGIC_DESC,
    NULL, NULL
};

static _DECL_INFO RText_Page5_White_Magic_1 = {
    &RText_Page5_White_Magic_Title, 
    &RText_Page5_White_Magic_2, 
    &RText_Name, 
    &RText_Page5_Black_Magic_1,
    0x66, 0x28, 1,
    HbRedirect_GaidenWhiteMagicPageCommon, HbPopuplate_GaidenWhiteMagicPageCommon
};

static _DECL_INFO RText_Page5_White_Magic_2 = {
    &RText_Page5_White_Magic_1, 
    &RText_Page5_White_Magic_3, 
    &RText_Name, 
    &RText_Page5_Black_Magic_2,
    0x66, 0x38, 2,
    HbRedirect_GaidenWhiteMagicPageCommon, HbPopuplate_GaidenWhiteMagicPageCommon
};

static _DECL_INFO RText_Page5_White_Magic_3 = {
    &RText_Page5_White_Magic_2, 
    &RText_Page5_White_Magic_4, 
    &RText_Name, 
    &RText_Page5_Black_Magic_3,
    0x66, 0x48, 3,
    HbRedirect_GaidenWhiteMagicPageCommon, HbPopuplate_GaidenWhiteMagicPageCommon
};

static _DECL_INFO RText_Page5_White_Magic_4 = {
    &RText_Page5_White_Magic_3, 
    &RText_Page5_White_Magic_5, 
    &RText_Name, 
    &RText_Page5_Black_Magic_4,
    0x66, 0x58, 4,
    HbRedirect_GaidenWhiteMagicPageCommon, HbPopuplate_GaidenWhiteMagicPageCommon
};

static _DECL_INFO RText_Page5_White_Magic_5 = {
    &RText_Page5_White_Magic_4, 
    &RText_Page5_White_Magic_Title, 
    &RText_Name, 
    &RText_Page5_Black_Magic_5,
    0x66, 0x68, 5,
    HbRedirect_GaidenWhiteMagicPageCommon, HbPopuplate_GaidenWhiteMagicPageCommon
};

/* Black magic */
static _DECL_INFO RText_Page5_Black_Magic_Title = {
    &RText_Page5_Black_Magic_5, 
    &RText_Page5_Black_Magic_1, 
    &RText_Page5_White_Magic_Title, 
    &RText_Name,
    0xA6, 0x18, MSG_STAT_SCREEN_BLACK_MAGIC_DESC,
    NULL, NULL
};

static _DECL_INFO RText_Page5_Black_Magic_1 = {
    &RText_Page5_Black_Magic_Title, 
    &RText_Page5_Black_Magic_2, 
    &RText_Page5_White_Magic_1, 
    &RText_Name,
    0xA6, 0x28, 6,
    HbRedirect_GaidenBlackMagicPageCommon, HbPopulate_GaidenBlackMagicPageCommon
};

static _DECL_INFO RText_Page5_Black_Magic_2 = {
    &RText_Page5_Black_Magic_1,
    &RText_Page5_Black_Magic_3,
    &RText_Page5_White_Magic_2, 
    &RText_Name,
    0xA6, 0x38, 7,
    HbRedirect_GaidenBlackMagicPageCommon, HbPopulate_GaidenBlackMagicPageCommon
};

static _DECL_INFO RText_Page5_Black_Magic_3 = {
    &RText_Page5_Black_Magic_2,
    &RText_Page5_Black_Magic_4,
    &RText_Page5_White_Magic_3,
    &RText_Name,
    0xA6, 0x48, 8,
    HbRedirect_GaidenBlackMagicPageCommon, HbPopulate_GaidenBlackMagicPageCommon
};

static _DECL_INFO RText_Page5_Black_Magic_4 = {
    &RText_Page5_Black_Magic_3,
    &RText_Page5_Black_Magic_5,
    &RText_Page5_White_Magic_4,
    &RText_Name,
    0xA6, 0x58, 9,
    HbRedirect_GaidenBlackMagicPageCommon, HbPopulate_GaidenBlackMagicPageCommon
};

static _DECL_INFO RText_Page5_Black_Magic_5 = {
    &RText_Page5_Black_Magic_4, 
    &RText_Page5_Black_Magic_Title, 
    &RText_Page5_White_Magic_5, 
    &RText_Name,
    0xA6, 0x68, 10,
    HbRedirect_GaidenBlackMagicPageCommon, HbPopulate_GaidenBlackMagicPageCommon
};


/* Page left */
static _DECL_INFO RText_Name = {
    &RText_Hp, &RText_Class, &RText_Page5_Black_Magic_Title, &RText_Page5_White_Magic_Title,
    0x18, 0x50, 0,
    NULL, HbPopulate_SSCharacter
};

static _DECL_INFO RText_Class = {
    &RText_Name, &RText_Level, &RText_Page5_Black_Magic_Title, &RText_Page5_White_Magic_Title,
    0x06, 0x68, 0,
    NULL, HbPopulate_SSClass
};

static _DECL_INFO RText_Level = {
    &RText_Class, &RText_Hp, &RText_Page5_Black_Magic_Title, &RText_Exp,
    0x06, 0x78, 0x542,
    NULL, NULL
};

static _DECL_INFO RText_Exp = {
    &RText_Class, &RText_Hp, &RText_Level, &RText_Page5_White_Magic_Title,
    0x26, 0x78, 0x543,
    NULL, NULL
};

static _DECL_INFO RText_Hp = {
    &RText_Level, &RText_Name, &RText_Page5_Black_Magic_Title, &RText_Page5_White_Magic_Title,
    0x06, 0x88, 0x544,
    NULL, NULL
};
