#include "common-chax.h"
#include "bwl.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "battle-system.h"
#include "eventinfo.h"
#include "../../../../Contants/Texts/build/msgs.h"
#include "jester_headers/soundtrack-ids.h"

static const int sSupportUnkLut[][2] = {
    { 0x0100, 0x0100 }, 
    { 0x0000, 0x0000 }
};

#define LOCAL_TRACE 0

static u8 * GetUnitBwlSupports(u8 pid)
{
    struct NewBwl * bwl = GetNewBwl(pid);
    if (bwl)
        return bwl->supports;

    return NULL;
}

static const EventListScr EventScr_NewMapSupportConversation[] = {
    EVBIT_MODIFY(0x3)
    BEQ(0x0, EVT_SLOT_2, EVT_SLOT_0)
    MUSC(0xffff)
    GOTO(0x1)
LABEL(0x0)
    MUSI
LABEL(0x1)
    SADD(EVT_SLOT_2, EVT_SLOT_5, EVT_SLOT_0)
    TEXTSHOW(0xffff)
    TEXTEND
    REMA
    BEQ(0x2, EVT_SLOT_4, EVT_SLOT_6)
    GOTO(0x3) // This isn't an A support and/or these aren't the required support partners, so we skip giving the item

LABEL(0x2)
    GIVEITEMTO(CHARACTER_EIRIKA)
    SVAL(EVT_SLOT_4, 0)

LABEL(0x3)
    SVAL(EVT_SLOT_6, 0)
    NOTIFY(0xc, SFX_UPDATE)
    EVBIT_T(7)
    ENDA
};

LYN_REPLACE_CHECK(CallMapSupportEvent);
void CallMapSupportEvent(u16 musicIndex, u16 textIndex) {
    // Calls text with music (just quiets music when id is 0)
    // On-map supports?
    CallEvent((u16 *)EventScr_NewMapSupportConversation, EV_EXEC_CUTSCENE);

    gEventSlots[0x2] = musicIndex;
    gEventSlots[0x5] = textIndex;
}

LYN_REPLACE_CHECK(CallSupportViewerEvent);
void CallSupportViewerEvent(u16 textIndex) {
    // Calls text with random background (support viewer?)
    CallEvent((u16 *)EventScr_SupportViewerConversation, EV_EXEC_QUIET);

    gEventSlots[0x2] = textIndex;
}

//! FE8U = 0x080847F8
LYN_REPLACE_CHECK(GetSupportTalkList);
struct SupportTalkEnt* GetSupportTalkList(void) {
    return (struct SupportTalkEnt*)gNewSupportTalkList;
}

//! FE8U = 0x08084748
LYN_REPLACE_CHECK(GetSupportTalkEntry);
struct SupportTalkEnt * GetSupportTalkEntry(u16 pidA, u16 pidB)
{
    const struct SupportTalkEnt * it;

    for (it = gNewSupportTalkList; it->unitA != 0xFFFF; it++)
    {
        if ((pidA == it->unitA) && (pidB == it->unitB))
            return (struct SupportTalkEnt *)it;  // Cast const away only on return
            
        if ((pidB == it->unitA) && (pidA == it->unitB))
            return (struct SupportTalkEnt *)it;  // Cast const away only on return
    }
    
    return NULL;
}

//! FE8U = 0x080a3724
LYN_REPLACE_CHECK(UpdateBestGlobalSupportValue);
bool UpdateBestGlobalSupportValue(int unitA, int unitB, int supportRank) {
    int convo;
    int var0;
    int var1;
    struct GlobalSaveInfo info;
    struct SupportTalkEnt* ptr;

    supportRank = supportRank & 3;

    if (!ReadGlobalSaveInfo(&info)) {
        return 0;
    }

    convo = 0;

    for (ptr = GetSupportTalkList(); ; ptr++) {

        if (ptr->unitA == 0xFFFF)
            break;

        if ((ptr->unitA == unitA) && (ptr->unitB == unitB))
            break;

        if ((ptr->unitA == unitB) && (ptr->unitB == unitA))
            break;

        convo++;
    }

    var0 = convo >> 2;
    var1 = (convo & 3) << 1;

    if (((info.SuppordRecord[var0] >> var1) & 3) >= (supportRank))
        return false;

    info.SuppordRecord[var0] &= ~(3 << var1);
    info.SuppordRecord[var0] += (supportRank << var1);

    WriteGlobalSaveInfo(&info);

    return true;
}

