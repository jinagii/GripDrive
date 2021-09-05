#pragma once

#include "GlobalDefine.h"
class D2DEngine;
class CTimeController;
class GameManager;

// IScene �� �ڽ� Ŭ������
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
	// �� �Ŵ��� �����ΰ� �ʿ��� ��Ʈ���� ���, �����ֱ� ���� ������ �־���.
	D2DEngine* m_pEngine;
	CTimeController* m_pTimeController;
	GameManager* m_GM;

	// �� ������
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
	int m_SoundCount = 1; // ������� ������ �ѹ��� ������ ��

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

