#include <stdio.h>
#include <SDL.h>
#include <cstdlib>
#include <cmath>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Frees media and closes SDL
void close();

//Print circle
void createCircle(int x, int y, int r);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//The surface rendered to the screen
SDL_Surface* gSurface = NULL;

int radius = 90;

double a = radius;
int b = radius;

bool radSwitch = false;

bool init()
{
	bool success = true;

	//Init. SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		success = false;
		printf("Failed to initialize the video! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		//Create window 
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			success = false;
			printf("The window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			gSurface = SDL_GetWindowSurface(gWindow);
			//Create renderer
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				success = false;
				printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
			}
		}
	}

	return success;
}

void close()
{
	SDL_FreeSurface(gSurface);
	gSurface = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_DestroyRenderer(gRenderer);

	SDL_Quit();
}

void createCircle(int x, int y, int r)
{
	for (int i = 0; i <= r; i++)
	{
		double actualY = b * sqrt((1 - pow(i / a, 2)));
		int j = round(actualY);

		SDL_RenderDrawPoint(gRenderer, x + i, y + j);
		SDL_RenderDrawPoint(gRenderer, x - i, y + j);
		SDL_RenderDrawPoint(gRenderer, x + i, y - j);
		SDL_RenderDrawPoint(gRenderer, x - i, y - j);	
	}
}

int main(int argc, char* args[])
{
	if (!init())
	{
		printf("Failed to initialize!\n");
		return 1;
	}
	else
	{
		//Main loop flag
		bool quit = false;

		//Event handler
		SDL_Event e;

		while (!quit)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				//Checks if user quits
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
			}

			//Check keyboard inputs using key states
			const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
			if (currentKeyStates[SDL_SCANCODE_W])
			{
				if (b == radius || b==0)
				{
					radSwitch = !radSwitch;
				}
				
				b += radSwitch ? -1 : 1;
			}
			else if (currentKeyStates[SDL_SCANCODE_S])
			{
				if (b == radius || b == 0)
				{
					radSwitch = !radSwitch;
				}

				b -= radSwitch ? -1 : 1;
			}

			//Clears screen to black
			SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
			SDL_RenderClear(gRenderer);

			//Changes color to white
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			createCircle(299, 299, radius);
			createCircle(299, 299, radius - 60);
			SDL_RenderPresent(gRenderer);

			if (b > 89)
			{
				SDL_Delay(10 + b/1.4);
			}
			else
			{
				SDL_Delay(10);
			}
		}
	}

	return 0;
}