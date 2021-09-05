#pragma once
#include <vector>
#include <CGameObject.h>

/// 충돌체크를 넣을 CarMK3 버전

using namespace std;

class Trail;
class D2DEngine;
class CTransform;
class Particle;
class CCircle;

class CarMK3 : public CGameObject
{
public:
	CarMK3(int index, D2DEngine* pEngine, D2D1_VECTOR_2F center, PCWSTR path);
	~CarMK3();

private:
	int m_CarIndex;	// 1~4

private:
	// 이동 관련 멤버변수

	vector<ID2D1Bitmap*> v_Sprite;

	D2D1_VECTOR_2F m_UpVector;	// 전방 벡터 (앞을 가리키는 벡터)
	D2D1_VECTOR_2F m_Velocity;	// 속도 벡터
	D2D1_VECTOR_2F m_ForwardVector;	// 방향 벡터 / 전방벡터에 회전행렬을 곱한것
	D2D1_VECTOR_2F m_DriftHelperVector; // 드리프트가 더 원활하게 도와준다

	float m_Acceleration;	// 가속도 
	float m_MaxVelocity;	// 최대 속도
	float m_RotationAngle;	// 기본 회전각
	float m_DriftAngle;		// 드리프트 회전각 
	float m_DriftPower;		// 드리프트 시 속도벡터 회전각
	float m_FrictionCoef;	// 마찰력 상수 (높을수록 마찰력이 높다)
	float m_DriftFrictionCoef; // 드리프트 마찰력 상수
	float m_MaxStaticFrictionCoef; // 최대정지 마찰력
	float m_Mass;			// 자동차의 무게 (충격량에 필요)

	bool m_BrakeOn;


	// 마찰력 관련 변수
	D2D1_VECTOR_2F m_FrictionVector;

	// 크기 관련 임시 변수
	int m_Width;
	int m_Height;

	int m_ColliderRadius;

	D2D1_MATRIX_3X2_F m_viewTM;

public:
	/// GameObject Override

	virtual void Update(float dTime) override;
	virtual void Draw(D2DEngine* pEngine, float dTime, D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM, bool isIsometric) override;

	virtual void CollisionCheck(CGameObject* target) override;
	virtual void CollisionCheck2(CGameObject* target) override;

	virtual D2D1_VECTOR_2F GetVelocity() override;
	virtual void SetVelocity(D2D1_VECTOR_2F velocity) override;

private:
	void CollisionCheck_Internal(CCircle* target);
	void CollisionCheck_Internal(CGameObject* target);

public:
	// 기타 함수
	virtual void MoveHorizontal(float val) override;
	virtual void MoveVertical(float val) override;
	virtual void ScaleObject(float val);

	// 자동차 조작 관련 함수
	void MoveForward(float speed);			// 단순히 앞으로 움직일때
	void DoAccelerate(float accelInc);
	void VelocityUpdate(float dTime);		// 속도와 가속도 모두 업데이트
	// 드리프트
	void DoDrift(float val1, float val2);

public:
	// Getter/Setter
	float GetRotationAngle() const { return m_RotationAngle; }
	void SetRotationAngle(float val) { m_RotationAngle += val; }
	float GetDriftAngle() const { return m_DriftAngle; }
	void SetDriftAngle(float val) { m_DriftAngle += val; }
	float GetDriftPower() const { return m_DriftPower; }
	void SetDriftPower(float val) { m_DriftPower += val; }
	float GetAcceleration() const { return m_Acceleration; }
	void SetAcceleration(float val) { m_Acceleration += val; }
	float GetFrictionCoef() const { return m_FrictionCoef; }
	void SetFrictionCoef(float val) { m_FrictionCoef += val; }
	float GetMass() const { return m_Mass; }
	void SetMass(float val) { m_Mass += val; }
	bool GetBrakeOn() const { return m_BrakeOn; }
	void SetBrakeOn() { m_BrakeOn = !m_BrakeOn; }
	int GetCarIndex() const { return m_CarIndex; }

	D2D1_VECTOR_2F GetFrictionVector() const { return m_FrictionVector; }
	// 현재 가속도 벡터를 가지고 크기 값을 구하는 함수
	float SizeOfVelocity();


public:
	// 트레일 이펙트
	Trail* m_Trail_TailRight;
	Trail* m_Trail_TailLeft;
	void DrawTrail(D2DEngine* pEngine);

	// 파티클 이펙트
	Particle* m_Smoke;
	Particle* m_Spark;

public:
	// 내부정보인 Transform 등을 Get/Set하기보다는 자체적으로 정보를 그려준다.
	void DebugInformation(D2DEngine* pEngine, int pos);
	void DebugVectorInformation(D2DEngine* pEngine, D2D1_MATRIX_3X2_F projTM, D2D1_MATRIX_3X2_F viewTM);
	void DebugDrawVector(D2DEngine* pEngine, D2D1_MATRIX_3X2_F projTM, D2D1_MATRIX_3X2_F viewTM, bool isIsometric);
	void ResetPosition();
	void ResetPositionByCheckPoint(D2D1_VECTOR_2F pos, float angle);
	void ResetEffect();

	// 사운드 관련
private:
	void CheckEngineSound();



	/// 임시. 충돌위치 반환
public:
	D2D1_VECTOR_2F m_ImpactPosition;


	// 순위 UI
private:
	ID2D1Bitmap* m_Rank[4];

	int m_RankIndex;

	void DrawRankUI(D2DEngine* pEngine, float dTime, D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM, bool isIsometric);

public:
	int RankIndex() const { return m_RankIndex; }
	void SetRankIndex(int val) { m_RankIndex = val; }
};