//! FE8U = 0x0808371C
LYN_REPLACE_CHECK(StartSupportTalk);
void StartSupportTalk(u8 pidA, u8 pidB, int rank) {
    struct SupportTalkEnt* ent = GetSupportTalkEntry(pidA, pidB);
    
    /* If this is an A rank support, set up slot 4 for item rewards based on the support partners */
#ifdef CONFIG_SUPPORT_REWARDS
    if (rank == 3)
    {
        gEventSlots[0x3] = ITEM_ANIMA_EXCALIBUR;
        gEventSlots[0x4] = 100; // Check against this value to see if the item should be given
    }
#endif

    gEventSlots[0x6] = 100;

    if (ent) {
        CallMapSupportEvent(
            GetSupportTalkSong(pidA, pidB, rank),
            ent->msgSupports[rank - 1]
        );

        UpdateBestGlobalSupportValue(pidA, pidB, rank);
    }

    return;
}

LYN_REPLACE_CHECK(ActionSupport);
s8 ActionSupport(ProcPtr proc)
{
    int subjectExp;
    int targetExp;
    u8 * supp1, * supp2;

    struct Unit * target = GetUnit(gActionData.targetIndex);

    int targetSupportNum = GetUnitSupporterNum(gActiveUnit, target->pCharacterData->number);
    int subjectSupportNum = GetUnitSupporterNum(target, gActiveUnit->pCharacterData->number);

    CanUnitSupportNow(target, subjectSupportNum);

    UnitGainSupportLevel(gActiveUnit, targetSupportNum);
    UnitGainSupportLevel(target, subjectSupportNum);

    StartSupportTalk(
        gActiveUnit->pCharacterData->number,
        target->pCharacterData->number,
        GetUnitSupportLevel(gActiveUnit, targetSupportNum)
    );

    supp1 = GetUnitBwlSupports(UNIT_CHAR_ID(gActiveUnit));
    supp2 = GetUnitBwlSupports(UNIT_CHAR_ID(target));

    if (supp1 && supp2)
    {
        subjectExp = supp1[targetSupportNum];
        targetExp = supp2[subjectSupportNum];

        if (subjectExp != targetExp)
        {
            if (subjectExp > targetExp)
                supp2[subjectSupportNum] = subjectExp;

            if (subjectExp < targetExp)
                supp1[targetSupportNum] = targetExp;
        }
    }

    return 0;
}

LYN_REPLACE_CHECK(GetUnitSupportLevel);
int GetUnitSupportLevel(struct Unit * unit, int num)
{
    int exp;
    u8 * supp;

    if (!UNIT_IS_VALID(unit))
        return SUPPORT_LEVEL_NONE;

    supp = GetUnitBwlSupports(UNIT_CHAR_ID(unit));
    if (supp)
    {
        exp = supp[num];
        // LTRACEF("[pid=%d, num=%d] exp=%d at BWL", UNIT_CHAR_ID(unit), num, exp);
    }
    else
    {
        /**
         * for none-BWL characters,
         * directly judge on its rom data
         */
        exp = unit->pCharacterData->pSupportData->supportExpBase[num];
        // LTRACEF("[pid=%d, num=%d] exp=%d at ROM", UNIT_CHAR_ID(unit), num, exp);
    }

    if (exp > 240)
        return SUPPORT_LEVEL_A;

    if (exp > 160)
        return SUPPORT_LEVEL_B;

    if (exp > 80)
        return SUPPORT_LEVEL_C;

    return SUPPORT_LEVEL_NONE;
}

