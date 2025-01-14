#pragma once

#include "Functions.h"

#include "TypeAMap.h"

class TypeAMap;

class TypeAObject
{
protected:
	Sprite *m_pSprite;		// sprite nhân vật
	TypeAMap *m_pMapTypeA;	// map mà nhân vật đang ở trong

	int m_iTop;
	int m_iLeft;

	int m_iIsFall;			// biến trạng thái rơi

public:
	TypeAObject(void);
	virtual ~TypeAObject(void);

	// các hàm lấy các giá trị: bottom, right của nhân vật
	virtual int GetBottom();
	int GetRight();
	int GetLeft();
	int GetTop();
	int GetWidth();

	int GetFall();

	Sprite *GetSprite();

	void SetLeft(int iVal);
	void SetTop(int iVal);

	// các hàm lấy các giá trị xử lý đụng độ bên trái, phải, dưới, trên, trong
	int GetLeftCollision(int iScope = cDef::OCEGRINE_MOVE_PIXELS);
	int GetRightCollision(int iScope = cDef::OCEGRINE_MOVE_PIXELS);
	int GetBottomCollision(int iScope = cDef::OCEGRINE_MOVE_PIXELS);
	int GetTopCollision(int iScope = cDef::OCEGRINE_MOVE_PIXELS);
	int GetInnerCollision();

	// hàm vẽ đối tượng tại khung hình hiện tại
	virtual void RenderSprite(CDC *pDC);

	int OnFall();

	int IsInSight();

	int GetCurrScreenX();

	friend class TypeAMap;
};
