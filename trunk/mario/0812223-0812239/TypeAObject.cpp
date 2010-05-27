#include "StdAfx.h"
#include "TypeAObject.h"

TypeAObject::TypeAObject(void)
{
}

TypeAObject::TypeAObject(Sprite *sprImage, TypeAMap *pCurrMap)
{
	m_pSprite = sprImage;

	m_iIsDead = m_iIsJump = m_iIsFall = 0;

	m_pMapTypeA = pCurrMap;
}

TypeAObject::~TypeAObject(void)
{
}

int TypeAObject::GetWidth()
{
	return m_pSprite->Width;
}

int TypeAObject::GetHeight()
{
	return m_pSprite->Height;
}

int TypeAObject::GetCenterX()
{
	return m_iLeft +  m_pSprite->Width / 2;
}

int TypeAObject::GetBottom()
{
	return m_iTop + m_pSprite->Height;
}

int TypeAObject::GetRight()
{
	return m_iLeft + m_pSprite->Width;
}

int TypeAObject::GetLeft()
{
	return m_iLeft;
}

int TypeAObject::GetTop()
{
	return m_iTop;
}

int TypeAObject::GetLeftCollision(int iScope)
{
	int iTiles = 0x0;
	for (int i = m_iTop; i < GetBottom(); i += 1)
	{
		iTiles |= m_pMapTypeA->GetTile(m_iLeft-iScope, i);
	}

	return iTiles;
}

int TypeAObject::GetRightCollision(int iScope)
{
	int iTiles = 0x0;
	for (int i = m_iTop; i < GetBottom(); i += 1)
	{
		iTiles |= m_pMapTypeA->GetTile(GetRight()+iScope, i);
	}

	return iTiles;
}

int TypeAObject::GetBottomCollision(int iScope)
{
	int iTiles = 0x0;
	for (int j = m_iLeft; j < GetRight(); j += 1)
	{
		iTiles |= m_pMapTypeA->GetTile(j, GetBottom()+iScope);
	}

	return iTiles;
}

int TypeAObject::GetTopCollision(int iScope)
{
	int iTiles = 0x0;
	for (int j = m_iLeft; j < GetRight(); j += 1)
	{
		iTiles |= m_pMapTypeA->GetTile(j, m_iTop-iScope);
	}

	return iTiles;
}

int TypeAObject::GetCenterCollision()
{
	int iTiles = 0x0;
	for (int i = m_iTop; i < GetBottom(); i += 1)
		for (int j = m_iLeft; j < GetRight(); j += 1)
		{
			iTiles |= m_pMapTypeA->GetTile(j, i);
		}

	return iTiles;
}

void TypeAObject::SetMap(TypeAMap *pCurrMap)
{
	m_pMapTypeA = pCurrMap;
}

void TypeAObject::SetLeft(int iLeft)
{
	m_iLeft = iLeft;
}

void TypeAObject::SetTop(int iTop)
{
	m_iTop = iTop;
}

void TypeAObject::SetJump(int iJumpStep)
{
	m_iIsJump = iJumpStep * MOVE_PIXELS;
}

int TypeAObject::GetScreenPosition()
{
	int iVal = GetCenterX() - SCREEN_WIDTH / 2;

	if (iVal < 0)
		return -1;
	else if (iVal > 0)
		return 1;
	else
		return 0;
}

void TypeAObject::MoveX(int iSign)
{
	m_iLeft += iSign * MOVE_PIXELS;
}

void TypeAObject::MoveY(int iSign)
{
	m_iTop += iSign * MOVE_PIXELS;
}

void TypeAObject::SetFall(int iStatus)
{
	m_iIsFall = iStatus;
}

int TypeAObject::GetJump()
{
	return m_iIsJump;
}

int TypeAObject::GetFall()
{
	return m_iIsFall;
}

void TypeAObject::ElapseJump()
{
	m_iIsJump -= MOVE_PIXELS;
}

