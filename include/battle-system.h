#pragma once


#include "debug-kit.h"

#define IS_BATTLE_UNIT(_unit) ((_unit) == &gBattleActor.unit  || (_unit) ==  &gBattleTarget.unit)
#define IS_BATTLE_ACTOR(bu) ((bu) == &gBattleActor)

struct BattleStatus {
	s16 atk, def, as, hit, avo, crit, dodge, silencer;
};

extern struct BattleStatus BattleSysBattleStatusBackup[2];
#define BattleUnitOriginalStatus(bu) (((bu) == &gBattleTarget) ? &BattleSysBattleStatusBackup[1] : &BattleSysBattleStatusBackup[0])

/* WTA bonus */
struct WeaponTriangleConf {
	s8 wtype_a;
	s8 wtype_b;

	u8 _pad_[3];

	bool is_buff;
	u16 sid;

	s8 bonus_atk;
	s8 bonus_def;
	s8 bonus_speed;
	s8 bonus_hit;
	s8 bonus_avoid;
	s8 bonus_crit;
	s8 bonus_dodge;
	s8 bonus_silencer;
};

extern struct WeaponTriangleConf const *const gpWeaponTriangleConfs;

struct WeaponTriangleItemConf {
	s8 valid, wtype;
	struct {
		s8 atk, def, hit, avo, crit, silencer;
	} battle_status;

	bool is_buff;
	u8 _pad_[3];
};

extern const struct WeaponTriangleItemConf gWeaponTriangleItemConf[0x100];
extern struct WeaponTriangleItemConf const *const gpWeaponTriangleItemConf;

/* Battle hit expansion */
#define NEW_BATTLE_HIT_MAX 0x20 /* This should align to gAnimRoundData */
extern struct BattleHit gBattleHitArrayRe[NEW_BATTLE_HIT_MAX];
extern u16 gAnimRoundDataRe[NEW_BATTLE_HIT_MAX];
extern u16 gEfxHpLutRe[NEW_BATTLE_HIT_MAX * 3];

extern void * const prBattleHitArray;
extern u8 BattleHitArrayWidth;

struct ExtBattleHit {
	u8 hp_drain, hp_cost;
	u8 _pad_[2];
};
extern struct ExtBattleHit gExtBattleHitArray[NEW_BATTLE_HIT_MAX];

static inline int GetBattleHitRound(struct BattleHit *hit) { return hit - gBattleHitArrayRe; }
static inline int GetCurrentBattleHitRound(void) { return GetBattleHitRound(gBattleHitIterator); }

static inline struct ExtBattleHit *GetExtBattleHit(int round) { return &gExtBattleHitArray[round]; }
static inline struct ExtBattleHit *GetCurrentExtBattleHit(void) { return GetExtBattleHit(GetCurrentBattleHitRound()); }
static inline struct ExtBattleHit *GetExtBattleHitFromHit(struct BattleHit *hit) { return GetExtBattleHit(GetBattleHitRound(hit)); }

bool CheckBattleHitOverflow(void);
bool CheckCanTwiceAttackOrder(struct BattleUnit *actor, struct BattleUnit *target);
int CalcBattleRealDamage(struct BattleUnit *attacker, struct BattleUnit *defender);

extern struct BattleGlobalFlags {
    u32 hitted : 1;
    u32 round_cnt : 5;      // align to NEW_BATTLE_HIT_MAX
    u32 round_cnt_hit : 5;  // align to NEW_BATTLE_HIT_MAX
    u32 round_cnt_avo : 5;  // align to NEW_BATTLE_HIT_MAX
    u32 enemy_defeated : 1;
    u32 allies_in_range3x3 : 5;
    u32 enemies_in_range3x3 : 5;
    u32 skill_activated_double_lion : 1;
    u32 skill_activated_astra : 1;
    u32 skill_activated_galeforce : 1;
    u32 skill_activated_vigor_dance : 1;
    u32 skill_activated_lead_by_example : 1;
    u32 skill_activated_capture : 1;
    u32 skill_activated_angerpoint : 1;
    u32 skill_activated_debilitator : 1;
    u32 skill_activated_resurrection : 1;
	u32 skill_activated_echo : 1;
	u32 skill_activated_dance : 1;
	u32 arenaFix: 1;
} gBattleActorGlobalFlag, gBattleTargetGlobalFlag;

static inline struct BattleGlobalFlags * GetBattleGlobalFlags(struct BattleUnit * bu)
{
    return (bu == &gBattleActor)
         ? &gBattleActorGlobalFlag
         : &gBattleTargetGlobalFlag;
}

void ClearBattleGlobalFlags(void);
void RegisterHitCnt(struct BattleUnit * bu, bool miss);

