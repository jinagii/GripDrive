#include "CarMK3.h"
#include "SoundManager.h"

#include "D2DEngine.h"
#include "CVector.h"
#include "CTransform.h"
#include "CCircle.h"
#include "Trail.h"
#include "Variables.h"
#include "Particle.h"
#include "atlstr.h"
#include "ModelObject.h"
#include "string"
#include "KeyInput.h"


CarMK3::CarMK3(int index, D2DEngine* pEngine, D2D1_VECTOR_2F center, PCWSTR path)
   : m_CarIndex(index), m_RankIndex(0)
{
   //m_bitmap = pEngine->LoadBitmapByPath(L"./data/Car360.png");

   for (int i = 0; i < 36; i++)
   {
      CString str;

      str.Append(path);

      str.AppendFormat(_T("%d.png"), i);

      ID2D1Bitmap* _bitmap = pEngine->LoadBitmapByPath(str);

      v_Sprite.push_back(_bitmap);
   }

   // transform ���� new
   m_Transform = new CTransform();
   m_Transform->SetLocalPosition(center);

   // ���� ���� �ʱ�ȭ
   m_Acceleration = Acceleration;
   m_MaxVelocity = MaxVelocity;
   m_RotationAngle = RotationAngle;
   m_DriftAngle = DriftAngle;
   m_DriftPower = DriftPower;
   m_FrictionCoef = FrictionCoefficient;
   m_DriftFrictionCoef = DriftFrictionCoefficient;
   m_MaxStaticFrictionCoef = MaxStaticFrictionCoef;
   m_Mass = Mass;

   // �浹 �ݰ�
   m_ColliderRadius = ColliderRadius;
   m_Collider = new CCircle(center, m_ColliderRadius);

   // ����Ʈ �ʱ�ȭ
   m_Trail_TailRight = new Trail();
   m_Trail_TailLeft = new Trail();

   m_Smoke = new Particle(L"./data/Smoke_900x300.png", 3);
   m_Spark = new Particle(L"./data/Sparkle_900x300.png", 3);

   // ��ƼŬ ������Ʈ Ǯ�� ��� ����� �� ������ ����
   m_Smoke->IntializeParticle(100);
   m_Spark->IntializeParticle(50);

   // �� UI�� �̸� �ε� �� �д�.
   m_Rank[0] = pEngine->LoadBitmapByPath(L"./data/ui/rank1.png");
   m_Rank[1] = pEngine->LoadBitmapByPath(L"./data/ui/rank2.png");
   m_Rank[2] = pEngine->LoadBitmapByPath(L"./data/ui/rank3.png");
   m_Rank[3] = pEngine->LoadBitmapByPath(L"./data/ui/rank4.png");
}

CarMK3::~CarMK3()
{
   delete m_Smoke;
   delete m_Collider;
   delete m_Transform;
   delete m_Trail_TailRight;
   delete m_Trail_TailLeft;
}

void CarMK3::Update(float dTime)
{
   // ���� ��ġ���� �ణ �������ش�.
   D2D1_VECTOR_2F _nowPos = m_Transform->GetLocalPosition();
   float _nowRot = m_Transform->GetLocalRotation();

   /// �������� ������ �ִ� �ݶ��̴��� ��ġ���� �������ش�.
   m_Collider->SetPosition(_nowPos);

   D2D1_POINT_2F _offset = D2D1::Point2F(20, 10);
   /// Ʈ���� ��� ����
   if (CVector2::SizeOfVector(m_FrictionVector) > 1.2)
   {
      m_Trail_TailRight->CheckPosition(_nowPos, _offset, _nowRot, true);
      _offset.x *= -1;
      m_Trail_TailLeft->CheckPosition(_nowPos, _offset, _nowRot, true);
   }
   else
   {
      m_Trail_TailRight->CheckPosition(_nowPos, _offset, _nowRot, false);
      _offset.x *= -1;
      m_Trail_TailLeft->CheckPosition(_nowPos, _offset, _nowRot, false);
   }

   /// ������ ���� ��ƼŬ�� �׷��ش�
   if (CVector2::SizeOfVector(m_ForwardVector) > 1)
   {
      m_Smoke->PlayParticle(_nowPos, D2D1::Point2F(0, 20), _nowRot);
   }

   /// ������ �ʱ�ȭ
   if (m_BrakeOn == false)
   {
      m_Acceleration = 0;
   }
   else
   {
      if (SizeOfVelocity() < MaxVelocity)
      {
         m_Acceleration = Acceleration;
      }
      else
      {
         m_Acceleration = 0;
      }
   }

   CheckEngineSound();

   CGameObject::Update(dTime);
}

