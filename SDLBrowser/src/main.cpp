// Main

// std
#include <stdio.h>
#include <string>
#include <memory>

// SDL
#include <SDL.h>

// Browser
#include "CBrowserWnd.h"


int main(int argc, char* args[])
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());		
	}
	else {
		std::shared_ptr<CBrowserWnd> pBrowserWnd = std::make_shared<CBrowserWnd>();
		pBrowserWnd->runEventLoop();		
	}

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}