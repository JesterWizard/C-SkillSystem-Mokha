#pragma once

#include "common-chax.h"
#include "efx-anim.h"
#include "list-verify.h"
#include "kernel-lib.h"
#include "constants/event-cmds.h"
#include "constants/skills.h"

#ifndef MAX_SKILL_NUM
    #define MAX_SKILL_NUM 0x3FF
#endif

#ifdef CONFIG_TURN_ON_ALL_SKILLS
    #define MAX_EQUIP_SKILL_NUM 0x3FF
#else
    #define MAX_EQUIP_SKILL_NUM 0xFF
#endif

#ifdef CONFIG_TURN_ON_ALL_SKILLS
    #define UNIT_RAM_SKILLS_LEN 5
#else
    #define UNIT_RAM_SKILLS_LEN 7
#endif

#ifdef CONFIG_TURN_ON_ALL_SKILLS

void StartSkillSwapTradeMenu(struct Unit * leftUnit, struct Unit * rightUnit);

static inline u16 GetUnitSkillIndex(const struct Unit* unit, int index) {
    if (index < 0 || index >= UNIT_RAM_SKILLS_LEN) return 0xFFFF;

    u64 buffer = 0;
    for (int i = 0; i < 7; ++i)
        buffer |= ((u64)unit->supports[i]) << (8 * i);

    return (buffer >> (index * 10)) & 0x3FF;
}

static inline void SetUnitSkillIndex(struct Unit* unit, int index, u16 sid) {
    if (index < 0 || index >= UNIT_RAM_SKILLS_LEN) return;

    u64 buffer = 0;
    for (int i = 0; i < 7; ++i)
        buffer |= ((u64)unit->supports[i]) << (8 * i);

    buffer &= ~(((u64)0x3FF) << (index * 10));
    buffer |= ((u64)(sid & 0x3FF)) << (index * 10);

    for (int i = 0; i < 7; ++i)
        unit->supports[i] = (buffer >> (8 * i)) & 0xFF;
}

#define GET_SKILL(unit, i) GetUnitSkillIndex(unit, i)
#define SET_SKILL(unit, i, sid) SetUnitSkillIndex(unit, i, sid)

#else

#define GET_SKILL(unit, i) (UNIT_RAM_SKILLS(unit)[i])
#define SET_SKILL(unit, i, sid) (UNIT_RAM_SKILLS(unit)[i] = (sid))

#endif

enum SkillInfoListss
{
    SKILL_INFO_EQUIP,
    SKILL_INFO_CHARACTER,
    SKILL_INFO_CLASS,
    SKILL_INFO_ITEM,

    Skill_INFO_MAX
};

#ifdef CONFIG_TURN_ON_ALL_SKILLS
    #define MAX_EQUIP_SKILL_NUM 0x3FF
    // For extended equippable skills, always use generic tester.
    #define SKILL_INDEX_LIST(sid) (0)
#else
    #define MAX_EQUIP_SKILL_NUM 0xFF
    #define SKILL_INDEX_LIST(sid) (((sid) >> 8) & 0x3FF)
#endif

#ifdef CONFIG_TURN_ON_ALL_SKILLS
    #define EQUIP_SKILL_VALID(sid) (sid > 0x000 && sid < 0x400)
#else
    #define EQUIP_SKILL_VALID(sid) (sid > 0x000 && sid < 0x0FF)
#endif

#define COMMON_SKILL_VALID(sid) (sid > 0x000 && sid < 0x400)

/**
 * Generic skills
 */

#define GET_UNIT_SKILL(unit, i) (                  \
    (((((u64)(unit)->supports[0]))     |           \
    (((u64)(unit)->supports[1]) << 8)  |           \
    (((u64)(unit)->supports[2]) << 16) |           \
    (((u64)(unit)->supports[3]) << 24) |           \
    (((u64)(unit)->supports[4]) << 32) |           \
    (((u64)(unit)->supports[5]) << 40) |           \
    (((u64)(unit)->supports[6]) << 48)) >> ((i) * 10)) & 0x3FF)
    

