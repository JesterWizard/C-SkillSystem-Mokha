#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/efx-skills.h"

const u8 EfxSkillAnimPriority[MAX_SKILL_NUM + 1] = {
#if (defined(SID_RuinedBlade) && COMMON_SKILL_VALID(SID_RuinedBlade))
    [SID_RuinedBlade] = EFX_PRIORITY_LOW,
#endif

#if (defined(SID_Vantage) && COMMON_SKILL_VALID(SID_Vantage))
    [SID_Vantage] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_DragonSkin) && COMMON_SKILL_VALID(SID_DragonSkin))
    [SID_DragonSkin] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_KeenFighter) && COMMON_SKILL_VALID(SID_KeenFighter))
    [SID_KeenFighter] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Desperation) && COMMON_SKILL_VALID(SID_Desperation))
    [SID_Desperation] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_QuickRiposte) && COMMON_SKILL_VALID(SID_QuickRiposte))
    [SID_QuickRiposte] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_WaryFighter) && COMMON_SKILL_VALID(SID_WaryFighter))
    [SID_WaryFighter] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_DoubleLion) && COMMON_SKILL_VALID(SID_DoubleLion))
    [SID_DoubleLion] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Adept) && COMMON_SKILL_VALID(SID_Adept))
    [SID_Adept] = EFX_PRIORITY_LOW,
#endif

#if (defined(SID_Astra) && COMMON_SKILL_VALID(SID_Astra))
    [SID_Astra] = EFX_PRIORITY_LOW,
#endif

#if (defined(SID_LoadstarRush) && COMMON_SKILL_VALID(SID_LoadstarRush))
    [SID_LoadstarRush] = EFX_PRIORITY_LOW,
#endif

#if (defined(SID_Aegis) && COMMON_SKILL_VALID(SID_Aegis))
    [SID_Aegis] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Pavise) && COMMON_SKILL_VALID(SID_Pavise))
    [SID_Pavise] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Bane) && COMMON_SKILL_VALID(SID_Bane))
    [SID_Bane] = EFX_PRIORITY_HIGH,
#endif

#if (defined(SID_Mercy) && COMMON_SKILL_VALID(SID_Mercy))
    [SID_Mercy] = EFX_PRIORITY_HIGH,
#endif

#if (defined(SID_Aether) && COMMON_SKILL_VALID(SID_Aether))
    [SID_Aether] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Corona) && COMMON_SKILL_VALID(SID_Corona))
    [SID_Corona] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Luna) && COMMON_SKILL_VALID(SID_Luna))
    [SID_Luna] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Flare) && COMMON_SKILL_VALID(SID_Flare))
    [SID_Flare] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Sol) && COMMON_SKILL_VALID(SID_Sol))
    [SID_Sol] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_DragonFang) && COMMON_SKILL_VALID(SID_DragonFang))
    [SID_DragonFang] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Colossus) && COMMON_SKILL_VALID(SID_Colossus))
    [SID_Colossus] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Corrosion) && COMMON_SKILL_VALID(SID_Corrosion))
    [SID_Corrosion] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Ignis) && COMMON_SKILL_VALID(SID_Ignis))
    [SID_Ignis] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Impale) && COMMON_SKILL_VALID(SID_Impale))
    [SID_Impale] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Glacies) && COMMON_SKILL_VALID(SID_Glacies))
    [SID_Glacies] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Vengeance) && COMMON_SKILL_VALID(SID_Vengeance))
    [SID_Vengeance] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Deadeye) && COMMON_SKILL_VALID(SID_Deadeye))
    [SID_Deadeye] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_AxeFaith) && COMMON_SKILL_VALID(SID_AxeFaith))
    [SID_AxeFaith] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Miracle) && COMMON_SKILL_VALID(SID_Miracle))
    [SID_Miracle] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Armsthrift) && COMMON_SKILL_VALID(SID_Armsthrift))
    [SID_Armsthrift] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_GreatShield) && COMMON_SKILL_VALID(SID_GreatShield))
    [SID_GreatShield] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_SureShot) && COMMON_SKILL_VALID(SID_SureShot))
    [SID_SureShot] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_RuinedBladePlus) && COMMON_SKILL_VALID(SID_RuinedBladePlus))
    [SID_RuinedBladePlus] = EFX_PRIORITY_LOW,
#endif