float CarMK3::SizeOfVelocity()
{
   float _vecSize = sqrt(pow(m_Velocity.x, 2) + pow(m_Velocity.y, 2));

   return _vecSize;
}

void CarMK3::MoveHorizontal(float val)
{
   CGameObject::MoveHorizontal(val);
}

void CarMK3::MoveVertical(float val)
{
   CGameObject::MoveVertical(val);
}

void CarMK3::MoveForward(float speed)
{
   // �ڽ��� ���� ���� (�� ������ ��Ÿ���� ����)�� ��� �˾Ƴ���?
   // �ڵ����� ���� ���� �ִٰ� ������ ������ (������ǥ�� ����. �� ������ ���� ����)
   D2D_VECTOR_2F _upVec = D2D1::Vector2F(0, -1.f * speed);

   // �����͸� ȸ���� ���
   D2D1::Matrix3x2F _rotationMatrix = D2D1::Matrix3x2F::Rotation(-1.0f * m_Transform->GetLocalRotation());

   // ��������� ȸ���� ���� ���� (�Լ��� ��ã�Ƽ� �������)
   m_ForwardVector = CVector2::VectorMultiplyMatrix(_upVec, _rotationMatrix);

   // ���溤�ͷ� �̵��Ѵ�.
   m_Transform->MoveByTranslate(m_ForwardVector);
}

void CarMK3::DoAccelerate(float accelInc)
{
   m_Acceleration = accelInc;
}

void CarMK3::VelocityUpdate(float dTime)
{
  
   // �ڵ����� ���� ���� �ִٰ� ������ ����
   D2D1_VECTOR_2F _upVector = D2D1::Vector2F(0.0f, -1.0f);
   // UpVector�� ȸ���� ����� �����
   D2D1::Matrix3x2F _rotationMatrix = D2D1::Matrix3x2F::Rotation(-1.0f * m_Transform->GetLocalRotation());
   // ��������� ȸ���� ���� ���� ���� == ���� ����
   D2D1_VECTOR_2F _ForwardVecNor = CVector2::VectorMultiplyMatrix(_upVector, _rotationMatrix);

   // ���ӷ��� ���� ���� ���̰� �ִ� ���溤�� ((x,y)�� ����� ũ�⸦ ���� offset)
   m_ForwardVector = CVector2::VectorMultiplyScalar(_ForwardVecNor, m_Acceleration);
   // ���� ���� ���� ���� (�ӷ� & ����)
   m_Velocity = CVector2::VectorPlusVector(m_Velocity, m_ForwardVector);

   // �����º��� ���ϱ�
   // ���溤�Ϳ� ��������� �ӵ����͸� �����Ѵ�
   float _dotScalar = CVector2::VectorDotProduct(m_Velocity, _ForwardVecNor);
   // ���� ���� ���Ϳ� �������� ���ϸ� �ӵ����Ͱ� ���溤�Ϳ� �����ȴ�
   D2D1_VECTOR_2F _forwardOrtho = CVector2::VectorMultiplyScalar(_ForwardVecNor, _dotScalar);
   // �׺��͸� �ӵ����Ϳ� ���� ������ ���Ͱ� �ȴ�.
   m_FrictionVector = CVector2::VectorMinus(m_Velocity, _forwardOrtho);

   // �ִ����� ������ �߻� �ӷ�
   if (SizeOfVelocity() < MaxStaticFrictionVelocity)
   {
      m_FrictionVector = CVector2::VectorMultiplyScalar(m_FrictionVector, m_MaxStaticFrictionCoef);
      m_Velocity = CVector2::VectorMultiplyScalar(m_Velocity, 1 - MaxStaticFrictionCoef);
   }
   else
   {
      m_FrictionVector = CVector2::VectorMultiplyScalar(m_FrictionVector, m_FrictionCoef); 
      m_Velocity = CVector2::VectorMultiplyScalar(m_Velocity, 1 - ForwardFriction);
   }

   // �������� �ӵ����Ϳ� �������� �� �ش�.
   m_Velocity = CVector2::VectorMinus(m_Velocity, m_FrictionVector);

   // ȭ�� ������ ������ ���� ó��
   if (abs(m_Transform->GetLocalPosition().x) > 10000 || abs(m_Transform->GetLocalPosition().y) > 10000)
   {
      m_Transform->SetLocalPosition(D2D1::Vector2F(111, 222));
      m_Velocity = D2D1::Vector2F(0, 0);
   }

   // ���� �̵� ���� ����
   D2D1_VECTOR_2F _moveVec = CVector2::VectorMultiplyScalar(m_Velocity, dTime);
   // �ӵ� ���ͷ� �̵��Ѵ�
   m_Transform->MoveByTranslate(_moveVec);
}