LYN_REPLACE_CHECK(UnitGainSupportExp);
void UnitGainSupportExp(struct Unit * unit, int num)
{
    u8 * supp = GetUnitBwlSupports(UNIT_CHAR_ID(unit));

    if (UNIT_SUPPORT_DATA(unit) && supp)
    {
        int gain = UNIT_SUPPORT_DATA(unit)->supportExpGrowth[num];
#ifdef CONFIG_VESLY_SUPPORT_POST_BATTLE
    if (gBattleActorGlobalFlag.enemy_defeated)
        gain += SUPPORT_RATE_KILL;
    else if (gActionData.unitActionType == UNIT_ACTION_COMBAT)
        gain += SUPPORT_RATE_COMBAT;
    else if (gActionData.unitActionType == UNIT_ACTION_DANCE)
        gain += SUPPORT_RATE_DANCE;
    else if (gActionData.unitActionType == UNIT_ACTION_STAFF)
        gain += SUPPORT_RATE_STAFF;
#else
        gain = UNIT_SUPPORT_DATA(unit)->supportExpGrowth[num];
#endif
        int currentExp = supp[num];
        int maxExp = sSupportMaxExpLookup[GetUnitSupportLevel(unit, num)];

        FORCE_DECLARE struct Unit * other = GetUnitSupporterUnit(unit, num);

#if defined(SID_SocialButterfly) && (COMMON_SKILL_VALID(SID_SocialButterfly))
        if (SkillTester(unit, SID_SocialButterfly) || SkillTester(other, SID_SocialButterfly))
            gain *= 2;
#endif

        if (currentExp + gain > maxExp)
            gain = maxExp - currentExp;

        supp[num] = currentExp + gain;
        gPlaySt.chapterTotalSupportGain += gain;
    }
}

LYN_REPLACE_CHECK(UnitGainSupportLevel);
void UnitGainSupportLevel(struct Unit* unit, int num)
{
    u8 * supp = GetUnitBwlSupports(UNIT_CHAR_ID(unit));
    if (supp)
        supp[num]++;

    gPlaySt.chapterTotalSupportGain++;

    SetSupportLevelGained(unit->pCharacterData->number, GetUnitSupporterCharacter(unit, num));
}

LYN_REPLACE_CHECK(CanUnitSupportNow);
s8 CanUnitSupportNow(struct Unit * unit, int num)
{
    int exp, maxExp;
    u8 * supp = GetUnitBwlSupports(UNIT_CHAR_ID(unit));

    if (gPlaySt.chapterStateBits & PLAY_FLAG_EXTRA_MAP)
        return FALSE;

    if (gPlaySt.chapterStateBits & PLAY_FLAG_TUTORIAL)
        return FALSE;

    if (HasUnitGainedSupportLevel(unit, num))
        return FALSE;

    if (GetUnitTotalSupportLevel(unit) >= MAX_SIMULTANEOUS_SUPPORT_COUNT)
        return FALSE;

    if (GetUnitTotalSupportLevel(GetUnitSupporterUnit(unit, num)) >= MAX_SIMULTANEOUS_SUPPORT_COUNT)
        return FALSE;

    if (!supp)
        return FALSE;

    exp    = supp[num];
    maxExp = sSupportMaxExpLookup[GetUnitSupportLevel(unit, num)];

    if (exp == SUPPORT_EXP_A)
        return FALSE;

    return (exp == maxExp) ? TRUE : FALSE;
}

LYN_REPLACE_CHECK(InitUnitsupports);
void InitUnitsupports(struct Unit * unit)
{
    u8 * supp1, * supp2;
    int i, count = GetUnitSupporterCount(unit);

    for (i = 0; i < count; ++i)
    {
        struct Unit * other = GetUnitSupporterUnit(unit, i);

        if (!other)
            continue;

        supp1 = GetUnitBwlSupports(UNIT_CHAR_ID(unit));
        supp2 = GetUnitBwlSupports(UNIT_CHAR_ID(other));

        if (!supp1 || !supp2)
            continue;

        supp2[GetUnitSupporterNum(other, unit->pCharacterData->number)] = 0;
        supp1[i] = 0;
    }
}

