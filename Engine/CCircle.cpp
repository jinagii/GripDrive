#include "CCircle.h"

#include "D2DEngine.h"
#include "CAABB.h"
#include "COBB.h"
#include "CVector.h"


CCircle::CCircle()
{

}

CCircle::CCircle(float posX, float posY, int radius)
	:m_Position(D2D1::Vector2F(posX, posY)), m_Radius(radius), m_IsCollide(false)
{

}

CCircle::CCircle(D2D1_VECTOR_2F point, int radius)
	:m_Position(D2D1::Vector2F(point.x, point.y)), m_Radius(radius), m_IsCollide(false)
{

}

CCircle::~CCircle()
{

}

void CCircle::Draw(D2DEngine* pEngine)
{
	pEngine->DrawEllipse_Debug(m_Position.x, m_Position.y, m_Radius, m_IsCollide);
}

void CCircle::MoveHorizontal(float val)
{
	m_Position.x += val;
}

void CCircle::MoveVertical(float val)
{
	m_Position.y += val;
}

bool CCircle::CollisionDetection(ICollisionObject* pTarget)
{
	CCircle* _pCircleTarget = dynamic_cast<CCircle*>(pTarget);

	if (_pCircleTarget != nullptr)
	{
		int _distX = this->m_Position.x - _pCircleTarget->GetPosition().x; // 밑변
		int _distY = this->m_Position.y - _pCircleTarget->GetPosition().y; // 높이
		int _distT = sqrt((_distX * _distX) + (_distY * _distY));		// 빗변

		// 두 원의 지름을 더 한 값이 거리 보다 더 크다면, 부딪혔다.
		if (_distT <= this->m_Radius + _pCircleTarget->GetRadius())
		{

			this->SetIsCollided(true);
			_pCircleTarget->SetIsCollided(true);
			return true;
		}
		else
		{
			this->SetIsCollided(false);
			_pCircleTarget->SetIsCollided(false);
			return false;
		}
	}

	CAABB* _pAABBTarget = dynamic_cast<CAABB*>(pTarget);

	if (_pAABBTarget != nullptr)
	{
		// 원을 AABB로 만들든, AABB를 원으로 만들든 하나를 택한다.
		// 원은 항상 타원형이 아니라는 전제하에, 원을 AABB로 만든다.
		// 바꿔말하면, 직사각형 형태의 AABB를 원으로 만들게되면 계산에 오차가 생길 것이기 때문이다.
		
		// 먼저 원의 중점을 가져온다.
		int _centerPosX = this->m_Position.x;
		int _centerPosY = this->m_Position.y;

		// 여기서 지름 만큼 빼거나 더해 rect로 만든다.
		int _halfOfRect = this->m_Radius;
		
		D2D1_RECT_F _fakeRect;
		_fakeRect.left = (float)_centerPosX - (float)_halfOfRect;
		_fakeRect.right = (float)_centerPosX + (float)_halfOfRect;
		_fakeRect.top = (float)_centerPosY - (float)_halfOfRect;
		_fakeRect.bottom = (float)_centerPosY + (float)_halfOfRect;
		
		if (_fakeRect.left					 <= _pAABBTarget->GetRectPosition().right &&
			_fakeRect.top					 <= _pAABBTarget->GetRectPosition().bottom &&
			_pAABBTarget->GetRectPosition().left <= _fakeRect.right &&
			_pAABBTarget->GetRectPosition().top  <= _fakeRect.bottom)
		{
			this->SetIsCollided(true);
			_pAABBTarget->SetIsCollided(true);
			return true;
		}
		else
		{
			this->SetIsCollided(false);
			_pAABBTarget->SetIsCollided(false);
			return false;
		}
	}
	return false;
}

bool CCircle::CollisionDetection(CCircle* pTarget)
{
	//CCircle* _pCircleTarget = dynamic_cast<CCircle*>(pTarget);

	if (pTarget != nullptr)
	{
		int _distX = this->m_Position.x - pTarget->GetPosition().x; // 밑변
		int _distY = this->m_Position.y - pTarget->GetPosition().y; // 높이
		int _distT = sqrt((_distX * _distX) + (_distY * _distY));		// 빗변

		// 두 원의 지름을 더 한 값이 거리 보다 더 크다면, 부딪혔다.
		if (_distT <= this->m_Radius + pTarget->GetRadius())
		{
			this->SetIsCollided(true);
			pTarget->SetIsCollided(true);
			return true;
		}
		else
		{
			this->SetIsCollided(false);
			pTarget->SetIsCollided(false);
			return false;
		}
	}
}

// 충돌 시
// 1. 제대로 된 반사벡터를 만들고

