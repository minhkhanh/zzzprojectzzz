#include "StdAfx.h"
#include "GameStateMenu.h"

GameStateMenu::GameStateMenu(MainGame *_mg)
{
	mg = _mg;
	GAME_STATE_DEF = cDef::STATE_MENU;

	// khởi tạo menu text
	m_arrMenuText.push_back(L"New Game");
	m_arrMenuText.push_back(L"Settings");

	m_iCurrMenuItem = 0;
}

GameStateMenu::~GameStateMenu(void)
{
}

void GameStateMenu::MessageEnter(int message)
{
	imgBG = new MyBitmap((LPCTSTR)IDIMG_MENUBACKGROUND);

	imgBG->DrawTransparent(&(mg->m_MemDC), 0, 0, RGB(255,255,255));
	mg->pDC->StretchBlt(0, 0, cDef::MAINGAME_WIDTH, cDef::MAINGAME_HEIGHT, &(mg->m_MemDC), 0, 0 , 240, 320, SRCCOPY);

	this->TextMenuOut();
}

void GameStateMenu::MessageUpdate(int message, int keys)
{
	if ((keys & GameKeys::Right) == GameKeys::Right
		|| (keys & GameKeys::Up) == GameKeys::Up)
	{
		++m_iCurrMenuItem;
		if (m_iCurrMenuItem >= MAX_ITEM)
			m_iCurrMenuItem = 0;

		this->TextMenuOut();
	}
	
	if ((keys & GameKeys::Left) == GameKeys::Left
		|| (keys & GameKeys::Down) == GameKeys::Down)
	{
		--m_iCurrMenuItem;
		if (m_iCurrMenuItem < 0)
			m_iCurrMenuItem = MAX_ITEM - 1;

		this->TextMenuOut();
	}

	if ((keys & GameKeys::Enter) == GameKeys::Enter)
	{
		mg->mBiz_SwitchState(cDef::STATE_LOGO);
	}
}

void GameStateMenu::MessagePaint(int message, CDC *pDC)
{
	
}

void GameStateMenu::MessageExit(int message, CDC *pDC)
{
	mg->mBiz_iCurrentState = cDef::STATE_TYPE_A;
}

void GameStateMenu::TextMenuOut()
{
	imgBG->DrawTransparent(&(mg->m_MemDC), 0, 0, RGB(255,255,255));
	mg->pDC->StretchBlt(0, 0, cDef::MAINGAME_WIDTH, cDef::MAINGAME_HEIGHT, &(mg->m_MemDC), 0, 0 , 240, 320, SRCCOPY);

	int iOldBkMode = mg->pDC->SetBkMode(TRANSPARENT);
	RECT rect;
	rect.top = 5;
	rect.left = 0;
	rect.right = cDef::MAINGAME_WIDTH;
	rect.bottom = 50;

	CFont font;
	font.CreatePointFont(170, L"Tahoma");
	CFont *pOldFont = mg->pDC->SelectObject(&font);
	mg->pDC->DrawText(m_arrMenuText[m_iCurrMenuItem], -1, &rect, DT_CENTER);

	mg->pDC->SelectObject(pOldFont);
	mg->pDC->SetBkMode(iOldBkMode);

	this->DrawMenuStatusBar();
}

void GameStateMenu::DrawMenuStatusBar()
{
	CBrush brushRed, brushGreen;
	brushRed.CreateSolidBrush(RGB(255,0,0));
	brushGreen.CreateSolidBrush(RGB(0,255,0));

	mg->pDC->SelectObject(&brushRed);

	int iStartPosX = cDef::MAINGAME_WIDTH / 2 - (MAX_ITEM / 2) * (MENU_STATUS_SIZE + 5);
	for (int i = 0; i < MAX_ITEM; ++i)
	{
		if (i == m_iCurrMenuItem)
			mg->pDC->SelectObject(&brushGreen);
		else
			mg->pDC->SelectObject(&brushRed);
		mg->pDC->Rectangle(iStartPosX + i*(MENU_STATUS_SIZE+5), 200, iStartPosX + i*(MENU_STATUS_SIZE+5) + MENU_STATUS_SIZE, 200 + MENU_STATUS_SIZE);
	}
}