#pragma once

#include "Functions.h"

class GameStateTypeA;
class MainGame;
class TypeAMonster;
class TypeAOcegrine;

class TypeAMap
{
private:
	GameStateTypeA *m_pStateTypeA;

	// mảng quái
	vector<TypeAMonster*> m_vMonster;

	// ảnh nền của map
	int m_idBackground;
	MyBitmap *m_pBackground;

	// đường dẫn đến file map
	char *m_strMapPath;

	// nội dung map
	vector<vector<int>> m_vData;

	// chiều rộng, dài của map
	int m_iMapWidth, m_iMapHeight;

public:
	TypeAMap(void);
	TypeAMap(char *strMapPath, int idImg, GameStateTypeA* stateTypeA);
	~TypeAMap(void);

	// các hàm lấy
	int GetMapWidth();
	int GetMapHeight();
	//int GetOnMapPosition();

	int GetCurrScreenX();
	int GetCurrMapIndex();

	// load dữ liệu map từ file vào mảng
	void LoadMap();
	void ReleaseData();

	// vẽ map
	void PaintMap(CDC* pDC);

	// di chuyển các con quái trong map
	int MoveMonsters();

	// vẽ quái lên màn hình
	void ThrowMonsters(CDC* pDC);

	// kiểm tra chướng ngại vật
	int GetTile(int j, int i);
	//int GetData(int y, int x);

	// sửa tile
	void SetTile(int j, int i, int iTile);

	void ActivateMonsters();

	// lấy điểm tựa đầu tiên khi chiếu từ iX xuống
	int CalcY(int iX, int iY);

	GameStateTypeA *GetGameStateTypeA();

	//void KillMonsters(TypeAOcegrine *pOcegrine);

	//void MarkMonster(int index);
	//void UnmarkMonster(int index);

	friend class GameStateTypeA;
	friend class TypeAOcegrine;
};
