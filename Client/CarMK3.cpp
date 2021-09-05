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

   // transform 관련 new
   m_Transform = new CTransform();
   m_Transform->SetLocalPosition(center);

   // 물리 변수 초기화
   m_Acceleration = Acceleration;
   m_MaxVelocity = MaxVelocity;
   m_RotationAngle = RotationAngle;
   m_DriftAngle = DriftAngle;
   m_DriftPower = DriftPower;
   m_FrictionCoef = FrictionCoefficient;
   m_DriftFrictionCoef = DriftFrictionCoefficient;
   m_MaxStaticFrictionCoef = MaxStaticFrictionCoef;
   m_Mass = Mass;

   // 충돌 반경
   m_ColliderRadius = ColliderRadius;
   m_Collider = new CCircle(center, m_ColliderRadius);

   // 이펙트 초기화
   m_Trail_TailRight = new Trail();
   m_Trail_TailLeft = new Trail();

   m_Smoke = new Particle(L"./data/Smoke_900x300.png", 3);
   m_Spark = new Particle(L"./data/Sparkle_900x300.png", 3);

   // 파티클 오브젝트 풀을 몇개나 만들어 둘 것인지 결정
   m_Smoke->IntializeParticle(100);
   m_Spark->IntializeParticle(50);

   // 차 UI를 미리 로드 해 둔다.
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
   // 현재 위치값을 약간 보정해준다.
   D2D1_VECTOR_2F _nowPos = m_Transform->GetLocalPosition();
   float _nowRot = m_Transform->GetLocalRotation();

   /// 포함으로 가지고 있는 콜라이더의 위치값을 갱신해준다.
   m_Collider->SetPosition(_nowPos);

   D2D1_POINT_2F _offset = D2D1::Point2F(20, 10);
   /// 트레일 노드 생성
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

   /// 앞으로 갈때 파티클을 그려준다
   if (CVector2::SizeOfVector(m_ForwardVector) > 1)
   {
      m_Smoke->PlayParticle(_nowPos, D2D1::Point2F(0, 20), _nowRot);
   }

   /// 물리량 초기화
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
   // 자신의 전방 벡터 (앞 방향을 나타내는 벡터)를 어떻게 알아낼까?
   // 자동차가 위를 보고 있다고 가정한 상태의 (로컬좌표계 기준. 원 상태의 전방 벡터)
   D2D_VECTOR_2F _upVec = D2D1::Vector2F(0, -1.f * speed);

   // 업벡터를 회전할 행렬
   D2D1::Matrix3x2F _rotationMatrix = D2D1::Matrix3x2F::Rotation(-1.0f * m_Transform->GetLocalRotation());

   // 결과적으로 회전된 전방 벡터 (함수를 못찾아서 만들었다)
   m_ForwardVector = CVector2::VectorMultiplyMatrix(_upVec, _rotationMatrix);

   // 전방벡터로 이동한다.
   m_Transform->MoveByTranslate(m_ForwardVector);
}

void CarMK3::DoAccelerate(float accelInc)
{
   m_Acceleration = accelInc;
}

