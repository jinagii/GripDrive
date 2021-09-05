#include "SettingScene.h"

#include "D2DEngine.h"
#include "SceneManager.h"
#include "KeyInput.h"
#include "GameManager.h"
#include "InGameScene.h"
#include "SoundManager.h"

SettingScene::SettingScene()
{

}

SettingScene::~SettingScene()
{

}

void SettingScene::Initialize(D2DEngine* pEngine, SceneManager* pScene)
{
	m_pSceneManager = pScene;

	m_pInGame = pScene->GetInGame();

	m_SettingVeiw = pEngine->LoadBitmapByPath(L"./data/ui/Setting_View.png");
	m_Setting_Quit_0 = pEngine->LoadBitmapByPath(L"./data/ui/Setting_Quit_0.png");
	m_Setting_Quit_1 = pEngine->LoadBitmapByPath(L"./data/ui/Setting_Quit_1.png");
	m_Setting_Restart_0 = pEngine->LoadBitmapByPath(L"./data/ui/Setting_Restart_0.png");
	m_Setting_Restart_1 = pEngine->LoadBitmapByPath(L"./data/ui/Setting_Restart_1.png");
	m_Setting_Return_0 = pEngine->LoadBitmapByPath(L"./data/ui/Setting_Return_0.png");
	m_Setting_Return_1 = pEngine->LoadBitmapByPath(L"./data/ui/Setting_Return_1.png");
	m_Setting_Select_0 = pEngine->LoadBitmapByPath(L"./data/ui/Setting_Select_0.png");
	m_Setting_Select_1 = pEngine->LoadBitmapByPath(L"./data/ui/Setting_Select_1.png");

}

bool SettingScene::OnUpdate(float dTime)
{
	KeyInput::KeyUpdate();

	if (KeyInput::InputKeyUp(VK_UP))
	{
		SoundManager::GetInstance()->PlayAuto(eSoundName::UI_Move);
		m_State--;
	}
	if (KeyInput::InputKeyUp(VK_DOWN))
	{
		SoundManager::GetInstance()->PlayAuto(eSoundName::UI_Move);
		m_State++;
	}

	m_State = abs(m_State);

	if (KeyInput::InputKeyUp(VK_RETURN))
	{
		if (abs(m_State % 3) == 0)
		{
			SoundManager::GetInstance()->PlayAuto(eSoundName::UI_Click);
			m_pInGame->SetShowSetting(false);
			return false;
		}
		if (abs(m_State % 3) == 1)
		{
			SoundManager::GetInstance()->PlayAuto(eSoundName::UI_Click);
			m_pSceneManager->SetNowScene(eTitleScene);
			m_pInGame->SetShowSetting(false);
			return false;
		}
		if (abs(m_State % 3) == 2)
		{
			SoundManager::GetInstance()->PlayAuto(eSoundName::UI_Click);
			m_pSceneManager->SetNowScene(eExit);
			PostQuitMessage(0);
			return false;
		}
	}

	return true;
}

void SettingScene::OnDraw(D2DEngine* pEngine, float dTime)
{

	pEngine->BeginRender();

	pEngine->ImageDrawByPoint(m_SettingVeiw, D2D1::Point2F(886, 290));
	if (abs(m_State % 3) == 0)
	{
		pEngine->ImageDrawByPoint(m_Setting_Return_1, D2D1::Point2F(1016, 357));
		pEngine->ImageDrawByPoint(m_Setting_Select_0, D2D1::Point2F(1016, 526));
		pEngine->ImageDrawByPoint(m_Setting_Quit_0, D2D1::Point2F(1016, 868));
	}

	if (abs(m_State % 3) == 1)
	{
		pEngine->ImageDrawByPoint(m_Setting_Return_0, D2D1::Point2F(1016, 357));
		pEngine->ImageDrawByPoint(m_Setting_Select_1, D2D1::Point2F(1016, 526));
		pEngine->ImageDrawByPoint(m_Setting_Quit_0, D2D1::Point2F(1016, 868));
	}

	if (abs(m_State % 3) == 2)
	{
		pEngine->ImageDrawByPoint(m_Setting_Return_0, D2D1::Point2F(1016, 357));
		pEngine->ImageDrawByPoint(m_Setting_Select_0, D2D1::Point2F(1016, 526));
		pEngine->ImageDrawByPoint(m_Setting_Quit_1, D2D1::Point2F(1016, 868));
	}

	pEngine->EndRender();
}

