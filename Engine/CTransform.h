#pragma once

#include "GlobalDefine.h"

enum eREFLECT_SIDE
{
	ORGIN = 0,
	HORIZONTAL,
	VERTICAL,
};

class CTransform
{
public:
	CTransform();
	~CTransform();

private:
	D2D_VECTOR_2F m_LocalPosition;
	float m_LocalRotation;
	D2D_VECTOR_2F m_LocalScale;

	// ��� ���е��� ���յ� ��� (����)
	D2D1_MATRIX_3X2_F m_TransformMatrix;
	// 3���� �������� �������� ��� (����)
	D2D1_MATRIX_3X2_F m_TranslateMatrix;		// �̵� ��� (����)
	D2D1_MATRIX_3X2_F m_RotationMatrix;			// ȸ�� ��� (����)
	D2D1_MATRIX_3X2_F m_ScaleMatrix;			// ũ�� ��ȯ ��� (����)

private:
	void MatirxIdentity(); // ������� �����

public:
	// ���� ��ǥ���� TRS�� �������� �����ϰ� �Ǵ� �Լ���
	void SetLocalPosition(D2D_VECTOR_2F pos);
	void SetLocalRotation(float rot);
	void SetLocalScale(D2D_VECTOR_2F scale);

	// ������Ʈ�� ���� ��ǥ�� ��ȯ�ϴ� �Լ���
	D2D_VECTOR_2F GetLocalPosition();
	float GetLocalRotation();
	D2D_VECTOR_2F GetLocalScale();

	// ������Ʈ�� ���������� ����� ��ǥ�� ��ȯ (����-��������-�� ������ �����Ѵ�)
	D2D_VECTOR_2F GetIsometricsPosition(D2D1_MATRIX_3X2_F projTM);

	// ������Ʈ�� ���� ��ǥ�� ��ȯ�ϴ� �Լ���
	D2D1_MATRIX_3X2_F GetWorldTransform();
	D2D_VECTOR_2F GetWorldPosition();
	float GetWorldRotation();
	D2D_VECTOR_2F GetWorldScale();

	// ���� ������ Input�� Ȱ���� �� �ִ� �Լ���

	// �ڱ� �ڽ��� �̵��ϱ�� ȸ���ϱ�
	void MoveByTranslate(D2D_VECTOR_2F translation);// (����)��ǥ�迡�� dx, dy��ŭ �̵�
	void TurnByRotation(float angle);				// (����)��ǥ�迡�� �ڱ� �ڽ��� �������� (������ǥ���� ������ �������� ȸ��)

public:
	// ��İ� ������ ����

public:
	static D2D1_MATRIX_3X2_F MakeRotateTransform(float angle, D2D_VECTOR_2F center);
	static D2D1_MATRIX_3X2_F MakeScaleTransform(float width, float height, D2D_VECTOR_2F center);
	static D2D1_MATRIX_3X2_F MakeReflectTransform(eREFLECT_SIDE side, D2D_VECTOR_2F center);


	/// ȸ��, ������ ������ �Ѵ�.
	static D2D1_MATRIX_3X2_F MakeComplexTransform(float angle, D2D_VECTOR_2F size, D2D_VECTOR_2F center);

	// ������Ʈ�� TRS�� �����Ͽ� SetTransform�� ����� ��ȯ�Ѵ�.
	D2D1_MATRIX_3X2_F GetTransformMatrix(D2D_VECTOR_2F pos, float angle, D2D_VECTOR_2F scale);
	D2D1_MATRIX_3X2_F GetTransformMatrix();

	/// �߸� �������.
	D2D1_MATRIX_3X2_F MakeScaleMatrix_byPoint(D2D_VECTOR_2F size, D2D_VECTOR_2F center);
	/// �߸� �������.
	D2D1_MATRIX_3X2_F MakeScaleMatrix_byPoint(float sizeX, float sizeY, float centerX, float centerY);

private:
public:

	static D2D1_MATRIX_3X2_F MatrixTranslation(D2D_VECTOR_2F center);
	static D2D1_MATRIX_3X2_F MatrixTranslation(float _x, float _y);
	static D2D1_MATRIX_3X2_F MatrixRotation (float _rad);
	static D2D1_MATRIX_3X2_F MatrixScale (float width, float height);

	static D2D1_POINT_2F VectorRotation(D2D1_VECTOR_2F center, D2D1_POINT_2F offset, float angle);
	//static	D2D1_VECTOR_2F VectorRotation(D2D1_VECTOR_2F vec, D2D1_POINT_2F offset, float angle);
	static D2D1_VECTOR_2F VectorRotation(D2D1_VECTOR_2F vec, float angle);
	static D2D1_VECTOR_2F VectorRotation(D2D1_VECTOR_2F vec, D2D1_MATRIX_3X2_F mat);
};

