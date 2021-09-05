#include "ObjectManager.h"

#include "D2DEngine.h"
#include "GameManager.h"
#include "RaceManager.h"
#include "KeyInput.h"
#include "CarMK3.h"
#include "ModelObject.h"
#include "CDummy.h"
#include "Variables.h"
#include "atlstr.h"
#include "ICollisionObject.h"
#include "SoundManager.h"
#include "CTransform.h"
//test
#include "CVector.h"
#include <algorithm>

#include "../DRCommonLib/XboxPadInput.h"
#pragma comment(lib, "../Debug/DRCommonLib.lib")



ObjectManager::ObjectManager(GameManager* gameManager)
{
	m_pEngine = D2DEngine::Instance();

	m_GM = gameManager;
}

ObjectManager::~ObjectManager()
{

}

void ObjectManager::Initialize(eGameTrack track)
{

	switch (track)
	{
		case eTrack1:
		{
			// 준비된 비트맵 데이터를 수동으로 넣어준다
			// 해당 순서는 ModelObject의 enum class에 ObjectName으로 정의되어 있다.
			ID2D1Bitmap* _tree = m_pEngine->LoadBitmapByPath(L"./data/object/tree1.png");
			ID2D1Bitmap* _cone = m_pEngine->LoadBitmapByPath(L"./data/object/Cone.png");
			ID2D1Bitmap* _tires = m_pEngine->LoadBitmapByPath(L"./data/object/Tires.png");
			ID2D1Bitmap* _barricade30 = m_pEngine->LoadBitmapByPath(L"./data/object/0030.png");
			ID2D1Bitmap* _barricade35 = m_pEngine->LoadBitmapByPath(L"./data/object/0035.png");
			ID2D1Bitmap* _barricade80 = m_pEngine->LoadBitmapByPath(L"./data/object/0080.png");
			ID2D1Bitmap* _barricade95 = m_pEngine->LoadBitmapByPath(L"./data/object/0095.png");
			ID2D1Bitmap* _barricade100 = m_pEngine->LoadBitmapByPath(L"./data/object/0100.png");
			ID2D1Bitmap* _barricade130 = m_pEngine->LoadBitmapByPath(L"./data/object/0130.png");
			ID2D1Bitmap* _barricade150 = m_pEngine->LoadBitmapByPath(L"./data/object/0150.png");
			ID2D1Bitmap* _barricade155 = m_pEngine->LoadBitmapByPath(L"./data/object/0155.png");
			ID2D1Bitmap* _fence = m_pEngine->LoadBitmapByPath(L"./data/object/WD.png");

			ID2D1Bitmap* _display = m_pEngine->LoadBitmapByPath(L"./data/object/Display.png");
			ID2D1Bitmap* _garage = m_pEngine->LoadBitmapByPath(L"./data/object/Garage.png");
			ID2D1Bitmap* _lamp = m_pEngine->LoadBitmapByPath(L"./data/object/Lamp.png");
			ID2D1Bitmap* _forest = m_pEngine->LoadBitmapByPath(L"./data/object/Forset.png");
			ID2D1Bitmap* _audience = m_pEngine->LoadBitmapByPath(L"./data/object/Audience.png");


			v_SpriteType.push_back(_tree);
			v_SpriteType.push_back(_cone);
			v_SpriteType.push_back(_tires);
			v_SpriteType.push_back(_barricade30);
			v_SpriteType.push_back(_barricade35);
			v_SpriteType.push_back(_barricade80);
			v_SpriteType.push_back(_barricade95);
			v_SpriteType.push_back(_barricade100);
			v_SpriteType.push_back(_barricade130);
			v_SpriteType.push_back(_barricade150);
			v_SpriteType.push_back(_barricade155);
			v_SpriteType.push_back(_fence);
			
			v_SpriteType.push_back(_display);
			v_SpriteType.push_back(_garage);
			v_SpriteType.push_back(_lamp);
			v_SpriteType.push_back(_forest);
			v_SpriteType.push_back(_audience);

			break;
		}


		case eTrack2:
		{
			ID2D1Bitmap* _stone1 = m_pEngine->LoadBitmapByPath(L"./data/object/TR2/ston1.png");
			ID2D1Bitmap* _stone2 = m_pEngine->LoadBitmapByPath(L"./data/object/TR2/ston2.png");
			ID2D1Bitmap* _longchain = m_pEngine->LoadBitmapByPath(L"./data/object/TR2/ChainL.png");
			ID2D1Bitmap* _diagonalchain = m_pEngine->LoadBitmapByPath(L"./data/object/TR2/ChainSF.png");
			ID2D1Bitmap* _shortchain = m_pEngine->LoadBitmapByPath(L"./data/object/TR2/ChainS40.png");

			v_SpriteType.push_back(_stone1);
			v_SpriteType.push_back(_stone2);
			v_SpriteType.push_back(_longchain);
			v_SpriteType.push_back(_diagonalchain);
			v_SpriteType.push_back(_shortchain);

			break;
		}

	}
}

