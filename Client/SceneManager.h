#pragma once

#include "GlobalDefine.h"
class D2DEngine;
class CTimeController;
class GameManager;

// IScene 의 자식 클래스들
class IScene;
class LogoScene;
class TitleScene;
class SelectScene;
class InGameScene;
class ResultScene;
class SettingScene;


enum eGameScene
{
	eLogoScene = 0,
	eTitleScene,
	eSelectScene,
	eGameProcedure,
	eResultScene,
	eSettingScene,
	eExit,
};

class SceneManager
{
public:
	SceneManager(D2DEngine* pEngine, CTimeController* pTime, GameManager* pGameManager);
	~SceneManager();

private:
	// 씬 매니저 스스로가 필요한 비트맵을 들고, 보여주기 위해 엔진을 넣었다.
	D2DEngine* m_pEngine;
	CTimeController* m_pTimeController;
	GameManager* m_GM;

	// 씬 구조들
	LogoScene*   m_pLogoScene;
	TitleScene* m_pTitleScene;
	SelectScene* m_pSeletScene;
	InGameScene* m_pInGame;
	ResultScene* m_pResultScene;
	SettingScene* m_pSettingScene;

	vector<IScene*> m_vec_Scene;


	eGameScene m_PrevScene;
	eGameScene m_NowScene;

	float m_DeltaTime;

	int m_Winner;
	int m_SoundCount = 1; // 배경음악 루프를 한번만 돌도록 함

public:
	void Initialize();
	void FiniteStateMachine();

	void SetNowScene(eGameScene sceneName) { m_NowScene = sceneName;  }
	void SetWinner(int carIndex) { m_Winner = carIndex;}
	int GetWinner() { return m_Winner; }
	GameManager* GetGameManger() { return m_GM; }
	InGameScene* GetInGame() { return m_pInGame; }

private:
	void OnStart();
	void SceneKeyUpdate();
};

