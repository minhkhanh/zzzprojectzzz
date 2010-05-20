#include "StdAfx.h"
#include "TypeAMap.h"

TypeAMap::TypeAMap(void)
{
}

TypeAMap::TypeAMap(char *strMapPath, int idImg, GameStateTypeA* stateTypeA)
{
	m_pMapPath = _strdup(strMapPath);
	m_idBackground = idImg;
	m_pStateTypeA = stateTypeA;
}

TypeAMap::~TypeAMap(void)
{
}

void TypeAMap::LoadMap()
{
	FILE   *f;
	if ((f = fopen(m_pMapPath, "rt")) == NULL)
		exit(0);

	char pTemp[INPUT_MAX_LEN];
	int indexCell = 0;
	m_iPixelMapWidth = m_iPixelMapHeight = 0;
	while (fgets(pTemp, INPUT_MAX_LEN, f))
	{
		++m_iPixelMapHeight;		// tính độ cao map

		vector<int> vMapRow;
		m_iPixelMapWidth = strlen(pTemp);
		for (int i = 0; i < m_iPixelMapWidth; ++i)
		{
			if (pTemp[i] == '?') 
				vMapRow.push_back(-1);
			else
			{
				//if	(pTemp[i] == 'a')		// bổ sung quái vật
				//{
				//	Sprite *sprMonster = new Sprite(0,0,24,32,0,3,0, new MyBitmap((LPCTSTR)IDIMG_MONSTER07));
				//	m_vMonster.push_back(new TypeAMonster(sprMonster, indexCell));
				//}

				vMapRow.push_back(pTemp[i]-65);
			}

			++indexCell;
		}

		m_vMap.push_back(vMapRow);
	}

	fclose(f);

	// load ảnh nền
	m_pBackground = new MyBitmap((LPCTSTR)m_idBackground);

	// chuyển sang đơn vị pixel
	m_iPixelMapWidth *= BASE_WIDTH;
	m_iPixelMapHeight *= BASE_HEIGHT;
}

void TypeAMap::PaintMap()
{
	m_pBackground->DrawTransparent(&(m_pStateTypeA->mg->m_MemDC), 0, 0, RGB(255,255,255));

	for (int i = 0; i < SCREEN_HEIGHT; i += BASE_HEIGHT)
		for (int j = - BASE_WIDTH; j < SCREEN_WIDTH + BASE_WIDTH; j += BASE_WIDTH)
		{
			int jj = j + m_pStateTypeA->m_iCurrScreenX;
			if (jj < 0 || jj > m_iPixelMapWidth)
				continue;

			CPoint cell = Functions::Pixel2Cell(jj, i);
			int iVal = m_vMap[cell.y][cell.x];
			if (iVal != 32 && iVal != -1)		// nếu ô đang xét không phải là quái và không phải rỗng
			{
				m_pStateTypeA->sprTiles->SetPositionFrame(iVal);
				m_pStateTypeA->sprTiles->Render(j-(m_pStateTypeA->m_iCurrScreenX % BASE_WIDTH), i, &(m_pStateTypeA->mg->m_MemDC));
			}
		}
}

int TypeAMap::WhatTile(int j, int i)
{
	if (i < 0)
		return HEAVEN_TILE;		// trên khung màn hình
	if (i >= m_iPixelMapHeight)
		return HELL_TILE;		// dướj khung màn hình

	j += m_pStateTypeA->m_iCurrScreenX;

	if (j < 0)
		return LEFT_SIDE_TILE;		// biên trái
	if (j >= m_iPixelMapWidth)
		return RIGHT_SIDE_TILE;		// biên phảj

	CPoint cell = Functions::Pixel2Cell(j, i);
	int iTile = m_vMap[cell.y][cell.x];
	if ((iTile >= 0 && iTile <= 6)
		|| iTile == 10
		|| (iTile >= 15 && iTile <= 17)
		|| (iTile >= 24 && iTile <= 27))
		return HARD_TILE;		// đây là các chướng ngại ko thể đi xuyên qua

	if (iTile == 13 || iTile == 14
		|| iTile == 19 || iTile == 20
		|| iTile == 21 || iTile == 22)
		return GATE_TILE;		// cổng qua màn

	if (iTile == 11 || iTile == 18)
		return DANGEROUS_TILE;		// chướng ngại gây nguy hiểm chết người

	return OTHER_TILE;	// còn lại???
}

int TypeAMap::CalcY(int iX, int iY)
{
	for (int i = iY; i < m_iPixelMapHeight; i += BASE_HEIGHT)
	{
		if (WhatTile(iX, i) == HARD_TILE)
			return i;
	}

	return -1;		// từ trên chiếu xuống không có điểm tựa
}