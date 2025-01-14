#include "StdAfx.h"
#include "GameStateMenu.h"

#include "MainGame.h"

GameStateMenu::GameStateMenu(MainGame *_mg): m_iCurrMenuItem(0)
{
	m_pMainGame = _mg;
	m_iGameStateDef = cDef::STATE_MENU;

	// khởi tạo menu text: phải đúng thứ tự #define như bên file .h
	m_vMenuText.push_back(L"    New Game  >>");
	m_vMenuText.push_back(L"<<  GamePlay  >>");
	m_vMenuText.push_back(L"<<  Settings    ");
}

GameStateMenu::~GameStateMenu(void)
{
}

void GameStateMenu::MessageEnter(int message)
{
	m_pMainGame->m_iSleepTime = 50;

	GameSound::Play(MENU_THEME_SOUND);

	m_pBackGround = new MyBitmap((LPCTSTR)IDIMG_MENUBACKGROUND);

	m_pBackGround->DrawTransparent(&(m_pMainGame->m_MemDC), 0, 0, RGB(113,113,113));
	m_pMainGame->m_pDC->StretchBlt(0, 0, cDef::SCREEN_WIDTH, cDef::SCREEN_HEIGHT, &(m_pMainGame->m_MemDC), 0, 0 , cDef::SCREEN_WIDTH, cDef::SCREEN_HEIGHT, SRCCOPY);

	this->TextMenuOut();
}

void GameStateMenu::MessageUpdate(int message, int keys)
{
	if ((keys & GameKeys::Right) && m_iCurrMenuItem < MAX_MENU_ITEM-1)
	{
		GameSound::Play(COIN_SOUND);

		++m_iCurrMenuItem;

		this->TextMenuOut();
		m_pMainGame->KeyRelease(GameKeys::Right);
	}
	
	if ((keys & GameKeys::Left) && m_iCurrMenuItem > 0)
	{
		GameSound::Play(COIN_SOUND);

		--m_iCurrMenuItem;

		this->TextMenuOut();
		m_pMainGame->KeyRelease(GameKeys::Left);
	}

	if ((keys & GameKeys::Enter) == GameKeys::Enter)
	{
		m_pMainGame->mBiz_SwitchState(1);	
	}
}

void GameStateMenu::MessagePaint(int message, CDC *pDC)
{
}

void GameStateMenu::MessageExit(int message, CDC *pDC)
{
	if (m_iCurrMenuItem == NEW_GAME_ITEM)
		m_pMainGame->mBiz_iCurrentState = cDef::STATE_TYPE_A;
	else if (m_iCurrMenuItem == HOWTOPLAY_ITEM)
		m_pMainGame->mBiz_iCurrentState = cDef::STATE_HOWTOPLAY;
	else
		m_pMainGame->mBiz_iCurrentState = cDef::STATE_SETTINGS;
}

void GameStateMenu::TextMenuOut()
{
	m_pBackGround->DrawTransparent(&(m_pMainGame->m_MemDC), 0, 0, RGB(113,113,113));
	m_pMainGame->m_pDC->StretchBlt(0, 0, cDef::SCREEN_WIDTH, cDef::SCREEN_HEIGHT, &(m_pMainGame->m_MemDC), 0, 0 , cDef::SCREEN_WIDTH, cDef::SCREEN_HEIGHT, SRCCOPY);

	int iOldBkMode = m_pMainGame->m_pDC->SetBkMode(TRANSPARENT);
	RECT rect;
	rect.top = 170;
	rect.left = 5;
	rect.right = cDef::SCREEN_WIDTH - 5;
	rect.bottom = 220;

	CFont font;
	font.CreatePointFont(200, L"Courier");
	CFont *pOldFont = m_pMainGame->m_pDC->SelectObject(&font);
	int iOldTxtClr = m_pMainGame->m_pDC->SetTextColor(RGB(0,255,0));
	m_pMainGame->m_pDC->DrawText(m_vMenuText[m_iCurrMenuItem], -1, &rect, DT_CENTER);

	m_pMainGame->m_pDC->SelectObject(pOldFont);
	m_pMainGame->m_pDC->SetBkMode(iOldBkMode);
	m_pMainGame->m_pDC->SetTextColor(iOldTxtClr);

	//this->DrawMenuNavigationBar();
}

void GameStateMenu::DrawMenuNavigationBar()
{
	CBrush brushRed, brushGreen;
	brushRed.CreateSolidBrush(RGB(255,0,0));
	brushGreen.CreateSolidBrush(RGB(0,255,0));

	m_pMainGame->m_pDC->SelectObject(&brushRed);

	int iStartPosX = cDef::SCREEN_WIDTH / 2 - (MAX_MENU_ITEM / 2) * (MENU_STATUS_SIZE + 5);
	for (int i = 0; i < MAX_MENU_ITEM; ++i)
	{
		if (i == m_iCurrMenuItem)
			m_pMainGame->m_pDC->SelectObject(&brushGreen);
		else
			m_pMainGame->m_pDC->SelectObject(&brushRed);
		//m_pMainGame->m_pDC->

		m_pMainGame->m_pDC->Rectangle(iStartPosX + i*(MENU_STATUS_SIZE+5), 200, iStartPosX + i*(MENU_STATUS_SIZE+5) + MENU_STATUS_SIZE, 200 + MENU_STATUS_SIZE);
	}
}