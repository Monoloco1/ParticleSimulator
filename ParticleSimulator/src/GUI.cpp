﻿/*
-------------------------------------
|	Software written by Cristian Niwelt (C)
|
|	The software is supplied as is and for educational purposes
|	without any guarantees nor responsibility of its use in any application.
-------------------------------------
*/


#include <math.h>
#include "GUI.h"

using std::min, std::max;

/*	world2Window(); window2World()
|-----------------------------------
|	These funcs change a position struct from/to window or world coordinates accordingly
|-----------------------------------
|	INPUT: data pair(DP)
|-----------------------------------
|	OUTPUT: transformed DP
*/
DP GUI::Camera::world2Window(const DP& dp) const {
	return ( dp - pos ) * zoom;
}
DP GUI::Camera::window2World(const DP& dp) const {
	return ( dp / zoom + pos ) ;
}

//	GUI::Camera setters/getters
DP GUI::Camera::getPos() const {
	return pos;
}
void GUI::Camera::setPos(const DP& newPos) {
	pos = newPos;
}
D GUI::Camera::getZoom() const {
	return zoom;
}
void GUI::Camera::setZoom(const D& newZoom) {
	zoom = newZoom;
}
DP GUI::Camera::getWindowSize() const {
	return windowSize;
}
void GUI::Camera::setWindowSize(const DP& newWindowSize) {
	windowSize = newWindowSize;
}

/*	moveProportional
|-----------------------------------
|	moves the camera struct by a fraction of the screen
|-----------------------------------
|	INPUT: DP of X & Y fractions by which to move camera
|-----------------------------------
|	OUTPUT: void
*/
void GUI::Camera::moveProportional(const DP& XY) {
	pos = pos + XY / zoom * std::min(windowSize.x, windowSize.y);
}

/*	changePerspective
|-----------------------------------
|	zooms & moves the camera struct so the pos indicated by mouse cursor
|	stays in the same place on the screen
|-----------------------------------
|	INPUT: D of zoom amount; DP of the pos indicated before
|-----------------------------------
|	OUTPUT: void
*/
void GUI::Camera::changePerspective(const D& multiplier, const DP& dp) {
	DP scaleFactor = { windowSize.x * (dp.x/windowSize.x*2.0), windowSize.y * (dp.y / windowSize.y * 2.0) };
	pos = pos + scaleFactor / 20.0 / zoom / multiplier * (multiplier >= 1.0 ? 1.0 : -1.0);
	zoom *= multiplier;
}

