#include "common-chax.h"
#include "battle-system.h"
#include "skill-system.h"
#include "unit-expa.h"
#include "kernel-lib.h"
#include "combat-art.h"
#include "efx-skill.h"
#include "strmag.h"
#include "kernel-tutorial.h"
#include "constants/skills.h"
#include "constants/combat-arts.h"
#include "playst-expa.h"
#include "jester_headers/custom-functions.h"

typedef void (*BattleDamageCalcFunc)(struct BattleUnit * buA, struct BattleUnit * buB);
extern BattleDamageCalcFunc const * const gpBattleDamageCalcFuncs;

typedef int (*BattleRealDamageCalcFunc)(int old, struct BattleUnit * buA, struct BattleUnit * buB);
extern BattleRealDamageCalcFunc const * const gpBattleRealDamageCalcFuncs;

int CalcBattleRealDamage(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    const BattleRealDamageCalcFunc * it;

    int damage = 0;

#if defined(SID_RuinedBlade) && (COMMON_SKILL_VALID(SID_RuinedBlade))
    if (BattleSkillTester(attacker, SID_RuinedBlade))
        damage += SKILL_EFF2(SID_RuinedBlade);
#endif

#if defined(SID_RuinedBladePlus) && (COMMON_SKILL_VALID(SID_RuinedBladePlus))
    if (BattleSkillTester(attacker, SID_RuinedBladePlus))
        damage += SKILL_EFF1(SID_RuinedBladePlus);
#endif

#if defined(SID_LunaAttack) && (COMMON_SKILL_VALID(SID_LunaAttack))
    if (BattleSkillTester(attacker, SID_LunaAttack))
        damage += defender->battleDefense / 4;
#endif

#if defined(SID_Bushido) && (COMMON_SKILL_VALID(SID_Bushido))
    if (BattleSkillTester(attacker, SID_Bushido))
        damage += SKILL_EFF1(SID_Bushido);
#endif

    for (it = gpBattleRealDamageCalcFuncs; *it; it++)
        damage = (*it)(damage, attacker, defender);

    return damage;
}

