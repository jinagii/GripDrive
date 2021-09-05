#pragma once
#include <CGameObject.h>

class Trail;
class CABON_Engine;
class CTransform;
class Smoke;

class Car : public CGameObject
{
public:
	Car();
	// 비트맵만 넣어주는 생성자
	Car(CABON_Engine* pEngine);
	// 비트맵을 넣어주고, AABB의 크기를 넣을 수 있는 생성자
	Car(CABON_Engine* pEngine, D2D1_RECT_F _rect);
	~Car();

private:
	// 이동 관련 멤버변수
	float m_Speed;
	D2D_VECTOR_2F m_UpVector;	// 전방 벡터 (앞을 가리키는 벡터)
	D2D_VECTOR_2F m_Velocity;	// 속도 벡터
	D2D_VECTOR_2F m_ForwardVec;	// 방향 벡터(회전된 전방벡터)
	float m_Acceleration;
	float m_DriftCoef;

	// 크기 관련 임시 변수
	int m_Width;
	int m_Height;
	
public:
	// 자동차 조작 관련 함수
	void MoveForward(float speed);		// 단순히 앞으로 움직일때
	void DoAccelerate(float accelInc);
	void VelocityUpdate(float dTime);		// 속도와 가속도 모두 업데이트

	void DoDrift(float val);
	void DriftClockwise();
	void DriftAntiClockwise();

	void FrictionOn(float val);

	void VectorDotProduct();


	// getter/setter
	float GetSpeed() const { return m_Speed; }
	void SetSpeed(float val) { m_Speed = val; }

	void SetVelocity(D2D_VECTOR_2F val) { m_Velocity = val; }


	// 기타 함수
	virtual void MoveHorizontal(float val) override;
	virtual void MoveVertical(float val) override;


	virtual void ScaleObject(float val);
	virtual void Draw(CABON_Engine* pEngine);

	void TestDraw(CABON_Engine* pEngine);

	void Draw(CABON_Engine* pEngine, ID2D1Bitmap* bitmap);



	// 트레일 이펙트
	Trail* m_Trail_TailRight;
	Trail* m_Trail_TailLeft;

	//Trail* m_Trail;

	// 연기 이펙트
	Smoke* m_Smoke;
	void DrawTrail(CABON_Engine* pEngine);

	virtual void Update();

};