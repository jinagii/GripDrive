#include "ModelObject.h"

#include "ICollisionObject.h"
#include "CTransform.h"
#include "CCircle.h"
#include "CAABB.h"
#include "ObjectManager.h"
#include "D2DEngine.h"
#include "CVector.h"


ModelObject::ModelObject()
{

}

ModelObject::ModelObject(D2D1_VECTOR_2F pos, ID2D1Bitmap* bitmap)
{
	// ������Ʈ�� ���� ��ġ�� �����ش�.
	m_Transform = new CTransform();

	// �ǳ׹��� ��Ʈ���� �־��ش�.
	m_bitmap = bitmap;

	int _width = m_bitmap->GetPixelSize().width;
	int _height = m_bitmap->GetPixelSize().height;

	D2D1_SIZE_F _size = m_bitmap->GetSize();

	pos.x += _size.width;
	pos.y += _size.height / 4;


	m_Transform->SetLocalPosition(pos);


	D2D1_VECTOR_2F _center = m_Transform->GetLocalPosition();
	


	// ���⼭�� _center�� ��Ʈ���� rect�� ������� 0,0 �� �׷��� ��Ʈ���� localPosition�� ���Ѵ�.
	m_bitmapCenter.x = pos.x - (_width / 2);
	m_bitmapCenter.y = pos.y - (_height / 2);

}

ModelObject::~ModelObject()
{

}

void ModelObject::Initialize(eObject1Name name)
{
	// �����ڷκ��� �ʱ�ȭ�� ������ �������� �߰��� �� �����͸� �����Ѵ�.
	//D2D1_VECTOR_2F _center = m_Collider->GetPosition();
	D2D1_VECTOR_2F _center = m_Transform->GetLocalPosition();
	int _width = m_bitmap->GetPixelSize().width;
	int _height = m_bitmap->GetPixelSize().height;

	_center.y = _center.y + (_height / 4);

	switch (name)
	{
		case eObject1Name::TREE:
		{
			m_Collider = new CCircle(_center.x, _center.y, _width / 4);
			m_Attr = eObjectAttribute::STATIC;
			break;
		}
		case eObject1Name::CONE:
		{
			m_Collider = new CCircle(_center.x, _center.y, _width / 2);
			m_Attr = eObjectAttribute::MOVABLE;
			break;
		}
		case eObject1Name::TIRES:
		{
			m_Collider = new CCircle(_center.x, _center.y, _width / 2);
			m_Attr = eObjectAttribute::MOVABLE;
			break;
		}
		case eObject1Name::BARRICADE30:
		case eObject1Name::BARRICADE35:
		case eObject1Name::BARRICADE80:
		case eObject1Name::BARRICADE95:
		case eObject1Name::BARRICADE100:
		case eObject1Name::BARRICADE130:
		case eObject1Name::BARRICADE150:
		case eObject1Name::BARRICADE155:
		{
			m_Collider = new CAABB(
				_center.x - (_width / 2),
				_center.y - (_height / 2),
				_width,
				_height);
			m_Attr = eObjectAttribute::STATIC;
			// test
			m_Name1 = eObject1Name::BARRICADE30;
			break;
		}
		case eObject1Name::FENCE:
		{
			m_Collider = new CAABB(
				_center.x - (_width / 2),
				_center.y - (_height / 2),
				_width,
				_height);
			m_Attr = eObjectAttribute::STATIC;
			// test
			m_Name1 = eObject1Name::FENCE;
			break;
		}
		case eObject1Name::DISPLAY:
		case eObject1Name::GARAGE:
		case eObject1Name::LAMP:
		case eObject1Name::FOREST: 
		case eObject1Name::AUDIENCE:
		{
			m_Collider = new CAABB(
				_center.x - (_width / 2),
				_center.y - (_height / 2),
				_width,
				_height);
			m_Attr = eObjectAttribute::STATIC;
			// test
			m_Name1 = name;
			break;
		}

		default:
			break;
	}
}

void ModelObject::Initialize(eObject2Name name)
{
	// �����ڷκ��� �ʱ�ȭ�� ������ �������� �߰��� �� �����͸� �����Ѵ�.
//D2D1_VECTOR_2F _center = m_Collider->GetPosition();
	D2D1_VECTOR_2F _center = m_Transform->GetLocalPosition();
	int _width = m_bitmap->GetPixelSize().width;
	int _height = m_bitmap->GetPixelSize().height;

	_center.y = _center.y + (_height / 4);

	switch (name)
	{
	case eObject2Name::STONE1:
	{
		m_Collider = new CCircle(_center.x, _center.y, _width / 4);
		m_Attr = eObjectAttribute::STATIC;
		break;
	}
	case eObject2Name::STONE2:
	{
		m_Collider = new CCircle(_center.x, _center.y, _width / 2);
		m_Attr = eObjectAttribute::STATIC;
		break;
	}
	case eObject2Name::LONGCHAIN:
	case eObject2Name::DIAGONALCHAIN:
	case eObject2Name::SHORTCHAIN:
	{
		m_Collider = new CAABB(
			_center.x - (_width / 2),
			_center.y - (_height / 2),
			_width,
			_height);
		m_Attr = eObjectAttribute::STATIC;
		m_Name2 = name;
		break;
	}
	default:
		break;
	}
}

