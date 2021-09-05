#pragma once

#include "GlobalDefine.h"

#include "IScene.h"
#include <vector>

class XboxPadInput;

class ResultScene :
    public IScene
{
public:
    ResultScene();
    ~ResultScene();

    // 점수 가져오려고 시도함
    //static vector<float> m_score;

public:
    virtual void Initialize(D2DEngine* pEngine, SceneManager* pScene) override;
    virtual void OnStart() override;
    virtual void OnUpdate(float dTime) override;
    virtual void OnDraw(D2DEngine* pEngine, float dTime) override;
	virtual void ChangeScene() override;

    void KeyUpdate(float dTime);

private:

    bool m_IsExit;
    int m_Winner;

    XboxPadInput* m_pUserInput;

    ID2D1Bitmap* m_Winner1Bitmap;
    ID2D1Bitmap* m_Winner2Bitmap;
    ID2D1Bitmap* m_Winner3Bitmap;
    ID2D1Bitmap* m_Winner4Bitmap;
    
    ID2D1Bitmap* m_Result_PopupBitmap;
    
	ID2D1Bitmap* m_Result_Select_0_Bitmap;
	ID2D1Bitmap* m_Result_Select_1_Bitmap;
	ID2D1Bitmap* m_Result_Restart_0_Bitmap;
	ID2D1Bitmap* m_Result_Restart_1_Bitmap;
	ID2D1Bitmap* m_Result_Quit_0_Bitmap;
    ID2D1Bitmap* m_Result_Quit_1_Bitmap;

    


};