void TypeAObject::MoveLeft(int &iCurrScreenX)
{
	m_pSprite->PreviousFrame();

	if (GetScreenPosition() != 0)	// đi qua trái khi nhân vật không nằm giữa màn hình
	{
		for (int i = MOVE_PIXELS; i > 0; --i)
			if (!(GetLeftCollision(i) & (HARD_TILE | LEFT_SIDE_TILE)))
			{
				Functions::Move(m_iLeft, 0, -i);
				break;
			}
	}
	else										// nhân vật đang nằm giữa màn hình
	{
		if (m_pMapTypeA->GetMapPosition() >= 0)		// khung màn hình ko nằm ở vị trí bắt đầu
		{
			for (int i = MOVE_PIXELS; i > 0; --i)
				if (!(GetLeftCollision(i) & HARD_TILE))
				{
					Functions::Move(iCurrScreenX, 0, -i);
					break;
				}
		}
		else										// khung màn hình ở vị trí bắt đầu
		{
			for (int i = MOVE_PIXELS; i > 0; --i)
				if (!(GetLeftCollision(i) & HARD_TILE))
				{
					Functions::Move(m_iLeft, 0, -i);
					break;
				}
		}
	}

	if (GetJump() == 0 && GetFall() == 0)			// khi không đang nhảy lên và không đang rơi xuống
	{
		if (!(GetBottomCollision(1) & HARD_TILE))
			SetFall(1);
	}
}

void TypeAObject::MoveRight(int &iCurrScreenX)
{
	m_pSprite->NextFrame();

	if (GetScreenPosition() != 0)	// khi nhân vật không nằm giữa màn hình, di chuyển nhân vật
	{
		for (int i = MOVE_PIXELS; i > 0; --i)
			if (!(GetRightCollision(i) & (HARD_TILE | RIGHT_SIDE_TILE)))
			{
				Functions::Move(m_iLeft, 0, i);
				break;
			}
	}
	else										// nhân vật đang nằm giữa màn hình
	{
		if (m_pMapTypeA->GetMapPosition() <= 0)			// di chuyển màn hình sang phải
		{
			for (int i = MOVE_PIXELS; i > 0; --i)
				if (!(GetRightCollision(i) & HARD_TILE))
				{
					Functions::Move(iCurrScreenX, 0, i);
					break;
				}
		}
		else									// di chuyển nhân vật
		{
			for (int i = MOVE_PIXELS; i > 0; --i)
				if (!(GetRightCollision(i) & HARD_TILE))
				{
					Functions::Move(m_iLeft, 0, i);
					break;
				}
		}
	}

	if (GetJump() == 0 && GetFall() == 0)
	{
		if (!(GetBottomCollision(1) & HARD_TILE))
			SetFall(1);
	}
}

int TypeAObject::OnAirHandle()
{
	if (GetJump() > 0)		// đang nhảy
	{
		int i = MOVE_PIXELS;
		for ( ; i > 0; --i)
			if (!(GetTopCollision(i) & HARD_TILE))
			{
				Functions::Move(m_iTop, 0, -i);			// nhảy lên giảm Y
				Functions::Move(m_iIsJump, 0, -i);		// giai đoạn nhảy trôi dần qua
				break;
			}

		if (i != MOVE_PIXELS)					// đụng nóc	
			SetJump(0);							// dừng giai đoạn nhảy

		if (GetJump() == 0)							// giai đoạn nhảy kết thúc, chuyển sang giai đoạn rơi
			SetFall(1);

		return 1;
	}
	else
	{
		if (GetFall() != 0)		// đang rơi
		{
			int i = MOVE_PIXELS;
			for ( ; i > 0; --i)
				if (!(GetBottomCollision(i) & HARD_TILE))
				{
					Functions::Move(m_iTop, 0, i);
					break;
				}

			if (i != MOVE_PIXELS)
				SetFall(0);

			return 1;
		}
	}

	return 0;			// ko đang nhảy và ko đang rơi
}

void TypeAObject::Jump()
{
	SetJump(JUMP_STEP);
	int i = MOVE_PIXELS;
	for ( ; i > 0; --i)
		if (!(GetTopCollision(i) & HARD_TILE))
		{
			Functions::Move(m_iTop, 0, -i);
			break;
		}

	if (i != MOVE_PIXELS)
		SetJump(0);			// nhảy lên đụng nóc ngay tức khắc, khỏi nhảy nữa
}