#define UNIT_RAM_SKILLS(unit) ((u8 *)((unit)->supports))

#define RAM_SKILL_LEN_EXT (                                             \
    gpKernelDesigerConfig->max_equipable_skill < UNIT_RAM_SKILLS_LEN    \
        ? gpKernelDesigerConfig->max_equipable_skill                    \
        : UNIT_RAM_SKILLS_LEN)

extern u16 const * const gpConstSkillTable_Person;
extern u16 const * const gpConstSkillTable_Job;
extern u16 const * const gpConstSkillTable_Item;
extern u16 const * const gpConstSkillTable_Weapon;

struct SkillInfo {
    const u8 * icon;
    u16 name, desc, price, capacity;
};

extern struct SkillInfo const * const gpSkillInfos;

const u8 * GetSkillIcon1(const u8 sid);
const u8 * GetSkillIcon2(const u8 sid);
const u8 * GetSkillIcon3(const u8 sid);
const u8 * GetSkillIcon4(const u8 sid);

u16 GetSkillDescMsg(const u16 sid);
u16 GetSkillNameMsg(const u16 sid);
char * GetSkillDescStr(const u16 sid);
char * SkillDescToName(char * str);
char * GetSkillNameStrFormDesc(const u16 sid);
char * GetSkillNameStr(const u16 sid);

/* An interesting quirk of the narrow font transformer in GetSkillNameStr is that the output is impossible
 to perform string comparisons on. Hence, a secondary function is need that just gives the raw output */
char * GetSkillNameStr_NormalFont(const u16 sid);

u16 GetSkillPrice(const u16 sid);
u16 GetSkillCapacity(const u16 sid);

/**
 * 7 generic skill
 * 2 person skill
 * 2 job skill
 * 10 item skill
 * 
 * (maybe todo) 2 weapon skill
 */
struct SkillList {
    struct UnitListHeader header;
    u8 amt;
    u16 sid[23];
};

struct SkillList *GetUnitSkillList(struct Unit *unit);

void SetupBattleSkillList(void);
void GenerateSkillListExt(struct Unit *unit, struct SkillList *list);
void ForceUpdateUnitSkillList(struct Unit *unit);
void DisableUnitSkilLList(struct Unit *unit);
void ResetSkillLists(void);
void AppendBattleUnitSkillList(struct BattleUnit *bu, u16 skill);

/* Skill testers */
extern bool (*_SkillTester)(struct Unit *unit, const u16 sid);
bool _SkillListTester(struct Unit *unit, const u16 sid);
// extern bool (*_BattleFastSkillTester)(struct BattleUnit *bu, const u16 sid);

// see: ../docs/SkillSys.md
#define SkillTester _SkillTester
#define SkillListTester(unit, sid) _SkillListTester(unit, sid)
// #define BattleFastSkillTester _BattleFastSkillTester

// Note this function can only exec for r0 = gBattleActor/gBattleTarget
extern bool (* _JudgeSkillViaList)(struct BattleUnit * unit, const u16 sid);
#define BattleSkillTester _JudgeSkillViaList
#define _BattleSkillTester(unit, sid) BattleSkillTester((struct BattleUnit *)(unit), sid)

bool CheckSkillActivate(struct Unit *unit, int sid, int rate);
bool CheckBattleSkillActivate(struct BattleUnit *actor, struct BattleUnit *target, int sid, int rate);


/* Prep equip skill list */
struct PrepEquipSkillList {
    struct UnitListHeader header;
    u16 amt;
    u16 sid[0x3FF];
};

void ResetPrepEquipSkillList(void);
struct PrepEquipSkillList * GetPrepEquipSkillList(struct Unit * unit);

/* Game data */
#ifdef CONFIG_TURN_ON_ALL_SKILLS
    #define SKILL_ROM_DATA_AMT 5
#else
    #define SKILL_ROM_DATA_AMT 7 /* Unit can learn 7 skills on lv0/5/10/15/20 */
#endif

