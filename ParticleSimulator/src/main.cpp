// ==========================================================================
//
// Software written by Boguslaw Cyganek (C) to be used with the book:
// INTRODUCTION TO PROGRAMMING WITH C++ FOR ENGINEERS
// Published by Wiley, 2020
//
// The software is supplied as is and for educational purposes
// without any guarantees nor responsibility of its use in any application. 
//
// ==========================================================================


//#include <iomanip>
//#include "../../SDL2/include/SDL.h"
//#include "../SDL2/include/SDL.h"
#include "GUI.h"

int main(int argc, char** argv) {
    GUI mainGUI{};
    mainGUI.createWindow();
    mainGUI.run();
    mainGUI.removeWindow();
    return 0;
}