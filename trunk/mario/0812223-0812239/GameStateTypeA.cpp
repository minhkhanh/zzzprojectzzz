#include "StdAfx.h"
#include "GameStateTypeA.h"

GameStateTypeA::GameStateTypeA(MainGame *_mg, TCHAR * pszPath)
{
	mg = _mg;
	GAME_STATE_DEF = cDef::STATE_TYPE_A;

	FILE   *f;
    if ((f = _tfopen(pszPath, _T("rt"))) == NULL)
       exit(0); 
    char szText[INPUT_MAX_LEN];
	int iTemp, iDem = 0, indexCell = 0;
    while (fgets(szText, INPUT_MAX_LEN, f))
	{
		// Bo ky tu \n o cuoi chuoi
		++iDem;
		//szText[wcslen(szText)-1] = '\0';
		vector<int> vTemp;
		iTemp = strlen(szText);
		if (iTemp == 300)
		{
			int i  = 0;
		}
		for (int i = 0; i < iTemp; ++i)
		{
			if (szText[i]=='?') 
				vTemp.push_back(-1);
			else
			{
				if	(szText[i] == 'a')		// bo sung quai vat, ak
				{
					Sprite *sprMonster = new Sprite(0,0,24,32,0,3,0, new MyBitmap((LPCTSTR)IDIMG_MONSTER07));
					m_vMonster.push_back(new TypeAMonster(sprMonster, indexCell));
				}

				vTemp.push_back(szText[i]-65);
			}

			++indexCell;
		}

		vMap.push_back(vTemp);
	}

	fclose(f);
	iMapLogicHeight = iDem;// * BASE_HEIGHT;
	iMapLogicWidth = iTemp;// * BASE_WIDTH;

	iMainX = 5;
	iMainY = 15;

	// tao nhan vat chinh, ak
	sprOcegrine = new Sprite(0,0,18,36,1,7,0,new MyBitmap((LPCTSTR)IDIMG_OCEGRINE_SPRITE));
	m_iCurrScreenFrameX = 0;
	m_iOcegrineX = ScreenFrameCenterX();
	m_iOcegrineY = CalcY(m_iOcegrineX);
	m_iIsJump = 0;
	m_iIsFall = 0;
}

GameStateTypeA::~GameStateTypeA(void)
{
}

void GameStateTypeA::MessageExit(int message, CDC *pDC)
{
	mg->mBiz_iCurrentState = cDef::STATE_LOGO;
}

void GameStateTypeA::MessageUpdate(int message, int keys)
{
	if ((keys & GameKeys::Left) == GameKeys::Left)
	{
		--iMainX;
		sprOcegrine->PreviousFrame();

		if (iMainX < 0)
			iMainX = 0;

		// xử lý đi về bên trái cho m_iCurrScreenFrameX, ak
		if (m_iOcegrineX >= ScreenFrameCenterX())
		{
			--m_iOcegrineX;
			if (IsBarrier(m_iOcegrineY, m_iOcegrineX))
				++m_iOcegrineX;
		}
		else
		{
			--m_iCurrScreenFrameX;
			if (m_iCurrScreenFrameX < 0)
			{
				m_iCurrScreenFrameX = 0;
				--m_iOcegrineX;
				if (m_iOcegrineX < 0)
					m_iOcegrineX = 0;
				else
				{
					if (IsBarrier(m_iOcegrineY, m_iOcegrineX))
						++m_iOcegrineX;
				}
			}
			else
			{
				if (IsBarrier(m_iOcegrineY, m_iOcegrineX))
					++m_iCurrScreenFrameX;
			}
		}

		if (m_iIsJump == 0 && m_iIsFall == 0 && !IsBarrier(m_iOcegrineY+1, m_iOcegrineX))
		{
			int iY = CalcY(m_iOcegrineX);
			if (iY != m_iOcegrineY)
			{
				m_iIsFall = 1;
			}
		}
	}
	
	if ((keys & GameKeys::Right) == GameKeys::Right)
	{
		++iMainX;
		sprOcegrine->NextFrame();

		if (iMainX >= iMapLogicWidth)
			iMainX = iMapLogicWidth - 1;

		// xử lý đi qua phải cho m_iCurrScreenFrameX, ak
		if (m_iOcegrineX < ScreenFrameCenterX())
		{
			++m_iOcegrineX;
			if (IsBarrier(m_iOcegrineY, m_iOcegrineX))
				--m_iOcegrineX;
		}
		else
		{
			++m_iCurrScreenFrameX;
			if (m_iCurrScreenFrameX + LOGIC_SCREEN_WIDTH > iMapLogicWidth)
			{
				m_iCurrScreenFrameX = iMapLogicWidth - LOGIC_SCREEN_WIDTH - 1;

				++m_iOcegrineX;
				if (m_iOcegrineX > LOGIC_SCREEN_WIDTH)
					m_iOcegrineX = LOGIC_SCREEN_WIDTH;
				else
				{
					if (IsBarrier(m_iOcegrineY, m_iOcegrineX))
						--m_iOcegrineX;

				}
			}
			else
			{
				if (IsBarrier(m_iOcegrineY, m_iOcegrineX))
				{
					--m_iCurrScreenFrameX;
				}
			}
		}

		if (m_iIsJump == 0 && m_iIsFall == 0 && !IsBarrier(m_iOcegrineY+1, m_iOcegrineX))
		{
			int iY = CalcY(m_iOcegrineX);
			if (iY != m_iOcegrineY)
			{
				m_iIsFall = 1;
			}
		}
	}

	if (m_iIsJump > 0)
	{

		--m_iOcegrineY;
		if (IsBarrier(m_iOcegrineY, m_iOcegrineX))
		{
			m_iOcegrineY+=2;		// nhảy lên đụng nóc
			m_iIsJump = 0;		//thì chuyển sang giai đoạn rơi
		}

		if (m_iIsJump > 0)
			--m_iIsJump;			// giai đoạn nhảy trôi dần qua

		if (m_iIsJump == 0)			// kết thúc giai đoạn nhảy, chuyển sang giai đoạn rơi
			m_iIsFall = 1;
	}
	else
	{
		if (m_iIsFall != 0)
		{
			++m_iOcegrineY;
			if (IsBarrier(m_iOcegrineY, m_iOcegrineX))
			{
				--m_iOcegrineY;
				m_iIsFall = 0;
			}
		}
		else
		{
			if ((keys & GameKeys::Up))	// xử lý nhảy khi không đang nhảy và không đang rơi
			{
				m_iIsJump = 3;
				--m_iOcegrineY;
				if (m_iOcegrineY < 0)		// nhảy lên đụng nóc ngay tức khắc
				{
					m_iOcegrineY = 0;
					m_iIsJump = 0;			// khỏi nhảy nữa
				}
			}
		}
	}
	
	if ((keys & GameKeys::Enter) == GameKeys::Enter)
	{
		mg->mBiz_SwitchState(cDef::STATE_MENU);
	}		
}