#if (defined(SID_GuardBearing) && COMMON_SKILL_VALID(SID_GuardBearing))
    [SID_GuardBearing] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_VengefulFighter) && COMMON_SKILL_VALID(SID_VengefulFighter))
    [SID_VengefulFighter] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_RapidFighter) && COMMON_SKILL_VALID(SID_RapidFighter))
    [SID_RapidFighter] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_RecklessFighter) && COMMON_SKILL_VALID(SID_RecklessFighter))
    [SID_RecklessFighter] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_BoldFighter) && COMMON_SKILL_VALID(SID_BoldFighter))
    [SID_BoldFighter] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Counter) && COMMON_SKILL_VALID(SID_Counter))
    [SID_Counter] = EFX_PRIORITY_HIGH,
#endif

#if (defined(SID_Multiscale) && COMMON_SKILL_VALID(SID_Multiscale))
    [SID_Multiscale] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_TowerShieldPlus) && COMMON_SKILL_VALID(SID_TowerShieldPlus))
    [SID_TowerShieldPlus] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Petrify) && COMMON_SKILL_VALID(SID_Petrify))
    [SID_Petrify] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Enrage) && COMMON_SKILL_VALID(SID_Enrage))
    [SID_Enrage] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Pickup) && COMMON_SKILL_VALID(SID_Pickup))
    [SID_Pickup] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_DevilsPact) && COMMON_SKILL_VALID(SID_DevilsPact))
    [SID_DevilsPact] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_DevilsWhim) && COMMON_SKILL_VALID(SID_DevilsWhim))
    [SID_DevilsWhim] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_DevilsLuck) && COMMON_SKILL_VALID(SID_DevilsLuck))
    [SID_DevilsLuck] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Expertise) && COMMON_SKILL_VALID(SID_Expertise))
    [SID_Expertise] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_CounterMagic) && COMMON_SKILL_VALID(SID_CounterMagic))
    [SID_CounterMagic] = EFX_PRIORITY_HIGH,
#endif

#if (defined(SID_LiquidOoze) && COMMON_SKILL_VALID(SID_LiquidOoze))
    [SID_LiquidOoze] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Eclipse) && COMMON_SKILL_VALID(SID_Eclipse))
    [SID_Eclipse] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_LunarBrace) && COMMON_SKILL_VALID(SID_LunarBrace))
    [SID_LunarBrace] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Pursuer) && COMMON_SKILL_VALID(SID_Pursuer))
    [SID_Pursuer] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_QuickLearner) && COMMON_SKILL_VALID(SID_QuickLearner))
    [SID_QuickLearner] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_EffectSpore) && COMMON_SKILL_VALID(SID_EffectSpore))
    [SID_EffectSpore] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_GaleWings) && COMMON_SKILL_VALID(SID_GaleWings))
    [SID_GaleWings] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Sturdy) && COMMON_SKILL_VALID(SID_Sturdy))
    [SID_Sturdy] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_HoldOut) && COMMON_SKILL_VALID(SID_HoldOut))
    [SID_HoldOut] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_ArmsthriftPlus) && COMMON_SKILL_VALID(SID_ArmsthriftPlus))
    [SID_ArmsthriftPlus] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_MagicBounce) && COMMON_SKILL_VALID(SID_MagicBounce))
    [SID_MagicBounce] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Fluffy) && COMMON_SKILL_VALID(SID_Fluffy))
    [SID_Fluffy] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_WeaponHeal) && COMMON_SKILL_VALID(SID_WeaponHeal))
    [SID_WeaponHeal] = EFX_PRIORITY_HIGH,
#endif

#if (defined(SID_IronDome) && COMMON_SKILL_VALID(SID_IronDome))
    [SID_IronDome] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Alacrity) && COMMON_SKILL_VALID(SID_Alacrity))
    [SID_Alacrity] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_AlacrityPlus) && COMMON_SKILL_VALID(SID_AlacrityPlus))
    [SID_AlacrityPlus] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Echo) && COMMON_SKILL_VALID(SID_Echo))
    [SID_Echo] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_WyvernCrash) && COMMON_SKILL_VALID(SID_WyvernCrash))
    [SID_WyvernCrash] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_LethalityPlus) && COMMON_SKILL_VALID(SID_LethalityPlus))
    [SID_LethalityPlus] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_GrimReaper) && COMMON_SKILL_VALID(SID_GrimReaper))
    [SID_GrimReaper] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_TrickRoom) && COMMON_SKILL_VALID(SID_TrickRoom))
    [SID_TrickRoom] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Rampart) && COMMON_SKILL_VALID(SID_Rampart))
    [SID_Rampart] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_RampartPlus) && COMMON_SKILL_VALID(SID_RampartPlus))
    [SID_RampartPlus] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Forcefield) && COMMON_SKILL_VALID(SID_Forcefield))
    [SID_Forcefield] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_ResolvedHeart) && COMMON_SKILL_VALID(SID_ResolvedHeart))
    [SID_ResolvedHeart] = EFX_PRIORITY_LOW,
