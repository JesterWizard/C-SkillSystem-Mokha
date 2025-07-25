#include "common-chax.h"
#include "battle-system.h"
#include "status-getter.h"
#include "weapon-range.h"
#include "skill-system.h"
#include "constants/skills.h"

LYN_REPLACE_CHECK(AiReachesByBirdsEyeDistance);
bool AiReachesByBirdsEyeDistance(struct Unit *unit, struct Unit *other, u16 item)
{
    int distance = RECT_DISTANCE(unit->xPos, unit->yPos, other->xPos, other->yPos);
    if (distance <= MovGetter(unit) + GetItemMaxRangeRework(item, unit))
        return true;

    return false;
}

LYN_REPLACE_CHECK(AiCouldReachByBirdsEyeDistance);
bool AiCouldReachByBirdsEyeDistance(struct Unit *unit, struct Unit *other, u16 item)
{

    int distance = RECT_DISTANCE(unit->xPos, unit->yPos, other->xPos, other->yPos);
    if (distance <= MovGetter(unit) + MovGetter(other) + GetItemMaxRangeRework(item, unit))
        return true;

    return false;
}

LYN_REPLACE_CHECK(GetUnitWeaponReachBits);
int GetUnitWeaponReachBits(struct Unit *unit, int slot)
{
    int i, item, result = 0;

    switch (slot)
    {
    case -1:
        for (i = 0; (i < UNIT_ITEM_COUNT) && (item = unit->items[i]); ++i)
            if (CanUnitUseWeapon(unit, item))
                result |= GetItemReachBitsRework(item, unit);
        break;

    default:
        item = GetItemFromSlot(unit, slot);
        result = GetItemReachBitsRework(item, unit);
        break;
    }

    return result;
}

LYN_REPLACE_CHECK(GetUnitItemUseReachBits);
int GetUnitItemUseReachBits(struct Unit *unit, int slot)
{
    int i;
    u16 item;
    u32 mask = 0;

    switch (slot)
    {
    case -1:
        for (i = 0; i < UNIT_ITEM_COUNT; i++)
        {
            item = unit->items[i];

            if (ITEM_INDEX(item) > 0 && CanUnitUseItem(unit, item))
                mask |= GetItemReachBitsRework(item, unit);
        }
        break;

    default:
        item = GetItemFromSlot(unit, slot);
        mask = REACH_NONE;
        if (CanUnitUseItem(unit, item))
            mask = GetItemReachBitsRework(item, unit);

        break;
    }
    return mask;
}

LYN_REPLACE_CHECK(GetUnitStaffReachBits);
int GetUnitStaffReachBits(struct Unit *unit)
{
    int i;
    u16 item;
    u32 mask = 0;

    for (i = 0; i < UNIT_ITEM_COUNT; i++)
    {
        item = unit->items[i];

        if (ITEM_INDEX(item) != 0 && CanUnitUseStaff(unit, item))
            mask |= GetItemReachBitsRework(item, unit);
    }
    return mask;
}

LYN_REPLACE_CHECK(AiFillReversedAttackRangeMap);
void AiFillReversedAttackRangeMap(struct Unit *unit, u16 item)
{
    BmMapFill(gBmMapRange, 0);

    // <!> not unit! here is target! we should judge for AI!
    AddMap(unit->xPos, unit->yPos, GetItemReachBitsRework(item, gActiveUnit), 1, 0);
}

LYN_REPLACE_CHECK(AiFloodMovementAndRange);
void AiFloodMovementAndRange(struct Unit *unit, u16 move, u16 item)
{
    int ix, iy;
    u32 mask;

    SetWorkingMoveCosts(GetUnitMovementCost(unit));
    SetWorkingBmMap(gBmMapMovement);
    GenerateMovementMap(unit->xPos, unit->yPos, move, unit->index);
    BmMapFill(gBmMapRange, 0);

    mask = GetItemReachBitsRework(item, unit);

    for (iy = gBmMapSize.y - 1; iy >= 0; iy--)
    {
        for (ix = gBmMapSize.x - 1; ix >= 0; ix--)
        {
            if (gBmMapMovement[iy][ix] > MAP_MOVEMENT_MAX)
                continue;

            AddMap(unit->xPos, unit->yPos, mask, 1, 0);
        }
    }
}

LYN_REPLACE_CHECK(AiGetInRangeCombatPositionScoreComponent);
int AiGetInRangeCombatPositionScoreComponent(int x, int y, struct Unit *unit)
{
    int dist = RECT_DISTANCE(unit->xPos, unit->yPos, x, y);
    u16 item = GetUnitEquippedWeapon(unit);

    if (ITEM_INDEX(item) == 0)
        return 0;

    if (dist > GetItemMaxRangeRework(item, unit))
        if (dist < GetItemMinRangeRework(item, unit))
            return 50;

    return 0;
}