extern struct {
    u32 nihil_on_actor  : 1;
    u32 nihil_on_target : 1;

    u32 desperation_order : 1;
    u32 vantage_order : 1;
    u32 tar_force_twice_order : 1;
    u32 act_force_twice_order : 1;

    u32 skill_activated_sure_shoot : 1;
    u32 skill_activated_dead_eye : 1;
    u32 skill_activated_aether : 1;

    u32 skill_roll12 : 4;
} gBattleTemporaryFlag;

enum BattleOrderSkills_Type {
    BORDER_DESPERATION,
    BORDER_VANTAGE,
    BORDER_ACT_TWICE,
    BORDER_TAR_TWICE,

    BORDER_MAX
};
extern u16 BattleOrderSkills[BORDER_MAX];

#define RegisterBattleOrderSkill(sid, type) (BattleOrderSkills[type] = (sid))

int GetWeaponCost(struct BattleUnit *bu, u16 item);

extern struct Vec2 gBattleTargetPositionBackup;

/**
 * BattleOrder
 */
enum {
	NOP_ATTACK = 0,
	ACT_ATTACK = 1,
	TAR_ATTACK = 2,
};

enum {
	UNWIND_VANTAGE = 1 << 0,
	UNWIND_DESPERA = 1 << 1,
	UNWIND_DOUBLE_ACT = 1 << 2,
	UNWIND_DOUBLE_TAR = 1 << 3,
};

extern const u8 BattleUnwindConfig[16][4];

struct EfxSkillQueue {
	u8 cur, max;
	u16 skill_pool[14];
};
extern struct EfxSkillQueue sEfxSkillQueue;

void ResetRoundEfxSkills(void);
void EnqueueRoundEfxSkill(u16 sid);
u16 DequeueRoundEfxSkill(void);

/**
 * BattleHit
 */
extern const u16 _DmgDecreaseRef[100];
#define DAMAGE_DECREASE(rate) _DmgDecreaseRef[rate]

void PreBattleGenerateHook(void);

/**
 * BattleHitAttr
 */
bool CheckBattleHpHalve(struct BattleUnit *attacker, struct BattleUnit *defender);
bool CheckDevilAttack(struct BattleUnit *attacker, struct BattleUnit *defender);
bool CheckBattleMiracle(struct BattleUnit *attacker, struct BattleUnit *defender);
void AppendHpDrain(struct BattleUnit *attacker, struct BattleUnit *defender, int drain);
void BattleHit_CalcHpDrain(struct BattleUnit *attacker, struct BattleUnit *defender);
void BattleHit_InjectNegativeStatus(struct BattleUnit *attacker, struct BattleUnit *defender);
void BattleHit_ConsumeWeapon(struct BattleUnit *attacker, struct BattleUnit *defender);

/**
 * BattleDamage
 */
extern struct {
	bool8 crit_atk;
	int result;
	int damage_base, attack, defense;
	int correction, real_damage, increase, decrease, crit_correction;
} gDmg;

int BattleHit_CalcDamage(struct BattleUnit *attacker, struct BattleUnit *defender);

/**
 * BattleUI
 */
void ModifyBattleStatusForUI(void);

/**
 * Leadership
 */
struct LeaderShipConf {
	bool8 en;
	bool8 cancel_out_opposing;
	struct {
		u8 hit, avo;
	} ally_bonus, enemy_bonus, npc_bonus;
};

extern struct LeaderShipConf const *const gpLeaderShipConf;

extern u8 const *const gpLeaderShipPConf;
extern u8 const *const gpLeaderShipJConf;

int GetUnitLeaderShip(struct Unit *unit);
void PreBattleCalcLeadershipBonus(struct BattleUnit *actor, struct BattleUnit *target);

/**
 * Bow2Decrease patch
 */
bool CheckWeaponCostForMissedBowAttack(struct BattleUnit *actor);

/**
 * Hp cost
 */
bool TryBattleHpCost(struct BattleUnit *bu, int hp_cost);
bool AddBattleHpCost(struct BattleUnit *bu, int round, int cost);

void BattleGenerateHitHpCost(struct BattleUnit *attacker, struct BattleUnit *defender);
bool MapAnimRoundAnim_DisplayHpCost(ProcPtr parent);
void NewEfxHpCost(struct Anim *anim);
bool EfxHpCostExists(void);

/**
 * Item slot
 */
enum {
	// BattleUnit::weaponSlotIndex
	CHAX_BUISLOT_BASE = UNIT_ITEM_COUNT + 4,
};

static inline bool CheckUnbreakableSpecialSlot(int slot)
{
	switch (slot) {

	default:
		return false;
	}
}

int GetItemFromSlot(struct Unit *unit, int slot);