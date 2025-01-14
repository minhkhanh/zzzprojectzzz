#pragma once

#include "Functions.h"

class MainGame;

class GameState
{
protected:
	MainGame *m_pMainGame;
	int m_iGameStateDef;

public:
	GameState(void);
	~GameState(void);

	virtual void MessageEnter(int message) = 0;
	virtual void MessageUpdate(int message, int keys) = 0;
	virtual void MessagePaint(int message, CDC *pDC) = 0;
	virtual void MessageExit(int message, CDC *pDC) = 0;

	// Handler cho các hàm xử lý tương ứng khi nhận message
	void StateHandler(int mesage, CDC *pDC, int keys);

	int GetGameStateDef();
};