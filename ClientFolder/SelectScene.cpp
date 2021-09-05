#include "SelectScene.h"
#include "D2DEngine.h"
#include "KeyInput.h"
#include "../DRCommonLib/XboxPadInput.h"
#include "ObjectManager.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "GameManager.h"
#include "CarMK3.h"

#pragma comment(lib, "../Debug/DRCommonLib.lib")

SelectScene::SelectScene()
	:m_Player1(0),
	m_Player2(0),
	m_Player3(0),
	m_Player4(0),
	m_CountTime(0),
	m_bCarSelect(false),
	m_SelectTrack(0)
{

}

SelectScene::~SelectScene()
{

}

void SelectScene::Initialize(D2DEngine* pEngine, SceneManager* pScene)
{
	m_pSceneManager = pScene;

	m_GM = pScene->GetGameManger();

	m_pUserInput = m_GM->GetLehideInput();

	m_pObjmanager = m_GM->m_pObjManager;

	m_RaceStart = pEngine->LoadBitmapByPath(L"./data/ui/RaceStart.png");

	m_CarImage1 = pEngine->LoadBitmapByPath(L"./data/ui/Car1Select.png");
	m_CarImage2 = pEngine->LoadBitmapByPath(L"./data/ui/Car2Select.png");
	m_CarImage3 = pEngine->LoadBitmapByPath(L"./data/ui/Car3Select.png");
	m_CarImage4 = pEngine->LoadBitmapByPath(L"./data/ui/Car4Select.png");

	m_TransparentBG1 = pEngine->LoadBitmapByPath(L"./data/ui/TrackSeletPopup.png");
	m_TransparentBG2 = pEngine->LoadBitmapByPath(L"./data/ui/CarSeletPopup.png");


	m_PlayerBox1 = pEngine->LoadBitmapByPath(L"./data/ui/Player1Box.png");
	m_PlayerBox2 = pEngine->LoadBitmapByPath(L"./data/ui/Player2Box.png");
	m_PlayerBox3 = pEngine->LoadBitmapByPath(L"./data/ui/Player3Box.png");
	m_PlayerBox4 = pEngine->LoadBitmapByPath(L"./data/ui/Player4Box.png");

	m_Track1 = pEngine->LoadBitmapByPath(L"./data/ui/Track1Thum.png");
	m_Track2 = pEngine->LoadBitmapByPath(L"./data/ui/Track2Thum.png");

	m_pTrackSelect = pEngine->LoadBitmapByPath(L"./data/ui/TrackSelect.png");
	m_TrackSelect2 = pEngine->LoadBitmapByPath(L"./data/ui/TrackSelectCian.png");

	m_Setting = pEngine->LoadBitmapByPath(L"./data/ui/setting.png");

	D2D1_POINT_2F _Car1Pos = D2D1::Point2F(375, 600);
	D2D1_POINT_2F _Car2Pos = D2D1::Point2F(840, 600);
	D2D1_POINT_2F _Car3Pos = D2D1::Point2F(1305, 600);
	D2D1_POINT_2F _Car4Pos = D2D1::Point2F(1770, 600);

	v_CarPos.push_back(_Car1Pos);
	v_CarPos.push_back(_Car2Pos);
	v_CarPos.push_back(_Car3Pos);
	v_CarPos.push_back(_Car4Pos);

	D2D1_POINT_2F _Track1Pos = D2D1::Point2F(640, 200);
	D2D1_POINT_2F _Track2Pos = D2D1::Point2F(1520, 200);

	v_TrackPos.push_back(_Track1Pos);
	v_TrackPos.push_back(_Track2Pos);
}

void SelectScene::OnStart()
{

}

void SelectScene::OnUpdate(float dTime)
{
	KeyUpdate(dTime);
}

