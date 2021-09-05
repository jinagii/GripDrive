#include "InGameScene.h"


#include "D2DEngine.h"
#include "CarMK3.h"
#include "GameManager.h"
#include "CameraManager.h"
#include "ObjectManager.h"
#include "Map.h"
#include "RaceManager.h"
#include "SceneManager.h"
#include "KeyInput.h"
#include "CTransform.h"
#include "SoundManager.h"
#include "SettingScene.h"

InGameScene::InGameScene()
	:m_GM(nullptr),
	m_pEngine(nullptr),
	m_pObjManager(nullptr),
	m_pMap(nullptr),
	m_pRaceManager(nullptr),
	m_pCamera(nullptr),
	m_TransformInfo(false),
	m_VectorInfo(false),
	m_Friction(false),
	m_DebugAll(false),
	m_CarVariable(false),
	m_MakeIsomatric(true),
	m_ShowGrid(false),
	m_SecondCheck(0),
	m_IntShowCount(0),
	m_FrameCount(0),
	m_StartCount(0)
{

}

InGameScene::~InGameScene()
{

}


void InGameScene::Initialize(D2DEngine* pEngine, SceneManager* pScene)
{
	m_pEngine = pEngine;
	m_pSceneManager = pScene;
	m_GM = pScene->GetGameManger();

	m_pObjManager = m_GM->m_pObjManager;
	m_pCamera = m_GM->m_pCamera;

	// 스타팅 카운트 이미지
	m_pNumber3 = pEngine->LoadBitmapByPath(L"./data/3.png");
	m_pNumber2 = pEngine->LoadBitmapByPath(L"./data/2.png");
	m_pNumber1 = pEngine->LoadBitmapByPath(L"./data/1.png");
	m_pGo = pEngine->LoadBitmapByPath(L"./data/GO.png");

	m_Rank1 = pEngine->LoadBitmapByPath(L"./data/ui/rank1.png");
	m_Rank2 = pEngine->LoadBitmapByPath(L"./data/ui/rank2.png");
	m_Rank3 = pEngine->LoadBitmapByPath(L"./data/ui/rank3.png");
	m_Rank4 = pEngine->LoadBitmapByPath(L"./data/ui/rank4.png");

	v_StartingCount.push_back(m_pNumber3);
	v_StartingCount.push_back(m_pNumber2);
	v_StartingCount.push_back(m_pNumber1);
	v_StartingCount.push_back(m_pGo);

	m_Setting = new SettingScene();
	m_Setting->Initialize(pEngine, pScene);

}

void InGameScene::OnStart()
{
	// 이전에 실행 중인 노래가 있으면 끈다
	// 원하는 노래를 튼다

	switch (m_GM->m_NowTrack)
	{
	case eGameTrack::eTrack1:
	{
		m_pObjManager->Initialize(eTrack1);
		m_GM->ModelObejctCreate(eTrack1);
		m_pMap = new Map(m_pEngine, eTrack1);
		break;
	}
	case eGameTrack::eTrack2:
	{
		m_pObjManager->Initialize(eTrack2);
		m_GM->ModelObejctCreate(eTrack2);
		m_pMap = new Map(m_pEngine, eTrack2);

		break;
	}
	default:
		break;
	}

	//m_pObjManager->CreateCar(m_pEngine, 4, D2D1::Vector2F());

	// 재시작을 염두에 두고 여기서 모든 것을 초기화 해준다.
	// 위치 정보 초기화 등등

	if (m_pRaceManager != nullptr)
	{
		delete m_pRaceManager;
	}
	m_pRaceManager = m_pObjManager->RaceManagerInit(3);

	SoundManager::GetInstance()->StopBGM();
	SoundManager::GetInstance()->PlayBGM(eSoundName::Race_BGM);

	m_pObjManager->ResetObject();

	m_pObjManager->MakeCarReady(m_GM->m_NowTrack);
	m_pObjManager->SortingByAxisY();

	m_pObjManager->KeyMapping();
}

