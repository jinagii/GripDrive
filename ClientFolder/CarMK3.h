#pragma once
#include <vector>
#include <CGameObject.h>

/// �浹üũ�� ���� CarMK3 ����

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
	// �̵� ���� �������

	vector<ID2D1Bitmap*> v_Sprite;

	D2D1_VECTOR_2F m_UpVector;	// ���� ���� (���� ����Ű�� ����)
	D2D1_VECTOR_2F m_Velocity;	// �ӵ� ����
	D2D1_VECTOR_2F m_ForwardVector;	// ���� ���� / ���溤�Ϳ� ȸ������� ���Ѱ�
	D2D1_VECTOR_2F m_DriftHelperVector; // �帮��Ʈ�� �� ��Ȱ�ϰ� �����ش�

	float m_Acceleration;	// ���ӵ� 
	float m_MaxVelocity;	// �ִ� �ӵ�
	float m_RotationAngle;	// �⺻ ȸ����
	float m_DriftAngle;		// �帮��Ʈ ȸ���� 
	float m_DriftPower;		// �帮��Ʈ �� �ӵ����� ȸ����
	float m_FrictionCoef;	// ������ ��� (�������� �������� ����)
	float m_DriftFrictionCoef; // �帮��Ʈ ������ ���
	float m_MaxStaticFrictionCoef; // �ִ����� ������
	float m_Mass;			// �ڵ����� ���� (��ݷ��� �ʿ�)

	bool m_BrakeOn;


	// ������ ���� ����
	D2D1_VECTOR_2F m_FrictionVector;

	// ũ�� ���� �ӽ� ����
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
	// ��Ÿ �Լ�
	virtual void MoveHorizontal(float val) override;
	virtual void MoveVertical(float val) override;
	virtual void ScaleObject(float val);

	// �ڵ��� ���� ���� �Լ�
	void MoveForward(float speed);			// �ܼ��� ������ �����϶�
	void DoAccelerate(float accelInc);
	void VelocityUpdate(float dTime);		// �ӵ��� ���ӵ� ��� ������Ʈ
	// �帮��Ʈ
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
	// ���� ���ӵ� ���͸� ������ ũ�� ���� ���ϴ� �Լ�
	float SizeOfVelocity();


public:
	// Ʈ���� ����Ʈ
	Trail* m_Trail_TailRight;
	Trail* m_Trail_TailLeft;
	void DrawTrail(D2DEngine* pEngine);

	// ��ƼŬ ����Ʈ
	Particle* m_Smoke;
	Particle* m_Spark;

public:
	// ���������� Transform ���� Get/Set�ϱ⺸�ٴ� ��ü������ ������ �׷��ش�.
	void DebugInformation(D2DEngine* pEngine, int pos);
	void DebugVectorInformation(D2DEngine* pEngine, D2D1_MATRIX_3X2_F projTM, D2D1_MATRIX_3X2_F viewTM);
	void DebugDrawVector(D2DEngine* pEngine, D2D1_MATRIX_3X2_F projTM, D2D1_MATRIX_3X2_F viewTM, bool isIsometric);
	void ResetPosition();
	void ResetPositionByCheckPoint(D2D1_VECTOR_2F pos, float angle);
	void ResetEffect();

	// ���� ����
private:
	void CheckEngineSound();



	/// �ӽ�. �浹��ġ ��ȯ
public:
	D2D1_VECTOR_2F m_ImpactPosition;


	// ���� UI
private:
	ID2D1Bitmap* m_Rank[4];

	int m_RankIndex;

	void DrawRankUI(D2DEngine* pEngine, float dTime, D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM, bool isIsometric);

public:
	int RankIndex() const { return m_RankIndex; }
	void SetRankIndex(int val) { m_RankIndex = val; }
};