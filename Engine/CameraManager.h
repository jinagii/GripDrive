#pragma once

#include "GlobalDefine.h"
#include "CGameObject.h"

/// ī�޶� Ŭ����
/// 2021.02.22 JinHak

class CameraManager : public CGameObject
{
public:
	CameraManager();
	~CameraManager();

private:
	D2D1_VECTOR_2F m_Viewport;			// ī�޶��� ����. (�������� �����̸� Ratio�� ���� �޶����� �Ѵ�)
	float m_Scale;						// ī�޶��� ���� ����

	D2D1_MATRIX_3X2_F m_ViewTM;
	D2D1_MATRIX_3X2_F m_ProjTM;
	D2D1_MATRIX_3X2_F m_ViewProjTM;

public:
	void MoveHorizontal(float val);
	void MoveVertical(float val);
	void Zoom(float val);
	void ZoomAbs(float zoom);

	D2D1_MATRIX_3X2_F GetViewTM();		// ī�޶��� �� ����� �����Ѵ�.
	D2D1_MATRIX_3X2_F GetProjTM();		// ī�޶��� �������� ����� �����Ѵ�. 2D�̹Ƿ� Isometric�� ���缭 ������ ��ȯ�Ѵ�.
	D2D1_MATRIX_3X2_F GetViewProjTM();

	// ���̼Ҹ�Ʈ�� �並 ������ش�. (�������� ���), �簢���� ȸ���� ��, y������ ��׷��߸���.
	void MakeIsometricTM(D2D1_VECTOR_2F center, float sizeY, float angle);

	// ž ���� projTM�� ������ش�. (identity��)
	void MakeTopViewTM();
};