CGameObject* ObjectManager::CreateCar(D2DEngine* pEngine, int carIndex, D2D1_VECTOR_2F pos)
{
	CString str;

	str.Format((L"./data/sprite/Car%d/"), carIndex);

	CarMK3* _newCar = new CarMK3(carIndex + 1, pEngine, pos, str);

	v_Cars.push_back(_newCar);
	// 이제 오브젝트와 자동차는 따로 그린다.
	//m_vec_AllObjects.push_back(_newCar);
	return _newCar;
}

void ObjectManager::CreateModelObjectByName(D2D1_VECTOR_2F pos, eObject1Name name)
{
	switch (name)
	{
		case eObject1Name::TREE:
		{
			ModelObject* _newObj = new ModelObject(pos, v_SpriteType[(int)eObject1Name::TREE]);
			_newObj->Initialize(eObject1Name::TREE);
			m_vec_AllObjects.push_back(_newObj);
			break;
		}
		case eObject1Name::CONE:
		{
			ModelObject* _newObj = new ModelObject(pos, v_SpriteType[(int)eObject1Name::CONE]);
			_newObj->Initialize(eObject1Name::CONE);
			m_vec_AllObjects.push_back(_newObj);
			break;
		}
		case eObject1Name::TIRES:
		{
			ModelObject* _newObj = new ModelObject(pos, v_SpriteType[(int)eObject1Name::TIRES]);
			_newObj->Initialize(eObject1Name::TIRES);
			m_vec_AllObjects.push_back(_newObj);
			break;
		}
		case eObject1Name::BARRICADE30:
		{
			ModelObject* _newObj = new ModelObject(pos, v_SpriteType[(int)eObject1Name::BARRICADE30]);
			_newObj->Initialize(eObject1Name::BARRICADE30);
			m_vec_AllObjects.push_back(_newObj);
			break;
		}
		case eObject1Name::BARRICADE35:
		{
			ModelObject* _newObj = new ModelObject(pos, v_SpriteType[(int)eObject1Name::BARRICADE35]);
			_newObj->Initialize(eObject1Name::BARRICADE35);
			m_vec_AllObjects.push_back(_newObj);
			break;
		}
		case eObject1Name::BARRICADE80:
		{
			ModelObject* _newObj = new ModelObject(pos, v_SpriteType[(int)eObject1Name::BARRICADE80]);
			_newObj->Initialize(eObject1Name::BARRICADE80);
			m_vec_AllObjects.push_back(_newObj);
			break;
		}
		case eObject1Name::BARRICADE95:
		{
			ModelObject* _newObj = new ModelObject(pos, v_SpriteType[(int)eObject1Name::BARRICADE95]);
			_newObj->Initialize(eObject1Name::BARRICADE95);
			m_vec_AllObjects.push_back(_newObj);
			break;
		}
		case eObject1Name::BARRICADE100:
		{
			ModelObject* _newObj = new ModelObject(pos, v_SpriteType[(int)eObject1Name::BARRICADE100]);
			_newObj->Initialize(eObject1Name::BARRICADE100);
			m_vec_AllObjects.push_back(_newObj);
			break;
		}
		case eObject1Name::BARRICADE130:
		{
			ModelObject* _newObj = new ModelObject(pos, v_SpriteType[(int)eObject1Name::BARRICADE130]);
			_newObj->Initialize(eObject1Name::BARRICADE130);
			m_vec_AllObjects.push_back(_newObj);
			break;
		}
		case eObject1Name::BARRICADE150:
		{
			ModelObject* _newObj = new ModelObject(pos, v_SpriteType[(int)eObject1Name::BARRICADE150]);
			_newObj->Initialize(eObject1Name::BARRICADE150);
			m_vec_AllObjects.push_back(_newObj);
			break;
		}
		case eObject1Name::BARRICADE155:
		{
			ModelObject* _newObj = new ModelObject(pos, v_SpriteType[(int)eObject1Name::BARRICADE155]);
			_newObj->Initialize(eObject1Name::BARRICADE155);
			m_vec_AllObjects.push_back(_newObj);
			break;
		}
		case eObject1Name::FENCE:
		{
			ModelObject* _newObj = new ModelObject(pos, v_SpriteType[(int)eObject1Name::FENCE]);
			_newObj->Initialize(eObject1Name::FENCE);
			m_vec_AllObjects.push_back(_newObj);
			break;
		}		
		case eObject1Name::DISPLAY:
		{
			ModelObject* _newObj = new ModelObject(pos, v_SpriteType[(int)eObject1Name::DISPLAY]);
			_newObj->Initialize(eObject1Name::DISPLAY);
			m_vec_AllObjects.push_back(_newObj);
			break;
		}
		case eObject1Name::GARAGE:
		{
			ModelObject* _newObj = new ModelObject(pos, v_SpriteType[(int)eObject1Name::GARAGE]);
			_newObj->Initialize(eObject1Name::GARAGE);
			m_vec_AllObjects.push_back(_newObj);
			break;
		}
		case eObject1Name::LAMP:
		{
			ModelObject* _newObj = new ModelObject(pos, v_SpriteType[(int)eObject1Name::LAMP]);
			_newObj->Initialize(eObject1Name::LAMP);
			m_vec_AllObjects.push_back(_newObj);
			break;
		}
		case eObject1Name::FOREST:
		{
			ModelObject* _newObj = new ModelObject(pos, v_SpriteType[(int)eObject1Name::FOREST]);
			_newObj->Initialize(eObject1Name::FOREST);
			m_vec_AllObjects.push_back(_newObj);
			break;
		}
		case eObject1Name::AUDIENCE:
		{
			ModelObject* _newObj = new ModelObject(pos, v_SpriteType[(int)eObject1Name::AUDIENCE]);
			_newObj->Initialize(eObject1Name::AUDIENCE);
			m_vec_AllObjects.push_back(_newObj);
			break;
		}
		default:
		{
			break;
		}
	}
}