int BattleHit_CalcDamage(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    const BattleDamageCalcFunc * it;
    bool alteredCrit = false;

    FORCE_DECLARE bool barricadePlus_activated;
    int result;
    FORCE_DECLARE int roll12_ID =
        15; // Set it to the maximum value for its bitfield, so it won't be accidentally triggered

    FORCE_DECLARE bool tintedLensPlus = false;

    FORCE_DECLARE struct BattleGlobalFlags *act_flags, *tar_flags;

    /**
     * result = ([atk + gDmg.correction - def])
     *              * (100% + increase%)
     *              * (100% + crit_correction%)
     *              / (100% + decrease%)
     *              + real_damage
     */

    if (attacker == &gBattleActor)
    {
        act_flags = &gBattleActorGlobalFlag;
        tar_flags = &gBattleTargetGlobalFlag;
    }
    else
    {
        act_flags = &gBattleTargetGlobalFlag;
        tar_flags = &gBattleActorGlobalFlag;
    }

#if defined(SID_Roll12) && (COMMON_SKILL_VALID(SID_Roll12))
    if (BattleSkillTester(attacker, SID_Roll12))
        roll12_ID = NextRN_N(9);
#endif

    /**
     * Step0: Roll critical and silencer attack
     */
    gDmg.crit_atk = false;

#if (defined(SID_Chipper) && (COMMON_SKILL_VALID(SID_Chipper)))
	if (BattleSkillTester(attacker, SID_Chipper))
		SetPartyGoldAmount(GetPartyGoldAmount() + SKILL_EFF0(SID_Chipper));
#endif


    if (
#if defined(SID_Fortune) && (COMMON_SKILL_VALID(SID_Fortune))
        !BattleSkillTester(defender, SID_Fortune)
#else
        (1)
#endif
        &&
#if defined(SID_Foresight) && (COMMON_SKILL_VALID(SID_Foresight))
        !BattleSkillTester(defender, SID_Foresight)
#else
        (1)
#endif
    )
    {
        if (BattleRoll1RN(gBattleStats.critRate, false))
        {
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_CRIT;
            gDmg.crit_atk = true;

#if defined(SID_AngerPoint) && (COMMON_SKILL_VALID(SID_AngerPoint))
            if (BattleSkillTester(defender, SID_AngerPoint) && !gBattleActorGlobalFlag.skill_activated_angerpoint)
                gBattleActorGlobalFlag.skill_activated_angerpoint = true;
#endif

            if (BattleRoll1RN(gBattleStats.silencerRate, false))
            {
                /* Directly return on silencer attack to fasten calc */
                gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_SILENCER;
                return BATTLE_MAX_DAMAGE;
            }
        }
    }

    /**
     * Step1: Calculate real damage
     */
    gDmg.real_damage = CalcBattleRealDamage(attacker, defender);

    /**
     * Step2: Calculate base damage (atk + gDmg.correction - def)
     */
    gDmg.attack = gBattleStats.attack;
    gDmg.defense = gBattleStats.defense;
    gDmg.correction = 0;

#if (defined(SID_Swarm) && (COMMON_SKILL_VALID(SID_Swarm)))
    if (BattleSkillTester(attacker, SID_Swarm))
    {
        if (gBattleStats.range == 1)
        {
            struct Unit * unit = GetUnit(defender->unit.index);
            int x = unit->xPos;
            int x2 = attacker->unit.xPos;
            int y = unit->yPos;
            int y2 = attacker->unit.yPos;
            bool noUnit = false;

            // if the target can be on any adjacent position, do nothing
            if (Generic_CanUnitBeOnPos(unit, x + 1, y, x2, y2))
            {
                noUnit = true;
            }
            if (noUnit || Generic_CanUnitBeOnPos(unit, x - 1, y, x2, y2))
            {
                noUnit = true;
            }
            if (noUnit || Generic_CanUnitBeOnPos(unit, x, y + 1, x2, y2))
            {
                noUnit = true;
            }
            if (noUnit || Generic_CanUnitBeOnPos(unit, x, y - 1, x2, y2))
            {
                noUnit = true;
            }

            if (!noUnit)
            {
                int dmg = attacker->battleAttack - defender->battleDefense;
                if (dmg < 0)
                    dmg = 0;
                int addDmg = Div(dmg * SKILL_EFF0(SID_Swarm), 100);
                attacker->battleAttack += addDmg;
            }
        }
    }
#endif

#if defined(SID_RampUp) && (COMMON_SKILL_VALID(SID_RampUp))
    if (BattleSkillTester(attacker, SID_RampUp) && gDmg.crit_atk)
    {
        alteredCrit = true;
        gDmg.attack *= 2;
    }
#endif

    if (attacker == &gBattleActor)
    {
        /**
         * I don't think it's a good idea to put calculation here.
         *
         * Also for the combat-art, it is better to judge on round count,
         * so that it can only take effect on first strike.
         *
         * Later we may optimize it.
         */
        switch (GetCombatArtInForce(&attacker->unit))
        {
            case CID_Detonate:
                if (!(GetItemAttributes(attacker->weapon) & IA_UNBREAKABLE))
                    gDmg.defense = 0;

                break;

            default:
                break;
        }
    }

#if (defined(SID_Flare) && (COMMON_SKILL_VALID(SID_Flare)))
    if (CheckBattleSkillActivate(attacker, defender, SID_Flare, attacker->unit.skl) || roll12_ID == 0)
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Flare);
        gDmg.defense = gDmg.defense / 2;
    }
#endif

    if (IsMagicAttack(attacker))
    {
#if (defined(SID_Corona) && (COMMON_SKILL_VALID(SID_Corona)))
        if (CheckBattleSkillActivate(attacker, defender, SID_Corona, attacker->unit.skl) || roll12_ID == 1)
        {
            RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Corona);
            gDmg.defense = 0;
        }
#endif
    }
    else
    {
#if (defined(SID_Luna) && (COMMON_SKILL_VALID(SID_Luna)))
        if (CheckBattleSkillActivate(attacker, defender, SID_Luna, attacker->unit.skl) || roll12_ID == 2)
        {
            RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Luna);
            gDmg.defense = 0;
        }
#endif
    }

