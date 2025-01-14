#include "StdAfx.h"
#include "MyScriptMap.h"

MyScriptMap::MyScriptMap(TCHAR *strScriptPath)
{
	FILE * f;
	if ((f = _wfopen(strScriptPath, _T("rt"))) == NULL)
		exit(0);

	char pTemp[INPUT_MAX_LEN];
	int indexCell = 0;
	while (fgets(pTemp, INPUT_MAX_LEN, f))
	{
		m_vScript.push_back(Functions::UTF8_to_WChar(pTemp));
	}
	fclose(f);
	m_vScript.push_back(wcsdup(_T("Nhấn Phím 'N' để tiếp tục...")));
	m_iCurScript = 0;
	ResetShowText();
}

MyScriptMap::~MyScriptMap(void)
{
	for (int i=0; i<m_vScript.size(); ++i)
	{
		delete m_vScript[i];
	}
	m_vScript.clear();
}

void MyScriptMap::NextScript()
{
	if (m_iCurScript < (m_vScript.size()-1)) ++m_iCurScript;
	ResetShowText();
}

void MyScriptMap::PreviousScript()
{
	if (m_iCurScript > 0) --m_iCurScript;
	ResetShowText();
}

void MyScriptMap::ShowText( int baseX, int baseY, CDC *pDC )
{
	CFont font;
	font.CreatePointFont(100, L"Arial");
	COLORREF color = pDC->SetTextColor(RGB(252, 255, 0));
	CFont * oldFont = pDC->SelectObject(&font);
	int oldBGRMode = pDC->SetBkMode(TRANSPARENT);
	FindNextWord();
	pDC->TextOut(baseX + m_iX, baseY + m_iY, &m_vScript[m_iCurScript][m_iCurWord], m_iCurLen);
	pDC->SelectObject(font);
	pDC->SetBkMode(oldBGRMode);
	pDC->SetTextColor(color);
}

void MyScriptMap::ShowText( int baseX, int baseY, Graphics *pGraphics )
{
	// Create a string.
	//WCHAR string[] = L"Trần Minh Khánh\n";

	// Initialize arguments.
	//FontFamily fontFamily = new FontFamily(L"Arial");
	Gdiplus::Font myFont(_T("Arial"), 16);
	PointF origin(baseX + m_iX, baseY + m_iY);
	SolidBrush blackBrush(Color(255, 0, 255, 0));

	// Draw string.
	int iSaveCurWord = m_iCurWord;
	int iSaveCurScript = m_iCurScript;
	FindNextWord();
	pGraphics->DrawString(&m_vScript[iSaveCurScript][iSaveCurWord], m_iCurLen, &myFont, origin, &blackBrush);
}

void MyScriptMap::ResetShowText()
{
	m_iCurWord = 0;
	m_iX = 20;
	m_iY = 0;
	m_iNumWord = 0;
}

void MyScriptMap::Reset()
{
	ResetShowText();
	m_iCurScript = 0;
}

void MyScriptMap::FindNextWord()
{
	m_iCurLen = 0;
	++m_iNumWord;
	while ((m_iCurWord<wcslen(m_vScript[m_iCurScript]))&&(m_vScript[m_iCurScript][m_iCurWord]!=' '))
	{
		++m_iCurWord;
		++m_iCurLen;
	}
	if (m_iCurWord==wcslen(m_vScript[m_iCurScript])) 
	{
		--m_iCurLen;
		m_bIsEndLine = true;
		NextScript();
		return;
	}
	else 
	{
		m_bIsEndLine = false;
		++m_iCurWord;
		++m_iCurLen;
	}
	if (m_iNumWord%5 == 0)
	{
		m_iX = 0;
		m_iY += 25;
	} 
	else
	{
		m_iX += 12*m_iCurLen;
	}
}

bool MyScriptMap::IsEndLine()
{
	return m_bIsEndLine;
}