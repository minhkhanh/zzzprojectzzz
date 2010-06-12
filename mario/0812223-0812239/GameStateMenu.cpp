#include "StdAfx.h"
#include "GameStateMenu.h"

#include "MainGame.h"

GameStateMenu::GameStateMenu(MainGame *_mg): m_iCurrMenuItem(0)
{
	m_pMainGame = _mg;
	m_iGameStateDef = cDef::STATE_MENU;

	// khởi tạo menu text
	m_vMenuText.push_back(L"New Game");
	m_vMenuText.push_back(L"Settings");
}

GameStateMenu::~GameStateMenu(void)
{
}

void GameStateMenu::MessageEnter(int message)
{
	GameSound::MenuTheme(1);

	m_pBackGround = new MyBitmap((LPCTSTR)IDIMG_MENUBACKGROUND);

	m_pBackGround->DrawTransparent(&(m_pMainGame->m_MemDC), 0, 0, RGB(255,255,255));
	m_pMainGame->m_pDC->StretchBlt(0, 0, cDef::SCREEN_WIDTH, cDef::SCREEN_HEIGHT, &(m_pMainGame->m_MemDC), 0, 0 , 240, 320, SRCCOPY);

	this->TextMenuOut();

	m_pMainGame->m_iSleepTime = 100;
}

void GameStateMenu::MessageUpdate(int message, int keys)
{
	if ((keys & GameKeys::Right) == GameKeys::Right
		|| (keys & GameKeys::Up) == GameKeys::Up)
	{
		GameSound::Coin(1);
		++m_iCurrMenuItem;
		if (m_iCurrMenuItem >= MAX_ITEM)
			m_iCurrMenuItem = 0;

		this->TextMenuOut();
		m_pMainGame->KeyRelease(GameKeys::Right);
		m_pMainGame->KeyRelease(GameKeys::Up);
	}
	
	if ((keys & GameKeys::Left) == GameKeys::Left
		|| (keys & GameKeys::Down) == GameKeys::Down)
	{
		GameSound::Coin(1);
		--m_iCurrMenuItem;
		if (m_iCurrMenuItem < 0)
			m_iCurrMenuItem = MAX_ITEM - 1;

		this->TextMenuOut();
		m_pMainGame->KeyRelease(GameKeys::Left);
		m_pMainGame->KeyRelease(GameKeys::Down);
	}

	if ((keys & GameKeys::Enter) == GameKeys::Enter)
	{
		GameSound::MenuTheme(0);
		GameSound::EnterMainGame(1);

		m_pMainGame->mBiz_SwitchState(cDef::STATE_LOGO);
		m_pMainGame->KeyRelease(GameKeys::Enter);
	}
}

void GameStateMenu::MessagePaint(int message, CDC *pDC)
{
}

void GameStateMenu::MessageExit(int message, CDC *pDC)
{
	GameSound::MenuTheme(0);
	m_pMainGame->mBiz_iCurrentState = cDef::STATE_TYPE_A;
}

void GameStateMenu::TextMenuOut()
{
	m_pBackGround->DrawTransparent(&(m_pMainGame->m_MemDC), 0, 0, RGB(255,255,255));
	m_pMainGame->m_pDC->StretchBlt(0, 0, cDef::SCREEN_WIDTH, cDef::SCREEN_HEIGHT, &(m_pMainGame->m_MemDC), 0, 0 , 240, 320, SRCCOPY);

	int iOldBkMode = m_pMainGame->m_pDC->SetBkMode(TRANSPARENT);
	RECT rect;
	rect.top = 5;
	rect.left = 0;
	rect.right = cDef::SCREEN_WIDTH;
	rect.bottom = 50;

	CFont font;
	font.CreatePointFont(170, L"Tahoma");
	CFont *pOldFont = m_pMainGame->m_pDC->SelectObject(&font);
	m_pMainGame->m_pDC->DrawText(m_vMenuText[m_iCurrMenuItem], -1, &rect, DT_CENTER);

	m_pMainGame->m_pDC->SelectObject(pOldFont);
	m_pMainGame->m_pDC->SetBkMode(iOldBkMode);

	this->DrawMenuStatusBar();
}

void GameStateMenu::DrawMenuStatusBar()
{
	CBrush brushRed, brushGreen;
	brushRed.CreateSolidBrush(RGB(255,0,0));
	brushGreen.CreateSolidBrush(RGB(0,255,0));

	m_pMainGame->m_pDC->SelectObject(&brushRed);

	int iStartPosX = cDef::SCREEN_WIDTH / 2 - (MAX_ITEM / 2) * (MENU_STATUS_SIZE + 5);
	for (int i = 0; i < MAX_ITEM; ++i)
	{
		if (i == m_iCurrMenuItem)
			m_pMainGame->m_pDC->SelectObject(&brushGreen);
		else
			m_pMainGame->m_pDC->SelectObject(&brushRed);

		m_pMainGame->m_pDC->Rectangle(iStartPosX + i*(MENU_STATUS_SIZE+5), 200, iStartPosX + i*(MENU_STATUS_SIZE+5) + MENU_STATUS_SIZE, 200 + MENU_STATUS_SIZE);
	}
}