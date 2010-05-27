#pragma once

#include "stdafx.h"
#include "GameState.h"
#include "MyBitmap.h"
#include "cDef.h"
#include "0812223-0812239.h"
#include "Sprite.h"
#include "GameKeys.h"
#include "MainGame.h"
#include "TypeAMap.h"
#include "Functions.h"

#define JUMP_STEP		90 / MOVE_PIXELS

class TypeAMap;
class TypeAObject;
class GameStateTypeA;
class Functions;

class TypeAObject
{
public:
	TypeAObject(void);
	TypeAObject(Sprite *sprImage, TypeAMap *pCurrMap);
	~TypeAObject(void);

	Sprite *m_pSprite;

	TypeAMap *m_pMapTypeA;

	int m_iTop;				// trên
	int m_iLeft;			// trái

	int m_iIsDead;			// cho biết đối tượng đã chết hay còn sống
	int m_iIsJump;			// biến trạng thái nhảy
	int m_iIsFall;			// biến trạng thái rơi

	int GetBottom();
	int GetRight();
	int GetLeft();
	int GetTop();
	int GetJump();
	int GetFall();

	void SetLeft(int iLeft);
	void SetTop(int iTop);
	void SetMap(TypeAMap *pCurrMap);
	void SetFall(int iStatus);

	void SetJump(int iJumpStep);
	void ElapseJump();

	// hàm lấy các kích thước của đối tượng
	int GetWidth();
	int GetHeight();

	// hàm lấy trung tâm của đối tượng
	int GetCenterX();

	int GetLeftCollision(int iScope = MOVE_PIXELS);
	int GetRightCollision(int iScope = MOVE_PIXELS);
	int GetBottomCollision(int iScope = MOVE_PIXELS);
	int GetTopCollision(int iScope = MOVE_PIXELS);
	int GetCenterCollision();

	// Cho biết vị trí đối tượng so với màn hình
	//-1: đối tượng lệch trái so với giữa màn hình
	//0: đứng ngay giữa
	//1: lệch phải
	int GetScreenPosition();

	void MoveX(int iSign);
	void MoveY(int iSign);

	void MoveLeft(int &iCurrScreenX);
	void MoveRight(int &iCurrScreenX);
	void Jump();
	int OnAirHandle();
};