void CarMK3::DoDrift(float driftAngle, float driftPower)
{
   /// �ڵ����� �պκ��� ȸ�����Ѽ� �帮��Ʈ (īƮ���̴� ó�� ����� �ʹ�)
   // velocity �����̼� ���� �� �� ũ�� �ش�
   m_Transform->TurnByRotation(driftAngle);

   // val2�� driftPower�� ���� ����
   // ���� �ӵ� ���͵� ���� Ʋ���ָ� īƮ���̴����� �帮��Ʈ�Ҷ� �ް��� ���� ��ȯó�� ad hoc���� ǥ����
   D2D1::Matrix3x2F _DriftPowerMatrix = D2D1::Matrix3x2F::Rotation(-1.0f * driftPower);
   m_Velocity = CVector2::VectorMultiplyMatrix(m_Velocity, _DriftPowerMatrix);
   
   // �������� coefficient ��ŭ ��¦ ���δ�
   m_FrictionVector = CVector2::VectorMultiplyScalar(m_FrictionVector, DriftFrictionCoefficient);
}

void CarMK3::SetVelocity(D2D1_VECTOR_2F velocity)
{
   m_Velocity = velocity;
}

D2D1_VECTOR_2F CarMK3::GetVelocity()
{
   return m_Velocity;
}

void CarMK3::ScaleObject(float val)
{
   CGameObject::ScaleObject(val);
}

void CarMK3::Draw(D2DEngine* pEngine, float dTime, D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM, bool isIsometric)
{
   pEngine->SetTransform(projTM * viewTM);

   m_Trail_TailRight->DrawTrailByBool(pEngine);
   m_Trail_TailLeft->DrawTrailByBool(pEngine);

   /// ��������Ʈ�� �׷��ִ� �Լ�

   // ī�޶�κ��� viewTM�� �����Ų��. ��, ���̼Ҹ�Ʈ���� Ư���� �������� ��Ʈ������ ���� ��ġ�� ���� ����ؾ� �Ѵ�.

   /// ���������� ���� �ϰ�, �� ���� �並 �ϴ� ���̴�.
   /// �׷��� ������, ī�޶� �̵��� ��Ʋ�� ���·� �ȴ�.

   // �������Ǳ��� SetTransform�� �־������ �� �׸��� ������������.
   pEngine->SetTransform(m_Transform->MatrixTranslation(m_Transform->GetIsometricsPosition(projTM)) * viewTM);

   /// �׸��� �׸���
   pEngine->DrawShadowByRotation(v_Sprite, projTM, m_Transform->GetLocalRotation(), 36, isIsometric);

   /// ��ü �׸���
   pEngine->DrawSpriteByRotation(v_Sprite, projTM, m_Transform->GetLocalRotation(), 36, isIsometric);


   /// ��ƼŬ�� �׸���.

   // ��ƼŬ�� ��ü���� �������� ���´�.
   //pEngine->SetTransform(projTM * viewTM);
   pEngine->SetTransform(projTM * viewTM);
   m_Smoke->DrawParticle();	// ����
   m_Spark->DrawParticle();	// �浹 �� ����ũ


   /// �� ���� �׷��� ���� UI
   DrawRankUI(pEngine, dTime, viewTM, projTM, isIsometric);
}

void CarMK3::DrawRankUI(D2DEngine* pEngine, float dTime, D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM, bool isIsometric)
{
   // �������Ǳ��� SetTransform�� �־������ �� �׸��� ������������.
   pEngine->SetTransform(m_Transform->MatrixTranslation(m_Transform->GetIsometricsPosition(projTM)) * viewTM);

   // UI�� �� ���� �׷��ֱ� ���� ������
   pEngine->ImageDrawByPoint(m_Rank[m_RankIndex], D2D1::Point2F(-150.0f, -200.0f));
}


