#include "Car.h"
#include "CABON_Engine.h"
#include "CTransform.h"
#include "CAABB.h"
#include "Trail.h"
#include "Smoke.h"
#include "CVector.h"

Car::Car()
{

}

Car::Car(CABON_Engine* pEngine)
{
	m_bitmap = pEngine->LoadBitmapByPath(L"./data/60mini.png");
	m_Trail_TailRight = new Trail();

	m_Smoke = new Smoke();
	m_Transform = new CTransform();
}

Car::Car(CABON_Engine* pEngine, D2D1_RECT_F _rect)
{
	m_bitmap = pEngine->LoadBitmapByPath(L"./data/CarMini.png");

	m_Transform = new CTransform();
	m_Collider = new CAABB(_rect);
	m_DriftCoef = 1.0f;
	m_Height = 100;
	m_Width = 60;

	// 이펙트 초기화
	m_Trail_TailRight = new Trail();
	m_Trail_TailLeft = new Trail();

	m_Smoke = new Smoke();
	
}

Car::~Car()
{

}

void Car::Update()
{
	// 현재 위치값을 약간 보정해준다.
	D2D1_VECTOR_2F _nowPos = m_Transform->GetLocalPosition();

	//m_Trail_TailRight->CheckPosition(_nowPos, D2D1::Point2F(20, 20));
	//m_Trail_TailLeft->CheckPosition(_nowPos, D2D1::Point2F(-20, 20));
	
	//m_Smoke->CreateParticle(_nowPos);

	/// 물리량 초기화
	m_Acceleration = 0;

	CGameObject::Update();
}

void Car::MoveHorizontal(float val)
{
	CGameObject::MoveHorizontal(val);
}

void Car::MoveVertical(float val)
{
	CGameObject::MoveVertical(val);
}

void Car::MoveForward(float speed)
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
	D2D_VECTOR_2F _forwardVec;
	_forwardVec.x = _upVec.x * _rotationMatrix.m11 + _upVec.y * _rotationMatrix.m12;
	_forwardVec.y = _upVec.x * _rotationMatrix.m21 + _upVec.y * _rotationMatrix.m22;

	/// 전방벡터의 크기를 조절한다

	// 전방벡터로 이동한다.
	m_Transform->MoveByTranslate(m_ForwardVec);
}

void Car::DoAccelerate(float accelInc)
{
	m_Acceleration = accelInc;
}

void Car::VelocityUpdate(float dTime)
{
	/// 물리 - 전방벡터를 이용해서 이동을 한다. (MoveForwardVec)
	// 자신의 전방 벡터 (앞 방향을 나타내는 벡터)를 어떻게 알아낼까?
	// 자동차가 위를 보고 있다고 가정한 상태의 (로컬좌표계 기준. 원 상태의 전방 벡터)
	m_UpVector = D2D1::Vector2F(0, -1.f * m_Acceleration);

	// 업벡터를 회전할 행렬
	D2D1::Matrix3x2F _rotationMatrix =
		D2D1::Matrix3x2F::Rotation(-1.0f * m_Transform->GetLocalRotation());

	// 결과적으로 회전된 전방 벡터 (함수를 못찾아서 만들었다)
	m_ForwardVec.x = m_UpVector.x * _rotationMatrix.m11 + m_UpVector.y * _rotationMatrix.m12;
	m_ForwardVec.y = m_UpVector.x * _rotationMatrix.m21 + m_UpVector.y * _rotationMatrix.m22;

	m_Velocity.x = (m_Velocity.x + m_ForwardVec.x);
	m_Velocity.y = (m_Velocity.y + m_ForwardVec.y);

	D2D1_VECTOR_2F _moveVec;

	_moveVec.x = m_Velocity.x * dTime;
	_moveVec.y = m_Velocity.y * dTime;

	// 속도 벡터로 이동한다.
	m_Transform->MoveByTranslate(_moveVec);




}

