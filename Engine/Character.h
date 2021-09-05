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

	// dTime을 계속 누적시켜줄 곳
	float m_CountdTime;

public:
	std::vector<CAMotion*> m_Motions;
	eState m_State;

	virtual void Update();

	virtual void MoveHorizontal(float val);
	virtual void MoveVertical(float val);
	virtual void RotateObject(float val);
	virtual void ScaleObject(float val);

	// 캐릭터는 애니메이션이 필수이므로 따로 만든다.
	// CGameObject에서는 더 이상 필요하지 않을 듯
	void PlayMotion(D2DEngine* pEngine, float dTime, eState m_State);
	void DrawDebugData(D2DEngine* pEngine, CASprite* _sprite);


};