void CarMK3::CollisionCheck(CGameObject* target)
{
   // �ݻ� ���� 
   D2D1_VECTOR_2F _reflectionVec = CVector2::VectorMinus(
      m_Collider->GetPosition(), target->GetCollider()->GetPosition());

   // �Լ� ȣ������� ������ ����������, ���⿡�� ���̽����� ���̽� ó���� �� �ش�.
   CarMK3* _carTarget = dynamic_cast<CarMK3*>(target);

   if (_carTarget != nullptr)
   {
      if (m_Collider->CollisionDetection(_carTarget->GetCollider()) == true)
      {
         SoundManager::GetInstance()->PlayOnce(eSoundName::Car_Collide1);

         m_Velocity = CVector2::VectorPlusVector(this->m_Velocity, (CVector2::VectorMultiplyScalar(_reflectionVec, 1 / m_Mass)));

         _carTarget->m_Velocity = CVector2::VectorPlusVector(_carTarget->m_Velocity, CVector2::VectorMultiplyScalar(_reflectionVec, -1 / TargetMass));

         return;
      }
      if (SoundManager::GetInstance()->PlayOnce(eSoundName::Car_Collide1) == true)
      {
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Car_Collide1, false);
      }
   }

   // ���� �� �����ϴ� �繰�� (Ÿ�̾�, ����, ���̵� ��)
   ModelObject* _modelTarget = dynamic_cast<ModelObject*>(target);

   if (_modelTarget != nullptr)
   {

      if (m_Collider->CollisionDetection(_modelTarget->GetCollider()) == true)
      {
         //SoundManager::GetInstance()->PlayOnce(eSoundName::tire_Collide1);

         this->m_Velocity = CVector2::VectorPlusVector(this->m_Velocity, (CVector2::VectorMultiplyScalar(_reflectionVec, 1 / m_Mass)));

         /// ������ �����Ǿ� �ִ� ���� ���� ������ �� ��� ���� �ø�����, �� ������Ʈ������ ������ �ؾ� �Ѵ�.
         _modelTarget->m_Velocity = CVector2::VectorPlusVector(_modelTarget->m_Velocity, CVector2::VectorMultiplyScalar(_reflectionVec, -1 / TargetMass));

         return;
      }
      if (SoundManager::GetInstance()->IsPlaying(eSoundName::tire_Collide1) == true)
      {
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::tire_Collide1, false);
      }
   }
}


// �� �� ������ MK2 ���� ������� �浹üũ
void CarMK3::CollisionCheck2(CGameObject* target)
{
   D2D1_VECTOR_2F _reflVec = D2D1::Vector2F(0, 0);

   // ���� ��� �������� �̿��ؼ� �浹������ �Ѵ�.
   if (m_Collider->CollisionDetection2(target->GetCollider(), &_reflVec) == true)
   {
      // ���� �ӵ� �̻��� ����϶��� ����� �� �ش�.
      if (1.5f < CVector2::SizeOfVector(_reflVec))
      {
         SoundManager::GetInstance()->PlayOnce(eSoundName::Car_Collide1);
      }
      else if (SoundManager::GetInstance()->IsPlaying(eSoundName::Car_Collide1) == true)
      {
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Car_Collide1, false);
      }


      // �ݻ纤�͸�ŭ �̵���Ų��. ���� ������Ʈ�� �ĸԴ�? ���� ����.
      m_Transform->MoveByTranslate(_reflVec);

      // ���� �ݻ縦 �����Ƿ�, �̵��ӵ� ���͸� �ణ ���� �� �ش�.
      m_Velocity = CVector2::VectorPlusVector(m_Velocity, (CVector2::VectorMultiplyScalar(_reflVec, m_Mass / (m_Mass + TargetMass))));

      // Ÿ�ٵ� �̵��� �ؾ� �Ѵ�. ��ݷ��� ������ �ݺ���ؼ� �������´�.
      target->SetVelocity(CVector2::VectorPlusVector(target->GetVelocity(), CVector2::VectorMultiplyScalar(_reflVec, -TargetMass / (m_Mass + TargetMass))));


      /// �浹 ������ �˷��ش�.
      D2D1_VECTOR_2F _pos = m_Collider->GetPosition();
      CCircle* _circle = dynamic_cast<CCircle*>(m_Collider);
      float _length = _circle->GetRadius() - CVector2::SizeOfVector(_reflVec);

      D2D1_VECTOR_2F _reverseReflVec = CVector2::VectorMultiplyScalar(_reflVec, -1.0f);
      _reverseReflVec = CVector2::VectorNormalize(_reverseReflVec);
      _reverseReflVec = CVector2::VectorMultiplyScalar(_reverseReflVec, _length);

      m_ImpactPosition = CVector2::VectorPlusVector(m_Collider->GetPosition(), _reverseReflVec);

      m_Spark->PlayParticle(m_ImpactPosition, D2D1::Point2F(), 0);

      return;
   }
}

