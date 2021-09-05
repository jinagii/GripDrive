#include "CameraManager.h"
#include "CTransform.h"
#include "CVector.h"


CameraManager::CameraManager()
	: CGameObject(), m_Viewport(D2D1::Vector2F()),
	m_Scale(0)
{
	m_Transform = new CTransform();

}

CameraManager::~CameraManager()
{

}

void CameraManager::MoveHorizontal(float val)
{
	CGameObject::MoveHorizontal(val);
}

void CameraManager::MoveVertical(float val)
{
	CGameObject::MoveVertical(val);
}

// 증감분이다.
void CameraManager::Zoom(float val)
{
	m_Scale += val;
	CGameObject::ScaleObject(val);
}

// 절대값을 셋팅할 수 있는 함수도 만들어두자.
void CameraManager::ZoomAbs(float zoom)
{

}

D2D1_MATRIX_3X2_F CameraManager::GetViewTM()
{
	// 줌 값도 반대로 만들어줘야 하나, 카메라 자체는 줌 값이 의미가 없으므로 그냥 사용
	float _zoom = m_Transform->GetLocalScale().x;

	// 현재 카메라의 Transform의 반대로 뭔가를 만들어준다.
	// 이동 값을 만들어준다. (카메라의 위치와 원점대칭)
	D2D1_VECTOR_2F _reverseVec = CVector2::VectorMultiplyScalar(m_Transform->GetLocalPosition(), -1.0f);

	// 뷰 행렬을 만들어준다.
	m_ViewTM = m_Transform->MatrixTranslation(_reverseVec);

	// 줌 값을 곱해서 스케일 성분에 넣어준다.
	m_ViewTM._11 *= _zoom;
	m_ViewTM._22 *= _zoom;

	return m_ViewTM;
}

D2D1_MATRIX_3X2_F CameraManager::GetProjTM()
{
	return m_ProjTM;
}

D2D1_MATRIX_3X2_F CameraManager::GetViewProjTM()
{
	m_ViewProjTM = m_ProjTM * GetViewTM();

	return m_ViewProjTM;
}

void CameraManager::MakeIsometricTM(D2D1_VECTOR_2F center, float sizeY, float angle)
{
	D2D1_VECTOR_2F _sizeVec;
	_sizeVec.x = 1.0f;
	_sizeVec.y = 1.0f * sizeY;

	m_ProjTM = CTransform::MakeComplexTransform(angle, _sizeVec, center);
}

void CameraManager::MakeTopViewTM()
{
	m_ProjTM = D2D1::Matrix3x2F::Identity();
}
