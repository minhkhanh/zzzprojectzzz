#include "StdAfx.h"
#include "GameStateTypeA.h"

GameStateTypeA::GameStateTypeA(MainGame *_mg)
{
	mg = _mg;
	GAME_STATE_DEF = cDef::STATE_TYPE_A;

	sprTiles = new Sprite(0,0,24,24,1,31,0,new MyBitmap((LPCTSTR)IDIMG_TILES));	// load các tile
	m_iCurrMap = 0;
	m_iCurrScreenX = 0;

	// khởi tạo tất cả các map Type A
	m_vMap.push_back(new TypeAMap("Resource\\Level1\\Map1.dat", IDIMG_BGR_TYPE_A_LEVEL_1, this));
	m_vMap.push_back(new TypeAMap("Resource\\Level2\\Map1.dat", IDIMG_BGR_TYPE_A_LEVEL_2, this));
	m_vMap.push_back(new TypeAMap("Resource\\Level3\\Map1.dat", IDIMG_BGR_TYPE_A_LEVEL_3, this));
	m_vMap.push_back(new TypeAMap("Resource\\Level4\\Map1.dat", IDIMG_BGR_TYPE_A_LEVEL_4, this));
	m_vMap.push_back(new TypeAMap("Resource\\Level5\\Map1.dat", IDIMG_BGR_TYPE_A_LEVEL_5, this));
	//---

	// load nhân vật nam chính
	m_pOcegrine = new TypeAObject(new Sprite(0,0,18,36,1,7,0,new MyBitmap((LPCTSTR)IDIMG_OCEGRINE_SPRITE)), m_vMap[m_iCurrMap]);
}

GameStateTypeA::~GameStateTypeA(void)
{
}

void GameStateTypeA::MessageEnter(int message)
{
	mg->iSleepTime = TYPE_A_SLEEP_TIME;
	PrepareLevel(m_iCurrMap);
}

void GameStateTypeA::MessageUpdate(int message, int keys)
{
	if (keys & GameKeys::Left)
	{
		m_pOcegrine->MoveLeft(m_iCurrScreenX);
	}
	
	if (keys & GameKeys::Right)
	{
		m_pOcegrine->MoveRight(m_iCurrScreenX);
	}

	if (m_pOcegrine->OnAirHandle() == 0)		// chỉ xử lý nhảy khi không đang nhảy và không đang rơi
	{
		if (keys & GameKeys::Up)
			m_pOcegrine->Jump();		
	}

	ReloadHandler();		// xử lý xem có cần load map ko

	if ((keys & GameKeys::Enter) == GameKeys::Enter)
	{
		mg->mBiz_SwitchState(cDef::STATE_MENU);
	}
}

void GameStateTypeA::OnKeyRight()
{
	m_pOcegrine->m_pSprite->NextFrame();

	if (m_pOcegrine->GetScreenPosition() != 0)	// khi nhân vật không nằm giữa màn hình, di chuyển nhân vật
	{
		if (!(m_pOcegrine->GetRightCollision() & (HARD_TILE | RIGHT_SIDE_TILE)))
			m_pOcegrine->MoveX(1);
	}
	else										// nhân vật đang nằm giữa màn hình
	{
		if (GetMapPosition() <= 0)			// di chuyển màn hình sang phải
		{
			if (!(m_pOcegrine->GetRightCollision() & HARD_TILE))
				Move(m_iCurrScreenX, 1);
		}
		else									// di chuyển nhân vật
		{
			if (!(m_pOcegrine->GetRightCollision() & HARD_TILE))
				m_pOcegrine->MoveX(1);
		}
	}

	if (m_pOcegrine->GetJump() == 0 && m_pOcegrine->GetFall() == 0)
	{
		if (!(m_pOcegrine->GetBottomCollision() & HARD_TILE))
			m_pOcegrine->SetFall(1);
	}
}

