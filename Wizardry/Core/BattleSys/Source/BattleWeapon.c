#include "common-chax.h"
#include "skill-system.h"
#include "combat-art.h"
#include "debuff.h"
#include "battle-system.h"
#include "weapon-range.h"
#include "weapon-lockex.h"
#include "gaiden-magic.h"
#include "constants/items.h"
#include "constants/skills.h"
#include "constants/combat-arts.h"

int GetItemFromSlot(struct Unit *unit, int slot)
{
	switch (slot) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		return unit->items[slot];
<<<<<<< HEAD

	case BU_ISLOT_5:
		return gBmSt.um_tmp_item;

	case BU_ISLOT_ARENA_PLAYER:
		return gArenaState.playerWeapon;

	case BU_ISLOT_ARENA_OPPONENT:
		return gArenaState.opponentWeapon;

	case BU_ISLOT_BALLISTA:
		return GetBallistaItemAt(unit->xPos, unit->yPos);

	case CHAX_BUISLOT_GAIDEN_BMAG1 ... CHAX_BUISLOT_GAIDEN_BMAG7:
	case CHAX_BUISLOT_GAIDEN_WMAG1 ... CHAX_BUISLOT_GAIDEN_WMAG7:
		return MakeNewItem(GetGaidenMagicItem(unit, slot));

	/* reserved */
	case CHAX_BUISLOT_THREEHOUSES_BMAG1 ... CHAX_BUISLOT_THREEHOUSES_BMAG7:
	case CHAX_BUISLOT_THREEHOUSES_WMAG1 ... CHAX_BUISLOT_THREEHOUSES_WMAG7:
	case CHAX_BUISLOT_ENGAGE_WEAPON1    ... CHAX_BUISLOT_ENGAGE_WEAPON7:
	case -1:
	default:
		return 0;
	}
}

STATIC_DECLAR void SetBattleUnitWeaponVanilla(struct BattleUnit *bu, int itemSlot)
{

    // if(GetUnitStatusIndex(GetUnit(bu->unit.index)) == NEW_UNIT_STATUS_BREAK)
    // {
    //     bu->canCounter = false;
    //     return;
    // }

    if (itemSlot == BU_ISLOT_AUTO)
        itemSlot = GetUnitEquippedWeaponSlot(&bu->unit);
=======
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc

	case BU_ISLOT_5:
		return gBmSt.um_tmp_item;

	case BU_ISLOT_ARENA_PLAYER:
		return gArenaState.playerWeapon;

<<<<<<< HEAD
    switch (itemSlot)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        bu->weaponSlotIndex = itemSlot;
        bu->weapon = bu->unit.items[bu->weaponSlotIndex];
        break;
=======
	case BU_ISLOT_ARENA_OPPONENT:
		return gArenaState.opponentWeapon;
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc

	case BU_ISLOT_BALLISTA:
		return GetBallistaItemAt(unit->xPos, unit->yPos);

	case CHAX_BUISLOT_GAIDEN_BMAG1 ... CHAX_BUISLOT_GAIDEN_BMAG7:
	case CHAX_BUISLOT_GAIDEN_WMAG1 ... CHAX_BUISLOT_GAIDEN_WMAG7:
		return MakeNewItem(GetGaidenMagicItem(unit, slot));

<<<<<<< HEAD
    case BU_ISLOT_ARENA_OPPONENT:
        bu->weaponSlotIndex = 0;
        bu->weapon = gArenaState.opponentWeapon;
        bu->canCounter = false;
        break;

    case BU_ISLOT_BALLISTA:
        bu->weaponSlotIndex = 0xFF;
        bu->weapon = GetBallistaItemAt(bu->unit.xPos, bu->unit.yPos);
        bu->canCounter = false;
        break;

    default:
        bu->weaponSlotIndex = 0xFF;
        bu->weapon = 0;
        bu->canCounter = false;
        break;
    }

    bu->weaponBefore = bu->weapon;
    bu->weaponAttributes = GetItemAttributes(bu->weapon);
    bu->weaponType = GetItemType(bu->weapon);

    if (!(gBattleStats.config & BATTLE_CONFIG_BIT2))
    {
        if (bu->weaponAttributes & IA_MAGICDAMAGE)
        {
            switch (GetItemIndex(bu->weapon))
            {

            case ITEM_SWORD_WINDSWORD:
                // if (gBattleStats.range == 2)
                if (gBattleStats.range >= 2)
                    bu->weaponType = ITYPE_ANIMA;
                else
                    bu->weaponAttributes = bu->weaponAttributes & ~IA_MAGICDAMAGE;
                break;

            case ITEM_SWORD_LIGHTBRAND:
                // if (gBattleStats.range == 2)
                if (gBattleStats.range >= 2)
                    bu->weaponType = ITYPE_LIGHT;
                else
                    bu->weaponAttributes = bu->weaponAttributes & ~IA_MAGICDAMAGE;
                break;

            case ITEM_SWORD_RUNESWORD:
                bu->weaponType = ITYPE_DARK;
                break;
            }
        }

#ifndef CHAX
        if (!IsItemCoveringRange(bu->weapon, gBattleStats.range) || bu->weaponSlotIndex == 0xFF)
        {
            bu->weapon = 0;
            bu->canCounter = false;
        }

        switch (bu->unit.statusIndex)
        {
        case UNIT_STATUS_SLEEP:
        case UNIT_STATUS_PETRIFY:
        case UNIT_STATUS_13:
            bu->weapon = 0;
            bu->canCounter = false;

            break;
        }
#endif
    }
}

