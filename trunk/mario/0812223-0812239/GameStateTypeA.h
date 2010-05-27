#pragma once

#include "stdafx.h"
#include "GameState.h"
#include "MyBitmap.h"
#include "cDef.h"
#include "0812223-0812239.h"
#include "Sprite.h"
#include "GameKeys.h"
#include "MainGame.h"
#include "TypeAObject.h"
#include "TypeAMap.h"

#define MAX_MAP_NUM			5
#define TYPE_A_SLEEP_TIME	30

class GameStateTypeA;
class TypeAObject;
class TypeAMap;

class GameStateTypeA :public GameState
{
public:
	GameStateTypeA(MainGame *_mg);
	virtual ~GameStateTypeA(void);

	TypeAObject *m_pOcegrine;

	Sprite *sprTiles;

	vector<TypeAMap*> m_vMap;
	int m_iCurrMap;

	int m_iCurrScreenX;		// cho biết vị trí bắt đầu của khung map hiện tại

	void MessageEnter(int message);
	void MessageUpdate(int message, int keys);
	void MessagePaint(int message, CDC *pDC);
	void MessageExit(int message, CDC *pDC);

	// load màn mới khi qua màn hoặc load lại màn cũ khi nhân vật chính tử nạn
	void PrepareLevel(int iMap);
	// xử lý cho việc đi tới
	void OnKeyRight();
	// xử lý cho việc đi ngược về
	void OnKeyLeft();
	// xử lý khi nhảy hoặc khi rơi
	int OverheadHandler();
	// xử lý cho load map
	void ReloadHandler();
	// tăng giảm biến số khi di chuyển
	void Move(int &iVal, int sign);

	// hàm lấy tọa độ X của điểm giữa màn hình game
	int GetScreenCenterX();
	// hàm cho biết khung màn hình đang ở đâu so với map
	int GetMapPosition();
	// hàm lấy giá trị biến m_iCurrScreenX
	int GetCurrScreenX();
};