LYN_REPLACE_CHECK(DisplayUnitEffectRange);
void DisplayUnitEffectRange(struct Unit *unit)
{
    u32 movelimitv_flag = MOVLIMITV_MMAP_BLUE;

    GenerateUnitMovementMapExt(gActiveUnit, MovGetter(gActiveUnit) - gActionData.moveCount);

    if (!(gActiveUnit->state & US_CANTOING))
    {
        BmMapFill(gBmMapOther, 0);

        if (UnitHasMagicRank(unit))
            GenerateMagicSealMap(1);

        BmMapFill(gBmMapRange, 0);

        switch (GetUnitWeaponUsabilityBits(gActiveUnit))
        {
        case (UNIT_USEBIT_STAFF | UNIT_USEBIT_WEAPON):
            if (gBmSt.swapActionRangeCount & 1)
            {
                GenerateUnitCompleteStaffRange(gActiveUnit);
                movelimitv_flag = MOVLIMITV_MMAP_BLUE | MOVLIMITV_RMAP_GREEN;
            }
            else
            {
                GenerateUnitCompleteAttackRange(gActiveUnit);
                movelimitv_flag = MOVLIMITV_MMAP_BLUE | MOVLIMITV_RMAP_RED;
            }
            break;

        case UNIT_USEBIT_STAFF:
            GenerateUnitCompleteStaffRange(gActiveUnit);
            movelimitv_flag = MOVLIMITV_MMAP_BLUE | MOVLIMITV_RMAP_GREEN;
            break;
        case UNIT_USEBIT_WEAPON:
            GenerateUnitCompleteAttackRange(gActiveUnit);
            movelimitv_flag = MOVLIMITV_MMAP_BLUE | MOVLIMITV_RMAP_RED;
            break;
        } /* switch */
    }

#if defined(SID_UnarmedCombat) && (COMMON_SKILL_VALID(SID_UnarmedCombat))
    if (SkillTester(gActiveUnit, SID_UnarmedCombat))
    {
        if (!GetUnitEquippedWeapon(gActiveUnit))
        {
            GenerateUnitCompleteAttackRange(gActiveUnit);
            movelimitv_flag = MOVLIMITV_MMAP_BLUE | MOVLIMITV_RMAP_RED;
        }
    }
#endif

    DisplayMoveRangeGraphics(movelimitv_flag);
}

LYN_REPLACE_CHECK(GenerateUnitMovementMap);
void GenerateUnitMovementMap(struct Unit *unit)
{
    SetWorkingMoveCosts(GetUnitMovementCost(unit));
    SetWorkingBmMap(gBmMapMovement);

    GenerateMovementMap(unit->xPos, unit->yPos, MovGetter(unit), unit->index);
}

LYN_REPLACE_CHECK(GenerateUnitCompleteAttackRange);
void GenerateUnitCompleteAttackRange(struct Unit *unit)
{
    int ix, iy;

#ifdef CONFIG_FASTER_MAP_RANGE
    int min, max;
#else
    u32 mask;
#endif

    BmMapFill(gBmMapRange, 0);

#ifdef CONFIG_FASTER_MAP_RANGE
    min = GetUnitMinRange(unit);
    max = GetUnitMaxRange(unit);

#else
    mask = GetUnitWeaponReachBits(unit, -1);
#endif

    if (UNIT_CATTRIBUTES(unit) & CA_BALLISTAE)
    {
        u16 item = GetBallistaItemAt(unit->xPos, unit->yPos);
        if (0 != item)
        {
#ifdef CONFIG_FASTER_MAP_RANGE
            int _max = GetItemMaxRangeRework(item, unit);
            int _min = GetItemMinRangeRework(item, unit);

            if (_max > max)
                max = _max;
            if (_min < min)
                min = _min;
#else
            mask |= GetItemReachBitsRework(item, unit);
#endif
        }
    }

    for (iy = 0; iy < gBmMapSize.y; iy++)
    {
        for (ix = 0; ix < gBmMapSize.x; ix++)
        {
            if (gBmMapMovement[iy][ix] > MAP_MOVEMENT_MAX)
                continue;

            if (gBmMapUnit[iy][ix])
                continue;

            if (gBmMapOther[iy][ix])
                continue;

#ifdef CONFIG_FASTER_MAP_RANGE
            MapAddInBoundedRange(ix, iy, min, max);
#else
            AddMap(ix, iy, mask, 1, 0);
#endif
        }
    }
    SetWorkingBmMap(gBmMapMovement);
}

LYN_REPLACE_CHECK(GenerateUnitStandingReachRange);
void GenerateUnitStandingReachRange(struct Unit *unit, int mask)
{
    BmMapFill(gBmMapRange, 0);
    AddMap(unit->xPos, unit->yPos, mask, 1, 0);
}

