#pragma once



#include "IScene.h"

class LHInput;

class LogoScene : public IScene
{
public:
	LogoScene();
	~LogoScene();

public:
	virtual void Initialize(D2DEngine* pEngine, SceneManager* pScene)override;
	virtual void OnStart() override;
	virtual void OnUpdate(float dTime) override;
	virtual void OnDraw(D2DEngine* pEngine, float dTime) override;
	virtual void ChangeScene() override;

private:
	CA_Animation* m_RedCarSprite;

	LHInput* m_pUserInput;

	// �ΰ�������� ���Ǵ� ����
	int m_Count;

};

