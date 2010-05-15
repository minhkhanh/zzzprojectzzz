#pragma once

#include "stdafx.h"

#include "GameState.h"
#include "MyBitmap.h"
#include "cDef.h"
#include "0812223-0812239.h"
#include "Sprite.h"
#include "GameKeys.h"
#include "MainGame.h"

class GameStateLogo: GameState
{
public:
	GameStateLogo(MainGame *_mg);
	~GameStateLogo(void);

public:
	MyBitmap *imgBG;		// Lưu trữ ảnh background
	Sprite *sprMenuIcon;	// Demo Sprite

	int m_iX;

	// Nạp chồng hàm xử lý các sự kiện tương ứng
	void MessageEnter(int message);
	void MessageUpdate(int message, int keys);
	void MessagePaint(int message, CDC *pDC);
	void MessageExit(int message, CDC *pDC);
};
