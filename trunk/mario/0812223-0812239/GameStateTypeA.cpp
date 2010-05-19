#include "StdAfx.h"
#include "GameStateTypeA.h"

GameStateTypeA::GameStateTypeA(MainGame *_mg): MAX_MAP_NUM(5)
{
	mg = _mg;
	GAME_STATE_DEF = cDef::STATE_TYPE_A;

	// khởi tạo tất cả các map Type A
	m_vMapInfo.push_back(new TypeAMapInfo("Resource\\Level1\\Map1.dat", IDIMG_BGR_TYPE_A_LEVEL_1, this));
	m_vMapInfo.push_back(new TypeAMapInfo("Resource\\Level2\\Map1.dat", IDIMG_BGR_TYPE_A_LEVEL_2, this));
	m_vMapInfo.push_back(new TypeAMapInfo("Resource\\Level3\\Map1.dat", IDIMG_BGR_TYPE_A_LEVEL_3, this));
	m_vMapInfo.push_back(new TypeAMapInfo("Resource\\Level4\\Map1.dat", IDIMG_BGR_TYPE_A_LEVEL_4, this));
	m_vMapInfo.push_back(new TypeAMapInfo("Resource\\Level5\\Map1.dat", IDIMG_BGR_TYPE_A_LEVEL_5, this));
	//---

	sprTiles = new Sprite(0,0,24,24,1,31,0,new MyBitmap((LPCTSTR)IDIMG_TILES));	// load các tile
}

GameStateTypeA::~GameStateTypeA(void)
{
}

void GameStateTypeA::MessageExit(int message, CDC *pDC)
{
	mg->mBiz_iCurrentState = cDef::STATE_LOGO;
}

void GameStateTypeA::MessageUpdate(int message, int keys)
{
	if ((keys & GameKeys::Left) == GameKeys::Left)
	{
		GoBackHandler();
	}
	
	if ((keys & GameKeys::Right) == GameKeys::Right)
	{
		GoAheadHandler();
	}

	if (OverheadHandler() == 0)
	{
		if ((keys & GameKeys::Up))	// chỉ xử lý nhảy khi không đang nhảy và không đang rơi
		{
			m_iIsJump = 4;
			--m_iOcegrineY;
			if (m_vMapInfo[m_iCurrMap]->WhatTile(m_iOcegrineY, m_iOcegrineX) == HARD_TILE)		// nhảy lên đụng nóc ngay tức khắc
			{
				++m_iOcegrineY;
				m_iIsJump = 0;			// khỏi nhảy nữa
			}
		}
	}

	ReloadHandler();		// xử lý xem có cần load map ko

	if ((keys & GameKeys::Enter) == GameKeys::Enter)
	{
		mg->mBiz_SwitchState(cDef::STATE_MENU);
	}
}

void GameStateTypeA::MessagePaint(int message, CDC *pDC)
{
	m_vMapInfo[m_iCurrMap]->PaintMap();
	mg->DrawGameStatus();
	mg->m_sprOcegrine->Render(m_iOcegrineX*BASE_WIDTH, m_iOcegrineY*BASE_HEIGHT - 12, &(mg->m_MemDC));

	pDC->StretchBlt(0, 0, 380, 480, &(mg->m_MemDC), 0, 0 , 380, 480, SRCCOPY);
}

void GameStateTypeA::MessageEnter(int message)
{
	LoadLevel(0);
}

int GameStateTypeA::ScreenFrameCenterX()
{
	return SCREEN_FRAME_WIDTH / 2;
}

void GameStateTypeA::LoadLevel(int iMap)
{
	if (iMap == MAX_MAP_NUM)
		return;

	m_iCurrMap = iMap;
	m_vMapInfo[m_iCurrMap]->LoadMap();

	m_iCurrScreenFrameX = 0;
	m_iOcegrineX = ScreenFrameCenterX();
	m_iOcegrineY = m_vMapInfo[m_iCurrMap]->CalcY(m_iOcegrineX);
	m_iIsJump = 0;
	m_iIsFall = 0;

}