LYN_REPLACE_CHECK(GenerateUnitCompleteStaffRange);
void GenerateUnitCompleteStaffRange(struct Unit *unit)
{
    int ix, iy;
    u32 mask = GetUnitStaffReachBits(unit);

    BmMapFill(gBmMapRange, 0);

    if (UNIT_CATTRIBUTES(unit) & CA_BALLISTAE)
    {
        u16 item = GetBallistaItemAt(unit->xPos, unit->yPos);
        if (0 != item)
            mask |= GetItemReachBitsRework(item, unit);
    }

    for (iy = 0; iy < gBmMapSize.y; iy++)
    {
        for (ix = 0; ix < gBmMapSize.x; ix++)
        {
            if (gBmMapMovement[iy][ix] > MAP_MOVEMENT_MAX)
                continue;

            if (gBmMapUnit[iy][ix])
                continue;

            if (gBmMapOther[iy][ix])
                continue;

            AddMap(ix, iy, mask, 1, 0);
        }
    }
    SetWorkingBmMap(gBmMapMovement);
}

LYN_REPLACE_CHECK(GenerateDangerZoneRange);
void GenerateDangerZoneRange(bool boolDisplayStaffRange)
{
    int i, enemyFaction;
    int hasMagicRank, prevHasMagicRank;
    u8 savedUnitId;

    prevHasMagicRank = -1;

    BmMapFill(gBmMapRange, 0);

    enemyFaction = GetNonActiveFaction();

    for (i = enemyFaction + 1; i < enemyFaction + 0x80; ++i)
    {
        struct Unit *unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
            continue; // not a unit

        if (boolDisplayStaffRange && !UnitHasMagicRank(unit))
            continue; // no magic in magic range mode

        if (gPlaySt.chapterVisionRange && (gBmMapFog[unit->yPos][unit->xPos] == 0))
            continue; // in the fog

        if (unit->state & US_UNDER_A_ROOF)
            continue; // under a roof

        // Fill movement map for unit
        GenerateUnitMovementMapExt(unit, MovGetter(unit));

        savedUnitId = gBmMapUnit[unit->yPos][unit->xPos];
        gBmMapUnit[unit->yPos][unit->xPos] = 0;

        hasMagicRank = UnitHasMagicRank(unit);

        if (prevHasMagicRank != hasMagicRank)
        {
            BmMapFill(gBmMapOther, 0);

            if (hasMagicRank)
                GenerateMagicSealMap(1);

            prevHasMagicRank = hasMagicRank;
        }

        SetWorkingBmMap(gBmMapRange);

        // Apply unit's range to range map

        if (boolDisplayStaffRange)
            GenerateUnitCompleteStaffRange(unit);
        else
            GenerateUnitCompleteAttackRange(unit);

        gBmMapUnit[unit->yPos][unit->xPos] = savedUnitId;
    }
}

LYN_REPLACE_CHECK(FillMovementAndRangeMapForItem);
void FillMovementAndRangeMapForItem(struct Unit *unit, u16 item)
{
    int ix, iy;

    GenerateUnitMovementMap(unit);
    BmMapFill(gBmMapRange, 0);

    for (iy = 0; iy < gBmMapSize.y; iy++)
    {
        for (ix = 0; ix < gBmMapSize.x; ix++)
        {
            if (gBmMapMovement[iy][ix] > MAP_MOVEMENT_MAX)
                continue;

            AddMap(ix, iy, GetItemReachBitsRework(item, unit), 1, 0);
        }
    }
}

LYN_REPLACE_CHECK(sub_803B678);
void sub_803B678(struct Unit *unit, u16 item)
{
    int ix, iy;

    GenerateUnitMovementMap(unit);
    BmMapFill(gBmMapRange, 0);

    for (iy = gBmMapSize.y - 1; iy >= 0; iy--)
    {
        for (ix = gBmMapSize.x - 1; ix >= 0; ix--)
        {
            if (gBmMapMovement[iy][ix] > MAP_MOVEMENT_MAX)
                continue;

            AddMap(ix, iy, GetItemReachBitsRework(item, unit), 1, 0);
        }
    }
}

LYN_REPLACE_CHECK(SetupUnitHealStaffAIFlags);
void SetupUnitHealStaffAIFlags(struct Unit *unit, u16 item)
{
    int flags = 0;

    if ((GetItemAttributes(item) & IA_WEAPON) && (GetItemMaxRangeRework(item, unit) > 1))
        flags = AI_UNIT_FLAG_6;

    switch (GetItemUseEffect(item))
    {
    case 0x01:
    case 0x02:
    case 0x03:
    case 0x04:
    case 0x05:
    case 0x21:
    case 0x22:
        flags |= AI_UNIT_FLAG_2;
        break;
    }

    unit->aiFlags |= flags;
}

