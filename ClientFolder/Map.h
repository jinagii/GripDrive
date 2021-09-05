#pragma once

// D2D를 넣어주는 헤더
#include "GlobalDefine.h"
#include "GameManager.h"

class D2DEngine;
class ICollisionObject;

class Map
{
public:
	Map();
	Map(D2DEngine* pEnginem, eGameTrack track);
	~Map();

private:
	vector<TileInfo*> v_TileInfo;
	vector<ICollisionObject*> v_Collider;

	ID2D1Bitmap* m_pBitmapBG;
	ID2D1Bitmap* m_pBitmapTrack;


	D2D_VECTOR_2F* m_Pos;

	/// 그리드 사이즈와 비트맵 크기
	int m_GridSize;
	int m_Width;
	int m_Height;

	int m_RowCount;				// 행의 크기
	int m_ColCount;				// 열의 크기
	int m_TotalCount;			// 모든 그리드, 행 x 열의 크기

public:
	void SetBitmap(ID2D1Bitmap* pBitmap);

	
	void Draw(D2DEngine* pEngine, D2D1_MATRIX_3X2_F viewTM);
	D2D1_MATRIX_3X2_F MakeIsometric(D2D1_VECTOR_2F center, float size, float rot);

	ID2D1Bitmap* GetBitmap() { return m_pBitmapTrack;	}

};

