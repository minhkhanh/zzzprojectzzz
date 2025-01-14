#pragma once

#include "Functions.h"

#include "GameState.h"

class MainGame;

class GameStateLogo: public GameState
{
private:
	MyBitmap *m_pBackground;		// Lưu trữ ảnh background
	Sprite *m_pMenuIcon;			// Demo Sprite

	int m_iX;

public:
	MyBitmap * m_pTenGame;
	
	GameStateLogo(MainGame *_mg);
	~GameStateLogo(void);

	// Nạp chồng hàm xử lý các sự kiện tương ứng
	void MessageEnter(int message);
	void MessageUpdate(int message, int keys);
	void MessagePaint(int message, CDC *pDC);
	void MessageExit(int message, CDC *pDC);
};
