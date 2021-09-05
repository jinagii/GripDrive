#pragma once

#include "CGameObject.h"
#include "CASprite.h"


enum class eState
{
	Idle = 0,
	Attack1,
};

class Character : public CGameObject
{
public:
	Character();
	~Character();

private:

	// dTime�� ��� ���������� ��
	float m_CountdTime;

public:
	std::vector<CAMotion*> m_Motions;
	eState m_State;

	virtual void Update();

	virtual void MoveHorizontal(float val);
	virtual void MoveVertical(float val);
	virtual void RotateObject(float val);
	virtual void ScaleObject(float val);

	// ĳ���ʹ� �ִϸ��̼��� �ʼ��̹Ƿ� ���� �����.
	// CGameObject������ �� �̻� �ʿ����� ���� ��
	void PlayMotion(D2DEngine* pEngine, float dTime, eState m_State);
	void DrawDebugData(D2DEngine* pEngine, CASprite* _sprite);


};

