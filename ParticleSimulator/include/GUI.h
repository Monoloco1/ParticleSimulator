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


#pragma once		// include this header in the translation unit only once

#include <iostream>
#include <string>
#include <cassert>

#include <SDL.h>
#include <SDL_opengl.h>

//#include "Physics.h"


class GUI {
private:
	SDL_Window* mainWindow{};
	SDL_Renderer* mainRenderer{};
	SDL_Event evt{};

public:
	void createWindow( );
	void run();
	void removeWindow();
};