#if defined(SID_ResolvedHeart) && (COMMON_SKILL_VALID(SID_ResolvedHeart))
    if (BattleSkillTester(attacker, SID_ResolvedHeart)) 
    {
        if (attacker == &gBattleActor && gActionData.unk08 == SID_ResolvedHeart && !CheckBitUES(GetUnit(attacker->unit.index), UES_BIT_RESOLVED_HEART_SKILL_USED))
            gDmg.defense = 0;
    }
#endif

#if (defined(SID_Ignis) && (COMMON_SKILL_VALID(SID_Ignis)))
    if (CheckBattleSkillActivate(attacker, defender, SID_Ignis, attacker->unit.skl) || roll12_ID == 3)
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Ignis);
        switch (attacker->weaponType)
        {
            case ITYPE_SWORD:
            case ITYPE_LANCE:
            case ITYPE_AXE:
            case ITYPE_BOW:
                gDmg.correction += attacker->unit.def / 2;
                break;

            case ITYPE_ANIMA:
            case ITYPE_LIGHT:
            case ITYPE_DARK:
                gDmg.correction += attacker->unit.res / 2;
                break;
            default:
                break;
        }
    }
#endif

    gBattleTemporaryFlag.skill_activated_aether = false;

#if (defined(SID_Aether) && (COMMON_SKILL_VALID(SID_Aether)))
    if (CheckBattleSkillActivate(attacker, defender, SID_Aether, attacker->unit.skl) || roll12_ID == 4)
    {
        gBattleTemporaryFlag.skill_activated_aether = true;
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Aether);
        gDmg.correction += defender->battleDefense * 4 / 5;
    }
#endif

#if (defined(SID_LunarBrace) && (COMMON_SKILL_VALID(SID_LunarBrace)))
    if (BattleSkillTester(attacker, SID_LunarBrace))
    {
        if (&gBattleActor == attacker)
        {
            RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_LunarBrace);
            gDmg.correction += defender->battleDefense * 1 / 4;
        }
    }
#endif

#if defined(SID_Glacies) && (COMMON_SKILL_VALID(SID_Glacies))
    if (CheckBattleSkillActivate(attacker, defender, SID_Glacies, attacker->unit.skl) || roll12_ID == 5)
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Glacies);
        gDmg.correction += attacker->unit.res;
    }
#endif

#if defined(SID_Vengeance) && (COMMON_SKILL_VALID(SID_Vengeance))
    if (CheckBattleSkillActivate(attacker, defender, SID_Vengeance, attacker->unit.skl) || roll12_ID == 6)
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Vengeance);
        gDmg.correction += (attacker->unit.maxHP - attacker->unit.curHP);
    }
#endif

#if (defined(SID_DragonsWrath) && (COMMON_SKILL_VALID(SID_DragonsWrath)))
    if (BattleSkillTester(attacker, SID_DragonsWrath) && act_flags->round_cnt_hit == 1)
        gDmg.correction += gDmg.attack * SKILL_EFF0(SID_DragonsWrath) / 100;
#endif

    /**
     * Step2.1: fasten calculation!
     */
    gDmg.damage_base = gDmg.attack + gDmg.correction - gDmg.defense;
    if (gDmg.damage_base <= 0)
        gDmg.damage_base = 0;

    if (gDmg.damage_base <= 0 && gDmg.real_damage <= 0)
    {
        /* If no damage taken, directly end the damage calculation */
        /* Jester turning this off now because of tinted lens, and what time does it save really? */
        // return 0;
    }
    else
    {
#if (defined(SID_GreatShield) && (COMMON_SKILL_VALID(SID_GreatShield)))
        if (CheckBattleSkillActivate(defender, attacker, SID_GreatShield, defender->unit.skl))
        {
            RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_GreatShield);
            return 0;
        }
#endif

#if (defined(SID_TowerShieldPlus) && (COMMON_SKILL_VALID(SID_TowerShieldPlus)))
        if (BattleSkillTester(defender, SID_TowerShieldPlus))
        {
            if (gBattleStats.range > 1)
            {
                RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_TowerShieldPlus);
                return 0;
            }
        }
#endif

#if (defined(SID_Dazzling) && (COMMON_SKILL_VALID(SID_Dazzling)))
        if (BattleSkillTester(defender, SID_Dazzling))
        {
            if (gBattleStats.range >= 3)
            {
                RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Dazzling);
                return 0;
            }
        }
