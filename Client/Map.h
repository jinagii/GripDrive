#pragma once

// D2D�� �־��ִ� ���
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

	/// �׸��� ������� ��Ʈ�� ũ��
	int m_GridSize;
	int m_Width;
	int m_Height;

	int m_RowCount;				// ���� ũ��
	int m_ColCount;				// ���� ũ��
	int m_TotalCount;			// ��� �׸���, �� x ���� ũ��

public:
	void SetBitmap(ID2D1Bitmap* pBitmap);

	
	void Draw(D2DEngine* pEngine, D2D1_MATRIX_3X2_F viewTM);
	D2D1_MATRIX_3X2_F MakeIsometric(D2D1_VECTOR_2F center, float size, float rot);

	ID2D1Bitmap* GetBitmap() { return m_pBitmapTrack;	}

};

