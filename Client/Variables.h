#pragma once

const float Acceleration = 8.0f;				// 가속도
const float MaxVelocity = 300.0f;				// 최대속도
const float BoosterVelocity = 500.0f;			// 부스터 속도

const float RotationAngle = 2.0f;				// 기본 핸들링 각속도 조정
const float DriftAngle = 2.5f;					// 드리프트 시 추가 각속도 조정
const float DriftPower = 2.0f;					// 드리프트 시 현재 속도벡터를 추가로 회전하여 감속을 늦추는 변수

const float FrictionCoefficient= 0.03f;			// 기본 마찰력 계수 (높을수록 마찰력이 높다)
const float DriftFrictionCoefficient = 0.003f;	// 드리프트시 마찰력 계수
const float MaxStaticFrictionCoef = 0.010f;		// 최대 정지 마찰력 계수
const float MaxStaticFrictionVelocity = 70;		// 최대 정지 마찰력이 발생하는 속도
const float ForwardFriction = 0.001f;

const int   ColliderRadius = 22;					// 충돌체크용 원 반지름

const float Mass = 3.0f;						// 자동차 무게 (높으면 덜 튕겨나감)
const float TargetMass = 3.0f;					// 타겟 자동차 무게

//const float StaticMass = 0.0f;				// 움직이지 않는 물체의 무게