void ObjectManager::CreateModelObjectByName(D2D1_VECTOR_2F pos, eObject2Name name)
{
	switch (name)
	{
		case eObject2Name::STONE1:
		{
			ModelObject* _newObj = new ModelObject(pos, v_SpriteType[(int)eObject2Name::STONE1]);
			_newObj->Initialize(eObject2Name::STONE1);
			m_vec_AllObjects.push_back(_newObj);
			break;
		}
		case eObject2Name::STONE2:
		{
			ModelObject* _newObj = new ModelObject(pos, v_SpriteType[(int)eObject2Name::STONE2]);
			_newObj->Initialize(eObject2Name::STONE2);
			m_vec_AllObjects.push_back(_newObj);
			break;
		}
		case eObject2Name::LONGCHAIN:
		{
			ModelObject* _newObj = new ModelObject(pos, v_SpriteType[(int)eObject2Name::LONGCHAIN]);
			_newObj->Initialize(eObject2Name::LONGCHAIN);
			m_vec_AllObjects.push_back(_newObj);
			break;
		}
		case eObject2Name::DIAGONALCHAIN:
		{
			ModelObject* _newObj = new ModelObject(pos, v_SpriteType[(int)eObject2Name::DIAGONALCHAIN]);
			_newObj->Initialize(eObject2Name::DIAGONALCHAIN);
			m_vec_AllObjects.push_back(_newObj);
			break;
		}
		case eObject2Name::SHORTCHAIN:
		{
			ModelObject* _newObj = new ModelObject(pos, v_SpriteType[(int)eObject2Name::SHORTCHAIN]);
			_newObj->Initialize(eObject2Name::SHORTCHAIN);
			m_vec_AllObjects.push_back(_newObj);
			break;
		}
		default:
		{
			break;
		}
	}
}

void ObjectManager::CreateDummyObject(D2D1_VECTOR_2F centerPos, float width, float height, float angle)
{
	CDummy* _newObj = new CDummy(centerPos, width, height, angle);
	_newObj->Initialize();
	m_vec_AllObjects.push_back(_newObj);
}