/*	createWindow
|-----------------------------------
|	Initializes SDL, OpenGL & ImGui
|	Creates window
|-----------------------------------
|	INPUT: void
|-----------------------------------
|	OUTPUT: void
*/
void GUI::createWindow() {
	SDL_Init(SDL_INIT_EVERYTHING);

	cameraSimulator.setWindowSize( { DEFAULT_WIDTH, DEFAULT_HEIGHT } );
	cameraEditor.setWindowSize({ DEFAULT_WIDTH, DEFAULT_HEIGHT });

	mainWindow = SDL_CreateWindow(
		"Particle Simulator",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		(int)cameraSimulator.getWindowSize().x,
		(int)cameraSimulator.getWindowSize().y,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_GLContext glContext = SDL_GL_CreateContext(mainWindow);

	SDL_GL_SetSwapInterval(1);	//VSYNC

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glViewport(0, 0, (GLsizei)cameraSimulator.getWindowSize().x, (GLsizei)cameraSimulator.getWindowSize().y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, cameraSimulator.getWindowSize().x, cameraSimulator.getWindowSize().y, 0, 0, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(mainWindow, glContext);
	ImGui_ImplOpenGL2_Init();

	//camera.init(mainWindow);
}

/*	removeWindow
|-----------------------------------
|	Removes SDL & ImGui windows and associated data
|-----------------------------------
|	INPUT: void
|-----------------------------------
|	OUTPUT: void
*/
void GUI::removeWindow() {
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_DestroyRenderer(mainRenderer);
	SDL_DestroyWindow(mainWindow);
	SDL_Quit();

}

/*	displayParticle
|-----------------------------------
|	draws a particle on the window by the rules of camera
|-----------------------------------
|	INPUT: reference to Particle to be drawn and Camera by which to draw it
|-----------------------------------
|	OUTPUT: void
*/
void GUI::displayParticle(const Particle& p, const Camera& camera, const int displayMode) {
	glColor4ub((const GLubyte)p.getColor().str.r,
		(const GLubyte)p.getColor().str.g,
		(const GLubyte)p.getColor().str.b,
		(const GLubyte)p.getColor().str.a);
	
	//	draw Particle vertexes
	switch (displayMode) {
	case PART_DISP_WMESH:
	case PART_DISP_WMESH_BB:
		glBegin(GL_LINE_LOOP);
		break;
	case PART_DISP_FILLED:
	case PART_DISP_FILLED_BB:
	default:
		glBegin(GL_POLYGON);
		break;
	}
	for (auto& v : p.getShape()) {
		glVertex2d(
			camera.world2Window(v + p.getPos()).x,
			camera.world2Window(v + p.getPos()).y
		);
	}
	glEnd();

	//	display BB
	if (displayMode == PART_DISP_WMESH_BB || displayMode == PART_DISP_FILLED_BB) {
		glColor3ub(255, 0, 255);
		glBegin(GL_LINE_LOOP);
		glVertex2d(
			camera.world2Window(p.getPos()).x + p.getBB().e * camera.getZoom(),
			camera.world2Window(p.getPos()).y + p.getBB().n * camera.getZoom());
		glVertex2d(
			camera.world2Window(p.getPos()).x + p.getBB().w * camera.getZoom(),
			camera.world2Window(p.getPos()).y + p.getBB().n * camera.getZoom());
		glVertex2d(
			camera.world2Window(p.getPos()).x + p.getBB().w * camera.getZoom(),
			camera.world2Window(p.getPos()).y + p.getBB().s * camera.getZoom());
		glVertex2d(
			camera.world2Window(p.getPos()).x + p.getBB().e * camera.getZoom(),
			camera.world2Window(p.getPos()).y + p.getBB().s * camera.getZoom());
		glEnd();
	}
}

/*	displayParticleVector
|-----------------------------------
|	draws a DPV on screen
|-----------------------------------
|	INPUT: reference to Particle vector & camera struct
|-----------------------------------
|	OUTPUT: void
*/
void GUI::displayParticleVector(const PV& pv, const Camera& camera, const int displayMode) {
	glLoadIdentity();
	for (auto& p : pv) {
		displayParticle(p, camera, displayMode);
	}
}

/*	displayImGUI
|-----------------------------------
|	Function to create & display the GUI of the project. The same Gui for Simulator and Editor windows
|	Uses ImGui library
|-----------------------------------
|	INPUT: void
|-----------------------------------
|	OUTPUT: void
*/
void GUI::displayImGUI() {
	using ImGui::BeginMainMenuBar, ImGui::MenuItem, ImGui::BeginMenu, ImGui::SeparatorText, ImGui::EndMenu, ImGui::EndMainMenuBar, ImGui::Begin, ImGui::End;
	using ImGui::Button, ImGui::SliderFloat;

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(100, 100), ImGuiCond_FirstUseEver);
	
	if ( BeginMainMenuBar() ) {
		if (BeginMenu("Plik")) {
			if (MenuItem("Restart symulacji", NULL, false))
				physicsEngine.setParticles(PV());

			SeparatorText("##");
			if (MenuItem("Zakoncz program", NULL, false))
				exit(1);

			EndMenu();
		}
		if (BeginMenu("Edytuj")) {
			if (MenuItem("Edytor czastek", NULL, showEditor, true)) {
				showEditor = !showEditor;
				if (showEditor) {
					cameraEditor.setPos(cameraEditor.getWindowSize() / (-2.0));
					cameraEditor.setZoom(1.0);
				}
			}
			if (showEditor && MenuItem("Resetuj czastke", NULL, false, true)) placedParticle = Particle({ 0.0, 0.0 });
			if (showEditor && MenuItem("Resetuj perspektywe", NULL, false, true)) {
				cameraEditor.setPos(cameraEditor.getWindowSize()/(-2.0));
				cameraEditor.setZoom(1.0);
			}

			//TODO: make the resizing slider usable
			if (showEditor && SliderFloat("Wielkosc", &placedParticleSizeMultiplier, 0.1f, 10.0f, "%.3f", ImGuiSliderFlags_Logarithmic)) {
				placedParticleSizeMultiplierApply = true;
			}
			else if (placedParticleSizeMultiplierApply) {
				placedParticleSizeMultiplierApply = false;
				placedParticle.setSize(placedParticleSizeMultiplier);
			}

			SeparatorText("##");
			if (MenuItem("Wlacz grawitacje", NULL, physicsEngine.getGravityBool(), true)) physicsEngine.setGravityBool(!physicsEngine.getGravityBool());
			float gravityCopiedX = static_cast<float>(physicsEngine.getGravity().x);
			float gravityCopiedY = static_cast<float>(physicsEngine.getGravity().y);
			bool updateGravity{ false };
			if (SliderFloat("Grawitacja Y", &gravityCopiedY, -10.0f, 10.0f)) updateGravity = true;
			if (SliderFloat("Grawitacja X", &gravityCopiedX, -10.0f, 10.0f)) updateGravity = true;
			if (updateGravity) physicsEngine.setGravity({ static_cast<D>(gravityCopiedX), static_cast<D>(gravityCopiedY)});
			SeparatorText("##");
			//if (MenuItem("##", NULL, displayModeGet(), true));
			//bool a;
			//ImGui::Checkbox("checkbox", &a);
			//ImGui::RadioButton("radio", &a);
			//ImGui::ListBox("Tryb wyswietlania", &displayMode, &displayModeNames, 4, 2);
			if (ImGui::BeginCombo("Tryb wyswietlania", displayModeNames[displayMode].c_str())) {
				for (int i{}; static_cast<size_t>(i) < displayModeNames.size(); ++i) {
					if (ImGui::Selectable(displayModeNames.at(i).c_str(), displayMode == i))
						displayMode = i;
				}
				//for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
				//	const bool is_selected = (item_current_idx == n);
				//	if (ImGui::Selectable(items[n], is_selected))
				//		item_current_idx = n;

				//	// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				//	if (is_selected)
				//		ImGui::SetItemDefaultFocus();
				//}
				ImGui::EndCombo();
			}
			//MenuItem("Sel1 En1", NULL, true, true);
			//MenuItem("Sel1 En0", NULL, true, false);
			//MenuItem("Sel0 En1", NULL, false, true);
			//MenuItem("Sel0 En0", NULL, false, false);
			
			EndMenu();
		}
		
		Button("Pomoc") ;
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				ImGui::SetTooltip(
					"Program napisany przez Cristiana Niwelta\n"
					"Studenta AGH\n\n"
					"Aby dodać cząstkę, kliknij lewym przyciskiem myszy w wolne miejsce\n"
					"Aby zmienić kształt cząstkek, wejdź w tryb edytora cząstek: Edytuj>Edytor cząstek"

				);
			}
			//EndMenu();

		EndMainMenuBar();
	}

}