#endif
        if (IsMagicAttack(attacker))
        {
#if (defined(SID_Aegis) && (COMMON_SKILL_VALID(SID_Aegis)))
            if (CheckBattleSkillActivate(defender, attacker, SID_Aegis, defender->unit.skl))
            {
                RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Aegis);
                return 0;
            }
#endif
        }
        else
        {
#if (defined(SID_Pavise) && (COMMON_SKILL_VALID(SID_Pavise)))
            if (CheckBattleSkillActivate(defender, attacker, SID_Pavise, defender->unit.skl))
            {
                RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Pavise);
                return 0;
            }
#endif
        }
    }

    /**
     * Step3: Calculate damage increase amplifier (100% + increase%)
     */
    gDmg.increase = 100;

#if defined(SID_DragonFang) && (COMMON_SKILL_VALID(SID_DragonFang))
    if (CheckBattleSkillActivate(attacker, defender, SID_DragonFang, attacker->unit.skl * 2) || roll12_ID == 7)
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DragonFang);
        gDmg.increase += SKILL_EFF0(SID_DragonFang);
    }
#endif

#if (defined(SID_Colossus) && (COMMON_SKILL_VALID(SID_Colossus)))
    if (CheckBattleSkillActivate(attacker, defender, SID_Colossus, attacker->unit.skl) || roll12_ID == 8)
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Colossus);
        gDmg.increase += 10 * SKILL_EFF0(SID_Colossus);
        gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_CRIT;
    }
#endif

#if defined(SID_Impale) && (COMMON_SKILL_VALID(SID_Impale))
    if (CheckBattleSkillActivate(attacker, defender, SID_Impale, attacker->unit.skl) || roll12_ID == 9)
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Impale);
        gDmg.increase += 10 * SKILL_EFF0(SID_Impale);
        gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_CRIT;
    }
#endif

#if defined(SID_SolarPower) && (COMMON_SKILL_VALID(SID_SolarPower))
    if (BattleSkillTester(attacker, SID_SolarPower))
        if (gPlaySt.chapterWeatherId == WEATHER_FLAMES && IsMagicAttack(attacker))
            gDmg.increase += SKILL_EFF0(SID_SolarPower);
#endif

#if (defined(SID_Fluffy) && (COMMON_SKILL_VALID(SID_Fluffy)))
    if (BattleSkillTester(defender, SID_Fluffy))
    {
        if (gBattleStats.range != 1)
        {
            RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Fluffy);
            gDmg.increase += SKILL_EFF0(SID_Fluffy);
        }
    }
#endif

    if (gBattleTemporaryFlag.skill_activated_sure_shoot)
        gDmg.increase += 50;

    /**
     * Step4: Calculate critial damage amplifier (200%  + crit_correction%)
     */
    gDmg.crit_correction = 100;
    if (gDmg.crit_atk && !alteredCrit)
    {
        gDmg.crit_correction = 300;

#if defined(SID_InfinityEdge) && (COMMON_SKILL_VALID(SID_InfinityEdge))
        if (BattleSkillTester(attacker, SID_InfinityEdge))
            gDmg.crit_correction += SKILL_EFF0(SID_InfinityEdge);
#endif

#if (defined(SID_Gambit) && (COMMON_SKILL_VALID(SID_Gambit)))
        if (BattleSkillTester(defender, SID_Gambit) && gBattleStats.range == 1)
            gDmg.crit_correction -= SKILL_EFF0(SID_Gambit);
#endif

#if (defined(SID_MagicGambit) && (COMMON_SKILL_VALID(SID_MagicGambit)))
        if (BattleSkillTester(defender, SID_MagicGambit) && gBattleStats.range > 1)
            gDmg.crit_correction -= SKILL_EFF0(SID_MagicGambit);
#endif
    }

    /**
     * Step5: Calculate damage decrease amplifier (100% + decrease%)
     */
    gDmg.decrease = 0x100;

#if defined(SID_LoadStarRush) && (COMMON_SKILL_VALID(SID_LoadStarRush))
    if (attacker == &gBattleActor && BattleSkillTester(attacker, SID_LoadStarRush))
        gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF1(SID_LoadstarRush));