void CarMK3::VelocityUpdate(float dTime)
{
  
   // 자동차가 위를 보고 있다고 가정한 상태
   D2D1_VECTOR_2F _upVector = D2D1::Vector2F(0.0f, -1.0f);
   // UpVector를 회전할 행렬을 만든다
   D2D1::Matrix3x2F _rotationMatrix = D2D1::Matrix3x2F::Rotation(-1.0f * m_Transform->GetLocalRotation());
   // 결과적으로 회전된 전방 단위 벡터 == 방향 벡터
   D2D1_VECTOR_2F _ForwardVecNor = CVector2::VectorMultiplyMatrix(_upVector, _rotationMatrix);

   // 가속력이 붙은 실제 길이가 있는 전방벡터 ((x,y)의 방향과 크기를 가진 offset)
   m_ForwardVector = CVector2::VectorMultiplyScalar(_ForwardVecNor, m_Acceleration);
   // 실제 진행 방향 벡터 (속력 & 방향)
   m_Velocity = CVector2::VectorPlusVector(m_Velocity, m_ForwardVector);

   // 마찰력벡터 구하기
   // 전방벡터와 진행방향인 속도벡터를 내적한다
   float _dotScalar = CVector2::VectorDotProduct(m_Velocity, _ForwardVecNor);
   // 전방 단위 벡터와 내적값을 곱하면 속도벡터가 전방벡터에 투영된다
   D2D1_VECTOR_2F _forwardOrtho = CVector2::VectorMultiplyScalar(_ForwardVecNor, _dotScalar);
   // 그벡터를 속도벡터와 빼면 마찰력 벡터가 된다.
   m_FrictionVector = CVector2::VectorMinus(m_Velocity, _forwardOrtho);

   // 최대정지 마찰력 발생 속력
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

   // 진행중인 속도벡터에 마찰력을 빼 준다.
   m_Velocity = CVector2::VectorMinus(m_Velocity, m_FrictionVector);

   // 화면 밖으로 나갈때 예외 처리
   if (abs(m_Transform->GetLocalPosition().x) > 10000 || abs(m_Transform->GetLocalPosition().y) > 10000)
   {
      m_Transform->SetLocalPosition(D2D1::Vector2F(111, 222));
      m_Velocity = D2D1::Vector2F(0, 0);
   }

   // 실제 이동 벡터 생성
   D2D1_VECTOR_2F _moveVec = CVector2::VectorMultiplyScalar(m_Velocity, dTime);
   // 속도 벡터로 이동한다
   m_Transform->MoveByTranslate(_moveVec);
}

void CarMK3::DoDrift(float driftAngle, float driftPower)
{
   /// 자동차의 앞부분을 회전시켜서 드리프트 (카트라이더 처럼 만들고 싶다)
   // velocity 로테이션 값을 좀 더 크게 준다
   m_Transform->TurnByRotation(driftAngle);

   // val2에 driftPower의 값이 들어간다
   // 진행 속도 벡터도 같이 틀어주면 카트라이더에서 드리프트할때 급격한 방향 전환처럼 ad hoc으로 표현됨
   D2D1::Matrix3x2F _DriftPowerMatrix = D2D1::Matrix3x2F::Rotation(-1.0f * driftPower);
   m_Velocity = CVector2::VectorMultiplyMatrix(m_Velocity, _DriftPowerMatrix);
   
   // 마찰력을 coefficient 만큼 살짝 줄인다
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

   /// 스프라이트를 그려주는 함수

   // 카메라로부터 viewTM은 적용시킨다. 단, 아이소매트릭의 특성상 프로젝션 매트릭스를 통한 위치는 수동 계산해야 한다.

   /// 프로젝션을 먼저 하고, 그 다음 뷰를 하는 것이다.
   /// 그렇지 않으면, 카메라 이동이 비틀린 상태로 된다.

   // 프로젝션까지 SetTransform에 넣어버리면 차 그림이 기울어져버린다.
   pEngine->SetTransform(m_Transform->MatrixTranslation(m_Transform->GetIsometricsPosition(projTM)) * viewTM);

   /// 그림자 그리기
   pEngine->DrawShadowByRotation(v_Sprite, projTM, m_Transform->GetLocalRotation(), 36, isIsometric);

   /// 차체 그리기
   pEngine->DrawSpriteByRotation(v_Sprite, projTM, m_Transform->GetLocalRotation(), 36, isIsometric);


   /// 파티클을 그린다.

   // 파티클은 자체적인 움직임을 갖는다.
   //pEngine->SetTransform(projTM * viewTM);
   pEngine->SetTransform(projTM * viewTM);
   m_Smoke->DrawParticle();	// 연기
   m_Spark->DrawParticle();	// 충돌 시 스파크


   /// 차 위에 그려질 순위 UI
   DrawRankUI(pEngine, dTime, viewTM, projTM, isIsometric);
}

void CarMK3::DrawRankUI(D2DEngine* pEngine, float dTime, D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM, bool isIsometric)
{
   // 프로젝션까지 SetTransform에 넣어버리면 차 그림이 기울어져버린다.
   pEngine->SetTransform(m_Transform->MatrixTranslation(m_Transform->GetIsometricsPosition(projTM)) * viewTM);

   // UI를 차 위에 그려주기 위한 오프셋
   pEngine->ImageDrawByPoint(m_Rank[m_RankIndex], D2D1::Point2F(-150.0f, -200.0f));
}


