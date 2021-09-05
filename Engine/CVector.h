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
	// 스칼라곱
	CVector2 operator*(const float& f);
	CVector2 operator/(const float& f);

	/// 벡터 연산들
	static D2D1_VECTOR_2F VectorPlusVector(D2D1_VECTOR_2F vec1, D2D1_VECTOR_2F vec2);
	static D2D1_VECTOR_2F VectorMinus(D2D1_VECTOR_2F vec1, D2D1_VECTOR_2F vec2);
	static D2D1_VECTOR_2F VectorMultiplyScalar(D2D1_VECTOR_2F vec1, float f);
	//static D2D1_VECTOR_2F VectorScalarPlus(D2D1_VECTOR_2F vec1, float f);
	// 내적
	static float VectorDotProduct(D2D1_VECTOR_2F vec1, D2D1_VECTOR_2F vec2);
	// 벡터 곱하기 행렬
	static D2D1_VECTOR_2F VectorMultiplyMatrix(D2D1_VECTOR_2F vec, D2D1_MATRIX_3X2_F mat);		// 수학함수처럼 행렬이 왼쪽에 온다.
	static D2D1_VECTOR_2F VectorMultiplyMatrix_DX(D2D1_VECTOR_2F vec, D2D1_MATRIX_3X2_F mat);	// DX는 행렬이 오른쪽에 온다.


	static float SizeOfVector(D2D1_VECTOR_2F vec);
	// normalize
	static D2D1_VECTOR_2F VectorNormalize(D2D1_VECTOR_2F vec);
};