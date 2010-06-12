#include "StdAfx.h"
#include "GameStateTypeA.h"

#include "TypeAMap.h"
#include "MainGame.h"
#include "TypeAOcegrine.h"

GameStateTypeA::GameStateTypeA(MainGame *_mg): m_iCurrMap(0)
{
	m_pMainGame = _mg;
	m_iGameStateDef = cDef::STATE_TYPE_A;

	m_pTiles = new Sprite(0,0,24,24,1,31,0,new MyBitmap((LPCTSTR)IDIMG_TILES));	// load các tile

	// khởi tạo tất cả các map Type A
	m_vMap.push_back(new TypeAMap("Resource\\Level1\\Map1.dat", IDIMG_BGR_TYPE_A_LEVEL_1, this));
	m_vMap.push_back(new TypeAMap("Resource\\Level2\\Map1.dat", IDIMG_BGR_TYPE_A_LEVEL_2, this));
	m_vMap.push_back(new TypeAMap("Resource\\Level3\\Map1.dat", IDIMG_BGR_TYPE_A_LEVEL_3, this));
	m_vMap.push_back(new TypeAMap("Resource\\Level4\\Map1.dat", IDIMG_BGR_TYPE_A_LEVEL_4, this));
	m_vMap.push_back(new TypeAMap("Resource\\Level5\\Map1.dat", IDIMG_BGR_TYPE_A_LEVEL_5, this));
	//---

	// tạo nhân vật nam chính
	m_pOcegrine = new TypeAOcegrine(new Sprite(0,0,18,36,1,14,1,new MyBitmap((LPCTSTR)IDIMG_OCEGRINE_SPRITE)), m_vMap[m_iCurrMap]);
}

GameStateTypeA::~GameStateTypeA(void)
{
}

void GameStateTypeA::MessageEnter(int message)
{
	GameSound::TypeATheme(1);

	m_pMainGame->SetSleepTime(TYPE_A_SLEEP_TIME);
	PrepareMap(m_iCurrMap);
}

void GameStateTypeA::MessageUpdate(int message, int keys)
{
	if (m_pMainGame->KeyGet() & GameKeys::Left)
		m_pOcegrine->MoveLeft();
	
	if (m_pMainGame->KeyGet() & GameKeys::Right)
		m_pOcegrine->MoveRight();

	if (m_pOcegrine->OnAirHandle(m_pMainGame->m_iCoinCount) == 0)		// chỉ xử lý nhảy khi không đang nhảy và không đang rơi
	{
		if (m_pMainGame->KeyGet() & GameKeys::Up)
		{
			GameSound::Jump(1);
			m_pOcegrine->Jump();
		}
	}

	if (m_pMainGame->KeyGet() & GameKeys::Down)
		m_pOcegrine->CrouchToggle();

	if (m_pMainGame->KeyGet() & GameKeys::Enter)
		m_pMainGame->mBiz_SwitchState(cDef::STATE_MENU);

	int iMap;

	m_vMap[m_iCurrMap]->ActivateMonsters();

	if (m_vMap[m_iCurrMap]->MoveMonsters() == 1)
	{
		--m_pMainGame->m_iLiveCount;
		iMap = m_iCurrMap;
	}
	else
		iMap = m_pOcegrine->InnerCollisionHandle(m_pMainGame->m_iLiveCount, m_pMainGame->m_iCoinCount);

	if (iMap >= 0)
		PrepareMap(iMap);
}

void GameStateTypeA::MessagePaint(int message, CDC *pDC)
{
	m_vMap[m_iCurrMap]->PaintMap(&(m_pMainGame->m_MemDC));

	m_vMap[m_iCurrMap]->ThrowMonsters(&(m_pMainGame->m_MemDC));
	m_pOcegrine->RenderSprite(&(m_pMainGame->m_MemDC));

	m_pMainGame->DrawGameStatus();

	pDC->StretchBlt(0, 0, cDef::SCREEN_WIDTH, cDef::SCREEN_HEIGHT, &(m_pMainGame->m_MemDC), 0, 0 , cDef::SCREEN_WIDTH, cDef::SCREEN_HEIGHT, SRCCOPY);
}

void GameStateTypeA::MessageExit(int message, CDC *pDC)
{
	GameSound::TypeATheme(0);

	m_pMainGame->mBiz_iCurrentState = cDef::STATE_LOGO;
}

void GameStateTypeA::PrepareMap(int iMap)
{
	if (iMap >= MAX_TYPEA_MAP_NUM)			// chỉ số map vượt quá số lượng map cho phép
		return;

	if (iMap == m_iCurrMap && m_pMainGame->m_iLiveCount < MAX_LIVES)
		DumpOcegrine();

	m_vMap[m_iCurrMap]->ReleaseData();
	m_iCurrMap = iMap;					// xác định map hiện tại
	m_vMap[m_iCurrMap]->LoadMap();		// đọc thông tin map từ file, load ảnh nền, load quái, tính các số liệu liên quan

	m_pOcegrine->Prepare(m_vMap[m_iCurrMap]);
}

int GameStateTypeA::GetCurrMapIndex()
{
	return m_iCurrMap;
}

TypeAOcegrine *GameStateTypeA::GetOcegrine()
{
	return m_pOcegrine;
}

Sprite * GameStateTypeA::GetTilesSprite()
{
	return m_pTiles;
}

void GameStateTypeA::DumpOcegrine()
{
	Sprite *pOceSprite =  m_pOcegrine->GetSprite();

	int arrStep[] = {8,8,4,4,2,2,1,1,-1,-1,-2,-2,-4,-4,-8,-8};

	for (int i = 0; i < 16; ++i)
	{
		m_pOcegrine->SetTop(m_pOcegrine->GetTop()-arrStep[i]);

		m_pOcegrine->RenderSprite(m_pMainGame->m_pDC);
	}
}