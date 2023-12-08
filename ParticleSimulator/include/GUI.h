/*
-------------------------------------
|	Software written by Cristian Niwelt (C)
|
|	The software is supplied as is and for educational purposes
|	without any guarantees nor responsibility of its use in any application.
-------------------------------------
*/


#pragma once		// include this header in the translation unit only once



#include "imgui.h"
#include "../imgui/backends/imgui_impl_sdl2.h"
#include "../imgui/backends/imgui_impl_opengl2.h"
//#include "imgui_demo.cpp"

#include <iostream>
#include <string>
#include <cassert>

#include <SDL.h>
#include <SDL_opengl.h>



#include "../../ParticleSimulator_PhysicsLib/include/Physics.h"


class GUI {
protected:
	SDL_Window* mainWindow{};
	SDL_Renderer* mainRenderer{};
	SDL_Event evt{};
	ImGuiIO io{};

	class camera {
	private:
		//SDL_Window* window{};
		DP pos;		//camera center position in the world
		D zoom{1};	//zoom/magnification the camera has on the world
		DP windowSize{};

	public:
		void init(SDL_Window* window);
		DP world2Window(const DP& dp);
		DP window2World(const DP& dp);

		DP getPos();
		void setPos(const DP& newPos);
		D getZoom();
		void setZoom(const D& newZoom);
		DP getWindowSize();
		void setWindowSize(const DP& newWindowSize);

		D multiplyZoom(const D& multiplier);

		void moveProportional(const DP & XY);
		void changePerspective(const D& multiplier, const DP& axis);

	} camera;

	Physics physicsEngine;

public:
	void createWindow();
	void displayParticle(Particle& p);
	void displayParticleVector(PV& pv);	//const ?
	void displayImGUI();
	void run();
	void removeWindow();
};


