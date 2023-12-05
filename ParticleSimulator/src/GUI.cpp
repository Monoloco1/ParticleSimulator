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

void GUI::createWindow() {
	SDL_Init(SDL_INIT_EVERYTHING);
	mainWindow = SDL_CreateWindow("Particle Simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_GL_CreateContext(mainWindow);
	SDL_GL_SetSwapInterval(1);	//VSYNC

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, 800, 600, 0, 0, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GUI::removeWindow() {
	SDL_DestroyRenderer(mainRenderer);
	SDL_DestroyWindow(mainWindow);
	SDL_Quit();
}

void GUI::displayParticleVector(PV& pv) {
	
	
	for (auto& p : pv) {
		glLoadIdentity();
		glTranslated(p.getPos().x, p.getPos().y, 0.0);	//set pos on screen(later by perspective)

		glColor4ub(	(const GLubyte)p.getColor().str.r,	//set displayed particle color
					(const GLubyte)p.getColor().str.g,
					(const GLubyte)p.getColor().str.b,
					(const GLubyte)p.getColor().str.a);
		//glColor4ub(255, 0, 0, 255);

		glBegin(GL_POLYGON);							//draw Particle vertexes
		glVertex2f(-10.0f, 10.0f);						//temporarily just boxes
		glVertex2f(10.0f, 10.0f);
		glVertex2f(10.0f, -10.0f);
		glVertex2f(-10.0f, -10.0f);
		glEnd();
	}
}

void GUI::run() {
	bool running{ true };
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	struct {
		DP pos;	//mouse position DP
		bool lClick{}, rClick{};
	} mouse;
	Particle testPart;
	while(running)
	{
		while ( SDL_PollEvent(&evt) ) {
			switch (evt.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			case SDL_MOUSEMOTION:
				mouse.pos.x = (D)evt.motion.x;
				mouse.pos.y = (D)evt.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
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
			case SDL_KEYDOWN:
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
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		


		//physicsEngine.runPhysicsIteration();
		if (mouse.lClick) {
			testPart = Particle(mouse.pos, { 0.0, 0.0 }, 1, prt::Red, { 10.0, -10.0, -10.0, 10.0 });
			physicsEngine.addParticle(
				testPart
			);
		};
		displayParticleVector(physicsEngine.getParticles());

		SDL_GL_SwapWindow(mainWindow);


		mouse.lClick = false;
		mouse.rClick = false;
		//SDL_Color red = {255, 0, 0, 255};
		//SDL_Delay(10);
	}
}

//TODO: functions to change window space to world space & viceversa, maybe SDL provides?
