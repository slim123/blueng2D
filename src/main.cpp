#include <iostream>
#include <string>
#include <sstream>
#include <math.h>

#include "CEngine.h"
#include "CGame.h"

using namespace std;

int main(int argc, char *argv[])
{
	unsigned int fpsCount = 0;
	unsigned int nextFpsClear = 0;
	stringstream fpsDisplayed;

	unsigned int nextTick = 0;

	ENG.CreateWindow(800, 600, "blueng2D v0.0.00");
	ENG.StopMusic();

	SDL_Color colour_fps =
	{ 255, 255, 255 };

	ENG.DebugOverlayLoadFont("data/font0.ttf", colour_fps);

	ENG.DebugOverlayAdd("GAME INIT BEGIN");
	GAME.Init();
	ENG.DebugOverlayAdd("entering update loop");

	while (ENG.Update())
	{
		ENG.FlipScreen();
		ENG.ClearScreen();
		fpsCount++;

		GAME.Render();

		ENG.RenderFont(ENG.DebugOverLayFont(), fpsDisplayed.str(), 20, 20, col_white);

		if(ENG.GetKeyDown(SDLK_F1))
		{
			ENG.DebugOverLayRender();
		}

		if(SDL_GetTicks() > nextTick)
		{
			nextTick += 40;

			GAME.Update();

			if(SDL_GetTicks() > nextFpsClear)
			{
				nextFpsClear += 1000;
				fpsDisplayed.str("");
				fpsDisplayed << "FPS:" << fpsCount << " M:" << ENG.mouseXPosition << "," << ENG.mouseYPosition;
				fpsCount=0;
			}
		}

		ENG.Sleep(1);
	}

	return 0;
}
