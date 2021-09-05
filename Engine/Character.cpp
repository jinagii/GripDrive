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
	// ���� ������Ʈ�� ������ �ִ� ����� ������ �´�.
	// m_State�� ĳ������ ���� ���¸� ��Ÿ����, �̸� ���� � ����� ���������� ���Ѵ�.
	/// ���� ���¿� ����� �̸��� ��ġ��Ų �� ������ ��� �� ���� �� ������?
	CAMotion* _motion = m_Motions[(int)m_State];

	// ���� ���ν����� ��ġ�� dTime�� ��� ������Ų��.
	// m_CountTime�� ���� ���� ��� ������Ʈ�� ����� �ͼӽ��״�.
	// �� ���, �� �� �̻��� GameObject�� �����Ǹ�, ����ȭ���� ���� ���¿��� ���������� ������ ������ �����Ѵ�.
	m_CountdTime += dTime;

	// m_Animation�� CAMotion Ŭ�������� ������ CASprite�� ��� ���͸� �ǹ��Ѵ�.
	// _maxIndex���� �� �ִϸ��̼��� ������ ǥ�õ�����, - 1�� ���� ������, �迭�� ������ 0 �̱� �����̴�.
	/// ������ �� ����� �� ������ �����������Ƿ�, �Լ��� ������ ���� �ƴ϶� �̸� MaxIndex�� �־��ֵ��� ����.
	int _maxIndex = _motion->m_Animation.size() - 1;

	// _nowIndex�� ����� ���� �ε����� ��Ÿ����.
	// �� ��쿡�� ����� �ٲ� ��츶�� �ʱ�ȭ���� �ʿ䰡 �ִ�.
	// �ʱ�ȭ���� ���� ���, ������ ���� ��ǿ��� �̾ �� ���̱� �����̴�.
	int _nowIndex = _motion->Index;

	// ������ ��������Ʈ�� ������ �ִ� ������ Ÿ������, ���������� �����Ǵ� m_CountTime�� �񱳵Ǵ� ���̴�.
	// ������ Ÿ���� ���� m_CountTime�� Ŀ����, ���� ��������Ʈ�� ����ϴ� ����̴�.
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

	// ���� ������ �����ٰ�, �׷��� ��Ʈ���� ����� 2�� ���� �߾Ӱ��� ������ �����ش�.
	// �̸� ���� ������Ʈ�� �߾Ӱ��� ���� �� �ִ�.


	//m_nowCenter.x = m_Position.x + (_sprite->m_Size.width / 2);
	//m_nowCenter.y = m_Position.y + (_sprite->m_Size.height / 2);

	// ��ħ�� �׷��ش�.
	//pEngine->DrawSprite(_sprite, _motion->Name, m_Position.x, m_Position.y);

	// ����� ���¶��
	DrawDebugData(pEngine, _sprite);
}

void Character::DrawDebugData(D2DEngine* pEngine, CASprite* _sprite)
{

	D2D1_RECT_F _rect = _sprite->m_HitBox->GetRectPosition();
	bool _isDection = _sprite->m_HitBox->IsCollide();

	//pEngine->DrawRectangle_Debug(m_Position.x + _rect.left, m_Position.y + //_rect.top, _rect.right, _rect.bottom, _isDection);
}


