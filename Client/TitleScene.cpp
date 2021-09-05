#include "TitleScene.h"

#include "D2DEngine.h"
#include "../DRCommonLib/LHInput.h"
#include "SceneManager.h"
#include "KeyInput.h"
#include "SoundManager.h"

#pragma comment(lib, "../Debug/DRCommonLib.lib")


TitleScene::TitleScene()
	:m_Direction(true),
	m_Opacity(0),
	m_StartFont(nullptr)
{

}

TitleScene::~TitleScene()
{

}

void TitleScene::Initialize(D2DEngine* pEngine, SceneManager* pScene)
{
	m_StartFont = pEngine->LoadBitmapByPath(L"./data/ui/start.png");
	m_TitleBG = pEngine->LoadBitmapByPath(L"./data/ui/MainTitle.png");
	m_pSceneManager = pScene;

	m_pUserInput = new LHInput();

}

void TitleScene::OnStart()
{
	SoundManager::GetInstance()->PlayBGM(eSoundName::UI_BGM);
}

void TitleScene::OnUpdate(float dTime)
{
	if (m_Direction)
	{
		m_Opacity -= 0.05f;
	}
	else
	{
		m_Opacity += 0.05f;
	}

	if (m_Opacity < 0 || m_Opacity > 1)
	{
		m_Direction = !m_Direction;
	}
	static float _countSec = 0;
	_countSec += dTime;
	if (_countSec > 0.2)
	{
		m_pUserInput->Update();
	}
}

void TitleScene::OnDraw(D2DEngine* pEngine, float dTime)
{
	pEngine->BeginRender();

	pEngine->ImageDrawByScale(m_TitleBG, D2D1::Point2F(), D2D1::SizeF(1.34f, 1.34f));
	pEngine->ImageDrawByPoint(m_StartFont, D2D1::Point2F(300, 600), m_Opacity);

	pEngine->EndRender();
}

void TitleScene::ChangeScene()
{	
	if (KeyInput::InputKeyUp(VK_RETURN) || m_pUserInput->GetKeyDown (0, XINPUT_GAMEPAD_START))
	{
		m_pSceneManager->SetNowScene(eSelectScene);
		SoundManager::GetInstance()->PlayOnce(eSoundName::UI_Click);		
	}
}
