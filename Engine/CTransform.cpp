#include "CTransform.h"

const float PI = 3.1415f;
#define AngleToRad(angle) (angle * PI / 180.0f)

CTransform::CTransform()
{
	D2D_VECTOR_2F _initializer;
	_initializer.x = 0.0f;
	_initializer.y = 0.0f;

	m_LocalPosition = _initializer;

	m_LocalRotation = 0.0f;

	_initializer.x = 1.0f;
	_initializer.y = 1.0f;

	m_LocalScale = _initializer;

	// TSR에 이어서 모든 행렬 멤버들을 초기화해준다.
	MatirxIdentity();
}
 
CTransform::~CTransform()
{
}

void CTransform::MatirxIdentity()
{
	m_TransformMatrix = D2D1::Matrix3x2F::Identity();
	m_TranslateMatrix = D2D1::Matrix3x2F::Identity();
	m_RotationMatrix = D2D1::Matrix3x2F::Identity();
	m_ScaleMatrix = D2D1::Matrix3x2F::Identity();
}

void CTransform::SetLocalPosition(D2D_VECTOR_2F pos)
{
	m_LocalPosition = pos;
	m_TransformMatrix = MakeComplexTransform(m_LocalRotation, m_LocalScale, m_LocalPosition);
}

void CTransform::SetLocalRotation(float rot)
{
	m_LocalRotation = rot;
	m_TransformMatrix = MakeComplexTransform(m_LocalRotation, m_LocalScale, m_LocalPosition);
}

void CTransform::SetLocalScale(D2D_VECTOR_2F scale)
{
	m_LocalScale = scale;
	m_TransformMatrix = MakeComplexTransform(m_LocalRotation, m_LocalScale, m_LocalPosition);
}

D2D_VECTOR_2F CTransform::GetLocalPosition()
{
	return m_LocalPosition;
}

float CTransform::GetLocalRotation()
{
	return m_LocalRotation;
}

D2D_VECTOR_2F CTransform::GetLocalScale()
{
	return m_LocalScale;
}

D2D_VECTOR_2F CTransform::GetIsometricsPosition(D2D1_MATRIX_3X2_F projTM)
{
	/// 프로젝션 행렬을 통해 위치만 바꾸도록
	D2D1::Matrix3x2F* _proj = D2D1::Matrix3x2F::ReinterpretBaseType(&projTM);
	D2D1_POINT_2F _offset = _proj->TransformPoint(D2D1::Point2F(m_LocalPosition.x, m_LocalPosition.y));

	return D2D1::Vector2F(_offset.x, _offset.y);
}

D2D1_MATRIX_3X2_F CTransform::GetWorldTransform()
{
	return m_TransformMatrix;
}

D2D_VECTOR_2F CTransform::GetWorldPosition()
{
	D2D1_MATRIX_3X2_F _transformMT = MakeComplexTransform(
		m_LocalRotation, m_LocalScale, m_LocalPosition);

	D2D_VECTOR_2F _result;
	_result.x = _transformMT.dx;
	_result.y = _transformMT.dy;

	return _result;
}

float CTransform::GetWorldRotation()
{
	D2D1_MATRIX_3X2_F _transformMT = MakeComplexTransform(
		m_LocalRotation, m_LocalScale, m_LocalPosition);

	return acosf(_transformMT.m11) * (180.0f / 3.1415f);
}

D2D_VECTOR_2F CTransform::GetWorldScale()
{
	D2D1_MATRIX_3X2_F _transformMT = MakeComplexTransform(
		m_LocalRotation, m_LocalScale, m_LocalPosition);

	D2D_VECTOR_2F _scale;
	_scale.x = sqrtf(powf(_transformMT.m11, 2.0f) + powf(_transformMT.m12, 2.0f));
	_scale.y = sqrtf(powf(_transformMT.m21, 2.0f) + powf(_transformMT.m22, 2.0f));

	return _scale;
}

