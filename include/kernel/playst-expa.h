#pragma once

#include "common-chax.h"

enum play_st_expa_bits {
	PLAYSTEXPA_BIT_DivineReprieve_Used,
	PLAYSTEXPA_BIT_DivineReprieve_InForce,
	PLAYSTEXPA_BIT_MasterForge_Used,
	PLAYSTEXPA_BIT_Duplicate_Used,
	PLAYSTEXPA_BIT_Reroll_Used,

	/* MAX */
	PLAYSTEXPA_BIT_USED,
	PLAYSTEXPA_BIT_MAX = 128,
};

extern u32 gPlayStExpa[4];

#define PlayStExpa_CheckBit(__bit) (_BIT_CHK(gPlayStExpa, __bit))
#define PlayStExpa_SetBit(__bit)   (_BIT_SET(gPlayStExpa, __bit))
#define PlayStExpa_ClearBit(__bit) (_BIT_CLR(gPlayStExpa, __bit))

void NewGame_InitPlayStExpa(void);
void ChapterInit_ResetPlayStExpa(void);
void EMS_SavePlayStExpa(u8 *dst, const u32 size);
void EMS_LoadPlayStExpa(u8 *src, const u32 size);
