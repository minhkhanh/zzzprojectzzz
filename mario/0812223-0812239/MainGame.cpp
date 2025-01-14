#include "StdAfx.h"
#include "MainGame.h"

#include "GameStateIntro.h"
#include "GameStateLogo.h"
#include "GameStateMenu.h"
#include "GameStateTypeA.h"
#include "GameStateHowToPlay.h"
#include "GameStateSettings.h"
#include "GameStateOver.h"
#include "GameStateFinish.h"

MainGame::MainGame(void): m_iKeys(0), m_iSleepTime(100), m_iPlaying(true), m_iLiveCount(3), m_iCoinCount(0)
{
	mBiz_iCurrentState = cDef::STATE_LOGO;

	// khởi tạo tất cả các State trong game:

	m_arrGameStates.push_back((GameState*)new GameStateLogo(this));
	m_arrGameStates.push_back((GameState*)new GameStateMenu(this));
	m_arrGameStates.push_back((GameState*)new GameStateTypeA(this));
	m_arrGameStates.push_back((GameState*)new GameStateIntro(this, L"Resource\\Level1\\ScriptText.txt"/*L"Resource\\Scripts\\GameOver_Script.txt"*/,(LPCTSTR)IDIMG_BGR_INTRO /*L"Resource\\background_intro.bmp"*/));
	m_arrGameStates.push_back((GameState*)new GameStateHowToPlay(this));
	m_arrGameStates.push_back((GameState*)new GameStateSettings(this));
	m_arrGameStates.push_back((GameState*)new GameStateOver(this, L"Resource\\Scripts\\GameOver_Script.txt"));
	m_arrGameStates.push_back((GameState*)new GameStateFinish(this, L"Resource\\Scripts\\GameFinish_Script.txt"));

	//--

	mBiz_bEnterNextState = true;
	mBiz_bExitCurrentState = false;
	// --

	// load icon của nhân vật nam chính
	m_bmOcegrineIcon = new MyBitmap((LPCTSTR)IDIMG_OCEGRINE_ICON);
	// load icon đồng xu
	m_bmCoinIcon = new MyBitmap((LPCTSTR)IDIMG_COIN_ICON);
}

MainGame::~MainGame(void)
{
	m_MemDC.DeleteDC();
	m_MemBitmap.DeleteObject();

//	m_pDC

	delete m_pGraphics;
	delete m_pMemGraphics;
	GdiplusShutdown(m_gdiplusToken);
}

GameState *MainGame::GetGameState(int _gsID)
{
	GameState *_gs = NULL;
	for (unsigned i = 0; i < m_arrGameStates.size(); ++i)
	{
		_gs = m_arrGameStates[i];
		if (_gs->GetGameStateDef() == _gsID)
		{
			return _gs;
		}
	}

	return NULL;
}

void MainGame::mBiz_SwitchState(int stateID)
{
	mBiz_bExitCurrentState = true;
	KeyClear();
}

void MainGame::mBiz_SendMessage(int message)
{
	GameState *_gs = GetGameState(mBiz_iCurrentState);
	if (_gs != NULL)
	{
		_gs->StateHandler(message, m_pDC, m_iKeys);
	}
}

void MainGame::GameLoop()
{
	while (m_iPlaying)
	{
		// xử lý khi mới vào State
		if (mBiz_bEnterNextState)
		{
			mBiz_bEnterNextState = false;
			mBiz_SendMessage(cDef::MESSAGE_ENTER);
		}

		// xử lý logic trong 1 vòng lặp Game
		mBiz_SendMessage(cDef::MESSAGE_UPDATE);

		if (mBiz_bExitCurrentState)
		{
			mBiz_SendMessage(cDef::MESSAGE_EXIT);
			mBiz_bExitCurrentState = false;
			mBiz_bEnterNextState = true;
		}
		else
			mBiz_SendMessage(cDef::MESSAGE_PAINT);

		::Sleep(m_iSleepTime);
	}
}

void MainGame::KeyPressed(int iKey)
{
	m_iKeys |= iKey;
}

void MainGame::KeyRelease(int iKey)
{
	m_iKeys &= ~iKey;
}

void MainGame::SetPDC(CDC *dc)
{
	m_pDC = dc;

	// tạo MemDC:
	int nScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);

	m_MemDC.CreateCompatibleDC(m_pDC);
	m_MemBitmap.CreateCompatibleBitmap(m_pDC, nScreenWidth, nScreenHeight);
	m_pOldBitmap = m_MemDC.SelectObject(&m_MemBitmap);

	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
	m_pImageGraphics = new Bitmap(cDef::SCREEN_WIDTH, cDef::SCREEN_HEIGHT);
	m_pGraphics = new Graphics(dc->m_hDC);
	m_pMemGraphics = new Graphics(m_pImageGraphics);
}

void MainGame::DrawGameStatus()
{
	int iOldBkMode = m_MemDC.SetBkMode(TRANSPARENT);

	m_bmOcegrineIcon->DrawTransparent(&m_MemDC, 1, 1, RGB(255,255,255));

	m_MemDC.SetTextColor(RGB(255,0,0));
	m_MemDC.TextOut(15, 4, L"X");

	TCHAR lpszCount[4];
	wsprintf(lpszCount, L"%d", m_iLiveCount);
	m_MemDC.TextOut(25, 4, lpszCount);

	m_bmCoinIcon->DrawTransparent(&m_MemDC, 1, 18, RGB(255,255,255));

	m_MemDC.TextOut(15, 21, L"X");
	wsprintf(lpszCount, L"%d", m_iCoinCount);
	m_MemDC.TextOut(25, 21, lpszCount);

	m_MemDC.SetBkMode(iOldBkMode);
}

void MainGame::KeyClear()
{
	m_iKeys = 0;
}

int MainGame::KeyGet()
{
	return m_iKeys;
}

void MainGame::SetSleepTime( int iVal )
{
	m_iSleepTime = iVal;
}

CDC * MainGame::GetMainDC()
{
	return m_pDC;
}

CDC * MainGame::GetMemDC()
{
	return &(m_MemDC);
}

void MainGame::Prepare()
{
	m_iLiveCount = 3;
	m_iCoinCount = 0;
}

void MainGame::CalcBonusLive()
{
	if (m_iCoinCount >= 100)
	{
		GameSound::Play(BONUS_LIVE_SOUND);
		m_iLiveCount += m_iCoinCount / 100;
		m_iCoinCount = m_iCoinCount % 100;
	}
}