#endif

#if defined(SID_Astra) && (COMMON_SKILL_VALID(SID_Astra))
    if (attacker == &gBattleActor && BattleSkillTester(attacker, SID_Astra) &&
        gBattleActorGlobalFlag.skill_activated_astra)
        gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF1(SID_Astra));
#endif

#if defined(SID_Echo) && (COMMON_SKILL_VALID(SID_Echo))
    if (attacker == &gBattleActor && BattleSkillTester(attacker, SID_Echo) &&
        gBattleActorGlobalFlag.skill_activated_astra)
        gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF1(SID_Echo));
#endif

#if (defined(SID_IronDome) && (COMMON_SKILL_VALID(SID_IronDome)))
    if (BattleSkillTester(defender, SID_IronDome))
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_IronDome);
        gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_IronDome));
    }
#endif

#if (defined(SID_DragonSkin) && (COMMON_SKILL_VALID(SID_DragonSkin)))
    if (BattleSkillTester(defender, SID_DragonSkin))
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DragonSkin);
        gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_DragonSkin));
    }
#endif

#if (defined(SID_Multiscale) && (COMMON_SKILL_VALID(SID_Multiscale)))
    if (BattleSkillTester(defender, SID_Multiscale))
    {
        if (defender->unit.curHP == defender->unit.maxHP)
        {
            RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Multiscale);
            gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_Multiscale));
        }
    }
#endif

#if defined(SID_Expertise) && (COMMON_SKILL_VALID(SID_Expertise))
    if (BattleSkillTester(defender, SID_Expertise) && gDmg.crit_atk)
        gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_Expertise));
#endif

#if (defined(SID_KeenFighter) && (COMMON_SKILL_VALID(SID_KeenFighter)))
    if (BattleSkillTester(defender, SID_KeenFighter) && CheckCanTwiceAttackOrder(attacker, defender))
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_KeenFighter);
        gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_KeenFighter));
    }
#endif

#if defined(SID_GuardBearing) && (COMMON_SKILL_VALID(SID_GuardBearing))
    if (BattleSkillTester(defender, SID_GuardBearing))
    {
        if (!AreUnitsAllied(defender->unit.index, gPlaySt.faction) && act_flags->round_cnt_hit == 1)
        {
            RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_GuardBearing);
            gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_GuardBearing));
        }
    }
#endif

bool rampartPlus_activated = false;

#if defined(SID_RampartPlus) && (COMMON_SKILL_VALID(SID_RampartPlus))
    if (BattleSkillTester(defender, SID_RampartPlus))
    {
        if (!AreUnitsAllied(defender->unit.index, gPlaySt.faction) && act_flags->round_cnt_hit == 1)
        {
            RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_RampartPlus);
            gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_RampartPlus));
            rampartPlus_activated = true;
        }
    }
#endif

#if defined(SID_Rampart) && (COMMON_SKILL_VALID(SID_Rampart))
    if (BattleSkillTester(defender, SID_Rampart) && !rampartPlus_activated)
    {
        if (!AreUnitsAllied(defender->unit.index, gPlaySt.faction) && act_flags->round_cnt_hit == 1)
        {
            RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Rampart);
            gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_Rampart));
        }
    }
#endif

#if defined(SID_Protect) && (COMMON_SKILL_VALID(SID_Protect))
    if (BattleSkillTester(defender, SID_Protect) && !CheckBitUES(GetUnit(defender->unit.index), UES_BIT_PROTECT_SKILL_USED))
    {
        if (!AreUnitsAllied(defender->unit.index, gPlaySt.faction) && act_flags->round_cnt_hit == 1)
        {
            RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Protect);
            gDmg.decrease += DAMAGE_DECREASE(99);
            SetBitUES(GetUnit(defender->unit.index), UES_BIT_PROTECT_SKILL_USED);
        }
    }
#endif

#if (defined(SID_BeastAssault) && (COMMON_SKILL_VALID(SID_BeastAssault)))
    if (BattleSkillTester(defender, SID_BeastAssault))
        gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_BeastAssault));
#endif