RaceManager* ObjectManager::RaceManagerInit(int checkpointCount)
{
	RaceManager* m_pRaceManager = new RaceManager();

	m_pRaceManager->Initialize(v_Cars, checkpointCount, m_GM->m_NowTrack);

	return m_pRaceManager;

}


void ObjectManager::UpdateAll(float dTime)
{
	for (int i = 0; i < v_Cars.size(); i++)
	{
		// 가지고 있는 각 물리량을 적용시켜준다.
		v_Cars[i]->VelocityUpdate(dTime);
	}

	for (int i = 0; i < v_Cars.size(); i++)
	{
		// 가지고 있는 각 물리량을 적용시켜준다.
		v_Cars[i]->Update(dTime);
	}

	for (int i = 0; i < m_vec_AllObjects.size(); i++)
	{
		m_vec_AllObjects[i]->Update(dTime);
	}
}

void ObjectManager::CollisionCheckAll()
{
	/// 충돌 체크 영역
	// 1. 자신을 포함한 모든 충돌 체크를 하게하면 안 된다.
	// 2. 이미 충돌한 친구를 다시 체크해서 false로 바꾸면 안 된다.
	// 3. 매 프레임마다 여길 지날 때 초기화를 시켜준다.

	// 차 대 오브젝트
	for (int i = 0; i < v_Cars.size(); i++)
	{
		CGameObject* _car = v_Cars[i];

		for (int j = 0; j < m_vec_AllObjects.size(); j++)
		{
			m_vec_AllObjects[j]->GetCollider()->SetIsCollided(false);
			// 나 자신이라면 패스
			if (i == j) continue;

			// 이미 충돌처리를 했다면 패스
			if (_car->GetCollider()->GetIsCollided() ||
				m_vec_AllObjects[j]->GetCollider()->GetIsCollided())
			{
				continue;
			}

			_car->CollisionCheck2(m_vec_AllObjects[j]);
		}
	}

	// 차 대 차는 모두 한다.
	CGameObject* _car1 = nullptr;
	CGameObject* _car2 = nullptr;

	for (int i = 0; i < v_Cars.size(); i++)
	{
		_car1 = v_Cars[i];

		for (int j = 0; j < v_Cars.size(); j++)
		{
			_car2 = v_Cars[j];
			_car2->GetCollider()->SetIsCollided(false);

			// 나 자신이라면 패스
			if (i == j) continue;

			// 이미 충돌처리를 했다면 패스
			if (_car1->GetCollider()->GetIsCollided() ||
				_car2->GetCollider()->GetIsCollided())
			{
				continue;
			}

			_car1->CollisionCheck2(_car2);
		}
	}
}

CarMK3* ObjectManager::GetCars(int index)
{
	return v_Cars[index];
}

void ObjectManager::RenderAll(D2DEngine* pEngine, float dTime, D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM, bool isIsometric)
{

	for (int i = 0; i < v_Cars.size(); i++)
	{
		// 가지고 있는 각 물리량을 적용시켜준다.
		v_Cars[i]->m_IsDrew = false;
	}

	for (int i = 0; i < m_vec_AllObjects.size(); i++)
	{
		// 배경 오브젝트는 그냥 그린다.
		m_vec_AllObjects[i]->Draw(pEngine, dTime, viewTM, projTM, isIsometric);

		for (int carIndex = 0; carIndex < v_Cars.size(); carIndex++)
		{
			if (!v_Cars[carIndex]->m_IsDrew)
			{
				if (m_vec_AllObjects[i]->GetTransform()->GetLocalPosition().y >
					v_Cars[carIndex]->GetTransform()->GetLocalPosition().y)
				{
					v_Cars[carIndex]->Draw(pEngine, dTime, viewTM, projTM, isIsometric);

					v_Cars[carIndex]->m_IsDrew = true;
				}
			}
		}
	}
}

void ObjectManager::RenderDebug(D2DEngine* pEngine, float dTime, D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM, bool isIsometric)
{
	//for (int i = 0; i < m_vec_AllObjects.size(); i++)
	//{
	//	m_vec_AllObjects[i]->DrawDebugData(m_pEngine, viewTM, projTM);
	//}

	for (int i = 0; i < v_Cars.size(); i++)
	{
		v_Cars[i]->DrawDebugData(m_pEngine, viewTM, projTM);
	}
}

