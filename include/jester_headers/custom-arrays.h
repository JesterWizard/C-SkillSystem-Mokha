#include "common-chax.h"

extern const int statScreenDenyClasses[];
extern const int sizeOfDenyClasses;
extern const struct ProcCmd ProcScr_ManimLevelUp_CUSTOM[];
extern const struct ClassData gClassData_NEW[];
extern const struct SoundRoomEnt gSoundRoomTable_NEW[];

#ifdef CONFIG_CUSTOM_SUPPORT_CONVOS
    extern const struct SupportTalkEnt gNewSupportTalkList[];
#endif

struct MpSystemPInfoConfig 
{ 
    u8 idleGeneration; 
    u8 battleGeneration;
    u8 killGeneration;
    u8 maxMP;
};

extern const struct MpSystemPInfoConfig gMpSystemPInfoConfigList[];