void InGameScene::OnUpdate(float dTime)
{

	if (m_ShowSetting)
	{
		bool m_setting = true;

		while (m_setting)
		{
			m_setting = m_Setting->OnUpdate(dTime);
			m_Setting->OnDraw(m_pEngine, dTime);
		}
	}

	m_StartCount += (dTime / 60);

	if (m_StartCount > 4)
	{
		m_pObjManager->InputUpdate();
		RaceInfoUpdate(dTime);

		// 모든 오브젝트를 업데이트
		m_pObjManager->UpdateAll(dTime);
		m_pObjManager->CollisionCheckAll();


		KeyUpdate();
	}
}

void InGameScene::OnDraw(D2DEngine* pEngine, float dTime)
{
	pEngine->BeginRender();

	pEngine->SetBrushColor(1.0f, 1.0f, 1.0f, 1.0f);

	/// 개발 도중, 탑뷰냐 아이소매트릭이냐
	if (m_MakeIsomatric)
	{
		m_pCamera->MakeIsometricTM(D2D1::Vector2F(1000, 0), 0.5f, 45.0f);
	}
	else
	{
		m_pCamera->MakeTopViewTM();
	}

	// 맵 (이미지)
	m_pMap->Draw(m_pEngine, m_pCamera->GetViewProjTM());
	
	// 모든 게임 오브젝트 그리기. 다형성 사용
	m_pObjManager->RenderAll(m_pEngine, dTime,
		m_pCamera->GetViewTM(), m_pCamera->GetProjTM(), m_MakeIsomatric);

	// 그리드 정보
	if (m_ShowGrid)
	{
		m_pObjManager->RenderDebug(m_pEngine, dTime,
			m_pCamera->GetViewTM(), m_pCamera->GetProjTM(), m_MakeIsomatric);
	}

	// 스타팅 카운트

	ShowDebug(dTime);

	pEngine->SetTransform(D2D1::Matrix3x2F::Identity());

	m_StartCount += dTime;
	int _secCount = m_StartCount;
	if (_secCount < 4)
	{
		pEngine->ImageDrawByPoint(v_StartingCount[_secCount], D2D1::Point2F(810, 150));

		if (_secCount < 3)
		{
			SoundManager::GetInstance()->PlayAuto(eSoundName::Count_Num);
		}
		else
		{
			SoundManager::GetInstance()->PlayAuto(eSoundName::Count_Go);
		}

	}

	m_pEngine->EndRender();
}


void InGameScene::ChangeScene()
{

}

void InGameScene::CameraUpdate()
{

	/// 카메라가 따라다닐 타겟(자동차)
	// 어떻게 위치를 가져와야할지 모르겠다...
	D2D1_VECTOR_2F _cameraCenter;
	//= m_pObjManager->GetCars(0)->GetTransform()->GetLocalPosition();

	_cameraCenter = m_pObjManager->GetCars(0)->GetTransform()->GetLocalPosition();

	//m_pCamera->SetNowCenter(_cameraCenter);

	/// 개발 도중, 탑뷰냐 아이소매트릭이냐
	if (m_MakeIsomatric)
	{
		m_pCamera->MakeIsometricTM(D2D1::Vector2F(1000, 0), 0.5f, 45.0f);
		//m_pCamera->MakeIsometricTM(_cameraCenter, 0.5f, 45.0f);
	}
	else
	{
		m_pCamera->MakeTopViewTM();
	}

	//m_pCamera->MoveHorizontal(_cameraCenter.x);
	//m_pCamera->MoveVertical(_cameraCenter.y);


	/// 선형 보간 lerp 곡선
	// 3가지 변수가 필요하다 	 
	//D2D1_VECTOR_2F _pos1;		// 처음 위치
	//D2D1_VECTOR_2F _pos2;		// 나중 위치
	//float factor = 0.5f;		// 중간에 보간될 위치

	//D2D1_VECTOR_2F _resultPos;	// 결과 위치
	//_resultPos = CVector2::VectorPlus(
	//	CVector2::VectorScalarMultiply(_pos1, 1.0f - factor),
	//	CVector2::VectorScalarMultiply(_pos2, factor));


}

