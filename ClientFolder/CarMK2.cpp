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

	// 이펙트 초기화
	m_Trail_TailRight = new Trail();
	m_Trail_TailLeft = new Trail();

	m_Smoke = new Smoke();

	// 파티클 오브젝트 풀을 몇개나 만들어 둘 것인지 결정
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

	// transform 관련 new
	m_Transform = new CTransform();
	m_Transform->SetLocalPosition(center);

	// 물리변수 초기화
	m_DriftCoef = 1.0f;
	m_FrictionCoef = 0.03f;

	// 충돌 반경
	m_ColliderRadius = 70;
	//m_Collider = new CCircle(center, m_ColliderRadius);

	// 이펙트 초기화
	m_Trail_TailRight = new Trail();
	m_Trail_TailLeft = new Trail();

	m_Smoke = new Smoke();

	// 파티클 오브젝트 풀을 몇개나 만들어 둘 것인지 결정
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
	// 현재 위치값을 약간 보정해준다.
	D2D1_VECTOR_2F _nowPos = m_Transform->GetLocalPosition();
	FLOAT _nowRot = m_Transform->GetLocalRotation();




	/// 트레일 노드 생성
	if (CVector2::SizeOfVector(m_FrictionVector) > 1)
	{
		m_Trail_TailRight->CheckPosition(_nowPos, D2D1::Point2F(10, 10), _nowRot);
		m_Trail_TailLeft->CheckPosition(_nowPos, D2D1::Point2F(-10, 10), _nowRot);
	}



	/// 자동차 배기가스
	if (CVector2::SizeOfVector(m_ForwardVector) > 1)
	{
		m_Smoke->PlayParticle(_nowPos, D2D1::Point2F(0, 20), _nowRot);
	}



	/// 물리량 초기화
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

	// 자신의 전방 벡터 (앞 방향을 나타내는 벡터)를 어떻게 알아낼까?

	// 자동차가 위를 보고 있다고 가정한 상태의 (로컬좌표계 기준. 원 상태의 전방 벡터)
	D2D_VECTOR_2F _upVec = D2D1::Vector2F(0, -1.f * speed);

	// 업벡터를 회전할 행렬
	D2D1::Matrix3x2F _rotationMatrix =
		D2D1::Matrix3x2F::Rotation(-1.0f * m_Transform->GetLocalRotation());

	// 결과적으로 회전된 전방 벡터 (함수를 못찾아서 만들었다)
	m_ForwardVector = CVector2::VectorMultiplyMatrix_Math(_upVec, _rotationMatrix);

	/// 전방벡터의 크기를 조절한다

	// 전방벡터로 이동한다.
	m_Transform->MoveByTranslate(m_ForwardVector);
}

void CarMK2::DoAccelerate(float accelInc)
{
	m_Acceleration = accelInc;
}

void CarMK2::VelocityUpdate(float dTime)
{
	/// 물리 - 전방벡터를 이용해서 이동을 한다. (MoveForwardVec)

	// 자신의 전방 벡터 (앞 방향을 나타내는 벡터)를 어떻게 알아낼까?
	// 자동차가 위를 보고 있다고 가정한 상태의 (로컬좌표계 기준. 원 상태의 전방 벡터)
	m_UpVector = D2D1::Vector2F(0, -1.0f);

	// 업벡터를 회전할 행렬
	D2D1::Matrix3x2F _rotationMatrix = D2D1::Matrix3x2F::Rotation(-1.f * m_Transform->GetLocalRotation());

	// 결과적으로 회전된 단위 전방 벡터 == 방향벡터
	D2D1_VECTOR_2F _ForwardVecNor = CVector2::VectorMultiplyMatrix_Math(m_UpVector, _rotationMatrix);

	// 가속력이 붙은 실제 길이가 있는 전방벡터
	m_ForwardVector = CVector2::VectorScalarMultiply(_ForwardVecNor, m_Acceleration);

	// 실제 진행 방향 벡터 (속력 & 방향)
	m_Velocity = CVector2::VectorPlus(m_Velocity, m_ForwardVector);

	// 마찰력벡터
	///_ForwardVecNor; // 방향벡터

	float _dotScalar = CVector2::VectorDotProduct(m_Velocity, _ForwardVecNor);
	D2D1_VECTOR_2F _forwardOrtho = CVector2::VectorScalarMultiply(_ForwardVecNor, _dotScalar);

	m_FrictionVector = CVector2::VectorMinus(m_Velocity, _forwardOrtho);
	m_FrictionVector = CVector2::VectorScalarMultiply(m_FrictionVector, m_FrictionCoef);	/// 일정 마찰계수

	// 마찰력을 빼 준다.
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


	// 이동 벡터 생성
	D2D1_VECTOR_2F _moveVec = CVector2::VectorScalarMultiply(m_Velocity, dTime);

	// 속도 벡터로 이동한다.
	m_Transform->MoveByTranslate(_moveVec);
}

