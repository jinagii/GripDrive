#pragma once

#include "IScene.h"

class CarMK3;
class GameManager;
class D2DEngine;
class ObjectManager;
class Map;
class RaceManager;
class CameraManager;
class SettingScene;

class InGameScene : public IScene
{
public:
	InGameScene();
	~InGameScene();

public:
	virtual void Initialize(D2DEngine* pEngine, SceneManager* pScene) override;
	virtual void OnStart() override;
	virtual void OnUpdate(float dTime) override;
	virtual void OnDraw(D2DEngine* pEngine, float dTime) override;
	virtual void ChangeScene() override;

	void CameraUpdate();

	void SetShowSetting(bool val) { m_ShowSetting = val; }

private:
	void RaceInfoUpdate(float dTime);
	void ShowDebug(float dTime);
	void KeyUpdate();


private:
	GameManager* m_GM;
	D2DEngine* m_pEngine;
	ObjectManager* m_pObjManager;
	Map* m_pMap;
	RaceManager* m_pRaceManager;
	CameraManager* m_pCamera;
	SettingScene* m_Setting;

	ID2D1Bitmap* m_pNumber1;
	ID2D1Bitmap* m_pNumber2;
	ID2D1Bitmap* m_pNumber3;
	ID2D1Bitmap* m_pGo;


	ID2D1Bitmap* m_Rank1;
	ID2D1Bitmap* m_Rank2;
	ID2D1Bitmap* m_Rank3;
	ID2D1Bitmap* m_Rank4;

	vector<ID2D1Bitmap*> v_StartingCount;

	/// 디버깅용 정보들 On/Off
	// 오브젝트 관련
	bool m_TransformInfo;
	bool m_VectorInfo;
	bool m_Friction;
	bool m_DebugAll;
	bool m_CarVariable;
	bool m_ShowFPS;
	bool m_ShowRaceInfo;

	// 맵 관련
	bool m_MakeIsomatric;
	bool m_ShowGrid;

	bool m_ShowSetting;

	/// FPS 계산 정보
	const float ONE_SECOND = 1.0f;

	float m_SecondCheck;
	float m_StartCount;
	int m_IntShowCount;
	int m_FrameCount;

};

