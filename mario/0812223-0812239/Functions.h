#pragma once

#include "GameKeys.h"
#include "stdafx.h"
#include "0812223-0812239.h"
#include "cDef.h"
#include "Sprite.h"
#include "GameSound.h"

#include <fstream>
#include <vector>
#include <GdiPlus.h>
#pragma comment(lib, "GdiPlus.lib")
using namespace std;
using namespace Gdiplus;

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

	//static bool IsWaveFile(LPWSTR lpszFilePath);
	//static bool IsMIDIFile(LPWSTR lpszFilePath);

	//static void PlayCommandWaveMidi(LPWSTR lpszFilePath);
	//static void PlayStringWaveMidi(LPWSTR lpszAlias);
};
