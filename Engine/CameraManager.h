#pragma once

#include "GlobalDefine.h"
#include "CGameObject.h"

/// 카메라 클래스
/// 2021.02.22 JinHak

class CameraManager : public CGameObject
{
public:
	CameraManager();
	~CameraManager();

private:
	D2D1_VECTOR_2F m_Viewport;			// 카메라의 영역. (보여지는 영역이며 Ratio에 따라 달라져야 한다)
	float m_Scale;						// 카메라의 줌인 정도

	D2D1_MATRIX_3X2_F m_ViewTM;
	D2D1_MATRIX_3X2_F m_ProjTM;
	D2D1_MATRIX_3X2_F m_ViewProjTM;

public:
	void MoveHorizontal(float val);
	void MoveVertical(float val);
	void Zoom(float val);
	void ZoomAbs(float zoom);

	D2D1_MATRIX_3X2_F GetViewTM();		// 카메라의 뷰 행렬을 리턴한다.
	D2D1_MATRIX_3X2_F GetProjTM();		// 카메라의 프로젝션 행렬을 리턴한다. 2D이므로 Isometric에 맞춰서 적절히 변환한다.
	D2D1_MATRIX_3X2_F GetViewProjTM();

	// 아이소매트릭 뷰를 만들어준다. (프로젝션 행렬), 사각형을 회전한 뒤, y축으로 찌그러뜨린다.
	void MakeIsometricTM(D2D1_VECTOR_2F center, float sizeY, float angle);

	// 탑 뷰의 projTM을 만들어준다. (identity다)
	void MakeTopViewTM();
};

