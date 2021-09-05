#pragma once

#include "GlobalDefine.h"

class CVector2
{
public:
	CVector2();
	CVector2(float x, float y);
	~CVector2();

public:
	float x;
	float y;

	CVector2 operator+(const CVector2& v);
	CVector2 operator-(const CVector2& v);
	// ��Į���
	CVector2 operator*(const float& f);
	CVector2 operator/(const float& f);

	/// ���� �����
	static D2D1_VECTOR_2F VectorPlusVector(D2D1_VECTOR_2F vec1, D2D1_VECTOR_2F vec2);
	static D2D1_VECTOR_2F VectorMinus(D2D1_VECTOR_2F vec1, D2D1_VECTOR_2F vec2);
	static D2D1_VECTOR_2F VectorMultiplyScalar(D2D1_VECTOR_2F vec1, float f);
	//static D2D1_VECTOR_2F VectorScalarPlus(D2D1_VECTOR_2F vec1, float f);
	// ����
	static float VectorDotProduct(D2D1_VECTOR_2F vec1, D2D1_VECTOR_2F vec2);
	// ���� ���ϱ� ���
	static D2D1_VECTOR_2F VectorMultiplyMatrix(D2D1_VECTOR_2F vec, D2D1_MATRIX_3X2_F mat);		// �����Լ�ó�� ����� ���ʿ� �´�.
	static D2D1_VECTOR_2F VectorMultiplyMatrix_DX(D2D1_VECTOR_2F vec, D2D1_MATRIX_3X2_F mat);	// DX�� ����� �����ʿ� �´�.


	static float SizeOfVector(D2D1_VECTOR_2F vec);
	// normalize
	static D2D1_VECTOR_2F VectorNormalize(D2D1_VECTOR_2F vec);
};