#pragma once

#include "IScene.h"

class D2DEngine;
class SceneManager;
class XboxPadInput;

class TitleScene : public IScene
{
public:
	TitleScene();
	~TitleScene();

public:
	virtual void Initialize(D2DEngine* pEngine, SceneManager* pScene) override;
	virtual void OnStart() override;
	virtual void OnUpdate(float dTime) override;
	virtual void OnDraw(D2DEngine* pEngine, float dTime) override;
	virtual void ChangeScene() override;

private:
	ID2D1Bitmap* m_StartFont;
	ID2D1Bitmap* m_TitleBG;

	bool m_Direction;
	float m_Opacity;

	XboxPadInput* m_pUserInput;
};

