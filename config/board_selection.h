#ifndef BOARD_SELECTION_H
#define BOARD_SELECTION_H

//--------------------------------------------------------------------+
// Board selection
// Select your board in CMakeLists.txt
//--------------------------------------------------------------------+

#define OPT_BOARD_MAKERDIARY_M2_DOCK    1

#if CFG_BOARD == OPT_BOARD_MAKERDIARY_M2_DOCK
    #include "boards/makerdiary/m2_dock.h"
#else
    #error "No board selected, please select your board in CMakeLists.txt"
#endif


#endif