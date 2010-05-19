#pragma once

#include "stdafx.h"

#include "GameState.h"
#include "MyBitmap.h"
#include "cDef.h"
#include "0812223-0812239.h"
#include "Sprite.h"
#include "GameKeys.h"
#include "MainGame.h"

class GameStateMenu :	public GameState
{
public:
	GameStateMenu(MainGame *_mg);
	virtual ~GameStateMenu(void);

	MyBitmap *imgBG;		// Lưu trữ ảnh background
	vector<LPCTSTR> m_arrMenuText;
	int m_iCurrMenuItem;

	const int MAX_ITEM;
	const int MENU_STATUS_SIZE;

	void MessageEnter(int message);
	void MessageUpdate(int message, int keys);
	void MessagePaint(int message, CDC *pDC);
	void MessageExit(int message, CDC *pDC);

	void TextMenuOut();
	void DrawMenuStatusBar();
};
