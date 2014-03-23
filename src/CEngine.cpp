#define WIN32_LEAN_AND_MEAN

#include "CEngine.h"
#ifdef __linux__
#include <SDL/SDL_image.h>
#include <stdint.h>
#else
#include "SDL_image.h"
#endif
#include <math.h>
#include <iostream>

using namespace std;

#ifdef CreateWindow
#undef CreateWindow
#endif

CEngine CEngine::engine;

CEngine& CEngine::Get()
{
	return engine;
}

CEngine::CEngine() :
		m_windowCreated(false), m_screen(0), m_debugFont(-1), m_textureLastBound(
				0xFFFFFFFF), mouseButton1Down(0), mouseButton2Down(0), mouseButton3Down(
				0), mouseButton4Down(0), mouseButton5Down(0), mouseWheelForward(
				0), mouseWheelBack(0), mouseXPosition(0), mouseYPosition(0), keyboardLastPressed(
				0)
{
}

CEngine::~CEngine()
{
	StopMusic();

	PurgeTextures();
	PurgeMusic();
	PurgeSounds();

	TTF_Quit();
	SDL_Quit();
}

bool CEngine::CreateWindow(int width, int height, std::string title)
{
	bool r = true;

	unsigned int m_SdlFlags = SDL_HWPALETTE;

	if (0 != SDL_Init(SDL_INIT_VIDEO))
	{
		cout << "ERROR: SDL_Init [%s]\n" << SDL_GetError() << endl;
		exit(-1);
	}

	if (0 != TTF_Init())
	{
		cout << "ERROR: TTF_Init [%s]\n" << TTF_GetError() << endl;
		exit(-2);
	}

	if (0 != Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096))
	{
		cout << "ERROR: Mix_OpenAudio [%s]\n" << Mix_GetError() << endl;
		exit(-3);
	}

	SDL_EnableUNICODE(SDL_ENABLE);

	const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();

	if (videoInfo->hw_available)
	{
		m_SdlFlags |= SDL_HWSURFACE;
	}
	else
	{
		m_SdlFlags |= SDL_SWSURFACE;
	}

	if (videoInfo->blit_hw)
	{
		m_SdlFlags |= SDL_HWACCEL;
	}

	m_screen = SDL_SetVideoMode(width, height, 32, m_SdlFlags);

	if (!m_screen)
	{
		r = false;
		cout << "ERROR: SDL_SetVideoMode [%s]" << SDL_GetError() << endl;
	}
	else
	{
		SDL_WM_SetCaption(title.c_str(), 0);
	}

	m_windowCreated = r;

	return r;
}

int CEngine::GetScreenWidth()
{
	int r = 0;

	if (m_screen)
	{
		r = m_screen->w;
	}

	return r;
}
int CEngine::GetScreenHeight()
{
	int r = 0;

	if (m_screen)
	{
		r = m_screen->h;
	}

	return r;
}

