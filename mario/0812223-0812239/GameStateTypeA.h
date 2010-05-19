#pragma once

#include "stdafx.h"
#include "GameState.h"
#include "MyBitmap.h"
#include "cDef.h"
#include "0812223-0812239.h"
#include "Sprite.h"
#include "GameKeys.h"
#include "MainGame.h"
#include "TypeAMonster.h"
#include "TypeAMapInfo.h"

class GameStateTypeA;
class TypeAMonster;
class TypeAMapInfo;

class GameStateTypeA :public GameState
{
public:
	GameStateTypeA(MainGame *_mg);
	virtual ~GameStateTypeA(void);

	const int MAX_MAP_NUM;

	Sprite *sprTiles;

	vector<TypeAMapInfo*> m_vMapInfo;
	int m_iCurrMap;

	int m_iCurrScreenFrameX;		// cho biết vị trí bắt đầu của khung map hiện tại
	int m_iOcegrineX, m_iOcegrineY;	// tọa độ nhân vật chính trên khung màn hình
	int m_iIsJump;					// biến trạng thái nhảy
	int m_iIsFall;					// biến trạng thái rơi

	void MessageEnter(int message);
	void MessageUpdate(int message, int keys);
	void MessagePaint(int message, CDC *pDC);
	void MessageExit(int message, CDC *pDC);

	// load màn mới khi qua màn hoặc load lại màn cũ khi nhân vật chính tử nạn
	void LoadLevel(int iMap);
	// xử lý cho việc đi tới
	void GoAheadHandler();
	// xử lý cho việc đi ngược về
	void GoBackHandler();
	// xử lý khi nhảy hoặc khi rơi
	int OverheadHandler();
	// xử lý cho load map
	void ReloadHandler();

	// hàm lấy tọa độ X của điểm giữa màn hình game
	int ScreenFrameCenterX();
};