void SelectScene::OnDraw(D2DEngine* pEngine, float dTime)
{
	pEngine->BeginRender();



	pEngine->ImageDrawByPoint(m_Track1, v_TrackPos[0], 1.0f);
	pEngine->ImageDrawByPoint(m_Track2, v_TrackPos[1], 1.0f);
	if (!m_bCarSelect)
	{
		pEngine->ImageDrawByPoint(m_TrackSelect2, v_TrackPos[m_SelectTrack % 2], 1.0f);
	}
	else
	{
		pEngine->ImageDrawByPoint(m_pTrackSelect, v_TrackPos[m_SelectTrack % 2], 1.0f);
	}

	pEngine->ImageDrawByScale(m_PlayerBox1, v_CarPos[m_Player1 % 4], D2D1::SizeF(0.5f, 0.5f));
	pEngine->ImageDrawByScale(m_PlayerBox2, v_CarPos[m_Player2 % 4], D2D1::SizeF(0.5f, 0.5f));
	pEngine->ImageDrawByScale(m_PlayerBox3, v_CarPos[m_Player3 % 4], D2D1::SizeF(0.5f, 0.5f));
	pEngine->ImageDrawByScale(m_PlayerBox4, v_CarPos[m_Player4 % 4], D2D1::SizeF(0.5f, 0.5f));

	// transparent background
	pEngine->ImageDrawByScale(m_TransparentBG1, D2D1::Point2F(252, 83), D2D1::SizeF(1.2f, 1.0f));
	pEngine->ImageDrawByScale(m_TransparentBG2, D2D1::Point2F(252, 594), D2D1::SizeF(1.2f, 1.0f));

	// trackimage
	pEngine->ImageDrawByPoint(m_TrackIamge1, D2D1::Point2F(570, 105));
	pEngine->ImageDrawByPoint(m_TrackIamge2, D2D1::Point2F(1447, 105));
	//
	//// car image
	pEngine->ImageDrawByPoint(m_CarImage1, D2D1::Point2F(430, 650));
	pEngine->ImageDrawByPoint(m_CarImage2, D2D1::Point2F(900, 650));
	pEngine->ImageDrawByPoint(m_CarImage3, D2D1::Point2F(1365, 650));
	pEngine->ImageDrawByPoint(m_CarImage4, D2D1::Point2F(1830, 650));
	//
	//// attribute
	pEngine->ImageDrawByPoint(m_CarAttr1, D2D1::Point2F(281, 1042));
	pEngine->ImageDrawByPoint(m_CarAttr2, D2D1::Point2F(779, 1042));
	pEngine->ImageDrawByPoint(m_CarAttr3, D2D1::Point2F(1287, 1042));
	pEngine->ImageDrawByPoint(m_CarAttr4, D2D1::Point2F(1795, 1042));
	//
	//// start image
	pEngine->ImageDrawByPoint(m_RaceStart, D2D1::Point2F(1100, 1256));

	pEngine->ImageDrawByScale(m_Setting, D2D1::Point2F(2400, 10), D2D1::SizeF(0.8f, 0.8f));

	pEngine->DrawText(D2D1::Point2F(100, 100), L"%d", m_Player1);
	pEngine->DrawText(D2D1::Point2F(100, 110), L"%d", m_Player2);
	pEngine->DrawText(D2D1::Point2F(100, 120), L"%d", m_Player3);
	pEngine->DrawText(D2D1::Point2F(100, 130), L"%d", m_Player4);

	m_CountTime += dTime;


	pEngine->EndRender();
}



void SelectScene::ChangeScene()
{
	
	/*if (m_Player1 != m_Player2 &&
		m_Player2 != m_Player3 &&
		m_Player3 != m_Player4 &&
		m_Player4 != m_Player1 )
	{*/
		//if (CABON_Input::InputKeyUp(VK_RETURN))
		if (m_pUserInput->GetKeyDown(0, XINPUT_GAMEPAD_START) || KeyInput::InputKeyUp(VK_RETURN))
		{
			m_pSceneManager->SetNowScene(eGameProcedure);
			SoundManager::GetInstance()->SetSoundPlaying(eSoundName::UI_Click, false);
			SoundManager::GetInstance()->PlayOnce(eSoundName::UI_Click);

			m_GM->m_Pad.clear();

			m_GM->m_Pad.insert({ ePlayerIndex::Player1, m_pObjmanager->GetCars(m_Player1) });
			m_GM->m_Pad.insert({ ePlayerIndex::Player2, m_pObjmanager->GetCars(m_Player2) });
			m_GM->m_Pad.insert({ ePlayerIndex::Player3, m_pObjmanager->GetCars(m_Player3) });
			m_GM->m_Pad.insert({ ePlayerIndex::Player4, m_pObjmanager->GetCars(m_Player4) });

			if (m_SelectTrack % 2 == 0)
			{
				m_GM->m_NowTrack = eTrack1;
			}
			else
			{
				m_GM->m_NowTrack = eTrack2;
			}
		}
	//}
}