void CarMK3::CollisionCheck_Internal(CCircle* target)
{
   SetNowCenter(m_Transform->GetLocalPosition());

   m_Collider->CollisionDetection(target);
}

void CarMK3::CollisionCheck_Internal(CGameObject* target)
{
   // �浹 üũ�� �ϱ� ����, ���� ������Ʈ�� RTTI�� ���ش�.
   CarMK3* _carTarget = dynamic_cast<CarMK3*>(target);

   // �ݻ� ���� 
   D2D1_VECTOR_2F _reflectionVec = CVector2::VectorMinus(
      m_Collider->GetPosition(), _carTarget->m_Collider->GetPosition());

   if (_carTarget != nullptr)
   {
      //SetNowCenter(D2D1::Point2F(m_Transform->GetLocalPosition().x, m_Transform->GetLocalPosition().y));

      // �浿�ϸ� �ݻ纤�͸� ��������ش�
      //m_Collider->CollisionDetection(_carTarger->GetCollider());		

      if (this->m_Collider->CollisionDetection(_carTarget->GetCollider()) == true)
      {
         this->m_Velocity = CVector2::VectorPlusVector(this->m_Velocity, (CVector2::VectorMultiplyScalar(_reflectionVec, 1 / m_Mass)));

         _carTarget->m_Velocity = CVector2::VectorPlusVector(_carTarget->m_Velocity, CVector2::VectorMultiplyScalar(_reflectionVec, -1 / TargetMass));

         return;
      }
   }

   // �浹 �Ŀ� �ٽ� false�� ���������Ѵ�
   //this->GetCollider()->SetIsCollided(false);
   //_carTarget->GetCollider()->SetIsCollided(false);
}

void CarMK3::DrawTrail(D2DEngine* pEngine)
{
   m_Trail_TailRight->DrawTrail(pEngine);
}

void CarMK3::DebugInformation(D2DEngine* pEngine, int pos)
{
   // UI�� ��� ȭ�鿡 �ٷ� �׸���.
   pEngine->SetTransform(D2D1::Matrix3x2F::Identity());

   D2D1_POINT_2F _point = D2D1::Point2F(10, 10);

   pos = pos * 200;

   pEngine->DrawText(_point.x + pos, _point.y, L"======%d Vehichle Property=====", pos / 300 + 1);
   pEngine->DrawText(_point.x + pos, _point.y += 16, L"LocalRotation = %f", m_Transform->GetLocalRotation());
   pEngine->DrawText(_point.x + pos, _point.y += 16, L"LocalPositionX = %f", m_Transform->GetLocalPosition().x);
   pEngine->DrawText(_point.x + pos, _point.y += 16, L"LocalPositionY = %f", m_Transform->GetLocalPosition().y);
   pEngine->DrawText(_point.x + pos, _point.y += 16, L"Now Acceleration = %f", m_Acceleration);
   //pEngine->DrawText(_point.x, _point.y += 16, L"Now Velocity = %f", GetVelocity());
}

