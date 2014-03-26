#include "CGameTest.h"
#include <string.h>

CGameTest::CGameTest()
{
	texSquareSheet = -1;
	texCircleSheet = -1;
	for (int i = 0; i < 4; i++)
	{
		texSquare[i] = -1;
		texCircle[i] = -1;
	}

	posXSquare = 0;
	posYSquare = 0;

	posXCircle = 0;
	posYCircle = 0;

	font0 = -1;
	font1 = -1;

	musMP3 = -1;
	musVolume = 25;
}

CGameTest::~CGameTest()
{
}

void CGameTest::Init()
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

	musMP3 = ENG.LoadMusic("data/BarrenLandscape.mp3");
	ENG.PlayMusic(musMP3);
	ENG.SetMusicVolume(musVolume);

	ENG.SetClearScreenColour(100, 100, 0);
}

void CGameTest::Render()
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

void CGameTest::Update()
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

	if(ENG.GetLastKeyPressed() == SDLK_p)
	{
		ENG.PauseMusic();
		ENG.DebugOverlayAdd("Paused music");
	}
	else if(ENG.GetLastKeyPressed() == SDLK_r)
	{
		ENG.ResumeMusic();
		ENG.DebugOverlayAdd("Resumed music");
	}
	else if(ENG.GetLastKeyPressed() == SDLK_d)
	{
		if(!ENG.DebugOverlayShown())
		{
			ENG.DebugOverlayShow(true);
			ENG.DebugOverlayAdd("Debug overlay on");
		}
		else
		{
			ENG.DebugOverlayShow(false);
			ENG.DebugOverlayAdd("Debug overlay off");
		}
	}
	else if(ENG.GetLastKeyPressed() == SDLK_t)
	{
		DEBUGLOG("test DEBUGLOG @" << ENG.GetTicks());
	}

	if(ENG.GetKeyDown(SDLK_l) && musVolume < 100)
	{
		musVolume++;
		ENG.SetMusicVolume(musVolume);
		ENG.DebugOverlayAdd("musVolume = " + musVolume);
	}
	else if(ENG.GetKeyDown(SDLK_k) && musVolume != 0)
	{
		musVolume--;
		ENG.SetMusicVolume(musVolume);
		ENG.DebugOverlayAdd("musVolume = " + musVolume);
	}
}

