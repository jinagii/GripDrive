#include "CVector.h"

CVector2::CVector2()
{
	x = 0;
	y = 0;
}

CVector2::CVector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

CVector2::~CVector2()
{

}

CVector2 CVector2::operator+(const CVector2& v)
{
	CVector2 _temp;

	_temp.x = this->x + v.x;
	_temp.y = this->y + v.y;

	return _temp;
}

CVector2 CVector2::operator-(const CVector2& v)
{
	CVector2 _temp;

	_temp.x = this->x - v.x;
	_temp.y = this->y - v.y;

	return _temp;
}

CVector2 CVector2::operator*(const float& f)
{
	CVector2 _temp;

	_temp.x = this->x * f;
	_temp.y = this->y * f;

	return _temp;
}

CVector2 CVector2::operator/(const float& f)
{
	CVector2 _temp;

	_temp.x = this->x / f;
	_temp.y = this->y / f;

	return _temp;
}

D2D1_VECTOR_2F CVector2::VectorPlusVector(D2D1_VECTOR_2F vec1, D2D1_VECTOR_2F vec2)
{
	D2D1_VECTOR_2F _temp;

	_temp.x = vec1.x + vec2.x;
	_temp.y = vec1.y + vec2.y;

	return _temp;
}

D2D1_VECTOR_2F CVector2::VectorMinus(D2D1_VECTOR_2F vec1, D2D1_VECTOR_2F vec2)
{
	D2D1_VECTOR_2F _temp;

	_temp.x = vec1.x - vec2.x;
	_temp.y = vec1.y - vec2.y;

	return _temp;
}

D2D1_VECTOR_2F CVector2::VectorMultiplyScalar(D2D1_VECTOR_2F vec1, float f)
{
	D2D1_VECTOR_2F _temp;

	_temp.x = vec1.x * f;
	_temp.y = vec1.y * f;

	return _temp;
}

float CVector2::VectorDotProduct(D2D1_VECTOR_2F vec1, D2D1_VECTOR_2F vec2)
{
	float _dotProduct = vec1.x * vec2.x + vec1.y * vec2.y;

	return _dotProduct;
}

D2D1_VECTOR_2F CVector2::VectorMultiplyMatrix(D2D1_VECTOR_2F vec, D2D1_MATRIX_3X2_F mat)
{
	D2D1_VECTOR_2F _temp;

	_temp.x = vec.x * mat.m11 + vec.y * mat.m12 + mat.dx;
	_temp.y = vec.x * mat.m21 + vec.y * mat.m22 + mat.dy;
	
	return _temp;
}

D2D1_VECTOR_2F CVector2::VectorMultiplyMatrix_DX(D2D1_VECTOR_2F vec, D2D1_MATRIX_3X2_F mat)
{
	D2D1_VECTOR_2F _temp;

	_temp.x = vec.x * mat.m11 + vec.y * mat.m21 + mat.dx;
	_temp.y = vec.x * mat.m12 + vec.y * mat.m22 + mat.dy;

	return _temp;
}

float CVector2::SizeOfVector(D2D1_VECTOR_2F vec)
{
	float _vecSize = sqrt(pow(vec.x, 2) + pow(vec.y, 2));

	return _vecSize;
}

D2D1_VECTOR_2F CVector2::VectorNormalize(D2D1_VECTOR_2F vec)
{
	D2D1_VECTOR_2F _temp;

	float _vecSize = sqrt(pow(vec.x, 2) + pow(vec.y, 2));

	_temp.x = vec.x / _vecSize;
	_temp.y = vec.y / _vecSize;

	return _temp;
}