bool IsGreaterThan(CGameObject* a, CGameObject* b)
{
	return a->GetTransform()->GetLocalPosition().y < b->GetTransform()->GetLocalPosition().y;
}

void ObjectManager::SortingByAxisY()
{
	// Standard Template Library
	std::sort(m_vec_AllObjects.begin(), m_vec_AllObjects.end(), IsGreaterThan);


	/*
	/// 부르탈한 소팅
	for (unsigned int i = 0; i < m_vec_AllObjects.size(); i++)
	{
		for (unsigned int j = 0; j < m_vec_AllObjects.size() - 1; j++)
		{
			int _AxixY1 = m_vec_AllObjects[j]->GetTransform()->GetLocalPosition().y;
			int _AxixY2 = m_vec_AllObjects[j + 1]->GetTransform()->GetLocalPosition().y;

			if (_AxixY1 > _AxixY2)
			{
				CGameObject* _temp = m_vec_AllObjects[j];

				m_vec_AllObjects[j] = nullptr;

				m_vec_AllObjects[j] = m_vec_AllObjects[j + 1];

				m_vec_AllObjects[j + 1] = nullptr;

				m_vec_AllObjects[j + 1] = _temp;
			}
		}
	}
	*/
}


void ObjectManager::DrawTransformInfo()
{
	m_pEngine->SetTransform(D2D1::Matrix3x2F::Identity());
	m_pEngine->DrawFillAlphaRect(D2D1::RectF(0, 0, 0, 100), 0.35f);

	for (int i = 0; i < v_Cars.size(); i++)
	{
		v_Cars[i]->DebugInformation(m_pEngine, i);
	}
}

void ObjectManager::DrawVectorInfo(D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM, bool isIsometric)
{
	for (int i = 0; i < v_Cars.size(); i++)
	{
		v_Cars[i]->DebugVectorInformation(m_pEngine, viewTM, projTM);
		v_Cars[i]->DebugDrawVector(m_pEngine, viewTM, projTM, isIsometric);
	}
}

void ObjectManager::DrawCarsVariable()
{
	m_pEngine->SetTransform(D2D1::Matrix3x2F::Identity());

	int _yPoint = 0;
	m_pEngine->DrawText(850, _yPoint, L"======Variable Info=====");
	m_pEngine->DrawText(850, _yPoint += 16, L"Acceleration =     %.1f	↑:R - ↓:F", v_Cars[0]->GetAcceleration());
	m_pEngine->DrawText(850, _yPoint += 16, L"Rotation Angle =  %.1f	↑:T - ↓:G", v_Cars[0]->GetRotationAngle());
	m_pEngine->DrawText(850, _yPoint += 16, L"Angle =	 %.1f	↑:Y - ↓:H", v_Cars[0]->GetDriftAngle());
	m_pEngine->DrawText(850, _yPoint += 16, L"Drift Power =	  %.1f	↑:U - ↓:J", v_Cars[0]->GetDriftPower());
	m_pEngine->DrawText(850, _yPoint += 16, L"Friction Coef =  %.3f	↑: I - ↓:K", v_Cars[0]->GetFrictionCoef());
}