struct SkillPreloadJConf { u16 skills[SKILL_ROM_DATA_AMT * (UNIT_LEVEL_MAX_RE / 5 + 1)];};
struct SkillPreloadPConf { u16 skills[SKILL_ROM_DATA_AMT * (UNIT_RECORDED_LEVEL_MAX / 5 + 1)];};

extern const struct SkillPreloadJConf gSkillPreloadJData[0x100];
extern const struct SkillPreloadPConf gSkillPreloadPData[0x100];

extern struct SkillPreloadJConf const * const gpSkillPreloadJData;
extern struct SkillPreloadPConf const * const gpSkillPreloadPData;

enum SkillAnimPriorityConfig {
    EFX_PRIORITY_LOW = 0x1,
    EFX_PRIORITY_NORMAL,
    EFX_PRIORITY_HIGH,
    EFX_PRIORITY_HIGHHIGH,
};

extern u8 const * const gpEfxSkillAnimPriority;
extern struct EfxAnimConf const * const * const gpEfxSkillAnims;

int GetEfxSkillPriority(const u16 sid);

/**
 * Skill mapanim
 */
enum mapanimskillfx_idx {
    MAPANIMFX_CHR_L = 0x19C,
    MAPANIMFX_CHR_R = 0x19E,

    MAPANIMFX_PAL = 4,
};

struct ProcMapAnimSkillfx {
    PROC_HEADER;

    u8 pos;
    u32 icon_idx;
    int timer;
    int x, y;
};

extern u8 const * const gpImg_MapAnimSKILL;
extern const struct ProcCmd ProcScr_MapAnimSkillfx[];

void NewSkillMapAnimMini(int x, int y, u16 sid, ProcPtr parent);
bool SkillMapAnimMiniExists(void);

void AnimOnActiveUnit(u16 sid, void (* callback1)(ProcPtr proc), void (* callback2)(ProcPtr proc));
void NewMuSkillAnimOnActiveUnit(u16 sid, void (* callback1)(ProcPtr proc), void (* callback2)(ProcPtr proc));
bool MuSkillAnimExists(void);
void NewMuSkillAnimOnActiveUnitWithDeamon(ProcPtr parent, u16 sid, void (*callback1)(ProcPtr proc), void (*callback2)(ProcPtr proc));
void RemoveMuForActiveUnit(int delay);

extern const EventScr EventScr_MuSkillAnim[];

/**
 * Event scripts
 */
enum EventSkillSubOps {
    EVSUBCMD_ADD_SKILL = 1,
    EVSUBCMD_ADD_SKILL_ACTIVE,
    EVSUBCMD_ADD_SKILL_AT,
    EVSUBCMD_ADD_SKILL_SC,

    EVSUBCMD_REMOVE_SKILL,
    EVSUBCMD_REMOVE_SKILL_ACTIVE,
    EVSUBCMD_REMOVE_SKILL_AT,
    EVSUBCMD_REMOVE_SKILL_SC,
};

#define Evt_AddSkill(sid, pid) _EvtArg0(EVENT_CMD_SKILL, 4, EVSUBCMD_ADD_SKILL, sid), _EvtParams2(pid, 0),
#define Evt_AddSkillActive(sid) _EvtArg0(EVENT_CMD_SKILL, 4, EVSUBCMD_ADD_SKILL_ACTIVE, sid), _EvtParams2(0, 0),
#define Evt_AddSkillAt(sid, x, y) _EvtArg0(EVENT_CMD_SKILL, 4, EVSUBCMD_ADD_SKILL_AT, sid), _EvtParams2(x, y),
#define Evt_AddSkillSC(sid) _EvtArg0(EVENT_CMD_SKILL, 4, EVSUBCMD_ADD_SKILL_SC, sid), _EvtParams2(0, 0),

#define Evt_RemoveSkill(sid, pid) _EvtArg0(EVENT_CMD_SKILL, 4, EVSUBCMD_REMOVE_SKILL, sid), _EvtParams2(pid, 0),
#define Evt_RemoveSkillActive(sid) _EvtArg0(EVENT_CMD_SKILL, 4, EVSUBCMD_REMOVE_SKILL_ACTIVE, sid), _EvtParams2(0, 0),
#define Evt_RemoveSkillAt(sid, x, y) _EvtArg0(EVENT_CMD_SKILL, 4, EVSUBCMD_REMOVE_SKILL_AT, sid), _EvtParams2(x, y),
#define Evt_RemoveSkillSC(sid) _EvtArg0(EVENT_CMD_SKILL, 4, EVSUBCMD_REMOVE_SKILL_SC, sid), _EvtParams2(0, 0),

