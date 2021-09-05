#pragma once

const float Acceleration = 8.0f;				// ���ӵ�
const float MaxVelocity = 300.0f;				// �ִ�ӵ�
const float BoosterVelocity = 500.0f;			// �ν��� �ӵ�

const float RotationAngle = 2.0f;				// �⺻ �ڵ鸵 ���ӵ� ����
const float DriftAngle = 2.5f;					// �帮��Ʈ �� �߰� ���ӵ� ����
const float DriftPower = 2.0f;					// �帮��Ʈ �� ���� �ӵ����͸� �߰��� ȸ���Ͽ� ������ ���ߴ� ����

const float FrictionCoefficient= 0.03f;			// �⺻ ������ ��� (�������� �������� ����)
const float DriftFrictionCoefficient = 0.003f;	// �帮��Ʈ�� ������ ���
const float MaxStaticFrictionCoef = 0.010f;		// �ִ� ���� ������ ���
const float MaxStaticFrictionVelocity = 70;		// �ִ� ���� �������� �߻��ϴ� �ӵ�
const float ForwardFriction = 0.001f;

const int   ColliderRadius = 22;					// �浹üũ�� �� ������

const float Mass = 3.0f;						// �ڵ��� ���� (������ �� ƨ�ܳ���)
const float TargetMass = 3.0f;					// Ÿ�� �ڵ��� ����

//const float StaticMass = 0.0f;				// �������� �ʴ� ��ü�� ����