void ObjectManager::InputUpdate()
{
	KeyInput::KeyUpdate();

	float _tempMaxVelocity = MaxVelocity;
	/// 키보드 업데이트 용 (1번 player)
	if (v_Cars[0]->SizeOfVelocity() < _tempMaxVelocity)
	{
		if (KeyInput::InputKey(VK_UP))
		{
			v_Cars[0]->DoAccelerate(Acceleration);

		}
		if (KeyInput::InputKey(VK_DOWN))
		{
			v_Cars[0]->DoAccelerate(-Acceleration / 2);
		}
	}

	/// 회전
	if (KeyInput::InputKey(VK_LEFT))
	{
		v_Cars[0]->RotateObject(-v_Cars[0]->GetRotationAngle());
	}
	if (KeyInput::InputKey(VK_RIGHT))
	{
		v_Cars[0]->RotateObject(v_Cars[0]->GetRotationAngle());
	}

	if (KeyInput::InputKey(VK_SHIFT))  // 드리프트
	{
		if (KeyInput::InputKey(VK_LEFT))
		{
			v_Cars[0]->DoDrift(-v_Cars[0]->GetDriftAngle(), -v_Cars[0]->GetDriftPower());
			SoundManager::GetInstance()->PlayAuto(eSoundName::Drift);
		}

		if (KeyInput::InputKey(VK_RIGHT))
		{
			v_Cars[0]->DoDrift(v_Cars[0]->GetDriftAngle(), v_Cars[0]->GetDriftPower());
			SoundManager::GetInstance()->PlayAuto(eSoundName::Drift);
		}
	}

	/// 위치 초기화
	if (KeyInput::InputKey(VK_L))
	{
		//v_Cars[0]->ResetPosition();
		ObjectManager::MakeCarReady(m_GM->m_NowTrack);
	}

	/// 패드 --------------------------------------------------------------------------------
	XboxPadInput* m_pUserInput = m_GM->GetLehideInput();

	m_pUserInput->Update();


	for (int i = 0; i < 4; i++)
	{

		if (m_pUserInput->GetKeyDown(i, XINPUT_GAMEPAD_LEFT_SHOULDER))
		{
			_tempMaxVelocity = BoosterVelocity;
		}
		else
		{
			_tempMaxVelocity = MaxVelocity;
		}

		if (v_MapCar[i]->SizeOfVelocity() < _tempMaxVelocity)	/// 최대속도
		{
			/// 트리거 가속
			short _triggerL = m_pUserInput->GetLTrigger(i);
			short _triggerR = m_pUserInput->GetRTrigger(i);

			if (10 < _triggerR)
			{
				v_MapCar[i]->DoAccelerate(Acceleration * ((float)_triggerR / 255));
			}
			if (10 < _triggerL)
			{
				v_MapCar[i]->DoAccelerate(-Acceleration / 2 * ((float)_triggerL / 255));
			}
		}

		/// DPad 세기 조정
		short _xAxis = m_pUserInput->GetAxisX(i);
		short _yAxis = m_pUserInput->GetAxisY(i);

		/// 회전
		if (10000 < abs(_xAxis))
		{
			v_MapCar[i]->RotateObject(v_MapCar[i]->GetRotationAngle() * ((float)_xAxis / 65535.0f));
		}
		if (m_pUserInput->GetKeyDown(i, XINPUT_GAMEPAD_DPAD_LEFT))
		{
			v_MapCar[i]->RotateObject(-v_MapCar[i]->GetRotationAngle());
		}
		if (m_pUserInput->GetKeyDown(i, XINPUT_GAMEPAD_DPAD_RIGHT))
		{
			v_MapCar[i]->RotateObject(v_MapCar[i]->GetRotationAngle());
		}

		/// 드리프트
		if (m_pUserInput->GetKeyDown(i, XINPUT_GAMEPAD_X))
		{
			if (_xAxis < 10)
			{
				v_MapCar[i]->DoDrift(
					-v_MapCar[i]->GetDriftAngle(),
					-v_MapCar[i]->GetDriftPower());
			}
			if (_xAxis > -10)
			{
				v_MapCar[i]->DoDrift(
					v_MapCar[i]->GetDriftAngle(),
					v_MapCar[i]->GetDriftPower());
			}

			SoundManager::GetInstance()->PlayAuto(eSoundName::Drift);
		}

		/// 진동
		if (m_pUserInput->GetKeyDown(i, XINPUT_GAMEPAD_X))
		{
			m_pUserInput->MakeBothVibration(i, 0.5f);
		}

		if (m_pUserInput->GetKeyDown(i, XINPUT_GAMEPAD_LEFT_SHOULDER))
		{
			m_pUserInput->MakeLeftVibration(i, 0.1f);
		}

		if (m_pUserInput->GetKeyDown(i, XINPUT_GAMEPAD_RIGHT_SHOULDER))
		{
			m_pUserInput->MakeRightVibration(i, 0.5f);
		}

		/// 위치 초기화
		if (m_pUserInput->GetKeyDown(i, XINPUT_GAMEPAD_Y))
		{
			v_MapCar[i]->ResetPositionByCheckPoint(D2D1::Vector2F(1430, 330), 270.0f);
		}
	}





#pragma region 변수 조절 구간

	/// 변수 조절용 -----------------------------------------------
	if (KeyInput::InputKeyUp(VK_R))	// acceleration
	{
		v_Cars[0]->SetAcceleration(0.1f);
	}
	if (KeyInput::InputKeyUp(VK_F))
	{
		v_Cars[0]->SetAcceleration(-0.1f);
	}
	if (KeyInput::InputKeyUp(VK_T))	// rotation angle
	{
		v_Cars[0]->SetRotationAngle(0.1f);
	}
	if (KeyInput::InputKeyUp(VK_G))
	{
		v_Cars[0]->SetRotationAngle(-0.1f);
	}
	if (KeyInput::InputKeyUp(VK_Y))	// drift angle
	{
		v_Cars[0]->SetDriftAngle(0.1f);
	}
	if (KeyInput::InputKeyUp(VK_H))
	{
		v_Cars[0]->SetDriftAngle(-0.1f);
	}
	if (KeyInput::InputKeyUp(VK_U))	// drift power
	{
		v_Cars[0]->SetDriftPower(0.1f);
	}
	if (KeyInput::InputKeyUp(VK_J))
	{
		v_Cars[0]->SetDriftPower(-0.1f);
	}
	if (KeyInput::InputKeyUp(VK_I))	// friction coefficient
	{
		v_Cars[0]->SetFrictionCoef(0.001f);
	}
	if (KeyInput::InputKeyUp(VK_K))
	{
		v_Cars[0]->SetFrictionCoef(-0.001f);
	}
	if (KeyInput::InputKeyUp(VK_O))	// Mass
	{
		v_Cars[0]->SetMass(0.1f);
	}
	if (KeyInput::InputKeyUp(VK_L))
	{
		v_Cars[0]->SetMass(-0.1f);
	}
	if (KeyInput::InputKeyUp(VK_X))
	{
		v_Cars[0]->SetBrakeOn();
	}
#pragma endregion 변수 조절 구간

}