void CarMK3::CollisionCheck(CGameObject* target)
{
   // 반사 벡터 
   D2D1_VECTOR_2F _reflectionVec = CVector2::VectorMinus(
      m_Collider->GetPosition(), target->GetCollider()->GetPosition());

   // 함수 호출까지는 다형성 동작이지만, 여기에서 케이스바이 케이스 처리를 해 준다.
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

   // 게임 상에 등장하는 사물들 (타이어, 나무, 가이드 등)
   ModelObject* _modelTarget = dynamic_cast<ModelObject*>(target);

   if (_modelTarget != nullptr)
   {

      if (m_Collider->CollisionDetection(_modelTarget->GetCollider()) == true)
      {
         //SoundManager::GetInstance()->PlayOnce(eSoundName::tire_Collide1);

         this->m_Velocity = CVector2::VectorPlusVector(this->m_Velocity, (CVector2::VectorMultiplyScalar(_reflectionVec, 1 / m_Mass)));

         /// 차에만 구현되어 있는 물리 관련 구현을 잘 묶어서 위로 올리던가, 모델 오브젝트에서도 구현을 해야 한다.
         _modelTarget->m_Velocity = CVector2::VectorPlusVector(_modelTarget->m_Velocity, CVector2::VectorMultiplyScalar(_reflectionVec, -1 / TargetMass));

         return;
      }
      if (SoundManager::GetInstance()->IsPlaying(eSoundName::tire_Collide1) == true)
      {
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::tire_Collide1, false);
      }
   }
}


// 좀 더 진보된 MK2 현재 사용중인 충돌체크
void CarMK3::CollisionCheck2(CGameObject* target)
{
   D2D1_VECTOR_2F _reflVec = D2D1::Vector2F(0, 0);

   // 이제 모두 다형성을 이용해서 충돌감지를 한다.
   if (m_Collider->CollisionDetection2(target->GetCollider(), &_reflVec) == true)
   {
      // 일정 속도 이상의 충격일때만 출력을 해 준다.
      if (1.5f < CVector2::SizeOfVector(_reflVec))
      {
         SoundManager::GetInstance()->PlayOnce(eSoundName::Car_Collide1);
      }
      else if (SoundManager::GetInstance()->IsPlaying(eSoundName::Car_Collide1) == true)
      {
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Car_Collide1, false);
      }


      // 반사벡터만큼 이동시킨다. 이제 오브젝트를 파먹는? 것은 없다.
      m_Transform->MoveByTranslate(_reflVec);

      // 또한 반사를 했으므로, 이동속도 벡터를 약간 수정 해 준다.
      m_Velocity = CVector2::VectorPlusVector(m_Velocity, (CVector2::VectorMultiplyScalar(_reflVec, m_Mass / (m_Mass + TargetMass))));

      // 타겟도 이동을 해야 한다. 충격량은 질량에 반비례해서 나눠갖는다.
      target->SetVelocity(CVector2::VectorPlusVector(target->GetVelocity(), CVector2::VectorMultiplyScalar(_reflVec, -TargetMass / (m_Mass + TargetMass))));


      /// 충돌 지점을 알려준다.
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
   // 충돌 체크를 하기 전에, 게임 오브젝트의 RTTI를 해준다.
   CarMK3* _carTarget = dynamic_cast<CarMK3*>(target);

   // 반사 벡터 
   D2D1_VECTOR_2F _reflectionVec = CVector2::VectorMinus(
      m_Collider->GetPosition(), _carTarget->m_Collider->GetPosition());

   if (_carTarget != nullptr)
   {
      //SetNowCenter(D2D1::Point2F(m_Transform->GetLocalPosition().x, m_Transform->GetLocalPosition().y));

      // 충동하면 반사벡터를 적용시켜준다
      //m_Collider->CollisionDetection(_carTarger->GetCollider());		

      if (this->m_Collider->CollisionDetection(_carTarget->GetCollider()) == true)
      {
         this->m_Velocity = CVector2::VectorPlusVector(this->m_Velocity, (CVector2::VectorMultiplyScalar(_reflectionVec, 1 / m_Mass)));

         _carTarget->m_Velocity = CVector2::VectorPlusVector(_carTarget->m_Velocity, CVector2::VectorMultiplyScalar(_reflectionVec, -1 / TargetMass));

         return;
      }
   }

   // 충돌 후에 다시 false로 돌려놔야한다
   //this->GetCollider()->SetIsCollided(false);
   //_carTarget->GetCollider()->SetIsCollided(false);
}

