#include "StdAfx.h"
#include "TypeAMonster.h"

#include "GameStateTypeA.h"
#include "TypeAOcegrine.h"

TypeAMonster::TypeAMonster(void)
{
}

TypeAMonster::TypeAMonster(Sprite *pSprite, TypeAMap *pCurrMap, int iLeft, int iTop, int iMoveDirection):
m_iMoveDirection(iMoveDirection),
m_iStatus(0)
{
	m_pSprite = pSprite;
	m_pMapTypeA = pCurrMap;

	m_iLeft = iLeft;
	m_iTop = iTop;
}

TypeAMonster::~TypeAMonster(void)
{
}

void TypeAMonster::Move()
{
	if (m_iStatus != 1)
		return;

	if (m_iMoveDirection & VERTICAL)
	{
		if (m_iMoveDirection == MOVE_DOWN)
			MoveDown();
		else if (m_iMoveDirection == MOVE_UP)
			MoveUp();

		NextMoveFrame();
	}
	else if (m_iMoveDirection & HORIZON)
	{
		if (m_iMoveDirection == MOVE_RIGHT)
			MoveRight();
		else if (m_iMoveDirection == MOVE_LEFT)
			MoveLeft();

		if (!(GetBottomCollision(1) & cDef::HARD_TILE))
			m_iIsFall = 1;

		OnFall();
	}
}

void TypeAMonster::MoveUp()
{
	int i = MONSTER_MOVE_PIXELS;
	for ( ; i > 0; --i)
		if (!(GetTopCollision(i) & (cDef::HARD_TILE | cDef::HEAVEN_TILE)))
		{
			Functions::Move(m_iTop, 0, -i);
			break;
		}

	if (i != MONSTER_MOVE_PIXELS)
	{
		m_iMoveDirection = MOVE_DOWN;
	}
}

void TypeAMonster::MoveDown()
{
	int i = MONSTER_MOVE_PIXELS;
	for ( ; i > 0; --i)
		if (!(GetBottomCollision(i) & (cDef::HARD_TILE | cDef::HELL_TILE)))
		{
			Functions::Move(m_iTop, 0, i);
			break;
		}

	if (i != MONSTER_MOVE_PIXELS)
	{
		m_iMoveDirection = MOVE_UP;
	}
}

void TypeAMonster::MoveLeft()
{
	if (m_pSprite->GetDirection() == 1)
		m_pSprite->TurnBack();

	PreviousMoveFrame();

	int i = MONSTER_MOVE_PIXELS;
	for ( ; i > 0; --i)
		if (!(GetLeftCollision(i) & (cDef::HARD_TILE | cDef::LEFT_SIDE_TILE)))
		{
			Functions::Move(m_iLeft, 0, -i);
			break;
		}

	if (i != MONSTER_MOVE_PIXELS)
	{
		m_iMoveDirection = MOVE_RIGHT;
	}
}

void TypeAMonster::MoveRight()
{
	if (m_pSprite->GetDirection() == -1)
		m_pSprite->TurnBack();

	NextMoveFrame();

	int i = MONSTER_MOVE_PIXELS;
	for ( ; i > 0; --i)
		if (!(GetRightCollision(i) & (cDef::HARD_TILE | cDef::RIGHT_SIDE_TILE)))
		{
			Functions::Move(m_iLeft, 0, i);
			break;
		}

	if (i != MONSTER_MOVE_PIXELS)
	{
		m_iMoveDirection = MOVE_LEFT;
	}
}

void TypeAMonster::PreviousMoveFrame()
{
	m_pSprite->PreviousFrame();

	if (m_pSprite->GetPositionFrame() < 4)
		m_pSprite->SetPositionFrame(6);
}

void TypeAMonster::NextMoveFrame()
{
	m_pSprite->NextFrame();

	if (m_pSprite->GetPositionFrame() > 3)
		m_pSprite->SetPositionFrame(1);
}

void TypeAMonster::Activate()
{
	m_iStatus = 1;
}

int TypeAMonster::InnerCollisionHandle()
{
	if (m_iStatus != 1)
		return 0;

	int iTiles = GetInnerCollision();

	if (iTiles & cDef::HELL_TILE)
	{
		Collapse();
		return 0;
	}
	else
	{
		int iOceCollision = GetOcegrineCollision();
		if (iOceCollision == -1)
		{
			Collapse();
			return -1;
		}
		else if (iOceCollision == 1)
			return 1;
		else
			return 0;
	}
}

int TypeAMonster::GetStatus()
{
	return m_iStatus;
}

int TypeAMonster::GetOcegrineCollision()
{
	GameStateTypeA *pGameState = m_pMapTypeA->GetGameStateTypeA();
	TypeAOcegrine *pOcegrine = pGameState->GetOcegrine();

	int iRight = GetRight();
	int iOceBottom = pOcegrine->GetBottom();
	int iOceLeft = pOcegrine->GetLeft();
	int iOceRight = pOcegrine->GetRight();

	if ((iOceRight >= m_iLeft && iOceLeft < m_iLeft)
		|| (iOceLeft < iRight && iOceRight >= iRight)
		|| (iOceLeft >= m_iLeft && iOceRight < iRight))
	{
		if (iOceBottom - m_iTop >= 1 && iOceBottom - m_iTop <= cDef::OCEGRINE_MOVE_PIXELS*2)
			return -1;		// quái chết
		else
		{
			if (iOceBottom - m_iTop > cDef::OCEGRINE_MOVE_PIXELS && pOcegrine->GetTop() < GetBottom())
				return 1;	// nhân vật chết
		}
	}
	
	return 0;
}

void TypeAMonster::Collapse()
{
	m_iStatus = -25;

	m_pSprite->SetPositionFrame(0);
}

void TypeAMonster::RenderSprite( CDC* pDC )
{
	if (m_iStatus >= -1 && m_iStatus != 1)
		return;

	if (m_iStatus < -1)
		++m_iStatus;

	TypeAObject::RenderSprite(pDC);
}