#if (defined(SID_Spurn) && (COMMON_SKILL_VALID(SID_Spurn)))
    if (BattleSkillTester(defender, SID_Spurn))
    {
        int _diff = defender->battleSpeed - attacker->battleSpeed;
        LIMIT_AREA(_diff, 0, 10);

        gDmg.decrease += DAMAGE_DECREASE(_diff * SKILL_EFF0(SID_Spurn));
    }
#endif

#if (defined(SID_Bushido) && (COMMON_SKILL_VALID(SID_Bushido)))
    if (BattleSkillTester(defender, SID_Bushido))
    {
        int _diff = defender->battleSpeed - attacker->battleSpeed;
        LIMIT_AREA(_diff, 0, 10);

        gDmg.decrease += DAMAGE_DECREASE(_diff * SKILL_EFF0(SID_Bushido));
    }
#endif

#if (defined(SID_DragonWall) && (COMMON_SKILL_VALID(SID_DragonWall)))
    if (BattleSkillTester(defender, SID_DragonWall))
    {
        int _diff = defender->unit.res - attacker->unit.res;
        LIMIT_AREA(_diff, 0, 10);

        gDmg.decrease += DAMAGE_DECREASE(_diff * SKILL_EFF0(SID_DragonWall));
    }
#endif

#if (defined(SID_BlueLionRule) && (COMMON_SKILL_VALID(SID_BlueLionRule)))
    if (BattleSkillTester(defender, SID_BlueLionRule))
    {
        int _diff = defender->unit.def - attacker->unit.def;
        LIMIT_AREA(_diff, 0, 10);

        gDmg.decrease += DAMAGE_DECREASE(_diff * SKILL_EFF0(SID_BlueLionRule));
    }
#endif

#if (defined(SID_CounterRoar) && (COMMON_SKILL_VALID(SID_CounterRoar)))
    if (BattleSkillTester(defender, SID_CounterRoar))
    {
        if (act_flags->round_cnt_hit == 1)
            gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_CounterRoar));
        else
            gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF1(SID_CounterRoar));
    }
#endif

#if (defined(SID_DragonsWrath) && (COMMON_SKILL_VALID(SID_DragonsWrath)))
    if (BattleSkillTester(defender, SID_DragonsWrath) && tar_flags->round_cnt_hit == 1)
        gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_DragonsWrath));
#endif

#if (defined(SID_CrusaderWard) && (COMMON_SKILL_VALID(SID_CrusaderWard)))
    if (BattleSkillTester(defender, SID_CrusaderWard) && tar_flags->round_cnt_hit > 1 && gBattleStats.range > 1)
        gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_CrusaderWard));
#endif

    /**
     * Boolean check to prevent Barricade+ and Barricade from stacking
     */
    barricadePlus_activated = false;

#if (defined(SID_BarricadePlus) && (COMMON_SKILL_VALID(SID_BarricadePlus)))
    if (BattleSkillTester(defender, SID_BarricadePlus))
    {
        if (act_flags->round_cnt_hit > 2)
        {
            int _i, _reduction = SKILL_EFF0(SID_BarricadePlus);
            int _base = _reduction;
            barricadePlus_activated = true;

            for (_i = 0; _i < act_flags->round_cnt_hit - 2; _i++)
            {
                _base = _base / 2;
                _reduction += _base;
            }
            gDmg.decrease += DAMAGE_DECREASE(_reduction);
        }
    }
#endif

#if (defined(SID_Barricade) && (COMMON_SKILL_VALID(SID_Barricade)))
    if (BattleSkillTester(defender, SID_Barricade) && !barricadePlus_activated)
    {
        if (defender->unit.curHP < defender->hpInitial)
            gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_Barricade));
    }
#endif

#if (defined(SID_Fluffy) && (COMMON_SKILL_VALID(SID_Fluffy)))
    if (BattleSkillTester(defender, SID_Fluffy))
    {
        if (gBattleStats.range == 1)
        {
            RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Fluffy);
            gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF1(SID_Fluffy));
        }
    }