// 2. 반사벡터를 현재 오브젝트의 위치에서 정확히 바로 빼 준다. (또는 조금 더)
// 2. 반사벡터의 일부(충격량을 질량의 반비례로 나눈만큼)를 이동 벡터에 더한다.
// 3. 2번의 작용은 일정 시간 내에 한 번만 일어나야 한다.
// (연속적인 충돌과 갖히는 것을 막기 위해서)
// 4. 3번의 실제적인 구현을 위해서 1번의 조금 더(엡실론)가 중요하다.
bool CCircle::CollisionDetection2(ICollisionObject* pTarget, D2D1_VECTOR_2F* reflVec)
{
	// 제대로 된 반사 벡터를 돌려줘야 한다.

	/// A. 원 대 원의 경우
	CCircle* _targetCircle = dynamic_cast<CCircle*>(pTarget);
	if (_targetCircle != nullptr)
	{
		float _distX = this->m_Position.x - _targetCircle->GetPosition().x;	// 밑변
		float _distY = this->m_Position.y - _targetCircle->GetPosition().y;	// 높이
		float _distT = sqrtf((_distX * _distX) + (_distY * _distY));	// 빗변

		// 두 원의 지름을 더 한 값이 거리 보다 더 크다면, 부딪혔다.
		if (_distT <= this->m_Radius + _targetCircle->GetRadius())
		{
			this->SetIsCollided(true);
			_targetCircle->SetIsCollided(true);

			// 반사벡터(타겟에게서 내게로 오는 벡터)의 방향을 정해준다.
			D2D1_VECTOR_2F _reflVector = D2D1::Vector2F(m_Position.x - _targetCircle->GetPosition().x,
				m_Position.y - _targetCircle->GetPosition().y);

			// 반사 벡터의 길이는 두 원의 반지름을 더한 것에서 중점간 거리를 뺀 것이다.
			_reflVector = CVector2::VectorNormalize(_reflVector);
			float _reflVecLength = (m_Radius + _targetCircle->GetRadius()) - _distT;
			_reflVector = CVector2::VectorMultiplyScalar(_reflVector, _reflVecLength);

			reflVec->x = _reflVector.x;
			reflVec->y = _reflVector.y;

			return true;
		}
		else
		{
			this->SetIsCollided(false);
			_targetCircle->SetIsCollided(false);

			// 반사 벡터는 없다.
			reflVec = nullptr;

			return false;
		}
	}


	/// B. 원 대 OBB의 경우
	COBB* _targetOBB = dynamic_cast<COBB*>(pTarget);
	if (_targetOBB != nullptr)
	{
		/// 1. 1-2 엣지만 체크한다.
		/// 엣지의 기준으로 차가 어디 있는가를 체크한다.
		CVECTOR2 _circleCenter(GetPosition().x, GetPosition().y);
		float _dist = _targetOBB->GetDistanceFromPoint(_targetOBB->edge1, _targetOBB->p1, _circleCenter);

		/// 2. 선분이어야 하므로, 2-3엣지의 시계방향쪽, 4-1엣지의 시계방향쪽에 중점이 있어야 한다.
		if (_dist < this->GetRadius())
		{
			if (_targetOBB->IsPointInOBB1_2Edge(_circleCenter) == true)
			{
				// 직선벡터를 90도 돌린 것이 반사벡터의 방향
				D2D1_VECTOR_2F _edgeVec = D2D1::Vector2F(_targetOBB->edge1.x, _targetOBB->edge1.y);
				D2D1::Matrix3x2F _rotationMatrix = D2D1::Matrix3x2F::Rotation(90.f);
				D2D1_VECTOR_2F _reflVector = CVector2::VectorMultiplyMatrix(_edgeVec, _rotationMatrix);
				_reflVector = CVector2::VectorNormalize(_reflVector);

				// 반지름에서 거리를 뺀 것이 반사벡터의 크기
				float _reflVecLength = m_Radius - _dist;
				_reflVector = CVector2::VectorMultiplyScalar(_reflVector, _reflVecLength);

				reflVec->x = _reflVector.x;
				reflVec->y = _reflVector.y;

				this->SetIsCollided(true);
				_targetOBB->SetIsCollided(true);

				return true;
			}

		}
		else
		{
			this->SetIsCollided(false);
			_targetOBB->SetIsCollided(false);

			// 반사 벡터는 없다.
			reflVec = nullptr;

			return false;
		}
	}

}

void CCircle::RotateAABB(float angle)
{

}

void CCircle::SetPosition(D2D1_VECTOR_2F pos)
{
	m_Position = pos;
}

D2D1_VECTOR_2F CCircle::GetPosition()
{
	return m_Position;
}

void CCircle::SetIsCollided(bool val)
{
	m_IsCollide = val;
}

bool CCircle::GetIsCollided()
{
	return m_IsCollide;
}

