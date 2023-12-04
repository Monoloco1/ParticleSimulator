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


#include <math.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "GUI.h"

void GUI::createWindow() {
	SDL_Init(SDL_INIT_EVERYTHING);
	mainWindow = SDL_CreateWindow("SDL2 with OpenGL Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
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

void GUI::run() {
	bool running{ true };
	while (running)
	{
		while (SDL_PollEvent(&evt))
		{
			switch (evt.type)
			{
			case SDL_QUIT:
				running = false;
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

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLoadIdentity();
		glTranslatef(400.0f, 300.0f, 0.0f);
		glColor3f(1.0f, 1.0f, 1.0f);

		glBegin(GL_POLYGON);
		glVertex2f(-100.0f, 100.0f);
		glVertex2f(100.0f, 100.0f);
		glVertex2f(100.0f, -100.0f);
		glVertex2f(-100.0f, -100.0f);
		glEnd();

		SDL_GL_SwapWindow(mainWindow);
		SDL_Color red = {255, 0, 0, 255};
		//SDL_Delay(10);
	}
}