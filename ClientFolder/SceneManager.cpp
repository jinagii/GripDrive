#include "SceneManager.h"
#include "D2DEngine.h"
#include "CTimeController.h"
#include "GameManager.h"
#include "KeyInput.h"
#include "SoundManager.h"

#include "LogoScene.h"
#include "TitleScene.h"
#include "SelectScene.h"
#include "InGameScene.h"
#include "ResultScene.h"
#include "SettingScene.h"

SceneManager::SceneManager(D2DEngine* pEngine, CTimeController* pTime, GameManager* pGameManager)
	:m_DeltaTime(0),
	m_PrevScene(eGameScene::eLogoScene),
	m_NowScene(eGameScene::eLogoScene),
	m_pLogoScene(nullptr),
	m_pTitleScene(nullptr),
	m_pSeletScene(nullptr),
	m_pTimeController(nullptr),
	m_pInGame(nullptr), 
	m_pSettingScene(nullptr)
{
	m_pEngine = pEngine;
	m_pTimeController = pTime;
	m_GM = pGameManager;
}

SceneManager::~SceneManager()
{

}

void SceneManager::Initialize()
{
	m_pLogoScene = new LogoScene();
	m_pTitleScene = new TitleScene();
	m_pSeletScene = new SelectScene();
	m_pInGame = new InGameScene();
	m_pResultScene = new ResultScene();

	m_vec_Scene.push_back(m_pLogoScene);
	m_vec_Scene.push_back(m_pTitleScene);
	m_vec_Scene.push_back(m_pSeletScene);
	m_vec_Scene.push_back(m_pInGame);
	m_vec_Scene.push_back(m_pResultScene);

	for (int i = 0; i < 5; i++)
	{
		m_vec_Scene[i]->Initialize(m_pEngine, this);
	}

}

void SceneManager::FiniteStateMachine()
{
	while (m_NowScene != eGameScene::eExit)
	{
		if (m_pTimeController->FPSCheck() == false)
		{
			continue;
		}

		m_DeltaTime = (m_pTimeController->GetElapsedTimeSec());

		OnStart();

		if (m_NowScene != eGameProcedure)
		{
			SceneKeyUpdate();
		}

		m_vec_Scene[m_NowScene]->OnUpdate(m_DeltaTime);
		m_vec_Scene[m_NowScene]->OnDraw(m_pEngine, m_DeltaTime);
		m_vec_Scene[m_NowScene]->ChangeScene();
	}
}

void SceneManager::OnStart()
{
	if (m_PrevScene != m_NowScene)
	{
		// 씬이 바뀌었을 때 처음 불려질 함수들
		m_vec_Scene[m_NowScene]->OnStart();
		
		m_PrevScene = m_NowScene;
	}
}

void SceneManager::SceneKeyUpdate()
{
	KeyInput::KeyUpdate();
}
