#include "Character.h"
#include "D2DEngine.h"
#include "CAABB.h"


Character::Character()
	: m_State(eState::Idle),
	m_CountdTime(0.0f)
{
	
}

Character::~Character()
{

}

void Character::Update()
{

}

void Character::MoveHorizontal(float val)
{

}

void Character::MoveVertical(float val)
{

}

void Character::RotateObject(float val)
{

}

void Character::ScaleObject(float val)
{

}

void Character::PlayMotion(D2DEngine* pEngine, float dTime, eState m_State)
{
	// 현재 오브젝트가 가지고 있는 모션을 가지고 온다.
	// m_State는 캐릭터의 현재 상태를 나타내며, 이를 통해 어떤 모션을 가져올지를 정한다.
	/// 현재 상태와 모션의 이름을 일치시킨 후 맵으로 모든 걸 끝낼 수 있을까?
	CAMotion* _motion = m_Motions[(int)m_State];

	// 게임 프로시져에 위치한 dTime을 계속 누적시킨다.
	// m_CountTime은 전역 변수 대신 오브젝트의 멤버로 귀속시켰다.
	// 이 경우, 두 개 이상의 GameObject가 생성되면, 동기화되지 않은 상태에서 개별적으로 움직일 것으로 예상한다.
	m_CountdTime += dTime;

	// m_Animation은 CAMotion 클래스에서 각각의 CASprite가 담긴 벡터를 의미한다.
	// _maxIndex에는 총 애니메이션의 개수가 표시되지만, - 1을 해준 이유는, 배열의 시작이 0 이기 때문이다.
	/// 어차피 각 모션의 총 개수는 정해져있으므로, 함수로 가져올 것이 아니라 미리 MaxIndex를 넣어주도록 하자.
	int _maxIndex = _motion->m_Animation.size() - 1;

	// _nowIndex는 모션의 현재 인덱스를 나타낸다.
	// 이 경우에는 모션이 바뀔 경우마다 초기화해줄 필요가 있다.
	// 초기화하지 않을 경우, 이전에 끊긴 모션에서 이어서 할 것이기 때문이다.
	int _nowIndex = _motion->Index;

	// 각각의 스프라이트가 가지고 있는 딜레이 타임으로, 지속적으로 누적되는 m_CountTime과 비교되는 값이다.
	// 딜레이 타임이 보다 m_CountTime이 커지면, 다음 스프라이트를 출력하는 방식이다.
	float _nowDelayTime = _motion->m_Animation[_nowIndex]->m_DelayFrame;

	if (m_CountdTime > _nowDelayTime)
	{
		_motion->Index++;
		m_CountdTime = 0.0f;

		if (_motion->Index > _maxIndex)
		{
			_motion->Index = 0;
		}
	}

	CASprite* _sprite = _motion->m_Animation[_nowIndex];

	// 현재 포지션 값에다가, 그려줄 비트맵의 사이즈를 2로 나눠 중앙값을 각각에 더해준다.
	// 이를 통해 오브젝트의 중앙값을 구할 수 있다.


	//m_nowCenter.x = m_Position.x + (_sprite->m_Size.width / 2);
	//m_nowCenter.y = m_Position.y + (_sprite->m_Size.height / 2);

	// 마침내 그려준다.
	//pEngine->DrawSprite(_sprite, _motion->Name, m_Position.x, m_Position.y);

	// 디버그 상태라면
	DrawDebugData(pEngine, _sprite);
}

void Character::DrawDebugData(D2DEngine* pEngine, CASprite* _sprite)
{

	D2D1_RECT_F _rect = _sprite->m_HitBox->GetRectPosition();
	bool _isDection = _sprite->m_HitBox->IsCollide();

	//pEngine->DrawRectangle_Debug(m_Position.x + _rect.left, m_Position.y + //_rect.top, _rect.right, _rect.bottom, _isDection);
}


