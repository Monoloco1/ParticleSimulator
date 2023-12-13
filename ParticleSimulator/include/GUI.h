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
	struct {
		DP pos;					//mouse position DP
		bool lClick{}, rClick{}, scrolled{};
		D scrollX{};			//wheel scroll, positive for right
		D scrollY{};			//wheel scroll, positive for up
	} mouse;

	ImGuiIO io{};

	bool showEditor{};
	float placedParticleSizeMultiplier{ 1.0 };
	bool placedParticleSizeMultiplierApply{ false };
	Particle placedParticle = Particle({0.0, 0.0});	//this Particle will be placed next, can be changed in the editor
	//float placedParticleScale{};

	class Camera {
	private:
		//SDL_Window* window{};
		DP pos;		//camera center position in the world
		D zoom{1};	//zoom/magnification the camera has on the world
		DP windowSize{};

	public:
		void init(SDL_Window* window);
		DP world2Window(const DP& dp) const;
		DP window2World(const DP& dp) const;

		DP getPos();
		void setPos(const DP& newPos);
		D getZoom();
		void setZoom(const D& newZoom);
		DP getWindowSize();
		void setWindowSize(const DP& newWindowSize);

		D multiplyZoom(const D& multiplier);

		void moveProportional(const DP & XY);
		void changePerspective(const D& multiplier, const DP& axis);

	} cameraSimulator, cameraEditor;

	Physics physicsEngine;

public:
	void createWindow();
	void displayParticle(const Particle& p);
	void displayParticle(const Particle& p, const Camera& camera);
	void displayParticleVector(const PV& pv);	//const ?
	void displayImGUI();
	void runSimulator();
	void runEditor();
	void run();
	void removeWindow();
};