//	GUI::displayMode setter/getter
int GUI::displayModeGet() const {
	return displayMode;
}
void GUI::displayModeSet(const int& newDisplayMode) {
	assert(newDisplayMode == PART_DISP_WMESH || newDisplayMode == PART_DISP_FILLED ||
		newDisplayMode == PART_DISP_WMESH_BB || newDisplayMode == PART_DISP_FILLED_BB);
	displayMode = newDisplayMode;
}

/*	runSimulator
|-----------------------------------
|	function that shows the simulated space, particles, runs collision functions
|	processes user input
|-----------------------------------
|	INPUT: void
|-----------------------------------
|	OUTPUT: void
*/
void GUI::runSimulator() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	physicsEngine.runPhysicsIteration();
	if (mouse.scrolled) {
		cameraSimulator.changePerspective(mouse.scrollY > 0 ? 1.1 : .9, mouse.pos);
	}
	if (mouse.lClick) {
		Particle newParticle = placedParticle;
		newParticle.setPos(cameraSimulator.window2World(mouse.pos));
		physicsEngine.addParticle(newParticle);
	};
	if (mouse.rPressed) {
		//	find the hovered particle
		if (mouse.rClick) {
			//TODO:	check this
			for (int p{}; static_cast<size_t>(p) < physicsEngine.getParticles().size(); ++p) {
				if (physicsEngine.hoverDetect(p, cameraSimulator.window2World(mouse.pos))) {
					holdedParticleIndex = p;
					break;
				}
			}
		}
		if (holdedParticleIndex > -1) {
			assert(static_cast<size_t>(holdedParticleIndex) < physicsEngine.getParticles().size());
			//	Change the holded particle vel towards mouse pointer
			auto deltaVel = cameraSimulator.window2World(mouse.pos) - physicsEngine.getParticles(holdedParticleIndex).getPos();
			deltaVel *= 0.1;
			physicsEngine.setParticleVel(holdedParticleIndex, physicsEngine.getParticleVel(holdedParticleIndex)+deltaVel);
			physicsEngine.setParticleVel(holdedParticleIndex, physicsEngine.getParticleVel(holdedParticleIndex) * 0.7);
		}
	}
	if (mouse.rUnclick) holdedParticleIndex = -1;

	displayParticleVector(physicsEngine.getParticles(), cameraSimulator, displayMode);
}

