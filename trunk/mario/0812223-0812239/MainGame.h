#pragma once

#ifndef _MainGame_
#define _MainGame

#include "stdafx.h"
#include "GameState.h"
#include "GameKeys.h"
#include "GameStateLogo.h"
#include "GameStateTypeA.h"
#include "GameStateMenu.h"
#include "TypeAObject.h"

class MainGame;
class GameState;
class TypeAObject;

class MainGame
{
public:
	MainGame(void);
	~MainGame(void);

public:
	CDC *pDC;		// con trỏ thiết bị vẽ hình
	int m_keys;		// lưu trữ key buffer

	int m_iLiveCount;	// số mạng
	int m_iCoinCount;	// số tiền

	CDC m_MemDC;	// memory DC
	CBitmap m_MemBitmap, *m_pOldBitmap;

	vector<GameState*> m_arrGameStates;

	int iSleepTime;
	bool m_playing;
	int mBiz_iCurrentState;

	bool mBiz_bExitCurrentState;
	bool mBiz_bEnterNextState;

	MyBitmap *m_bmOcegrineIcon;
	MyBitmap *m_bmCoinIcon;

	// vẽ tình trạng người chơi (số mạng còn lại, tiền xu kiếm được...)
	void DrawGameStatus();

	// lấy trạng thái hiện hành của game
	GameState *GetGameState(int _gsId);

	// chuyển đổi trạng thái
	void mBiz_SwitchState(int stateID);

	// gửi xử lý đến trạng thái tương ứng
	void mBiz_SendMessage(int message);

	void GameLoop();
	void KeyPressed(int iKey);
	void KeyRelease(int iKey);

	// truyền CDC để MainGame vẽ ra
	void SetPDC(CDC *dc);
};

#endif