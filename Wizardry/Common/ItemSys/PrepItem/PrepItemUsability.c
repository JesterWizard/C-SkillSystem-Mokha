#include "common-chax.h"
#include "item-sys.h"

LYN_REPLACE_CHECK(CanUnitUseItemPrepScreen);
bool CanUnitUseItemPrepScreen(struct Unit * unit, int item)
{
    PrepItemUsabilityFunc_t it = gPrepItemUsabilityFuncs[ITEM_INDEX(item)];

    if (it)
        return it(unit, item);

    return false;
}

bool CanUnitUseMetiStome(struct Unit * unit, int item)
{
    if (unit->state & US_GROWTH_BOOST)
        return false;

    return true;
}

LYN_REPLACE_CHECK(CanUnitUseItem);
s8 CanUnitUseItem(struct Unit* unit, int item)
{
    if ((GetItemAttributes(item) & IA_STAFF) && !CanUnitUseStaff(unit, item))
        return FALSE;

    switch (GetItemIndex(item))
    {

    case ITEM_STAFF_HEAL:
    case ITEM_STAFF_MEND:
    case ITEM_STAFF_RECOVER:
        return HasSelectTarget(unit, MakeTargetListForAdjacentHeal);

    case ITEM_STAFF_PHYSIC:
        return HasSelectTarget(unit, MakeTargetListForRangedHeal);

    case ITEM_STAFF_FORTIFY:
        return HasSelectTarget(unit, MakeTargetListForRangedHeal);

    case ITEM_STAFF_RESTORE:
        return HasSelectTarget(unit, MakeTargetListForRestore);

    case ITEM_STAFF_RESCUE:
        return HasSelectTarget(unit, MakeTargetListForRescueStaff);

    case CONFIG_ITEM_INDEX_FORCE_STAFF:
    case CONFIG_ITEM_INDEX_TEMPEST_STAFF:
    case CONFIG_ITEM_INDEX_ACUITY_STAFF:
    case CONFIG_ITEM_INDEX_SPRINT_STAFF:
    case CONFIG_ITEM_INDEX_FORTUNE_STAFF:
    case CONFIG_ITEM_INDEX_IRON_STAFF:
    case ITEM_STAFF_BARRIER:
        return HasSelectTarget(unit, MakeTargetListForBarrier);

    case ITEM_STAFF_SILENCE:
        return HasSelectTarget(unit, MakeTargetListForSilence);

    case CONFIG_ITEM_INDEX_SLOW_STAFF:
    case ITEM_STAFF_SLEEP:
        return HasSelectTarget(unit, MakeTargetListForSleep);

    case ITEM_STAFF_BERSERK:
        return HasSelectTarget(unit, MakeTargetListForBerserk);

    case ITEM_STAFF_WARP:
        return HasSelectTarget(unit, MakeTargetListForWarp);

    case ITEM_STAFF_REPAIR:
        return HasSelectTarget(unit, MakeTargetListForHammerne);

    case ITEM_STAFF_UNLOCK:
        return HasSelectTarget(unit, MakeTargetListForUnlock);

    case ITEM_BOOSTER_HP:
    case ITEM_BOOSTER_POW:
    case ITEM_BOOSTER_SKL:
    case ITEM_BOOSTER_SPD:
    case ITEM_BOOSTER_LCK:
    case ITEM_BOOSTER_DEF:
    case ITEM_BOOSTER_RES:
    case ITEM_BOOSTER_MOV:
    case ITEM_BOOSTER_CON:
        return CanUnitUseStatGainItem(unit, item);

    case ITEM_HEROCREST:
    case ITEM_KNIGHTCREST:
    case ITEM_ORIONSBOLT:
    case ITEM_ELYSIANWHIP:
    case ITEM_GUIDINGRING:
    case ITEM_MASTERSEAL:
    case ITEM_HEAVENSEAL:
    case ITEM_OCEANSEAL:
    case ITEM_LUNARBRACE:
    case ITEM_SOLARBRACE:
    // case ITEM_UNK_C1:
        return CanUnitUsePromotionItem(unit, item);

    case ITEM_VULNERARY:
    case ITEM_ELIXIR:
    case ITEM_VULNERARY_2:
        return CanUnitUseHealItem(unit);

    case ITEM_PUREWATER:
        return CanUnitUsePureWaterItem(unit);

    case ITEM_TORCH:
        return CanUnitUseTorchItem(unit);

    case ITEM_ANTITOXIN:
        return CanUnitUseAntitoxinItem(unit);

    case ITEM_CHESTKEY:
    case ITEM_CHESTKEY_BUNDLE:
        return CanUnitUseChestKeyItem(unit);

    case ITEM_DOORKEY:
        return CanUnitUseDoorKeyItem(unit);

    case ITEM_LOCKPICK:
        return CanUnitUseLockpickItem(unit);

    case ITEM_STAFF_LATONA:
        return HasSelectTarget(unit, MakeTargetListForLatona);

    case CONFIG_ITEM_INDEX_MINE_STAFF:
    case ITEM_MINE:
        return HasSelectTarget(unit, MakeTargetListForMine);

    case CONFIG_ITEM_INDEX_RUNE_STAFF:
    case ITEM_LIGHTRUNE:
        return HasSelectTarget(unit, MakeTargetListForLightRune);

    case ITEM_STAFF_TORCH:
        return gPlaySt.chapterVisionRange != 0;

    case ITEM_FILLAS_MIGHT:
    case ITEM_NINISS_GRACE:
    case ITEM_THORS_IRE:
    case ITEM_SETS_LITANY:
        return HasSelectTarget(unit, MakeTargetListForDanceRing);

    case ITEM_METISSTOME:
        if (unit->state & US_GROWTH_BOOST)
            return FALSE;

        return TRUE;

    case ITEM_JUNAFRUIT:
        return CanUnitUseFruitItem(unit);

    default:
        return FALSE;

    }
}

