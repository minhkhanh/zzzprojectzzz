#include "StdAfx.h"
#include "TypeAOcegrine.h"

TypeAOcegrine::TypeAOcegrine(void)
{
}

TypeAOcegrine::TypeAOcegrine(Sprite *sprImage, TypeAMap *pCurrMap)
{
	m_pSprite = sprImage;
	m_pMapTypeA = pCurrMap;

	m_iIsJump = m_iIsCrouch = 0;
}

TypeAOcegrine::~TypeAOcegrine(void)
{
}

void TypeAOcegrine::SetJump(int iJumpStep)
{
	m_iIsJump = iJumpStep * MOVE_PIXELS;
}

int TypeAOcegrine::GetBottom()
{
	return m_iTop + GetHeight();
}

int TypeAOcegrine::GetHeight()
{
	return m_pSprite->GetHeight() - m_iIsCrouch*13;
}

void TypeAOcegrine::MoveLeft()
{
	if (m_pSprite->GetDirection() == 1)
		m_pSprite->TurnBack();

	if (m_iIsJump == 0 && m_iIsFall == 0 && m_iIsCrouch == 0)
		PreviousMoveFrame();

	for (int i = MOVE_PIXELS; i > 0; --i)
		if (!(GetLeftCollision(i) & (HARD_TILE | LEFT_SIDE_TILE)))
		{
			m_iLeft -= i;
			break;
		}

	if (m_iIsJump == 0 && m_iIsFall == 0)			// khi không đang nhảy lên và không đang rơi xuống
	{
		if (!(GetBottomCollision(1) & HARD_TILE))
			m_iIsFall = 1;
	}
}

void TypeAOcegrine::MoveRight()
{
	if (m_pSprite->GetDirection() == -1)
		m_pSprite->TurnBack();

	if (m_iIsJump == 0 && m_iIsFall == 0 && m_iIsCrouch == 0)
		NextMoveFrame();

	for (int i = MOVE_PIXELS; i > 0; --i)
		if (!(GetRightCollision(i) & (HARD_TILE | RIGHT_SIDE_TILE)))
		{
			m_iLeft += i;
			break;
		}

	if (m_iIsJump == 0 && m_iIsFall == 0)
	{
		if (!(GetBottomCollision(1) & HARD_TILE))
			m_iIsFall = 1;
	}
}

int TypeAOcegrine::OnAirHandle(int &iCoinCount)
{
	if (OnJump(iCoinCount))	// xử lý nhảy
		return 1;
	
	if (OnFall())				// xử lý rơi
		return 1;

	return 0;			// ko đang nhảy và ko đang rơi
}

int TypeAOcegrine::OnJump(int &iCoinCount)
{
	if (m_iIsJump > 0)
	{
		int iTiles = 0x0;
		int i = MOVE_PIXELS;
		for ( ; i > 0; --i)
		{
			int iTmp = GetTopCollision(i);
			if (!(iTmp & HARD_TILE))
			{
				m_iTop -= i;			// nhảy lên giảm Y
				m_iIsJump -= i;		// giai đoạn nhảy trôi dần qua
				break;
			}
			else
				iTiles = iTmp;
		}
	
		if (i != MOVE_PIXELS)			// đụng nóc
		{
			m_iIsJump = 0;				// dừng giai đoạn nhảy
			if (iTiles & ITEM_TILE)		// khi nhảy đội trúng hộp vật phẩm
			{
				int iRight = GetRight();
				for (int j = m_iLeft; j < iRight; ++j)
					if (m_pMapTypeA->GetTile(j, m_iTop-1) & ITEM_TILE)
					{
						++iCoinCount;			// hiện tại hộp vật phẩm chỉ chứa 1 đồng xu
						m_pMapTypeA->SetTile(j, m_iTop-1, 0);	// đổi tile cho hộp vật phẩm sau khi bị an
					}
			}
		}
	
		if (m_iIsJump == 0)			// giai đoạn nhảy kết thúc, chuyển sang giai đoạn rơi
			m_iIsFall = 1;

		return 1;
	}

	return 0;
}

void TypeAOcegrine::Jump()
{
	SetJump(JUMP_STEP);

	if (m_iIsCrouch)
		return;

	if (m_pSprite->GetDirection() == 1)
		m_pSprite->SetPositionFrame(3);
	else
		m_pSprite->SetPositionFrame(10);
}

void TypeAOcegrine::Prepare(TypeAMap *pTypeAMap)
{
	if (m_iIsCrouch)
		CrouchToggle();

	m_pMapTypeA = pTypeAMap;

	m_iLeft = Functions::GetScreenCenterX();
	m_iTop = m_pMapTypeA->CalcY(m_iLeft, 0) - GetHeight();

	m_iIsFall = 1;
	m_iIsJump = 0;
}

int TypeAOcegrine::InnerCollisionHandle(int &iLiveCount, int &iCoinCount)
{
	int iCurrMap = m_pMapTypeA->GetCurrMapIndex();
	int iTiles = GetInnerCollision();

	if (iTiles & HELL_TILE)		// nhân vật nam chính chết giữa đường, chơi lại map
	{
		--iLiveCount;		// giảm mạng
		return iCurrMap;
	}
	else if (iTiles & DANGEROUS_TILE)	// chạm phải chướng ngại nguy hiểm chết người, đi lại từ đầu
	{
		--iLiveCount;
		return iCurrMap;
	}
	else if (iTiles & GATE_TILE)		// chạm cổng qua màn, load màn kế tiếp
	{
		return iCurrMap + 1;
	}
	
	if (iTiles & COIN_TILE)		// ăn tiền
	{
		int iBottom = GetBottom();
		int iRight = GetRight();
		for (int i = m_iTop; i < iBottom; ++i)
			for (int j = m_iLeft; j < iRight; ++j)
				if (m_pMapTypeA->GetTile(j, i) & COIN_TILE)
				{
					m_pMapTypeA->SetTile(j, i, 31);
					++iCoinCount;
				}
	}

	// sắp xếp câu lệnh if như trên là theo thứ tự ưu tiên: rớt xuống vực, chết > chạm quái, chết > chạm cửa, qua màn > ăn tiền

	return -1;
}

void TypeAOcegrine::PreviousMoveFrame()
{
	if (m_iIsCrouch != 0)
		return;

	m_pSprite->PreviousFrame();
	if (m_pSprite->GetPositionFrame() < 10)
		m_pSprite->SetPositionFrame(13);
}

void TypeAOcegrine::NextMoveFrame()
{
	if (m_iIsCrouch != 0)
		return;

	m_pSprite->NextFrame();
	if (m_pSprite->GetPositionFrame() > 3)
		m_pSprite->SetPositionFrame(0);
}

void TypeAOcegrine::CrouchToggle()
{
	m_iIsCrouch = 1 - m_iIsCrouch;

	if (m_iIsCrouch)
	{
		m_iTop += 13;
		m_pSprite->AddOffSetY(13);

		if (m_pSprite->GetDirection() == 1)
			m_pSprite->SetPositionFrame(5);
		else
			m_pSprite->SetPositionFrame(8);
	}
	else
	{
		m_iTop -= 13;
		m_pSprite->AddOffSetY(-13);

		if (m_pSprite->GetDirection() == 1)
			m_pSprite->SetPositionFrame(0);
		else
			m_pSprite->SetPositionFrame(13);
	}
}