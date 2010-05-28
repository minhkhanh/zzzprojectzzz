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

	// tạo nhân vật nam chính
	m_pOcegrine = new TypeAOcegrine(new Sprite(0,0,18,36,1,14,0,new MyBitmap((LPCTSTR)IDIMG_OCEGRINE_SPRITE)), m_vMap[m_iCurrMap]);
}

GameStateTypeA::~GameStateTypeA(void)
{
}

void GameStateTypeA::MessageEnter(int message)
{
	mg->iSleepTime = TYPE_A_SLEEP_TIME;
	PrepareMap(m_iCurrMap);
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

	if (m_pOcegrine->OnAirHandle(mg->m_iCoinCount) == 0)		// chỉ xử lý nhảy khi không đang nhảy và không đang rơi
	{
		if (keys & GameKeys::Up)
			m_pOcegrine->Jump();		
	}

	int iMap = m_pOcegrine->InnerCollisionHandle(mg->m_iLiveCount, mg->m_iCoinCount, m_iCurrMap);
	if (iMap >= 0)
		PrepareMap(iMap);

	if ((keys & GameKeys::Enter) == GameKeys::Enter)
	{
		mg->mBiz_SwitchState(cDef::STATE_MENU);
	}
}

void GameStateTypeA::MessagePaint(int message, CDC *pDC)
{
	m_vMap[m_iCurrMap]->PaintMap();
	mg->DrawGameStatus();
	m_pOcegrine->RenderSprite(&(mg->m_MemDC));

	pDC->StretchBlt(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, &(mg->m_MemDC), 0, 0 , SCREEN_WIDTH, SCREEN_HEIGHT, SRCCOPY);
}

void GameStateTypeA::MessageExit(int message, CDC *pDC)
{
	mg->mBiz_iCurrentState = cDef::STATE_LOGO;
}

void GameStateTypeA::PrepareMap(int iMap)
{
	if (iMap >= MAX_MAP_NUM)			// chỉ số map vượt quá số lượng map cho phép
		return;

	m_iCurrMap = iMap;					// xác định map hiện tại
	m_vMap[m_iCurrMap]->LoadMap();		// đọc thông tin map từ file, load ảnh nền, tính các số liệu liên quan
	m_iCurrScreenX = 0;					// trả màn hình về vị trí bắt đầu

	m_pOcegrine->Prepare(m_vMap[m_iCurrMap]);
}

int GameStateTypeA::GetOnMapPosition()
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