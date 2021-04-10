void CarManager::InputUpdate()
{
  // 자동차를 키보드로 업데이트
  // 테스트, 디버그로도 활용
	KeyboardInput::KeyUpdate();

	float _tempMaxVelocity = MaxVelocity;
  // 1번 자동차 키보드 테스트용으로 활용
	if (v_Cars[0]->SizeOfVelocity() < _tempMaxVelocity)
	{
		if (KeyboardInput::InputKey(VK_UP))    // 전진 
		{
			v_Cars[0]->DoAccelerate(Acceleration);
		}
		if (KeyboardInput::InputKey(VK_DOWN))  // 후진
		{
			v_Cars[0]->DoAccelerate(-Acceleration / 2);
		}
	}

	// 자동차 조향 컨트롤
	if (KeyboardInput::InputKey(VK_LEFT))   // 좌회전
	{
		v_Cars[0]->RotateObject(-v_Cars[0]->GetRotationAngle());
	}
	if (KeyboardInput::InputKey(VK_RIGHT))  // 우회전
	{
		v_Cars[0]->RotateObject(v_Cars[0]->GetRotationAngle());
	}
  
  // 드리프트
	if (KeyboardInput::InputKey(VK_SHIFT))
	{
		if (KeyboardInput::InputKey(VK_LEFT))
		{
			v_Cars[0]->DoDrift(-v_Cars[0]->GetDriftAngle(), -v_Cars[0]->GetDriftPower());
			SoundManager::GetInstance()->PlayAuto(eSoundName::Drift);
		}
		if (KeyboardInput::InputKey(VK_RIGHT))
		{
			v_Cars[0]->DoDrift(v_Cars[0]->GetDriftAngle(), v_Cars[0]->GetDriftPower());
			SoundManager::GetInstance()->PlayAuto(eSoundName::Drift);
		}
	}

	// 자동차 위치 초기화
	if (KeyboardInput::InputKey(VK_R))
	{
		v_Cars[0]->ResetPosition();
	}

	/// XBOX Pad --------------------------------------------------------------------------------
	// 자동차 멤버별로 XBOX 패드를 사용 
  PadInput* m_pUserInput = m_GM->GetPadInput();
	m_pUserInput->Update();

	for (int i = 0; i < 4; i++)   // 4대의 자동차
	{
		if (m_pUserInput->GetKeyDown(i, XINPUT_GAMEPAD_LEFT_SHOULDER))
		{
			_tempMaxVelocity = BoosterVelocity;   // 
		}
		else
		{
			_tempMaxVelocity = MaxVelocity;
		}

		if (v_Car[i]->SizeOfVelocity() < _tempMaxVelocity)	/// 최대속도
		{
			// 트리거 키로  가속
			short _triggerL = m_pUserInput->GetLTrigger(i);
			short _triggerR = m_pUserInput->GetRTrigger(i);

			if (10 < _triggerR)
			{
				v_Car[i]->DoAccelerate(Acceleration * ((float)_triggerR / 255));
			}
			if (10 < _triggerL)
			{
				v_Car[i]->DoAccelerate(-Acceleration / 2 * ((float)_triggerL / 255));
			}
		}

		// Pad joystick 부분 세기 조정
		short _xAxis = m_pUserInput->GetAxisX(i);
		short _yAxis = m_pUserInput->GetAxisY(i);

		// 회전
		if (10000 < abs(_xAxis))
		{
			v_Car[i]->RotateObject(v_Car[i]->GetRotationAngle() * ((float)_xAxis / 65535.0f));
		}
		if (m_pUserInput->GetKeyDown(i, XINPUT_GAMEPAD_DPAD_LEFT))
		{
			v_Car[i]->RotateObject(-v_Car[i]->GetRotationAngle());
		}
		if (m_pUserInput->GetKeyDown(i, XINPUT_GAMEPAD_DPAD_RIGHT))
		{
			v_Car[i]->RotateObject(v_Car[i]->GetRotationAngle());
		}

		// 드리프트
		if (m_pUserInput->GetKeyDown(i, XINPUT_GAMEPAD_X))
		{
			if (_xAxis < 10)
			{
				v_Car[i]->DoDrift(
					-v_Car[i]->GetDriftAngle(),
					-v_Car[i]->GetDriftPower());
			}
			if (_xAxis > -10)
			{
				v_Car[i]->DoDrift(
					v_Car[i]->GetDriftAngle(),
					v_Car[i]->GetDriftPower());
			}

			SoundManager::GetInstance()->PlayAuto(eSoundName::Drift);
		}

		// 진동 테스트
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

		// 위치 초기화
		if (m_pUserInput->GetKeyDown(i, XINPUT_GAMEPAD_Y))
		{
			v_Car[i]->ResetPositionByCheckPoint(D2D1::Vector2F(1430, 330), 270.0f);
		}
	}

#pragma region 변수 조절 구간

	// 변수 조절용(가속, 회전속도, 드리프트 등)---------------------------------------
	if (KeyboardInput::InputKeyUp(VK_R))	// acceleration
	{
		v_Cars[0]->SetAcceleration(0.1f);
	}
	if (KeyboardInput::InputKeyUp(VK_F))
	{
		v_Cars[0]->SetAcceleration(-0.1f);
	}
	if (KeyboardInput::InputKeyUp(VK_T))	// rotation angle
	{
		v_Cars[0]->SetRotationAngle(0.1f);
	}
	if (KeyboardInput::InputKeyUp(VK_G))
	{
		v_Cars[0]->SetRotationAngle(-0.1f);
	}
	if (KeyboardInput::InputKeyUp(VK_Y))	// drift angle
	{
		v_Cars[0]->SetDriftAngle(0.1f);
	}
	if (KeyboardInput::InputKeyUp(VK_H))
	{
		v_Cars[0]->SetDriftAngle(-0.1f);
	}
	if (KeyboardInput::InputKeyUp(VK_U))	// drift power
	{
		v_Cars[0]->SetDriftPower(0.1f);
	}
	if (KeyboardInput::InputKeyUp(VK_J))
	{
		v_Cars[0]->SetDriftPower(-0.1f);
	}
	if (KeyboardInput::InputKeyUp(VK_I))	// friction coefficient
	{
		v_Cars[0]->SetFrictionCoef(0.001f);
	}
	if (KeyboardInput::InputKeyUp(VK_K))
	{
		v_Cars[0]->SetFrictionCoef(-0.001f);
	}
	if (KeyboardInput::InputKeyUp(VK_O))	// Mass
	{
		v_Cars[0]->SetMass(0.1f);
	}
	if (KeyboardInput::InputKeyUp(VK_L))
	{
		v_Cars[0]->SetMass(-0.1f);
	}
	if (KeyboardInput::InputKeyUp(VK_X))
	{
		v_Cars[0]->SetBrakeOn();
	}
  
#pragma endregion 변수 조절 구간
}