LYN_REPLACE_CHECK(SetWorkingMoveCosts);
void SetWorkingMoveCosts(const s8 mct[])
{
    int i;

    /**
     * Right now these skills interfere with each other as
     * they all read the tile table seperately.
     * So a short term fix is to return after finishing 
     * applying the first skill here the unit has.
     * I'll look into a better way to do it at some point.
     */


#if (defined(SID_Ruinator) && COMMON_SKILL_VALID(SID_Ruinator))
     if (SkillTester(gActiveUnit, SID_Ruinator))
     {
         for (i = 0; i < TERRAIN_COUNT; ++i)
         {
             if (i == TERRAIN_TILE_00)
                 gWorkingTerrainMoveCosts[i] = 1;
             else
                 gWorkingTerrainMoveCosts[i] = mct[i];
         }
         return;
     }
#endif

#if (defined(SID_PhaseShift) && COMMON_SKILL_VALID(SID_PhaseShift))
     if (SkillTester(gActiveUnit, SID_PhaseShift))
     {
         for (i = 0; i < TERRAIN_COUNT; ++i)
         {
             if (i == TERRAIN_WALL_1A || i == TERRAIN_WALL_1B)
                 gWorkingTerrainMoveCosts[i] = 1;
             else
                 gWorkingTerrainMoveCosts[i] = mct[i];
         }
         return;
     }
 #endif

#if (defined(SID_Acrobat) && COMMON_SKILL_VALID(SID_Acrobat))
    if (SkillTester(gActiveUnit, SID_Acrobat))
    {
        for (i = 0; i < TERRAIN_COUNT; ++i)
        {
            if (mct[i] > 1)
                gWorkingTerrainMoveCosts[i] = 1;
            else
                gWorkingTerrainMoveCosts[i] = mct[i];
        }
        return;
    }
#endif

#if (defined(SID_WaterWalkingPlus) && COMMON_SKILL_VALID(SID_WaterWalkingPlus))
    if (SkillTester(gActiveUnit, SID_WaterWalkingPlus))
    {
        for (i = 0; i < TERRAIN_COUNT; ++i)
        {
            if (i == TERRAIN_WATER || i == TERRAIN_RIVER || i == TERRAIN_SEA || i == TERRAIN_LAKE)
                gWorkingTerrainMoveCosts[i] = 1;
            else
                gWorkingTerrainMoveCosts[i] = mct[i];
        }
        return;
    }
#endif

#if (defined(SID_WaterWalking) && COMMON_SKILL_VALID(SID_WaterWalking))
    if (SkillTester(gActiveUnit, SID_WaterWalking))
    {
        for (i = 0; i < TERRAIN_COUNT; ++i)
        {
            if (i == TERRAIN_WATER || i == TERRAIN_RIVER || i == TERRAIN_SEA || i == TERRAIN_LAKE)
                gWorkingTerrainMoveCosts[i] = 3;
            else
                gWorkingTerrainMoveCosts[i] = mct[i];
        }
        return;
    }
#endif

#if (defined(SID_MountainClimberPlus) && COMMON_SKILL_VALID(SID_MountainClimberPlus))
    if (SkillTester(gActiveUnit, SID_MountainClimberPlus))
    {
        for (i = 0; i < TERRAIN_COUNT; ++i)
        {
            if (i == TERRAIN_MOUNTAIN || i == TERRAIN_PEAK)
                gWorkingTerrainMoveCosts[i] = 1;
            else
                gWorkingTerrainMoveCosts[i] = mct[i];
        }
        return;
    }
#endif

#if (defined(SID_MountainClimber) && COMMON_SKILL_VALID(SID_MountainClimber))
    if (SkillTester(gActiveUnit, SID_MountainClimber))
    {
        for (i = 0; i < TERRAIN_COUNT; ++i)
        {
            if (i == TERRAIN_MOUNTAIN || i == TERRAIN_PEAK)
                gWorkingTerrainMoveCosts[i] = 3;
            else
                gWorkingTerrainMoveCosts[i] = mct[i];
        }
        return;
    }
#endif

#if (defined(SID_DesertTiger) && COMMON_SKILL_VALID(SID_DesertTiger))
    if (SkillTester(gActiveUnit, SID_DesertTiger))
    {
        for (i = 0; i < TERRAIN_COUNT; ++i)
        {
            if (i == TERRAIN_SAND)
                gWorkingTerrainMoveCosts[i] = 1;
            else
                gWorkingTerrainMoveCosts[i] = mct[i];
        }
        return;
    }
#endif

    for (i = 0; i < TERRAIN_COUNT; ++i)
        gWorkingTerrainMoveCosts[i] = mct[i];

}