STATIC_DECLAR void PostSetBattleUnitWeaponVanillaHook(struct BattleUnit *bu, int slot)
{
    FORCE_DECLARE int gBattleActorWeapon;
    FORCE_DECLARE int gBattleTargetWeapon;

    if (!(gBattleStats.config & BATTLE_CONFIG_BIT2))
    {
        if (!IsItemCoveringRangeRework(bu->weapon, gBattleStats.range, &bu->unit))
        {
            bu->weapon = 0;
            bu->canCounter = false;
            return;
        }
=======
	/* reserved */
	case CHAX_BUISLOT_THREEHOUSES_BMAG1 ... CHAX_BUISLOT_THREEHOUSES_BMAG7:
	case CHAX_BUISLOT_THREEHOUSES_WMAG1 ... CHAX_BUISLOT_THREEHOUSES_WMAG7:
	case CHAX_BUISLOT_ENGAGE_WEAPON1    ... CHAX_BUISLOT_ENGAGE_WEAPON7:
	case -1:
	default:
		return 0;
	}
}

LYN_REPLACE_CHECK(GetUnitEquippedWeaponSlot);
int GetUnitEquippedWeaponSlot(struct Unit *unit)
{
	int i;
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc

	for (i = 0; i < UNIT_ITEM_COUNT; ++i)
		if (CanUnitUseWeaponNow(unit, unit->items[i]) == TRUE)
			return i;

#if CHAX
	/* gaiden magic */
	if (gpKernelDesigerConfig->gaiden_magic_en) {
		i = GetGaidenMagicAutoEquipSlot(unit);
		if (i > 0)
			return i;
	}

	/* thress houses magic */

	/* engage weapon */
#endif /* CHAX */

	return -1;
}

LYN_REPLACE_CHECK(GetUnitEquippedWeapon);
u16 GetUnitEquippedWeapon(struct Unit *unit)
{
	return GetItemFromSlot(unit, GetUnitEquippedWeaponSlot(unit));
}

