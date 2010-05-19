#pragma once

#include "stdafx.h"
#include "GameStateTypeA.h"
#include "MyBitmap.h"
#include "cDef.h"
#include "0812223-0812239.h"
#include "Sprite.h"
#include "GameKeys.h"
#include "MainGame.h"
#include "TypeAMonster.h"

#define BASE_HEIGHT						24
#define BASE_WIDTH						24

#define SCREEN_FRAME_WIDTH				16
#define SCREEN_FRAME_HEIGHT				20

#define SCREEN_WIDTH					380
#define SCREEN_HEIGHT					480

#define OTHER_TILE						0
#define GATE_TILE						1
#define HARD_TILE						2
#define HEAVEN_TILE						3
#define HELL_TILE						4
#define SIDE_TILE						5
#define DANGEROUS_TILE					6
#define COIN_TILE						7

class GameStateTypeA;
class TypeAMonster;

class TypeAMapInfo
{
public:
	TypeAMapInfo(void);
	TypeAMapInfo(char *strMapPath, int idImg, GameStateTypeA* stateTypeA);
	~TypeAMapInfo(void);

	GameStateTypeA *m_pStateTypeA;

	MyBitmap *m_pBackground;
	char *m_pMapPath;
	int m_idBackground;

	vector<vector<int>> m_vMap;
	vector<TypeAMonster*> m_vMonster;

	int m_iMapWidth, m_iMapHeight;

	// load dữ liệu map từ file vào mảng
	void LoadMap();
	// vẽ map
	void PaintMap();
	// kiểm tra chướng ngại vật
	int WhatTile(int i, int j);
	// lấy điểm tựa đầu tiên khi chiếu từ iX xuống
	int CalcY(int iX);
};