void CarMK3::DebugVectorInformation(D2DEngine* pEngine, D2D1_MATRIX_3X2_F projTM, D2D1_MATRIX_3X2_F viewTM)
{
   // Proj�� ȸ���� Ÿ�� �ʰ� ��� ��ȯ ����� Ÿ���� ���� ��
   pEngine->SetTransform(m_Transform->MatrixTranslation(m_Transform->GetIsometricsPosition(projTM)) * viewTM);

   D2D1_VECTOR_2F _localPos = m_Transform->GetLocalPosition();
   D2D1_VECTOR_2F _isoPos = m_Transform->GetIsometricsPosition(projTM);

   D2D1_POINT_2F _point = D2D1::Point2F(0., 0);

   _point.x += 20;
   ///_point.y -= 100;

   float frictionsize = CVector2::SizeOfVector(m_FrictionVector);

   pEngine->DrawText(_point.x, _point.y, L"======Vehichle VectorInfo=====");
   //pEngine->DrawText(_point.x, _point.y += 16, L"Up Vector X = %.2f", m_UpVector.x);	// ������ǥ�迡���� �����ʹ� �׻� -1�̹Ƿ� ����� �ǹ̾���.
   //pEngine->DrawText(_point.x, _point.y += 16, L"Up Vector Y = %.2f", m_UpVector.y);
   pEngine->DrawText(_point.x, _point.y += 16, L"Velocity X = %.2f", m_Velocity.x);
   pEngine->DrawText(_point.x, _point.y += 16, L"Velocity Y = %.2f", m_Velocity.y);
   pEngine->DrawText(_point.x, _point.y += 16, L"Forward Vector X = %.2f", m_ForwardVector.x);
   pEngine->DrawText(_point.x, _point.y += 16, L"Forward Vector Y = %.2f", m_ForwardVector.y);
   pEngine->DrawText(_point.x, _point.y += 16, L"Angle = %.2f", m_Transform->GetLocalRotation());			// ������
   pEngine->DrawText(_point.x, _point.y += 16, L"WorldPos X = %.2f", m_Transform->GetLocalPosition().x);	// ������� ��ġ
   pEngine->DrawText(_point.x, _point.y += 16, L"WorldPos Y = %.2f", m_Transform->GetLocalPosition().y);	// ������� ��ġ
   pEngine->DrawText(_point.x, _point.y += 16, L"IsoPos Y = %.2f", _isoPos.y);								// ���̼Ҹ�Ʈ������ ��ġ
   pEngine->DrawText(_point.x, _point.y += 16, L"IsoPos Y = %.2f", _isoPos.y);								// ���̼Ҹ�Ʈ������ ��ġ
   pEngine->DrawText(_point.x, _point.y += 16, L"Friction = %.5f", frictionsize);
}

void CarMK3::DebugDrawVector(D2DEngine* pEngine, D2D1_MATRIX_3X2_F projTM, D2D1_MATRIX_3X2_F viewTM, bool isIsometric)
{
   // ��� ��ȯ ����� Ÿ���� ���� ��
   pEngine->SetTransform(m_Transform->MatrixTranslation(m_Transform->GetLocalPosition()) * projTM * viewTM);

   ///------------------------------------------------------
   /// UpVector
   // UpVector�� ����
   D2D1::ColorF _upColor = (D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f));

   // UpVector�� ���� ���� �۱� ������ Ű���� ��������¡�Ѵ�.
   //pEngine->DrawLineByVector(
   //   D2D1::Vector2F(),
   //   CVector2::VectorScalarMultiply(m_UpVector, 30),
   //   _upColor);

   ///------------------------------------------------------
   /// Forward Vector
   D2D1::ColorF _forwardColor = (D2D1::ColorF(1.0f, 1.0f, 0.0f, 1.0f));

   pEngine->DrawLineByVector(
      D2D1::Vector2F(),
      CVector2::VectorMultiplyScalar(m_ForwardVector, 30),
      _forwardColor);

   ///------------------------------------------------------
   /// Velocity Vector
   D2D1::ColorF _velocityColor = (D2D1::ColorF(0.0f, 1.0f, 1.0f, 1.0f));

   pEngine->DrawLineByVector(
      D2D1::Vector2F(),
      m_Velocity,
      _velocityColor);

   ///------------------------------------------------------
   /// Friction Vector
   D2D1::ColorF _friction = (D2D1::ColorF(0.0f, 0.0f, 1.0f, 1.0f));

   pEngine->DrawLineByVector(
      D2D1::Vector2F(),
      CVector2::VectorMultiplyScalar(m_FrictionVector, 30),
      _friction);
}

void CarMK3::ResetPosition()
{
   // ��ġ�� ���ӵ� ���͸� �ʱ�ȭ 
   m_Transform->SetLocalPosition(D2D1::Vector2F(100, 100));
   m_Velocity = D2D1::Vector2F(0, 0);
}

void CarMK3::ResetPositionByCheckPoint(D2D1_VECTOR_2F pos, float angle)
{
   SetVelocity(D2D1::Vector2F());
   m_Transform->SetLocalPosition(pos);
   m_Transform->SetLocalRotation(angle);
}

void CarMK3::ResetEffect()
{
   m_Smoke->ResetParticle();
   m_Trail_TailRight->ResetTrail();
   m_Trail_TailLeft->ResetTrail();
}

