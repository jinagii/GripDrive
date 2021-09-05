#pragma once

#include <vector>
#include <CGameObject.h>

using namespace std;

class Trail;
class CABON_Engine;
class CTransform;
class Smoke;

class CarMK2 : public CGameObject
{
public:
	CarMK2();
	// 비트맵만 넣어주는 생성자
	CarMK2(CABON_Engine* pEngine);
	// 비트맵을 넣어주고, AABB의 크기를 넣을 수 있는 생성자
	CarMK2(CABON_Engine* pEngine, D2D1_RECT_F _rect);
	CarMK2(CABON_Engine* pEngine, D2D1_VECTOR_2F center);
	~CarMK2();

private:
	// 이동 관련 멤버변수
	
	vector<ID2D1Bitmap*> v_Sprite;

	D2D1_VECTOR_2F m_UpVector;	// 전방 벡터 (앞을 가리키는 벡터)
	D2D1_VECTOR_2F m_Velocity;	// 속도 벡터
	D2D1_VECTOR_2F m_ForwardVector;	// 방향 벡터 / 전방벡터에 회전행렬을 곱한것
	float m_Acceleration;
	float m_DriftCoef;
	float m_FrictionCoef;

	// 마찰력 관련 변수
	D2D1_VECTOR_2F m_FrictionVector;

	// 크기 관련 임시 변수
	int m_Width;
	int m_Height;
	int m_ColliderRadius;

public:
	// 기타 함수
	virtual void MoveHorizontal(float val) override;
	virtual void MoveVertical(float val) override;
	virtual void ScaleObject(float val);
	virtual void Draw(CABON_Engine* pEngine);

	virtual void Update();

	// 자동차 조작 관련 함수
	void MoveForward(float speed);			// 단순히 앞으로 움직일때
	void DoAccelerate(float accelInc);
	void VelocityUpdate(float dTime);		// 속도와 가속도 모두 업데이트

	// 안됨
	void DoDrift(float val);

	void FrictionByVectorSize();

	//void FrictionOn(float val);

	void VectorDotProduct_SideFriction();

public:
	// getter/setter
	void SetVelocity(D2D_VECTOR_2F val) { m_Velocity = val; }
	// 현재 가속도 벡터를 가지고 크기 값을 구하는 함수
	FLOAT GetVelocity();

	void TestDraw(CABON_Engine* pEngine);

	void Draw(CABON_Engine* pEngine, ID2D1Bitmap* bitmap);


public:
	// 트레일 이펙트
	Trail* m_Trail_TailRight;
	Trail* m_Trail_TailLeft;
	void DrawTrail(CABON_Engine* pEngine);

	// 연기 이펙트
	Smoke* m_Smoke;



public:
	// 내부정보인 Transform 등을 Get/Set하기보다는 자체적으로 정보를 그려준다.
	void DebugInformation(CABON_Engine* pEngine, int pos);
	void DebugVectorInformation(CABON_Engine* pEngine);
	void DebugDrawVector(CABON_Engine* pEngine);
	void ResetPosition();

};