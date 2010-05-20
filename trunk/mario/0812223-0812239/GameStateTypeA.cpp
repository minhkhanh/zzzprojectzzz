#include "StdAfx.h"
#include "GameStateTypeA.h"

GameStateTypeA::GameStateTypeA(MainGame *_mg)
{
	mg = _mg;
	GAME_STATE_DEF = cDef::STATE_TYPE_A;

	sprTiles = new Sprite(0,0,24,24,1,31,0,new MyBitmap((LPCTSTR)IDIMG_TILES));	// load các tile
	m_iCurrMap = 0;
	m_iCurrScreenX = 0;

	// load nhân vật nam chính
	m_pOcegrine = new TypeAObject(new Sprite(0,0,18,36,1,7,0,new MyBitmap((LPCTSTR)IDIMG_OCEGRINE_SPRITE)));

	// khởi tạo tất cả các map Type A
	m_vMap.push_back(new TypeAMap("Resource\\Level1\\Map1.dat", IDIMG_BGR_TYPE_A_LEVEL_1, this));
	m_vMap.push_back(new TypeAMap("Resource\\Level2\\Map1.dat", IDIMG_BGR_TYPE_A_LEVEL_2, this));
	m_vMap.push_back(new TypeAMap("Resource\\Level3\\Map1.dat", IDIMG_BGR_TYPE_A_LEVEL_3, this));
	m_vMap.push_back(new TypeAMap("Resource\\Level4\\Map1.dat", IDIMG_BGR_TYPE_A_LEVEL_4, this));
	m_vMap.push_back(new TypeAMap("Resource\\Level5\\Map1.dat", IDIMG_BGR_TYPE_A_LEVEL_5, this));
	//---

	// khởi tạo các hằng số
	MAX_MAP_NUM = 5;
	MOVE_PIXELS = 10;
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
		OnKeyLeft();
	}
	
	if ((keys & GameKeys::Right) == GameKeys::Right)
	{
		OnKeyRight();
	}

	if (OverheadHandler() == 0)
	{
		if ((keys & GameKeys::Up))	// chỉ xử lý nhảy khi không đang nhảy và không đang rơi
		{
			m_pOcegrine->m_iIsJump = 10;
			if (!(GetTopCollision(m_pOcegrine) & HARD_TILE))
			{
				Move(m_pOcegrine->m_iTop, -1);
			}
			else
				m_pOcegrine->m_iIsJump = 0;			// nhảy lên đụng nóc ngay tức khắc, khỏi nhảy nữa
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
	m_vMap[m_iCurrMap]->PaintMap();
	mg->DrawGameStatus();
	m_pOcegrine->m_pSprite->Render(m_pOcegrine->m_iLeft, m_pOcegrine->m_iTop, &(mg->m_MemDC));

	pDC->StretchBlt(0, 0, 380, 480, &(mg->m_MemDC), 0, 0 , 380, 480, SRCCOPY);
}

void GameStateTypeA::MessageEnter(int message)
{
	mg->iSleepTime = 30;
	PrepareLevel(m_iCurrMap);
}

int GameStateTypeA::GetScreenCenterX()
{
	return SCREEN_WIDTH / 2;
}

void GameStateTypeA::PrepareLevel(int iMap)
{
	if (iMap == MAX_MAP_NUM)
		return;

	m_iCurrMap = iMap;
	m_vMap[m_iCurrMap]->LoadMap();

	m_iCurrScreenX = 0;
	m_pOcegrine->m_iLeft = GetScreenCenterX() - m_pOcegrine->GetWidth()/2;
	m_pOcegrine->m_iTop = m_vMap[m_iCurrMap]->CalcY(m_pOcegrine->m_iLeft, 0) - m_pOcegrine->GetHeight();
}

void GameStateTypeA::OnKeyRight()
{
	m_pOcegrine->m_pSprite->NextFrame();

	if (GetOcegrinePositionStatus() < 0)
	{
		if (!(GetRightCollision(m_pOcegrine) & HARD_TILE))
			Move(m_pOcegrine->m_iLeft, 1);
	}
	else
	{
		Move(m_iCurrScreenX, 1);
		if (m_iCurrScreenX + SCREEN_WIDTH > m_vMap[m_iCurrMap]->m_iPixelMapWidth)		//!!!
		{
			m_iCurrScreenX = m_vMap[m_iCurrMap]->m_iPixelMapWidth - SCREEN_WIDTH - 1;

			if (!(GetRightCollision(m_pOcegrine) & HARD_TILE)
				&& !(GetRightCollision(m_pOcegrine) & RIGHT_SIDE_TILE))
				Move(m_pOcegrine->m_iLeft, 1);
		}
		else
		{
			if (GetRightCollision(m_pOcegrine) & HARD_TILE)
				Move(m_iCurrScreenX, -1);

		}
	}

	if (m_pOcegrine->m_iIsJump == 0 && m_pOcegrine->m_iIsFall == 0)
	{
		if (!(GetBottomCollision(m_pOcegrine) & HARD_TILE))
			m_pOcegrine->m_iIsFall = 1;
	}
}

void GameStateTypeA::OnKeyLeft()
{
	m_pOcegrine->m_pSprite->PreviousFrame();

	if (GetOcegrinePositionStatus() > 0)		// nhân vật đang đứng lệch về bên phải so với trung tâm màn hình
	{
		if (!(GetLeftCollision(m_pOcegrine) & HARD_TILE))
			Move(m_pOcegrine->m_iLeft, -1);
	}
	else
	{
		Move(m_iCurrScreenX, -1);
		if (m_iCurrScreenX < 0)
		{
			m_iCurrScreenX = 0;
			if (!(GetLeftCollision(m_pOcegrine) & HARD_TILE)
				&& !(GetLeftCollision(m_pOcegrine) & LEFT_SIDE_TILE))
				Move(m_pOcegrine->m_iLeft, -1);
		}
		else
		{
			if (GetLeftCollision(m_pOcegrine) & HARD_TILE)
				Move(m_iCurrScreenX, 1);
		}
	}

	if (m_pOcegrine->m_iIsJump == 0 && m_pOcegrine->m_iIsFall == 0)
	{// khi không đang nhảy lên và không đang rơi xuống
		if ((GetBottomCollision(m_pOcegrine) & HARD_TILE) != 1)	// nếu ở ngay dưới không có va chạm
			m_pOcegrine->m_iIsFall = 1;						//thì bắt đầu rơi
	}
}

int GameStateTypeA::OverheadHandler()
{
	if (m_pOcegrine->m_iIsJump > 0)
	{
		if (!(GetTopCollision(m_pOcegrine) & HARD_TILE))
		{
			Move(m_pOcegrine->m_iTop, -1);
			//Move(m_pOcegrine->m_iTop, 2);		
		}
		else
			m_pOcegrine->m_iIsJump = 0;		// nhảy lên đụng nóc thì chuyển sang giai đoạn rơi

		if (m_pOcegrine->m_iIsJump > 0)
			--m_pOcegrine->m_iIsJump;			// giai đoạn nhảy trôi dần qua

		if (m_pOcegrine->m_iIsJump == 0)			// kết thúc giai đoạn nhảy, chuyển sang giai đoạn rơi
			m_pOcegrine->m_iIsFall = 1;

		return 1;
	}
	else
	{
		if (m_pOcegrine->m_iIsFall != 0)
		{
			if (!(GetBottomCollision(m_pOcegrine) & HARD_TILE))
				Move(m_pOcegrine->m_iTop, 1);			
			else
				m_pOcegrine->m_iIsFall = 0;		// rơi xuống đất cứng thì dừng rơi

			return 1;
		}
	}

	return 0;			// ko đang nhảy và ko đang rơi
}

void GameStateTypeA::ReloadHandler()
{
	int iTiles = GetCenterCollision(m_pOcegrine);
	if (iTiles & HELL_TILE)
	{
		--mg->m_iLiveCount;
		PrepareLevel(m_iCurrMap);
	}
	else if (iTiles & GATE_TILE)
	{
		PrepareLevel(m_iCurrMap+1);		// chạm cổng qua màn, load màn kế tiếp
	}
	else if (iTiles & DANGEROUS_TILE)
	{
		PrepareLevel(m_iCurrMap);		// chạm phải chướng ngại nguy hiểm chết người, đi lại từ đầu
	}
}

int GameStateTypeA::GetLeftCollision(TypeAObject *pObject)
{
	int iTiles = 0x0;
	for (int i = pObject->m_iTop; i <= pObject->GetBottom(); i += BASE_HEIGHT)
	{
		iTiles |= m_vMap[m_iCurrMap]->WhatTile(pObject->m_iLeft-1, i);
	}

	return iTiles;
}

int GameStateTypeA::GetRightCollision(TypeAObject *pObject)
{
	int iTiles = 0x0;
	for (int i = pObject->m_iTop; i <= pObject->GetBottom(); i += BASE_HEIGHT)
	{
		iTiles |= m_vMap[m_iCurrMap]->WhatTile(pObject->GetRight()+1, i);
	}

	return iTiles;
}

int GameStateTypeA::GetBottomCollision(TypeAObject *pObject)
{
	int iTiles = 0x0;
	for (int i = pObject->m_iLeft; i <= pObject->GetRight(); i += BASE_WIDTH)
	{
		iTiles |= m_vMap[m_iCurrMap]->WhatTile(i, pObject->GetBottom()+1);
	}

	return iTiles;
}

int GameStateTypeA::GetTopCollision(TypeAObject *pObject)
{
	int iTiles = 0x0;
	for (int i = pObject->m_iLeft; i <= pObject->GetRight(); i += BASE_WIDTH)
	{
		iTiles |= m_vMap[m_iCurrMap]->WhatTile(i, pObject->m_iTop-1);
	}

	return iTiles;
}

int GameStateTypeA::GetCenterCollision(TypeAObject *pObject)
{
	int iTiles = 0x0;
	for (int i = pObject->m_iTop; i <= pObject->GetBottom(); i += BASE_HEIGHT)
		for (int j = pObject->m_iLeft; j <= pObject->GetRight(); j += BASE_WIDTH)
		{
			iTiles |= m_vMap[m_iCurrMap]->WhatTile(j+m_iCurrScreenX, i);
		}

	return iTiles;
}

int GameStateTypeA::GetOcegrinePositionStatus()
{
	if (m_pOcegrine->m_iLeft == 0)
		return -2;
	if (m_pOcegrine->GetRight() == SCREEN_WIDTH)
		return 2;

	int iVal = m_pOcegrine->GetCenterX() - GetScreenCenterX();
	if (iVal < 0)
		return -1;
	else if (iVal > 0)
		return 1;
	else
		return 0;
}

int GameStateTypeA::GetScreenPositionStatus()
{
	if (m_iCurrScreenX == 0)
		return -1;
	else if (m_iCurrScreenX+SCREEN_WIDTH == m_vMap[m_iCurrMap]->m_iPixelMapWidth-1)
		return 1;
	else
		return 0;
}

void GameStateTypeA::Move(int &iVal, int sign)
{
	iVal += sign*MOVE_PIXELS;
}