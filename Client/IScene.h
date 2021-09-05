#pragma once

#include "GlobalDefine.h"

class D2DEngine;
class SceneManager;

class IScene
{
	
protected:
	SceneManager* m_pSceneManager;

public:
	
	virtual void Initialize(D2DEngine* pEngine, SceneManager* pScene) abstract;	// �� Ŭ������ �ʱ�ȭ���ش�.
	virtual void OnStart() abstract;
	virtual void OnUpdate(float dTime) abstract;				// �� Ŭ���� ������Ʈ
	virtual void OnDraw(D2DEngine* pEngine, float dTime) abstract;					// �� �׸���
	virtual void ChangeScene() abstract;

};