LYN_REPLACE_CHECK(UnitLoadSupports);
void UnitLoadSupports(struct Unit * unit)
{
    int i, count = GetUnitSupporterCount(unit);
    u8 * supp = GetUnitBwlSupports(UNIT_CHAR_ID(unit));
    if (supp)
    {
        CpuFill16(0, supp, UNIT_SUPPORT_MAX_COUNT);

        for (i = 0; i < count; ++i)
            supp[i] = GetUnitSupporterInitialExp(unit, i);
    }
}

/* We need to hook this to view the support conversation menu */
LYN_REPLACE_CHECK(GetGlobalSupportListFromSave);
void GetGlobalSupportListFromSave(int unitId, u8* data, struct GlobalSaveInfo* info)
{
    struct GlobalSaveInfo local_info;
    struct SupportTalkEnt* ptr;
    int i;
    int j;

    if (gCharacterData[unitId-1].pSupportData == 0) {
        for (i = 0; i < UNIT_SUPPORT_MAX_COUNT; data++, i++)
            *data = 0;

        return;
    }

    j = 0;
    ptr = GetSupportTalkList();

    if (info == NULL) {
        info = &local_info;
        ReadGlobalSaveInfo(info);
    }

    for (; ; j++, ptr++) {
        int tmp1, tmp2;

        if (ptr->unitA == 0xFFFF)
            break;

        if ((ptr->unitA != unitId) && (ptr->unitB != unitId))
            continue;

        tmp1 = j >> 2;
        tmp2 = (j & 3) << 1;

        for (i = 0; i < gCharacterData[unitId-1].pSupportData->supportCount; i++) {

            if ((ptr->unitA != gCharacterData[unitId-1].pSupportData->characters[i]) &&
                (ptr->unitB != gCharacterData[unitId-1].pSupportData->characters[i])) {
                continue;
            }

            data[i] = (info->SuppordRecord[tmp1] >> (tmp2)) & 3;

            break;
        }
    }

    for (i = gCharacterData[unitId-1].pSupportData->supportCount; i < UNIT_SUPPORT_MAX_COUNT; i++) {
        data[i] = 0;
    }

    return;
}

LYN_REPLACE_CHECK(GetUnitsAverageSupportValue);
int GetUnitsAverageSupportValue(const int unitA, const int unitB)
{
    int i;

    for (i = 0; 0 != sSupportUnkLut[i][0]; i++) {
        if (sSupportUnkLut[i][0] == unitA)
            if (sSupportUnkLut[i][1] != unitB)
                return 2;

        if (sSupportUnkLut[i][0] == unitB)
            if (sSupportUnkLut[i][1] != unitA)
                return 2;
            
        if (sSupportUnkLut[i][1] == unitA)
            if (sSupportUnkLut[i][0] != unitB)
                return 2;

        if (sSupportUnkLut[i][1] == unitB)
            if (sSupportUnkLut[i][0] != unitA)
                return 2;
    }

    return 3;
}

LYN_REPLACE_CHECK(GetTotalAverageSupportValue);
int GetTotalAverageSupportValue()
{
    int ret = 0;
    struct SupportTalkEnt *buf = GetSupportTalkList();

    for (; 0xFFFF != buf->unitA; buf++)
        ret += GetUnitsAverageSupportValue(buf->unitA, buf->unitB);

    return ret;
}


//! FE8U = 0x080A0B20
LYN_REPLACE_CHECK(GetSupportScreenPartnerCharId);
int GetSupportScreenPartnerCharId(int idx, int partner) {
    return gCharacterData[GetSupportScreenCharIdAt(idx) - 1].pSupportData->characters[partner];
}