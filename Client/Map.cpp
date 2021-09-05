#include "Map.h"

// 이 경우 엔진 구현 사항을 알고 있어야하므로 헤더를 넣어준다.
#include "D2DEngine.h"
#include "ICollisionObject.h"
#include "CCircle.h"
#include "CTransform.h"

Map::Map()
	:m_pBitmapTrack(nullptr),
	m_Pos(0)
{

}

Map::Map(D2DEngine* pEngine, eGameTrack track)
	:m_Pos(0)
{
	switch (track)
	{
	case eTrack1:
	{
		m_pBitmapBG = pEngine->LoadBitmapByPath(L"./data/track/Track1BG.png");
		m_pBitmapTrack = pEngine->LoadBitmapByPath(L"./data/track/Track1.png");

		break;
	}
	case eTrack2:
	{
		m_pBitmapBG = pEngine->LoadBitmapByPath(L"./data/track/Track2GD.png");
		m_pBitmapTrack = pEngine->LoadBitmapByPath(L"./data/track/track2.png");
		break;
	}
	default:
		break;
	}


}

Map::~Map()
{

}

void Map::SetBitmap(ID2D1Bitmap* pBitmap)
{
	m_pBitmapTrack = pBitmap;
}

void Map::Draw(D2DEngine* pEngine, D2D1_MATRIX_3X2_F viewTM)
{
	pEngine->DrawUIImage(m_pBitmapBG);

	pEngine->SetTransform(viewTM);

	pEngine->DrawUIImage(m_pBitmapTrack);

	
	/*
	// 디버깅을 위해 직접 그리드를 그리겠당
	{
		int _width = m_pBitmapTrack->GetPixelSize().width;
		int _height = m_pBitmapTrack->GetPixelSize().height;

		int _row = _width / 100;
		int _col = _height / 100;

		for (int y = 0; y < _col; y++)
		{
			pEngine->DrawLine_PreSetCol(0, y * 100, _width, y * 100, GREEN_COLOR);
			pEngine->DrawText(10, ((y * 100) + 1), L"%d", y * 100);
		}

		for (int x = 0; x < _row; x++)
		{
			pEngine->DrawLine_PreSetCol(x * 100, 0, x * 100, _height, GREEN_COLOR);
			pEngine->DrawText(((x * 100) + 1), 10, L"%d", x * 100);
		}
	}
	*/

}

D2D1_MATRIX_3X2_F Map::MakeIsometric(D2D1_VECTOR_2F center, float size, float rot)
{
	D2D1_VECTOR_2F _sizeVec;
	_sizeVec.x = 1.0f;
	_sizeVec.y = 1.0f * size;

	D2D1_MATRIX_3X2_F transMT = CTransform::MakeComplexTransform(rot, _sizeVec, center);

	return transMT;
}