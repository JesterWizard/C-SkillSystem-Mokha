#include <common-chax.h>
#include <gaiden-magic.h>
#include <skill-system.h>
#include <constants/skills.h>
#include "jester_headers/custom-arrays.h"

/* CONFIG_MP_SYSTEM */

const struct MpSystemPInfoConfig gMpSystemPInfoConfigList[0x100] = {
	[CHARACTER_EIRIKA] = {
		.idleGeneration = 30,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 30
	},
	[CHARACTER_SETH] = {
		.idleGeneration = 3,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 20,
	},
	[CHARACTER_GILLIAM] = {
		.idleGeneration = 0,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 10,
	},
	[CHARACTER_FRANZ] = {
		.idleGeneration = 11,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 22,
	},
	[CHARACTER_MOULDER] = {
		.idleGeneration = 3,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 40,
	},
	[CHARACTER_VANESSA] = {
		.idleGeneration = 2,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 40,
	},
	[CHARACTER_ROSS] = {
		.idleGeneration = 0,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 10,
	},
	[CHARACTER_NEIMI] = {
		.idleGeneration = 1,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 20,
	},
	[CHARACTER_COLM] = {
		.idleGeneration = 0,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 10,
	},
	[CHARACTER_GARCIA] = {
		.idleGeneration = 0,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 10,
	},
	[CHARACTER_INNES] = {
		.idleGeneration = 3,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 15,
	},
	[CHARACTER_LUTE] = {
		.idleGeneration = 5,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 65,
	},
	[CHARACTER_NATASHA] = {
		.idleGeneration = 1,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 60,
	},
	[CHARACTER_CORMAG] = {
		.idleGeneration = 0,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 10,
	},
	[CHARACTER_EPHRAIM] = {
		.idleGeneration = 0,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 10,
	},
	[CHARACTER_FORDE] = {
		.idleGeneration = 2,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 25,
	},
	[CHARACTER_KYLE] = {
		.idleGeneration = 0,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 10,
	},
	[CHARACTER_AMELIA] = {
		.idleGeneration = 3,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 30,
	},
	[CHARACTER_ARTUR] = {
		.idleGeneration = 5,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 50,
	},
	[CHARACTER_GERIK] = {
		.idleGeneration = 0,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 30,
	},
	[CHARACTER_TETHYS] = {
		.idleGeneration = 4,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 40,
	},
	[CHARACTER_MARISA] = {
		.idleGeneration = 0,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 10,
	},
	[CHARACTER_SALEH] = {
		.idleGeneration = 11,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 30,
	},
	[CHARACTER_EWAN] = {
		.idleGeneration = 2,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 45,
	},
	[CHARACTER_LARACHEL] = {
		.idleGeneration = 5,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 50,
	},
	[CHARACTER_DOZLA] = {
		.idleGeneration = 2,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 25,
	},
	[CHARACTER_RENNAC] = {
		.idleGeneration = 5,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 35,
	},
	[CHARACTER_DUESSEL] = {
		.idleGeneration = 2,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 10,
	},
	[CHARACTER_MYRRH] = {
		.idleGeneration = 6,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 50,
	},
	[CHARACTER_KNOLL] = {
		.idleGeneration = 10,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 50,
	},
	[CHARACTER_JOSHUA] = {
		.idleGeneration = 2,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 30,
	},
	[CHARACTER_SYRENE] = {
		.idleGeneration = 4,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 40,
	},
	[CHARACTER_TANA] = {
		.idleGeneration = 2,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 30,
	},
	[CHARACTER_LYON_CC] = {
		.idleGeneration = 13,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 50,
	},
	[CHARACTER_ORSON_CC] = {
		.idleGeneration = 0,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 0,
	},
	[CHARACTER_GLEN_CC] = {
		.idleGeneration = 0,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 10,
	},
	[CHARACTER_SELENA_CC] = {
		.idleGeneration = 10,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 40,
	},
	[CHARACTER_VALTER_CC] = {
		.idleGeneration = 4,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 20,
	},
	[CHARACTER_RIEV_CC] = {
		.idleGeneration = 10,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 45,
	},
	[CHARACTER_CAELLACH_CC] = {
		.idleGeneration = 0,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 0,
	},
	[CHARACTER_FADO_CC] = {
		.idleGeneration = 1,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 10,
	},
	[CHARACTER_ISMAIRE_CC] = {
		.idleGeneration = 2,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 20,
	},
	[CHARACTER_HAYDEN_CC] = {
		.idleGeneration = 0,
		.battleGeneration = 2,
		.killGeneration = 5,
		.maxMP = 10,
	},
};