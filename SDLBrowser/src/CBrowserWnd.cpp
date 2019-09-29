// Impl

#include <windows.h>
#include "CBrowserWnd.h"
#include "SDLContainer.h"

// std
#include <iostream>
#include <fstream>

// SDL
#include <SDL.h>
#include <SDL_image.h>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTexture = NULL;

CBrowserWnd::CBrowserWnd()
{
	// Create window
	if (!init()) {
		printf("Failed to initialize!\n");
	}
	else {
		// Load media
		if (!loadMedia()) {
			printf("Failed to load media!\n");
		}
	}

    m_container = std::make_shared<SDLContainer>(&m_browser_context, gRenderer);
}


CBrowserWnd::~CBrowserWnd()
{
	//Free loaded image
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
}


SDL_Window* CBrowserWnd::getWindow()
{
	return gWindow;
}


SDL_Renderer* CBrowserWnd::getRenderer()
{
	return gRenderer;
}


bool CBrowserWnd::runEventLoop()
{
    {
        std::ifstream cssFile("master.css");
        std::stringstream buffer;
        buffer << cssFile.rdbuf();
        auto strContents = buffer.str();
        auto len = strContents.length() + 1;
        auto wstrContents = new WCHAR[len];
        MultiByteToWideChar(CP_ACP, 0, strContents.c_str(), -1, (LPWSTR)wstrContents, len);
        m_browser_context.load_master_stylesheet(wstrContents);
        delete wstrContents;
    }

    {
        std::ifstream htmlFile("test.html");
        std::stringstream buffer;
        buffer << htmlFile.rdbuf();
        auto strContents = buffer.str();
        auto len = strContents.length() + 1;
        auto wstrContents = new WCHAR[len];
        MultiByteToWideChar(CP_ACP, 0, strContents.c_str(), -1, (LPWSTR)wstrContents, len);

        // LPCWSTR txt = L"<html><head><style>table, th, td {    border: 1px solid black;}</style></head><body><table><tr><th><a href=\"www.google.com\">Firstname</a></th><th>Lastname</th></tr></table></body></html>";
        //auto html_text = new WCHAR[lstrlen(txt) + 1];
        //lstrcpy(html_text, txt);  

        m_doc = litehtml::document::createFromString(wstrContents, m_container.get(), &m_browser_context);
        m_doc->render(SCREEN_WIDTH);
        delete wstrContents;
    }

    const litehtml::position pos(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

	//While application is running
	while (!quit) {
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0) {
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		//Clear screen
		SDL_RenderClear(gRenderer);

        m_doc->draw(0, 0, 0, &pos);

		//Update screen
		SDL_RenderPresent(gRenderer);
	}



	return true;
}


bool CBrowserWnd::init()
{
	// Initialization flag
	bool success = true;

	// Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		printf("Warning: Linear texture filtering not enabled!");
	}

	// Create window
	gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL) {
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		// Create renderer for window
		gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
		if (gRenderer == NULL)
		{
			printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Initialize renderer color
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

			//Initialize PNG loading
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags))
			{
				printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
				success = false;
			}
		}
	}	

	return success;
}

bool CBrowserWnd::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load PNG texture
	gTexture = loadTexture("texture.png");
	if (gTexture == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}

	return success;
}


SDL_Texture* CBrowserWnd::loadTexture(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		std::cout << "Unable to load image %s! SDL_image Error: " << path.c_str() << ", " << IMG_GetError();
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}