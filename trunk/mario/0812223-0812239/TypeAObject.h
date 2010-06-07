#pragma once

#include "Functions.h"

#include "TypeAMap.h"

class Sprite;
class TypeAMap;
class TypeAObject;

class TypeAObject
{
public:
	TypeAObject(void);
	virtual ~TypeAObject(void);

protected:
	Sprite *m_pSprite;		// sprite nhân vật
	TypeAMap *m_pMapTypeA;	// map mà nhân vật đang ở trong

	int m_iTop;				// trên
	int m_iLeft;			// trái

	int m_iIsJump;			// biến đếm nhảy
	int m_iIsFall;			// biến trạng thái rơi
	int m_iIsCrouch;		// đang thu người lại
};