void GameStateTypeA::OnKeyLeft()
{
	m_pOcegrine->m_pSprite->PreviousFrame();

	if (m_pOcegrine->GetScreenPosition() != 0)	// đi qua trái khi nhân vật không nằm giữa màn hình
	{
		if (!(m_pOcegrine->GetLeftCollision() & (HARD_TILE | LEFT_SIDE_TILE)))
			m_pOcegrine->MoveX(-1);
	}
	else										// nhân vật đang nằm giữa màn hình
	{
		if (GetMapPosition() >= 0)
		{
			if (!(m_pOcegrine->GetLeftCollision() & HARD_TILE))
				Move(m_iCurrScreenX, -1);
		}
		else
		{
			if (!(m_pOcegrine->GetLeftCollision() & HARD_TILE))
				m_pOcegrine->MoveX(-1);
		}
	}

	if (m_pOcegrine->GetJump() == 0 && m_pOcegrine->GetFall() == 0)
	{// khi không đang nhảy lên và không đang rơi xuống
		if (!(m_pOcegrine->GetBottomCollision() & HARD_TILE))	// nếu ở ngay dưới không có va chạm
			m_pOcegrine->SetFall(1);						//thì bắt đầu rơi
	}
}

int GameStateTypeA::OverheadHandler()
{
	if (m_pOcegrine->GetJump() > 0)		// đang nhảy
	{
		if ((m_pOcegrine->GetTopCollision() & HARD_TILE))	// đụng nóc
		{
			m_pOcegrine->MoveY(1);					// lưu ý rớt xuống là tăng Y
			m_pOcegrine->m_iIsJump = 0;				// chuyển sang giai đoạn rơi
		}
		else
			m_pOcegrine->MoveY(-1);

		if (m_pOcegrine->GetJump() > 0)
			m_pOcegrine->ElapseJump();			// giai đoạn nhảy trôi dần qua

		if (m_pOcegrine->GetJump() == 0)			// kết thúc giai đoạn nhảy, chuyển sang giai đoạn rơi
			m_pOcegrine->SetFall(1);

		return 1;
	}
	else
	{
		if (m_pOcegrine->GetFall() != 0)		// đang rơi
		{
			if (!(m_pOcegrine->GetBottomCollision() & HARD_TILE))		// chưa chạm đất thì cứ rơi
				m_pOcegrine->MoveY(1);
			else
				m_pOcegrine->SetFall(0);		// rơi xuống đất cứng thì dừng rơi

			return 1;
		}
	}

	return 0;			// ko đang nhảy và ko đang rơi
}

void GameStateTypeA::ReloadHandler()
{
	int iTiles = m_pOcegrine->GetCenterCollision();

	if (iTiles & HELL_TILE)		// nhân vật nam chính chết giữa đường, chơi lại map
	{
		--mg->m_iLiveCount;
		PrepareLevel(m_iCurrMap);
	}
	else if (iTiles & GATE_TILE)	// chạm cổng qua màn, load màn kế tiếp
	{
		PrepareLevel(m_iCurrMap+1);
	}
	else if (iTiles & DANGEROUS_TILE)
	{
		PrepareLevel(m_iCurrMap);		// chạm phải chướng ngại nguy hiểm chết người, đi lại từ đầu
	}

	// sắp xếp câu lệnh if như trên là theo thứ tự ưu tiên: rớt xuống vực, chết > chạm cửa, qua màn > chạm quái, chết
}

void GameStateTypeA::MessagePaint(int message, CDC *pDC)
{
	m_vMap[m_iCurrMap]->PaintMap();
	mg->DrawGameStatus();
	m_pOcegrine->m_pSprite->Render(m_pOcegrine->GetLeft(), m_pOcegrine->GetTop(), &(mg->m_MemDC));

	pDC->StretchBlt(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, &(mg->m_MemDC), 0, 0 , SCREEN_WIDTH, SCREEN_HEIGHT, SRCCOPY);
}

void GameStateTypeA::MessageExit(int message, CDC *pDC)
{
	mg->mBiz_iCurrentState = cDef::STATE_LOGO;
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

	m_pOcegrine->SetMap(m_vMap[m_iCurrMap]);
	m_pOcegrine->SetLeft(GetScreenCenterX() - m_pOcegrine->GetWidth()/2);
	m_pOcegrine->SetTop(m_vMap[m_iCurrMap]->CalcY(m_pOcegrine->GetLeft(), 0) - m_pOcegrine->GetHeight());
	m_pOcegrine->SetFall(0);
	m_pOcegrine->SetJump(0);
}

void GameStateTypeA::Move(int &iVal, int sign)
{
	iVal += sign*MOVE_PIXELS;
}

int GameStateTypeA::GetMapPosition()
{
	if (m_iCurrScreenX == 0)
		return -1;
	
	if (m_iCurrScreenX + SCREEN_WIDTH >= m_vMap[m_iCurrMap]->GetMapWidth())
		return 1;

	return 0;
}

int GameStateTypeA::GetCurrScreenX()
{
	return m_iCurrScreenX;
}