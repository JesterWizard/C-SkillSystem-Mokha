#pragma once
#include "common-chax.h"

bool isWeaponTriangleAdvantage(int attackerWeapon, int defenderWeapon); // weapon type
bool weaponHasSpecialEffect(int weaponAttributes);  // weaponID
int findMax(u8 *array, int size);  // find max value in array and return index

int RandSkill(int id, struct Unit * unit);

static const u8 monsterClasses[25] = {
    CLASS_MANAKETE,
    CLASS_MANAKETE_MYRRH,
    CLASS_PHANTOM,
    CLASS_REVENANT,
    CLASS_ENTOMBED,
    CLASS_BONEWALKER,
    CLASS_BONEWALKER_BOW,
    CLASS_WIGHT,
    CLASS_WIGHT_BOW,
    CLASS_BAEL,
    CLASS_ELDER_BAEL,
    CLASS_CYCLOPS,
    CLASS_MAUTHEDOOG,
    CLASS_GWYLLGI,
    CLASS_TARVOS,
    CLASS_MAELDUIN,
    CLASS_MOGALL,
    CLASS_ARCH_MOGALL,
    CLASS_GORGON,
    CLASS_GORGONEGG,
    CLASS_GORGONEGG2,
    CLASS_GARGOYLE,
    CLASS_DEATHGOYLE,
    CLASS_DRACO_ZOMBIE,
    CLASS_DEMON_KING,
};

#define MONSTER_CLASS_COUNT (sizeof(monsterClasses) / sizeof(monsterClasses[0]))

bool isMonsterClass(int classId);
bool isMonsterClass(int classId) {
    for (unsigned i = 0; i < MONSTER_CLASS_COUNT; i++) {
        if (monsterClasses[i] == classId) {
            return true;
        }
    }
    return false;
}