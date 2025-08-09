#pragma once

#include "common-chax.h"
#include "bwl.h"

#ifndef CONFIG_UNIT_AMT_ALLY
    #define CONFIG_UNIT_AMT_ALLY 51
#endif

#ifndef CONFIG_UNIT_AMT_ENEMY
    #define CONFIG_UNIT_AMT_ENEMY 50
#endif

#ifndef CONFIG_UNIT_AMT_NPC
    #define CONFIG_UNIT_AMT_NPC 8
#endif

#define GAIDEN_MAGIC_LIST_MAX 5

struct GaidenMagicSpellList {
    u8 list[GAIDEN_MAGIC_LIST_MAX];
    u8 amt;
};

extern struct GaidenMagicSpellList sWhiteMagicPList[CONFIG_UNIT_AMT_ALLY];
extern struct GaidenMagicSpellList sBlackMagicPList[CONFIG_UNIT_AMT_ALLY];

void Gaiden_ResetMagicLists(void);

void Gaiden_AddWhiteMagic(struct Unit * unit, u8 spellId);
void Gaiden_AddBlackMagic(struct Unit * unit, u8 spellId);

void Gaiden_RemoveWhiteMagic(struct Unit * unit, u8 spellId);
void Gaiden_RemoveBlackMagic(struct Unit * unit, u8 spellId);

bool Gaiden_CheckWhiteMagic(struct Unit * unit, u8 spellId);
bool Gaiden_CheckBlackMagic(struct Unit * unit, u8 spellId);

struct GaidenMagicSpellList * Gaiden_GetUnitWhiteMagicList(struct Unit *unit);
struct GaidenMagicSpellList * Gaiden_GetUnitBlackMagicList(struct Unit *unit);