/**
 * Skill menu
 */
#define UNIT_MENU_SKILL_AMOUNT 4
extern u16 UnitMenuSkills[UNIT_MENU_SKILL_AMOUNT];
extern struct MenuItemDef const * const gpSkillMenuInfos;
#define GetSkillMenuInfo(sid) (&gpSkillMenuInfos[sid])

u8 MenuSkills_OnHelpBox(struct MenuProc * menu, struct MenuItemProc * item);
u8 MenuSkills_Usability(const struct MenuItemDef * self, int number);
int MenuSkills_OnDraw(struct MenuProc * menu, struct MenuItemProc * item);
u8 MenuSkills_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 MenuSkills_Idle(struct MenuProc * menu, struct MenuItemProc * item);
int MenuSkills_Hover(struct MenuProc * menu, struct MenuItemProc * item);
int MenuSkills_Unhover(struct MenuProc * menu, struct MenuItemProc * item);

#define MENU_SKILL_INDEX(menu_item) ((menu_item)->helpMsgId) /* We use rtext msg as index */
#define IS_SKILL_MENU_ITEM(menu_item) ((menu_item)->isAvailable == MenuSkills_Usability)

/**
 * Skill action
 */
typedef bool (* SkillActionFunc_t)(ProcPtr);
extern SkillActionFunc_t const * const gpSkillActionFuncTable;

/**
 * Skill scroll
 */
char * GetSkillScrollItemName(int item);
int GetSkillScrollItemDescId(int item);
int GetSkillScrollItemUseDescId(int item);
int GetSkillScrollItemIconId(int item);

extern const struct MenuDef RemoveSkillMenuDef;
extern const struct MenuDef PredationSkillMenuDef;
extern const struct MenuDef ScrollScribeMenuDef;
extern const struct ProcCmd ProcScr_SkillScrollUseSoftLock[];
extern const struct ProcCmd ProcScr_PredationSoftLock[];
extern const struct ProcCmd ProcScr_PredationPlusSoftLock[];

/**
 * Catch Em All
 */
#if defined(SID_CatchEmAll) && (COMMON_SKILL_VALID(SID_CatchEmAll))
    extern const unsigned int gCatchEmAllId;
#endif

/**
 * Miscs
 */

struct SkillExtraInfo {
    s8 priv[10];
};
extern struct SkillExtraInfo const * const gpSkillExtraInfo;
#define SKILL_EFF0(sid) (gpSkillExtraInfo[sid].priv[0])
#define SKILL_EFF1(sid) (gpSkillExtraInfo[sid].priv[1])
#define SKILL_EFF2(sid) (gpSkillExtraInfo[sid].priv[2])
#define SKILL_EFF3(sid) (gpSkillExtraInfo[sid].priv[3])
#define SKILL_EFF4(sid) (gpSkillExtraInfo[sid].priv[4])
#define SKILL_EFF5(sid) (gpSkillExtraInfo[sid].priv[5])
#define SKILL_EFF6(sid) (gpSkillExtraInfo[sid].priv[6])
#define SKILL_EFF7(sid) (gpSkillExtraInfo[sid].priv[7])
#define SKILL_EFF8(sid) (gpSkillExtraInfo[sid].priv[8])
#define SKILL_EFF9(sid) (gpSkillExtraInfo[sid].priv[9])


bool IsSkillLearned(struct Unit * unit, const u16 sid);
void LearnSkill(struct Unit * unit, const u16 sid);
void ForgetSkill(struct Unit * unit, const u16 sid);
void ResetUnitLearnedSkillLists(void);                      /* GameInitHook */
void SaveUnitLearnedSkillLists(u8 * dst, const u32 size);   /* SaveData */
void LoadUnitLearnedSkillLists(u8 * src, const u32 size);   /* LoadData */

