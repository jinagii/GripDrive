#include "CarMK2.h"
#include "CABON_Engine.h"
#include "CVector.h"
#include "CTransform.h"
#include "CCircle.h"
#include "Trail.h"
#include "Smoke.h"
#include "atlstr.h"

CarMK2::CarMK2()
{

}

CarMK2::CarMK2(CABON_Engine* pEngine)
{
	m_bitmap = pEngine->LoadBitmapByPath(L"./data/60mini.png");
	m_Trail_TailRight = new Trail();

	m_Smoke = new Smoke();
	m_Transform = new CTransform();
}

CarMK2::CarMK2(CABON_Engine* pEngine, D2D1_RECT_F _rect)
{
	m_bitmap = pEngine->LoadBitmapByPath(L"./data/Car360.png");

	for (int i = 0; i < 36; i++)
	{
		CString str;

		str.Format(_T("./data/sprite/%d.png"), i);

		ID2D1Bitmap* _bitmap = pEngine->LoadBitmapByPath(str);

		v_Sprite.push_back(_bitmap);
}

	m_Transform = new CTransform();
	//m_Collider = new CAABB(_rect);

	D2D1_VECTOR_2F _center;

	_center.x = (_rect.right + _rect.left) / 2;
	_center.y = ((_rect.bottom + _rect.top) / 2) + 100;

	m_Transform->SetLocalPosition(_center);

	m_DriftCoef = 1.0f;
	m_FrictionCoef = 0.03f;
	m_Height = 100;
	m_Width = 60;

	// ����Ʈ �ʱ�ȭ
	m_Trail_TailRight = new Trail();
	m_Trail_TailLeft = new Trail();

	m_Smoke = new Smoke();

	// ��ƼŬ ������Ʈ Ǯ�� ��� ����� �� ������ ����
	m_Smoke->IntializeParticle(100);
}

CarMK2::CarMK2(CABON_Engine* pEngine, D2D1_VECTOR_2F center)
{
	m_bitmap = pEngine->LoadBitmapByPath(L"./data/Car360.png");

	for (int i = 0; i < 36; i++)
	{
		CString str;

		str.Format(_T("./data/sprite/%d.png"), i);

		ID2D1Bitmap* _bitmap = pEngine->LoadBitmapByPath(str);

		v_Sprite.push_back(_bitmap);
	}

	// transform ���� new
	m_Transform = new CTransform();
	m_Transform->SetLocalPosition(center);

	// �������� �ʱ�ȭ
	m_DriftCoef = 1.0f;
	m_FrictionCoef = 0.03f;

	// �浹 �ݰ�
	m_ColliderRadius = 70;
	//m_Collider = new CCircle(center, m_ColliderRadius);

	// ����Ʈ �ʱ�ȭ
	m_Trail_TailRight = new Trail();
	m_Trail_TailLeft = new Trail();

	m_Smoke = new Smoke();

	// ��ƼŬ ������Ʈ Ǯ�� ��� ����� �� ������ ����
	m_Smoke->IntializeParticle(100);
}

CarMK2::~CarMK2()
{
	delete m_Smoke;
	delete m_Collider;
	delete m_Transform;
	delete m_Trail_TailRight;
	delete m_Trail_TailLeft;
}

void CarMK2::Update()
{
	// ���� ��ġ���� �ణ �������ش�.
	D2D1_VECTOR_2F _nowPos = m_Transform->GetLocalPosition();
	FLOAT _nowRot = m_Transform->GetLocalRotation();




	/// Ʈ���� ��� ����
	if (CVector2::SizeOfVector(m_FrictionVector) > 1)
	{
		m_Trail_TailRight->CheckPosition(_nowPos, D2D1::Point2F(10, 10), _nowRot);
		m_Trail_TailLeft->CheckPosition(_nowPos, D2D1::Point2F(-10, 10), _nowRot);
	}



	/// �ڵ��� ��Ⱑ��
	if (CVector2::SizeOfVector(m_ForwardVector) > 1)
	{
		m_Smoke->PlayParticle(_nowPos, D2D1::Point2F(0, 20), _nowRot);
	}



	/// ������ �ʱ�ȭ
	m_Acceleration = 0;

	CGameObject::Update();
}


FLOAT CarMK2::GetVelocity()
{
	float _vecSize = sqrt(pow(m_Velocity.x, 2) + pow(m_Velocity.y, 2));

	return _vecSize;
}

void CarMK2::MoveHorizontal(float val)
{
	CGameObject::MoveHorizontal(val);
}

void CarMK2::MoveVertical(float val)
{
	CGameObject::MoveVertical(val);
}

