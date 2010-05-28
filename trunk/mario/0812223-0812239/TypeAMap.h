#pragma once

#include "stdafx.h"
#include "GameStateTypeA.h"
#include "MyBitmap.h"
#include "cDef.h"
#include "0812223-0812239.h"
#include "Sprite.h"
#include "GameKeys.h"
#include "MainGame.h"
#include "TypeAOcegrine.h"
#include "Functions.h"

class GameStateTypeA;
class TypeAOcegrine;
class TypeAMap;

class TypeAMap
{
public:
	TypeAMap(void);
	TypeAMap(char *strMapPath, int idImg, GameStateTypeA* stateTypeA);
	~TypeAMap(void);

	GameStateTypeA *m_pStateTypeA;

	MyBitmap *m_pBackground;
	char *m_pMapPath;
	int m_idBackground;

	vector<vector<int>> m_vData;
	//vector<TypeAOcegrine*> m_vMonster;

	int m_iMapWidth, m_iMapHeight;

	int GetMapWidth();
	int GetMapHeight();
	int GetOnMapPosition();

	// load dữ liệu map từ file vào mảng
	void LoadMap();
	// vẽ map
	void PaintMap();

	// kiểm tra chướng ngại vật
	int GetTile(int j, int i);

	// sửa tile
	void SetTile(int j, int i, int iTile);

	// lấy điểm tựa đầu tiên khi chiếu từ iX xuống
	int CalcY(int iX, int iY);
};
