/*
 * CEngine.h
 *
 *  Created on: 30 Mar 2013
 *      Author: rob
 */

#ifndef CENGINE_H_
#define CENGINE_H_

#include <vector>
#include <deque>
#include <string>
#include <sstream>
#include <stdint.h>

#ifdef __linux__
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#else
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#endif

#define DEBUGLOG(x) \
	{\
	std::stringstream debugStream;\
	debugStream << x;\
	ENG.DebugOverlayAdd(debugStream.str());\
	}

const SDL_Color col_red =
{ 255, 0, 0, 255 };
const SDL_Color col_green =
{ 0, 255, 0, 255 };
const SDL_Color col_blue =
{ 0, 0, 255, 255 };
const SDL_Color col_white =
{ 255, 255, 255, 255 };
const SDL_Color col_black =
{ 0, 0, 0, 255 };

class CGame
{
public:
	CGame()
	{
	}

	virtual ~CGame()
	{
	}

	virtual void Init()=0;
	virtual void Render()=0;
	virtual void Update()=0;
};

class CSubTexture
{
public:
	unsigned int m_texture;
	SDL_Rect m_src;
};

class CEngine
{
	bool m_windowCreated;
	SDL_Surface* m_screen;
	std::vector<TTF_Font*> m_fonts;
	std::vector<SDL_Surface*> m_textures;
	std::vector<CSubTexture> m_subtextures;
	bool m_keyboardState[SDLK_LAST];
	SDLKey m_lastKeyPressed;
	std::vector<Mix_Music*> m_music;
	std::vector<Mix_Chunk*> m_sounds;

	static const int m_debugFontSize = 12;
	static const int m_debugOverlayMaxLines = 25;
	int m_debugFont;
	std::deque<std::string> m_debugMessages;SDL_Colour m_debugColour;
	bool m_debugShown;

	unsigned int m_textureLastBound;

	int m_fpsCount;
	int m_tickRate;
	unsigned int m_clearScreenColour;

	CEngine();
	virtual ~CEngine();
	bool CreateFontTexture(int width, int height);
	static CEngine engine;

	bool Update();
	bool CreateWindow(int width, int height, std::string title);
	void FlipScreen();
	void ClearScreen();

public:
	static CEngine& Get();
	bool Run(CGame* game, int screenWidth, int screenHeight,
			const char* windowTitle);

	int GetScreenWidth();
	int GetScreenHeight();

	unsigned int GetTicks();
	void Sleep(unsigned int ms);

	int LoadTexture(std::string filename);
	bool RenderTexture(int textureNumber, int x, int y);
	void PurgeTextures();
	int GetTextureWidth(int textureNumber);
	int GetTextureHeight(int textureNumber);
	unsigned int GetTexturePixel(int textureNumber, int x, int y);

	int LoadSubTexture(unsigned int textureNumber, int x, int y, int w, int h);
	bool RenderSubTexture(int subTextureNumber, int x, int y);
	void PurgeSubTextures();
	int GetSubTextureWidth(int subTextureNumber);
	int GetSubTextureHeight(int subTextureNumber);

	int LoadFont(std::string filename, int size);
	bool RenderFont(int fontNumber, std::string text, int x, int y,
	SDL_Colour colour);
	void PurgeFonts();

	int LoadMusic(std::string filename);
	bool PlayMusic(int musicNumber);
	bool PauseMusic();
	bool ResumeMusic();
	bool StopMusic();
	void PurgeMusic();
	void SetMusicVolume(int percent);

	int LoadSound(std::string filename);
	bool PlaySound(int soundNumber);
	void PurgeSounds();

	bool DebugOverlayLoadFont(std::string filename, SDL_Color colour);
	void DebugOverlayClear();
	void DebugOverlayAdd(std::string s);
	void DebugOverLayRender();
	int DebugOverLayFont();
	void DebugOverlayShow(bool show);
	bool DebugOverlayShown();

	int GetFPSCount();
	void SetTickRate(int tickRate);
	void SetClearScreenColour(unsigned char r, unsigned char g,
			unsigned char b);

	bool mouseButton1Down;
	bool mouseButton2Down;
	bool mouseButton3Down;
	bool mouseButton4Down;
	bool mouseButton5Down;
	bool mouseWheelForward;
	bool mouseWheelBack;
	unsigned int mouseXPosition;
	unsigned int mouseYPosition;

	bool GetKeyDown(SDLKey key);
	SDLKey GetLastKeyPressed();

	bool CollideCircleCircle(int x1, int y1, int r1, int x2, int y2, int r2);
	bool CollideRectRect(int x1, int y1, int w1, int h1, int x2, int y2, int w2,
			int h2);

	bool CollidePointRect(int x1, int y1, int x2, int y2, int w2, int h2);
	bool CollidePointCircle(int x1, int y1, int x2, int y2, int r2);

	bool CollideCircleRect(int x1, int y1, int r1, int x2, int y2, int w2,
			int h2);
};

#define ENG CEngine::Get()

#endif /* CENGINE_H_ */