void CarMK2::MoveForward(float speed)
{
	/*D2D1_VECTOR_2F _vec = m_Transform->GetLocalPosition();

	D2D1_POINT_2F _point = D2D1::Point2F(_vec.x, _vec.y);

	m_Trail->AddTrail(_point);*/

	// �ڽ��� ���� ���� (�� ������ ��Ÿ���� ����)�� ��� �˾Ƴ���?

	// �ڵ����� ���� ���� �ִٰ� ������ ������ (������ǥ�� ����. �� ������ ���� ����)
	D2D_VECTOR_2F _upVec = D2D1::Vector2F(0, -1.f * speed);

	// �����͸� ȸ���� ���
	D2D1::Matrix3x2F _rotationMatrix =
		D2D1::Matrix3x2F::Rotation(-1.0f * m_Transform->GetLocalRotation());

	// ��������� ȸ���� ���� ���� (�Լ��� ��ã�Ƽ� �������)
	m_ForwardVector = CVector2::VectorMultiplyMatrix_Math(_upVec, _rotationMatrix);

	/// ���溤���� ũ�⸦ �����Ѵ�

	// ���溤�ͷ� �̵��Ѵ�.
	m_Transform->MoveByTranslate(m_ForwardVector);
}

void CarMK2::DoAccelerate(float accelInc)
{
	m_Acceleration = accelInc;
}

void CarMK2::VelocityUpdate(float dTime)
{
	/// ���� - ���溤�͸� �̿��ؼ� �̵��� �Ѵ�. (MoveForwardVec)

	// �ڽ��� ���� ���� (�� ������ ��Ÿ���� ����)�� ��� �˾Ƴ���?
	// �ڵ����� ���� ���� �ִٰ� ������ ������ (������ǥ�� ����. �� ������ ���� ����)
	m_UpVector = D2D1::Vector2F(0, -1.0f);

	// �����͸� ȸ���� ���
	D2D1::Matrix3x2F _rotationMatrix = D2D1::Matrix3x2F::Rotation(-1.f * m_Transform->GetLocalRotation());

	// ��������� ȸ���� ���� ���� ���� == ���⺤��
	D2D1_VECTOR_2F _ForwardVecNor = CVector2::VectorMultiplyMatrix_Math(m_UpVector, _rotationMatrix);

	// ���ӷ��� ���� ���� ���̰� �ִ� ���溤��
	m_ForwardVector = CVector2::VectorScalarMultiply(_ForwardVecNor, m_Acceleration);

	// ���� ���� ���� ���� (�ӷ� & ����)
	m_Velocity = CVector2::VectorPlus(m_Velocity, m_ForwardVector);

	// �����º���
	///_ForwardVecNor; // ���⺤��

	float _dotScalar = CVector2::VectorDotProduct(m_Velocity, _ForwardVecNor);
	D2D1_VECTOR_2F _forwardOrtho = CVector2::VectorScalarMultiply(_ForwardVecNor, _dotScalar);

	m_FrictionVector = CVector2::VectorMinus(m_Velocity, _forwardOrtho);
	m_FrictionVector = CVector2::VectorScalarMultiply(m_FrictionVector, m_FrictionCoef);	/// ���� �������

	// �������� �� �ش�.
	m_Velocity = CVector2::VectorMinus(m_Velocity, m_FrictionVector);

	/*
	m_FrictionVector = CVector2::VectorPlus(m_FrictionVector, m_ForwardVector);
	D2D1_VECTOR_2F _normalizedFrictionVector = CVector2::VectorNormalize(m_FrictionVector);

	D2D1_VECTOR_2F _moveVec = CVector2::VectorScalarMultiply(m_Velocity, dTime);

	float _velocitySize = sqrt(pow(m_Velocity.x, 2) + pow(m_Velocity.y, 2));
	if (_velocitySize > 0)
	{
		m_Velocity = CVector2::VectorScalarMultiply(m_Velocity, 0.99f);
		m_FrictionVector = CVector2::VectorScalarMultiply(m_FrictionVector, 0.99f);
	}
	*/

	//_moveVec = CVector2::VectorPlus(CVector2::VectorScalarMultiply(m_FrictionVector, 0.1f), _moveVec);


	// �̵� ���� ����
	D2D1_VECTOR_2F _moveVec = CVector2::VectorScalarMultiply(m_Velocity, dTime);

	// �ӵ� ���ͷ� �̵��Ѵ�.
	m_Transform->MoveByTranslate(_moveVec);
}

