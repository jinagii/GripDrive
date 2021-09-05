#include "CASprite.h"



CASprite::CASprite()
	:m_SpritePos(D2D1::RectF(0, 0, 0, 0)),
	m_CenterPos(D2D1::Point2F(0, 0)),
	m_Size(D2D1::SizeF(0, 0)),
	m_DelayFrame(0.0f),
	m_HitAttribute(eHitAttribute::Normal),
	m_AttackAttribute(eAttackAttribute::Top),
	m_Damage(0),
	m_Armor(0)
{

}

CASprite::~CASprite()
{

}

CAMotion::CAMotion()
{

}

CAMotion::~CAMotion()
{

}

void CAMotion::AddSprite(CASprite* sprite)
{
	m_Animation.push_back(sprite);
}
