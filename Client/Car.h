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
	// ��Ʈ�ʸ� �־��ִ� ������
	Car(CABON_Engine* pEngine);
	// ��Ʈ���� �־��ְ�, AABB�� ũ�⸦ ���� �� �ִ� ������
	Car(CABON_Engine* pEngine, D2D1_RECT_F _rect);
	~Car();

private:
	// �̵� ���� �������
	float m_Speed;
	D2D_VECTOR_2F m_UpVector;	// ���� ���� (���� ����Ű�� ����)
	D2D_VECTOR_2F m_Velocity;	// �ӵ� ����
	D2D_VECTOR_2F m_ForwardVec;	// ���� ����(ȸ���� ���溤��)
	float m_Acceleration;
	float m_DriftCoef;

	// ũ�� ���� �ӽ� ����
	int m_Width;
	int m_Height;
	
public:
	// �ڵ��� ���� ���� �Լ�
	void MoveForward(float speed);		// �ܼ��� ������ �����϶�
	void DoAccelerate(float accelInc);
	void VelocityUpdate(float dTime);		// �ӵ��� ���ӵ� ��� ������Ʈ

	void DoDrift(float val);
	void DriftClockwise();
	void DriftAntiClockwise();

	void FrictionOn(float val);

	void VectorDotProduct();


	// getter/setter
	float GetSpeed() const { return m_Speed; }
	void SetSpeed(float val) { m_Speed = val; }

	void SetVelocity(D2D_VECTOR_2F val) { m_Velocity = val; }


	// ��Ÿ �Լ�
	virtual void MoveHorizontal(float val) override;
	virtual void MoveVertical(float val) override;


	virtual void ScaleObject(float val);
	virtual void Draw(CABON_Engine* pEngine);

	void TestDraw(CABON_Engine* pEngine);

	void Draw(CABON_Engine* pEngine, ID2D1Bitmap* bitmap);



	// Ʈ���� ����Ʈ
	Trail* m_Trail_TailRight;
	Trail* m_Trail_TailLeft;

	//Trail* m_Trail;

	// ���� ����Ʈ
	Smoke* m_Smoke;
	void DrawTrail(CABON_Engine* pEngine);

	virtual void Update();

};