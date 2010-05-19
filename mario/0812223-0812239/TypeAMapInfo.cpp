#include "StdAfx.h"
#include "TypeAMapInfo.h"

TypeAMapInfo::TypeAMapInfo(void)
{
}

TypeAMapInfo::TypeAMapInfo(char *strMapPath, int idImg, GameStateTypeA* stateTypeA)
{
	m_pMapPath = _strdup(strMapPath);
	m_idBackground = idImg;
	m_pStateTypeA = stateTypeA;
}

TypeAMapInfo::~TypeAMapInfo(void)
{
}

void TypeAMapInfo::LoadMap()
{
	FILE   *f;
	if ((f = fopen(m_pMapPath, "rt")) == NULL)
		exit(0);

	char pTemp[INPUT_MAX_LEN];
	int indexCell = 0;
	m_iMapFrameWidth = m_iMapFrameHeight = 0;
	while (fgets(pTemp, INPUT_MAX_LEN, f))
	{
		++m_iMapFrameHeight;		// tính độ cao map

		vector<int> vMapRow;
		m_iMapFrameWidth = strlen(pTemp);
		for (int i = 0; i < m_iMapFrameWidth; ++i)
		{
			if (pTemp[i] == '?') 
				vMapRow.push_back(-1);
			else
			{
				if	(pTemp[i] == 'a')		// bo sung quai vat, ak
				{
					Sprite *sprMonster = new Sprite(0,0,24,32,0,3,0, new MyBitmap((LPCTSTR)IDIMG_MONSTER07));
					m_vMonster.push_back(new TypeAMonster(sprMonster, indexCell));
				}

				vMapRow.push_back(pTemp[i]-65);
			}

			++indexCell;
		}

		m_vMap.push_back(vMapRow);
	}

	fclose(f);

	// load ảnh nền
	m_pBackground = new MyBitmap((LPCTSTR)m_idBackground);
}

void TypeAMapInfo::PaintMap()
{
	m_pBackground->DrawTransparent(&(m_pStateTypeA->mg->m_MemDC), 0, 0, RGB(255,255,255));

	for (int i = 0; i < SCREEN_FRAME_HEIGHT; ++i)
		for (int j = 0; j < SCREEN_FRAME_WIDTH; ++j)
		{
			if (m_vMap[i][j+m_pStateTypeA->m_iCurrScreenFrameX] != 32 && m_vMap[i][j+m_pStateTypeA->m_iCurrScreenFrameX] != -1)		// nếu ô đang xét không phải là quái và không phải rỗng
			{
				m_pStateTypeA->sprTiles->SetPositionFrame(m_vMap[i][j+m_pStateTypeA->m_iCurrScreenFrameX]);
				m_pStateTypeA->sprTiles->Render(j*BASE_WIDTH, i*BASE_HEIGHT, &(m_pStateTypeA->mg->m_MemDC));
			}
		}
}

int TypeAMapInfo::WhatTile(int i, int j)
{
	if (i < 0)
		return HEAVEN_TILE;		// trên khung màn hình
	if (i >= m_iMapFrameHeight)
		return HELL_TILE;		// dưới khung màn hình

	if (j < 0 || j >= m_iMapFrameWidth)
		return SIDE_TILE;		// quá hai biên hai bên
		

	j = j + m_pStateTypeA->m_iCurrScreenFrameX;

	if ((m_vMap[i][j] >= 0 && m_vMap[i][j] <= 6)
		|| m_vMap[i][j] == 10
		|| (m_vMap[i][j] >= 15 && m_vMap[i][j] <= 17)
		|| (m_vMap[i][j] >= 24 && m_vMap[i][j] <= 27))
		return HARD_TILE;		// đây là các chướng ngại ko thể đi xuyên qua

	if (m_vMap[i][j] == 13 || m_vMap[i][j] == 14
		|| m_vMap[i][j] == 19 || m_vMap[i][j] == 20
		|| m_vMap[i][j] == 21 || m_vMap[i][j] == 22)
		return GATE_TILE;		// cổng qua màn

	if (m_vMap[i][j] == 11 || m_vMap[i][j] == 18)
		return DANGEROUS_TILE;		// chướng ngại gây nguy hiểm chết người

	return OTHER_TILE;	// còn lại???
}

int TypeAMapInfo::CalcY(int iX)
{
	for (int i = 0; i < m_iMapFrameHeight; ++i)
	{
		if (WhatTile(i, iX))
			return i-1;
	}

	return -1;		// từ trên chiếu xuống không có điểm tựa
}