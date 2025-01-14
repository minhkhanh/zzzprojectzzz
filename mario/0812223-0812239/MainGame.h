#pragma once

#include "Functions.h"

class GameState;

#define MAX_LIVES	3

class MainGame
{
private:
	CDC *m_pDC;		// con trỏ thiết bị vẽ hình
	int m_iKeys;		// lưu trữ key buffer

	int m_iLiveCount;	// số mạng
	int m_iCoinCount;	// số tiền

	CDC m_MemDC;		// memory DC
	CBitmap m_MemBitmap, *m_pOldBitmap;

	vector<GameState*> m_arrGameStates;

	int m_iSleepTime;
	bool m_iPlaying;
	int mBiz_iCurrentState;

	bool mBiz_bExitCurrentState;
	bool mBiz_bEnterNextState;

	MyBitmap *m_bmOcegrineIcon;
	MyBitmap *m_bmCoinIcon;

public:
	Graphics * m_pGraphics;
	Graphics * m_pMemGraphics;
	Image * m_pImageGraphics;
	ULONG_PTR m_gdiplusToken;

	MainGame(void);
	~MainGame(void);

	void Prepare();

	// vẽ tình trạng người chơi (số mạng còn lại, tiền xu kiếm được...)
	void DrawGameStatus();

	void SetSleepTime(int iVal);
	CDC *GetMainDC();
	CDC *GetMemDC();

	// lấy trạng thái hiện hành của game
	GameState *GetGameState(int _gsId);

	// chuyển đổi trạng thái
	void mBiz_SwitchState(int stateID);

	// gửi xử lý đến trạng thái tương ứng
	void mBiz_SendMessage(int message);

	void GameLoop();
	void KeyPressed(int iKey);
	void KeyRelease(int iKey);
	void KeyClear();
	int KeyGet();

	// truyền CDC để MainGame vẽ ra
	void SetPDC(CDC *dc);

	void CalcBonusLive();

	friend class GameStateFinish;
	friend class GameStateOver;
	friend class GameStateSettings;
	friend class GameStateHowToPlay;
	friend class GameStateTypeA;
	friend class GameStateIntro;
	friend class GameStateLogo;
	friend class GameStateMenu;
	friend class GameState;
};