STATIC_DECLAR void SetBattleUnitWeaponVanilla(struct BattleUnit *bu, int itemSlot)
{
	if (itemSlot == BU_ISLOT_AUTO)
		itemSlot = GetUnitEquippedWeaponSlot(&bu->unit);

	if (bu->unit.state & US_IN_BALLISTA)
		itemSlot = BU_ISLOT_BALLISTA;

	bu->canCounter = true;

	switch (itemSlot) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		bu->weaponSlotIndex = itemSlot;
		bu->weapon = bu->unit.items[bu->weaponSlotIndex];
		break;

	case BU_ISLOT_5:
		bu->weaponSlotIndex = 0xFF;
		bu->weapon = gBmSt.um_tmp_item;
		break;

	case BU_ISLOT_ARENA_PLAYER:
		bu->weaponSlotIndex = 0;
		bu->weapon = gArenaState.playerWeapon;
		bu->canCounter = false;
		break;

	case BU_ISLOT_ARENA_OPPONENT:
		bu->weaponSlotIndex = 0;
		bu->weapon = gArenaState.opponentWeapon;
		bu->canCounter = false;
		break;

	case BU_ISLOT_BALLISTA:
		bu->weaponSlotIndex = 0xFF;
		bu->weapon = GetBallistaItemAt(bu->unit.xPos, bu->unit.yPos);
		bu->canCounter = false;
		break;

#if CHAX
	case CHAX_BUISLOT_GAIDEN_BMAG1 ... CHAX_BUISLOT_GAIDEN_BMAG7:
	case CHAX_BUISLOT_GAIDEN_WMAG1 ... CHAX_BUISLOT_GAIDEN_WMAG7:
		bu->weaponSlotIndex = itemSlot;
		bu->weapon = MakeNewItem(GetGaidenMagicItem(&bu->unit, itemSlot));
		bu->canCounter = false;
		break;

	/* reserved */
	case CHAX_BUISLOT_THREEHOUSES_BMAG1 ... CHAX_BUISLOT_THREEHOUSES_BMAG7:
	case CHAX_BUISLOT_THREEHOUSES_WMAG1 ... CHAX_BUISLOT_THREEHOUSES_WMAG7:
	case CHAX_BUISLOT_ENGAGE_WEAPON1    ... CHAX_BUISLOT_ENGAGE_WEAPON7:
#endif

	default:
		bu->weaponSlotIndex = 0xFF;
		bu->weapon = 0;
		bu->canCounter = false;
		break;

	}

	bu->weaponBefore = bu->weapon;
	bu->weaponAttributes = GetItemAttributes(bu->weapon);
	bu->weaponType = GetItemType(bu->weapon);

	if (!(gBattleStats.config & BATTLE_CONFIG_BIT2)) {
		if (bu->weaponAttributes & IA_MAGICDAMAGE) {
			switch (GetItemIndex(bu->weapon)) {

			case ITEM_SWORD_WINDSWORD:
				// if (gBattleStats.range == 2)
				if (gBattleStats.range >= 2)
					bu->weaponType = ITYPE_ANIMA;
				else
					bu->weaponAttributes = bu->weaponAttributes & ~IA_MAGICDAMAGE;
				break;

			case ITEM_SWORD_LIGHTBRAND:
				// if (gBattleStats.range == 2)
				if (gBattleStats.range >= 2)
					bu->weaponType = ITYPE_LIGHT;
				else
					bu->weaponAttributes = bu->weaponAttributes & ~IA_MAGICDAMAGE;
				break;

			case ITEM_SWORD_RUNESWORD:
				bu->weaponType = ITYPE_DARK;
				break;
			}
		}

#ifndef CHAX
		if (!IsItemCoveringRange(bu->weapon, gBattleStats.range) || bu->weaponSlotIndex == 0xFF) {
			bu->weapon = 0;
			bu->canCounter = false;
		}

		switch (bu->unit.statusIndex) {
		case UNIT_STATUS_SLEEP:
		case UNIT_STATUS_PETRIFY:
		case UNIT_STATUS_13:
			bu->weapon = 0;
			bu->canCounter = false;

			break;

		}
#endif
	}
}

