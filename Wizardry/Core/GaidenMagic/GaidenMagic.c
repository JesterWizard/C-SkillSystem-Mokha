#include "common-chax.h"
#include "kernel-lib.h"
#include "jester_headers/gaiden-magic.h"

void Gaiden_ResetMagicLists(void)
{
    // Use CpuFill16 to zero out the list and counter for all units.
    // This ensures the 'amt' variable is set to 0, which is critical.
    CpuFill16(0, sWhiteMagicPList, sizeof(sWhiteMagicPList));
    CpuFill16(0, sBlackMagicPList, sizeof(sBlackMagicPList));
}


void Gaiden_AddWhiteMagic(struct Unit * unit, u8 spellId)
{
    u8 pid = UNIT_CHAR_ID(unit);
    struct GaidenMagicSpellList *list = &sWhiteMagicPList[pid];

    /* Make sure the unit we're checking is playable */
    if (UNIT_FACTION(unit) != FACTION_BLUE)
        return;

    // First check if the list is full
    if (list->amt >= GAIDEN_MAGIC_LIST_MAX)
        return;
    
    // Check if the spell is already in the list
    for (int i = 0; i < list->amt; ++i) {
        if (list->list[i] == spellId) {
            return;
        }
    }

    // Add the new spell to the end of the list and increment the counter
    list->list[list->amt++] = spellId;
}


void Gaiden_AddBlackMagic(struct Unit * unit, u8 spellId)
{
    u8 pid = UNIT_CHAR_ID(unit);
    struct GaidenMagicSpellList *list = &sBlackMagicPList[pid];

    /* Make sure the unit we're checking is playable */
    if (UNIT_FACTION(unit) != FACTION_BLUE)
        return;

    // First check if the list is full
    if (list->amt >= GAIDEN_MAGIC_LIST_MAX)
        return;

    // Check if the spell is already in the list
    for (int i = 0; i < list->amt; ++i) {
        if (list->list[i] == spellId) {
            return;
        }
    }

    // Add the new spell to the end of the list and increment the counter
    list->list[list->amt++] = spellId;
}


void Gaiden_RemoveWhiteMagic(struct Unit * unit, u8 spellId)
{
    u8 pid = UNIT_CHAR_ID(unit);
    struct GaidenMagicSpellList *list = &sWhiteMagicPList[pid];

    for (int i = 0; i < list->amt; ++i) {
        if (list->list[i] == spellId) {
            // Found the spell, now shift all subsequent elements left
            for (int j = i; j < list->amt - 1; ++j) {
                list->list[j] = list->list[j+1];
            }
            // Decrement the count of spells
            list->amt--;
            // Spell removed, exit the loop
            return;
        }
    }
}


void Gaiden_RemoveBlackMagic(struct Unit * unit, u8 spellId)
{
    u8 pid = UNIT_CHAR_ID(unit);
    struct GaidenMagicSpellList *list = &sBlackMagicPList[pid];

    for (int i = 0; i < list->amt; ++i) {
        if (list->list[i] == spellId) {
            // Found the spell, now shift all subsequent elements left
            for (int j = i; j < list->amt - 1; ++j) {
                list->list[j] = list->list[j+1];
            }
            // Decrement the count of spells
            list->amt--;
            // Spell removed, exit the loop
            return;
        }
    }
}


bool Gaiden_CheckWhiteMagic(struct Unit * unit, u8 spellId)
{
    u8 pid = UNIT_CHAR_ID(unit);
    struct GaidenMagicSpellList *list = &sWhiteMagicPList[pid];

    for (int i = 0; i < list->amt; ++i) {
        if (list->list[i] == spellId) {
            return true;
        }
    }
    return false;
}


bool Gaiden_CheckBlackMagic(struct Unit * unit, u8 spellId)
{
    u8 pid = UNIT_CHAR_ID(unit);
    struct GaidenMagicSpellList *list = &sBlackMagicPList[pid];

    for (int i = 0; i < list->amt; ++i) {
        if (list->list[i] == spellId) {
            return true;
        }
    }
    return false;
}


struct GaidenMagicSpellList * Gaiden_GetUnitWhiteMagicList(struct Unit *unit)
{
    u8 pid = UNIT_CHAR_ID(unit);
    return &sWhiteMagicPList[pid];
}

struct GaidenMagicSpellList * Gaiden_GetUnitBlackMagicList(struct Unit *unit)
{
    u8 pid = UNIT_CHAR_ID(unit);
    return &sBlackMagicPList[pid];
}