void UnitAutoLoadSkills(struct Unit * unit);
int GetSkillSlot(struct Unit * unit, int sid);
int GetFreeSkillSlot(struct Unit * unit);
bool CanRemoveSkill(struct Unit * unit, const u16 sid);
int RemoveSkill(struct Unit * unit, const u16 sid);
int AddSkill(struct Unit * unit, const u16 sid);
void TryAddSkillLvup(struct Unit * unit, int level);
void TryAddSkillPromotion(struct Unit * unit, int jid);


/**
 * Popups
 */
void ResetPopupSkillStack(void);
void PushSkillListStack(u16 sid);
int PopSkillListStack(void);

int PoprGetLen_SkillIcon(struct PopupProc *proc, const struct PopupInstruction *inst);
void PoprDisp_SkillIcon(struct Text *text, const struct PopupInstruction *inst);
int PoprGetLen_SkillName(struct PopupProc *proc, const struct PopupInstruction *inst);
void PoprDisp_SkillName(struct Text *text, const struct PopupInstruction *inst);
bool PopR_SetupLearnSkill(void);
extern const struct PopupInstruction PopupScr_LearnSkill[];
extern const struct PopupInstruction PopupScr_ObtainedSkill[];

/**
 * External MiscSkillEffects
 */
bool GetTeleportationRandomPosition(struct Unit * unit, struct Vec2 * out);

/* lucky 7 */
enum skill_lucky_seven_idx {
    LUCKY7_POW = 0,
    LUCKY7_MAG,
    LUCKY7_SKL,
    LUCKY7_SPD,
    LUCKY7_LCK,
    LUCKY7_DEF,
    LUCKY7_RES,
    LUCKY7_MOV,
};

/* Legendary skill */
extern u8 const * const gpLegendSkillPool;
int TryActivateLegendSkill(struct Unit * unit, const u16 sid);
bool SkillTesterLegendActivated(struct Unit * unit, const u16 sid);
void PreBattleCalcLegendSkills(struct BattleUnit * attacker, struct BattleUnit * defender);
int SpdGetterLegendSkills(int status, struct Unit * unit);
int DefGetterLegendSkills(int status, struct Unit * unit);
int ResGetterLegendSkills(int status, struct Unit * unit);