STATIC_DECLAR void PostSetBattleUnitWeaponVanillaHook(struct BattleUnit *bu, int slot)
{
	if (!(gBattleStats.config & BATTLE_CONFIG_BIT2)) {
		if (!IsItemCoveringRangeRework(bu->weapon, gBattleStats.range, &bu->unit)) {
			bu->weapon = 0;
			bu->canCounter = false;
			return;
		}

		if (bu->weaponSlotIndex == 0xFF) {
			bu->weapon = 0;
			bu->canCounter = false;
			return;
		}

#if (defined(SID_Mimic) && (COMMON_SKILL_VALID(SID_Mimic)))
        if (BattleSkillTester(bu, SID_Mimic))
        {
            gBattleTargetWeapon = GetUnitEquippedWeapon(GetUnit(gBattleTarget.unit.index));
            gBattleActorWeapon = GetUnitEquippedWeapon(GetUnit(gBattleActor.unit.index));

            if (bu == &gBattleActor)
            {
                bu->weapon = gBattleTargetWeapon;
                bu->weaponBefore = gBattleTargetWeapon;
                bu->weaponType = gBattleTargetWeapon;
            }
            else
            {
                bu->weapon = gBattleActorWeapon;
                bu->weaponBefore = gBattleActorWeapon;
                bu->weaponType = gBattleActorWeapon;
            }
        }
#endif

#if (defined(SID_Dazzle) && (COMMON_SKILL_VALID(SID_Dazzle)))
		if (bu == &gBattleTarget && BattleFastSkillTester(&gBattleActor, SID_Dazzle)) {
			bu->weapon = 0;
			bu->canCounter = false;
			return;
		}
#endif

#if (defined(SID_Moonlight) && (COMMON_SKILL_VALID(SID_Moonlight)))
		if (bu == &gBattleTarget && BattleFastSkillTester(&gBattleActor, SID_Moonlight)) {
			bu->weapon = 0;
			bu->canCounter = false;
			return;
		}
#endif

<<<<<<< HEAD
#if (defined(SID_Comatose) && (COMMON_SKILL_VALID(SID_Comatose)))
        if (BattleSkillTester(bu, SID_Comatose))
            return;
        else
        {
            switch (GetUnitStatusIndex(&bu->unit))
            {
            case UNIT_STATUS_SLEEP:
            case UNIT_STATUS_PETRIFY:
            case UNIT_STATUS_13:
                bu->weapon = 0;
                bu->canCounter = false;
                return;

            default:
                break;
            };
        }
#else
        switch (GetUnitStatusIndex(&bu->unit))
        {
        case UNIT_STATUS_SLEEP:
        case UNIT_STATUS_PETRIFY:
        case UNIT_STATUS_13:
            bu->weapon = 0;
            bu->canCounter = false;
            return;

        default:
            break;
        };
#endif
    }
=======
		switch (GetUnitStatusIndex(&bu->unit)) {
		case UNIT_STATUS_SLEEP:
		case UNIT_STATUS_PETRIFY:
		case UNIT_STATUS_13:
			bu->weapon = 0;
			bu->canCounter = false;
			return;

		default:
			break;
		};
	}
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc
}

LYN_REPLACE_CHECK(SetBattleUnitWeapon);
void SetBattleUnitWeapon(struct BattleUnit *bu, int slot)
{
	int cid;

	SetBattleUnitWeaponVanilla(bu, slot);
	PostSetBattleUnitWeaponVanillaHook(bu, slot);

	/* Check combat-art */
	cid = GetCombatArtInForce(&bu->unit);
	if (COMBART_VALID(cid)) {
		if (GetCombatArtInfo(cid)->magic_attack)
			bu->weaponAttributes |= IA_MAGICDAMAGE | IA_MAGIC;
	}
}

