#include "CGame.h"
#include "CEngine.h"

#include <sstream>
#include <string.h>

CGame CGame::game;

CGame& CGame::Get()
{
	return game;
}

CGame::CGame()
{
	texSquareSheet = 0;
	memset(texSquare, 0, sizeof(texSquare));
	texCircleSheet = 0;
	memset(texCircle, 0, sizeof(texCircle));

	posXSquare = 0;
	posYSquare = 0;

	posXCircle = 0;
	posYCircle = 0;

	font0 = 0;
	font1 = 0;
}

CGame::~CGame()
{

}

void CGame::Init()
{
	font0 = ENG.LoadFont("data/font0.ttf", 12);
	font1= ENG.LoadFont("data/font1.ttf", 16);

	posXSquare = (ENG.GetScreenWidth()/2)-25;
	posYSquare = (ENG.GetScreenHeight()/2)-25;

	posXCircle = (ENG.GetScreenWidth()/2)-25;;
	posYCircle = (ENG.GetScreenHeight()/2)-25;

	texSquareSheet = ENG.LoadTexture("data/bluSquare50x50x4.png");
	texCircleSheet = ENG.LoadTexture("data/bluCircle50x50x4.png");

	for(int x=0;x<4;x++)
	{
		texSquare[x] = ENG.LoadSubTexture(texSquareSheet,x*50,0,50,50);
		texCircle[x] = ENG.LoadSubTexture(texCircleSheet,x*50,0,50,50);
	}
}

void CGame::Render()
{
	int frame = (ENG.GetTicks()/100) % 4;

	ENG.RenderSubTexture(texSquare[frame],posXSquare,posYSquare);
	ENG.RenderSubTexture(texCircle[frame],posXCircle,posYCircle);

	if(ENG.CollideCircleCircle(posXCircle+25, posYCircle+25,25,posXSquare+25,posYSquare+25,25))
	{
		ENG.RenderFont(font0,"CircleCircle Collision!", 10, 50, col_red);
	}

	if(ENG.CollideRectRect(posXCircle, posYCircle,50,50,posXSquare,posYSquare,50,50))
	{
		ENG.RenderFont(font0,"RectRect Collision!", 10, 70, col_red);
	}

	if(ENG.CollidePointRect(ENG.mouseXPosition, ENG.mouseYPosition,posXSquare,posYSquare,50,50))
	{
		ENG.RenderFont(font0,"PointRect Collision!", 10, 90, col_red);
	}

	if(ENG.CollidePointCircle(ENG.mouseXPosition, ENG.mouseYPosition,posXCircle+25, posYCircle+25,25))
	{
		ENG.RenderFont(font0,"PointCircle Collision!", 10, 110, col_red);
	}

	if(ENG.CollideCircleRect(posXCircle+25, posYCircle+25,25, posXSquare,posYSquare,50,50))
	{
		ENG.RenderFont(font0,"CircleRect Collision!", 10, 130, col_red);
	}

}

void CGame::Update()
{
	if (ENG.GetKeyDown(SDLK_UP) && posYCircle > -25)
	{
		posYCircle--;
	}
	if(ENG.GetKeyDown(SDLK_DOWN) && posYCircle < ENG.GetScreenHeight()-25)
	{
		posYCircle++;
	}

	if(ENG.GetKeyDown(SDLK_LEFT) && posXCircle > -25)
	{
		posXCircle--;
	}
	if(ENG.GetKeyDown(SDLK_RIGHT) && posXCircle < ENG.GetScreenWidth() - 25)
	{
		posXCircle++;
	}
}

