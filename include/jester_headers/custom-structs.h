
// extern u16 sBEXP[CONFIG_BEXP];

// struct CustomGlobalsSt { // Chapter Data Struct
//     /* 00 */ u16 bexp;
// };

// extern EWRAM_DATA struct CustomGlobalSt gCustomGlobalSt;

// typedef struct
// {
//     const int values[4];
// } Forge;

// static const Forge forge_values[] = {
//     {0, 0, 0, 0},
//     {0, 0, 0, 5},
//     {0, 0, 0, 10},
//     {0, 0, 0, 15},
//     {0, 0, 0, 20},
//     {0, 0, 0, 25},
//     {0, 0, 1, 0},
//     {0, 0, 1, 5},
//     {0, 0, 1, 10},
//     {0, 0, 1, 15},
//     {0, 0, 1, 20},
//     {0, 0, 1, 25},
//     {0, 0, 2, 0},
//     {0, 0, 2, 5},
//     {0, 0, 2, 10},
//     {0, 0, 2, 15},
//     {0, 0, 2, 20},
//     {0, 0, 2, 25},
//     {0, 0, 3, 0},
//     {0, 0, 3, 5},
//     {0, 0, 3, 10},
//     {0, 0, 3, 15},
//     {0, 0, 3, 20},
//     {0, 0, 3, 25},
//     {0, 0, 4, 0},
//     {0, 0, 4, 5},
//     {0, 0, 4, 10},
//     {0, 0, 4, 15},
//     {0, 0, 4, 20},
//     {0, 0, 4, 25},
//     {0, 0, 5, 0},
//     {0, 0, 5, 5},
//     {0, 0, 5, 10},
//     {0, 0, 5, 15},
//     {0, 0, 5, 20},
//     {0, 0, 5, 25},
//     {0, 5, 0, 0},
//     {0, 5, 0, 5},
//     {0, 5, 0, 10},
//     {0, 5, 0, 15},
//     {0, 5, 0, 20},
//     {0, 5, 0, 25},
//     {0, 5, 1, 0},
//     {0, 5, 1, 5},
//     {0, 5, 1, 10},
//     {0, 5, 1, 15},
//     {0, 5, 1, 20},
//     {0, 5, 1, 25},
//     {0, 5, 2, 0},
//     {0, 5, 2, 5},
//     {0, 5, 2, 10},
//     {0, 5, 2, 15},
//     {0, 5, 2, 20},
//     {0, 5, 2, 25},
//     {0, 5, 3, 0},
//     {0, 5, 3, 5},
//     {0, 5, 3, 10},
//     {0, 5, 3, 15},
//     {0, 5, 3, 20},
//     {0, 5, 3, 25},
//     {0, 5, 4, 0},
//     {0, 5, 4, 5},
//     {0, 5, 4, 10},
//     {0, 5, 4, 15},
//     {0, 5, 4, 20},
//     {0, 5, 4, 25},
//     {0, 5, 5, 0},
//     {0, 5, 5, 5},
//     {0, 5, 5, 10},
//     {0, 5, 5, 15},
//     {0, 5, 5, 20},
//     {0, 5, 5, 25},
//     {0, 10, 0, 0},
//     {0, 10, 0, 5},
//     {0, 10, 0, 10},
//     {0, 10, 0, 15},
//     {0, 10, 0, 20},
//     {0, 10, 0, 25},
//     {0, 10, 1, 0},
//     {0, 10, 1, 5},
//     {0, 10, 1, 10},
//     {0, 10, 1, 15},
//     {0, 10, 1, 20},
//     {0, 10, 1, 25},
//     {0, 10, 2, 0},
//     {0, 10, 2, 5},
//     {0, 10, 2, 10},
//     {0, 10, 2, 15},
//     {0, 10, 2, 20},
//     {0, 10, 2, 25},
//     {0, 10, 3, 0},
//     {0, 10, 3, 5},
//     {0, 10, 3, 10},
//     {0, 10, 3, 15},
//     {0, 10, 3, 20},
//     {0, 10, 3, 25},
//     {0, 10, 4, 0},
//     {0, 10, 4, 5},
//     {0, 10, 4, 10},
//     {0, 10, 4, 15},
//     {0, 10, 4, 20},
//     {0, 10, 4, 25},
//     {0, 10, 5, 0},
//     {0, 10, 5, 5},
//     {0, 10, 5, 10},
//     {0, 10, 5, 15},
//     {0, 10, 5, 20},
//     {0, 10, 5, 25},
//     {0, 15, 0, 0},
//     {0, 15, 0, 5},
//     {0, 15, 0, 10},
//     {0, 15, 0, 15},
//     {0, 15, 0, 20},
//     {0, 15, 0, 25},
//     {0, 15, 1, 0},
//     {0, 15, 1, 5},
//     {0, 15, 1, 10},
//     {0, 15, 1, 15},
//     {0, 15, 1, 20},
//     {0, 15, 1, 25},
//     {0, 15, 2, 0},
//     {0, 15, 2, 5},
//     {0, 15, 2, 10},
//     {0, 15, 2, 15},
//     {0, 15, 2, 20},
//     {0, 15, 2, 25},
//     {0, 15, 3, 0},
//     {0, 15, 3, 5},
//     {0, 15, 3, 10},
//     {0, 15, 3, 15},
//     {0, 15, 3, 20},
//     {0, 15, 3, 25},
//     {0, 15, 4, 0},
//     {0, 15, 4, 5},
//     {0, 15, 4, 10},
//     {0, 15, 4, 15},
//     {0, 15, 4, 20},
//     {0, 15, 4, 25},
//     {0, 15, 5, 0},
//     {0, 15, 5, 5},
//     {0, 15, 5, 10},
//     {0, 15, 5, 15},
//     {0, 15, 5, 20},
//     {0, 15, 5, 25},
//     {0, 20, 0, 0},
//     {0, 20, 0, 5},
//     {0, 20, 0, 10},
//     {0, 20, 0, 15},
//     {0, 20, 0, 20},
//     {0, 20, 0, 25},
//     {0, 20, 1, 0},
//     {0, 20, 1, 5},
//     {0, 20, 1, 10},
//     {0, 20, 1, 15},
//     {0, 20, 1, 20},
//     {0, 20, 1, 25},
//     {0, 20, 2, 0},
//     {0, 20, 2, 5},
//     {0, 20, 2, 10},
//     {0, 20, 2, 15},
//     {0, 20, 2, 20},
//     {0, 20, 2, 25},
//     {0, 20, 3, 0},
//     {0, 20, 3, 5},
//     {0, 20, 3, 10},
//     {0, 20, 3, 15},
//     {0, 20, 3, 20},
//     {0, 20, 3, 25},
//     {0, 20, 4, 0},
//     {0, 20, 4, 5},
//     {0, 20, 4, 10},
//     {0, 20, 4, 15},
//     {0, 20, 4, 20},
//     {0, 20, 4, 25},
//     {0, 20, 5, 0},
//     {0, 20, 5, 5},
//     {0, 20, 5, 10},
//     {0, 20, 5, 15},
//     {0, 20, 5, 20},
//     {0, 20, 5, 25},
//     {0, 25, 0, 0},
//     {0, 25, 0, 5},
//     {0, 25, 0, 10},
//     {0, 25, 0, 15},
//     {0, 25, 0, 20},
//     {0, 25, 0, 25},
//     {0, 25, 1, 0},
//     {0, 25, 1, 5},
//     {0, 25, 1, 10},
//     {0, 25, 1, 15},
//     {0, 25, 1, 20},
//     {0, 25, 1, 25},
//     {0, 25, 2, 0},
//     {0, 25, 2, 5},
//     {0, 25, 2, 10},
//     {0, 25, 2, 15},
//     {0, 25, 2, 20},
//     {0, 25, 2, 25},
//     {0, 25, 3, 0},
//     {0, 25, 3, 5},
//     {0, 25, 3, 10},
//     {0, 25, 3, 15},
//     {0, 25, 3, 20},
//     {0, 25, 3, 25},
//     {0, 25, 4, 0},
//     {0, 25, 4, 5},
//     {0, 25, 4, 10},
//     {0, 25, 4, 15},
//     {0, 25, 4, 20},
//     {0, 25, 4, 25},
//     {0, 25, 5, 0},
//     {0, 25, 5, 5},
//     {0, 25, 5, 10},
//     {0, 25, 5, 15},
//     {0, 25, 5, 20},
//     {0, 25, 5, 25},
//     {1, 0, 0, 0},
//     {1, 0, 0, 5},
//     {1, 0, 0, 10},
//     {1, 0, 0, 15},
//     {1, 0, 0, 20},
//     {1, 0, 0, 25},
//     {1, 0, 1, 0},
//     {1, 0, 1, 5},
//     {1, 0, 1, 10},
//     {1, 0, 1, 15},
//     {1, 0, 1, 20},
//     {1, 0, 1, 25},
//     {1, 0, 2, 0},
//     {1, 0, 2, 5},
//     {1, 0, 2, 10},
//     {1, 0, 2, 15},
//     {1, 0, 2, 20},
//     {1, 0, 2, 25},
//     {1, 0, 3, 0},
//     {1, 0, 3, 5},
//     {1, 0, 3, 10},
//     {1, 0, 3, 15},
//     {1, 0, 3, 20},
//     {1, 0, 3, 25},
//     {1, 0, 4, 0},
//     {1, 0, 4, 5},
//     {1, 0, 4, 10},
//     {1, 0, 4, 15},
//     {1, 0, 4, 20},
//     {1, 0, 4, 25},
//     {1, 0, 5, 0},
//     {1, 0, 5, 5},
//     {1, 0, 5, 10},
//     {1, 0, 5, 15},
//     {1, 0, 5, 20},
//     {1, 0, 5, 25},
//     {1, 5, 0, 0},
//     {1, 5, 0, 5},
//     {1, 5, 0, 10},
//     {1, 5, 0, 15},
//     {1, 5, 0, 20},
//     {1, 5, 0, 25},
//     {1, 5, 1, 0},
//     {1, 5, 1, 5},
//     {1, 5, 1, 10},
//     {1, 5, 1, 15},
//     {1, 5, 1, 20},
//     {1, 5, 1, 25},
//     {1, 5, 2, 0},
//     {1, 5, 2, 5},
//     {1, 5, 2, 10},
//     {1, 5, 2, 15},
//     {1, 5, 2, 20},
//     {1, 5, 2, 25},
//     {1, 5, 3, 0},
//     {1, 5, 3, 5},
//     {1, 5, 3, 10},
//     {1, 5, 3, 15},
//     {1, 5, 3, 20},
//     {1, 5, 3, 25},
//     {1, 5, 4, 0},
//     {1, 5, 4, 5},
//     {1, 5, 4, 10},
//     {1, 5, 4, 15},
//     {1, 5, 4, 20},
//     {1, 5, 4, 25},
//     {1, 5, 5, 0},
//     {1, 5, 5, 5},
//     {1, 5, 5, 10},
//     {1, 5, 5, 15},
//     {1, 5, 5, 20},
//     {1, 5, 5, 25},
//     {1, 10, 0, 0},
//     {1, 10, 0, 5},
//     {1, 10, 0, 10},
//     {1, 10, 0, 15},
//     {1, 10, 0, 20},
//     {1, 10, 0, 25},
//     {1, 10, 1, 0},
//     {1, 10, 1, 5},
//     {1, 10, 1, 10},
//     {1, 10, 1, 15},
//     {1, 10, 1, 20},
//     {1, 10, 1, 25},
//     {1, 10, 2, 0},
//     {1, 10, 2, 5},
//     {1, 10, 2, 10},
//     {1, 10, 2, 15},
//     {1, 10, 2, 20},
//     {1, 10, 2, 25},
//     {1, 10, 3, 0},
//     {1, 10, 3, 5},
//     {1, 10, 3, 10},
//     {1, 10, 3, 15},
//     {1, 10, 3, 20},
//     {1, 10, 3, 25},
//     {1, 10, 4, 0},
//     {1, 10, 4, 5},
//     {1, 10, 4, 10},
//     {1, 10, 4, 15},
//     {1, 10, 4, 20},
//     {1, 10, 4, 25},
//     {1, 10, 5, 0},
//     {1, 10, 5, 5},
//     {1, 10, 5, 10},
//     {1, 10, 5, 15},
//     {1, 10, 5, 20},
//     {1, 10, 5, 25},
//     {1, 15, 0, 0},
//     {1, 15, 0, 5},
//     {1, 15, 0, 10},
//     {1, 15, 0, 15},
//     {1, 15, 0, 20},
//     {1, 15, 0, 25},
//     {1, 15, 1, 0},
//     {1, 15, 1, 5},
//     {1, 15, 1, 10},
//     {1, 15, 1, 15},
//     {1, 15, 1, 20},
//     {1, 15, 1, 25},
//     {1, 15, 2, 0},
//     {1, 15, 2, 5},
//     {1, 15, 2, 10},
//     {1, 15, 2, 15},
//     {1, 15, 2, 20},
//     {1, 15, 2, 25},
//     {1, 15, 3, 0},
//     {1, 15, 3, 5},
//     {1, 15, 3, 10},
//     {1, 15, 3, 15},
//     {1, 15, 3, 20},
//     {1, 15, 3, 25},
//     {1, 15, 4, 0},
//     {1, 15, 4, 5},
//     {1, 15, 4, 10},
//     {1, 15, 4, 15},
//     {1, 15, 4, 20},
//     {1, 15, 4, 25},
//     {1, 15, 5, 0},
//     {1, 15, 5, 5},
//     {1, 15, 5, 10},
//     {1, 15, 5, 15},
//     {1, 15, 5, 20},
//     {1, 15, 5, 25},
//     {1, 20, 0, 0},
//     {1, 20, 0, 5},
//     {1, 20, 0, 10},
//     {1, 20, 0, 15},
//     {1, 20, 0, 20},
//     {1, 20, 0, 25},
//     {1, 20, 1, 0},
//     {1, 20, 1, 5},
//     {1, 20, 1, 10},
//     {1, 20, 1, 15},
//     {1, 20, 1, 20},
//     {1, 20, 1, 25},
//     {1, 20, 2, 0},
//     {1, 20, 2, 5},
//     {1, 20, 2, 10},
//     {1, 20, 2, 15},
//     {1, 20, 2, 20},
//     {1, 20, 2, 25},
//     {1, 20, 3, 0},
//     {1, 20, 3, 5},
//     {1, 20, 3, 10},
//     {1, 20, 3, 15},
//     {1, 20, 3, 20},
//     {1, 20, 3, 25},
//     {1, 20, 4, 0},
//     {1, 20, 4, 5},
//     {1, 20, 4, 10},
//     {1, 20, 4, 15},
//     {1, 20, 4, 20},
//     {1, 20, 4, 25},
//     {1, 20, 5, 0},
//     {1, 20, 5, 5},
//     {1, 20, 5, 10},
//     {1, 20, 5, 15},
//     {1, 20, 5, 20},
//     {1, 20, 5, 25},
//     {1, 25, 0, 0},
//     {1, 25, 0, 5},
//     {1, 25, 0, 10},
//     {1, 25, 0, 15},
//     {1, 25, 0, 20},
//     {1, 25, 0, 25},
//     {1, 25, 1, 0},
//     {1, 25, 1, 5},
//     {1, 25, 1, 10},
//     {1, 25, 1, 15},
//     {1, 25, 1, 20},
//     {1, 25, 1, 25},
//     {1, 25, 2, 0},
//     {1, 25, 2, 5},
//     {1, 25, 2, 10},
//     {1, 25, 2, 15},
//     {1, 25, 2, 20},
//     {1, 25, 2, 25},
//     {1, 25, 3, 0},
//     {1, 25, 3, 5},
//     {1, 25, 3, 10},
//     {1, 25, 3, 15},
//     {1, 25, 3, 20},
//     {1, 25, 3, 25},
//     {1, 25, 4, 0},
//     {1, 25, 4, 5},
//     {1, 25, 4, 10},
//     {1, 25, 4, 15},
//     {1, 25, 4, 20},
//     {1, 25, 4, 25},
//     {1, 25, 5, 0},
//     {1, 25, 5, 5},
//     {1, 25, 5, 10},
//     {1, 25, 5, 15},
//     {1, 25, 5, 20},
//     {1, 25, 5, 25},
//     {2, 0, 0, 0},
//     {2, 0, 0, 5},
//     {2, 0, 0, 10},
//     {2, 0, 0, 15},
//     {2, 0, 0, 20},
//     {2, 0, 0, 25},
//     {2, 0, 1, 0},
//     {2, 0, 1, 5},
//     {2, 0, 1, 10},
//     {2, 0, 1, 15},
//     {2, 0, 1, 20},
//     {2, 0, 1, 25},
//     {2, 0, 2, 0},
//     {2, 0, 2, 5},
//     {2, 0, 2, 10},
//     {2, 0, 2, 15},
//     {2, 0, 2, 20},
//     {2, 0, 2, 25},
//     {2, 0, 3, 0},
//     {2, 0, 3, 5},
//     {2, 0, 3, 10},
//     {2, 0, 3, 15},
//     {2, 0, 3, 20},
//     {2, 0, 3, 25},
//     {2, 0, 4, 0},
//     {2, 0, 4, 5},
//     {2, 0, 4, 10},
//     {2, 0, 4, 15},
//     {2, 0, 4, 20},
//     {2, 0, 4, 25},
//     {2, 0, 5, 0},
//     {2, 0, 5, 5},
//     {2, 0, 5, 10},
//     {2, 0, 5, 15},
//     {2, 0, 5, 20},
//     {2, 0, 5, 25},
//     {2, 5, 0, 0},
//     {2, 5, 0, 5},
//     {2, 5, 0, 10},
//     {2, 5, 0, 15},
//     {2, 5, 0, 20},
//     {2, 5, 0, 25},
//     {2, 5, 1, 0},
//     {2, 5, 1, 5},
//     {2, 5, 1, 10},
//     {2, 5, 1, 15},
//     {2, 5, 1, 20},
//     {2, 5, 1, 25},
//     {2, 5, 2, 0},
//     {2, 5, 2, 5},
//     {2, 5, 2, 10},
//     {2, 5, 2, 15},
//     {2, 5, 2, 20},
//     {2, 5, 2, 25},
//     {2, 5, 3, 0},
//     {2, 5, 3, 5},
//     {2, 5, 3, 10},
//     {2, 5, 3, 15},
//     {2, 5, 3, 20},
//     {2, 5, 3, 25},
//     {2, 5, 4, 0},
//     {2, 5, 4, 5},
//     {2, 5, 4, 10},
//     {2, 5, 4, 15},
//     {2, 5, 4, 20},
//     {2, 5, 4, 25},
//     {2, 5, 5, 0},
//     {2, 5, 5, 5},
//     {2, 5, 5, 10},
//     {2, 5, 5, 15},
//     {2, 5, 5, 20},
//     {2, 5, 5, 25},
//     {2, 10, 0, 0},
//     {2, 10, 0, 5},
//     {2, 10, 0, 10},
//     {2, 10, 0, 15},
//     {2, 10, 0, 20},
//     {2, 10, 0, 25},
//     {2, 10, 1, 0},
//     {2, 10, 1, 5},
//     {2, 10, 1, 10},
//     {2, 10, 1, 15},
//     {2, 10, 1, 20},
//     {2, 10, 1, 25},
//     {2, 10, 2, 0},
//     {2, 10, 2, 5},
//     {2, 10, 2, 10},
//     {2, 10, 2, 15},
//     {2, 10, 2, 20},
//     {2, 10, 2, 25},
//     {2, 10, 3, 0},
//     {2, 10, 3, 5},
//     {2, 10, 3, 10},
//     {2, 10, 3, 15},
//     {2, 10, 3, 20},
//     {2, 10, 3, 25},
//     {2, 10, 4, 0},
//     {2, 10, 4, 5},
//     {2, 10, 4, 10},
//     {2, 10, 4, 15},
//     {2, 10, 4, 20},
//     {2, 10, 4, 25},
//     {2, 10, 5, 0},
//     {2, 10, 5, 5},
//     {2, 10, 5, 10},
//     {2, 10, 5, 15},
//     {2, 10, 5, 20},
//     {2, 10, 5, 25},
//     {2, 15, 0, 0},
//     {2, 15, 0, 5},
//     {2, 15, 0, 10},
//     {2, 15, 0, 15},
//     {2, 15, 0, 20},
//     {2, 15, 0, 25},
//     {2, 15, 1, 0},
//     {2, 15, 1, 5},
//     {2, 15, 1, 10},
//     {2, 15, 1, 15},
//     {2, 15, 1, 20},
//     {2, 15, 1, 25},
//     {2, 15, 2, 0},
//     {2, 15, 2, 5},
//     {2, 15, 2, 10},
//     {2, 15, 2, 15},
//     {2, 15, 2, 20},
//     {2, 15, 2, 25},
//     {2, 15, 3, 0},
//     {2, 15, 3, 5},
//     {2, 15, 3, 10},
//     {2, 15, 3, 15},
//     {2, 15, 3, 20},
//     {2, 15, 3, 25},
//     {2, 15, 4, 0},
//     {2, 15, 4, 5},
//     {2, 15, 4, 10},
//     {2, 15, 4, 15},
//     {2, 15, 4, 20},
//     {2, 15, 4, 25},
//     {2, 15, 5, 0},
//     {2, 15, 5, 5},
//     {2, 15, 5, 10},
//     {2, 15, 5, 15},
//     {2, 15, 5, 20},
//     {2, 15, 5, 25},
//     {2, 20, 0, 0},
//     {2, 20, 0, 5},
//     {2, 20, 0, 10},
//     {2, 20, 0, 15},
//     {2, 20, 0, 20},
//     {2, 20, 0, 25},
//     {2, 20, 1, 0},
//     {2, 20, 1, 5},
//     {2, 20, 1, 10},
//     {2, 20, 1, 15},
//     {2, 20, 1, 20},
//     {2, 20, 1, 25},
//     {2, 20, 2, 0},
//     {2, 20, 2, 5},
//     {2, 20, 2, 10},
//     {2, 20, 2, 15},
//     {2, 20, 2, 20},
//     {2, 20, 2, 25},
//     {2, 20, 3, 0},
//     {2, 20, 3, 5},
//     {2, 20, 3, 10},
//     {2, 20, 3, 15},
//     {2, 20, 3, 20},
//     {2, 20, 3, 25},
//     {2, 20, 4, 0},
//     {2, 20, 4, 5},
//     {2, 20, 4, 10},
//     {2, 20, 4, 15},
//     {2, 20, 4, 20},
//     {2, 20, 4, 25},
//     {2, 20, 5, 0},
//     {2, 20, 5, 5},
//     {2, 20, 5, 10},
//     {2, 20, 5, 15},
//     {2, 20, 5, 20},
//     {2, 20, 5, 25},
//     {2, 25, 0, 0},
//     {2, 25, 0, 5},
//     {2, 25, 0, 10},
//     {2, 25, 0, 15},
//     {2, 25, 0, 20},
//     {2, 25, 0, 25},
//     {2, 25, 1, 0},
//     {2, 25, 1, 5},
//     {2, 25, 1, 10},
//     {2, 25, 1, 15},
//     {2, 25, 1, 20},
//     {2, 25, 1, 25},
//     {2, 25, 2, 0},
//     {2, 25, 2, 5},
//     {2, 25, 2, 10},
//     {2, 25, 2, 15},
//     {2, 25, 2, 20},
//     {2, 25, 2, 25},
//     {2, 25, 3, 0},
//     {2, 25, 3, 5},
//     {2, 25, 3, 10},
//     {2, 25, 3, 15},
//     {2, 25, 3, 20},
//     {2, 25, 3, 25},
//     {2, 25, 4, 0},
//     {2, 25, 4, 5},
//     {2, 25, 4, 10},
//     {2, 25, 4, 15},
//     {2, 25, 4, 20},
//     {2, 25, 4, 25},
//     {2, 25, 5, 0},
//     {2, 25, 5, 5},
//     {2, 25, 5, 10},
//     {2, 25, 5, 15},
//     {2, 25, 5, 20},
//     {2, 25, 5, 25},
//     {3, 0, 0, 0},
//     {3, 0, 0, 5},
//     {3, 0, 0, 10},
//     {3, 0, 0, 15},
//     {3, 0, 0, 20},
//     {3, 0, 0, 25},
//     {3, 0, 1, 0},
//     {3, 0, 1, 5},
//     {3, 0, 1, 10},
//     {3, 0, 1, 15},
//     {3, 0, 1, 20},
//     {3, 0, 1, 25},
//     {3, 0, 2, 0},
//     {3, 0, 2, 5},
//     {3, 0, 2, 10},
//     {3, 0, 2, 15},
//     {3, 0, 2, 20},
//     {3, 0, 2, 25},
//     {3, 0, 3, 0},
//     {3, 0, 3, 5},
//     {3, 0, 3, 10},
//     {3, 0, 3, 15},
//     {3, 0, 3, 20},
//     {3, 0, 3, 25},
//     {3, 0, 4, 0},
//     {3, 0, 4, 5},
//     {3, 0, 4, 10},
//     {3, 0, 4, 15},
//     {3, 0, 4, 20},
//     {3, 0, 4, 25},
//     {3, 0, 5, 0},
//     {3, 0, 5, 5},
//     {3, 0, 5, 10},
//     {3, 0, 5, 15},
//     {3, 0, 5, 20},
//     {3, 0, 5, 25},
//     {3, 5, 0, 0},
//     {3, 5, 0, 5},
//     {3, 5, 0, 10},
//     {3, 5, 0, 15},
//     {3, 5, 0, 20},
//     {3, 5, 0, 25},
//     {3, 5, 1, 0},
//     {3, 5, 1, 5},
//     {3, 5, 1, 10},
//     {3, 5, 1, 15},
//     {3, 5, 1, 20},
//     {3, 5, 1, 25},
//     {3, 5, 2, 0},
//     {3, 5, 2, 5},
//     {3, 5, 2, 10},
//     {3, 5, 2, 15},
//     {3, 5, 2, 20},
//     {3, 5, 2, 25},
//     {3, 5, 3, 0},
//     {3, 5, 3, 5},
//     {3, 5, 3, 10},
//     {3, 5, 3, 15},
//     {3, 5, 3, 20},
//     {3, 5, 3, 25},
//     {3, 5, 4, 0},
//     {3, 5, 4, 5},
//     {3, 5, 4, 10},
//     {3, 5, 4, 15},
//     {3, 5, 4, 20},
//     {3, 5, 4, 25},
//     {3, 5, 5, 0},
//     {3, 5, 5, 5},
//     {3, 5, 5, 10},
//     {3, 5, 5, 15},
//     {3, 5, 5, 20},
//     {3, 5, 5, 25},
//     {3, 10, 0, 0},
//     {3, 10, 0, 5},
//     {3, 10, 0, 10},
//     {3, 10, 0, 15},
//     {3, 10, 0, 20},
//     {3, 10, 0, 25},
//     {3, 10, 1, 0},
//     {3, 10, 1, 5},
//     {3, 10, 1, 10},
//     {3, 10, 1, 15},
//     {3, 10, 1, 20},
//     {3, 10, 1, 25},
//     {3, 10, 2, 0},
//     {3, 10, 2, 5},
//     {3, 10, 2, 10},
//     {3, 10, 2, 15},
//     {3, 10, 2, 20},
//     {3, 10, 2, 25},
//     {3, 10, 3, 0},
//     {3, 10, 3, 5},
//     {3, 10, 3, 10},
//     {3, 10, 3, 15},
//     {3, 10, 3, 20},
//     {3, 10, 3, 25},
//     {3, 10, 4, 0},
//     {3, 10, 4, 5},
//     {3, 10, 4, 10},
//     {3, 10, 4, 15},
//     {3, 10, 4, 20},
//     {3, 10, 4, 25},
//     {3, 10, 5, 0},
//     {3, 10, 5, 5},
//     {3, 10, 5, 10},
//     {3, 10, 5, 15},
//     {3, 10, 5, 20},
//     {3, 10, 5, 25},
//     {3, 15, 0, 0},
//     {3, 15, 0, 5},
//     {3, 15, 0, 10},
//     {3, 15, 0, 15},
//     {3, 15, 0, 20},
//     {3, 15, 0, 25},
//     {3, 15, 1, 0},
//     {3, 15, 1, 5},
//     {3, 15, 1, 10},
//     {3, 15, 1, 15},
//     {3, 15, 1, 20},
//     {3, 15, 1, 25},
//     {3, 15, 2, 0},
//     {3, 15, 2, 5},
//     {3, 15, 2, 10},
//     {3, 15, 2, 15},
//     {3, 15, 2, 20},
//     {3, 15, 2, 25},
//     {3, 15, 3, 0},
//     {3, 15, 3, 5},
//     {3, 15, 3, 10},
//     {3, 15, 3, 15},
//     {3, 15, 3, 20},
//     {3, 15, 3, 25},
//     {3, 15, 4, 0},
//     {3, 15, 4, 5},
//     {3, 15, 4, 10},
//     {3, 15, 4, 15},
//     {3, 15, 4, 20},
//     {3, 15, 4, 25},
//     {3, 15, 5, 0},
//     {3, 15, 5, 5},
//     {3, 15, 5, 10},
//     {3, 15, 5, 15},
//     {3, 15, 5, 20},
//     {3, 15, 5, 25},
//     {3, 20, 0, 0},
//     {3, 20, 0, 5},
//     {3, 20, 0, 10},
//     {3, 20, 0, 15},
//     {3, 20, 0, 20},
//     {3, 20, 0, 25},
//     {3, 20, 1, 0},
//     {3, 20, 1, 5},
//     {3, 20, 1, 10},
//     {3, 20, 1, 15},
//     {3, 20, 1, 20},
//     {3, 20, 1, 25},
//     {3, 20, 2, 0},
//     {3, 20, 2, 5},
//     {3, 20, 2, 10},
//     {3, 20, 2, 15},
//     {3, 20, 2, 20},
//     {3, 20, 2, 25},
//     {3, 20, 3, 0},
//     {3, 20, 3, 5},
//     {3, 20, 3, 10},
//     {3, 20, 3, 15},
//     {3, 20, 3, 20},
//     {3, 20, 3, 25},
//     {3, 20, 4, 0},
//     {3, 20, 4, 5},
//     {3, 20, 4, 10},
//     {3, 20, 4, 15},
//     {3, 20, 4, 20},
//     {3, 20, 4, 25},
//     {3, 20, 5, 0},
//     {3, 20, 5, 5},
//     {3, 20, 5, 10},
//     {3, 20, 5, 15},
//     {3, 20, 5, 20},
//     {3, 20, 5, 25},
//     {3, 25, 0, 0},
//     {3, 25, 0, 5},
//     {3, 25, 0, 10},
//     {3, 25, 0, 15},
//     {3, 25, 0, 20},
//     {3, 25, 0, 25},
//     {3, 25, 1, 0},
//     {3, 25, 1, 5},
//     {3, 25, 1, 10},
//     {3, 25, 1, 15},
//     {3, 25, 1, 20},
//     {3, 25, 1, 25},
//     {3, 25, 2, 0},
//     {3, 25, 2, 5},
//     {3, 25, 2, 10},
//     {3, 25, 2, 15},
//     {3, 25, 2, 20},
//     {3, 25, 2, 25},
//     {3, 25, 3, 0},
//     {3, 25, 3, 5},
//     {3, 25, 3, 10},
//     {3, 25, 3, 15},
//     {3, 25, 3, 20},
//     {3, 25, 3, 25},
//     {3, 25, 4, 0},
//     {3, 25, 4, 5},
//     {3, 25, 4, 10},
//     {3, 25, 4, 15},
//     {3, 25, 4, 20},
//     {3, 25, 4, 25},
//     {3, 25, 5, 0},
//     {3, 25, 5, 5},
//     {3, 25, 5, 10},
//     {3, 25, 5, 15},
//     {3, 25, 5, 20},
//     {3, 25, 5, 25},
//     {4, 0, 0, 0},
//     {4, 0, 0, 5},
//     {4, 0, 0, 10},
//     {4, 0, 0, 15},
//     {4, 0, 0, 20},
//     {4, 0, 0, 25},
//     {4, 0, 1, 0},
//     {4, 0, 1, 5},
//     {4, 0, 1, 10},
//     {4, 0, 1, 15},
//     {4, 0, 1, 20},
//     {4, 0, 1, 25},
//     {4, 0, 2, 0},
//     {4, 0, 2, 5},
//     {4, 0, 2, 10},
//     {4, 0, 2, 15},
//     {4, 0, 2, 20},
//     {4, 0, 2, 25},
//     {4, 0, 3, 0},
//     {4, 0, 3, 5},
//     {4, 0, 3, 10},
//     {4, 0, 3, 15},
//     {4, 0, 3, 20},
//     {4, 0, 3, 25},
//     {4, 0, 4, 0},
//     {4, 0, 4, 5},
//     {4, 0, 4, 10},
//     {4, 0, 4, 15},
//     {4, 0, 4, 20},
//     {4, 0, 4, 25},
//     {4, 0, 5, 0},
//     {4, 0, 5, 5},
//     {4, 0, 5, 10},
//     {4, 0, 5, 15},
//     {4, 0, 5, 20},
//     {4, 0, 5, 25},
//     {4, 5, 0, 0},
//     {4, 5, 0, 5},
//     {4, 5, 0, 10},
//     {4, 5, 0, 15},
//     {4, 5, 0, 20},
//     {4, 5, 0, 25},
//     {4, 5, 1, 0},
//     {4, 5, 1, 5},
//     {4, 5, 1, 10},
//     {4, 5, 1, 15},
//     {4, 5, 1, 20},
//     {4, 5, 1, 25},
//     {4, 5, 2, 0},
//     {4, 5, 2, 5},
//     {4, 5, 2, 10},
//     {4, 5, 2, 15},
//     {4, 5, 2, 20},
//     {4, 5, 2, 25},
//     {4, 5, 3, 0},
//     {4, 5, 3, 5},
//     {4, 5, 3, 10},
//     {4, 5, 3, 15},
//     {4, 5, 3, 20},
//     {4, 5, 3, 25},
//     {4, 5, 4, 0},
//     {4, 5, 4, 5},
//     {4, 5, 4, 10},
//     {4, 5, 4, 15},
//     {4, 5, 4, 20},
//     {4, 5, 4, 25},
//     {4, 5, 5, 0},
//     {4, 5, 5, 5},
//     {4, 5, 5, 10},
//     {4, 5, 5, 15},
//     {4, 5, 5, 20},
//     {4, 5, 5, 25},
//     {4, 10, 0, 0},
//     {4, 10, 0, 5},
//     {4, 10, 0, 10},
//     {4, 10, 0, 15},
//     {4, 10, 0, 20},
//     {4, 10, 0, 25},
//     {4, 10, 1, 0},
//     {4, 10, 1, 5},
//     {4, 10, 1, 10},
//     {4, 10, 1, 15},
//     {4, 10, 1, 20},
//     {4, 10, 1, 25},
//     {4, 10, 2, 0},
//     {4, 10, 2, 5},
//     {4, 10, 2, 10},
//     {4, 10, 2, 15},
//     {4, 10, 2, 20},
//     {4, 10, 2, 25},
//     {4, 10, 3, 0},
//     {4, 10, 3, 5},
//     {4, 10, 3, 10},
//     {4, 10, 3, 15},
//     {4, 10, 3, 20},
//     {4, 10, 3, 25},
//     {4, 10, 4, 0},
//     {4, 10, 4, 5},
//     {4, 10, 4, 10},
//     {4, 10, 4, 15},
//     {4, 10, 4, 20},
//     {4, 10, 4, 25},
//     {4, 10, 5, 0},
//     {4, 10, 5, 5},
//     {4, 10, 5, 10},
//     {4, 10, 5, 15},
//     {4, 10, 5, 20},
//     {4, 10, 5, 25},
//     {4, 15, 0, 0},
//     {4, 15, 0, 5},
//     {4, 15, 0, 10},
//     {4, 15, 0, 15},
//     {4, 15, 0, 20},
//     {4, 15, 0, 25},
//     {4, 15, 1, 0},
//     {4, 15, 1, 5},
//     {4, 15, 1, 10},
//     {4, 15, 1, 15},
//     {4, 15, 1, 20},
//     {4, 15, 1, 25},
//     {4, 15, 2, 0},
//     {4, 15, 2, 5},
//     {4, 15, 2, 10},
//     {4, 15, 2, 15},
//     {4, 15, 2, 20},
//     {4, 15, 2, 25},
//     {4, 15, 3, 0},
//     {4, 15, 3, 5},
//     {4, 15, 3, 10},
//     {4, 15, 3, 15},
//     {4, 15, 3, 20},
//     {4, 15, 3, 25},
//     {4, 15, 4, 0},
//     {4, 15, 4, 5},
//     {4, 15, 4, 10},
//     {4, 15, 4, 15},
//     {4, 15, 4, 20},
//     {4, 15, 4, 25},
//     {4, 15, 5, 0},
//     {4, 15, 5, 5},
//     {4, 15, 5, 10},
//     {4, 15, 5, 15},
//     {4, 15, 5, 20},
//     {4, 15, 5, 25},
//     {4, 20, 0, 0},
//     {4, 20, 0, 5},
//     {4, 20, 0, 10},
//     {4, 20, 0, 15},
//     {4, 20, 0, 20},
//     {4, 20, 0, 25},
//     {4, 20, 1, 0},
//     {4, 20, 1, 5},
//     {4, 20, 1, 10},
//     {4, 20, 1, 15},
//     {4, 20, 1, 20},
//     {4, 20, 1, 25},
//     {4, 20, 2, 0},
//     {4, 20, 2, 5},
//     {4, 20, 2, 10},
//     {4, 20, 2, 15},
//     {4, 20, 2, 20},
//     {4, 20, 2, 25},
//     {4, 20, 3, 0},
//     {4, 20, 3, 5},
//     {4, 20, 3, 10},
//     {4, 20, 3, 15},
//     {4, 20, 3, 20},
//     {4, 20, 3, 25},
//     {4, 20, 4, 0},
//     {4, 20, 4, 5},
//     {4, 20, 4, 10},
//     {4, 20, 4, 15},
//     {4, 20, 4, 20},
//     {4, 20, 4, 25},
//     {4, 20, 5, 0},
//     {4, 20, 5, 5},
//     {4, 20, 5, 10},
//     {4, 20, 5, 15},
//     {4, 20, 5, 20},
//     {4, 20, 5, 25},
//     {4, 25, 0, 0},
//     {4, 25, 0, 5},
//     {4, 25, 0, 10},
//     {4, 25, 0, 15},
//     {4, 25, 0, 20},
//     {4, 25, 0, 25},
//     {4, 25, 1, 0},
//     {4, 25, 1, 5},
//     {4, 25, 1, 10},
//     {4, 25, 1, 15},
//     {4, 25, 1, 20},
//     {4, 25, 1, 25},
//     {4, 25, 2, 0},
//     {4, 25, 2, 5},
//     {4, 25, 2, 10},
//     {4, 25, 2, 15},
//     {4, 25, 2, 20},
//     {4, 25, 2, 25},
//     {4, 25, 3, 0},
//     {4, 25, 3, 5},
//     {4, 25, 3, 10},
//     {4, 25, 3, 15},
//     {4, 25, 3, 20},
//     {4, 25, 3, 25},
//     {4, 25, 4, 0},
//     {4, 25, 4, 5},
//     {4, 25, 4, 10},
//     {4, 25, 4, 15},
//     {4, 25, 4, 20},
//     {4, 25, 4, 25},
//     {4, 25, 5, 0},
//     {4, 25, 5, 5},
//     {4, 25, 5, 10},
//     {4, 25, 5, 15},
//     {4, 25, 5, 20},
//     {4, 25, 5, 25},
//     {5, 0, 0, 0},
//     {5, 0, 0, 5},
//     {5, 0, 0, 10},
//     {5, 0, 0, 15},
//     {5, 0, 0, 20},
//     {5, 0, 0, 25},
//     {5, 0, 1, 0},
//     {5, 0, 1, 5},
//     {5, 0, 1, 10},
//     {5, 0, 1, 15},
//     {5, 0, 1, 20},
//     {5, 0, 1, 25},
//     {5, 0, 2, 0},
//     {5, 0, 2, 5},
//     {5, 0, 2, 10},
//     {5, 0, 2, 15},
//     {5, 0, 2, 20},
//     {5, 0, 2, 25},
//     {5, 0, 3, 0},
//     {5, 0, 3, 5},
//     {5, 0, 3, 10},
//     {5, 0, 3, 15},
//     {5, 0, 3, 20},
//     {5, 0, 3, 25},
//     {5, 0, 4, 0},
//     {5, 0, 4, 5},
//     {5, 0, 4, 10},
//     {5, 0, 4, 15},
//     {5, 0, 4, 20},
//     {5, 0, 4, 25},
//     {5, 0, 5, 0},
//     {5, 0, 5, 5},
//     {5, 0, 5, 10},
//     {5, 0, 5, 15},
//     {5, 0, 5, 20},
//     {5, 0, 5, 25},
//     {5, 5, 0, 0},
//     {5, 5, 0, 5},
//     {5, 5, 0, 10},
//     {5, 5, 0, 15},
//     {5, 5, 0, 20},
//     {5, 5, 0, 25},
//     {5, 5, 1, 0},
//     {5, 5, 1, 5},
//     {5, 5, 1, 10},
//     {5, 5, 1, 15},
//     {5, 5, 1, 20},
//     {5, 5, 1, 25},
//     {5, 5, 2, 0},
//     {5, 5, 2, 5},
//     {5, 5, 2, 10},
//     {5, 5, 2, 15},
//     {5, 5, 2, 20},
//     {5, 5, 2, 25},
//     {5, 5, 3, 0},
//     {5, 5, 3, 5},
//     {5, 5, 3, 10},
//     {5, 5, 3, 15},
//     {5, 5, 3, 20},
//     {5, 5, 3, 25},
//     {5, 5, 4, 0},
//     {5, 5, 4, 5},
//     {5, 5, 4, 10},
//     {5, 5, 4, 15},
//     {5, 5, 4, 20},
//     {5, 5, 4, 25},
//     {5, 5, 5, 0},
//     {5, 5, 5, 5},
//     {5, 5, 5, 10},
//     {5, 5, 5, 15},
//     {5, 5, 5, 20},
//     {5, 5, 5, 25},
//     {5, 10, 0, 0},
//     {5, 10, 0, 5},
//     {5, 10, 0, 10},
//     {5, 10, 0, 15},
//     {5, 10, 0, 20},
//     {5, 10, 0, 25},
//     {5, 10, 1, 0},
//     {5, 10, 1, 5},
//     {5, 10, 1, 10},
//     {5, 10, 1, 15},
//     {5, 10, 1, 20},
//     {5, 10, 1, 25},
//     {5, 10, 2, 0},
//     {5, 10, 2, 5},
//     {5, 10, 2, 10},
//     {5, 10, 2, 15},
//     {5, 10, 2, 20},
//     {5, 10, 2, 25},
//     {5, 10, 3, 0},
//     {5, 10, 3, 5},
//     {5, 10, 3, 10},
//     {5, 10, 3, 15},
//     {5, 10, 3, 20},
//     {5, 10, 3, 25},
//     {5, 10, 4, 0},
//     {5, 10, 4, 5},
//     {5, 10, 4, 10},
//     {5, 10, 4, 15},
//     {5, 10, 4, 20},
//     {5, 10, 4, 25},
//     {5, 10, 5, 0},
//     {5, 10, 5, 5},
//     {5, 10, 5, 10},
//     {5, 10, 5, 15},
//     {5, 10, 5, 20},
//     {5, 10, 5, 25},
//     {5, 15, 0, 0},
//     {5, 15, 0, 5},
//     {5, 15, 0, 10},
//     {5, 15, 0, 15},
//     {5, 15, 0, 20},
//     {5, 15, 0, 25},
//     {5, 15, 1, 0},
//     {5, 15, 1, 5},
//     {5, 15, 1, 10},
//     {5, 15, 1, 15},
//     {5, 15, 1, 20},
//     {5, 15, 1, 25},
//     {5, 15, 2, 0},
//     {5, 15, 2, 5},
//     {5, 15, 2, 10},
//     {5, 15, 2, 15},
//     {5, 15, 2, 20},
//     {5, 15, 2, 25},
//     {5, 15, 3, 0},
//     {5, 15, 3, 5},
//     {5, 15, 3, 10},
//     {5, 15, 3, 15},
//     {5, 15, 3, 20},
//     {5, 15, 3, 25},
//     {5, 15, 4, 0},
//     {5, 15, 4, 5},
//     {5, 15, 4, 10},
//     {5, 15, 4, 15},
//     {5, 15, 4, 20},
//     {5, 15, 4, 25},
//     {5, 15, 5, 0},
//     {5, 15, 5, 5},
//     {5, 15, 5, 10},
//     {5, 15, 5, 15},
//     {5, 15, 5, 20},
//     {5, 15, 5, 25},
//     {5, 20, 0, 0},
//     {5, 20, 0, 5},
//     {5, 20, 0, 10},
//     {5, 20, 0, 15},
//     {5, 20, 0, 20},
//     {5, 20, 0, 25},
//     {5, 20, 1, 0},
//     {5, 20, 1, 5},
//     {5, 20, 1, 10},
//     {5, 20, 1, 15},
//     {5, 20, 1, 20},
//     {5, 20, 1, 25},
//     {5, 20, 2, 0},
//     {5, 20, 2, 5},
//     {5, 20, 2, 10},
//     {5, 20, 2, 15},
//     {5, 20, 2, 20},
//     {5, 20, 2, 25},
//     {5, 20, 3, 0},
//     {5, 20, 3, 5},
//     {5, 20, 3, 10},
//     {5, 20, 3, 15},
//     {5, 20, 3, 20},
//     {5, 20, 3, 25},
//     {5, 20, 4, 0},
//     {5, 20, 4, 5},
//     {5, 20, 4, 10},
//     {5, 20, 4, 15},
//     {5, 20, 4, 20},
//     {5, 20, 4, 25},
//     {5, 20, 5, 0},
//     {5, 20, 5, 5},
//     {5, 20, 5, 10},
//     {5, 20, 5, 15},
//     {5, 20, 5, 20},
//     {5, 20, 5, 25},
//     {5, 25, 0, 0},
//     {5, 25, 0, 5},
//     {5, 25, 0, 10},
//     {5, 25, 0, 15},
//     {5, 25, 0, 20},
//     {5, 25, 0, 25},
//     {5, 25, 1, 0},
//     {5, 25, 1, 5},
//     {5, 25, 1, 10},
//     {5, 25, 1, 15},
//     {5, 25, 1, 20},
//     {5, 25, 1, 25},
//     {5, 25, 2, 0},
//     {5, 25, 2, 5},
//     {5, 25, 2, 10},
//     {5, 25, 2, 15},
//     {5, 25, 2, 20},
//     {5, 25, 2, 25},
//     {5, 25, 3, 0},
//     {5, 25, 3, 5},
//     {5, 25, 3, 10},
//     {5, 25, 3, 15},
//     {5, 25, 3, 20},
//     {5, 25, 3, 25},
//     {5, 25, 4, 0},
//     {5, 25, 4, 5},
//     {5, 25, 4, 10},
//     {5, 25, 4, 15},
//     {5, 25, 4, 20},
//     {5, 25, 4, 25},
//     {5, 25, 5, 0},
//     {5, 25, 5, 5},
//     {5, 25, 5, 10},
//     {5, 25, 5, 15},
//     {5, 25, 5, 20},
//     {5, 25, 5, 25},
// };