LYN_REPLACE_CHECK(CanUnitUseWeapon);
s8 CanUnitUseWeapon(struct Unit *unit, int item)
{
	if (item == 0)
		return false;

	if (!(GetItemAttributes(item) & IA_WEAPON))
		return false;

<<<<<<< HEAD
#if (defined(SID_Amische) && (COMMON_SKILL_VALID(SID_Amische)))
    if (SkillTester(unit, SID_Amische))
        switch(ITEM_INDEX(item)) {
        case ITEM_SWORD_IRON:
        case ITEM_LANCE_IRON:
        case ITEM_AXE_IRON:
        case ITEM_BOW_IRON:
            break;
        default:
            return false;
        } 
#endif

    if (GetItemAttributes(item) & IA_LOCK_ANY)
    {
        // Check for item locks
=======
	if (GetItemAttributes(item) & IA_LOCK_ANY) {
		// Check for item locks
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc

		if ((GetItemAttributes(item) & IA_LOCK_1) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_1))
			return false;

		if ((GetItemAttributes(item) & IA_LOCK_4) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_4))
			return false;

		if ((GetItemAttributes(item) & IA_LOCK_5) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_5))
			return false;

		if ((GetItemAttributes(item) & IA_LOCK_6) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_6))
			return false;

		if ((GetItemAttributes(item) & IA_LOCK_7) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_7))
			return false;

		if ((GetItemAttributes(item) & IA_LOCK_2) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_2))
			return false;

		// Monster lock is special
		if (GetItemAttributes(item) & IA_LOCK_3) {
			if (!(UNIT_CATTRIBUTES(unit) & CA_LOCK_3))
				return false;

			return true;
		}

		if (GetItemAttributes(item) & IA_UNUSABLE)
			if (!(IsItemUnsealedForUnit(unit, item)))
				return false;
	}

#if CHAX
	if ((GetUnitStatusIndex(unit) == UNIT_STATUS_SILENCED) && (GetItemAttributes(item) & IA_MAGIC))
#else
	if ((unit->statusIndex == UNIT_STATUS_SILENCED) && (GetItemAttributes(item) & IA_MAGIC))
#endif
		return false;

