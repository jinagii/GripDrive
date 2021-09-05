#pragma once
#include "GlobalDefine.h"

using namespace std;

#define MAX_LOADSTRING 100

// 전역 변수
//WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
//WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

class D2DEngine;
class CTimeController;
class XboxPadInput;
class Map;
class RaceManager;
class CameraManager;
class CGameObject;
class CarMK3;
class SoundManager;
class ObjectManager;
class SceneManager;
class PlayerManager;

enum eGameTrack
{
	eTrack1 = 1,
	eTrack2,
};

enum ePlayerIndex
{
	Player1 = 0,
	Player2,
	Player3,
	Player4,
};

class GameManager
{
	const wchar_t* szTitle = L"Arthur's Fighters Demo";
	const wchar_t* szWindowClass = L"Fighters Demo";

	D2DEngine* m_pEngine;
	CTimeController* m_pTimeController;

private:
	HWND m_hWnd;

	// FPS 관련 변수
	float m_DeltaTime;
	float m_SecondCheck;
	int m_FrameCount;
	int m_IntShowCount;
	const float ONE_SECOND = 1.0f;

public:
	// 오브젝트 매니저
	ObjectManager* m_pObjManager;

	PlayerManager* m_pPlayerManager;

	// 맵 클래스
	Map* m_pMap;

	// 레이스 매니저 클래스
	RaceManager* m_pRaceManager;

	// 카메라 매니저 클래스
	CameraManager* m_pCamera;
	SceneManager* m_pSceneManager;

	eGameTrack m_NowTrack;

	map<int, CarMK3*> m_Pad;

private:
	// 키 입력
	XboxPadInput* m_pUserInput;

	// 테스트용 그리드가 있는 맵 (비트맵 1장이며, 자체적으로 기울인다..)
	CGameObject* m_debugGrid;

	/// 디버깅용 정보들 On/Off
	// 오브젝트 관련
	bool m_TransformInfo;
	bool m_VectorInfo;
	bool m_Friction;
	bool m_DebugAll;
	bool m_CarVariable;

	// 맵 관련
	bool m_MakeIsomatric;
	bool m_ShowGrid;

	/// Sound 관련
	SoundManager* m_BGMManager;
	SoundManager* m_EffectManager;
	

public:
	/// 게임 시작 시 초기화 관련
	void Initialize(HINSTANCE hInst);	
	void ObjectInitializer();		// 게임 오브젝트들을 불러온다.
	void ModelObejctCreate(eGameTrack track);

	void CameraUpdate(CarMK3* target);

	/// 게임 실행 및 종료
	void Looping();					// PeekMessage와 실제 게임 프로시저를 반복하는 구간
	void Finish();					// 게임 종료 시 실행

private:

	/// 게임 FSM
	/*
	void GameFSM();
	void GameProc();

	void UpdateAll();		// 오브젝트들의 움직임 및 키입력, 콜리젼 체크 등
	void RenderAll();		// 모든 것들을 렌더링하는 함수

	void KeyUpdate(CABON_Engine* m_pEngine);
	void PadUpdate();
	void RaceInfoUpdate();
	void ShowDebug(float dTime);
	*/

public:
	XboxPadInput* GetLehideInput() { return m_pUserInput; }
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

