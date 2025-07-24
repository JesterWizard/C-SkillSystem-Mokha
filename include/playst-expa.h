#pragma once

#include "common-chax.h"

/* We use "inforce" as a means of signifying the skill is active for other checks and "used" to say no one else should be able to use it */
enum play_st_expa_bits {
	PLAYSTEXPA_BIT_DivineReprieve_Used,
	PLAYSTEXPA_BIT_DivineReprieve_InForce,
	PLAYSTEXPA_BIT_Shuffle_Used,
	PLAYSTEXPA_BIT_ShufflePlus_Used,
	PLAYSTEXPA_BIT_Persuade_Used,
	PLAYSTEXPA_BIT_PersuadePlus_Used,
	PLAYSTEXPA_BIT_Reinforcements_Used,
	PLAYSTEXPA_BIT_Ballistary_Used,
	PLAYSTEXPA_BIT_LethalityPlus_Used,
	PLAYSTEXPA_BIT_CoinFlip_Used, // Special case, we'll undo this once the skill has activated since its cost is fairly high as it is
	PLAYSTEXPA_BIT_AbsorbAlternation_InForce, 
	PLAYSTEXPA_BIT_SealedZone_Used,
	PLAYSTEXPA_BIT_Thrust_Used, 

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