void CTransform::MoveByTranslate(D2D_VECTOR_2F translation)
{
	m_LocalPosition.x += translation.x;
	m_LocalPosition.y += translation.y;

	m_TransformMatrix = GetTransformMatrix(m_LocalPosition, m_LocalRotation, m_LocalScale);
}

void CTransform::TurnByRotation(float angle)
{
	m_LocalRotation += angle;
	
	if (m_LocalRotation >= 360)
	{
		m_LocalRotation = m_LocalRotation - 360;
	}	
	if (m_LocalRotation < 0)
	{
		m_LocalRotation = m_LocalRotation + 360;
	}
	
	m_TransformMatrix = GetTransformMatrix(m_LocalPosition, m_LocalRotation, m_LocalScale);
}

D2D1_MATRIX_3X2_F CTransform::MakeRotateTransform(float angle, D2D_VECTOR_2F center)
{
	// 1. angle을 라디안 값으로 변환한다.

	// 2.인자 center는 회전을 할 지점을 의미한다. 쉽게 생각하면 pivot의 위치라고 보면 된다.
	// 2-1. center값과 평행이동 행렬(translate)로 center matrix를 하나 만든다.
	// 2-2. 이를 통해 기저 벡터가 해당 지점으로 이동한다.

	// 3. 변환된 라디안값을 회전 행렬에 대입한 값으로 rotate matrix를 만든다.

	// 4. center로 기저 벡터가 움직였으므로 초기값으로 돌려주는 orgin matrix를 만든다.
	// 4-1. 이는 center 다시 평행이동 행렬에서 center 엘리멘트를 빼주는 것으로 구할 수 있다.

	// 마지막으로 center matirx * rotate matrix * orgin matrix를 구해서 최종 matirx를 구한다.
	// 주어진 값은 TargetRender의 SetTransform 함수에 파라미터로 전달해 화면의 기저벡터를 회전시킨다.

	float _rad = AngleToRad(angle);

	D2D1_MATRIX_3X2_F _translateToCenter = MatrixTranslation(center.x, center.y);
	D2D1_MATRIX_3X2_F _rotateOnCenter = MatrixRotation(_rad);
	D2D1_MATRIX_3X2_F _translateToOrgin = MatrixTranslation(-center.x, -center.y);

	// center translate가 끝에 들어가지만, 컴퓨터 입장에서는 오른쪽부터 연산하므로 첫번째다.
	D2D1_MATRIX_3X2_F _resultMatrix = _translateToOrgin * _rotateOnCenter * _translateToCenter;

	return _resultMatrix;
}


D2D1_MATRIX_3X2_F CTransform::MakeScaleTransform(float width, float height, D2D_VECTOR_2F center)
{

	D2D1_MATRIX_3X2_F _translateToCenter = MatrixTranslation(center.x, center.y);
	D2D1_MATRIX_3X2_F _scaleTransform = MatrixScale(width, height);
	D2D1_MATRIX_3X2_F _translateToOrgin = MatrixTranslation(-center.x, -center.y);

	D2D1_MATRIX_3X2_F _resultMatrix = _translateToOrgin * _scaleTransform * _translateToCenter;

	return _resultMatrix;
}

