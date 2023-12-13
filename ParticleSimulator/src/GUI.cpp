/*
-------------------------------------
|	Software written by Cristian Niwelt (C)
|
|	The software is supplied as is and for educational purposes
|	without any guarantees nor responsibility of its use in any application.
-------------------------------------
*/

#define DEFAULT_WIDTH  640.0
#define DEFAULT_HEIGHT 480.0

#include <math.h>
#include "GUI.h"
#include <string>

void GUI::Camera::init(SDL_Window* window) {
	
	//SDL_GetWindowBordersSize(window, &top, &left, &bottom, &right);
	//windowCenter = { windowSize.x/2.0, windowSize.y/2.0 };		//check if conversion ok
}

DP GUI::Camera::world2Window(const DP& dp) const {
	return ( dp - pos ) * zoom;
	//return dp * zoom - pos;
}
DP GUI::Camera::window2World(const DP& dp) const {
	return ( dp / zoom + pos ) ;
	//return (dp + pos) / zoom;
}

DP GUI::Camera::getPos() {
	return pos;
}
void GUI::Camera::setPos(const DP& newPos) {
	pos = newPos;
}
D GUI::Camera::getZoom() {
	return zoom;
}
void GUI::Camera::setZoom(const D& newZoom) {
	zoom = newZoom;
}
DP GUI::Camera::getWindowSize() {
	return windowSize;
}
void GUI::Camera::setWindowSize(const DP& newWindowSize) {
	windowSize = newWindowSize;
}

D GUI::Camera::multiplyZoom(const D& multiplier) {
	zoom *= multiplier;
	return zoom;
}
void GUI::Camera::moveProportional(const DP& XY) {
	pos = pos + XY / zoom * std::min(windowSize.x, windowSize.y);
}
void GUI::Camera::changePerspective(const D& multiplier, const DP& dp) {
	DP scaleFactor = { windowSize.x * (dp.x/windowSize.x*2.0), windowSize.y * (dp.y / windowSize.y * 2.0) };
	pos = pos + scaleFactor / 20.0 / zoom / multiplier * (multiplier >= 1.0 ? 1.0 : -1.0);
	zoom *= multiplier;
}

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
void GUI::removeWindow() {
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_DestroyRenderer(mainRenderer);
	SDL_DestroyWindow(mainWindow);
	SDL_Quit();

}
void GUI::displayParticle(const Particle& p) {
	glColor4ub((const GLubyte)p.getColor().str.r,
		(const GLubyte)p.getColor().str.g,
		(const GLubyte)p.getColor().str.b,
		(const GLubyte)p.getColor().str.a);

	glBegin(GL_POLYGON);							//draw Particle vertexes
	for (auto& v : p.getShape()) {
		glVertex2d(
			cameraSimulator.world2Window(v + p.getPos()).x,
			cameraSimulator.world2Window(v + p.getPos()).y
		);
	}
	glEnd();
}
void GUI::displayParticle(const Particle& p, const Camera& camera) {
	glColor4ub((const GLubyte)p.getColor().str.r,
		(const GLubyte)p.getColor().str.g,
		(const GLubyte)p.getColor().str.b,
		(const GLubyte)p.getColor().str.a);

	glBegin(GL_POLYGON);							//draw Particle vertexes
	for (auto& v : p.getShape()) {
		glVertex2d(
			camera.world2Window(v + p.getPos()).x,
			camera.world2Window(v + p.getPos()).y
		);
	}
	glEnd();
}
void GUI::displayParticleVector(const PV& pv) {
	glLoadIdentity();
	for (auto& p : pv) {
		displayParticle(p);
	}
	
}

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
			float gravityCopiedX = physicsEngine.getGravity().x;
			float gravityCopiedY = physicsEngine.getGravity().y;
			bool updateGravity{ false };
			if (SliderFloat("Grawitacja Y", &gravityCopiedY, -10.0f, 10.0f)) updateGravity = true;
			if (SliderFloat("Grawitacja X", &gravityCopiedX, -10.0f, 10.0f)) updateGravity = true;
			if (updateGravity) physicsEngine.setGravity({ static_cast<D>(gravityCopiedX), static_cast<D>(gravityCopiedY)});

					

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

void GUI::runSimulator() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	physicsEngine.runPhysicsIteration();
	if (mouse.scrolled) {
		cameraSimulator.changePerspective(mouse.scrollY > 0 ? 1.1 : .9, mouse.pos);
	}
	if (mouse.lClick) {
		Particle newParticle = placedParticle;
		newParticle.setPos(cameraSimulator.window2World(mouse.pos));
		physicsEngine.addParticle(
			newParticle
		);
	};
	displayParticleVector(physicsEngine.getParticles());
}

void GUI::runEditor() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (mouse.scrolled) {
		cameraEditor.changePerspective(mouse.scrollY > 0 ? 1.1 : .9, mouse.pos);
	}
	if (mouse.lClick) {
		//placedParticle = Particle(cameraSimulator.window2World(mouse.pos));
		//physicsEngine.addParticle(
		//	placedParticle
		//);
	};

	//cameraEditor.setZoom(1.0);
	//cameraEditor.setPos(placedParticle.getPos() - cameraEditor.getWindowSize()/2.0);
	

	displayParticle(placedParticle, cameraEditor);
}

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
					cameraSimulator.setWindowSize({(D)evt.window.data1, (D)evt.window.data2 });

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
						break;
					case SDL_BUTTON_RIGHT:
						mouse.rClick = true;
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
		mouse.scrolled = false;
	}
}