void CarMK3::DrawTrail(D2DEngine* pEngine)
{
   m_Trail_TailRight->DrawTrail(pEngine);
}

void CarMK3::DebugInformation(D2DEngine* pEngine, int pos)
{
   // UI는 모두 화면에 바로 그린다.
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
   // Proj의 회전만 타지 않고 모든 변환 행렬을 타도록 했을 때
   pEngine->SetTransform(m_Transform->MatrixTranslation(m_Transform->GetIsometricsPosition(projTM)) * viewTM);

   D2D1_VECTOR_2F _localPos = m_Transform->GetLocalPosition();
   D2D1_VECTOR_2F _isoPos = m_Transform->GetIsometricsPosition(projTM);

   D2D1_POINT_2F _point = D2D1::Point2F(0., 0);

   _point.x += 20;
   ///_point.y -= 100;

   float frictionsize = CVector2::SizeOfVector(m_FrictionVector);

   pEngine->DrawText(_point.x, _point.y, L"======Vehichle VectorInfo=====");
   //pEngine->DrawText(_point.x, _point.y += 16, L"Up Vector X = %.2f", m_UpVector.x);	// 로컬좌표계에서의 업벡터는 항상 -1이므로 출력이 의미없다.
   //pEngine->DrawText(_point.x, _point.y += 16, L"Up Vector Y = %.2f", m_UpVector.y);
   pEngine->DrawText(_point.x, _point.y += 16, L"Velocity X = %.2f", m_Velocity.x);
   pEngine->DrawText(_point.x, _point.y += 16, L"Velocity Y = %.2f", m_Velocity.y);
   pEngine->DrawText(_point.x, _point.y += 16, L"Forward Vector X = %.2f", m_ForwardVector.x);
   pEngine->DrawText(_point.x, _point.y += 16, L"Forward Vector Y = %.2f", m_ForwardVector.y);
   pEngine->DrawText(_point.x, _point.y += 16, L"Angle = %.2f", m_Transform->GetLocalRotation());			// 각도값
   pEngine->DrawText(_point.x, _point.y += 16, L"WorldPos X = %.2f", m_Transform->GetLocalPosition().x);	// 월드상의 위치
   pEngine->DrawText(_point.x, _point.y += 16, L"WorldPos Y = %.2f", m_Transform->GetLocalPosition().y);	// 월드상의 위치
   pEngine->DrawText(_point.x, _point.y += 16, L"IsoPos Y = %.2f", _isoPos.y);								// 아이소매트릭상의 위치
   pEngine->DrawText(_point.x, _point.y += 16, L"IsoPos Y = %.2f", _isoPos.y);								// 아이소매트릭상의 위치
   pEngine->DrawText(_point.x, _point.y += 16, L"Friction = %.5f", frictionsize);
}

