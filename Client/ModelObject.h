#pragma once


#include "CGameObject.h"

#include "GlobalDefine.h"

enum class eObject1Name
{
	TREE = 0,
	CONE,
	TIRES,
	BARRICADE30,
	BARRICADE35,
	BARRICADE80,
	BARRICADE95,
	BARRICADE100,
	BARRICADE130,
	BARRICADE150,
	BARRICADE155,
	FENCE,
	DISPLAY,
	GARAGE, 
	LAMP,
	FOREST,
	AUDIENCE,
};


enum class eObject2Name
{
	STONE1 = 0,
	STONE2,
	LONGCHAIN,
	DIAGONALCHAIN,
	SHORTCHAIN,
};



enum class eObjectAttribute
{
	STATIC = 100,
	MOVABLE = 1,
};

class ModelObject : public CGameObject
{
public:
	ModelObject();
	ModelObject(D2D1_VECTOR_2F pos, ID2D1Bitmap* bitmap);
	~ModelObject();


private:
	// �̵� ����
	eObjectAttribute m_Attr;	// �̵� �����Ѱ� �Ұ����Ѱ�?
	eObject1Name m_Name1;
	eObject2Name m_Name2;


public:
	D2D1_VECTOR_2F m_Velocity;	// �ӵ� ����
	float m_FrictionCoef;		// ������ ��� (�������� �������� ����)


public:
	void Initialize(eObject1Name name);
	void Initialize(eObject2Name name);

	/// GameObject Override
	virtual void Update(float dTime) override;
	virtual void Draw(D2DEngine* pEngine, float dTime, D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM, bool isIsometric) override;
	virtual void DrawDebugData(D2DEngine* pEngine, D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM);

	virtual D2D1_VECTOR_2F GetVelocity() override;
	virtual void SetVelocity(D2D1_VECTOR_2F velocity) override;

};