#endif

    /**
     * External calc!
     */
    for (it = gpBattleDamageCalcFuncs; *it; it++)
        (*it)(attacker, defender);

    /**
     * Step6: Calculate result
     */
    {
        u32 dividend, divisor, quotient;

        dividend = gDmg.damage_base * gDmg.increase * gDmg.crit_correction * 0x100;
        divisor = 100 * 100 * gDmg.decrease;

        quotient = DIV_ROUND_CLOSEST(dividend, divisor);
        Debugf("dividend=%ld, divisor=%ld, quotient=%ld", dividend, divisor, quotient);
        result = quotient;
    }

    // if (result == 0 && gDmg.damage_base > 0)
    //     result = 1; // at least 1 damage left.

    result += gDmg.real_damage;

#if (defined(SID_Bide) && (COMMON_SKILL_VALID(SID_Bide)))
    if (BattleSkillTester(attacker, SID_Bide) && gActionData.unk08 == SID_Bide)
        result = (attacker->unit.curHP - 1) * 2;
#endif

#if (defined(SID_TintedLensPlus) && (COMMON_SKILL_VALID(SID_TintedLensPlus)))
    if (BattleSkillTester(attacker, SID_TintedLensPlus))
    {
        if (result < 6)
        {
            tintedLensPlus = true;
            result = 6;
        }
    }
#endif

#if (defined(SID_TintedLens) && (COMMON_SKILL_VALID(SID_TintedLens)))
    if ((BattleSkillTester(attacker, SID_TintedLens) || BattleSkillTester(defender, SID_TintedLens)) && !tintedLensPlus)
    {
        if (result < 6)
            result = 6;
    }
#endif

#if (defined(SID_Decadon) && (COMMON_SKILL_VALID(SID_Decadon)))
    if (BattleSkillTester(attacker, SID_Decadon))
        result = 10;
#endif

#if (defined(SID_LethalityPlus) && (COMMON_SKILL_VALID(SID_LethalityPlus)))
    if (BattleSkillTester(attacker, SID_LethalityPlus))
        if (gActionData.unk08 == SID_LethalityPlus && !PlayStExpa_CheckBit(PLAYSTEXPA_BIT_LethalityPlus_Used))
        {
            PlayStExpa_SetBit(PLAYSTEXPA_BIT_LethalityPlus_Used);
            gActionData.unk08 = 0;
            result = defender->unit.curHP;
        }
#endif

#if (defined(SID_GrimReaper) && (COMMON_SKILL_VALID(SID_GrimReaper)))
    if (BattleSkillTester(attacker, SID_GrimReaper))
        if (gActionData.unk08 == SID_GrimReaper && CheckBitUES(GetUnit(attacker->unit.index), UES_BIT_GRIM_REAPER_SKILL_USED))
        {
            ClearBitUES(GetUnit(attacker->unit.index), UES_BIT_GRIM_REAPER_SKILL_USED);
            gActionData.unk08 = 0;
            result = defender->unit.curHP;
            
            if (UNIT_CATTRIBUTES(&defender->unit) && CA_BOSS)
                GetUnit(attacker->unit.index)->maxHP -= 5;
            else
                GetUnit(attacker->unit.index)->maxHP -= 1;
        }
#endif

#if (defined(SID_Levitate) && (COMMON_SKILL_VALID(SID_Levitate)))
        if (BattleSkillTester(defender, SID_Levitate))
        {
            if (IsMagicAttack(attacker) || isFlyingClass(UNIT_CLASS_ID(GetUnit(attacker->unit.index))))
                result = result;
            else
                result = 0;
        }
#endif

    Printf(
        "[round %d] dmg=%d: base=%d (atk=%d, def=%d, cor=%d), inc=%d, crt=%d, dec=%d, real=%d",
        GetBattleHitRound(gBattleHitIterator), result, gDmg.damage_base, gDmg.attack, gDmg.defense, gDmg.correction,
        gDmg.increase, gDmg.crit_correction, gDmg.decrease, gDmg.real_damage);

#if (defined(SID_Equalizer) && (COMMON_SKILL_VALID(SID_Equalizer)))
    if (BattleSkillTester(attacker, SID_Equalizer) || BattleSkillTester(defender, SID_Equalizer))
        result = ((defender->battleAttack - attacker->battleDefense) + (attacker->battleAttack - defender->battleDefense)) / 2;
#endif

    if (result > BATTLE_MAX_DAMAGE)
        result = BATTLE_MAX_DAMAGE;

    return result;
}