D2D1_MATRIX_3X2_F CTransform::MakeReflectTransform(eREFLECT_SIDE side, D2D_VECTOR_2F center)
{
	// HORIZONTAL, VERTICAL 둘 다 적용된 상태
	if (side == eREFLECT_SIDE::ORGIN)
	{
		D2D1_MATRIX_3X2_F _translateToCenter = MatrixTranslation(center.x, center.y);

		D2D1_MATRIX_3X2_F _reflectMatrix;

		_reflectMatrix.m11 = -1.0f;		_reflectMatrix.m12 = 0.0f;
		_reflectMatrix.m21 = 0.0f;		_reflectMatrix.m22 = -1.0f;
		_reflectMatrix.dx = center.x;	 _reflectMatrix.dy = center.y;

		D2D1_MATRIX_3X2_F _translateToOrgin = MatrixTranslation(-center.x, -center.y);
		D2D1_MATRIX_3X2_F _resultMatrix = _translateToOrgin * _reflectMatrix * _translateToCenter;

		return _resultMatrix;
	}

	if (side == eREFLECT_SIDE::HORIZONTAL)
	{
		D2D1_MATRIX_3X2_F _translateToCenter = MatrixTranslation(center.x, center.y);

		D2D1_MATRIX_3X2_F _reflectMatrix;

		_reflectMatrix.m11 = 1.0f;		_reflectMatrix.m12 = 0.0f;
		_reflectMatrix.m21 = 0.0f;		_reflectMatrix.m22 = -1.0f;
		_reflectMatrix.dx = center.x;	 _reflectMatrix.dy = center.y;

		D2D1_MATRIX_3X2_F _translateToOrgin = MatrixTranslation(-center.x, -center.y);
		D2D1_MATRIX_3X2_F _resultMatrix = _translateToOrgin * _reflectMatrix * _translateToCenter;

		return _resultMatrix;
	}

	if (side == eREFLECT_SIDE::VERTICAL)
	{

		D2D1_MATRIX_3X2_F _translateToCenter = MatrixTranslation(center.x, center.y);

		D2D1_MATRIX_3X2_F _reflectMatrix;

		_reflectMatrix.m11 = -1.0f;		_reflectMatrix.m12 = 0.0f;
		_reflectMatrix.m21 = 0.0f;		_reflectMatrix.m22 = 1.0f;
		_reflectMatrix.dx = center.x;	 _reflectMatrix.dy = center.y;

		D2D1_MATRIX_3X2_F _translateToOrgin = MatrixTranslation(-center.x, -center.y);
		D2D1_MATRIX_3X2_F _resultMatrix = _translateToOrgin * _reflectMatrix * _translateToCenter;

		return _resultMatrix;
	}
}

D2D1_MATRIX_3X2_F CTransform::MakeComplexTransform(float angle, D2D_VECTOR_2F size, D2D_VECTOR_2F center)
{
	float _rad = AngleToRad(angle);

	// 	D2D1_MATRIX_3X2_F _translateToCenter = ComplexTranslation(size.x, size.y, center.x, center.y);
	// 	D2D1_MATRIX_3X2_F _rotateOnCenter = MatrixRotation(_rad);
	// 	D2D1_MATRIX_3X2_F _translateToOrgin = ComplexTranslation(size.x, size.y, -center.x, -center.y);

		// 특정 점이 원점으로 가도록 변환
		//D2D1_MATRIX_3X2_F _translateToOrgin = MatrixTranslation(center.x, center.y);
	D2D1_MATRIX_3X2_F _translateToOrgin = MatrixTranslation(-center.x, -center.y);

	// 특정 점이 원점인 상태에서 회전, 스케일
	D2D1_MATRIX_3X2_F _rotateOnCenter = MatrixRotation(_rad);
	D2D1_MATRIX_3X2_F _scaleOnCenter = MatrixScale(size.x, size.y);

	// 원점이 특정 점으로 가도록 다시 복구
	//D2D1_MATRIX_3X2_F _translateToCenter = MatrixTranslation(-center.x, -center.y);
	D2D1_MATRIX_3X2_F _translateToCenter = MatrixTranslation(center.x, center.y);

	// center translate가 끝에 들어가지만, 컴퓨터 입장에서는 오른쪽부터 연산하므로 첫번째다.
	D2D1_MATRIX_3X2_F _resultMatrix = _translateToOrgin * _rotateOnCenter * _scaleOnCenter * _translateToCenter;

	/*
	/// 변환 행렬을 직접 벡터에 곱한다.
	D2D1_VECTOR_2F _MyPos = D2D1::Vector2F(100.f, 100.f);

	D2D1::Matrix3x2F

	D2D1_VECTOR_2F _ResultPos1 = _resultMatrix * _MyPos;	// 1. 수학처럼 벡터가 뒤에 나옴
	D2D1_VECTOR_2F _ResultPos2 = _MyPos * _resultMatrix; // 2. DX에서는 벡터를 앞에 배치

	//D2D1_MATRIX_3X2_F _resultMatrix =  _rotateOnCenter * _translateToCenter;
	*/

	return _resultMatrix;
}

