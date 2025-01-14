#pragma once

#include "Functions.h"

#include "gamestate.h"

class MyScriptMap;

class GameStateFinish :	public GameState
{
private:
	MyScriptMap * m_pScriptText;
	MyBitmap *m_pBackGroundBMP;
	bool bNeedRedraw;

	int m_iStep;

public:
	GameStateFinish(MainGame *mg, TCHAR* szScriptPath);
	virtual ~GameStateFinish(void);

	void MessageEnter(int message);
	void MessageUpdate(int message, int keys);
	void MessagePaint(int message, CDC *pDC);
	void MessageExit(int message, CDC *pDC);
};
