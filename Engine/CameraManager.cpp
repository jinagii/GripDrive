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

// �������̴�.
void CameraManager::Zoom(float val)
{
	m_Scale += val;
	CGameObject::ScaleObject(val);
}

// ���밪�� ������ �� �ִ� �Լ��� ��������.
void CameraManager::ZoomAbs(float zoom)
{

}

D2D1_MATRIX_3X2_F CameraManager::GetViewTM()
{
	// �� ���� �ݴ�� �������� �ϳ�, ī�޶� ��ü�� �� ���� �ǹ̰� �����Ƿ� �׳� ���
	float _zoom = m_Transform->GetLocalScale().x;

	// ���� ī�޶��� Transform�� �ݴ�� ������ ������ش�.
	// �̵� ���� ������ش�. (ī�޶��� ��ġ�� ������Ī)
	D2D1_VECTOR_2F _reverseVec = CVector2::VectorMultiplyScalar(m_Transform->GetLocalPosition(), -1.0f);

	// �� ����� ������ش�.
	m_ViewTM = m_Transform->MatrixTranslation(_reverseVec);

	// �� ���� ���ؼ� ������ ���п� �־��ش�.
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
