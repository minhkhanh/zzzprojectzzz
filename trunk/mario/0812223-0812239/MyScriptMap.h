#pragma once
#include "stdafx.h"
#include "cDef.h"
#include "GameState.h"
#include "Resource.h"

class MyScriptMap
{
public:
	vector< TCHAR* > m_vScript;
	int m_iCurScript;
	MyScriptMap(TCHAR *strScriptPath);
	void NextScript();
	void PreviousScript();
	void ShowText(int X, int Y, CDC *pDC);
	virtual ~MyScriptMap(void);
};