/* Menu skills */
u8 HealingFocus_Usability(const struct MenuItemDef * def, int number);
u8 HealingFocus_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Teleportation_Usability(const struct MenuItemDef * def, int number);
u8 Teleportation_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 LightRune_Usability(const struct MenuItemDef * def, int number);
u8 LightRune_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Mine_Usability(const struct MenuItemDef * def, int number);
u8 Mine_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
int Rally_Hover(struct MenuProc * menu, struct MenuItemProc * item);
int Rally_Unhover(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 Rally_Usability(const struct MenuItemDef * def, int number);
u8 Rally_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
int GoddessDance_Hover(struct MenuProc * menu, struct MenuItemProc * item);
int GoddessDance_Unhover(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 GoddessDance_Usability(const struct MenuItemDef * def, int number);
u8 GoddessDance_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
int Stride_Hover(struct MenuProc * menu, struct MenuItemProc * item);
int Stride_Unhover(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 Stride_Usability(const struct MenuItemDef * def, int number);
u8 Stride_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Swarp_Usability(const struct MenuItemDef * def, int number);
u8 Swarp_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Blacksmith_Usability(const struct MenuItemDef * def, int number);
u8 Blacksmith_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Swap_Usability(const struct MenuItemDef * def, int number);
u8 Swap_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Transform_Usability(const struct MenuItemDef * def, int number);
u8 Transform_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 DrawBack_Usability(const struct MenuItemDef * def, int number);
u8 DrawBack_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Pivot_Usability(const struct MenuItemDef * def, int number);
u8 Pivot_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Reposition_Usability(const struct MenuItemDef * def, int number);
u8 Reposition_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Shove_Usability(const struct MenuItemDef * def, int number);
u8 Shove_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Smite_Usability(const struct MenuItemDef * def, int number);
u8 Smite_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 StealPlus_Usability(const struct MenuItemDef * def, int number);
u8 StealPlus_Effect(struct MenuProc* menu, struct MenuItemProc* menuItem);
u8 ArdentSacrifice_Usability(const struct MenuItemDef * def, int number);
u8 ArdentSacrifice_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 ReciprocalAid_Usability(const struct MenuItemDef * def, int number);
u8 ReciprocalAid_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Sacrifice_Usability(const struct MenuItemDef * def, int number);
u8 Sacrifice_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Capture_Usability(const struct MenuItemDef * def, int number);
u8 Capture_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Doppleganger_Usability(const struct MenuItemDef * def, int number);
u8 Doppleganger_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Dismount_Usability(const struct MenuItemDef * def, int number);
u8 Dismount_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Entrepreneur_Usability(const struct MenuItemDef * def, int number);
u8 Entrepreneur_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Arise_Usability(const struct MenuItemDef * def, int number);
u8 Arise_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 AssignDecoy_Usability(const struct MenuItemDef * def, int number);
u8 AssignDecoy_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 ShadowFlash_Usability(const struct MenuItemDef * def, int number);
u8 ShadowFlash_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
int ShadowFlash_Hover(struct MenuProc * menu, struct MenuItemProc * item);
int ShadowFlash_Unhover(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 Refuge_Usability(const struct MenuItemDef * def, int number);
u8 Refuge_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 DeathBlight_Usability(const struct MenuItemDef * def, int number);
u8 DeathBlight_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 DivineReprieve_Usability(const struct MenuItemDef *def, int number);
u8 DivineReprieve_OnSelected(struct MenuProc *menu, struct MenuItemProc *item);
u8 LoadstarRush_Usability(const struct MenuItemDef * def, int number);
u8 LoadstarRush_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 WyvernCrash_Usability(const struct MenuItemDef * def, int number);
u8 WyvernCrash_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Sellsword_Usability(const struct MenuItemDef * def, int number);
u8 Sellsword_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Switcher_Usability(const struct MenuItemDef * def, int number);
u8 Switcher_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 LethalityPlus_Usability(const struct MenuItemDef * def, int number);
u8 LethalityPlus_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 GrimReaper_Usability(const struct MenuItemDef * def, int number);
u8 GrimReaper_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Hide_Usability(const struct MenuItemDef * def, int number);
u8 Hide_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Salve_Usability(const struct MenuItemDef * def, int number);
u8 Salve_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 SkillSwap_Usability(const struct MenuItemDef * def, int number);
u8 SkillSwap_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Shuffle_Usability(const struct MenuItemDef * def, int number);
u8 Shuffle_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 ShufflePlus_Usability(const struct MenuItemDef * def, int number);
u8 ShufflePlus_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Persuade_Usability(const struct MenuItemDef * def, int number);
u8 Persuade_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 PersuadePlus_Usability(const struct MenuItemDef * def, int number);
u8 PersuadePlus_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
int Kamikaze_Hover(struct MenuProc * menu, struct MenuItemProc * item);
int Kamikaze_Unhover(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 Kamikaze_Usability(const struct MenuItemDef * def, int number);
u8 Kamikaze_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
int Reinforcements_Hover(struct MenuProc * menu, struct MenuItemProc * item);
int Reinforcements_Unhover(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 Reinforcements_Usability(const struct MenuItemDef * def, int number);
u8 Reinforcements_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 ResolvedHeart_Usability(const struct MenuItemDef * def, int number);
u8 ResolvedHeart_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Ballistary_Usability(const struct MenuItemDef * def, int number);
u8 Ballistary_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Bide_Usability(const struct MenuItemDef * def, int number);
u8 Bide_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 CoinFlip_Usability(const struct MenuItemDef * def, int number);
u8 CoinFlip_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Geomancy_Usability(const struct MenuItemDef * def, int number);
u8 Geomancy_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 ScrollScribePlus_Usability(const struct MenuItemDef * def, int number);
u8 ScrollScribePlus_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
int DancePlus_Hover(struct MenuProc * menu, struct MenuItemProc * item);
int DancePlus_Unhover(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 DancePlus_Usability(const struct MenuItemDef * def, int number);
u8 DancePlus_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Acidic_Usability(const struct MenuItemDef * def, int number);
u8 Acidic_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 AcidicPlus_Usability(const struct MenuItemDef * def, int number);
u8 AcidicPlus_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Reinforce_Usability(const struct MenuItemDef * def, int number);
u8 Reinforce_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 SealedZone_Usability(const struct MenuItemDef * def, int number);
u8 SealedZone_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Thrust_Usability(const struct MenuItemDef * def, int number);
u8 Thrust_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);

u8 Transform_Laguz_Usability(const struct MenuItemDef * def, int number);
u8 Transform_Laguz_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Transform_Laguz_Effect(struct MenuProc * menu, struct MenuItemProc * item);

/* Skill actions */
bool Action_HealingFocus(ProcPtr proc);
bool Action_Teleportation(ProcPtr parent);
bool Action_LightRune(ProcPtr parent);
bool Action_Mine(ProcPtr parent);
bool Action_Rally(ProcPtr parent);
bool Action_GoddessDance(ProcPtr parent);
bool Action_Stride(ProcPtr parent);
bool Action_Swarp(ProcPtr parent);
bool Action_Blacksmith(ProcPtr parent);
bool Action_Swap(ProcPtr parent);
bool Action_Transform(ProcPtr parent);
bool Action_DrawBack(ProcPtr parent);
bool Action_Pivot(ProcPtr parent);
bool Action_Reposition(ProcPtr parent);
bool Action_Shove(ProcPtr parent);
bool Action_Smite(ProcPtr parent);
bool Action_StealPlus(ProcPtr parent);
bool Action_ArdentSacrifice(ProcPtr parent);
bool Action_ReciprocalAid(ProcPtr parent);
bool Action_Sacrifice(ProcPtr parent);
bool Action_Capture(ProcPtr parent);
bool Action_Doppleganger(ProcPtr parent);
bool Action_Dismount(ProcPtr parent);
bool Action_Entrepreneur(ProcPtr parent);
bool Action_Arise(ProcPtr parent);
bool Action_AssignDecoy(ProcPtr parent);
bool Action_ShadowFlash(ProcPtr parent);
bool Action_Refuge(ProcPtr parent);
bool Action_DeathBlight(ProcPtr parent);
bool Action_DivineReprieve(ProcPtr parent);
bool Action_LoadstarRush(ProcPtr parent);
bool Action_WyvernCrash(ProcPtr parent);
bool Action_Sellsword(ProcPtr parent);
bool Action_Switcher(ProcPtr parent);
bool Action_LethalityPlus(ProcPtr parent);
bool Action_GrimReaper(ProcPtr parent);
bool Action_Hide(ProcPtr parent);
bool Action_Salve(ProcPtr parent);
bool Action_SkillSwap(ProcPtr parent);
bool Action_Shuffle(ProcPtr parent);
bool Action_ShufflePlus(ProcPtr parent);
bool Action_Persuade(ProcPtr parent);
bool Action_PersuadePlus(ProcPtr parent);
bool Action_Kamikaze(ProcPtr parent);
bool Action_Reinforcements(ProcPtr parent);
bool Action_ResolvedHeart(ProcPtr parent);
bool Action_Ballistary(ProcPtr parent);
bool Action_Bide(ProcPtr parent);
bool Action_CoinFlip(ProcPtr parent);
bool Action_Geomancy(ProcPtr parent);
bool Action_ScrollScribePlus(ProcPtr parent);
bool Action_DancePlus(ProcPtr parent);
bool Action_Acidic(ProcPtr parent);
bool Action_AcidicPlus(ProcPtr parent);
bool Action_Reinforce(ProcPtr parent);
bool Action_SealedZone(ProcPtr parent);
bool Action_Thrust(ProcPtr parent);

bool Action_Transform_Laguz(ProcPtr parent);

u8 SummonPlusCommandEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);