void GameStateTypeA::GoAheadHandler()
{
	mg->m_sprOcegrine->NextFrame();

	if (m_iOcegrineX < ScreenFrameCenterX())
	{
		++m_iOcegrineX;
		if (m_vMapInfo[m_iCurrMap]->WhatTile(m_iOcegrineY, m_iOcegrineX) == HARD_TILE)
			--m_iOcegrineX;
	}
	else
	{
		++m_iCurrScreenFrameX;
		if (m_iCurrScreenFrameX + SCREEN_FRAME_WIDTH > m_vMapInfo[m_iCurrMap]->m_iMapFrameWidth)
		{
			m_iCurrScreenFrameX = m_vMapInfo[m_iCurrMap]->m_iMapFrameWidth - SCREEN_FRAME_WIDTH - 1;

			++m_iOcegrineX;
			if (m_iOcegrineX > SCREEN_FRAME_WIDTH)
				m_iOcegrineX = SCREEN_FRAME_WIDTH;
			else
			{
				if (m_vMapInfo[m_iCurrMap]->WhatTile(m_iOcegrineY, m_iOcegrineX) == HARD_TILE)
					--m_iOcegrineX;

			}
		}
		else
		{
			if (m_vMapInfo[m_iCurrMap]->WhatTile(m_iOcegrineY, m_iOcegrineX) == HARD_TILE)
			{
				--m_iCurrScreenFrameX;
			}
		}
	}

	if (m_iIsJump == 0 && m_iIsFall == 0 && m_vMapInfo[m_iCurrMap]->WhatTile(m_iOcegrineY+1, m_iOcegrineX) != HARD_TILE)
	{
		int iY = m_vMapInfo[m_iCurrMap]->CalcY(m_iOcegrineX);
		if (iY != m_iOcegrineY)
		{
			m_iIsFall = 1;
		}
	}
}

void GameStateTypeA::GoBackHandler()
{
	mg->m_sprOcegrine->PreviousFrame();

	// xử lý đi về bên trái cho m_iCurrScreenFrameX, ak
	if (m_iOcegrineX >= ScreenFrameCenterX())
	{
		--m_iOcegrineX;
		if (m_vMapInfo[m_iCurrMap]->WhatTile(m_iOcegrineY, m_iOcegrineX) == HARD_TILE)
			++m_iOcegrineX;
	}
	else
	{
		--m_iCurrScreenFrameX;
		if (m_iCurrScreenFrameX < 0)
		{
			m_iCurrScreenFrameX = 0;
			--m_iOcegrineX;
			if (m_iOcegrineX < 0)
				m_iOcegrineX = 0;
			else
			{
				if (m_vMapInfo[m_iCurrMap]->WhatTile(m_iOcegrineY, m_iOcegrineX) == HARD_TILE)
					++m_iOcegrineX;
			}
		}
		else
		{
			if (m_vMapInfo[m_iCurrMap]->WhatTile(m_iOcegrineY, m_iOcegrineX) == HARD_TILE)
				++m_iCurrScreenFrameX;
		}
	}

	if (m_iIsJump == 0 && m_iIsFall == 0 && m_vMapInfo[m_iCurrMap]->WhatTile(m_iOcegrineY+1, m_iOcegrineX) != HARD_TILE)
	{
		int iY = m_vMapInfo[m_iCurrMap]->CalcY(m_iOcegrineX);
		if (iY != m_iOcegrineY)
		{
			m_iIsFall = 1;
		}
	}
}

int GameStateTypeA::OverheadHandler()
{
	if (m_iIsJump > 0)
	{
		--m_iOcegrineY;
		//int iFlag = m_vMapInfo[m_iCurrMap]->WhatTile(m_iOcegrineY, m_iOcegrineX);
		if (m_vMapInfo[m_iCurrMap]->WhatTile(m_iOcegrineY, m_iOcegrineX) == HARD_TILE)
		{
			m_iOcegrineY+=2;		// nhảy lên đụng nóc
			m_iIsJump = 0;		//thì chuyển sang giai đoạn rơi
		}

		if (m_iIsJump > 0)
			--m_iIsJump;			// giai đoạn nhảy trôi dần qua

		if (m_iIsJump == 0)			// kết thúc giai đoạn nhảy, chuyển sang giai đoạn rơi
			m_iIsFall = 1;

		return 1;
	}
	else
	{
		if (m_iIsFall != 0)
		{
			++m_iOcegrineY;
			int iFlag = m_vMapInfo[m_iCurrMap]->WhatTile(m_iOcegrineY, m_iOcegrineX);
			if (iFlag == HARD_TILE)
			{
				--m_iOcegrineY;
				m_iIsFall = 0;
			}
			else if (iFlag == HELL_TILE)
			{
				--mg->m_iLiveCount;
				LoadLevel(m_iCurrMap);	// rớt xuống vực, chơi lại
			}

			return 1;
		}
	}

	return 0;			// ko đang nhảy và ko đang rơi
}

void GameStateTypeA::ReloadHandler()
{
	if (m_vMapInfo[m_iCurrMap]->WhatTile(m_iOcegrineY, m_iOcegrineX) == GATE_TILE)
	{
		LoadLevel(m_iCurrMap+1);		// chạm cổng qua màn, load màn kế tiếp
	}
	else if (m_vMapInfo[m_iCurrMap]->WhatTile(m_iOcegrineY, m_iOcegrineX) == DANGEROUS_TILE)
	{
		LoadLevel(m_iCurrMap);		// chạm phải chướng ngại nguy hiểm, đi lại từ đầu
	}
}
