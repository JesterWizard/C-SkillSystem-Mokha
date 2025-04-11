#ifndef GUARD_BM_RELIANCE_H
#define GUARD_BM_RELIANCE_H

#include "global.h"

enum
{
    // Config

    MAX_SIMULTANEOUS_SUPPORT_COUNT = 5,
    SUPPORT_BONUSES_MAX_DISTANCE = 3,
};

enum
{
    SUPPORT_LEVEL_NONE,
    SUPPORT_LEVEL_C,
    SUPPORT_LEVEL_B,
    SUPPORT_LEVEL_A,
};

enum
{
    SUPPORT_EXP_C = 81,
    SUPPORT_EXP_B = 161,
    SUPPORT_EXP_A = 241,
};

struct SupportData
{
    /* 00 */ u8 characters[UNIT_SUPPORT_MAX_COUNT];
    /* 07 */ u8 supportExpBase[UNIT_SUPPORT_MAX_COUNT];
    /* 0E */ u8 supportExpGrowth[UNIT_SUPPORT_MAX_COUNT];
    /* 15 */ u8 supportCount;
};

struct SupportBonuses
{
    /* 00 */ u8 affinity;

    /* 01 */ u8 bonusAttack;
    /* 02 */ u8 bonusDefense;
    /* 03 */ u8 bonusHit;
    /* 04 */ u8 bonusAvoid;
    /* 05 */ u8 bonusCrit;
    /* 06 */ u8 bonusDodge;
};

extern struct SupportData CONST_DATA gSupportData[];
extern CONST_DATA struct SupportBonuses gAffinityBonuses[];

int GetUnitSupporterCount(struct Unit* unit);
u8 GetUnitSupporterCharacter(struct Unit* unit, int num);
struct Unit* GetUnitSupporterUnit(struct Unit* unit, int num);
int GetUnitSupportLevel(struct Unit* unit, int num);
int GetUnitTotalSupportLevel(struct Unit* unit);
void UnitGainSupportExp(struct Unit* unit, int num);
void UnitGainSupportLevel(struct Unit* unit, int num);
s8 CanUnitSupportNow(struct Unit* unit, int num);
int GetUnitSupporterInitialExp(struct Unit* unit, int num);
int GetUnitSupporterNum(struct Unit* unit, u8 charId);
void InitUnitsupports(struct Unit* unit);
void ProcessTurnSupportExp(void);
int GetUnitSupportBonuses(struct Unit* unit, struct SupportBonuses* bonuses);
int GetUnitAffinityIcon(struct Unit* unit);
int GetCharacterAffinityIcon(int characterId);
int GetSupportLevelUiChar(int level);
char* GetAffinityName(int affinity); // unused?
s8 HaveCharactersMaxSupport(u8 charA, u8 charB);
void SwapUnitStats(struct Unit* unitA, struct Unit* unitB);

extern CONST_DATA struct SupportData SupportData_Eirika;
extern CONST_DATA struct SupportData SupportData_Ephraim;
extern CONST_DATA struct SupportData SupportData_Seth;
extern CONST_DATA struct SupportData SupportData_Franz;
extern CONST_DATA struct SupportData SupportData_Gilliam;
extern CONST_DATA struct SupportData SupportData_Moulder;
extern CONST_DATA struct SupportData SupportData_Vanessa;
extern CONST_DATA struct SupportData SupportData_Garcia;
extern CONST_DATA struct SupportData SupportData_Ross;
extern CONST_DATA struct SupportData SupportData_Colm;
extern CONST_DATA struct SupportData SupportData_Neimi;
extern CONST_DATA struct SupportData SupportData_Artur;
extern CONST_DATA struct SupportData SupportData_Lute;
extern CONST_DATA struct SupportData SupportData_Natasha;
extern CONST_DATA struct SupportData SupportData_Joshua;
extern CONST_DATA struct SupportData SupportData_Kyle;
extern CONST_DATA struct SupportData SupportData_Forde;
extern CONST_DATA struct SupportData SupportData_Tana;
extern CONST_DATA struct SupportData SupportData_Innes;
extern CONST_DATA struct SupportData SupportData_Gerik;
extern CONST_DATA struct SupportData SupportData_Tethys;
extern CONST_DATA struct SupportData SupportData_Marisa;
extern CONST_DATA struct SupportData SupportData_Saleh;
extern CONST_DATA struct SupportData SupportData_Ewan;
extern CONST_DATA struct SupportData SupportData_LArachel;
extern CONST_DATA struct SupportData SupportData_Dozla;
extern CONST_DATA struct SupportData SupportData_Cormag;
extern CONST_DATA struct SupportData SupportData_Amelia;
extern CONST_DATA struct SupportData SupportData_Rennac;
extern CONST_DATA struct SupportData SupportData_Duessel;
extern CONST_DATA struct SupportData SupportData_Myrrh;
extern CONST_DATA struct SupportData SupportData_Knoll;
extern CONST_DATA struct SupportData SupportData_Syrene;

extern const struct SupportData SupportDataNew_Eirika;
extern const struct SupportData SupportDataNew_Ephraim;
extern const struct SupportData SupportDataNew_Seth;
extern const struct SupportData SupportDataNew_Franz;
extern const struct SupportData SupportDataNew_Gilliam;
extern const struct SupportData SupportDataNew_Moulder;
extern const struct SupportData SupportDataNew_Vanessa;
extern const struct SupportData SupportDataNew_Garcia;
extern const struct SupportData SupportDataNew_Ross;
extern const struct SupportData SupportDataNew_Colm;
extern const struct SupportData SupportDataNew_Neimi;
extern const struct SupportData SupportDataNew_Artur;
extern const struct SupportData SupportDataNew_Lute;
extern const struct SupportData SupportDataNew_Natasha;
extern const struct SupportData SupportDataNew_Joshua;
extern const struct SupportData SupportDataNew_Kyle;
extern const struct SupportData SupportDataNew_Forde;
extern const struct SupportData SupportDataNew_Tana;
extern const struct SupportData SupportDataNew_Innes;
extern const struct SupportData SupportDataNew_Gerik;
extern const struct SupportData SupportDataNew_Tethys;
extern const struct SupportData SupportDataNew_Marisa;
extern const struct SupportData SupportDataNew_Saleh;
extern const struct SupportData SupportDataNew_Ewan;
extern const struct SupportData SupportDataNew_LArachel;
extern const struct SupportData SupportDataNew_Dozla;
extern const struct SupportData SupportDataNew_Cormag;
extern const struct SupportData SupportDataNew_Amelia;
extern const struct SupportData SupportDataNew_Rennac;
extern const struct SupportData SupportDataNew_Duessel;
extern const struct SupportData SupportDataNew_Myrrh;
extern const struct SupportData SupportDataNew_Knoll;
extern const struct SupportData SupportDataNew_Syrene;

#endif // GUARD_BM_RELIANCE_H