void ObjectManager::KeyMapping()
{
	m_Player1 = m_GM->m_Pad.at(ePlayerIndex::Player1);
	m_Player2 = m_GM->m_Pad.at(ePlayerIndex::Player2);
	m_Player3 = m_GM->m_Pad.at(ePlayerIndex::Player3);
	m_Player4 = m_GM->m_Pad.at(ePlayerIndex::Player4);

	v_MapCar.push_back(m_Player1);
	v_MapCar.push_back(m_Player2);
	v_MapCar.push_back(m_Player3);
	v_MapCar.push_back(m_Player4);
}

void ObjectManager::MakeCarReady(eGameTrack track)
{
	switch (track)
	{
		case eTrack1:
		{
			v_Cars[0]->GetTransform()->SetLocalPosition(D2D1::Vector2F(1430, 330));
			v_Cars[1]->GetTransform()->SetLocalPosition(D2D1::Vector2F(1460, 260));
			v_Cars[2]->GetTransform()->SetLocalPosition(D2D1::Vector2F(1560, 330));
			v_Cars[3]->GetTransform()->SetLocalPosition(D2D1::Vector2F(1590, 260));

			for (int i = 0; i < v_Cars.size(); i++)
			{
				CarMK3* _car = v_Cars[i];

				_car->GetTransform()->SetLocalRotation(270);
				_car->SetVelocity(D2D1::Vector2F());
				_car->ResetEffect();
			}


			break;
		}
		case eTrack2:
		{
			v_Cars[0]->GetTransform()->SetLocalPosition(D2D1::Vector2F(2000, 1490));
			v_Cars[1]->GetTransform()->SetLocalPosition(D2D1::Vector2F(2000, 1560));
			v_Cars[2]->GetTransform()->SetLocalPosition(D2D1::Vector2F(2000, 1630));
			v_Cars[3]->GetTransform()->SetLocalPosition(D2D1::Vector2F(2000, 1700));

			for (int i = 0; i < v_Cars.size(); i++)
			{
				CarMK3* _car = v_Cars[i];

				_car->GetTransform()->SetLocalRotation(270);
				_car->SetVelocity(D2D1::Vector2F());
				_car->ResetEffect();
			}


			break;
		}
	}


}

void ObjectManager::ResetObject()
{
	for (int i = 0; i < m_vec_AllObjects.size(); i++)
	{
		delete m_vec_AllObjects[i];

		m_vec_AllObjects[i] = nullptr;
	}

	m_vec_AllObjects.clear();

	m_GM->ModelObejctCreate(m_GM->m_NowTrack);
}
