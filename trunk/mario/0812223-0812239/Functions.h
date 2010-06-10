#pragma once

#include "GameKeys.h"
#include "stdafx.h"
#include "0812223-0812239.h"
#include "cDef.h"
#include "Sprite.h"

#include <fstream>
#include <vector>
#include <GdiPlus.h>
#pragma comment(lib, "GdiPlus.lib")
using namespace std;
using namespace Gdiplus;

#define BASE_HEIGHT						24
#define BASE_WIDTH						24

#define SCREEN_FRAME_WIDTH				16
#define SCREEN_FRAME_HEIGHT				20

#define SCREEN_WIDTH					380
#define SCREEN_HEIGHT					480

//#define OTHER_TILE					0x00
#define GATE_TILE						0x01
#define HARD_TILE						0x02
#define HEAVEN_TILE						0x04
#define HELL_TILE						0x08
#define LEFT_SIDE_TILE					0x10
#define DANGEROUS_TILE					0x20
#define COIN_TILE						0x40
#define RIGHT_SIDE_TILE					0x80
#define ITEM_TILE						0x0100
#define MONSTER_TILE					0x0200

#define MOVE_PIXELS		5

#define BOM8A 0xEF
#define BOM8B 0xBB
#define BOM8C 0xBF

class Functions
{
public:
	Functions(void);
	~Functions(void);

	// 2 hàm cho biết tọa độ pixel nằm trong ô nào
	static CPoint Pixel2Cell(CPoint pixel);
	static CPoint Pixel2Cell(int pixX, int pixY);

	// lấy tọa độ pixel (trái, trên) của ô
	static CPoint Cell2Pixel(CPoint cell);
	static CPoint Cell2Pixel(int cellX, int cellY);

	static void SetBit(int &a, int x, bool v);
	static bool GetBit(int a, int x);

	static int GetBits(int a, int x, int y);
	static void SetBits(int &a, int x, int y, int v);

	// hàm dùng để thay đổi giá trị iVal
	static void Move(int &iVal, int iStep, int iPixel = 0);

	// hàm lấy điểm giữa màn hình
	static int GetScreenCenterX();

	static wchar_t *UTF8_to_WChar(const char *string);
};
