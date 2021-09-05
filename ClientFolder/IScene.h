#pragma once

#include "GlobalDefine.h"

class D2DEngine;
class SceneManager;

class IScene
{
	
protected:
	SceneManager* m_pSceneManager;

public:
	
	virtual void Initialize(D2DEngine* pEngine, SceneManager* pScene) abstract;	// 씬 클래스를 초기화해준다.
	virtual void OnStart() abstract;
	virtual void OnUpdate(float dTime) abstract;				// 씬 클래스 업데이트
	virtual void OnDraw(D2DEngine* pEngine, float dTime) abstract;					// 씬 그리기
	virtual void ChangeScene() abstract;

};