/*	findClosestVertexesTo
|-----------------------------------
|	functions that find the index of the particle that is closest to pos indicated
|-----------------------------------
|	INPUT: DPV of vertexes(DP's), pos to compare, returned index/indexes of vertex/vertexes
|-----------------------------------
|	OUTPUT: void
*/
void GUI::findClosestVertexesTo(const DPV& shape, const DP& pos, int& index) {
	//	find closest vertex
	const auto shapeSize = shape.size();
	if (shapeSize > 1) {
		D dist{ PSFunc::distanceBetween(pos, shape.at(0)) };
		int dp{ 0 };
		if (shapeSize < 2) {
			index = dp;
			return;
		}
		else {
			for (int i{ 1 }; static_cast<size_t>(i) < shapeSize; ++i) {
				D distI = PSFunc::distanceBetween(pos, shape.at(i));
				if (distI < dist) {
					dist = distI;
					dp = i;
				}
			}
			index = dp;
			return;
		}
	}
}
void GUI::findClosestVertexesTo(const DPV& shape, const DP& pos, int& index1, int& index2) {
	//	find 2 closest vertexes
	if (shape.size() > 1) {
		D dist1{ PSFunc::distanceBetween(pos, shape.at(0)) };
		D dist2{ PSFunc::distanceBetween(pos, shape.at(1)) };
		int dp1{ 0 }, dp2{ 1 };
		if (shape.size() < 3) {
			index1 = dp1;
			index2 = dp2;
			return;
		} else {
			for (int i{ 2 }; static_cast<size_t>(i) < shape.size(); ++i) {
				D distI = PSFunc::distanceBetween(pos, shape.at(i));
				if (distI < dist1) {
					dist2 = dist1;
					dp2 = dp1;
					dist1 = distI;
					dp1 = i;
				}
				else if (distI < dist2) {
					dist2 = distI;
					dp2 = i;
				}
			}
			index1 = dp1;
			index2 = dp2;
			return;
		}
	}
}

/*	recalculateBB
|-----------------------------------
|	function that calculates BB so it fits the shape of the Particle
|-----------------------------------
|	INPUT: reference to particle that will be acted upon
|-----------------------------------
|	OUTPUT: void
*/
void GUI::recalculateBB(Particle& particle) {
	D e{}, w{}, n{}, s{};
	for (const auto& v : particle.getShape()) {
		e = min(e, v.x);
		w = max(w, v.x);
		n = min(n, v.y);
		s = max(s, v.y);
	}
	particle.setBB(BB({n, s, w, e}));
}

/*	runEditor
|-----------------------------------
|	function that shows the edited particle and processes inputs for editing it
|-----------------------------------
|	INPUT: void
|-----------------------------------
|	OUTPUT: void
*/
void GUI::runEditor() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//	camera zoom
	if (mouse.scrolled) {
		cameraEditor.changePerspective(mouse.scrollY > 0 ? 1.1 : .9, mouse.pos);
	}

	//	add vertex
	if (mouse.lClick) {
		//int dp1{}, dp2{};
		int dp{};
		auto shapeCopy = placedParticle.getShape();
		//findClosestVertexesTo(shapeCopy, cameraEditor.window2World(mouse.pos) - placedParticle.getPos(), dp1, dp2);
		findClosestVertexesTo(shapeCopy, cameraEditor.window2World(mouse.pos) - placedParticle.getPos(), dp);
		//	insert new vertex between two vertexes closest to mouse pos
		//maybe create insert function in shape struct?
		//shapeCopy.insert( shapeCopy.begin()+std::max(dp1, dp2), cameraEditor.window2World(mouse.pos)-placedParticle.getPos() );
		shapeCopy.insert(shapeCopy.begin() + dp, cameraEditor.window2World(mouse.pos) - placedParticle.getPos());
		placedParticle.setShape(shapeCopy);
		recalculateBB(placedParticle);
	}

	//	remove vertex
	if (mouse.rClick) {
		if (placedParticle.getShape().size() > 3) {
			auto shapeCopy = placedParticle.getShape();
			int index{};
			findClosestVertexesTo(shapeCopy, cameraEditor.window2World(mouse.pos) - placedParticle.getPos(), index);
			shapeCopy.erase(shapeCopy.begin() + index);
			placedParticle.setShape(shapeCopy);
		}
		recalculateBB(placedParticle);
	}

	displayParticle(placedParticle, cameraEditor, displayMode);
}

