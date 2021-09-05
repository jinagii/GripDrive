#pragma once
#include "GlobalDefine.h"

class SceneManager;
class InGameScene;
class D2DEngine;

class SettingScene
{
public:
	SettingScene();
	~SettingScene();

public:
	 void Initialize(D2DEngine* pEngine, SceneManager* pScene);
	 bool OnUpdate(float dTime);
	 void OnDraw(D2DEngine* pEngine, float dTime);

private:
	SceneManager* m_pSceneManager;
	InGameScene* m_pInGame;

	int m_State;

	ID2D1Bitmap* m_SettingVeiw;
	ID2D1Bitmap* m_Setting_Quit_0;
	ID2D1Bitmap* m_Setting_Quit_1;
	ID2D1Bitmap* m_Setting_Restart_0;
	ID2D1Bitmap* m_Setting_Restart_1;
	ID2D1Bitmap* m_Setting_Return_0;
	ID2D1Bitmap* m_Setting_Return_1;
	ID2D1Bitmap* m_Setting_Select_0;
	ID2D1Bitmap* m_Setting_Select_1;
};

