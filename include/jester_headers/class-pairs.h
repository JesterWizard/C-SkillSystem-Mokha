// Predefine an array of key-value pairs
#include "common-chax.h"

// Declare extern variables
extern const int transformationPairs[1][2];
extern const int transformationListSize;

extern const int dopplegangerPairs[1][2];
extern const int dopplegangerListSize;

extern const int dismountPairs[14][2];
extern const int dismountListSize;

// Predefine an array of key-value pairs
const int dismountPairs[14][2] = {
    { CLASS_EIRIKA_MASTER_LORD, CLASS_EIRIKA_LORD },
    { CLASS_EPHRAIM_MASTER_LORD, CLASS_EPHRAIM_LORD },
    { CLASS_PALADIN, CLASS_SOLDIER },
    { CLASS_PALADIN_F, CLASS_SOLDIER },
    { CLASS_GREAT_KNIGHT, CLASS_SOLDIER },
    { CLASS_GREAT_KNIGHT_F, CLASS_SOLDIER },
    { CLASS_PEGASUS_KNIGHT, CLASS_SOLDIER },
    { CLASS_FALCON_KNIGHT, CLASS_SOLDIER },
    { CLASS_WYVERN_RIDER, CLASS_SOLDIER },
    { CLASS_WYVERN_RIDER_F, CLASS_SOLDIER },
    { CLASS_WYVERN_KNIGHT, CLASS_SOLDIER },
    { CLASS_WYVERN_KNIGHT_F, CLASS_SOLDIER },
    { CLASS_WYVERN_LORD, CLASS_SOLDIER },
    { CLASS_WYVERN_LORD_F, CLASS_SOLDIER },
};

// Define the size of the array
const int dismountListSize = sizeof(dismountPairs) / sizeof(dismountPairs[0]);