void Car::DoDrift(float val)
{
	/// 자동차의 앞부분을 회전시켜서 드리프트 
	//D2D_VECTOR_2F _upVec = D2D1::Vector2F(0, -1.f * m_Speed);

	D2D1::Matrix3x2F _driftMatrix =
		D2D1::Matrix3x2F::Rotation(
			val * m_Transform->GetLocalRotation(),
			D2D1::Point2F(m_Transform->GetLocalPosition().x, m_Transform->GetLocalPosition().y - 50));

	m_ForwardVec.x = m_UpVector.x * _driftMatrix.m11 + m_UpVector.y * _driftMatrix.m12;
	m_ForwardVec.y = m_UpVector.x * _driftMatrix.m21 + m_UpVector.y * _driftMatrix.m22;

	m_Transform->MoveByTranslate(m_ForwardVec);

	/// 이렇게 하는건가
	/*m_DriftCoef = m_Transform->GetLocalRotation();

	m_DriftCoef += val;

	m_Transform->GetTransformMatrix(
		D2D1::Vector2F(m_Transform->GetLocalPosition().x, m_Transform->GetLocalPosition().y - 50),
		m_DriftCoef,
		m_Transform->GetLoacalScale());*/
}

void Car::DriftClockwise()
{

}

void Car::DriftAntiClockwise()
{

}

void Car::FrictionOn(float val)
{
	if (m_Speed > 30)
	{
		m_Speed = m_Speed - val;
	}
	else if (m_Speed > 0 && m_Speed < 30)
	{
		m_Speed = m_Speed - 2 * val;
	}
	else
	{
		m_Speed = 0;
	}
}

void Car::VectorDotProduct()
{
	// 속도 벡터와 방향 벡터의 dot연산으로 속도 벡터를 감속해준다
	float _dotProduct = m_ForwardVec.x * m_Velocity.x + m_ForwardVec.y * m_Velocity.y;
	float _forwadVecSize = sqrt(pow(m_ForwardVec.x, 2) + pow(m_ForwardVec.y, 2));
	float _velocitySize = sqrt(pow(m_Velocity.x, 2) + pow(m_Velocity.y, 2));

	// 코사인 값 & 각도
	float _cosTheta = _dotProduct / (_forwadVecSize * _velocitySize);
	float _theta = acosf(_cosTheta);

	if ((_forwadVecSize * _velocitySize) == 0)
	{
		return;
	}
	m_Velocity.x = m_Velocity.x - _cosTheta;
	m_Velocity.y = m_Velocity.y - _cosTheta;


}

void Car::ScaleObject(float val)
{
	CGameObject::ScaleObject(val);
}

void Car::Draw(CABON_Engine* pEngine)
{

	/// 스프라이트를 그려주는 함수
	pEngine->SetTransform(m_Transform->MatrixTranslation(m_Transform->GetLocalPosition()));

	pEngine->DrawSpriteByRotation(m_bitmap, m_Transform->GetLocalRotation(), 12);

	pEngine->SetTransform(D2D1::Matrix3x2F::Identity());

	// 들어갈 위치를 정확히 해야한다.
	m_Trail_TailRight->DrawTrail(pEngine);
	m_Trail_TailLeft->DrawTrail(pEngine);

	//m_Smoke->DrawParticle();
	//CGameObject::DrawDebugData(pEngine);
}

void Car::Draw(CABON_Engine* pEngine, ID2D1Bitmap* bitmap)
{
	pEngine->SetTransform(m_Transform->GetWorldTransform());

	pEngine->ApplyBlend(this, bitmap);

	pEngine->SetTransform(D2D1::Matrix3x2F::Identity());
}

void Car::TestDraw(CABON_Engine* pEngine)
{
	// 현재 오브젝트의 최종 TransformMatrix를 가져온다
/// 사각형을 그려주는 함수
	pEngine->SetTransform(m_Transform->GetWorldTransform());

	// 테스트용 draw
	//pEngine->DrawFillRect_NowCol(D2D1::RectF(-30, -50, 30, 50));
	pEngine->DrawFillRect_NowCol(D2D1::RectF(-m_Width / 2, -m_Height / 2, m_Width / 2, m_Height / 2));

	pEngine->SetTransform(D2D1::Matrix3x2F::Identity());
}

void Car::DrawTrail(CABON_Engine* pEngine)
{
	m_Trail_TailRight->DrawTrail(pEngine);
}