void CarMK2::DoDrift(float val)
{
	/// 자동차의 앞부분을 회전시켜서 드리프트 
	//D2D_VECTOR_2F _upVec = D2D1::Vector2F(0, -1.f * m_Speed);

	D2D1::Matrix3x2F _driftMatrix =
		D2D1::Matrix3x2F::Rotation(
			val * m_Transform->GetLocalRotation(),
			D2D1::Point2F(m_Transform->GetLocalPosition().x, m_Transform->GetLocalPosition().y - 50));

	m_ForwardVector.x = m_UpVector.x * _driftMatrix.m11 + m_UpVector.y * _driftMatrix.m12;
	m_ForwardVector.y = m_UpVector.x * _driftMatrix.m21 + m_UpVector.y * _driftMatrix.m22;

	m_Transform->MoveByTranslate(m_ForwardVector);

	/// 이렇게 하는건가
	/*m_DriftCoef = m_Transform->GetLocalRotation();

	m_DriftCoef += val;

	m_Transform->GetTransformMatrix(
		D2D1::Vector2F(m_Transform->GetLocalPosition().x, m_Transform->GetLocalPosition().y - 50),
		m_DriftCoef,
		m_Transform->GetLoacalScale());*/
}

void CarMK2::FrictionByVectorSize()
{
	m_Velocity = CVector2::VectorScalarMultiply(m_Velocity, 0.99f/*임시마찰력*/);
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
	// 속도 벡터와 방향 벡터의 dot연산으로 속도 벡터를 감속해준다
	float _dotProduct = CVector2::VectorDotProduct(m_ForwardVector, m_Velocity);

	// 벡터 사이즈
	float _forwadVecSize = sqrt(pow(m_ForwardVector.x, 2) + pow(m_ForwardVector.y, 2));
	float _velocitySize = sqrt(pow(m_Velocity.x, 2) + pow(m_Velocity.y, 2));

	// 코사인 값 & 각도
	float _cosTheta = _dotProduct / (_forwadVecSize * _velocitySize);
	float _theta = acosf(_cosTheta);

	// 분모가 0이 되면 안된다
	if ((_forwadVecSize * _velocitySize) == 0)
	{
		return;
	}

	/// 진행/이동하는 벡터의 사이즈를 감소시킨다? 모르것다...
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


	/// 스프라이트를 그려주는 함수
	pEngine->SetTransform(m_Transform->MatrixTranslation(m_Transform->GetLocalPosition()));

	/// 그림자 테스트 함수, 독립적인 스프라이트의 벡터(배열)을 넘겨서 그린다.
	///pEngine->DrawShadowByRotation(v_Sprite, m_Transform->GetLocalRotation(), 36);
	///pEngine->DrawSpriteByRotation(m_bitmap, m_Transform->GetLocalRotation(), 36);


	pEngine->SetTransform(D2D1::Matrix3x2F::Identity());



	/// 파티클을 재생해주는 함수
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
	// 현재 오브젝트의 최종 TransformMatrix를 가져온다
/// 사각형을 그려주는 함수
	pEngine->SetTransform(m_Transform->GetWorldTransform());

	// 테스트용 draw
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

	// 각도값
	pEngine->DrawText(_point.x, _point.y += 16, L"Angle = %f", m_Transform->GetLocalRotation());

	// 마찰력
	float _frictionPower = CVector2::SizeOfVector(m_FrictionVector);
	pEngine->DrawText(_point.x, _point.y += 16, L"Friction = %f", _frictionPower);
}

void CarMK2::DebugDrawVector(CABON_Engine* pEngine)
{
	pEngine->SetTransform(m_Transform->MatrixTranslation(m_Transform->GetLocalPosition()));

	///------------------------------------------------------
	/// UpVector
	// UpVector의 색깔
	D2D1::ColorF _upColor = (D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f));

	// UpVector의 값이 워낙 작기 때문에 키워서 비쥬얼라이징한다.
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
	// 위치와 가속도 벡터를 초기화 
	m_Transform->SetLocalPosition(D2D1::Vector2F(100, 100));
	m_Velocity = D2D1::Vector2F(0, 0);
}