#if CHAX
<<<<<<< HEAD
    switch (CheckWeaponLockEx(unit, item))
    {
    case 1:
        return true;
=======
	switch (CheckWeaponLockEx(unit, item)) {
	case 1:
		return true;
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc

	case -1:
		return false;
	}
#endif

<<<<<<< HEAD
#if (defined(SID_ShadowgiftPlus) && (COMMON_SKILL_VALID(SID_ShadowgiftPlus)))
    if (SkillTester(unit, SID_ShadowgiftPlus))
        if (GetItemType(item) == ITYPE_DARK)
            if (unit->ranks[ITYPE_DARK] == 0)
                if (GetItemRequiredExp(item) <= WPN_EXP_A) // A rank max
                    return true;
#endif

#if (defined(SID_Shadowgift) && (COMMON_SKILL_VALID(SID_Shadowgift)))
    if (SkillTester(unit, SID_Shadowgift))
        if (GetItemType(item) == ITYPE_DARK)
            if (unit->ranks[ITYPE_DARK] == 0)
                if (GetItemRequiredExp(item) <= WPN_EXP_C) // C rank max
                    return true;
#endif

#if (defined(SID_LuminaPlus) && (COMMON_SKILL_VALID(SID_LuminaPlus)))
    if (SkillTester(unit, SID_LuminaPlus))
        if (GetItemType(item) == ITYPE_LIGHT)
            if (unit->ranks[ITYPE_LIGHT] == 0)
                if (GetItemRequiredExp(item) <= WPN_EXP_A) // A rank max
                    return true;
#endif

#if (defined(SID_Lumina) && (COMMON_SKILL_VALID(SID_Lumina)))
    if (SkillTester(unit, SID_Lumina))
        if (GetItemType(item) == ITYPE_LIGHT)
            if (unit->ranks[ITYPE_LIGHT] == 0)
                if (GetItemRequiredExp(item) <= WPN_EXP_C) // C rank max
                    return true;
#endif

#if (defined(SID_StormgiftPlus) && (COMMON_SKILL_VALID(SID_StormgiftPlus)))
    if (SkillTester(unit, SID_StormgiftPlus))
        if (GetItemType(item) == ITYPE_ANIMA)
            if (unit->ranks[ITYPE_ANIMA] == 0)
                if (GetItemRequiredExp(item) <= WPN_EXP_A) // A rank max
                    return true;
#endif

#if (defined(SID_Stormgift) && (COMMON_SKILL_VALID(SID_Stormgift)))
    if (SkillTester(unit, SID_Stormgift))
        if (GetItemType(item) == ITYPE_ANIMA)
            if (unit->ranks[ITYPE_ANIMA] == 0)
                if (GetItemRequiredExp(item) <= WPN_EXP_C) // C rank max
                    return true;
#endif

#if (defined(SID_BladegiftPlus) && (COMMON_SKILL_VALID(SID_BladegiftPlus)))
    if (SkillTester(unit, SID_BladegiftPlus))
        if (GetItemType(item) == ITYPE_SWORD)
            if (unit->ranks[ITYPE_SWORD] == 0)
                if (GetItemRequiredExp(item) <= WPN_EXP_A) // A rank max
                    return true;
#endif

#if (defined(SID_Bladegift) && (COMMON_SKILL_VALID(SID_Bladegift)))
    if (SkillTester(unit, SID_Bladegift))
        if (GetItemType(item) == ITYPE_SWORD)
            if (unit->ranks[ITYPE_SWORD] == 0)
                if (GetItemRequiredExp(item) <= WPN_EXP_C) // C rank max
                    return true;
#endif

#if (defined(SID_PiercegiftPlus) && (COMMON_SKILL_VALID(SID_PiercegiftPlus)))
    if (SkillTester(unit, SID_PiercegiftPlus))
        if (GetItemType(item) == ITYPE_LANCE)
            if (unit->ranks[ITYPE_LANCE] == 0)
                if (GetItemRequiredExp(item) <= WPN_EXP_A) // A rank max
                    return true;
#endif

#if (defined(SID_Piercegift) && (COMMON_SKILL_VALID(SID_Piercegift)))
    if (SkillTester(unit, SID_Piercegift))
        if (GetItemType(item) == ITYPE_LANCE)
            if (unit->ranks[ITYPE_LANCE] == 0)
                if (GetItemRequiredExp(item) <= WPN_EXP_C) // C rank max
                    return true;
#endif

#if (defined(SID_HackgiftPlus) && (COMMON_SKILL_VALID(SID_HackgiftPlus)))
    if (SkillTester(unit, SID_HackgiftPlus))
        if (GetItemType(item) == ITYPE_AXE)
            if (unit->ranks[ITYPE_AXE] == 0)
                if (GetItemRequiredExp(item) <= WPN_EXP_A) // A rank max
                    return true;
#endif

#if (defined(SID_Hackgift) && (COMMON_SKILL_VALID(SID_Hackgift)))
    if (SkillTester(unit, SID_Hackgift))
        if (GetItemType(item) == ITYPE_AXE)
            if (unit->ranks[ITYPE_AXE] == 0)
                if (GetItemRequiredExp(item) <= WPN_EXP_C) // C rank max
                    return true;
#endif

#if (defined(SID_ArcgiftPlus) && (COMMON_SKILL_VALID(SID_ArcgiftPlus)))
    if (SkillTester(unit, SID_ArcgiftPlus))
        if (GetItemType(item) == ITYPE_BOW)
            if (unit->ranks[ITYPE_BOW] == 0)
                if (GetItemRequiredExp(item) <= WPN_EXP_A) // A rank max
                    return true;
#endif

#if (defined(SID_Arcgift) && (COMMON_SKILL_VALID(SID_Arcgift)))
    if (SkillTester(unit, SID_Arcgift))
        if (GetItemType(item) == ITYPE_BOW)
            if (unit->ranks[ITYPE_BOW] == 0)
                if (GetItemRequiredExp(item) <= WPN_EXP_C) // C rank max
                    return true;
#endif

    return (unit->ranks[GetItemType(item)] >= GetItemRequiredExp(item)) ? true : false;
}