#endif

#if (defined(SID_LastStand) && COMMON_SKILL_VALID(SID_LastStand))
    [SID_LastStand] = EFX_PRIORITY_NORMAL,
#endif

#if (defined(SID_Reinforce) && COMMON_SKILL_VALID(SID_Reinforce))
    [SID_Reinforce] = EFX_PRIORITY_NORMAL,
#endif
};

struct EfxAnimConf const * const EfxSkillAnims[MAX_SKILL_NUM + 1] = {
#if (defined(SID_RuinedBlade) && COMMON_SKILL_VALID(SID_RuinedBlade))
    [SID_RuinedBlade] = &EfxSkillVanilla,
#endif

#if (defined(SID_Vantage) && COMMON_SKILL_VALID(SID_Vantage))
    [SID_Vantage] = &EfxSkillVanilla,
#endif

#if (defined(SID_DragonSkin) && COMMON_SKILL_VALID(SID_DragonSkin))
    [SID_DragonSkin] = &EfxSkillVanilla,
#endif

#if (defined(SID_KeenFighter) && COMMON_SKILL_VALID(SID_KeenFighter))
    [SID_KeenFighter] = &EfxSkillVanilla,
#endif

#if (defined(SID_Desperation) && COMMON_SKILL_VALID(SID_Desperation))
    [SID_Desperation] = &EfxSkillVanilla,
#endif

#if (defined(SID_QuickRiposte) && COMMON_SKILL_VALID(SID_QuickRiposte))
    [SID_QuickRiposte] = &EfxSkillVanilla,
#endif

#if (defined(SID_WaryFighter) && COMMON_SKILL_VALID(SID_WaryFighter))
    [SID_WaryFighter] = &EfxSkillVanilla,
#endif

#if (defined(SID_DoubleLion) && COMMON_SKILL_VALID(SID_DoubleLion))
    [SID_DoubleLion] = &EfxSkillVanilla,
#endif

#if (defined(SID_Adept) && COMMON_SKILL_VALID(SID_Adept))
    [SID_Adept] = &EfxSkillVanilla,
#endif

#if (defined(SID_Astra) && COMMON_SKILL_VALID(SID_Astra))
    [SID_Astra] = &EfxSkillVanilla,
#endif

#if (defined(SID_LoadstarRush) && COMMON_SKILL_VALID(SID_LoadstarRush))
    [SID_LoadstarRush] = &EfxSkillVanilla,
#endif

#if (defined(SID_Aegis) && COMMON_SKILL_VALID(SID_Aegis))
    [SID_Aegis] = &EfxSkillVanilla,
#endif

#if (defined(SID_Pavise) && COMMON_SKILL_VALID(SID_Pavise))
    [SID_Pavise] = &EfxSkillVanilla,
#endif

#if (defined(SID_Bane) && COMMON_SKILL_VALID(SID_Bane))
    [SID_Bane] = &EfxSkillVanilla,
#endif

#if (defined(SID_Mercy) && COMMON_SKILL_VALID(SID_Mercy))
    [SID_Mercy] = &EfxSkillVanilla,
#endif

#if (defined(SID_Aether) && COMMON_SKILL_VALID(SID_Aether))
    [SID_Aether] = &EfxSkillVanilla,
#endif

#if (defined(SID_Corona) && COMMON_SKILL_VALID(SID_Corona))
    [SID_Corona] = &EfxSkillVanilla,
#endif

#if (defined(SID_Luna) && COMMON_SKILL_VALID(SID_Luna))
    [SID_Luna] = &EfxSkillVanilla,
#endif

#if (defined(SID_Flare) && COMMON_SKILL_VALID(SID_Flare))
    [SID_Flare] = &EfxSkillVanilla,
#endif

#if (defined(SID_Sol) && COMMON_SKILL_VALID(SID_Sol))
    [SID_Sol] = &EfxSkillVanilla,
#endif

#if (defined(SID_DragonFang) && COMMON_SKILL_VALID(SID_DragonFang))
    [SID_DragonFang] = &EfxSkillVanilla,
#endif

#if (defined(SID_Colossus) && COMMON_SKILL_VALID(SID_Colossus))
    [SID_Colossus] = &EfxSkillVanilla,
#endif

#if (defined(SID_Corrosion) && COMMON_SKILL_VALID(SID_Corrosion))
    [SID_Corrosion] = &EfxSkillVanilla,
#endif

#if (defined(SID_Ignis) && COMMON_SKILL_VALID(SID_Ignis))
    [SID_Ignis] = &EfxSkillVanilla,
#endif

#if (defined(SID_Impale) && COMMON_SKILL_VALID(SID_Impale))
    [SID_Impale] = &EfxSkillVanilla,
#endif

#if (defined(SID_Glacies) && COMMON_SKILL_VALID(SID_Glacies))
    [SID_Glacies] = &EfxSkillVanilla,
#endif

#if (defined(SID_Vengeance) && COMMON_SKILL_VALID(SID_Vengeance))
    [SID_Vengeance] = &EfxSkillVanilla,
#endif

#if (defined(SID_Deadeye) && COMMON_SKILL_VALID(SID_Deadeye))
    [SID_Deadeye] = &EfxSkillVanilla,
#endif

#if (defined(SID_AxeFaith) && COMMON_SKILL_VALID(SID_AxeFaith))
    [SID_AxeFaith] = &EfxSkillVanilla,
#endif

#if (defined(SID_Miracle) && COMMON_SKILL_VALID(SID_Miracle))
    [SID_Miracle] = &EfxSkillVanilla,
#endif

#if (defined(SID_Armsthrift) && COMMON_SKILL_VALID(SID_Armsthrift))
    [SID_Armsthrift] = &EfxSkillVanilla,
#endif

#if (defined(SID_GreatShield) && COMMON_SKILL_VALID(SID_GreatShield))
    [SID_GreatShield] = &EfxSkillVanilla,
#endif

#if (defined(SID_SureShot) && COMMON_SKILL_VALID(SID_SureShot))
    [SID_SureShot] = &EfxSkillVanilla,
#endif

#if (defined(SID_RuinedBladePlus) && COMMON_SKILL_VALID(SID_RuinedBladePlus))
    [SID_RuinedBladePlus] = &EfxSkillVanilla,
#endif

#if (defined(SID_GuardBearing) && COMMON_SKILL_VALID(SID_GuardBearing))
    [SID_GuardBearing] = &EfxSkillVanilla,
#endif

#if (defined(SID_VengefulFighter) && COMMON_SKILL_VALID(SID_VengefulFighter))
    [SID_VengefulFighter] = &EfxSkillVanilla,
#endif

#if (defined(SID_RapidFighter) && COMMON_SKILL_VALID(SID_RapidFighter))
    [SID_RapidFighter] = &EfxSkillVanilla,
#endif

#if (defined(SID_RecklessFighter) && COMMON_SKILL_VALID(SID_RecklessFighter))
    [SID_RecklessFighter] = &EfxSkillVanilla,
#endif

#if (defined(SID_BoldFighter) && COMMON_SKILL_VALID(SID_BoldFighter))
    [SID_BoldFighter] = &EfxSkillVanilla,
#endif

#if (defined(SID_Counter) && COMMON_SKILL_VALID(SID_Counter))
    [SID_Counter] = &EfxSkillVanilla,
#endif

#if (defined(SID_Multiscale) && COMMON_SKILL_VALID(SID_Multiscale))
    [SID_Multiscale] = &EfxSkillVanilla,
#endif

#if (defined(SID_TowerShieldPlus) && COMMON_SKILL_VALID(SID_TowerShieldPlus))
    [SID_TowerShieldPlus] = &EfxSkillVanilla,
#endif

#if (defined(SID_Petrify) && COMMON_SKILL_VALID(SID_Petrify))
    [SID_Petrify] = &EfxSkillVanilla,
#endif

#if (defined(SID_Enrage) && COMMON_SKILL_VALID(SID_Enrage))
    [SID_Enrage] = &EfxSkillVanilla,
#endif

#if (defined(SID_Pickup) && COMMON_SKILL_VALID(SID_Pickup))
    [SID_Pickup] = &EfxSkillVanilla,
#endif

#if (defined(SID_DevilsPact) && COMMON_SKILL_VALID(SID_DevilsPact))
    [SID_DevilsPact] = &EfxSkillVanilla,
#endif

#if (defined(SID_DevilsWhim) && COMMON_SKILL_VALID(SID_DevilsWhim))
    [SID_DevilsWhim] = &EfxSkillVanilla,
#endif

#if (defined(SID_DevilsLuck) && COMMON_SKILL_VALID(SID_DevilsLuck))
    [SID_DevilsLuck] = &EfxSkillVanilla,
#endif

#if (defined(SID_Expertise) && COMMON_SKILL_VALID(SID_Expertise))
    [SID_Expertise] = &EfxSkillVanilla,
#endif

#if (defined(SID_CounterMagic) && COMMON_SKILL_VALID(SID_CounterMagic))
    [SID_CounterMagic] = &EfxSkillVanilla,
#endif

#if (defined(SID_LiquidOoze) && COMMON_SKILL_VALID(SID_LiquidOoze))
    [SID_LiquidOoze] = &EfxSkillVanilla,
#endif

#if (defined(SID_Eclipse) && COMMON_SKILL_VALID(SID_Eclipse))
    [SID_Eclipse] = &EfxSkillVanilla,
#endif

#if (defined(SID_LunarBrace) && COMMON_SKILL_VALID(SID_LunarBrace))
    [SID_LunarBrace] = &EfxSkillVanilla,
#endif

#if (defined(SID_Pursuer) && COMMON_SKILL_VALID(SID_Pursuer))
    [SID_Pursuer] = &EfxSkillVanilla,
#endif

#if (defined(SID_QuickLearner) && COMMON_SKILL_VALID(SID_QuickLearner))
    [SID_QuickLearner] = &EfxSkillVanilla,
#endif

#if (defined(SID_EffectSpore) && COMMON_SKILL_VALID(SID_EffectSpore))
    [SID_EffectSpore] = &EfxSkillVanilla,
#endif

#if (defined(SID_GaleWings) && COMMON_SKILL_VALID(SID_GaleWings))
    [SID_GaleWings] = &EfxSkillVanilla,
#endif

#if (defined(SID_Sturdy) && COMMON_SKILL_VALID(SID_Sturdy))
    [SID_Sturdy] = &EfxSkillVanilla,
#endif

#if (defined(SID_ArmsthriftPlus) && COMMON_SKILL_VALID(SID_ArmsthriftPlus))
    [SID_ArmsthriftPlus] = &EfxSkillVanilla,
#endif

#if (defined(SID_MagicBounce) && COMMON_SKILL_VALID(SID_MagicBounce))
    [SID_MagicBounce] = &EfxSkillVanilla,
#endif

#if (defined(SID_Fluffy) && COMMON_SKILL_VALID(SID_Fluffy))
    [SID_Fluffy] = &EfxSkillVanilla,
#endif

#if (defined(SID_WeaponHeal) && COMMON_SKILL_VALID(SID_WeaponHeal))
    [SID_WeaponHeal] = &EfxSkillVanilla,
#endif

#if (defined(SID_IronDome) && COMMON_SKILL_VALID(SID_IronDome))
    [SID_IronDome] = &EfxSkillVanilla,
#endif

#if (defined(SID_Alacrity) && COMMON_SKILL_VALID(SID_Alacrity))
    [SID_Alacrity] = &EfxSkillVanilla,
#endif

#if (defined(SID_AlacrityPlus) && COMMON_SKILL_VALID(SID_AlacrityPlus))
    [SID_AlacrityPlus] = &EfxSkillVanilla,
#endif

#if (defined(SID_Echo) && COMMON_SKILL_VALID(SID_Echo))
    [SID_Echo] = &EfxSkillVanilla,
#endif

#if (defined(SID_WyvernCrash) && COMMON_SKILL_VALID(SID_WyvernCrash))
    [SID_WyvernCrash] = &EfxSkillVanilla,
#endif

#if (defined(SID_LethalityPlus) && COMMON_SKILL_VALID(SID_LethalityPlus))
    [SID_LethalityPlus] = &EfxSkillVanilla,
#endif

#if (defined(SID_GrimReaper) && COMMON_SKILL_VALID(SID_GrimReaper))
    [SID_GrimReaper] = &EfxSkillVanilla,
#endif

#if (defined(SID_TrickRoom) && COMMON_SKILL_VALID(SID_TrickRoom))
    [SID_TrickRoom] = &EfxSkillVanilla,
#endif

#if (defined(SID_Rampart) && COMMON_SKILL_VALID(SID_Rampart))
    [SID_Rampart] = &EfxSkillVanilla,
#endif

#if (defined(SID_RampartPlus) && COMMON_SKILL_VALID(SID_RampartPlus))
    [SID_RampartPlus] = &EfxSkillVanilla,
#endif

#if (defined(SID_Forcefield) && COMMON_SKILL_VALID(SID_Forcefield))
    [SID_Forcefield] = &EfxSkillVanilla,
#endif

#if (defined(SID_ResolvedHeart) && COMMON_SKILL_VALID(SID_ResolvedHeart))
    [SID_ResolvedHeart] = &EfxSkillVanilla,
#endif

#if (defined(SID_LastStand) && COMMON_SKILL_VALID(SID_LastStand))
    [SID_LastStand] = &EfxSkillVanilla,
#endif

#if (defined(SID_Reinforce) && COMMON_SKILL_VALID(SID_Reinforce))
    [SID_Reinforce] = &EfxSkillVanilla,
#endif
};
