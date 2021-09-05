#pragma once
#include "GlobalDefine.h"

using namespace std;

#define MAX_LOADSTRING 100

// ���� ����
//WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
//WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

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

	// FPS ���� ����
	float m_DeltaTime;
	float m_SecondCheck;
	int m_FrameCount;
	int m_IntShowCount;
	const float ONE_SECOND = 1.0f;

public:
	// ������Ʈ �Ŵ���
	ObjectManager* m_pObjManager;

	PlayerManager* m_pPlayerManager;

	// �� Ŭ����
	Map* m_pMap;

	// ���̽� �Ŵ��� Ŭ����
	RaceManager* m_pRaceManager;

	// ī�޶� �Ŵ��� Ŭ����
	CameraManager* m_pCamera;
	SceneManager* m_pSceneManager;

	eGameTrack m_NowTrack;

	map<int, CarMK3*> m_Pad;

private:
	// Ű �Է�
	XboxPadInput* m_pUserInput;

	// �׽�Ʈ�� �׸��尡 �ִ� �� (��Ʈ�� 1���̸�, ��ü������ ����δ�..)
	CGameObject* m_debugGrid;

	/// ������ ������ On/Off
	// ������Ʈ ����
	bool m_TransformInfo;
	bool m_VectorInfo;
	bool m_Friction;
	bool m_DebugAll;
	bool m_CarVariable;

	// �� ����
	bool m_MakeIsomatric;
	bool m_ShowGrid;

	/// Sound ����
	SoundManager* m_BGMManager;
	SoundManager* m_EffectManager;
	

public:
	/// ���� ���� �� �ʱ�ȭ ����
	void Initialize(HINSTANCE hInst);	
	void ObjectInitializer();		// ���� ������Ʈ���� �ҷ��´�.
	void ModelObejctCreate(eGameTrack track);

	void CameraUpdate(CarMK3* target);

	/// ���� ���� �� ����
	void Looping();					// PeekMessage�� ���� ���� ���ν����� �ݺ��ϴ� ����
	void Finish();					// ���� ���� �� ����

private:

	/// ���� FSM
	/*
	void GameFSM();
	void GameProc();

	void UpdateAll();		// ������Ʈ���� ������ �� Ű�Է�, �ݸ��� üũ ��
	void RenderAll();		// ��� �͵��� �������ϴ� �Լ�

	void KeyUpdate(CABON_Engine* m_pEngine);
	void PadUpdate();
	void RaceInfoUpdate();
	void ShowDebug(float dTime);
	*/

public:
	XboxPadInput* GetLehideInput() { return m_pUserInput; }
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

