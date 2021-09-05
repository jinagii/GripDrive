#include "CDummy.h"

#include "CTransform.h"
#include "COBB.h"
#include "D2DEngine.h"


CDummy::CDummy(D2D1_VECTOR_2F centerPos, float width, float height, float angle)
{
	// 오브젝트의 현재 위치를 정해준다.
	m_Transform = new CTransform();
	m_Transform->SetLocalPosition(centerPos);

	m_Collider = new COBB(centerPos.x, centerPos.y, width, height, angle);
}

CDummy::~CDummy()
{

}

void CDummy::Initialize()
{

}

void CDummy::Update(float dTime)
{
	// 이동 없음
}

void CDummy::Draw(D2DEngine* pEngine, float dTime, D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM, bool isIsometric)
{

}

void CDummy::DrawDebugData(D2DEngine* pEngine, D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM)
{
	/// OBB를 그려주면 된다.
	D2D1_VECTOR_2F _nowPos = m_Collider->GetPosition();
	//D2D1_VECTOR_2F _nowPos = m_Transform->GetLocalPosition();
	pEngine->SetTransform(m_Transform->MatrixTranslation(_nowPos) * projTM * viewTM);

	COBB* _OBB = dynamic_cast<COBB*>(m_Collider);

	if (_OBB != nullptr)
	{
		pEngine->DrawLineByVector(D2D1::Vector2F(_OBB->p1.x, _OBB->p1.y), D2D1::Vector2F(_OBB->p2.x, _OBB->p2.y), D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f));
		pEngine->DrawLineByVector(D2D1::Vector2F(_OBB->p2.x, _OBB->p2.y), D2D1::Vector2F(_OBB->p3.x, _OBB->p3.y), D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f));
		pEngine->DrawLineByVector(D2D1::Vector2F(_OBB->p3.x, _OBB->p3.y), D2D1::Vector2F(_OBB->p4.x, _OBB->p4.y), D2D1::ColorF(0.0f, 1.0f, 0.0f, 1.0f));
		pEngine->DrawLineByVector(D2D1::Vector2F(_OBB->p4.x, _OBB->p4.y), D2D1::Vector2F(_OBB->p1.x, _OBB->p1.y), D2D1::ColorF(0.0f, 1.0f, 1.0f, 1.0f));
	}
}
