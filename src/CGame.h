#ifndef CGAME_H_
#define CGAME_H_

#include <vector>
#include <queue>

class CGame
{
	static CGame game;

	int texSquareSheet;
	int texSquare[4];
	int posXSquare;
	int posYSquare;

	int texCircleSheet;
	int texCircle[4];
	int posXCircle;
	int posYCircle;

	int font0;
	int font1;

	int musMP3;

public:
	static CGame& Get();

	CGame();
	virtual ~CGame();

	void Init();
	void Render();
	void Update();
};

#define GAME CGame::Get()

#endif /* CGAME_H_ */
