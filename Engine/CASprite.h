#pragma once

#include "GlobalDefine.h"

class CAABB;

enum class eHitAttribute
{
	Normal = 0,			// 보통 상태 (idle)
	Invincible,			// 무적
	SuperArmor,
	Defence,
	LowDefence,			// 하단 방어
	AirDefence,			// 공중 방어
	Disabled,			// 가사 상태
	HitAttributeMax
};

enum class eAttackAttribute
{
	Top = 0,
	Middle,
	Bottom,
	Airial,
	Gabulgi,			// 가불기
	GuardCrush,			// 방어뚫고 데미지를 준다
	Firing,				// 투사체 발사 상태 (예를 들면 움직이지 못하게 한다)
	AttackAttributeMax
};

class CASprite
{
public:
	CASprite();
	~CASprite();

private:
public:
	int	m_Index;			// 한 개의 모션(애니메이션) 안에서 몇 번째의 스프라이트인지

	// 소스 이미지
	// D2D에서 이미지를 부를 때, CreateDecoderFromFilename을 사용한다.
	// 파일의 경로를 파라미터로 전달할 때 WCHAR를 사용한다.
	// WCHAR m_SourceFile;
	// ID2D1Bitmap* m_Bitmap;	현재 사용하지 않음

	// 애니메이션 관련
	D2D1_RECT_F m_SpritePos;	// 소스 이미지에서 가져올 부분
	D2D1_POINT_2F m_CenterPos;	// 스프라이트의 중심이다. (캐릭터의 발이 어디에 위치하느냐를 표현)
								// 좌상단이 원점이 된다.
	D2D1_SIZE_F m_Size;			// 스프라이트의 크기를 나타낸다. 
	float	m_DelayFrame;			// 애니메이션 딜레이 (어떤 모션이 얼마나 유지될지)

	// 게임 관련
	CAABB* m_HitBox;			// 충돌영역1 (e.g. 피격)
	CAABB* m_AttackBox;			// 충돌영역2 (e.g. 공격)

	eHitAttribute m_HitAttribute;			// 피격 시 속성을 결정한다. (무적, 슈퍼아머 등)
	eAttackAttribute m_AttackAttribute;		// 공격 시 속성을 결정한다. (상단, 중단, 하단 등)

	int	m_Damage;				// 공격 시 줄 피해량
	float m_Armor;				// 피격 시 피해 경감량 (방어력)

	// 투사체를 발사여부
	bool m_IsFired;
	D2D1_POINT_2F m_FirePos;	// 투사체 발사 위치

	// 체공 여부
	bool m_IsArial;

	// 앉기 여부
	bool m_IsCrouch;
};

class CAMotion
{
public:
	CAMotion();
	~CAMotion();

private:
public:

	// 모션의 이름
	std::string Name;

	ID2D1Bitmap* m_Bitmap;

	// 모션의 인덱스
	int Index;

	// 이 모션의 스프라이트 갯수
	int SpriteNum;

	// 루프 애니메이션인가 (루프 애니메이션이라면 지속, 아니라면 다른 애니메이션으로 전환해야 한다)
	bool m_IsLoop;

	// 이 모션에 들어있는 스프라이트들
	std::vector<CASprite*> m_Animation;

	// 툴에서는 이름과 모션을 정의해주고, 이름을 통해서 벡터를 꺼내올 수 있을 듯
	// std::map<std::string, std::vector<CASprite*> > map_Motion;

public:
	void AddSprite(CASprite* sprite);		/// 이 부분은 사실 툴에 가 있어야한다.
};