void CarMK3::DebugDrawVector(D2DEngine* pEngine, D2D1_MATRIX_3X2_F projTM, D2D1_MATRIX_3X2_F viewTM, bool isIsometric)
{
   // 모든 변환 행렬을 타도록 했을 때
   pEngine->SetTransform(m_Transform->MatrixTranslation(m_Transform->GetLocalPosition()) * projTM * viewTM);

   ///------------------------------------------------------
   /// UpVector
   // UpVector의 색깔
   D2D1::ColorF _upColor = (D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f));

   // UpVector의 값이 워낙 작기 때문에 키워서 비쥬얼라이징한다.
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
   // 위치와 가속도 벡터를 초기화 
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
   // 내가 몇 번 차인지 알고 있다.
   //if (m_CarIndex != 1) return;

   if (m_CarIndex == 1)
   {
      float _nowVelocity = SizeOfVelocity();	// 0~최대속도(200)

      if (0 <= _nowVelocity && _nowVelocity < 40)
      {
         // 이 구간에 들어왔을 때 딱 한번만 틀어준다.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine0_0) == false)
         {
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine0_0, m_CarIndex);
         }

         // 다른 엔진음을 사용할 수 있도록 재생중인가 여부를 꺼 준다.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_3, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_4, false);
      }
      else if (40 <= _nowVelocity && _nowVelocity < 80)
      {
         // 이 구간에 들어왔을 때 딱 한번만 틀어준다.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine0_1) == false)
         {
            // 이전 엔진음을 끄고 새로 틀어준다.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine0_1, m_CarIndex);
         }

         // 다른 엔진음을 사용할 수 있도록 재생중인가 여부를 꺼 준다.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_3, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_4, false);
      }
      else if (80 <= _nowVelocity && _nowVelocity < 120)
      {
         // 이 구간에 들어왔을 때 딱 한번만 틀어준다.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine0_2) == false)
         {
            // 이전 엔진음을 끄고 새로 틀어준다.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine0_2, m_CarIndex);
         }

         // 다른 엔진음을 사용할 수 있도록 재생중인가 여부를 꺼 준다.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_3, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_4, false);
      }
      else if (120 <= _nowVelocity && _nowVelocity < 160)
      {
         // 이 구간에 들어왔을 때 딱 한번만 틀어준다.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine0_3) == false)
         {
            // 이전 엔진음을 끄고 새로 틀어준다.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine0_3, m_CarIndex);
         }

         // 다른 엔진음을 사용할 수 있도록 재생중인가 여부를 꺼 준다.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_4, false);
      }
      else if (160 <= _nowVelocity && _nowVelocity < 200)
      {
         // 이 구간에 들어왔을 때 딱 한번만 틀어준다.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine0_4) == false)
         {
            // 이전 엔진음을 끄고 새로 틀어준다.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine0_4, m_CarIndex);
         }

         // 다른 엔진음을 사용할 수 있도록 재생중인가 여부를 꺼 준다.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine0_3, false);
      }
   }

   if (m_CarIndex == 2)
   {
      float _nowVelocity = SizeOfVelocity();	// 0~최대속도(200)

      if (0 <= _nowVelocity && _nowVelocity < 60)
      {
         // 이 구간에 들어왔을 때 딱 한번만 틀어준다.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine1_0) == false)
         {
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine1_0, m_CarIndex);
         }

         // 다른 엔진음을 사용할 수 있도록 재생중인가 여부를 꺼 준다.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_3, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_4, false);
      }
      else if (60 <= _nowVelocity && _nowVelocity < 120)
      {
         // 이 구간에 들어왔을 때 딱 한번만 틀어준다.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine1_1) == false)
         {
            // 이전 엔진음을 끄고 새로 틀어준다.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine1_1, m_CarIndex);
         }

         // 다른 엔진음을 사용할 수 있도록 재생중인가 여부를 꺼 준다.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_3, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_4, false);
      }
      else if (120 <= _nowVelocity && _nowVelocity < 180)
      {
         // 이 구간에 들어왔을 때 딱 한번만 틀어준다.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine1_2) == false)
         {
            // 이전 엔진음을 끄고 새로 틀어준다.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine1_2, m_CarIndex);
         }

         // 다른 엔진음을 사용할 수 있도록 재생중인가 여부를 꺼 준다.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_3, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_4, false);
      }
      else if (180 <= _nowVelocity && _nowVelocity < 240)
      {
         // 이 구간에 들어왔을 때 딱 한번만 틀어준다.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine1_3) == false)
         {
            // 이전 엔진음을 끄고 새로 틀어준다.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine1_3, m_CarIndex);
         }

         // 다른 엔진음을 사용할 수 있도록 재생중인가 여부를 꺼 준다.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_4, false);
      }
      else if (240 <= _nowVelocity && _nowVelocity < 300)
      {
         // 이 구간에 들어왔을 때 딱 한번만 틀어준다.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine1_4) == false)
         {
            // 이전 엔진음을 끄고 새로 틀어준다.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine1_4, m_CarIndex);
         }

         // 다른 엔진음을 사용할 수 있도록 재생중인가 여부를 꺼 준다.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine1_3, false);
      }
   }

   if (m_CarIndex == 3)
   {
      float _nowVelocity = SizeOfVelocity();	// 0~최대속도(200)

      if (0 <= _nowVelocity && _nowVelocity < 60)
      {
         // 이 구간에 들어왔을 때 딱 한번만 틀어준다.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine2_0) == false)
         {
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine2_0, m_CarIndex);
         }

         // 다른 엔진음을 사용할 수 있도록 재생중인가 여부를 꺼 준다.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_3, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_4, false);
      }
      else if (60 <= _nowVelocity && _nowVelocity < 120)
      {
         // 이 구간에 들어왔을 때 딱 한번만 틀어준다.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine2_1) == false)
         {
            // 이전 엔진음을 끄고 새로 틀어준다.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine2_1, m_CarIndex);
         }

         // 다른 엔진음을 사용할 수 있도록 재생중인가 여부를 꺼 준다.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_3, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_4, false);
      }
      else if (120 <= _nowVelocity && _nowVelocity < 180)
      {
         // 이 구간에 들어왔을 때 딱 한번만 틀어준다.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine2_2) == false)
         {
            // 이전 엔진음을 끄고 새로 틀어준다.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine2_2, m_CarIndex);
         }

         // 다른 엔진음을 사용할 수 있도록 재생중인가 여부를 꺼 준다.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_3, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_4, false);
      }
      else if (180 <= _nowVelocity && _nowVelocity < 240)
      {
         // 이 구간에 들어왔을 때 딱 한번만 틀어준다.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine2_3) == false)
         {
            // 이전 엔진음을 끄고 새로 틀어준다.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine2_3, m_CarIndex);
         }

         // 다른 엔진음을 사용할 수 있도록 재생중인가 여부를 꺼 준다.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_4, false);
      }
      else if (240 <= _nowVelocity && _nowVelocity < 300)
      {
         // 이 구간에 들어왔을 때 딱 한번만 틀어준다.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine2_4) == false)
         {
            // 이전 엔진음을 끄고 새로 틀어준다.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine2_4, m_CarIndex);
         }

         // 다른 엔진음을 사용할 수 있도록 재생중인가 여부를 꺼 준다.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine2_3, false);
      }
   }

   if (m_CarIndex == 4)
   {
      float _nowVelocity = SizeOfVelocity();	// 0~최대속도(200)

      if (0 <= _nowVelocity && _nowVelocity < 60)
      {
         // 이 구간에 들어왔을 때 딱 한번만 틀어준다.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine3_0) == false)
         {
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine3_0, m_CarIndex);
         }

         // 다른 엔진음을 사용할 수 있도록 재생중인가 여부를 꺼 준다.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_3, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_4, false);
      }
      else if (60 <= _nowVelocity && _nowVelocity < 120)
      {
         // 이 구간에 들어왔을 때 딱 한번만 틀어준다.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine3_1) == false)
         {
            // 이전 엔진음을 끄고 새로 틀어준다.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine3_1, m_CarIndex);
         }

         // 다른 엔진음을 사용할 수 있도록 재생중인가 여부를 꺼 준다.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_3, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_4, false);
      }
      else if (120 <= _nowVelocity && _nowVelocity < 180)
      {
         // 이 구간에 들어왔을 때 딱 한번만 틀어준다.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine3_2) == false)
         {
            // 이전 엔진음을 끄고 새로 틀어준다.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine3_2, m_CarIndex);
         }

         // 다른 엔진음을 사용할 수 있도록 재생중인가 여부를 꺼 준다.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_3, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_4, false);
      }
      else if (180 <= _nowVelocity && _nowVelocity < 240)
      {
         // 이 구간에 들어왔을 때 딱 한번만 틀어준다.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine3_3) == false)
         {
            // 이전 엔진음을 끄고 새로 틀어준다.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine3_3, m_CarIndex);
         }

         // 다른 엔진음을 사용할 수 있도록 재생중인가 여부를 꺼 준다.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_4, false);
      }
      else if (240 <= _nowVelocity && _nowVelocity < 300)
      {
         // 이 구간에 들어왔을 때 딱 한번만 틀어준다.
         if (SoundManager::GetInstance()->IsPlaying(eSoundName::Engine3_4) == false)
         {
            // 이전 엔진음을 끄고 새로 틀어준다.
            SoundManager::GetInstance()->StopEngineSound(m_CarIndex);
            SoundManager::GetInstance()->PlayEngineSound(eSoundName::Engine3_4, m_CarIndex);
         }

         // 다른 엔진음을 사용할 수 있도록 재생중인가 여부를 꺼 준다.
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_0, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_1, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_2, false);
         SoundManager::GetInstance()->SetSoundPlaying(eSoundName::Engine3_3, false);
      }
   }

}

