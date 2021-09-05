#include "CAABB.h"
#include "D2DEngine.h"
#include "CCircle.h"

CAABB::CAABB()
{

}

CAABB::CAABB(float posX, float posY, int width, int height)
	:m_Rect(D2D1::RectF(posX, posY, posX + width, posY + height)),
	m_IsCollide(false),
	m_Size(D2D1::SizeF(width, height)),
	m_Angle(0.0f)
{

}

CAABB::CAABB(D2D1_RECT_F _rect)
	:m_Rect(_rect),
	m_IsCollide(false),
	m_Size(D2D1::SizeF(_rect.right - _rect.left, _rect.bottom - _rect.top)),
	m_Angle(0.0f)
{

}

CAABB::~CAABB()
{

}

void CAABB::Draw(D2DEngine* pEngine)
{
	pEngine->DrawRectangle_Debug(m_Rect.left, m_Rect.top, m_Size.width, m_Size.height, IsCollide());
}

void CAABB::MoveHorizontal(float val)
{
	m_Rect.left += val;
	m_Rect.right += val;
}

void CAABB::MoveVertical(float val)
{
	m_Rect.top += val;
	m_Rect.bottom += val;
}

void CAABB::RotateAABB(float angle)
{
	m_Angle = GetAngle() + angle;
}

bool CAABB::CollisionDetection(ICollisionObject* pTarget)
{

	CAABB* _pAABBTarget = dynamic_cast<CAABB*>(pTarget);

	if (_pAABBTarget != nullptr)
	{
		if (this->m_Rect.left < _pAABBTarget->GetRectPosition().right &&
			this->m_Rect.top < _pAABBTarget->GetRectPosition().bottom &&
			_pAABBTarget->GetRectPosition().left < this->m_Rect.right &&
			_pAABBTarget->GetRectPosition().top < this->m_Rect.bottom)
		{
			this->SetCollide(true);
			_pAABBTarget->SetCollide(true);
			return true;
		}
		else
		{
			this->SetCollide(false);
			_pAABBTarget->SetCollide(false);
			return false;
		}
	}

	CCircle* _pCircleTarget = dynamic_cast<CCircle*>(pTarget);

	if (_pCircleTarget != nullptr)
	{
		int _centerPosX = _pCircleTarget->GetPosition().x;
		int _centerPosY = _pCircleTarget->GetPosition().y;

		int _halfOfRect = _pCircleTarget->GetRadius();

		D2D1_RECT_F _fakeRect;
		_fakeRect.left   = (float)_centerPosX - (float)_halfOfRect;
		_fakeRect.right  = (float)_centerPosX + (float)_halfOfRect;
		_fakeRect.top	 = (float)_centerPosY - (float)_halfOfRect;
		_fakeRect.bottom = (float)_centerPosY + (float)_halfOfRect;

		if (_fakeRect.left	  <= this->m_Rect.right	&&
			_fakeRect.top	  <= this->m_Rect.bottom &&
			this->m_Rect.left <= _fakeRect.right		&&
			this->m_Rect.top  <= _fakeRect.bottom)
		{
			this->SetCollide(true);
			_pCircleTarget->SetCollide(true);
			return true;
		}
		else
		{
			this->SetCollide(false);
			_pCircleTarget->SetCollide(false);
			return false;
		}
	}
	return false;
}

bool CAABB::CollisionDetection2(ICollisionObject* pTarget, D2D1_VECTOR_2F* reflVec)
{

	return true;
}

void CAABB::SetPosition(D2D1_VECTOR_2F pos)
{
	// 인자로 받는 벡터는 AABB의 중점으로 한다.
	int halfWidth = m_Size.width * 0.5f;
	int halfHeight = m_Size.height * 0.5f;

	m_Rect.left = pos.x - halfWidth;
	m_Rect.right = pos.x + halfWidth;
	m_Rect.top = pos.y - halfHeight;
	m_Rect.bottom = pos.y + halfHeight;
}

D2D1_VECTOR_2F CAABB::GetPosition()
{

	return D2D1::Vector2F(m_Rect.left, m_Rect.top);
}

void CAABB::SetIsCollided(bool val)
{
	m_IsCollide = val;
}

bool CAABB::GetIsCollided()
{
	return m_IsCollide;
}
