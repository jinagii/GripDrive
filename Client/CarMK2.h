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
	// ��Ʈ�ʸ� �־��ִ� ������
	CarMK2(CABON_Engine* pEngine);
	// ��Ʈ���� �־��ְ�, AABB�� ũ�⸦ ���� �� �ִ� ������
	CarMK2(CABON_Engine* pEngine, D2D1_RECT_F _rect);
	CarMK2(CABON_Engine* pEngine, D2D1_VECTOR_2F center);
	~CarMK2();

private:
	// �̵� ���� �������
	
	vector<ID2D1Bitmap*> v_Sprite;

	D2D1_VECTOR_2F m_UpVector;	// ���� ���� (���� ����Ű�� ����)
	D2D1_VECTOR_2F m_Velocity;	// �ӵ� ����
	D2D1_VECTOR_2F m_ForwardVector;	// ���� ���� / ���溤�Ϳ� ȸ������� ���Ѱ�
	float m_Acceleration;
	float m_DriftCoef;
	float m_FrictionCoef;

	// ������ ���� ����
	D2D1_VECTOR_2F m_FrictionVector;

	// ũ�� ���� �ӽ� ����
	int m_Width;
	int m_Height;
	int m_ColliderRadius;

public:
	// ��Ÿ �Լ�
	virtual void MoveHorizontal(float val) override;
	virtual void MoveVertical(float val) override;
	virtual void ScaleObject(float val);
	virtual void Draw(CABON_Engine* pEngine);

	virtual void Update();

	// �ڵ��� ���� ���� �Լ�
	void MoveForward(float speed);			// �ܼ��� ������ �����϶�
	void DoAccelerate(float accelInc);
	void VelocityUpdate(float dTime);		// �ӵ��� ���ӵ� ��� ������Ʈ

	// �ȵ�
	void DoDrift(float val);

	void FrictionByVectorSize();

	//void FrictionOn(float val);

	void VectorDotProduct_SideFriction();

public:
	// getter/setter
	void SetVelocity(D2D_VECTOR_2F val) { m_Velocity = val; }
	// ���� ���ӵ� ���͸� ������ ũ�� ���� ���ϴ� �Լ�
	FLOAT GetVelocity();

	void TestDraw(CABON_Engine* pEngine);

	void Draw(CABON_Engine* pEngine, ID2D1Bitmap* bitmap);


public:
	// Ʈ���� ����Ʈ
	Trail* m_Trail_TailRight;
	Trail* m_Trail_TailLeft;
	void DrawTrail(CABON_Engine* pEngine);

	// ���� ����Ʈ
	Smoke* m_Smoke;



public:
	// ���������� Transform ���� Get/Set�ϱ⺸�ٴ� ��ü������ ������ �׷��ش�.
	void DebugInformation(CABON_Engine* pEngine, int pos);
	void DebugVectorInformation(CABON_Engine* pEngine);
	void DebugDrawVector(CABON_Engine* pEngine);
	void ResetPosition();

};