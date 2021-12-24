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
void createCircle(int x, int y, int r, double a, int b);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//The surface rendered to the screen
SDL_Surface* gSurface = NULL;

int maxRadius = 90;
double originalOffset[] = { 1, .333 };
double linearOffset[] = { 0, 0 };
int radii[] = { maxRadius, maxRadius - 60 };
int colorCount[] = {0, 0};

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

void createCircle(int x, int y, double a, int b)
{
	for (int i = 0; i <= a; i++)
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

			if (floor(linearOffset[0]) >= radii[0] || floor(linearOffset[0]) <= 0)
			{
				radSwitch = !radSwitch;
			}

			//Check keyboard inputs using key states
			const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
			if (currentKeyStates[SDL_SCANCODE_W])
			{
				for (int i = 0; i < 2; i++)
				{
					linearOffset[i] += radSwitch ? -originalOffset[i] : originalOffset[i];
				}
			}
			if (currentKeyStates[SDL_SCANCODE_S])
			{
				for (int i = 0; i < 2; i++)
				{
					linearOffset[i] -= radSwitch ? -originalOffset[i] : originalOffset[i];
				}
			}

			//Clears screen to black
			SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
			SDL_RenderClear(gRenderer);

			for (int i = 0; i < 2; i++)
			{
				if (ceil(linearOffset[i]) >= radii[i])
				{
					colorCount[i]++;
				}

				if (colorCount[i] % 2 == 1)
				{
					//Changes color to red
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
					createCircle(320, 240, radii[i], radii[i] - linearOffset[i]);
				}
				else
				{
					//Changes color to green
					SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
					createCircle(320, 240, radii[i], radii[i] - linearOffset[i]);
				}
			}
			SDL_RenderPresent(gRenderer);

			printf("%f, %f\n", linearOffset[0], linearOffset[1]);

			SDL_Delay(10);
		}
	}

	return 0;
}