void CarMK3::CheckEngineSound()
{
   // ���� �� �� ������ �˰� �ִ�.
   //if (m_CarIndex != 1) return;

   if (m_CarIndex == 1)
   {
      float _nowVelocity = SizeOfVelocity();	// 0~�ִ�ӵ�(200)

      if (0 <= _nowVelocity && _nowVelocity < 40)
      {
         // �� ������ ������ �� �� �ѹ��� Ʋ���ش�.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine0_0) == false)
         {
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine0_0, m_CarIndex);
         }

         // �ٸ� �������� ����� �� �ֵ��� ������ΰ� ���θ� �� �ش�.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_3, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_4, false);
      }
      else if (40 <= _nowVelocity && _nowVelocity < 80)
      {
         // �� ������ ������ �� �� �ѹ��� Ʋ���ش�.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine0_1) == false)
         {
            // ���� �������� ���� ���� Ʋ���ش�.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine0_1, m_CarIndex);
         }

         // �ٸ� �������� ����� �� �ֵ��� ������ΰ� ���θ� �� �ش�.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_3, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_4, false);
      }
      else if (80 <= _nowVelocity && _nowVelocity < 120)
      {
         // �� ������ ������ �� �� �ѹ��� Ʋ���ش�.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine0_2) == false)
         {
            // ���� �������� ���� ���� Ʋ���ش�.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine0_2, m_CarIndex);
         }

         // �ٸ� �������� ����� �� �ֵ��� ������ΰ� ���θ� �� �ش�.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_3, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_4, false);
      }
      else if (120 <= _nowVelocity && _nowVelocity < 160)
      {
         // �� ������ ������ �� �� �ѹ��� Ʋ���ش�.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine0_3) == false)
         {
            // ���� �������� ���� ���� Ʋ���ش�.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine0_3, m_CarIndex);
         }

         // �ٸ� �������� ����� �� �ֵ��� ������ΰ� ���θ� �� �ش�.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_4, false);
      }
      else if (160 <= _nowVelocity && _nowVelocity < 200)
      {
         // �� ������ ������ �� �� �ѹ��� Ʋ���ش�.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine0_4) == false)
         {
            // ���� �������� ���� ���� Ʋ���ش�.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine0_4, m_CarIndex);
         }

         // �ٸ� �������� ����� �� �ֵ��� ������ΰ� ���θ� �� �ش�.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_3, false);
      }
   }

   if (m_CarIndex == 2)
   {
      float _nowVelocity = SizeOfVelocity();	// 0~�ִ�ӵ�(200)

      if (0 <= _nowVelocity && _nowVelocity < 60)
      {
         // �� ������ ������ �� �� �ѹ��� Ʋ���ش�.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine1_0) == false)
         {
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine1_0, m_CarIndex);
         }

         // �ٸ� �������� ����� �� �ֵ��� ������ΰ� ���θ� �� �ش�.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_3, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_4, false);
      }
      else if (60 <= _nowVelocity && _nowVelocity < 120)
      {
         // �� ������ ������ �� �� �ѹ��� Ʋ���ش�.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine1_1) == false)
         {
            // ���� �������� ���� ���� Ʋ���ش�.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine1_1, m_CarIndex);
         }

         // �ٸ� �������� ����� �� �ֵ��� ������ΰ� ���θ� �� �ش�.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_3, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_4, false);
      }
      else if (120 <= _nowVelocity && _nowVelocity < 180)
      {
         // �� ������ ������ �� �� �ѹ��� Ʋ���ش�.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine1_2) == false)
         {
            // ���� �������� ���� ���� Ʋ���ش�.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine1_2, m_CarIndex);
         }

         // �ٸ� �������� ����� �� �ֵ��� ������ΰ� ���θ� �� �ش�.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_3, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_4, false);
      }
      else if (180 <= _nowVelocity && _nowVelocity < 240)
      {
         // �� ������ ������ �� �� �ѹ��� Ʋ���ش�.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine1_3) == false)
         {
            // ���� �������� ���� ���� Ʋ���ش�.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine1_3, m_CarIndex);
         }

         // �ٸ� �������� ����� �� �ֵ��� ������ΰ� ���θ� �� �ش�.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_4, false);
      }
      else if (240 <= _nowVelocity && _nowVelocity < 300)
      {
         // �� ������ ������ �� �� �ѹ��� Ʋ���ش�.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine1_4) == false)
         {
            // ���� �������� ���� ���� Ʋ���ش�.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine1_4, m_CarIndex);
         }

         // �ٸ� �������� ����� �� �ֵ��� ������ΰ� ���θ� �� �ش�.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_3, false);
      }
   }

   if (m_CarIndex == 3)
   {
      float _nowVelocity = SizeOfVelocity();	// 0~�ִ�ӵ�(200)

      if (0 <= _nowVelocity && _nowVelocity < 60)
      {
         // �� ������ ������ �� �� �ѹ��� Ʋ���ش�.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine2_0) == false)
         {
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine2_0, m_CarIndex);
         }

         // �ٸ� �������� ����� �� �ֵ��� ������ΰ� ���θ� �� �ش�.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_3, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_4, false);
      }
      else if (60 <= _nowVelocity && _nowVelocity < 120)
      {
         // �� ������ ������ �� �� �ѹ��� Ʋ���ش�.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine2_1) == false)
         {
            // ���� �������� ���� ���� Ʋ���ش�.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine2_1, m_CarIndex);
         }

         // �ٸ� �������� ����� �� �ֵ��� ������ΰ� ���θ� �� �ش�.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_3, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_4, false);
      }
      else if (120 <= _nowVelocity && _nowVelocity < 180)
      {
         // �� ������ ������ �� �� �ѹ��� Ʋ���ش�.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine2_2) == false)
         {
            // ���� �������� ���� ���� Ʋ���ش�.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine2_2, m_CarIndex);
         }

         // �ٸ� �������� ����� �� �ֵ��� ������ΰ� ���θ� �� �ش�.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_3, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_4, false);
      }
      else if (180 <= _nowVelocity && _nowVelocity < 240)
      {
         // �� ������ ������ �� �� �ѹ��� Ʋ���ش�.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine2_3) == false)
         {
            // ���� �������� ���� ���� Ʋ���ش�.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine2_3, m_CarIndex);
         }

         // �ٸ� �������� ����� �� �ֵ��� ������ΰ� ���θ� �� �ش�.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_4, false);
      }
      else if (240 <= _nowVelocity && _nowVelocity < 300)
      {
         // �� ������ ������ �� �� �ѹ��� Ʋ���ش�.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine2_4) == false)
         {
            // ���� �������� ���� ���� Ʋ���ش�.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine2_4, m_CarIndex);
         }

         // �ٸ� �������� ����� �� �ֵ��� ������ΰ� ���θ� �� �ش�.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_3, false);
      }
   }

   if (m_CarIndex == 4)
   {
      float _nowVelocity = SizeOfVelocity();	// 0~�ִ�ӵ�(200)

      if (0 <= _nowVelocity && _nowVelocity < 60)
      {
         // �� ������ ������ �� �� �ѹ��� Ʋ���ش�.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine3_0) == false)
         {
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine3_0, m_CarIndex);
         }

         // �ٸ� �������� ����� �� �ֵ��� ������ΰ� ���θ� �� �ش�.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_3, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_4, false);
      }
      else if (60 <= _nowVelocity && _nowVelocity < 120)
      {
         // �� ������ ������ �� �� �ѹ��� Ʋ���ش�.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine3_1) == false)
         {
            // ���� �������� ���� ���� Ʋ���ش�.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine3_1, m_CarIndex);
         }

         // �ٸ� �������� ����� �� �ֵ��� ������ΰ� ���θ� �� �ش�.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_3, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_4, false);
      }
      else if (120 <= _nowVelocity && _nowVelocity < 180)
      {
         // �� ������ ������ �� �� �ѹ��� Ʋ���ش�.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine3_2) == false)
         {
            // ���� �������� ���� ���� Ʋ���ش�.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine3_2, m_CarIndex);
         }

         // �ٸ� �������� ����� �� �ֵ��� ������ΰ� ���θ� �� �ش�.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_3, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_4, false);
      }
      else if (180 <= _nowVelocity && _nowVelocity < 240)
      {
         // �� ������ ������ �� �� �ѹ��� Ʋ���ش�.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine3_3) == false)
         {
            // ���� �������� ���� ���� Ʋ���ش�.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine3_3, m_CarIndex);
         }

         // �ٸ� �������� ����� �� �ֵ��� ������ΰ� ���θ� �� �ش�.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_4, false);
      }
      else if (240 <= _nowVelocity && _nowVelocity < 300)
      {
         // �� ������ ������ �� �� �ѹ��� Ʋ���ش�.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine3_4) == false)
         {
            // ���� �������� ���� ���� Ʋ���ش�.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine3_4, m_CarIndex);
         }

         // �ٸ� �������� ����� �� �ֵ��� ������ΰ� ���θ� �� �ش�.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_3, false);
      }
   }

}