/*	run
|-----------------------------------
|	This is the program's main loop. Processes SDL & ImGui events(mainly user input)
|	Decides to draw simulator or to draw editor
|-----------------------------------
|	INPUT: void
|-----------------------------------
|	OUTPUT: void
*/
void GUI::run() {
	bool running{ true };
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	while(running) {
		while ( SDL_PollEvent(&evt) ) {
			ImGui_ImplSDL2_ProcessEvent(&evt);
			ImGuiIO& io = ImGui::GetIO();

			switch (evt.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_WINDOWEVENT:
				if (evt.window.event == SDL_WINDOWEVENT_RESIZED) {
					cameraSimulator.setWindowSize( {(D)evt.window.data1, (D)evt.window.data2 } );
					cameraEditor.setWindowSize({ (D)evt.window.data1, (D)evt.window.data2 } );

					glViewport(0, 0, (GLsizei)cameraSimulator.getWindowSize().x, (GLsizei)cameraSimulator.getWindowSize().y);
					glMatrixMode(GL_PROJECTION);
					glLoadIdentity();
					glOrtho(0, cameraSimulator.getWindowSize().x, cameraSimulator.getWindowSize().y, 0, 0, 1000);
					glMatrixMode(GL_MODELVIEW);
				}
				break;
			case SDL_MOUSEMOTION:
				mouse.pos.x = (D)evt.motion.x;
				mouse.pos.y = (D)evt.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
				//io.AddMouseButtonEvent(evt.button.button, true);
				if (!io.WantCaptureMouse)
					switch (evt.button.button) {
					case SDL_BUTTON_LEFT:
						mouse.lClick = true;
						mouse.lPressed = true;
						break;
					case SDL_BUTTON_RIGHT:
						mouse.rClick = true;
						mouse.rPressed = true;
						break;
					default:
					break;
					}
				break;
			case SDL_MOUSEBUTTONUP:
				if (!io.WantCaptureMouse)
					switch (evt.button.button) {
					case SDL_BUTTON_LEFT:
						mouse.lPressed = false;
						mouse.lUnclick = true;
						break;
					case SDL_BUTTON_RIGHT:
						mouse.rPressed = false;
						mouse.rUnclick = true;
						break;
					default:
						break;
					}
				break;
			case SDL_MOUSEWHEEL:
				if (!io.WantCaptureMouse)
					switch (evt.wheel.type) {
					case SDL_MOUSEWHEEL:
						mouse.scrolled = true;
						mouse.scrollX = evt.wheel.x;
						mouse.scrollY = evt.wheel.y;
						break;

					default:
						break;
					}
				break;
			case SDL_KEYDOWN:
				if (!io.WantCaptureKeyboard)
				switch (evt.key.keysym.sym) {
				case SDLK_UP:
					cameraSimulator.moveProportional({ 0.0, -0.1 });
					break;
				case SDLK_DOWN:
					cameraSimulator.moveProportional({ 0.0, 0.1 });
					break;
				case SDLK_LEFT:
					cameraSimulator.moveProportional({-0.1, 0.0});
					break;
				case SDLK_RIGHT:
					cameraSimulator.moveProportional({ 0.1, 0.0 });
					break;
				}
				break;
			case SDL_KEYUP:
				switch (evt.key.keysym.sym) {
				case SDLK_UP:
					break;
				case SDLK_DOWN:
					break;
				case SDLK_LEFT:
					break;
				case SDLK_RIGHT:
					break;
				}
				break;
			}
			
		}
		
		// Start the Dear ImGui frame
		displayImGUI();
		
		if (showEditor)
			runEditor();
		else runSimulator();

		//	draw the imGUI
		ImGui::Render();
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

		//	draw the openGL content
		SDL_GL_SwapWindow(mainWindow);

		mouse.lClick = false;
		mouse.rClick = false;
		mouse.lUnclick = false;
		mouse.rUnclick = false;
		mouse.scrolled = false;
	}
}
