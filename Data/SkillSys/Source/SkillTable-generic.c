#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const struct SkillPreloadPConf gSkillPreloadPData[0x100] = {
	[CHARACTER_EIRIKA] = {{
		[0] = SID_HalfMP
	}},

	[CHARACTER_EPHRAIM] = {{
	}},

	[CHARACTER_SETH] = {{
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
