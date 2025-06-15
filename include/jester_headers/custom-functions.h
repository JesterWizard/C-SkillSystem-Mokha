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

void AddExp_Event(int exp);

static void TransferStatsandExperience(void)
{
    UpdateUnitFromBattle(gActiveUnit, &gBattleActor);
    gActiveUnit->state &= ~US_HIDDEN;       // Remove the hidden state that seems to get randomly set?
    gActiveUnit->state |= US_UNSELECTABLE;  // Ensure the unit cannot be selected again
}

const struct ProcCmd ProcScr_AddExp[] = {
    PROC_CALL(LockGame),
    PROC_WHILE(BattleEventEngineExists),
    PROC_CALL(DeleteBattleAnimInfoThing),
    PROC_SLEEP(1),
    PROC_CALL(MapAnim_DisplayExpBar),
    PROC_YIELD,
    PROC_CALL(UnlockGame),
    PROC_CALL(MapAnim_Cleanup),
    PROC_CALL(TransferStatsandExperience),
    PROC_END
};

static void BattleApplyMiscActionExpGains_Modular(int exp) {
    if (UNIT_FACTION(&gBattleActor.unit) != FACTION_BLUE)
        return;

    if (!CanBattleUnitGainLevels(&gBattleActor))
        return;

    if (gPlaySt.chapterStateBits & PLAY_FLAG_EXTRA_MAP)
        return;

    gBattleActor.expGain = exp;
    gBattleActor.unit.exp += exp;

    CheckBattleUnitLevelUp(&gBattleActor);
}

void AddExp_Event(int exp)
{
    gManimSt.actorCount = 1;
    gManimSt.hp_changing = 0;
    gManimSt.subjectActorId = 0;
    gManimSt.targetActorId = 0;
    InitBattleUnit(&gBattleActor, gActiveUnit);
    BattleApplyMiscActionExpGains_Modular(exp);
    SetupMapBattleAnim(&gBattleActor, &gBattleTarget, gBattleHitArray);
    Proc_StartBlocking(ProcScr_AddExp, PROC_TREE_3);
}

void ForEachUnit_Custom(int x, int y, void(*func)(struct Unit* unit), u8 minRange, u8 maxRange) {
    InitTargets(x, y);

    MapAddInRange(x, y, maxRange, 1);
    MapAddInRange(x, y, minRange, -1);

    ForEachUnitInRange(func);

    return;
}

void TryAddUnitToCustomTargetList(struct Unit* unit) {

    bool isEnemy = gEventSlots[EVT_SLOT_9];

    if (AreUnitsAllied(gSubjectUnit->index, unit->index) && isEnemy) {
        return;
    }

    if (!AreUnitsAllied(gSubjectUnit->index, unit->index) && !isEnemy) {
        return;
    }

    if (unit->state & US_RESCUED) {
        return;
    }

    AddTarget(unit->xPos, unit->yPos, unit->index, 0);

    return;
}

void MakeTargetList(struct Unit* unit) {
    int x = unit->xPos;
    int y = unit->yPos;

    gSubjectUnit = unit;

    u8 minRange = gEventSlots[EVT_SLOT_7];
    u8 maxRange = gEventSlots[EVT_SLOT_8];

    BmMapFill(gBmMapRange, 0);

    ForEachUnit_Custom(x, y, TryAddUnitToCustomTargetList, minRange, maxRange);

    return;
}