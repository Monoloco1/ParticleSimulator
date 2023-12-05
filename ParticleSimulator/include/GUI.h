/*
-------------------------------------
|	Software written by Cristian Niwelt (C)
|
|	The software is supplied as is and for educational purposes
|	without any guarantees nor responsibility of its use in any application.
-------------------------------------
*/


#pragma once		// include this header in the translation unit only once

#include <iostream>
#include <string>
#include <cassert>

#include <SDL.h>
#include <SDL_opengl.h>

#include "../../ParticleSimulator_PhysicsLib/include/Physics.h"


class GUI {
private:
	SDL_Window* mainWindow{};
	SDL_Renderer* mainRenderer{};
	SDL_Event evt{};

	Physics physicsEngine;

public:
	void createWindow();
	void displayParticleVector(PV& pv);	//const ?
	void run();
	void removeWindow();
};


