#pragma once

#include "Functions.h"

#include "GameState.h"

class MainGame;
class MyBitmap;

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