// 한 번에 위치, 자체 회전값, 자체 스케일값으로 변환 행렬(Transform) 조합
D2D1_MATRIX_3X2_F CTransform::GetTransformMatrix(D2D_VECTOR_2F pos, float angle, D2D_VECTOR_2F scale)
{
	float _rad = AngleToRad(angle);

	D2D1_MATRIX_3X2_F _ObjectTranslateMT = MatrixTranslation(pos);
	D2D1_MATRIX_3X2_F _ObjectRotateMT = MatrixRotation(_rad);
	D2D1_MATRIX_3X2_F _ObjectScaleMT = MatrixScale(scale.x, scale.y);

	D2D1_MATRIX_3X2_F _resultMT = _ObjectScaleMT * _ObjectRotateMT * _ObjectTranslateMT;

	return _resultMT;
}

// 기존에 가지고 있던 각각의 성분(T, R, S)으로 변환 행렬(Transform) 조합
D2D1_MATRIX_3X2_F CTransform::GetTransformMatrix()
{
	float _rad = AngleToRad(m_LocalRotation);

	D2D1_MATRIX_3X2_F _ObjectTraslateMT = MatrixTranslation(m_LocalPosition);
	D2D1_MATRIX_3X2_F _ObjectRotateMT = MatrixRotation(_rad);
	D2D1_MATRIX_3X2_F _ObjectScaleMT = MatrixScale(m_LocalScale.x, m_LocalScale.y);

	D2D1_MATRIX_3X2_F _resultMT = _ObjectScaleMT * _ObjectRotateMT * _ObjectTraslateMT;

	return _resultMT;
}

// 특정 점에 대해서 크기 변환
D2D1_MATRIX_3X2_F CTransform::MakeScaleMatrix_byPoint(D2D_VECTOR_2F size, D2D_VECTOR_2F center)
{
	/// 윈점이동, 스케일, 원점복구 3단계를 거쳐야 함

	D2D1_MATRIX_3X2_F _translateToOrgin = MatrixTranslation(center);
	D2D1_MATRIX_3X2_F _scaleOnCenter = MatrixScale(size.x, size.y);
	D2D1_MATRIX_3X2_F _translaterToCenter = MatrixTranslation(-center.x, -center.y);

	D2D1_MATRIX_3X2_F _resultMT = _translateToOrgin * _scaleOnCenter * _translaterToCenter;

	return _resultMT;

	// 코드를 잘못 짠 흔적
	/*
	D2D1_MATRIX_3X2_F _complexTranslation;

	_complexTranslation.m11 = size.x;	_complexTranslation.m12 = 0.0f;
	_complexTranslation.m21 = 0.0f;			_complexTranslation.m22 = size.y ;
	_complexTranslation.dx = center.x;		_complexTranslation.dy = center.y;
	return _complexTranslation;

	*/
}

// 특정 점에 대해서 크기 변환2
D2D1_MATRIX_3X2_F CTransform::MakeScaleMatrix_byPoint(float sizeX, float sizeY, float centerX, float centerY)
{
	D2D1_MATRIX_3X2_F _translateToOrgin = MatrixTranslation(centerX, centerY);
	D2D1_MATRIX_3X2_F _scaleOnCenter = MatrixScale(sizeX, sizeY);
	D2D1_MATRIX_3X2_F _translaterToCenter = MatrixTranslation(-centerX, -centerY);

	D2D1_MATRIX_3X2_F _resultMT = _translateToOrgin * _scaleOnCenter * _translaterToCenter;

	return _resultMT;

	/// 윈점이동, 스케일, 원점복구 3단계를 거쳐야 함
	/*
	D2D1_MATRIX_3X2_F _complexTranslation;

	_complexTranslation.m11 = sizeX;	_complexTranslation.m12 = 0.0f;
	_complexTranslation.m21 = 0.0f;			_complexTranslation.m22 = sizeY;
	_complexTranslation.dx = centerX;		_complexTranslation.dy = centerY;
	return _complexTranslation;
	*/
}

