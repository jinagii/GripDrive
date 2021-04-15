// 
void CarMK3::VelocityUpdate(float dTime)
{
	/// 물리 - 전방벡터를 이용해서 이동을 한다 (velocity로 진행방향)	
	// 자신의 전방 벡터 (앞 방향을 나타내는 벡터)를 어떻게 알아낼까?
	// 자동차가 위를 보고 있다고 가정한 상태의 (로컬좌표계 기준. 원 상태의 전방 벡터)
	m_UpVector = D2D1::Vector2F(0, -1.0f);

	// 업벡터를 회전할 행렬
	D2D1::Matrix3x2F _rotationMatrix = D2D1::Matrix3x2F::Rotation(-1.f * m_Transform->GetLocalRotation());

	// 결과적으로 회전된 단위 전방 벡터 == 방향벡터
	D2D1_VECTOR_2F _ForwardVecNor = CVector2::VectorMultiplyMatrix_Math(m_UpVector, _rotationMatrix);

	// 가속력이 붙은 실제 길이가 있는 전방벡터
	m_ForwardVector = CVector2::VectorScalarMultiply(_ForwardVecNor, m_Acceleration);

	// 실제 진행 방향 벡터 (속력 & 방향)
	m_Velocity = CVector2::VectorPlus(m_Velocity, m_ForwardVector);

	// 마찰력벡터
	///_ForwardVecNor; // 방향벡터
	float _dotScalar = CVector2::VectorDotProduct(m_Velocity, _ForwardVecNor);
	D2D1_VECTOR_2F _forwardOrtho = CVector2::VectorScalarMultiply(_ForwardVecNor, _dotScalar);

	m_FrictionVector = CVector2::VectorMinus(m_Velocity, _forwardOrtho);

	/// 최대정지 마찰력 발생 속력
	if (SizeOfVelocity() < MaxStaticFrictionVelocity)
	{
		m_FrictionVector = CVector2::VectorScalarMultiply(m_FrictionVector, m_MaxStaticFrictionCoef);
		m_Velocity = CVector2::VectorScalarMultiply(m_Velocity, 1 - MaxStaticFrictionCoef);
	}
	else
	{
		m_FrictionVector = CVector2::VectorScalarMultiply(m_FrictionVector, m_FrictionCoef); /// 일정 마찰계수
		m_Velocity = CVector2::VectorScalarMultiply(m_Velocity, 1 - ForwadFriction);
	}

	// 마찰력을 뺀다
	m_Velocity = CVector2::VectorMinus(m_Velocity, m_FrictionVector);

	// 화면 밖으로 나갈때 예외 처리
	if (abs(m_Transform->GetLocalPosition().x) > 10000 ||
		abs(m_Transform->GetLocalPosition().y) > 10000)
	{
		m_Transform->SetLocalPosition(D2D1::Vector2F(111, 222));
		m_Velocity = D2D1::Vector2F(0, 0);
	}

	//_moveVec = CVector2::VectorPlus(CVector2::VectorScalarMultiply(m_FrictionVector, 0.1f), _moveVec);

	// 이동 벡터 생성
	D2D1_VECTOR_2F _moveVec = CVector2::VectorScalarMultiply(m_Velocity, dTime);

	// 속도 벡터로 이동한다
	m_Transform->MoveByTranslate(_moveVec);
}

void CarMK3::DoDrift(float val1, float val2)
{
	/// 자동차의 앞부분을 회전시켜서 드리프트
	// velocity 로테이션 값도 조정해서 좀더 카트라이더 같다
	m_Transform->TurnByRotation(val1);

	D2D1::Matrix3x2F _DriftPowerMatrix = D2D1::Matrix3x2F::Rotation(-1.0f * val2);
	m_Velocity = CVector2::VectorMultiplyMatrix_Math(m_Velocity, _DriftPowerMatrix);

	/*D2D1::Matrix3x2F _driftMatrix = D2D1::Matrix3x2F::Rotation(-1.0f * m_DriftPower);
	m_Velocity = CVector2::VectorMultiplyMatrix(m_Velocity, _driftMatrix);*/

	// 마찰력을 줄인다
	m_FrictionVector = CVector2::VectorScalarMultiply(m_FrictionVector, DriftFrictionCoefficient);
	//m_DriftHelperVector = CVector2::VectorScalarMultiply(m_ForwardVector, 2.0f);
}

/// 디버그를 위해서 벡터의 방향을 시각화 
void CarMK3::DrawDebugVector(GameEngine* pEngine, D2D1_MATRIX_3X2_F projTM, D2D1_MATRIX_3X2_F viewTM, bool isIsometric)
{
	// 모든 변환 행렬을 타도록 했을 때
	pEngine->SetTransform(m_Transform->MatrixTranslation(m_Transform->GetLocalPosition()) * projTM * viewTM);

	/// UpVector
	D2D1::ColorF _upColor = (D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f));
	// UpVector의 값이 워낙 작아 스케일해서 비쥬얼라이징한다.
	pEngine->DrawLineByVector( D2D1::Vector2F(), CVector2::VectorScalarMultiply(m_UpVector, 30), _upColor);

	/// Forward Vector
	D2D1::ColorF _forwardColor = (D2D1::ColorF(1.0f, 1.0f, 0.0f, 1.0f));
	pEngine->DrawLineByVector(D2D1::Vector2F(), CVector2::VectorScalarMultiply(m_ForwardVector, 30), _forwardColor);
	
	/// Velocity Vector
	D2D1::ColorF _velocityColor = (D2D1::ColorF(0.0f, 1.0f, 1.0f, 1.0f));
	pEngine->DrawLineByVector(D2D1::Vector2F(),	m_Velocity,	_velocityColor);

	/// Friction Vector
	D2D1::ColorF _friction = (D2D1::ColorF(0.0f, 0.0f, 1.0f, 1.0f));
	pEngine->DrawLineByVector(D2D1::Vector2F(), CVector2::VectorScalarMultiply(m_FrictionVector, 30), _friction);
}

