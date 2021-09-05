#include "ResultScene.h"

#include "../DRCommonLib/LHInput.h"
#include "D2DEngine.h"
#include "SceneManager.h"
#include "KeyInput.h"
#include "SoundManager.h"


ResultScene::ResultScene()
	:m_IsExit(false),
	m_Winner(-1)
{

}

ResultScene::~ResultScene()
{

}

void ResultScene::Initialize(D2DEngine* pEngine, SceneManager* pScene)
{
	m_pSceneManager = pScene;

	m_Winner1Bitmap = pEngine->LoadBitmapByPath(L"./data/ui/player1.png");
	m_Winner2Bitmap = pEngine->LoadBitmapByPath(L"./data/ui/player2.png");
	m_Winner3Bitmap = pEngine->LoadBitmapByPath(L"./data/ui/player3.png");
	m_Winner4Bitmap = pEngine->LoadBitmapByPath(L"./data/ui/player4.png");

	// 팝업창
	m_Result_PopupBitmap = pEngine->LoadBitmapByPath(L"./data/ui/Result_Popup.png");

	// UI 버튼
	m_Result_Select_0_Bitmap = pEngine->LoadBitmapByPath(L"./data/ui/Result_Select_0.png");
	m_Result_Select_1_Bitmap = pEngine->LoadBitmapByPath(L"./data/ui/Result_Select_1.png");
	m_Result_Restart_0_Bitmap = pEngine->LoadBitmapByPath(L"./data/ui/Result_Restart_0.png");
	m_Result_Restart_1_Bitmap = pEngine->LoadBitmapByPath(L"./data/ui/Result_Restart_1.png");
	m_Result_Quit_0_Bitmap = pEngine->LoadBitmapByPath(L"./data/ui/Result_Quit_0.png");
	m_Result_Quit_1_Bitmap = pEngine->LoadBitmapByPath(L"./data/ui/Result_Quit_1.png");
}

void ResultScene::OnStart()
{

}

void ResultScene::OnUpdate(float dTime)
{
	m_Winner = m_pSceneManager->GetWinner();

	KeyUpdate(dTime);
}

void ResultScene::OnDraw(D2DEngine* pEngine, float dTime)
{
	pEngine->BeginRender();

	switch (m_Winner)
	{
		case 0:
			pEngine->ImageDrawByPoint(m_Winner1Bitmap, D2D1::Point2F(640, 50));
			break;
		case 1:
			pEngine->DrawUIImage(m_Winner2Bitmap);
			break;
		case 2:
			pEngine->DrawUIImage(m_Winner3Bitmap);
			break;
		case 3:
			pEngine->DrawUIImage(m_Winner4Bitmap);
			break;
		default:
			break;
	}

	/*float _lapTime_Player2 = m_RaceManager
	float _lapTime_Player3 = m_RaceManager
	float _lapTime_Player4 = m_RaceManager*/

	/// 자동차의 시간이 들어와햐 함. 
	//pEngine->DrawText(D2D1::Point2F(100, 100), L"%d",);
	//pEngine->DrawText(D2D1::Point2F(100, 110), L"%d", );
	//pEngine->DrawText(D2D1::Point2F(100, 120), L"%d", );
	//pEngine->DrawText(D2D1::Point2F(100, 130), L"%d", );

	//pEngine->ImageDrawByPoint(m_Result_PopupBitmap, D2D1::Point2F(640, 50));
	//pEngine->ImageDrawByPoint(m_Result_Quit_0_Bitmap, D2D1::Point2F(772, 616));
	//pEngine->ImageDrawByPoint(m_Result_Quit_1_Bitmap, D2D1::Point2F(772, 616)); // 클릭하면 이 이미지가 나와야함.
	//pEngine->ImageDrawByPoint(m_Result_Restart_0_Bitmap, D2D1::Point2F(772, 746));
	//pEngine->ImageDrawByPoint(m_Result_Restart_1_Bitmap, D2D1::Point2F(772, 746));
	//pEngine->ImageDrawByPoint(m_Result_Select_0_Bitmap, D2D1::Point2F(772, 876));
	//pEngine->ImageDrawByPoint(m_Result_Select_1_Bitmap, D2D1::Point2F(772, 876));

	pEngine->ImageDrawByScale(m_Result_PopupBitmap, D2D1::Point2F(640, 50), D2D1::SizeF(0.5f, 0.5f));

	if (m_IsExit)
	{
		pEngine->ImageDrawByScale(m_Result_Quit_1_Bitmap, D2D1::Point2F(1008, 1040), D2D1::SizeF(0.5f, 0.5f));
		pEngine->ImageDrawByScale(m_Result_Select_0_Bitmap, D2D1::Point2F(1008, 824), D2D1::SizeF(0.5f, 0.5f));
	}
	else
	{
		pEngine->ImageDrawByScale(m_Result_Select_1_Bitmap, D2D1::Point2F(1008, 824), D2D1::SizeF(0.5f, 0.5f));
		pEngine->ImageDrawByScale(m_Result_Quit_0_Bitmap, D2D1::Point2F(1008, 1040), D2D1::SizeF(0.5f, 0.5f));
	}

	pEngine->EndRender();

}

void ResultScene::ChangeScene()
{
	if (KeyInput::InputKeyUp(VK_RETURN))
	{
		if (m_IsExit)
		{
			SoundManager::GetInstance()->PlayAuto(eSoundName::UI_Click);
			m_pSceneManager->SetNowScene(eExit);
			PostQuitMessage(0);
		}
		else
		{
			SoundManager::GetInstance()->PlayAuto(eSoundName::UI_Click);
			m_pSceneManager->SetNowScene(eTitleScene);
		}
	}
}

void ResultScene::KeyUpdate(float dTime)
{
	if (KeyInput::InputKeyUp(VK_UP))
	{
		m_IsExit = !m_IsExit;
		SoundManager::GetInstance()->PlayAuto(eSoundName::UI_Move);
	}
	if (KeyInput::InputKeyUp(VK_DOWN))
	{
		m_IsExit = !m_IsExit;
		SoundManager::GetInstance()->PlayAuto(eSoundName::UI_Move);
	}
}

