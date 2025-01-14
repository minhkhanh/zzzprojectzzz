#pragma once
#include "MyBitmap.h"

// lớp quản lý Animation trong Game
class Sprite
{
private:
	int m_iDirection;

	int m_frameDirection; //0 vertical, 1 horizontal
	int m_loopType; //0 loop back to begining, 1 bounce between begin and end
	int m_loopDirection;
	int m_frameCount;
	int m_currentFrame;
	int m_srcX;
	int m_srcY;
	int m_frameDelay;
	int m_curDelay;
	MyBitmap *m_image;
	int Width;
	int Height;

public:
	Sprite(int srcX, int srcY, int Width, int Height, int frameDirection, int frameCount, int frameDelay, MyBitmap *image, int iDirection = 1);
	void SetLoopType(int loopType);
	void NextFrame();
	void PreviousFrame();
	void SetPositionFrame(int iFrame);
	void SetDelay(int Delay);
	void Render(int X, int Y, CDC *pDC);

	void TurnBack();
	int GetDirection();

	int GetPositionFrame();

	void SetHeight(int iHeight);
	int GetHeight();
	int GetWidth();

	void AddOffSetY(int iOffSet);

	Sprite(void);
	~Sprite(void);
};