void SelectScene::KeyUpdate(float dTime)
{
	static float _countSec=0;
	_countSec += dTime;
	if (_countSec > 0.2)
	{
		m_pUserInput->Update();
	}

	if (KeyInput::InputKeyUp(VK_UP))
	{
		m_bCarSelect = !m_bCarSelect;
		SoundManager::GetInstance()->PlayAuto(eSoundName::UI_Move);
	}
	if (KeyInput::InputKeyUp(VK_DOWN))
	{
		m_bCarSelect = !m_bCarSelect;
		SoundManager::GetInstance()->PlayAuto(eSoundName::UI_Move);

	}
	if (KeyInput::InputKeyUp(VK_RIGHT))
	{
		if (!m_bCarSelect) m_SelectTrack++;
		else
		{
			m_Player1++;
			if (m_Player1 < 0) m_Player1 = 0;
		}
		SoundManager::GetInstance()->PlayAuto(eSoundName::UI_Move);
	}
	if (KeyInput::InputKeyUp(VK_LEFT))
	{
		if (!m_bCarSelect) m_SelectTrack--;
		else
		{
			m_Player1--;
			if (m_Player1 < 0) m_Player1 = 0;
		}
		SoundManager::GetInstance()->PlayAuto(eSoundName::UI_Move);
	}

	m_CountTime += dTime;

	if (m_CountTime > 0.2)
	{
		m_CountTime = 0;
	}
	else
	{
		return;
	}

	for (int i = 0; i < 4; i++)
	{
		if (m_pUserInput->GetKeyDown(i, XINPUT_GAMEPAD_DPAD_UP))
		{
			SoundManager::GetInstance()->PlayAuto(eSoundName::UI_Move);
			switch (i)
			{
				case ePlayerIndex::Player1:
				{
					m_bCarSelect = !m_bCarSelect;
					break;
				}
				case ePlayerIndex::Player2:
				{
					m_Player2++;
					break;
				}
				case ePlayerIndex::Player3:
				{
					m_Player3++;
					break;
				}
				case ePlayerIndex::Player4:
				{
					m_Player4++;
					break;
				}
			}
		}
		if (m_pUserInput->GetKeyDown(i, XINPUT_GAMEPAD_DPAD_DOWN))
		{
			SoundManager::GetInstance()->PlayAuto(eSoundName::UI_Move);
			switch (i)
			{
				case ePlayerIndex::Player1:
				{
					m_bCarSelect = !m_bCarSelect;
					break;
				}
				case ePlayerIndex::Player2:
				{
					m_Player2--;
					if (m_Player2 < 0) m_Player2 = 0;
					break;
				}
				case ePlayerIndex::Player3:
				{
					m_Player3--;
					if (m_Player2 < 0) m_Player3 = 0;
					break;
				}
				case ePlayerIndex::Player4:
				{
					m_Player4--;
					if (m_Player2 < 0) m_Player4 = 0;
					break;
				}
			}
		}
		if (m_pUserInput->GetKeyDown(i, XINPUT_GAMEPAD_DPAD_LEFT))
		{
			SoundManager::GetInstance()->PlayAuto(eSoundName::UI_Move);
			switch (i)
			{
				case ePlayerIndex::Player1:
				{
					if (!m_bCarSelect) 
					{
						m_SelectTrack--;
						if (m_SelectTrack < 0) m_SelectTrack = 0;
					}					
					else
					{
						m_Player1--;
						if (m_Player1 < 0) m_Player1 = 0;
					}
					break;
				}
				case ePlayerIndex::Player2:
				{
					m_Player2--;
					if (m_Player2 < 0) m_Player2 = 0;
					break;
				}
				case ePlayerIndex::Player3:
				{
					m_Player3--;
					if (m_Player2 < 0) m_Player3 = 0;
					break;
				}
				case ePlayerIndex::Player4:
				{
					m_Player4--;
					if (m_Player2 < 0) m_Player4 = 0;
					break;
				}
			}
		}
		if (m_pUserInput->GetKeyDown(i, XINPUT_GAMEPAD_DPAD_RIGHT))
		{
			SoundManager::GetInstance()->PlayAuto(eSoundName::UI_Move);
			switch (i)
			{
				case ePlayerIndex::Player1:
				{
					if (!m_bCarSelect) 
					{
						m_SelectTrack++;
						if (m_SelectTrack < 0) m_SelectTrack = 0;
					}					
					else
					{
						m_Player1++;
						if (m_Player1 < 0) m_Player1 = 0;
					}
					break;
				}
				case ePlayerIndex::Player2:
				{
					m_Player2++;
					break;
				}
				case ePlayerIndex::Player3:
				{
					m_Player3++;
					break;
				}
				case ePlayerIndex::Player4:
				{
					m_Player4++;
					break;
				}
			}
		}
	}
}
