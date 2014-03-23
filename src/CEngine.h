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

const SDL_Color col_red =
{ 255, 0, 0, 255 };
const SDL_Color col_green =
{ 0, 255, 0, 255 };
const SDL_Color col_blue =
{ 0, 0, 255, 255 };
const SDL_Color col_white =
{ 255, 255, 255, 255 };

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
	std::vector<Mix_Music*> m_music;
	std::vector<Mix_Chunk*> m_sounds;

	int m_debugFont;
	std::deque<std::string> m_debugMessages;SDL_Colour m_debugColour;

	unsigned int m_textureLastBound;

	CEngine();
	virtual ~CEngine();
	bool CreateFontTexture(int width, int height);
	static CEngine engine;
public:
	static CEngine& Get();

	bool CreateWindow(int width, int height, std::string title);
	int GetScreenWidth();
	int GetScreenHeight();

	bool Update();
	unsigned int GetTicks();
	void Sleep(unsigned int ms);

	void FlipScreen();
	void ClearScreen();

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

	int LoadSound(std::string filename);
	bool PlaySound(int soundNumber);
	void PurgeSounds();

	bool DebugOverlayLoadFont(std::string filename, SDL_Color colour);
	void DebugOverlayClear();
	void DebugOverlayAdd(std::string s);
	void DebugOverLayRender();
	int DebugOverLayFont();

	bool mouseButton1Down;
	bool mouseButton2Down;
	bool mouseButton3Down;
	bool mouseButton4Down;
	bool mouseButton5Down;
	bool mouseWheelForward;
	bool mouseWheelBack;
	unsigned int mouseXPosition;
	unsigned int mouseYPosition;

	char keyboardLastPressed;

	bool GetKeyDown(SDLKey key);

	bool CollideCircleCircle(int x1, int y1, int r1, int x2, int y2, int r2);
	bool CollideRectRect(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

	bool CollidePointRect(int x1, int y1, int x2, int y2, int w2, int h2);
	bool CollidePointCircle(int x1, int y1, int x2, int y2, int r2);

	bool CollideCircleRect(int x1, int y1, int r1, int x2, int y2, int w2, int h2);
};

#define ENG CEngine::Get()

#endif /* CENGINE_H_ */