void CarMK2::DoDrift(float val)
{
	/// �ڵ����� �պκ��� ȸ�����Ѽ� �帮��Ʈ 
	//D2D_VECTOR_2F _upVec = D2D1::Vector2F(0, -1.f * m_Speed);

	D2D1::Matrix3x2F _driftMatrix =
		D2D1::Matrix3x2F::Rotation(
			val * m_Transform->GetLocalRotation(),
			D2D1::Point2F(m_Transform->GetLocalPosition().x, m_Transform->GetLocalPosition().y - 50));

	m_ForwardVector.x = m_UpVector.x * _driftMatrix.m11 + m_UpVector.y * _driftMatrix.m12;
	m_ForwardVector.y = m_UpVector.x * _driftMatrix.m21 + m_UpVector.y * _driftMatrix.m22;

	m_Transform->MoveByTranslate(m_ForwardVector);

	/// �̷��� �ϴ°ǰ�
	/*m_DriftCoef = m_Transform->GetLocalRotation();

	m_DriftCoef += val;

	m_Transform->GetTransformMatrix(
		D2D1::Vector2F(m_Transform->GetLocalPosition().x, m_Transform->GetLocalPosition().y - 50),
		m_DriftCoef,
		m_Transform->GetLoacalScale());*/
}

void CarMK2::FrictionByVectorSize()
{
	m_Velocity = CVector2::VectorScalarMultiply(m_Velocity, 0.99f/*�ӽø�����*/);
}

//void CarMK2::FrictionOn(float val)
//{
//	if (m_Speed > 30)
//	{
//		m_Speed = m_Speed - val;
//	}
//	else if (m_Speed > 0 && m_Speed < 30)
//	{
//		m_Speed = m_Speed - 2 * val;
//	}
//	else
//	{
//		m_Speed = 0;
//	}
//}

void CarMK2::VectorDotProduct_SideFriction()
{
	// �ӵ� ���Ϳ� ���� ������ dot�������� �ӵ� ���͸� �������ش�
	float _dotProduct = CVector2::VectorDotProduct(m_ForwardVector, m_Velocity);

	// ���� ������
	float _forwadVecSize = sqrt(pow(m_ForwardVector.x, 2) + pow(m_ForwardVector.y, 2));
	float _velocitySize = sqrt(pow(m_Velocity.x, 2) + pow(m_Velocity.y, 2));

	// �ڻ��� �� & ����
	float _cosTheta = _dotProduct / (_forwadVecSize * _velocitySize);
	float _theta = acosf(_cosTheta);

	// �и� 0�� �Ǹ� �ȵȴ�
	if ((_forwadVecSize * _velocitySize) == 0)
	{
		return;
	}

	/// ����/�̵��ϴ� ������ ����� ���ҽ�Ų��? �𸣰ʹ�...
	m_Velocity.x = m_Velocity.x - _cosTheta;
	m_Velocity.y = m_Velocity.y - _cosTheta;
}

void CarMK2::ScaleObject(float val)
{
	CGameObject::ScaleObject(val);
}

void CarMK2::Draw(CABON_Engine* pEngine)
{
	pEngine->SetTransform(D2D1::Matrix3x2F::Identity());

	m_Trail_TailRight->DrawTrail(pEngine);
	m_Trail_TailLeft->DrawTrail(pEngine);


	/// ��������Ʈ�� �׷��ִ� �Լ�
	pEngine->SetTransform(m_Transform->MatrixTranslation(m_Transform->GetLocalPosition()));

	/// �׸��� �׽�Ʈ �Լ�, �������� ��������Ʈ�� ����(�迭)�� �Ѱܼ� �׸���.
	///pEngine->DrawShadowByRotation(v_Sprite, m_Transform->GetLocalRotation(), 36);
	///pEngine->DrawSpriteByRotation(m_bitmap, m_Transform->GetLocalRotation(), 36);


	pEngine->SetTransform(D2D1::Matrix3x2F::Identity());



	/// ��ƼŬ�� ������ִ� �Լ�
	m_Smoke->DrawParticle();
}

void CarMK2::Draw(CABON_Engine* pEngine, ID2D1Bitmap* bitmap)
{
	pEngine->SetTransform(m_Transform->GetWorldTransform());

	pEngine->ApplyBlend(this, bitmap);

	pEngine->SetTransform(D2D1::Matrix3x2F::Identity());
}

void CarMK2::TestDraw(CABON_Engine* pEngine)
{
	// ���� ������Ʈ�� ���� TransformMatrix�� �����´�
/// �簢���� �׷��ִ� �Լ�
	pEngine->SetTransform(m_Transform->GetWorldTransform());

	// �׽�Ʈ�� draw
	//pEngine->DrawFillRect_NowCol(D2D1::RectF(-30, -50, 30, 50));
	pEngine->DrawFillRect_NowCol(D2D1::RectF(-m_Width / 2, -m_Height / 2, m_Width / 2, m_Height / 2));

	pEngine->SetTransform(D2D1::Matrix3x2F::Identity());
}

