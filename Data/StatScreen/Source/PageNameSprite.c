#include "common-chax.h"

STATIC_DECLAR const u16 Sprite_Page0NameRework[] = {
	3,
	0x4104, 0x9008, TILEREF(0, 0),
	0x4104, 0x9028, TILEREF(4, 0),
	0x4104, 0x9048, TILEREF(8, 0),
};

STATIC_DECLAR const u16 Sprite_Page1NameRework[] = {
	2,
	0x4104, 0x901E, TILEREF(0, 0),
	0x4104, 0x903E, TILEREF(4, 0),
};

STATIC_DECLAR const u16 Sprite_Page2NameRework[] = {
	5,
	0x4108, 0x9004, TILEREF(6,  0),
	0x4108, 0x9024, TILEREF(10, 0),
	0x4108, 0x9044, TILEREF(14, 0),
	0x4100, 0x9000, TILEREF(0,  0),
	0x0100, 0x5020, TILEREF(4,  0),
};

STATIC_DECLAR const u16 Sprite_Page3NameRework[] = {
	3,
	0x4104, 0x9008, TILEREF(0, 0),
	0x4104, 0x9028, TILEREF(4, 0),
	0x4104, 0x9048, TILEREF(8, 0),
};

STATIC_DECLAR const u16 Sprite_Page4NameRework[] = {
	3,
	0x4104, 0x9008, TILEREF(14, 0),
	0x4104, 0x9028, TILEREF(18, 0),
	0x4104, 0x9048, TILEREF(22, 0),
};

STATIC_DECLAR const u16 Sprite_Page5NameRework[] =
{
    3,
    0x4104, 0x9008, TILEREF(14, 0),
    0x4104, 0x9028, TILEREF(18, 0),
    0x4104, 0x9048, TILEREF(22, 0),
};


STATIC_DECLAR const u16 Sprite_Page6NameRework[] =
{
    3,
    0x4104, 0x9008, TILEREF(14, 0),
    0x4104, 0x9028, TILEREF(18, 0),
    0x4104, 0x9048, TILEREF(22, 0),
};

u16 const *const Sprites_PageNameRework[] = {
		Sprite_Page0NameRework,
		Sprite_Page1NameRework,
		Sprite_Page2NameRework,
		Sprite_Page3NameRework,
		Sprite_Page4NameRework,
#ifdef CONFIG_MP_SYSTEM
		Sprite_Page5NameRework,
#endif

#ifdef CONFIG_STAT_PAGE_PERSONAL_INFO
		Sprite_Page6NameRework,
#endif
};

#ifdef CONFIG_MP_SYSTEM
	const u16 gPageNameChrOffsetLutRe[] = {
		0x00, 0x40, 0x0E, 0x80, 0x80, 0x80
	};
#else
	const u16 gPageNameChrOffsetLutRe[] = {
		0x00, 0x40, 0x0E, 0x80, 0x80
	};
#endif