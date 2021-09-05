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

	// 모든 성분들이 조합된 행렬 (로컬)
	D2D1_MATRIX_3X2_F m_TransformMatrix;
	// 3개의 성분으로 나누어진 행렬 (로컬)
	D2D1_MATRIX_3X2_F m_TranslateMatrix;		// 이동 행렬 (로컬)
	D2D1_MATRIX_3X2_F m_RotationMatrix;			// 회전 행렬 (로컬)
	D2D1_MATRIX_3X2_F m_ScaleMatrix;			// 크기 변환 행렬 (로컬)

private:
	void MatirxIdentity(); // 단위행렬 만들기

public:
	// 로컬 좌표계의 TRS를 직접으로 조절하게 되는 함수들
	void SetLocalPosition(D2D_VECTOR_2F pos);
	void SetLocalRotation(float rot);
	void SetLocalScale(D2D_VECTOR_2F scale);

	// 오브젝트의 로컬 좌표를 반환하는 함수들
	D2D_VECTOR_2F GetLocalPosition();
	float GetLocalRotation();
	D2D_VECTOR_2F GetLocalScale();

	// 오브젝트의 프로젝션이 적용된 좌표를 반환 (로컬-프로젝션-뷰 순서로 적용한다)
	D2D_VECTOR_2F GetIsometricsPosition(D2D1_MATRIX_3X2_F projTM);

	// 오브젝트의 월드 좌표를 반환하는 함수들
	D2D1_MATRIX_3X2_F GetWorldTransform();
	D2D_VECTOR_2F GetWorldPosition();
	float GetWorldRotation();
	D2D_VECTOR_2F GetWorldScale();

	// 게임 내에서 Input을 활용할 수 있는 함수들

	// 자기 자신의 이동하기와 회전하기
	void MoveByTranslate(D2D_VECTOR_2F translation);// (로컬)좌표계에서 dx, dy만큼 이동
	void TurnByRotation(float angle);				// (로컬)좌표계에서 자기 자신을 기준으로 (로컬좌표계의 원점을 기준으로 회전)

public:
	// 행렬과 벡터의 곱셈

public:
	static D2D1_MATRIX_3X2_F MakeRotateTransform(float angle, D2D_VECTOR_2F center);
	static D2D1_MATRIX_3X2_F MakeScaleTransform(float width, float height, D2D_VECTOR_2F center);
	static D2D1_MATRIX_3X2_F MakeReflectTransform(eREFLECT_SIDE side, D2D_VECTOR_2F center);


	/// 회전, 스케일 순서로 한다.
	static D2D1_MATRIX_3X2_F MakeComplexTransform(float angle, D2D_VECTOR_2F size, D2D_VECTOR_2F center);

	// 오브젝트의 TRS를 참조하여 SetTransform할 행렬을 반환한다.
	D2D1_MATRIX_3X2_F GetTransformMatrix(D2D_VECTOR_2F pos, float angle, D2D_VECTOR_2F scale);
	D2D1_MATRIX_3X2_F GetTransformMatrix();

	/// 잘못 만들었다.
	D2D1_MATRIX_3X2_F MakeScaleMatrix_byPoint(D2D_VECTOR_2F size, D2D_VECTOR_2F center);
	/// 잘못 만들었다.
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