LYN_REPLACE_CHECK(CanUnitUseWeaponNow);
s8 CanUnitUseWeaponNow(struct Unit* unit, int item) {

    if (item == 0)
        return FALSE;

    if (!(GetItemAttributes(item) & IA_WEAPON))
        return FALSE;

    if ((GetItemAttributes(item) & IA_MAGIC) && IsUnitMagicSealed(unit))
        return FALSE;

    return CanUnitUseWeapon(unit, item);
}

LYN_REPLACE_CHECK(CanUnitUseStaff);
s8 CanUnitUseStaff(struct Unit* unit, int item) {
    if (item == 0)
        return FALSE;

    if (!(GetItemAttributes(item) & IA_STAFF))
        return FALSE;

    if (unit->statusIndex == UNIT_STATUS_SLEEP)
        return FALSE;

    if (unit->statusIndex == UNIT_STATUS_BERSERK)
        return FALSE;

    if (unit->statusIndex == UNIT_STATUS_SILENCED)
        return FALSE;

#if (defined(SID_GracegiftPlus) && (COMMON_SKILL_VALID(SID_GracegiftPlus)))
    if (SkillTester(unit, SID_GracegiftPlus))
        if (GetItemType(item) == ITYPE_STAFF)
            if (unit->ranks[ITYPE_STAFF] == 0)
                if (GetItemRequiredExp(item) <= WPN_EXP_A) // A rank max
                    return true;
#endif

#if (defined(SID_Gracegift) && (COMMON_SKILL_VALID(SID_Gracegift)))
    if (SkillTester(unit, SID_Gracegift))
        if (GetItemType(item) == ITYPE_STAFF)
            if (unit->ranks[ITYPE_STAFF] == 0)
                if (GetItemRequiredExp(item) <= WPN_EXP_C) // C rank max
                    return true;
#endif

    {
        int wRank = GetItemRequiredExp(item);
        int uRank = unit->ranks[GetItemType(item)];

        return (uRank >= wRank) ? TRUE : FALSE;
    }
}

=======
	return (unit->ranks[GetItemType(item)] >= GetItemRequiredExp(item)) ? true : false;
}

>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc
int GetWeaponCost(struct BattleUnit *bu, u16 item)
{
	int cost;

	if (GetItemAttributes(item) & IA_UNBREAKABLE)
		return 0;

	cost = 1;

	if (bu == &gBattleActor) {
		int cid = GetCombatArtInForce(&bu->unit);

		if (COMBART_VALID(cid)) {
			int _cost = GetCombatArtInfo(cid)->cost;

			if (_cost > 1)
				cost = _cost;
		}
	}

#if (defined(SID_FaerghusAncestry) && (COMMON_SKILL_VALID(SID_FaerghusAncestry)))
	if (BattleFastSkillTester(bu, SID_FaerghusAncestry))
		cost = cost * 2;
#endif

	LIMIT_AREA(cost, 0, 255);
	return cost;
}