void ModelObject::Update(float dTime)
{
	// ���� ��ġ���� �ణ �������ش�.
	D2D1_VECTOR_2F _nowPos = m_Transform->GetLocalPosition();
	float _nowRot = m_Transform->GetLocalRotation();

	/// �������� ������ �ִ� �ݶ��̴��� ��ġ���� �������ش�.
	m_Collider->SetPosition(_nowPos);


	if (m_Attr == eObjectAttribute::MOVABLE)
	{
		// ������ ������
		D2D1_VECTOR_2F _frictionVec = CVector2::VectorMultiplyScalar(m_Velocity, 0.8f * dTime);	// 1�ʿ� 80%������ �Ҵ´�.
		m_Velocity = CVector2::VectorMinus(m_Velocity, _frictionVec);
	}
	else if (m_Attr == eObjectAttribute::STATIC)
	{
		m_Velocity = CVector2::VectorMultiplyScalar(m_Velocity, 0);
	}

	// �̵� ���� ����
	D2D1_VECTOR_2F _moveVec = CVector2::VectorMultiplyScalar(m_Velocity, dTime);
	// �ӵ� ���ͷ� �̵��Ѵ�.
	m_Transform->MoveByTranslate(_moveVec);
}

void ModelObject::Draw(D2DEngine* pEngine, float dTime, D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM, bool isIsometric)
{
	// �� ������Ʈ�� Skew(�� ���� Rotate, Scale)�� ���� �ʴ´�
	/// �̺κ� �� �𸣰ڽ��ϴ�...
	/// ������ �Ƿ��� �����ߴ�. ���׸� ã�ٴ�..

	// �̰� ����
	//D2D1_VECTOR_2F _nowPos = CVector2::VectorMultiplyMatrix_DX(m_bitmapCenter, projTM);
	pEngine->SetTransform(m_Transform->MatrixTranslation(m_Transform->GetIsometricsPosition(projTM)) * viewTM);


	// �����Բ��� ����� �Լ�
	///pEngine->SetTransform(m_Transform->MatrixTranslation(m_Transform->GetIsometricsPosition(projTM)) * viewTM);
	//pEngine->DrawEllipse_PreSetCol(0, 0, 5, BLUE_COLOR);
	pEngine->DrawGOImage(m_bitmap);
}

void ModelObject::DrawDebugData(D2DEngine* pEngine, D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM)
{
	// ����� ��� �ÿ��� Skew�� �ʿ��ϹǷ� projTM�� viewTM�� �Բ� ��������� �Ѵ�.
	/// ����ϰ� ������� ������, ���󿡼� �ణ Ʋ�����ִ�. �� �׷��� �˾ƺ��߰ڴ�
	//D2D1_VECTOR_2F _pos = m_Transform->GetLocalPosition();

	D2D1_VECTOR_2F _nowPos = m_Collider->GetPosition();
	//D2D1_VECTOR_2F _nowPos = m_Transform->GetLocalPosition();
	pEngine->SetTransform(m_Transform->MatrixTranslation(_nowPos) * projTM * viewTM);

	int _width = m_bitmap->GetPixelSize().width;
	int _height = m_bitmap->GetPixelSize().height;

	if (m_Name1 == eObject1Name::BARRICADE30)
	{
		pEngine->DrawRectangle_Debug(0, 0, _width, _height, m_Collider->GetIsCollided());
	}
	else
	{
		pEngine->DrawEllipse_Debug(0, 0, 10, m_Collider->GetIsCollided());
	}

	D2D1::ColorF _velocityColor = (D2D1::ColorF(0.0f, 1.0f, 1.0f, 1.0f));

	pEngine->DrawLineByVector(D2D1::Vector2F(), m_Velocity, _velocityColor);

	//pEngine->DrawText(400.0f, 400.0f, L"Velocity X = %.2f", m_Velocity.x);
	//pEngine->DrawText(400.0f, 400.0f, L"Velocity Y = %.2f", m_Velocity.y);
}

D2D1_VECTOR_2F ModelObject::GetVelocity()
{
	return m_Velocity;
}

void ModelObject::SetVelocity(D2D1_VECTOR_2F velocity)
{
	m_Velocity = velocity;
}