D2D1_MATRIX_3X2_F CTransform::MatrixTranslation(D2D_VECTOR_2F center)
{
	D2D1_MATRIX_3X2_F _translation;

	_translation.m11 = 1.0f;	_translation.m12 = 0.0f;
	_translation.m21 = 0.0f;	_translation.m22 = 1.0f;
	_translation.dx = center.x; _translation.dy = center.y;

	return _translation;
}

D2D1_MATRIX_3X2_F CTransform::MatrixTranslation(float _x, float _y)
{
	D2D1_MATRIX_3X2_F _translation;

	_translation.m11 = 1.0f;	_translation.m12 = 0.0f;
	_translation.m21 = 0.0f;	_translation.m22 = 1.0f;
	_translation.dx = _x; _translation.dy = _y;

	return _translation;
}

D2D1_MATRIX_3X2_F CTransform::MatrixRotation(float _rad)
{
	D2D1_MATRIX_3X2_F _rotation;

	_rotation.m11 = cos(_rad);	 _rotation.m12 = sin(_rad);
	_rotation.m21 = -sin(_rad);  _rotation.m22 = cos(_rad);
	_rotation.dx = 0.0f;		 _rotation.dy = 0.0f;

	return _rotation;
}

D2D1_MATRIX_3X2_F CTransform::MatrixScale(float width, float height)
{
	D2D1_MATRIX_3X2_F _scale;

	_scale.m11 = width;			_scale.m12 = 0.0f;
	_scale.m21 = 0.0f;			_scale.m22 = height;
	_scale.dx = 0.0f;			_scale.dy = 0.0f;

	return _scale;
}

D2D1_VECTOR_2F CTransform::VectorRotation(D2D1_VECTOR_2F vec, float angle)
{
	
	D2D1_MATRIX_3X2_F _rotationMatrix = D2D1::Matrix3x2F::Rotation(angle);

	vec.x = vec.x * _rotationMatrix.m11 + vec.y * _rotationMatrix.m12;
	vec.y = vec.x * _rotationMatrix.m21 + vec.y * _rotationMatrix.m22;
	
	return vec;
}

D2D1_POINT_2F CTransform::VectorRotation(D2D1_VECTOR_2F center, D2D1_POINT_2F offset, float angle)
{	
	D2D1::Matrix3x2F _rotationMatrix = D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(center.x, center.y));
	D2D1_POINT_2F _Center = _rotationMatrix.TransformPoint(D2D1::Point2F(center.x + offset.x, center.y + offset.y));

	return _Center;
}

D2D1_VECTOR_2F CTransform::VectorRotation(D2D1_VECTOR_2F vec, D2D1_MATRIX_3X2_F mat)
{
	vec.x = vec.x * mat.m11 + vec.y * mat.m12;
	vec.y = vec.x * mat.m21 + vec.y * mat.m22;

	return vec;
}




/*

D2D1_VECTOR_2F CTransform::VectorRotation(D2D1_VECTOR_2F vec, float angle)
{
	D2D1_MATRIX_3X2_F _ObjectTraslateMT = D2D1::Matrix3x2F::Translation(vec.x, vec.y);
	D2D1_MATRIX_3X2_F _ObjectRotateMT = D2D1::Matrix3x2F::Rotation(angle);
	D2D1_MATRIX_3X2_F _ObjcetToOrigin = D2D1::Matrix3x2F::Translation(-vec.x, -vec.y);

	D2D1_MATRIX_3X2_F _resultMT = _ObjcetToOrigin  * _ObjectRotateMT * _ObjectTraslateMT;


	vec.x = vec.x * _resultMT.m11 + vec.y * _resultMT.m12;
	vec.y = vec.x * _resultMT.m21 + vec.y * _resultMT.m22;

	// 결과적으로 회전된 전방 벡터 (함수를 못찾아서 만들었다)

	return vec;
}
*/
