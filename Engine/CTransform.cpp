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

	// TSR�� �̾ ��� ��� ������� �ʱ�ȭ���ش�.
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
	/// �������� ����� ���� ��ġ�� �ٲٵ���
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
	// 1. angle�� ���� ������ ��ȯ�Ѵ�.

	// 2.���� center�� ȸ���� �� ������ �ǹ��Ѵ�. ���� �����ϸ� pivot�� ��ġ��� ���� �ȴ�.
	// 2-1. center���� �����̵� ���(translate)�� center matrix�� �ϳ� �����.
	// 2-2. �̸� ���� ���� ���Ͱ� �ش� �������� �̵��Ѵ�.

	// 3. ��ȯ�� ���Ȱ��� ȸ�� ��Ŀ� ������ ������ rotate matrix�� �����.

	// 4. center�� ���� ���Ͱ� ���������Ƿ� �ʱⰪ���� �����ִ� orgin matrix�� �����.
	// 4-1. �̴� center �ٽ� �����̵� ��Ŀ��� center ������Ʈ�� ���ִ� ������ ���� �� �ִ�.

	// ���������� center matirx * rotate matrix * orgin matrix�� ���ؼ� ���� matirx�� ���Ѵ�.
	// �־��� ���� TargetRender�� SetTransform �Լ��� �Ķ���ͷ� ������ ȭ���� �������͸� ȸ����Ų��.

	float _rad = AngleToRad(angle);

	D2D1_MATRIX_3X2_F _translateToCenter = MatrixTranslation(center.x, center.y);
	D2D1_MATRIX_3X2_F _rotateOnCenter = MatrixRotation(_rad);
	D2D1_MATRIX_3X2_F _translateToOrgin = MatrixTranslation(-center.x, -center.y);

	// center translate�� ���� ������, ��ǻ�� ���忡���� �����ʺ��� �����ϹǷ� ù��°��.
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
	// HORIZONTAL, VERTICAL �� �� ����� ����
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

		// Ư�� ���� �������� ������ ��ȯ
		//D2D1_MATRIX_3X2_F _translateToOrgin = MatrixTranslation(center.x, center.y);
	D2D1_MATRIX_3X2_F _translateToOrgin = MatrixTranslation(-center.x, -center.y);

	// Ư�� ���� ������ ���¿��� ȸ��, ������
	D2D1_MATRIX_3X2_F _rotateOnCenter = MatrixRotation(_rad);
	D2D1_MATRIX_3X2_F _scaleOnCenter = MatrixScale(size.x, size.y);

	// ������ Ư�� ������ ������ �ٽ� ����
	//D2D1_MATRIX_3X2_F _translateToCenter = MatrixTranslation(-center.x, -center.y);
	D2D1_MATRIX_3X2_F _translateToCenter = MatrixTranslation(center.x, center.y);

	// center translate�� ���� ������, ��ǻ�� ���忡���� �����ʺ��� �����ϹǷ� ù��°��.
	D2D1_MATRIX_3X2_F _resultMatrix = _translateToOrgin * _rotateOnCenter * _scaleOnCenter * _translateToCenter;

	/*
	/// ��ȯ ����� ���� ���Ϳ� ���Ѵ�.
	D2D1_VECTOR_2F _MyPos = D2D1::Vector2F(100.f, 100.f);

	D2D1::Matrix3x2F

	D2D1_VECTOR_2F _ResultPos1 = _resultMatrix * _MyPos;	// 1. ����ó�� ���Ͱ� �ڿ� ����
	D2D1_VECTOR_2F _ResultPos2 = _MyPos * _resultMatrix; // 2. DX������ ���͸� �տ� ��ġ

	//D2D1_MATRIX_3X2_F _resultMatrix =  _rotateOnCenter * _translateToCenter;
	*/

	return _resultMatrix;
}

// �� ���� ��ġ, ��ü ȸ����, ��ü �����ϰ����� ��ȯ ���(Transform) ����
D2D1_MATRIX_3X2_F CTransform::GetTransformMatrix(D2D_VECTOR_2F pos, float angle, D2D_VECTOR_2F scale)
{
	float _rad = AngleToRad(angle);

	D2D1_MATRIX_3X2_F _ObjectTranslateMT = MatrixTranslation(pos);
	D2D1_MATRIX_3X2_F _ObjectRotateMT = MatrixRotation(_rad);
	D2D1_MATRIX_3X2_F _ObjectScaleMT = MatrixScale(scale.x, scale.y);

	D2D1_MATRIX_3X2_F _resultMT = _ObjectScaleMT * _ObjectRotateMT * _ObjectTranslateMT;

	return _resultMT;
}

// ������ ������ �ִ� ������ ����(T, R, S)���� ��ȯ ���(Transform) ����
D2D1_MATRIX_3X2_F CTransform::GetTransformMatrix()
{
	float _rad = AngleToRad(m_LocalRotation);

	D2D1_MATRIX_3X2_F _ObjectTraslateMT = MatrixTranslation(m_LocalPosition);
	D2D1_MATRIX_3X2_F _ObjectRotateMT = MatrixRotation(_rad);
	D2D1_MATRIX_3X2_F _ObjectScaleMT = MatrixScale(m_LocalScale.x, m_LocalScale.y);

	D2D1_MATRIX_3X2_F _resultMT = _ObjectScaleMT * _ObjectRotateMT * _ObjectTraslateMT;

	return _resultMT;
}

// Ư�� ���� ���ؼ� ũ�� ��ȯ
D2D1_MATRIX_3X2_F CTransform::MakeScaleMatrix_byPoint(D2D_VECTOR_2F size, D2D_VECTOR_2F center)
{
	/// �����̵�, ������, �������� 3�ܰ踦 ���ľ� ��

	D2D1_MATRIX_3X2_F _translateToOrgin = MatrixTranslation(center);
	D2D1_MATRIX_3X2_F _scaleOnCenter = MatrixScale(size.x, size.y);
	D2D1_MATRIX_3X2_F _translaterToCenter = MatrixTranslation(-center.x, -center.y);

	D2D1_MATRIX_3X2_F _resultMT = _translateToOrgin * _scaleOnCenter * _translaterToCenter;

	return _resultMT;

	// �ڵ带 �߸� § ����
	/*
	D2D1_MATRIX_3X2_F _complexTranslation;

	_complexTranslation.m11 = size.x;	_complexTranslation.m12 = 0.0f;
	_complexTranslation.m21 = 0.0f;			_complexTranslation.m22 = size.y ;
	_complexTranslation.dx = center.x;		_complexTranslation.dy = center.y;
	return _complexTranslation;

	*/
}

// Ư�� ���� ���ؼ� ũ�� ��ȯ2
D2D1_MATRIX_3X2_F CTransform::MakeScaleMatrix_byPoint(float sizeX, float sizeY, float centerX, float centerY)
{
	D2D1_MATRIX_3X2_F _translateToOrgin = MatrixTranslation(centerX, centerY);
	D2D1_MATRIX_3X2_F _scaleOnCenter = MatrixScale(sizeX, sizeY);
	D2D1_MATRIX_3X2_F _translaterToCenter = MatrixTranslation(-centerX, -centerY);

	D2D1_MATRIX_3X2_F _resultMT = _translateToOrgin * _scaleOnCenter * _translaterToCenter;

	return _resultMT;

	/// �����̵�, ������, �������� 3�ܰ踦 ���ľ� ��
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

	// ��������� ȸ���� ���� ���� (�Լ��� ��ã�Ƽ� �������)

	return vec;
}
*/