void GameStateTypeA::MessagePaint(int message, CDC *pDC)
{
	imgBG->DrawTransparent(&(mg->m_MemDC), 0, 0, RGB(255,255,255));
	//PaintMap(iMainX, iMainY, sprTiles, &(mg->m_MemDC));
	akPaintMap();
	sprOcegrine->Render(m_iOcegrineX*BASE_WIDTH, m_iOcegrineY*BASE_HEIGHT - 12, &(mg->m_MemDC));

	pDC->StretchBlt(0, 0, 380, 480, &(mg->m_MemDC), 0, 0 , 380, 480, SRCCOPY);
}

void GameStateTypeA::MessageEnter(int message)
{
	imgBG = new MyBitmap((LPCTSTR)IDIMG_BGR_TYPE_A_LEVEL_1);
	sprTiles = new Sprite(0,0,24,24,1,31,0,new MyBitmap((LPCTSTR)IDIMG_TILES_TYPE_A_LEVEL_1));
}

void GameStateTypeA::PaintMap(int iX, int iY, Sprite *sprImage, CDC *pDC)
{
	int i = iX - LOGIC_SCREEN_WIDTH/2;
	int j = iY - LOGIC_SCREEN_HEIGHT/2;

	if (i < 0)
		i = 0;
	if (j < 0)
		j = 0;

	if (i + LOGIC_SCREEN_WIDTH > iMapLogicWidth)
		i = iMapLogicWidth - LOGIC_SCREEN_WIDTH - 1;

	if (j + LOGIC_SCREEN_HEIGHT > iMapLogicHeight)
		j = iMapLogicHeight - LOGIC_SCREEN_HEIGHT;

	for (int jj = 0; jj < LOGIC_SCREEN_HEIGHT; ++jj)
		for (int ii = 0; ii < LOGIC_SCREEN_WIDTH; ++ii)
		{
			if ((vMap[j+jj][i+ii] + 65) != 'a' && vMap[j+jj][i+ii] != 0)
			{
				sprImage->SetPositionFrame(vMap[j+jj][i+ii]);
				sprImage->Render(ii*BASE_WIDTH, jj*BASE_HEIGHT, pDC);
			}
		}
}

void GameStateTypeA::akPaintMonster()
{
	for (unsigned i = 0; i < m_vMonster.size(); ++i)
	{
		int iX = m_vMonster[i]->m_iPos % iMapLogicWidth;
		int iY = m_vMonster[i]->m_iPos / iMapLogicWidth;

		if (iX >= m_iCurrScreenFrameX && iX <= m_iCurrScreenFrameX + LOGIC_SCREEN_WIDTH)
		{
			m_vMonster[i]->m_pSprite->Render((iX-m_iCurrScreenFrameX)*BASE_WIDTH, iY*BASE_HEIGHT, &(mg->m_MemDC));
		}
	}
}

void GameStateTypeA::akPaintMap()
{
	for (int i = 0; i < LOGIC_SCREEN_HEIGHT; ++i)
		for (int j = 0; j < LOGIC_SCREEN_WIDTH; ++j)
		{
			if (vMap[i][j+m_iCurrScreenFrameX] != 32 && vMap[i][j+m_iCurrScreenFrameX] != -1)
			{
				sprTiles->SetPositionFrame(vMap[i][j+m_iCurrScreenFrameX]);
				sprTiles->Render(j*BASE_WIDTH, i*BASE_HEIGHT, &(mg->m_MemDC));
			}
		}
}

int GameStateTypeA::CalcY(int iX)
{
	for (int i = 0; i < iMapLogicHeight; ++i)
	{
		if (IsBarrier(i, iX))
			return i-1;
	}

	return -1;		// từ trên chiếu xuống không có điểm tựa
}

int GameStateTypeA::ScreenFrameCenterX()
{
	return LOGIC_SCREEN_WIDTH / 2;
}

bool GameStateTypeA::IsBarrier(int i, int jFrame)
{
	int j = jFrame + m_iCurrScreenFrameX;

	if (i < 0 || i > iMapLogicHeight || j < 0 || j > iMapLogicWidth)
		return true;

	return ((vMap[i][j] >= 0 && vMap[i][j] <= 6)
		|| vMap[i][j] == 10
		|| (vMap[i][j] >= 15 && vMap[i][j] <= 17)
		|| (vMap[i][j] >= 24 && vMap[i][j] <= 27));
}