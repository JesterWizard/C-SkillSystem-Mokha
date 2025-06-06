#pragma once

#include "common-chax.h"
#include "efx-anim.h"
#include "skill-system.h"

#define COMBART_VALID(cid) (((cid) > 0) && ((cid) < 0xFF))
#define CA_WTYPE_ANY ((u8)(-1))

struct CombatArtInfo {
    const u8 * icon;
    u16 name, desc;

    u8 wtype;
    s8 range_bonus;
    s16 cost;

    struct {
        s8 atk, def, hit, avo, crit, silencer, dodge, display_en_n;
    } battle_status;

    u8 double_attack;
    u8 quintuple_attack;
    bool8 magic_attack;
    u8 effectiveness;

    /* debuffs */
    u8 debuff;
    bool8 aoe_debuff;

    u8 _pad_[7];
};

enum combat_art_effectiveness {
    /* CombatArtInfo::effectiveness */
    COMBART_EFF_NONE,
    COMBART_EFF_ALL,
    COMBART_EFF_ARMOR,
    COMBART_EFF_CAVALRY,
    COMBART_EFF_FLIER,
    COMBART_EFF_DRAGON,
    COMBART_EFF_MONSTER,
};

enum combat_art_double {
    /* CombatArtInfo::double_attack */
    COMBART_DOUBLE_DISABLED = 0,
    COMBART_DOUBLE_ENABLED,
    COMBART_DOUBLE_FORCE_ENABLED,
};

enum combat_art_quintuple {
    /* CombatArtInfo::double_attack */
    COMBART_QUINTUPLE_DISABLED = 0,
    COMBART_QUINTUPLE_ENABLED,
    COMBART_QUINTUPLE_FORCE_ENABLED,
};

extern const struct CombatArtInfo gCombatArtInfos[0x100];
extern struct CombatArtInfo const * const gpCombatArtInfos;

static inline const struct CombatArtInfo * GetCombatArtInfo(u8 cid)
{
    return &gpCombatArtInfos[cid];
}

static inline u16 GetCombatArtName(u8 cid)
{
    return GetCombatArtInfo(cid)->name;
}

static inline u16 GetCombatArtDesc(u8 cid)
{
    if (0 == GetCombatArtInfo(cid)->desc)
        return GetCombatArtInfo(cid)->name;

    return GetCombatArtInfo(cid)->desc;
}

/* Combat-art status */
struct CombatArtStatus {
    u8 cid;
    s8 uid;
    bool hitted;
    u8 x, y;
    u8 _pad_[0x10 - 0x8];
};

extern struct CombatArtStatus gCombatArtStatus;

u8 GetCombatArtInForce(struct Unit * unit);
void RegisterCombatArtStatus(struct Unit * unit, u8 cid);
void RegisterCombatArtHitted(void);
void RegisterCombatArtTargetPos(u8 x, u8 y);
void ResetCombatArtStatus(void);
void SaveCombatArtStatus(u8 * dst, const u32 size);
void LoadCombatArtStatus(u8 * src, const u32 size);

/* Combat-art list */
#define COMBART_LIST_MAX_AMT 6
struct CombatArtList {
    struct UnitListHeader ref;
    u8 wtype;
    u8 amt;
    u8 cid[COMBART_LIST_MAX_AMT];
};

struct CombatArtList * GetCombatArtList(struct Unit * unit, u8 wtype);
void ResetCombatArtList(void);

static inline struct CombatArtList * AutoGetCombatArtList(struct Unit * unit)
{
    return GetCombatArtList(unit, GetItemType(GetUnitEquippedWeapon(unit)));
}

struct CombatArtRomTable {
    u8 cid_sword[8];
    u8 cid_lance[8];
    u8 cid_axe[8];
    u8 cid_bow[8];
};

extern struct CombatArtRomTable const * const gpCombatArtRomPTable;
extern struct CombatArtRomTable const * const gpCombatArtRomJTable;
extern struct CombatArtRomTable const * const gpCombatArtDefaultTable;
extern u8 const * const gpCombatArtWeaponTable;
extern u8 const * const gpCombatArtSkillTable;

/* TargetSel & BKSEL */
ProcPtr NewTargetSelectionRework(const struct SelectInfo * selectInfo);
bool CombatArtSelectTargetExist(void);
u8 GetCombatArtByTargetSelIndex(void);

/* HelpBox related */
void DrawHelpBoxCombatArtBkselLabels(void);
void DrawHelpBoxCombatArtBkselStats(struct ProcHelpBoxIntro * proc);

/* EfxSkill */
extern u8 const * const gpEfxCombatArtAnimPriority;
extern struct EfxAnimConf const * const * const gpEfxCombatArtAnims;

void InitEfxCombatArtRoundData(void);
int GetEfxCombatArtPriority(const u8 cid);
void RegisterEfxSkillCombatArt(int round, const u8 cid);
u8 GetEfxCombatArt(int round);

/* Menu */
u8 CombatArtActionCommandUsability(const struct MenuItemDef * def, int number);
int CombatArtActionCommandOnDarw(struct MenuProc * menu, struct MenuItemProc * item);
u8 CombatArtActionCommandEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);
int CombatArtActionCommandHover(struct MenuProc * menu, struct MenuItemProc * menuItem);
int CombatArtActionCommandUnhover(struct MenuProc * menu, struct MenuItemProc * menuItem);
extern const struct MenuDef gCombatArtItemMenuDef;
extern const struct SelectInfo gSelectInfoCombatArt;

/* Misc */
bool CanUnitPlayCombatArt(struct Unit * unit, u16 item);
u8 GetBestRangeBonusCid(struct Unit * unit, u16 item);
const u8 * GetCombatArtIcon(const u8 cid);
int WeaponRangeGetterCombatArt(int range, struct Unit * unit, u16 item);
void PreBattleCalcCombatArt(struct BattleUnit * bu, struct BattleUnit * defender);
bool PostActionCombatArtEffect(ProcPtr parent);