bool CEngine::Update()
{
	bool r = true;
	SDL_Event e_event;

	mouseWheelForward = false;
	mouseWheelBack = false;
	keyboardLastPressed = SDLK_UNKNOWN;

	while (SDL_PollEvent(&e_event))
	{
		switch (e_event.type)
		{
		case SDL_QUIT:
			r = false;
			break;
		case SDL_MOUSEMOTION:
			mouseXPosition = e_event.button.x;
			mouseYPosition = e_event.button.y;
			break;
		case SDL_MOUSEBUTTONDOWN:
			mouseXPosition = e_event.button.x;
			mouseYPosition = e_event.button.y;
			switch (e_event.button.button)
			{
			case 1:
				mouseButton1Down = true;
				break;
			case 2:
				mouseButton2Down = true;
				break;
			case 3:
				mouseButton3Down = true;
				break;
			case 4:
				mouseWheelBack = true;
				break;
			case 5:
				mouseWheelForward = true;
				break;
			case 6:
				mouseButton4Down = true;
				break;
			case 7:
				mouseButton5Down = true;
				break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			mouseXPosition = e_event.button.x;
			mouseYPosition = e_event.button.y;
			switch (e_event.button.button)
			{
			case 1:
				mouseButton1Down = false;
				break;
			case 2:
				mouseButton2Down = false;
				break;
			case 3:
				mouseButton3Down = false;
				break;
			case 4:
			case 5:
				break;
			case 6:
				mouseButton4Down = false;
				break;
			case 7:
				mouseButton5Down = false;
				break;
			}
			break;
		case SDL_KEYDOWN:
			m_keyboardState[e_event.key.keysym.sym] = true;
			keyboardLastPressed = e_event.key.keysym.unicode;
			break;
		case SDL_KEYUP:
			m_keyboardState[e_event.key.keysym.sym] = false;
			break;
		}
	}
	return r;
}
unsigned int CEngine::GetTicks()
{
	return SDL_GetTicks();
}
void CEngine::Sleep(unsigned int ms)
{
	SDL_Delay(ms);
}
void CEngine::FlipScreen()
{
	SDL_Flip(m_screen);
}
void CEngine::ClearScreen()
{
	SDL_FillRect(m_screen, 0, 0);
}

int CEngine::LoadTexture(std::string filename)
{
	int r = -1;

	SDL_Surface* image_loaded = 0;
	SDL_Surface* image_optimised = 0;

	if (!m_windowCreated)
	{
		cout << "WARNING: Window not created before loading image" << endl;
	}
	else if (0 == (image_loaded = IMG_Load(filename.c_str())))
	{
		cout << "WARNING: SDL_LoadBMP [" << SDL_GetError() << "]" << endl;
	}
	else if (0 == (image_optimised = SDL_DisplayFormatAlpha(image_loaded)))
	{
		cout << "WARNING: SDL_DisplayFormat [" << SDL_GetError() << "]" << endl;
	}
	else
	{
		m_textures.push_back(image_optimised);
		r = m_textures.size() - 1;
	}

	if (image_loaded)
	{
		SDL_FreeSurface(image_loaded);
	}

	return r;
}

bool CEngine::RenderTexture(int textureNumber, int x, int y)
{
	bool r = false;

	SDL_Rect rect =
	{ 0, 0, 0, 0 };
	rect.x = x;
	rect.y = y;

	if (textureNumber < 0)
	{
	}
	else if (static_cast<size_t>(textureNumber) >= m_textures.size())
	{
	}
	else
	{
		SDL_BlitSurface(m_textures[textureNumber], 0, m_screen, &rect);
	}

	return r;
}

void CEngine::PurgeTextures()
{
	std::vector<SDL_Surface*>::iterator textures_it = m_textures.begin();
	for (textures_it = m_textures.begin(); textures_it != m_textures.end();
			textures_it++)
	{
		SDL_FreeSurface(*textures_it);
	}
	m_textures.clear();

	PurgeSubTextures();
}

int CEngine::GetTextureWidth(int textureNumber)
{
	int r = 0;

	if (textureNumber < 0)
	{
	}
	else if (static_cast<size_t>(textureNumber) >= m_textures.size())
	{
	}
	else
	{
		r = m_textures[textureNumber]->w;
	}

	return r;
}
int CEngine::GetTextureHeight(int textureNumber)
{
	int r = 0;

	if (textureNumber < 0)
	{
	}
	else if (static_cast<size_t>(textureNumber) >= m_textures.size())
	{
	}
	else
	{
		r = m_textures[textureNumber]->h;
	}

	return r;
}
unsigned int CEngine::GetTexturePixel(int textureNumber, int x, int y)
{
	unsigned int r = 0;
	SDL_Surface* t = 0;

	if (textureNumber < 0)
	{
	}
	else if (x < 0)
	{
	}
	else if (y < 0)
	{
	}
	else if (static_cast<size_t>(textureNumber) >= m_textures.size())
	{
	}
	else if (!(t = m_textures[textureNumber]))
	{
	}
	else if (x > t->w)
	{
	}
	else if (y > t->h)
	{
	}
	else
	{
		unsigned int *ptr = (unsigned int*) t->pixels;
		int lineoffset = y * (t->pitch / 4);
		r = ptr[lineoffset + x];
	}

	return r;
}

int CEngine::LoadSubTexture(unsigned int textureNumber, int x, int y, int w,
		int h)
{
	int r = -1;

	if (textureNumber < 0)
	{
	}
	else if (w < 0 || h < 0)
	{
	}
	else if (static_cast<size_t>(textureNumber) >= m_textures.size())
	{
	}
	else
	{
		CSubTexture sub;
		sub.m_texture = textureNumber;
		sub.m_src.x = x;
		sub.m_src.y = y;
		sub.m_src.w = w;
		sub.m_src.h = h;
		m_subtextures.push_back(sub);
		r = m_subtextures.size() - 1;
	}

	return r;
}
bool CEngine::RenderSubTexture(int subTextureNumber, int x, int y)
{
	bool r = false;

	if (subTextureNumber < 0)
	{
	}
	else if (static_cast<size_t>(subTextureNumber) >= m_subtextures.size())
	{
	}
	else
	{
		CSubTexture sub = m_subtextures[subTextureNumber];

		if (sub.m_texture < 0)
		{
		}
		else if (static_cast<size_t>(sub.m_texture) >= m_textures.size())
		{
		}
		else
		{
			SDL_Rect dst =
			{ 0, 0, 0, 0 };
			dst.x = x;
			dst.y = y;

			SDL_BlitSurface(m_textures[sub.m_texture], &sub.m_src, m_screen,
					&dst);
		}
	}

	return r;
}
void CEngine::PurgeSubTextures()
{
	m_subtextures.clear();
}

int CEngine::GetSubTextureWidth(int subTextureNumber)
{
	int r = 0;

	if (subTextureNumber < 0)
	{
	}
	else if (static_cast<size_t>(subTextureNumber) >= m_subtextures.size())
	{
	}
	else
	{
		r = m_subtextures[subTextureNumber].m_src.w;
	}

	return r;
}
int CEngine::GetSubTextureHeight(int subTextureNumber)
{
	int r = 0;

	if (subTextureNumber < 0)
	{
	}
	else if (static_cast<size_t>(subTextureNumber) >= m_subtextures.size())
	{
	}
	else
	{
		r = m_subtextures[subTextureNumber].m_src.h;
	}

	return r;
}

int CEngine::LoadFont(std::string filename, int size)
{
	int r = -1;

	TTF_Font* ttfFont = 0;

	if (0 == (ttfFont = TTF_OpenFont(filename.c_str(), size)))
	{
		cout << "WARNING: TTF_OpenFont [%s]\n" << TTF_GetError() << endl;
	}
	else
	{
		m_fonts.push_back(ttfFont);
		r = m_fonts.size() - 1;
	}

	return r;
}
bool CEngine::RenderFont(int fontNumber, std::string text, int x, int y,
SDL_Colour colour)
{
	bool r = false;

	if (fontNumber < 0)
	{
	}
	else if (static_cast<size_t>(fontNumber) >= m_fonts.size())
	{
	}
	else
	{
		SDL_Surface* s = TTF_RenderText_Solid(m_fonts[fontNumber], text.c_str(),
				colour);

		SDL_Rect rect =
		{ 0, 0, 0, 0 };
		rect.x = x;
		rect.y = y;

		if (s != 0)
		{
			SDL_BlitSurface(s, 0, m_screen, &rect);
		}

		SDL_FreeSurface(s);
	}

	return r;
}
void CEngine::PurgeFonts()
{
	m_fonts.clear();
}

bool CEngine::GetKeyDown(SDLKey key)
{
	bool r = false;

	if (key < SDLK_LAST)
	{
		r = m_keyboardState[key];
	}

	return r;
}

int CEngine::LoadMusic(std::string filename)
{
	int r = -1;

	Mix_Music* music = 0;

	if (0 == (music = Mix_LoadMUS(filename.c_str())))
	{
		cout << "WARNING: Mix_LoadMUS [%s]\n" << Mix_GetError() << endl;
	}
	else
	{
		m_music.push_back(music);
		r = m_music.size() - 1;
	}

	return r;
}
bool CEngine::PlayMusic(int musicNumber)
{
	bool r = false;

	Mix_Music* music = 0;

	if (musicNumber < 0)
	{
	}
	else if (static_cast<size_t>(musicNumber) >= m_music.size())
	{
	}
	else if (0 == (music = m_music[musicNumber]))
	{
	}
	else
	{
		if (Mix_PlayingMusic())
		{
			Mix_HaltMusic();
		}
		Mix_PlayMusic(music, -1);

		r = true;
	}

	return r;
}
bool CEngine::PauseMusic()
{
	bool r = false;

	if (0 == Mix_PlayingMusic())
	{
	}
	else if (0 != Mix_PausedMusic())
	{
	}
	else
	{
		Mix_PauseMusic();
		r = true;
	}

	return r;
}
bool CEngine::ResumeMusic()
{
	bool r = false;

	if (0 == Mix_PlayingMusic())
	{
	}
	else if (0 == Mix_PausedMusic())
	{
	}
	else
	{
		Mix_ResumeMusic();
		r = true;
	}

	return r;
}
bool CEngine::StopMusic()
{
	bool r = false;

	if (0 == Mix_PlayingMusic())
	{
	}
	else
	{
		Mix_HaltMusic();
		r = true;
	}

	return r;
}
void CEngine::PurgeMusic()
{
	std::vector<Mix_Music*>::iterator music_it = m_music.begin();
	for (music_it = m_music.begin(); music_it != m_music.end(); music_it++)
	{
		Mix_FreeMusic(*music_it);
		*music_it = 0;
	}
	m_music.clear();
}

int CEngine::LoadSound(std::string filename)
{
	int r = -1;

	Mix_Chunk* music = 0;

	if (0 == (music = Mix_LoadWAV(filename.c_str())))
	{
		cout << "WARNING: Mix_LoadWAV [%s]\n" << Mix_GetError() << endl;
	}
	else
	{
		m_sounds.push_back(music);
		r = m_sounds.size() - 1;
	}

	return r;
}
bool CEngine::PlaySound(int soundNumber)
{
	bool r = false;

	if (soundNumber < 0)
	{
	}
	else if (static_cast<size_t>(soundNumber) >= m_sounds.size())
	{
	}
	else if (0 == m_sounds[soundNumber])
	{
	}
	else if (0 != Mix_PlayChannel(-1, m_sounds[soundNumber], 0))
	{
	}
	else
	{
		r = true;
	}

	return r;
}
void CEngine::PurgeSounds()
{
	std::vector<Mix_Chunk*>::iterator sound_it = m_sounds.begin();
	for (sound_it = m_sounds.begin(); sound_it != m_sounds.end(); sound_it++)
	{
		Mix_FreeChunk(*sound_it);
		*sound_it = 0;
	}
	m_sounds.clear();
}

bool CEngine::CollideCircleCircle(int x1, int y1, int r1, int x2, int y2,
		int r2)
{
	x1 -= x2;
	y1 -= y2;
	r1 += r2;

	x1 *= x1;
	y1 *= y1;
	r1 *= r1;

	if (x1 + y1 < r1)
	{
		return true;
	}

	return false;
}

bool CEngine::CollideRectRect(int x1, int y1, int w1, int h1, int x2, int y2,
		int w2, int h2)
{
	if ((x2 < x1 + w1) && (x2 + w2 > x1))
	{
		if ((y2 < y1 + h1) && (y2 + h2 > y1))
		{
			return true;
		}
	}

	return false;
}

bool CEngine::CollidePointRect(int x1, int y1, int x2, int y2, int w2, int h2)
{
	if ((x2 < x1) && (x2 + w2 > x1))
	{
		if ((y2 < y1) && (y2 + h2 > y1))
		{
			return true;
		}
	}

	return false;
}

bool CEngine::CollidePointCircle(int x1, int y1, int x2, int y2, int r2)
{
	x1 -= x2;
	y1 -= y2;

	x1 *= x1;
	y1 *= y1;
	r2 *= r2;

	if (x1 + y1 < r2)
	{
		return true;
	}

	return false;
}

bool CEngine::CollideCircleRect(int x1, int y1, int r1, int x2, int y2, int w2,
		int h2)
{
	if (CollidePointRect(x1, y1, x2, y2, w2, h2))
	{
		return true;
	}

	int x3 = x1;
	int y3 = y1;

	if (x1 < x2)
	{
		x3 = x2;
	}
	else if (x1 > x2 + w2)
	{
		x3 = x2 + w2;
	}

	if (y1 < y2)
	{
		y3 = y2;
	}
	else if (y1 > y2 + h2)
	{
		y3 = y2 + h2;
	}

	if(CollidePointCircle(x3,y3,x1,y1,r1))
	{
		return true;
	}

	return false;
}

bool CEngine::DebugOverlayLoadFont(std::string filename, SDL_Color colour)
{
	bool r = true;

	m_debugFont = LoadFont(filename, 18);
	m_debugColour = colour;

	return r;
}
void CEngine::DebugOverlayClear()
{
	m_debugMessages.clear();
}
void CEngine::DebugOverlayAdd(std::string s)
{
	m_debugMessages.push_back(s);

	if (m_debugMessages.size() > 25)
	{
		m_debugMessages.pop_front();
	}
}
void CEngine::DebugOverLayRender()
{
	int y = m_screen->h - 15;

	deque<string>::iterator debug_it = m_debugMessages.begin();
	for (debug_it = m_debugMessages.begin(); debug_it != m_debugMessages.end();
			++debug_it)
	{
		RenderFont(m_debugFont, (*debug_it), 5, y, m_debugColour);
		y -= 18;
	}
}

int CEngine::DebugOverLayFont()
{
	return m_debugFont;
}