LYN_REPLACE_CHECK(DoUseBarrierStaff);
void DoUseBarrierStaff(struct Unit* unit)
{
    MakeTargetListForBarrier(unit);

    char * str = "NULL";

    int itemId = GetItemIndex(unit->items[0]);

    switch (itemId)
    {
    case CONFIG_ITEM_INDEX_FORCE_STAFF:
        str = "Select which character's strength to bolster";
        break;

    case CONFIG_ITEM_INDEX_TEMPEST_STAFF:
        str = "Select which character's magic to bolster";
        break;

    case CONFIG_ITEM_INDEX_ACUITY_STAFF:
        str = "Select which character's skill to bolster";
        break;

    case CONFIG_ITEM_INDEX_SPRINT_STAFF:
        str = "Select which character's speed to bolster";
        break;

    case CONFIG_ITEM_INDEX_FORTUNE_STAFF:
        str = "Select which character's luck to bolster";
        break;

    case CONFIG_ITEM_INDEX_IRON_STAFF:
        str = "Select which character's defense to bolster";
        break;

    case ITEM_STAFF_BARRIER:
        str = "Select which character's resistance to bolster";
        break;

    default:
        break;
    }

    BmMapFill(gBmMapMovement, -1);

    StartSubtitleHelp(NewTargetSelection(&gSelectInfo_Barrier), str);
}

LYN_REPLACE_CHECK(DoItemUse);
void DoItemUse(struct Unit* unit, int item)
{
    ClearBg0Bg1();
    EndFaceById(0);

    switch (GetItemIndex(item))
    {

    case ITEM_STAFF_HEAL:
    case ITEM_STAFF_MEND:
    case ITEM_STAFF_RECOVER:
        DoUseHealStaff(unit, MakeTargetListForAdjacentHeal);
        break;

    case ITEM_STAFF_PHYSIC:
        DoUseHealStaff(unit, MakeTargetListForRangedHeal);
        break;

    case ITEM_STAFF_RESCUE:
        DoUseRescueStaff(unit, MakeTargetListForRescueStaff);
        break;

    case ITEM_STAFF_RESTORE:
        DoUseRestoreStaff(unit, MakeTargetListForRestore);
        break;

    case ITEM_STAFF_SILENCE:
        DoUseAttackStaff(unit, MakeTargetListForSilence);
        break;

    case CONFIG_ITEM_INDEX_SLOW_STAFF:
    case ITEM_STAFF_SLEEP:
        DoUseAttackStaff(unit, MakeTargetListForSleep);
        break;

    case ITEM_STAFF_BERSERK:
        DoUseAttackStaff(unit, MakeTargetListForBerserk);
        break;

    case CONFIG_ITEM_INDEX_FORCE_STAFF:
    case CONFIG_ITEM_INDEX_TEMPEST_STAFF:
    case CONFIG_ITEM_INDEX_ACUITY_STAFF:
    case CONFIG_ITEM_INDEX_SPRINT_STAFF:
    case CONFIG_ITEM_INDEX_FORTUNE_STAFF:
    case CONFIG_ITEM_INDEX_IRON_STAFF:
    case ITEM_STAFF_BARRIER:
        DoUseBarrierStaff(unit);
        break;

    case ITEM_STAFF_UNLOCK:
        DoUsePutTrap(unit, MakeTargetListForUnlock, 0x87A); // TODO: msgid "Select a door to open."
        break;

    case ITEM_STAFF_WARP:
        DoUseWarpStaff(unit);
        break;

    case ITEM_STAFF_REPAIR:
        DoUseRepairStaff(unit);
        break;

    case ITEM_STAFF_FORTIFY:
    case ITEM_STAFF_LATONA:
        SetStaffUseAction(unit);
        break;

    case CONFIG_ITEM_INDEX_MINE_STAFF:
    case ITEM_MINE:
        DoUsePutTrap(unit, MakeTargetListForMine, 0x87D); // TODO: msgid "Select an area to trap."
        break;

    case CONFIG_ITEM_INDEX_RUNE_STAFF:
    case ITEM_LIGHTRUNE:
        DoUsePutTrap(unit, MakeTargetListForLightRune, 0x87E); // TODO: msgid "Select an area to trap."
        break;

    case ITEM_STAFF_TORCH:
        DoUseTorchStaff(unit);
        break;

    case ITEM_FILLAS_MIGHT:
    case ITEM_NINISS_GRACE:
    case ITEM_THORS_IRE:
    case ITEM_SETS_LITANY:
        DoUseSpecialDance(unit, MakeTargetListForDanceRing, 0x87F); // TODO: msgid "Select a character to bless."
        break;

    default:
        SetItemUseAction(unit);
        break;

    }
}