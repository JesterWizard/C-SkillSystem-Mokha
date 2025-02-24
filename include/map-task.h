#pragma once

enum MapTaskConfIndex {
<<<<<<< HEAD
    MTSKCONF_HPBAR,
    MTSKCONF_COMM_BUFF,
    MTSKCONF_COMM_DEBUFF,
    MTSKCONF_RESCUING,
    MTSKCONF_BOSS,
    MTSKCONF_PROTECTION,
    MTSKCONF_TALK
=======
	MTSKCONF_HPBAR,
	MTSKCONF_COMM_BUFF,
	MTSKCONF_COMM_DEBUFF,
	MTSKCONF_RESCUING,
	MTSKCONF_BOSS,
	MTSKCONF_PROTECTION,
>>>>>>> 7b86e9495edda39a0eb0d27d352d8795a134d7fc
};

#include "common-chax.h"

struct MapTaskConf {
	u16 x, y;
	u16 *obj;
};

// extern struct MapTaskConf const gMapTaskConfs[];
extern struct MapTaskConf const *const gpMapTaskConfs;

// extern u8 const gHpBarChr[];
extern u8 const *const gpHpBarChr;

extern struct Vec2u MapTaskVec;

extern void (*_MapTaskPutOamHi)(int type, int oam2);
#define MapTaskPutOamHi _MapTaskPutOamHi
