#pragma once

#include "CGameObject.h"
#include "GlobalDefine.h"

/// 충돌감지만을 위한 더미 오브젝트. OBB기반
/// 이동불가. 주로 트랙을 만들기 위해서 사용
///
/// 2021.02.25 LeHide
class CDummy : public CGameObject
{
public:
	CDummy(D2D1_VECTOR_2F centerPos, float width, float height, float angle);
	~CDummy();

public:
	void Initialize();

	/// GameObject Override
	virtual void Update(float dTime) override;
	virtual void Draw(D2DEngine* pEngine, float dTime, D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM, bool isIsometric) override;
	virtual void DrawDebugData(D2DEngine* pEngine, D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM);

};

