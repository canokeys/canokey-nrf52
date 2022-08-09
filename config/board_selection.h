#ifndef BOARD_SELECTION_H
#define BOARD_SELECTION_H

//--------------------------------------------------------------------+
// Board selection
// Select your board in CMakeLists.txt
//--------------------------------------------------------------------+

//--------------------------------------------------------------------+
// MakerDiary M.2 DOCK
#define OPT_BOARD_MAKERDIARY_M2_DOCK    1

// nRF52840 Dongle (aka PCA10059)
#define OPT_BOARD_NRF52840_DONGLE       2
#define OPT_BOARD_PCA10059              OPT_BOARD_NRF52840_DONGLE
//--------------------------------------------------------------------+

#if CFG_BOARD == OPT_BOARD_NRF52840_DONGLE
    #define BOARD_PCA10059
    #include "boards/nordic/pca10059.h"
#elif CFG_BOARD == OPT_BOARD_MAKERDIARY_M2_DOCK
    #include "boards/makerdiary/m2_dock.h"
#else
    #error "No board selected, please select your board in CMakeLists.txt"
#endif


#endif