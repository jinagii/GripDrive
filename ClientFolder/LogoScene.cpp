#include "LogoScene.h"

#include "D2DEngine.h"
#include "../DRCommonLib/XboxPadInput.h"

#include "SceneManager.h"
#include "KeyInput.h"
#include "../DRCommonLib/XboxPadInput.h"
#include "SoundManager.h"

#pragma comment(lib, "../Debug/DRCommonLib.lib")


LogoScene::LogoScene()
	:m_RedCarSprite(nullptr)
{

}

LogoScene::~LogoScene()
{

}

void LogoScene::Initialize(D2DEngine* pEngine, SceneManager* pScene)
{
	m_RedCarSprite = pEngine->LoadSpriteSheetByPath(L"./data/Car360.png", 36, 3, false);
	m_pSceneManager = pScene;
	m_pUserInput = new XboxPadInput();
}

void LogoScene::OnStart()
{
	// 원하는 노래를 틀어
}

void LogoScene::OnUpdate(float dTime)
{
	static float _countSec = 0;
	_countSec += dTime;
	if (_countSec > 0.2)
	{
		m_pUserInput->Update();
	}
}

void LogoScene::OnDraw(D2DEngine* pEngine, float dTime)
{
	pEngine->BeginRender();

	m_Count++;
	pEngine->DrawSpriteAnimation(m_RedCarSprite, D2D1::Vector2F(m_Count * 10, 200), dTime);

	pEngine->EndRender();
}

void LogoScene::ChangeScene()
{
	//if (m_pUserInput->GetKeyDown(0, XINPUT_GAMEPAD_START) || CABON_Input::InputKeyUp(VK_RETURN))
	//{
	//	//m_pSceneManager->SetNowScene(eGameProcedure);
	//	////SoundManager::GetInstance()->SetSoundPlaying(eSoundName::UI_Click, false);
	//	//SoundManager::GetInstance()->PlayAuto(eSoundName::UI_Click);
	//	//
	//	//m_pSceneManager->SetNowScene(eTitleScene);

	//	//m_GM->m_Pad.clear();
	//}
	if (KeyInput::InputKeyUp(VK_RETURN))
	{
		m_pSceneManager->SetNowScene(eTitleScene);
		SoundManager::GetInstance()->PlayAuto(eSoundName::UI_Click);
	}
}