void CarMK2::DrawTrail(CABON_Engine* pEngine)
{
	m_Trail_TailRight->DrawTrail(pEngine);
}

void CarMK2::DebugInformation(CABON_Engine* pEngine, int pos)
{
	D2D1_POINT_2F _point = D2D1::Point2F(100, 10);

	pos = pos * 300;

	pEngine->DrawText(_point.x + pos, _point.y, L"======%d Vehichle Property=====", pos / 300 + 1);
	pEngine->DrawText(_point.x + pos, _point.y += 16, L"LocalRotation = %f", m_Transform->GetLocalRotation());
	pEngine->DrawText(_point.x + pos, _point.y += 16, L"LocalPositionX = %f", m_Transform->GetLocalPosition().x);
	pEngine->DrawText(_point.x + pos, _point.y += 16, L"LocalPositionY = %f", m_Transform->GetLocalPosition().y);
	pEngine->DrawText(_point.x + pos, _point.y += 16, L"Now Acceleration = %f", m_Acceleration);
	//pEngine->DrawText(_point.x, _point.y += 16, L"Now Velocity = %f", GetVelocity());
}

void CarMK2::DebugVectorInformation(CABON_Engine* pEngine)
{
	D2D1_VECTOR_2F _nowPos = m_Transform->GetLocalPosition();
	D2D1_POINT_2F _point = D2D1::Point2F(_nowPos.x, _nowPos.y);

	_point.x += 20;
	_point.y -= 100;

	pEngine->DrawText(_point.x, _point.y, L"======Vehichle VectorInfo=====");
	pEngine->DrawText(_point.x, _point.y += 16, L"Up Vector X = %f", m_UpVector.x);
	pEngine->DrawText(_point.x, _point.y += 16, L"Up Vector Y = %f", m_UpVector.y);
	pEngine->DrawText(_point.x, _point.y += 16, L"Velocity X = %f", m_Velocity.x);
	pEngine->DrawText(_point.x, _point.y += 16, L"Velocity Y = %f", m_Velocity.y);
	pEngine->DrawText(_point.x, _point.y += 16, L"Forward Vector X = %f", m_ForwardVector.x);
	pEngine->DrawText(_point.x, _point.y += 16, L"Forward Vector Y = %f", m_ForwardVector.y);

	// ������
	pEngine->DrawText(_point.x, _point.y += 16, L"Angle = %f", m_Transform->GetLocalRotation());

	// ������
	float _frictionPower = CVector2::SizeOfVector(m_FrictionVector);
	pEngine->DrawText(_point.x, _point.y += 16, L"Friction = %f", _frictionPower);
}

void CarMK2::DebugDrawVector(CABON_Engine* pEngine)
{
	pEngine->SetTransform(m_Transform->MatrixTranslation(m_Transform->GetLocalPosition()));

	///------------------------------------------------------
	/// UpVector
	// UpVector�� ����
	D2D1::ColorF _upColor = (D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f));

	// UpVector�� ���� ���� �۱� ������ Ű���� ��������¡�Ѵ�.
	pEngine->DrawLineByVector(
		D2D1::Vector2F(),
		CVector2::VectorScalarMultiply(m_UpVector, 30),
		_upColor);


	///------------------------------------------------------
	/// Forward Vector
	D2D1::ColorF _forwardColor = (D2D1::ColorF(1.0f, 1.0f, 0.0f, 1.0f));

	pEngine->DrawLineByVector(
		D2D1::Vector2F(),
		CVector2::VectorScalarMultiply(m_ForwardVector, 30),
		_forwardColor);


	///------------------------------------------------------
	/// Velocity Vector
	D2D1::ColorF _velocityColor = (D2D1::ColorF(0.0f, 1.0f, 1.0f, 1.0f));


	pEngine->DrawLineByVector(
		D2D1::Vector2F(),
		m_Velocity,
		_velocityColor);

	///------------------------------------------------------
	D2D1::ColorF _friction = (D2D1::ColorF(0.0f, 0.0f, 1.0f, 1.0f));


	pEngine->DrawLineByVector(
		D2D1::Vector2F(),
		CVector2::VectorScalarMultiply(m_FrictionVector, 30),
		_friction);


	pEngine->SetTransform(D2D1::Matrix3x2F::Identity());
}

void CarMK2::ResetPosition()
{
	// ��ġ�� ���ӵ� ���͸� �ʱ�ȭ 
	m_Transform->SetLocalPosition(D2D1::Vector2F(100, 100));
	m_Velocity = D2D1::Vector2F(0, 0);
}
