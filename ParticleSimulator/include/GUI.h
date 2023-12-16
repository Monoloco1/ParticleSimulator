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
#include <array>

#include <SDL.h>
#include <SDL_opengl.h>

#include "../../ParticleSimulator_PhysicsLib/include/Physics.h"
using std::string;

#define PART_DISP_WMESH 1
#define PART_DISP_FILLED 0
#define PART_DISP_WMESH_BB 3
#define PART_DISP_FILLED_BB 2

class GUI {
protected:
	SDL_Window* mainWindow{};
	SDL_Renderer* mainRenderer{};
	SDL_Event evt{};
	struct {
		DP pos;					//mouse position DP
		// Click - change to pressed; Unclick - change to unpressed; Pressed - is holded down
		bool lClick{}, rClick{}, lUnclick{}, rUnclick{}, lPressed{}, rPressed{};
		bool scrolled{};
		D scrollX{};			//wheel scroll, positive for right
		D scrollY{};			//wheel scroll, positive for up
	} mouse;

	ImGuiIO io{};

	bool showEditor{};
	float placedParticleSizeMultiplier{ 1.0 };
	bool placedParticleSizeMultiplierApply{ false };
	//this Particle will be placed next, can be edited in the editor
	Particle placedParticle = Particle({0.0, 0.0});

	//TODO: maybe change the way the holded particle is stored
	//	Index of particle which is holded by the user, -1 for none
	int holdedParticleIndex = -1;

	
	std::array<const string, 4> displayModeNames {
		"Wypelnione",
		"Kontur",
		"Wypelnione + bounding box",
		"Kontur + bounding box"
	};
	//	Display mode(filled, mesh, show BB)
	int displayMode = PART_DISP_FILLED;

	class Camera {
	private:
		//SDL_Window* window{};
		DP pos;		//camera center position in the world
		D zoom{1};	//zoom/magnification the camera has on the world
		DP windowSize{};

	public:
		DP world2Window(const DP& dp) const;
		DP window2World(const DP& dp) const;

		DP getPos() const;
		void setPos(const DP& newPos);
		D getZoom() const;
		void setZoom(const D& newZoom);
		DP getWindowSize() const;
		void setWindowSize(const DP& newWindowSize);

		D multiplyZoom(const D& multiplier) const;

		void moveProportional(const DP & XY);
		void changePerspective(const D& multiplier, const DP& axis);

	} cameraSimulator, cameraEditor;

	Physics physicsEngine;
	void findClosestVertexesTo(const DPV& shape, const DP& pos, int& index1, int& index2);
	void findClosestVertexesTo(const DPV& shape, const DP& pos, int& index);
	void recalculateBB(Particle& particle);

public:
	void createWindow();
	void displayParticle(const Particle& p, const Camera& camera, const int displayMode);
	void displayParticleVector(const PV& pv, const Camera& camera, const int displayMode);
	void displayImGUI();
	int displayModeGet() const;
	void displayModeSet(const int& newDisplayMode);
	void runSimulator();
	void runEditor();
	void run();
	void removeWindow();
};


