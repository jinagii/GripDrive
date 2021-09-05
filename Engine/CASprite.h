#pragma once

#include "GlobalDefine.h"

class CAABB;

enum class eHitAttribute
{
	Normal = 0,			// ���� ���� (idle)
	Invincible,			// ����
	SuperArmor,
	Defence,
	LowDefence,			// �ϴ� ���
	AirDefence,			// ���� ���
	Disabled,			// ���� ����
	HitAttributeMax
};

enum class eAttackAttribute
{
	Top = 0,
	Middle,
	Bottom,
	Airial,
	Gabulgi,			// ���ұ�
	GuardCrush,			// ���հ� �������� �ش�
	Firing,				// ����ü �߻� ���� (���� ��� �������� ���ϰ� �Ѵ�)
	AttackAttributeMax
};

class CASprite
{
public:
	CASprite();
	~CASprite();

private:
public:
	int	m_Index;			// �� ���� ���(�ִϸ��̼�) �ȿ��� �� ��°�� ��������Ʈ����

	// �ҽ� �̹���
	// D2D���� �̹����� �θ� ��, CreateDecoderFromFilename�� ����Ѵ�.
	// ������ ��θ� �Ķ���ͷ� ������ �� WCHAR�� ����Ѵ�.
	// WCHAR m_SourceFile;
	// ID2D1Bitmap* m_Bitmap;	���� ������� ����

	// �ִϸ��̼� ����
	D2D1_RECT_F m_SpritePos;	// �ҽ� �̹������� ������ �κ�
	D2D1_POINT_2F m_CenterPos;	// ��������Ʈ�� �߽��̴�. (ĳ������ ���� ��� ��ġ�ϴ��ĸ� ǥ��)
								// �»���� ������ �ȴ�.
	D2D1_SIZE_F m_Size;			// ��������Ʈ�� ũ�⸦ ��Ÿ����. 
	float	m_DelayFrame;			// �ִϸ��̼� ������ (� ����� �󸶳� ��������)

	// ���� ����
	CAABB* m_HitBox;			// �浹����1 (e.g. �ǰ�)
	CAABB* m_AttackBox;			// �浹����2 (e.g. ����)

	eHitAttribute m_HitAttribute;			// �ǰ� �� �Ӽ��� �����Ѵ�. (����, ���۾Ƹ� ��)
	eAttackAttribute m_AttackAttribute;		// ���� �� �Ӽ��� �����Ѵ�. (���, �ߴ�, �ϴ� ��)

	int	m_Damage;				// ���� �� �� ���ط�
	float m_Armor;				// �ǰ� �� ���� �氨�� (����)

	// ����ü�� �߻翩��
	bool m_IsFired;
	D2D1_POINT_2F m_FirePos;	// ����ü �߻� ��ġ

	// ü�� ����
	bool m_IsArial;

	// �ɱ� ����
	bool m_IsCrouch;
};

class CAMotion
{
public:
	CAMotion();
	~CAMotion();

private:
public:

	// ����� �̸�
	std::string Name;

	ID2D1Bitmap* m_Bitmap;

	// ����� �ε���
	int Index;

	// �� ����� ��������Ʈ ����
	int SpriteNum;

	// ���� �ִϸ��̼��ΰ� (���� �ִϸ��̼��̶�� ����, �ƴ϶�� �ٸ� �ִϸ��̼����� ��ȯ�ؾ� �Ѵ�)
	bool m_IsLoop;

	// �� ��ǿ� ����ִ� ��������Ʈ��
	std::vector<CASprite*> m_Animation;

	// �������� �̸��� ����� �������ְ�, �̸��� ���ؼ� ���͸� ������ �� ���� ��
	// std::map<std::string, std::vector<CASprite*> > map_Motion;

public:
	void AddSprite(CASprite* sprite);		/// �� �κ��� ��� ���� �� �־���Ѵ�.
};

