#include <common-chax.h>
#include <gaiden-magic.h>
#include <skill-system.h>
#include <constants/skills.h>
#include "jester_headers/custom-arrays.h"

const struct MpSystemPInfoConfig gMpSystemPInfoConfigList[0x100] = {
    [CHARACTER_EIRIKA] = {
        .generationRate = 5,
        .maxMP = 30
    },
    [CHARACTER_SETH] = {
        .generationRate = 3,
        .maxMP = 20,
    },
    [CHARACTER_GILLIAM] = {
		.generationRate = 0,
		.maxMP = 10,
	},
	[CHARACTER_FRANZ] = {
		.generationRate = 11,
		.maxMP = 22,
	},
	[CHARACTER_MOULDER] = {
		.generationRate = 3,
		.maxMP = 40,
	},
	[CHARACTER_VANESSA] = {
		.generationRate = 2,
		.maxMP = 40,
	},
	[CHARACTER_ROSS] = {
		.generationRate = 0,
		.maxMP = 10,
	},
	[CHARACTER_NEIMI] = {
		.generationRate = 1,
		.maxMP = 20,
	},
	[CHARACTER_COLM] = {
		.generationRate = 0,
		.maxMP = 10,
	},
	[CHARACTER_GARCIA] = {
		.generationRate = 0,
		.maxMP = 10,
	},
	[CHARACTER_INNES] = {
		.generationRate = 3,
		.maxMP = 15,
	},
	[CHARACTER_LUTE] = {
		.generationRate = 5,
		.maxMP = 65,
	},
	[CHARACTER_NATASHA] = {
		.generationRate = 1,
		.maxMP = 60,
	},
	[CHARACTER_CORMAG] = {
		.generationRate = 0,
		.maxMP = 10,
	},
	[CHARACTER_EPHRAIM] = {
		.generationRate = 0,
		.maxMP = 10,
	},
	[CHARACTER_FORDE] = {
		.generationRate = 2,
		.maxMP = 25,
	},
	[CHARACTER_KYLE] = {
		.generationRate = 0,
		.maxMP = 10,
	},
	[CHARACTER_AMELIA] = {
		.generationRate = 3,
		.maxMP = 30,
	},
	[CHARACTER_ARTUR] = {
		.generationRate = 5,
		.maxMP = 50,
	},
	[CHARACTER_GERIK] = {
		.generationRate = 0,
		.maxMP = 30,
	},
	[CHARACTER_TETHYS] = {
		.generationRate = 4,
		.maxMP = 40,
	},
	[CHARACTER_MARISA] = {
		.generationRate = 0,
		.maxMP = 10,
	},
	[CHARACTER_SALEH] = {
		.generationRate = 11,
		.maxMP = 30,
	},
	[CHARACTER_EWAN] = {
		.generationRate = 2,
		.maxMP = 45,
	},
	[CHARACTER_LARACHEL] = {
		.generationRate = 5,
		.maxMP = 50,
	},
	[CHARACTER_DOZLA] = {
		.generationRate = 2,
		.maxMP = 25,
	},
	[CHARACTER_RENNAC] = {
		.generationRate = 5,
		.maxMP = 35,
	},
	[CHARACTER_DUESSEL] = {
		.generationRate = 2,
		.maxMP = 10,
	},
	[CHARACTER_MYRRH] = {
		.generationRate = 6,
		.maxMP = 50,
	},
	[CHARACTER_KNOLL] = {
		.generationRate = 10,
		.maxMP = 50,
	},
	[CHARACTER_JOSHUA] = {
		.generationRate = 2,
		.maxMP = 30,
	},
	[CHARACTER_SYRENE] = {
		.generationRate = 4,
		.maxMP = 40,
	},
	[CHARACTER_TANA] = {
		.generationRate = 2,
		.maxMP = 30,
	},
    [CHARACTER_LYON_CC] = {
		.generationRate = 13,
		.maxMP = 50,
	},
	[CHARACTER_ORSON_CC] = {
		.generationRate = 0,
		.maxMP = 0,
	},
	[CHARACTER_GLEN_CC] = {
		.generationRate = 0,
		.maxMP = 10,
	},
	[CHARACTER_SELENA_CC] = {
		.generationRate = 10,
		.maxMP = 40,
	},
	[CHARACTER_VALTER_CC] = {
		.generationRate = 4,
		.maxMP = 20,
	},
	[CHARACTER_RIEV_CC] = {
		.generationRate = 10,
		.maxMP = 45,
	},
	[CHARACTER_CAELLACH_CC] = {
		.generationRate = 0,
		.maxMP = 0,
	},
	[CHARACTER_FADO_CC] = {
		.generationRate = 1,
		.maxMP = 10,
	},
	[CHARACTER_ISMAIRE_CC] = {
		.generationRate = 2,
		.maxMP = 20,
	},
	[CHARACTER_HAYDEN_CC] = {
		.generationRate = 0,
		.maxMP = 10,
	},
};