LYN_REPLACE_CHECK(GetUnitWeaponUsabilityBits);
int GetUnitWeaponUsabilityBits(struct Unit *unit)
{
	int i, item, result = 0;

	for (i = 0; (i < UNIT_ITEM_COUNT) && (item = unit->items[i]); ++i) {
		if ((GetItemAttributes(item) & IA_WEAPON) && CanUnitUseWeapon(unit, item))
			result |= UNIT_USEBIT_WEAPON;

		if ((GetItemAttributes(item) & IA_STAFF) && CanUnitUseStaff(unit, item))
			result |= UNIT_USEBIT_STAFF;
	}

#if CHAX
	/* gaiden magic */
	if (gpKernelDesigerConfig->gaiden_magic_en) {
		struct GaidenMagicList *list = GetGaidenMagicList(unit);

		for (i = 0; i < GAIDEN_MAGIC_LIST_LEN; i++) {
			item = list->bmags[i];

			if (item == ITEM_NONE)
				break;

			if (!CanUnitUseGaidenMagic(unit, item))
				continue;

			if (GetItemAttributes(item) & IA_WEAPON)
				result |= UNIT_USEBIT_WEAPON;

			if (GetItemAttributes(item) & IA_STAFF)
				result |= UNIT_USEBIT_STAFF;
		}

		for (i = 0; i < GAIDEN_MAGIC_LIST_LEN; i++) {
			item = list->wmags[i];

			if (item == ITEM_NONE)
				break;

			if (!CanUnitUseGaidenMagic(unit, item))
				continue;

			if (GetItemAttributes(item) & IA_WEAPON)
				result |= UNIT_USEBIT_WEAPON;

			if (GetItemAttributes(item) & IA_STAFF)
				result |= UNIT_USEBIT_STAFF;
		}
	}

	/* thress houses magic */

	/* engage weapon */
#endif

	return result;
}

LYN_REPLACE_CHECK(BattleInitItemEffect);
void BattleInitItemEffect(struct Unit *unit, int slot)
{
	int item;

#if CHAX
	item = GetItemFromSlot(unit, slot);
#else
	item = unit->items[slot];

	if (slot < 0)
		item = 0;
#endif

	gBattleStats.config = 0;

	InitBattleUnit(&gBattleActor, unit);

	SetBattleUnitTerrainBonusesAuto(&gBattleActor);
	ComputeBattleUnitBaseDefense(&gBattleActor);
	ComputeBattleUnitSupportBonuses(&gBattleActor, NULL);

	gBattleActor.battleAttack = 0xFF;
	gBattleActor.battleEffectiveHitRate = 100;
	gBattleActor.battleEffectiveCritRate = 0xFF;

	gBattleActor.weapon = item;
	gBattleActor.weaponBefore = item;
	gBattleActor.weaponSlotIndex = slot;
	gBattleActor.weaponType = GetItemType(item);
	gBattleActor.weaponAttributes = GetItemAttributes(item);

	gBattleActor.canCounter = TRUE;
	gBattleActor.hasItemEffectTarget = FALSE;

	gBattleActor.statusOut = -1;
	gBattleTarget.statusOut = -1;

	ClearBattleHits();
}

LYN_REPLACE_CHECK(BattleUnitTargetCheckCanCounter);
void BattleUnitTargetCheckCanCounter(struct BattleUnit *bu)
{
	if (bu->canCounter)
		return;

#if CHAX
	if (CheckUnbreakableSpecialSlot(bu->weaponSlotIndex))
		return;
#endif

	bu->battleAttack = 0xFF;
	bu->battleHitRate = 0xFF;
	bu->battleEffectiveHitRate = 0xFF;
	bu->battleCritRate = 0xFF;
	bu->battleEffectiveCritRate = 0xFF;
}

LYN_REPLACE_CHECK(BattleUnitTargetSetEquippedWeapon);
void BattleUnitTargetSetEquippedWeapon(struct BattleUnit *bu)
{
	int i, item;

	if (bu->weaponBefore)
		return;

	bu->weaponBefore = GetUnitEquippedWeapon(&bu->unit);
	if (bu->weaponBefore)
		return;

	if (!UnitHasMagicRank(&bu->unit))
		return;

	for (i = 0; i < UNIT_ITEM_COUNT; ++i) {
		item = bu->unit.items[i];

		if (item == 0)
			break;

		if (CanUnitUseStaff(&bu->unit, item)) {
			bu->weaponBefore = item;
			return;
		}
	}

#if CHAX
	if (gpKernelDesigerConfig->gaiden_magic_en) {
		item = GetGaidenMagicAutoEquipStaff(&bu->unit);

		if (item != ITEM_NONE) {
			bu->weaponBefore = MakeNewItem(item);
			return;
		}
	}
#endif
}
