#include "common-chax.h"
#include "jester_headers/soundtrack-ids.h"

const struct ROMChapterData Chapter02 = {
    .map = {
       .obj1Id = 1,
       .obj2Id= 0,
       .paletteId = 2,
       .tileConfigId = 3,
       .mainLayerId = 0xB, // Map pointer. In blocks of 4 per chapter
       .objAnimId = 5,
       .paletteAnimId = 0,
       .changeLayerId = 0xC,
    },
    .initialFogLevel = 0,
    .hasPrepScreen = FALSE,
    .chapTitleId = 0x02, // Chapter 2 - The Protected
    .chapTitleIdInHectorStory = 0,
    .initialPosX = 8,   // Starting position of the camera
    .initialPosY = 5,
    .initialWeather = WEATHER_FINE,
    .battleTileSet = 6,
    .easyModeLevelMalus = 0,
    .difficultModeLevelBonus = 0,
    .normalModeLevelMalus = 1,
    .mapBgmIds = {
        [MAP_BGM_BLUE] = BGM_DISTANT_ROADS,
        [MAP_BGM_RED] = BGM_SHADOW_OF_THE_ENEMY,
        [MAP_BGM_GREEN] = BGM_BINDING_VOW,
        [MAP_BGM_BLUE_HECTOR] = 13,
        [MAP_BGM_RED_HECTOR] = 21,
        [MAP_BGM_GREEN_HECTOR] = 15,
        [MAP_BGM_BLUE_GREEN_ALT] = 65535,
        [MAP_BGM_RED_ALT] = 65535,
        [MAP_BGM_PROLOGUE_LYN] = 11,
        [MAP_BGM_PROLOGUE] = 65535,
        [MAP_BGM_PROLOGUE_HECTOR] = 65535,
    },
    .mapCrackedWallHeath = 50,
    .mapEventDataId = 0xD,
    .gmapEventId = 4,
    .victorySongEnemyThreshold = BGM_THE_VALIANT,
    .fadeToBlack = TRUE,
    .statusObjectiveTextId = 0x1A5,
    .goalWindowTextId = 414,
    .goalWindowDataType = GOAL_TYPE_DEFEAT_ALL,
    .goalWindowEndTurnNumber = 0,
    .protectCharacterIndex = 0,
    .destPosX = 255,
    .destPosY = 0,
    .unk91 = 29,
    .unk92 = 0,
    .unk93 = 0,
};
