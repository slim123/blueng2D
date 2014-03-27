#ifndef CGAMETEST_H_
#define CGAMETEST_H_

#include "CEngine.h"

class CGameTest : public CGame
{
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
	int musVolume;

	int sndAlien;
	int sndRooster;

public:
	CGameTest();
	virtual ~CGameTest();

	void Init();
	void Render();
	void Update();
};

#endif /* CGAMETEST_H_ */