void InGameScene::RaceInfoUpdate(float dTime)
{
	m_pRaceManager->CheckReached(dTime);
	m_pRaceManager->CalculateLapTime();
	m_pRaceManager->CalculateRank();


	int _winner = m_pRaceManager->GoToResult();

	if (_winner != -1)
	{
		m_pSceneManager->SetWinner(_winner);
		m_pSceneManager->SetNowScene(eResultScene);
	}

}

void InGameScene::ShowDebug(float dTime)
{
	// m_DeltaTime으로 어차피 시간 계산을 할 수 있다.
	m_SecondCheck += dTime;
	if (ONE_SECOND <= m_SecondCheck)
	{
		// 1초가 지났다.
		m_SecondCheck = 0;
		m_IntShowCount = m_FrameCount;
		m_FrameCount = 0;
	}
	else
	{
		m_FrameCount++;
	}

	// UI는 모두 화면에 바로 그린다.
	if (m_ShowFPS)
	{
		m_pEngine->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pEngine->DrawText(10, 10, L"FPS %d", m_IntShowCount);
	}


	if (m_TransformInfo)
	{
		// UI는 모두 화면에 바로 그린다.
		m_pObjManager->DrawTransformInfo();
	}

	if (m_VectorInfo)
	{
		m_pObjManager->DrawVectorInfo(m_pCamera->GetProjTM(), m_pCamera->GetViewTM(), m_MakeIsomatric);

	}

	// 차량 변수 출력
	if (m_CarVariable)
	{
		m_pObjManager->DrawCarsVariable();
	}

	// 레이스 정보 출력
	if (m_ShowRaceInfo)
	{
		m_pRaceManager->DrawCheckPoint(m_pEngine, m_pCamera->GetViewTM(), m_pCamera->GetProjTM(), m_MakeIsomatric);
		m_pRaceManager->ShowScoreInfo(m_pEngine);
		m_pRaceManager->ShowRank(m_pEngine);
	}
}


void InGameScene::KeyUpdate()
{

	/// 디버깅 정보 출력 -----------------------------------------------

	if (KeyInput::InputKeyUp(VK_F1))
	{
		m_TransformInfo = !m_TransformInfo;
		m_CarVariable = !m_CarVariable;
	}

	if (KeyInput::InputKeyUp(VK_F2))
	{
		m_VectorInfo = !m_VectorInfo;
		m_Friction = !m_Friction;
	}

	if (KeyInput::InputKeyUp(VK_F3))
	{
		m_ShowGrid = !m_ShowGrid;
	}

	if (KeyInput::InputKeyUp(VK_F4))
	{
		m_ShowRaceInfo = !m_ShowRaceInfo;
	}

	if (KeyInput::InputKeyUp(VK_F5))
	{
		m_MakeIsomatric = !m_MakeIsomatric;
	}

	if (KeyInput::InputKeyUp(VK_F6))
	{
		m_ShowFPS = !m_ShowFPS;
	}


	//CameraManager* _Camera = dynamic_cast<CameraManager*>(m_pCamera);


	if (KeyInput::InputKey(VK_W))
	{
		m_pCamera->MoveVertical(-10);
	}
	if (KeyInput::InputKey(VK_S))
	{
		m_pCamera->MoveVertical(10);

	}
	if (KeyInput::InputKey(VK_A))
	{
		m_pCamera->MoveHorizontal(-10);

	}
	if (KeyInput::InputKey(VK_D))
	{
		m_pCamera->MoveHorizontal(10);

	}
	if (KeyInput::InputKey(VK_Q))
	{
		m_pCamera->Zoom(0.01);
	}
	if (KeyInput::InputKey(VK_E))
	{
		m_pCamera->Zoom(-0.01);
	}


	/// 종료 -----------------------------------------------

	if (KeyInput::InputKey(VK_ESCAPE))
	{

		m_ShowSetting = !m_ShowSetting;


	}
}