#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

/* 
** If CONFIG_TURN_ALL_SKILLS_ON is enabled
** then you are limited to 5 skills here, otherwise it's 7
*/

const struct SkillPreloadPConf gSkillPreloadPData[0x100] = {
	[CHARACTER_EIRIKA] = {{
		[0] = SID_Pyrotechnics
	}},

	[CHARACTER_EPHRAIM] = {{
	}},

	[CHARACTER_SETH] = {{
		[0] = SID_Replicate,
		[1] = SID_Blacksmith,
		[2] = SID_Sol,
		[3] = SID_Adept,
		[4] = SID_Acrobat
	}},

	[CHARACTER_INNES] = {{
	}},

	[CHARACTER_SALEH] = {{
	}},

	[CHARACTER_MYRRH] = {{
	}},
};

const struct SkillPreloadJConf gSkillPreloadJData[0x100] = {
	[CLASS_SNIPER] = {{
	}},

	[CLASS_SNIPER_F] = {{
	}},

	[CLASS_MANAKETE_MYRRH] = {{
	}},
};
