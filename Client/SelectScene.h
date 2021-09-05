#pragma once
#include "IScene.h"

#include "GameManager.h"

class D2DEngine;
class SceneManager;
class GameManager;
class CarMK3;
class LHInput;
class ObjectManager;

class SelectScene : public IScene
{
public:
   SelectScene();
   ~SelectScene();

public:

   virtual void Initialize(D2DEngine* pEngine, SceneManager* pScene);
   virtual void OnStart() override;
   virtual void OnUpdate(float dTime);
   virtual void OnDraw(D2DEngine* pEngine, float dTime);
   virtual void ChangeScene() override;

   void KeyUpdate(float dTime);

public:

   float m_CountTime;

   /// 각각의 플레이어들은 차를 가리키게 될 예정
   int m_Player1;
   int m_Player2;
   int m_Player3;
   int m_Player4;

   int m_SelectTrack;


private:
   vector<D2D1_POINT_2F> v_CarPos;
   vector<D2D1_POINT_2F> v_TrackPos;

   LHInput* m_pUserInput;

   GameManager* m_GM;
   ObjectManager* m_pObjmanager;

   //ID2D1Bitmap* m_SelectBG;

   ID2D1Bitmap* m_TrackIamge1;
   ID2D1Bitmap* m_TrackIamge2;
   ID2D1Bitmap* m_TransparentBG1;
   ID2D1Bitmap* m_TransparentBG2;

   ID2D1Bitmap* m_CarImage1;
   ID2D1Bitmap* m_CarImage2;
   ID2D1Bitmap* m_CarImage3;
   ID2D1Bitmap* m_CarImage4;

   ID2D1Bitmap* m_CarAttr1;
   ID2D1Bitmap* m_CarAttr2;
   ID2D1Bitmap* m_CarAttr3;
   ID2D1Bitmap* m_CarAttr4;

   ID2D1Bitmap* m_RaceStart;

   ID2D1Bitmap* m_PlayerBox1;
   ID2D1Bitmap* m_PlayerBox2;
   ID2D1Bitmap* m_PlayerBox3;
   ID2D1Bitmap* m_PlayerBox4;

   ID2D1Bitmap* m_Setting;

   ID2D1Bitmap* m_Track1;
   ID2D1Bitmap* m_Track2;
   ID2D1Bitmap* m_pTrackSelect;
   ID2D1Bitmap* m_TrackSelect2;

   bool m_bCarSelect;
};

