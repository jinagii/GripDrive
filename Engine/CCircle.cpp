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
		int _distX = this->m_Position.x - _pCircleTarget->GetPosition().x; // �غ�
		int _distY = this->m_Position.y - _pCircleTarget->GetPosition().y; // ����
		int _distT = sqrt((_distX * _distX) + (_distY * _distY));		// ����

		// �� ���� ������ �� �� ���� �Ÿ� ���� �� ũ�ٸ�, �ε�����.
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
		// ���� AABB�� �����, AABB�� ������ ����� �ϳ��� ���Ѵ�.
		// ���� �׻� Ÿ������ �ƴ϶�� �����Ͽ�, ���� AABB�� �����.
		// �ٲ㸻�ϸ�, ���簢�� ������ AABB�� ������ ����ԵǸ� ��꿡 ������ ���� ���̱� �����̴�.
		
		// ���� ���� ������ �����´�.
		int _centerPosX = this->m_Position.x;
		int _centerPosY = this->m_Position.y;

		// ���⼭ ���� ��ŭ ���ų� ���� rect�� �����.
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
		int _distX = this->m_Position.x - pTarget->GetPosition().x; // �غ�
		int _distY = this->m_Position.y - pTarget->GetPosition().y; // ����
		int _distT = sqrt((_distX * _distX) + (_distY * _distY));		// ����

		// �� ���� ������ �� �� ���� �Ÿ� ���� �� ũ�ٸ�, �ε�����.
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

// �浹 ��
// 1. ����� �� �ݻ纤�͸� �����

// 2. �ݻ纤�͸� ���� ������Ʈ�� ��ġ���� ��Ȯ�� �ٷ� �� �ش�. (�Ǵ� ���� ��)
// 2. �ݻ纤���� �Ϻ�(��ݷ��� ������ �ݺ�ʷ� ������ŭ)�� �̵� ���Ϳ� ���Ѵ�.
// 3. 2���� �ۿ��� ���� �ð� ���� �� ���� �Ͼ�� �Ѵ�.
// (�������� �浹�� ������ ���� ���� ���ؼ�)
// 4. 3���� �������� ������ ���ؼ� 1���� ���� ��(���Ƿ�)�� �߿��ϴ�.
bool CCircle::CollisionDetection2(ICollisionObject* pTarget, D2D1_VECTOR_2F* reflVec)
{
	// ����� �� �ݻ� ���͸� ������� �Ѵ�.

	/// A. �� �� ���� ���
	CCircle* _targetCircle = dynamic_cast<CCircle*>(pTarget);
	if (_targetCircle != nullptr)
	{
		float _distX = this->m_Position.x - _targetCircle->GetPosition().x;	// �غ�
		float _distY = this->m_Position.y - _targetCircle->GetPosition().y;	// ����
		float _distT = sqrtf((_distX * _distX) + (_distY * _distY));	// ����

		// �� ���� ������ �� �� ���� �Ÿ� ���� �� ũ�ٸ�, �ε�����.
		if (_distT <= this->m_Radius + _targetCircle->GetRadius())
		{
			this->SetIsCollided(true);
			_targetCircle->SetIsCollided(true);

			// �ݻ纤��(Ÿ�ٿ��Լ� ���Է� ���� ����)�� ������ �����ش�.
			D2D1_VECTOR_2F _reflVector = D2D1::Vector2F(m_Position.x - _targetCircle->GetPosition().x,
				m_Position.y - _targetCircle->GetPosition().y);

			// �ݻ� ������ ���̴� �� ���� �������� ���� �Ϳ��� ������ �Ÿ��� �� ���̴�.
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

			// �ݻ� ���ʹ� ����.
			reflVec = nullptr;

			return false;
		}
	}


	/// B. �� �� OBB�� ���
	COBB* _targetOBB = dynamic_cast<COBB*>(pTarget);
	if (_targetOBB != nullptr)
	{
		/// 1. 1-2 ������ üũ�Ѵ�.
		/// ������ �������� ���� ��� �ִ°��� üũ�Ѵ�.
		CVECTOR2 _circleCenter(GetPosition().x, GetPosition().y);
		float _dist = _targetOBB->GetDistanceFromPoint(_targetOBB->edge1, _targetOBB->p1, _circleCenter);

		/// 2. �����̾�� �ϹǷ�, 2-3������ �ð������, 4-1������ �ð�����ʿ� ������ �־�� �Ѵ�.
		if (_dist < this->GetRadius())
		{
			if (_targetOBB->IsPointInOBB1_2Edge(_circleCenter) == true)
			{
				// �������͸� 90�� ���� ���� �ݻ纤���� ����
				D2D1_VECTOR_2F _edgeVec = D2D1::Vector2F(_targetOBB->edge1.x, _targetOBB->edge1.y);
				D2D1::Matrix3x2F _rotationMatrix = D2D1::Matrix3x2F::Rotation(90.f);
				D2D1_VECTOR_2F _reflVector = CVector2::VectorMultiplyMatrix(_edgeVec, _rotationMatrix);
				_reflVector = CVector2::VectorNormalize(_reflVector);

				// ���������� �Ÿ��� �� ���� �ݻ纤���� ũ��
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

			// �ݻ� ���ʹ� ����.
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

