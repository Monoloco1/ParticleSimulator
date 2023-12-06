/*
-------------------------------------
|	Software written by Cristian Niwelt (C)
|
|	The software is supplied as is and for educational purposes
|	without any guarantees nor responsibility of its use in any application.
-------------------------------------
*/


#include <math.h>
//#include <SDL.h>
//#include <SDL_opengl.h>
#include "GUI.h"

void GUI::camera::init(SDL_Window* window) {
	
	//SDL_GetWindowBordersSize(window, &top, &left, &bottom, &right);
	//windowCenter = { windowSize.x/2.0, windowSize.y/2.0 };		//check if conversion ok
}

DP GUI::camera::world2Window(const DP& dp) {
	return ( dp - pos ) * zoom;
	//return dp * zoom - pos;
}

DP GUI::camera::window2World(const DP& dp) {
	return ( dp / zoom + pos ) ;
	//return (dp + pos) / zoom;
}

DP GUI::camera::getPos() {
	return pos;
}
void GUI::camera::setPos(const DP& newPos) {
	pos = newPos;
}
D GUI::camera::getZoom() {
	return zoom;
}
void GUI::camera::setZoom(const D& newZoom) {
	zoom = newZoom;
}
DP GUI::camera::getWindowSize() {
	return windowSize;
}
void GUI::camera::setWindowSize(const DP& newWindowSize) {
	windowSize = newWindowSize;
}

D GUI::camera::multiplyZoom(const D& multiplier) {
	zoom *= multiplier;
	return zoom;
}

void GUI::camera::moveProportional(const DP& XY) {
	pos = pos + XY / zoom * std::min(windowSize.x, windowSize.y);
}

void GUI::camera::changePerspective(const D& multiplier, const DP& dp) {
	DP scaleFactor = { windowSize.x * (dp.x/windowSize.x*2.0), windowSize.y * (dp.y / windowSize.y * 2.0) };
	pos = pos + scaleFactor / 20.0 / zoom / multiplier * (multiplier >= 1.0 ? 1.0 : -1.0);
	zoom *= multiplier;
}

void GUI::createWindow() {
	SDL_Init(SDL_INIT_EVERYTHING);

	camera.setWindowSize( { 800.0, 600.0 } );

	mainWindow = SDL_CreateWindow(
		"Particle Simulator",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		(int)camera.getWindowSize().x,
		(int)camera.getWindowSize().y,
		SDL_WINDOW_OPENGL );
	mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_GLContext glContext = SDL_GL_CreateContext(mainWindow);

	SDL_GL_SetSwapInterval(1);	//VSYNC

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glViewport(0, 0, (GLsizei)camera.getWindowSize().x, (GLsizei)camera.getWindowSize().y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, camera.getWindowSize().x, camera.getWindowSize().y, 0, 0, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
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

void GUI::displayParticle(Particle& p) {
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

void GUI::displayParticleVector(PV& pv) {
	glLoadIdentity();
	for (auto& p : pv) {
		displayParticle(p);
	}
	
}

void GUI::run() {
	bool running{ true };
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	struct {
		DP pos;	//mouse position DP
		bool lClick{}, rClick{}, scrolled{};
		D scrollX{};			//wheel scroll, positive for right
		D scrollY{};			//wheel scroll, positive for up
	} mouse;

	Particle testPart;

	while(running)
	{
		while ( SDL_PollEvent(&evt) ) {
			ImGui_ImplSDL2_ProcessEvent(&evt);
			ImGuiIO& io = ImGui::GetIO();
			switch (evt.type) {
			case SDL_QUIT:
				running = false;
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
				switch (evt.key.keysym.sym)
				{
				case SDLK_UP:
					camera.moveProportional({ 0.0, -0.1 });
					break;
				case SDLK_DOWN:
					camera.moveProportional({ 0.0, 0.1 });
					break;
				case SDLK_LEFT:
					camera.moveProportional({-0.1, 0.0});
					break;
				case SDLK_RIGHT:
					camera.moveProportional({ 0.1, 0.0 });
					break;
				}
				break;
			case SDL_KEYUP:
				switch (evt.key.keysym.sym)
				{
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
		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow(); // Show demo window! :)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//physicsEngine.runPhysicsIteration();
		if (mouse.scrolled) {
			camera.changePerspective( mouse.scrollY>0? 1.1 : .9, mouse.pos );
		}
		if (mouse.lClick) {
			testPart = Particle(camera.window2World(mouse.pos));
			physicsEngine.addParticle(
				testPart
			);
		};
		displayParticleVector(physicsEngine.getParticles());

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
