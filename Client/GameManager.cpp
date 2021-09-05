#include "Resource.h"
#include "KeyInput.h"
#include "GameManager.h"
#include "../DRCommonLib/LHInput.h"
#include <wchar.h>
#include "D2DEngine.h"
#include "CTimeController.h"
#include "Character.h"
#include "Map.h"
#include "CGameObject.h"
#include "CarMK3.h"
#include "CVector.h"
#include "CTransform.h"
#include "CCircle.h"
#include "RaceManager.h"
#include "CameraManager.h"
#include "ModelObject.h"
#include "ObjectManager.h"
#include "SoundManager.h" 
#include "SceneManager.h"
#include "PlayerManager.h"

#include "atlstr.h"

#pragma comment(lib, "../Debug/DRCommonLib.lib")

// 창 클래스를 등록하고, 주 프로그램 창을 만든 다음 표시한다.
void GameManager::Initialize(HINSTANCE hInst)
{
   WNDCLASSEXW wcex;

   wcex.cbSize = sizeof(WNDCLASSEX);
   wcex.style = CS_HREDRAW | CS_VREDRAW;
   wcex.lpfnWndProc = WndProc;
   wcex.cbClsExtra = 0;
   wcex.cbWndExtra = 0;
   wcex.hInstance = hInst;
   wcex.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_CABONDEMO));
   wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
   wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
   wcex.lpszMenuName = NULL; //MAKEINTRESOURCEW(IDC_CABONDEMO);
   wcex.lpszClassName = szWindowClass;
   wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

   RegisterClassExW(&wcex);

   m_hWnd = CreateWindowExW(
      WS_EX_APPWINDOW,
      szWindowClass,				// 윈도우 클래스 이름
      szTitle,					// 제목 표시줄에 보여줄 이름
      WS_POPUP,		// 타이틀 바, 시스템 메뉴, 경계선 등 평범한 윈도우 스타일
      CW_USEDEFAULT,				// 윈도우 생성 시 초기 위치 x값
      0,							// 윈도우 생성 시 초기 위치 y값
      CW_USEDEFAULT,				// 윈도우 넓이
      0,							// 윈도우 높이
      GetDesktopWindow(),
      nullptr,
      hInst,
      nullptr);

   if (!m_hWnd)
   {
      return;
   }

   // 생성된 윈도우 핸들을 띄워준다.
   ShowWindow(m_hWnd, SW_SHOWDEFAULT);

   DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
   DWORD dwRemove = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
   DWORD dwNewStyle = dwStyle & ~dwRemove;

   ::SetWindowLong(m_hWnd, GWL_STYLE, dwNewStyle);
   ::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE
      | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

   HDC hDC = ::GetWindowDC(NULL);
   ::SetWindowPos(m_hWnd, NULL, 0, 0, ::GetDeviceCaps(hDC, HORZRES), ::GetDeviceCaps(hDC, VERTRES), SWP_FRAMECHANGED);

   UpdateWindow(m_hWnd);

   ObjectInitializer();

   m_BGMManager = SoundManager::GetInstance();
   SoundManager::Init();
   m_BGMManager->LoadAll();
}

void GameManager::ObjectInitializer()
{
   m_pEngine = D2DEngine::Instance();
   m_pEngine->Initialize(m_hWnd);

   m_pTimeController = new CTimeController();
   m_pTimeController->Initialize();
   m_pTimeController->StartTimeStamp();

   // 유저 입력
   m_pUserInput = new LHInput();
   m_pUserInput->Initialize();

   // 오브젝트, 맵, 레이스 매니저는 인게임씬 OnStart에서 로드해주자

   // 카메라
   m_pCamera = new CameraManager();


   // 오브젝트 매니저
   m_pObjManager = new ObjectManager(this);

   // 자동차 / 플레이어 new
   for (int i = 0; i < 4; i++)
   {
      m_pObjManager->CreateCar(m_pEngine, i, D2D1::Vector2F(50 + i * 50, 100));
   }

   // 레이스 매니저
   // 모든 플레이어가 생성되면 그때 시작한다.
   // 인자로 체크 포인트의 개수를 넘겨준다.
   m_pSceneManager = new SceneManager(m_pEngine, m_pTimeController, this);
   m_pSceneManager->Initialize();
}

void GameManager::ModelObejctCreate(eGameTrack track)
{
   switch (track)
   {
      case eTrack1:
      {
         //상단1
         for (int i = 0; i < 10; i++)
         {
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(780 + 200 * i, 170), eObject1Name::FENCE);
         }
         //상단1
         for (int i = 0; i < 9; i++)
         {
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(930 + 200 * i, 375 + 0.001f * i), eObject1Name::BARRICADE35);
         }
         //상단2
         for (int i = 0; i < 5; i++)
         {
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1500 + 200 * i, 440), eObject1Name::BARRICADE35);
         }
         //상단2
         for (int i = 0; i < 4; i++)
         {
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1650 + 200 * i, 630), eObject1Name::BARRICADE35);
         }

         //대각1
         {	float x = -10;//기울기
         float y = 100;
         int num = 5;
         float strX = 2710;//
         float strY = 600;
         float interval = 170;//간격 타이어30 바리게이트 150

         //타이어 간격동일하게 normalize
         float dist = x * x + y * y;
         x *= interval / sqrt(dist);
         y *= interval / sqrt(dist);

         for (int i = 0; i < num; i++)
         {
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(strX + x * i, strY + y * i), eObject1Name::BARRICADE150);
         }
         }
         //대각 1
         {	float x = -10;//기울기
         float y = 100;
         int num = 6;
         float strX = 2930;//
         float strY = 480;
         float interval = 200;//간격 타이어30 바리게이트 150

         //타이어 간격동일하게 normalize
         float dist = x * x + y * y;
         x *= interval / sqrt(dist);
         y *= interval / sqrt(dist);

         for (int i = 0; i < num; i++)
         {
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(strX + x * i, strY + y * i), eObject1Name::BARRICADE150);
         }
         }

         ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         //대각 2	
         {	float x = 10;//기울기
         float y = -2;
         int num = 1;
         float strX = 2400;//
         float strY = 1570;
         float interval = 150;//간격 타이어30 바리게이트 150

         //타이어 간격동일하게 normalize
         float dist = x * x + y * y;
         x *= interval / sqrt(dist);
         y *= interval / sqrt(dist);

         for (int i = 0; i < num; i++)
         {
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(strX + x * i, strY + y * i), eObject1Name::BARRICADE30);
         }
         }
         {	float x = 10;//기울기
         float y = -2;
         int num = 2;
         float strX = 2360;//
         float strY = 1800;
         float interval = 190;//간격 타이어30 바리게이트 150

         //타이어 간격동일하게 normalize
         float dist = x * x + y * y;
         x *= interval / sqrt(dist);
         y *= interval / sqrt(dist);

         for (int i = 0; i < num; i++)
         {
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(strX + x * i, strY + y * i), eObject1Name::BARRICADE30);
         }
         }



         ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         //대각 3


         {	float x = 10;//기울기
         float y = 8;
         int num = 2;
         float strX = 1880;//
         float strY = 1210;
         float interval = 170;//간격 타이어30 바리게이트 150

         //타이어 간격동일하게 normalize
         float dist = x * x + y * y;
         x *= interval / sqrt(dist);
         y *= interval / sqrt(dist);

         for (int i = 0; i < num; i++)
         {
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(strX + x * i, strY + y * i), eObject1Name::BARRICADE80);
         }
         }
         {	float x = 10;//기울기
         float y = 8;
         int num = 2;
         float strX = 2025;//
         float strY = 1095;
         float interval = 250;//간격 타이어30 바리게이트 150

         //타이어 간격동일하게 normalize
         float dist = x * x + y * y;
         x *= interval / sqrt(dist);
         y *= interval / sqrt(dist);

         for (int i = 0; i < num; i++)
         {
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(strX + x * i, strY + y * i), eObject1Name::BARRICADE80);
         }
         }
         ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         //대각 3-1 임시
         {	float x = 10;//기울기
         float y = 8;
         int num = 1;
         float strX = 2020;//
         float strY = 1510;
         float interval = 170;//간격 타이어30 바리게이트 150

         //타이어 간격동일하게 normalize
         float dist = x * x + y * y;
         x *= interval / sqrt(dist);
         y *= interval / sqrt(dist);

         for (int i = 0; i < num; i++)
         {
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(strX + x * i, strY + y * i), eObject1Name::BARRICADE150);
         }
         }

         {	float x = 10;//기울기
         float y = 8;
         int num = 1;
         float strX = 2240;//
         float strY = 1460;
         float interval = 200;//간격 타이어30 바리게이트 150

         //타이어 간격동일하게 normalize
         float dist = x * x + y * y;
         x *= interval / sqrt(dist);
         y *= interval / sqrt(dist);

         for (int i = 0; i < num; i++)
         {
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(strX + x * i, strY + y * i), eObject1Name::BARRICADE150);
         }
         }

         ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         //대각4
         {	float x = 10;//기울기
         float y = 10;
         int num = 1;
         float strX = 2230;//
         float strY = 830;
         float interval = 150;//간격 타이어30 바리게이트 150

         //타이어 간격동일하게 normalize
         float dist = x * x + y * y;
         x *= interval / sqrt(dist);
         y *= interval / sqrt(dist);

         for (int i = 0; i < num; i++)
         {
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(strX + x * i, strY + y * i), eObject1Name::BARRICADE95);
         }
         }

         {	float x = 10;//기울기
         float y = 12;
         int num = 1;
         float strX = 2200;//
         float strY = 1040;
         float interval = 150;//간격 타이어30 바리게이트 150

         //타이어 간격동일하게 normalize
         float dist = x * x + y * y;
         x *= interval / sqrt(dist);
         y *= interval / sqrt(dist);

         for (int i = 0; i < num; i++)
         {
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(strX + x * i, strY + y * i), eObject1Name::BARRICADE80);
         }
         }
         ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

         //대각5
         {	float x = -10;//기울기
         float y = 40;
         int num = 1;
         float strX = 2400;//
         float strY = 830;
         float interval = 150;//간격 타이어30 바리게이트 150

         //타이어 간격동일하게 normalize
         float dist = x * x + y * y;
         x *= interval / sqrt(dist);
         y *= interval / sqrt(dist);

         for (int i = 0; i < num; i++)
         {
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(strX + x * i, strY + y * i), eObject1Name::BARRICADE155);
         }
         }

         {	float x = -10;//기울기
         float y = 40;
         int num = 2;
         float strX = 2610;//
         float strY = 770;
         float interval = 190;//간격 타이어30 바리게이트 150

         //타이어 간격동일하게 normalize
         float dist = x * x + y * y;
         x *= interval / sqrt(dist);
         y *= interval / sqrt(dist);

         for (int i = 0; i < num; i++)
         {
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(strX + x * i, strY + y * i), eObject1Name::BARRICADE155);
         }
         }

         ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         //대각6
         {	float x = 10;//기울기
         float y = 13.5;
         int num = 4;
         float strX = 730;//
         float strY = 660;
         float interval = 250;//간격 타이어30 바리게이트 150

         //타이어 간격동일하게 normalize
         float dist = x * x + y * y;
         x *= interval / sqrt(dist);
         y *= interval / sqrt(dist);

         for (int i = 0; i < num; i++)
         {
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(strX + x * i, strY + y * i), eObject1Name::BARRICADE100);
         }
         }

         {	float x = 10;//기울기
         float y = 13;
         int num = 4;
         float strX = 910;//
         float strY = 610;
         float interval = 250;//간격 타이어30 바리게이트 150

         //타이어 간격동일하게 normalize
         float dist = x * x + y * y;
         x *= interval / sqrt(dist);
         y *= interval / sqrt(dist);

         for (int i = 0; i < num; i++)
         {
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(strX + x * i, strY + y * i), eObject1Name::BARRICADE100);
         }
         }

         ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         //대각7
         {	float x = 10;//기울기
         float y = 29;
         int num = 2;
         float strX = 1260;//
         float strY = 840;
         float interval = 220;//간격 타이어30 바리게이트 150

         //타이어 간격동일하게 normalize
         float dist = x * x + y * y;
         x *= interval / sqrt(dist);
         y *= interval / sqrt(dist);

         for (int i = 0; i < num; i++)
         {
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(strX + x * i, strY + y * i), eObject1Name::BARRICADE130);
         }
         }

         {	float x = 10;//기울기
         float y = 32;
         int num = 2;
         float strX = 1450;//
         float strY = 845;
         float interval = 110;//간격 타이어30 바리게이트 150

         //타이어 간격동일하게 normalize
         float dist = x * x + y * y;
         x *= interval / sqrt(dist);
         y *= interval / sqrt(dist);

         for (int i = 0; i < num; i++)
         {
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(strX + x * i, strY + y * i), eObject1Name::BARRICADE130);
         }
         }

         ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         //대각8
         {	float x = 10;//기울기
         float y = 12;
         int num = 2;
         float strX = 1510;//
         float strY = 1225;
         float interval = 250;//간격 타이어30 바리게이트 150

         //타이어 간격동일하게 normalize
         float dist = x * x + y * y;
         x *= interval / sqrt(dist);
         y *= interval / sqrt(dist);

         for (int i = 0; i < num; i++)
         {
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(strX + x * i, strY + y * i), eObject1Name::BARRICADE95);
         }
         }
         {	float x = 10;//기울기
         float y = 12;
         int num = 3;
         float strX = 1660;//
         float strY = 1145;
         float interval = 180;//간격 타이어30 바리게이트 150

         //타이어 간격동일하게 normalize
         float dist = x * x + y * y;
         x *= interval / sqrt(dist);
         y *= interval / sqrt(dist);

         for (int i = 0; i < num; i++)
         {
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(strX + x * i, strY + y * i), eObject1Name::BARRICADE95);
         }
         }
         ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         //대각9
         {	float x = 10;//기울기
         float y = -1;
         int num = 2;
         float strX = 1500;//
         float strY = 1730;
         float interval = 100;//간격 타이어30 바리게이트 150

         //타이어 간격동일하게 normalize
         float dist = x * x + y * y;
         x *= interval / sqrt(dist);
         y *= interval / sqrt(dist);

         for (int i = 0; i < num; i++)
         {
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(strX + x * i, strY + y * i), eObject1Name::BARRICADE30);
         }
         }

         {	float x = 10;//기울기
         float y = -2;
         int num = 2;
         float strX = 1530;//
         float strY = 1930;
         float interval = 200;//간격 타이어30 바리게이트 150

         //타이어 간격동일하게 normalize
         float dist = x * x + y * y;
         x *= interval / sqrt(dist);
         y *= interval / sqrt(dist);

         for (int i = 0; i < num; i++)
         {
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(strX + x * i, strY + y * i), eObject1Name::BARRICADE30);
         }
         }

         //타이어 1
         {float tx = -10;
         float ty = -10;
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2845 + tx, 210 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2875 + tx, 220 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2905 + tx, 230 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2930 + tx, 240 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2950 + tx, 260 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2975 + tx, 275 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2995 + tx, 300 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(3010 + tx, 320 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(3030 + tx, 345 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(3045 + tx, 370 + ty), eObject1Name::TIRES);
         } {float tx = -25;
         float ty = 10;
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2890 + tx, 525 + ty), eObjectName::TIRES);
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2880 + tx, 500 + ty), eObjectName::TIRES);
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2890 + tx, 485 + ty), eObjectName::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2870 + tx, 470 + ty), eObject1Name::TIRES);
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2875 + tx, 455 + ty), eObjectName::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2860 + tx, 440 + ty), eObject1Name::TIRES);
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2850 + tx, 430 + ty), eObjectName::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2840 + tx, 420 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2820 + tx, 410 + ty), eObject1Name::TIRES);
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2795 + tx, 400 + ty), eObjectName::TIRES);
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2770 + tx, 395 + ty), eObjectName::TIRES);
         }
         //타이어 2	
         {
            float tx = -30;
            float ty = -30;
            //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2970, 1500), eObjectName::TIRES);
            //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2960, 1530), eObjectName::TIRES);
            //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2950, 1560), eObjectName::TIRES);
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2935, 1590), eObject1Name::TIRES);
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2920, 1620), eObject1Name::TIRES);
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2900, 1645), eObject1Name::TIRES);
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2880, 1670), eObject1Name::TIRES);
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2855, 1690), eObject1Name::TIRES);
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2830, 1710), eObject1Name::TIRES);
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2805, 1720), eObject1Name::TIRES);
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2780, 1735), eObject1Name::TIRES);

            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2780 + tx, 1415 + ty), eObject1Name::TIRES);
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2765 + tx, 1440 + ty), eObject1Name::TIRES);
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2755 + tx, 1465 + ty), eObject1Name::TIRES);
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2745 + tx, 1485 + ty), eObject1Name::TIRES);
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2735 + tx, 1510 + ty), eObject1Name::TIRES);
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2715 + tx, 1530 + ty), eObject1Name::TIRES);
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2690 + tx, 1550 + ty), eObject1Name::TIRES);
            m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2660 + tx, 1560 + ty), eObject1Name::TIRES);
         }
         //타이어 3

         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2600, 490), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2625, 500), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2645, 520), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2670, 535), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2690, 555), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2710, 575), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2725, 600), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2735, 625), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2745, 650), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2755, 675), eObject1Name::TIRES);

         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2585, 780), eObjectName::TIRES);
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2570, 755), eObjectName::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2560, 730), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2550, 705), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2525, 685), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2501, 670), eObject1Name::TIRES);

         //타이어 4
         //우측 5시 안
         {float tx = -30;
         float ty = -30;
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2670, 1075), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2650, 1100), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2625, 1125), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2600, 1145), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2570, 1165), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2535, 1175), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2500, 1185), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2470, 1185), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2440, 1190), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2410, 1180), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2375, 1175), eObject1Name::TIRES);

         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2385 + tx, 970 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2410 + tx, 980 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2430 + tx, 990 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2455 + tx, 995 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2480 + tx, 990 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2500 + tx, 975 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2515 + tx, 955 + ty), eObject1Name::TIRES);
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2530, 945), eObjectName::TIRES);
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2545, 925), eObjectName::TIRES);
         }
         //타이어 5
         //우측 6시밖
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2395, 1815), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2365, 1810), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2340, 1800), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2315, 1785), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2290, 1770), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2265, 1750), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2245, 1730), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2230, 1705), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2215, 1680), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2205, 1660), eObject1Name::TIRES);
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2200, 1625), eObjectName::TIRES);

         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2455, 1640), eObjectName::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2440, 1620), eObject1Name::TIRES);
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2420, 1630), eObjectName::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2405, 1620), eObject1Name::TIRES);
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2395, 1610), eObjectName::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2390, 1590), eObject1Name::TIRES);
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2390, 1575), eObjectName::TIRES);
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2390, 1555), eObjectName::TIRES);
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2390, 1540), eObjectName::TIRES);

         //타이어 6
         //중심
         {float tx = -30;
         float ty = -20;
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2170 + tx, 765 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2145 + tx, 755 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2120 + tx, 745 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2090 + tx, 740 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2060 + tx, 730 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2030 + tx, 730 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2000 + tx, 730 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1975 + tx, 740 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1945 + tx, 750 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1920 + tx, 760 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1895 + tx, 775 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1880 + tx, 790 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1865 + tx, 810 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1845 + tx, 825 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1835 + tx, 840 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1825 + tx, 860 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1815 + tx, 875 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1805 + tx, 900 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1795 + tx, 930 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1785 + tx, 955 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1780 + tx, 985 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1785 + tx, 1010 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1790 + tx, 1040 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1795 + tx, 1070 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1805 + tx, 1095 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1820 + tx, 1120 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1835 + tx, 1140 + ty), eObject1Name::TIRES);
         }
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2010, 1085), eObjectName::TIRES);
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2000, 1065), eObjectName::TIRES);
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1990, 1050), eObjectName::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1980, 1030), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1975, 1010), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1980, 990), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1985, 970), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1995, 955), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2005, 940), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2020, 930), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2040, 920), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2055, 925), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2075, 935), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2095, 940), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2115, 950), eObject1Name::TIRES);

         //타이어7
            // 첫번째 코너 바깥

         {float tx = -25;
         float ty = -20;
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(880+tx, 210+ty), eObjectName::TIRES);
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(860+tx, 220+ty), eObjectName::TIRES);
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(840+tx, 225+ty), eObjectName::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(820 + tx, 230 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(800 + tx, 240 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(780 + tx, 250 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(760 + tx, 260 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(745 + tx, 280 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(730 + tx, 300 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(715 + tx, 315 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(700 + tx, 335 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(690 + tx, 355 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(680 + tx, 375 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(670 + tx, 395 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(665 + tx, 415 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(665 + tx, 435 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(670 + tx, 455 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(670 + tx, 475 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(675 + tx, 495 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(680 + tx, 515 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(690 + tx, 535 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(700 + tx, 555 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(710 + tx, 575 + ty), eObject1Name::TIRES);
         // 첫번째 코너 안쪽									  +	  tx	 +ty			  1
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(960 + tx, 400 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(935 + tx, 405 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(920 + tx, 420 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(915 + tx, 440 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(905 + tx, 460 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(900 + tx, 480 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(895 + tx, 500 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(895 + tx, 520 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(900 + tx, 540 + ty), eObject1Name::TIRES);
         }
         //타이어8
         //두 번째 코너 바깥
         {float tx = -30;
         float ty = -20;
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1356 + tx, 1452 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1366 + tx, 1472 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1367 + tx, 1493 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1365 + tx, 1515 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1358 + tx, 1539 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1337 + tx, 1562 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1327 + tx, 1582 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1314 + tx, 1604 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1303 + tx, 1626 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1291 + tx, 1648 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1283 + tx, 1673 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1278 + tx, 1697 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1277 + tx, 1721 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1278 + tx, 1745 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1283 + tx, 1772 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1291 + tx, 1796 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1299 + tx, 1820 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1311 + tx, 1843 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1323 + tx, 1864 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1340 + tx, 1883 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1358 + tx, 1900 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1380 + tx, 1915 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1402 + tx, 1928 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1426 + tx, 1937 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1452 + tx, 1943 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1477 + tx, 1947 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1503 + tx, 1949 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1528 + tx, 1950 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1558 + tx, 1950 + ty), eObject1Name::TIRES);
         //두 번째 코너 안쪽
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1506, 1356), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1517, 1377), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1526, 1400), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1533, 1424), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1538, 1447), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1544, 1470), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1543, 1494), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1542, 1518), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1539, 1541), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1533, 1564), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1526, 1587), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1516, 1608), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1505, 1630), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1493, 1651), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1478, 1670), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1477, 1695), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1476, 1719), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1482, 1743), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1497, 1759), eObject1Name::TIRES);
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1510, 1765), eObjectName::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1525, 1760), eObject1Name::TIRES);
         }
         //타이어9
         //세 번째 코너 안쪽
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1842, 1731), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1858, 1716), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1869, 1695), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1870, 1671), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1865, 1648), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1858, 1625), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1844, 1607), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1830, 1588), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1815, 1569), eObject1Name::TIRES);
         //세 번재 코너 바깥쪽
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1964, 1858), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1981, 1841), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1996, 1823), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2008, 1803), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2018, 1782), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2026, 1762), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2034, 1737), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2039, 1716), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2040, 1692), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2040, 1669), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2039, 1646), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2036, 1621), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2030, 1598), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(2024, 1576), eObject1Name::TIRES);

         //10네 번째 코너 바깥쪽
         {float tx = -35;
         float ty = -25;
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1380+tx, 790+ty), eObjectName::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1376 + tx, 767 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1372 + tx, 743 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1371 + tx, 718 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1373 + tx, 694 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1376 + tx, 671 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1378 + tx, 648 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1382 + tx, 624 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1389 + tx, 603 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1402 + tx, 584 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1413 + tx, 565 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1429 + tx, 547 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1445 + tx, 533 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1462 + tx, 517 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1483 + tx, 506 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1504 + tx, 495 + ty), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1526 + tx, 488 + ty), eObject1Name::TIRES);
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1548, 482), eObjectName::TIRES);
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1572, 479), eObjectName::TIRES);
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1595, 476), eObjectName::TIRES);
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1618, 473), eObjectName::TIRES);
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1642, 473), eObjectName::TIRES);
         }
         //네 번째 코너 안쪽
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1569, 823), eObjectName::TIRES);
         //m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1569, 801), eObjectName::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1567, 778), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1567, 756), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1567, 734), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1567, 711), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1571, 689), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1590, 676), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1611, 666), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1631, 657), eObject1Name::TIRES);
         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(1650, 645), eObject1Name::TIRES);



         // 트랙용 더미 오브젝트
         //x축1_시작라인
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1900, 120), 2500.0f, 200.0f, 180.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1870, 430), 1750.0f, 100.0f, 0.0f);

         //타이어7 
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(700, 180), 500.0f, 200.0f, 150.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(690, 190), 500.0f, 200.0f, 135.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(630, 280), 500.0f, 200.0f, 120.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(580, 300), 500.0f, 200.0f, 90.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(610, 600), 200.0f, 200.0f, 60.0f);

         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1010, 450), 100.0f, 100.0f, -60.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(995, 510), 100.0f, 100.0f, -90.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(997, 565), 50.0f, 100.0f, -100.0f);

         //대각6
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(958, 1070), 1050.0f, 200.0f, 53.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1265, 950), 950.0f, 100.0f, 233.0f);

         //타이어8
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1300, 1450), 100.0f, 100.0f, 53.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1270, 1490), 50.0f, 200.0f, 90.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1270, 1490), 50.0f, 200.0f, 110.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1290, 1550), 200.0f, 100.0f, 123.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1190, 1800), 300.0f, 200.0f, 90.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1260, 1950), 300.0f, 200.0f, 60.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1310, 1980), 300.0f, 200.0f, 30.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1500, 2000), 300.0f, 100.0f, 5.0f);

         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1590, 1440), 250.0f, 100.0f, -107.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1595, 1500), 200.0f, 100.0f, -90.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1590, 1550), 250.0f, 100.0f, -70.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1600, 1700), 70.0f, 200.0f, -90.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1605, 1695), 70.0f, 200.0f, -120.0f);

         //대각9
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1680, 1715), 300.0f, 100.0f, 170.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1750, 1985), 450.0f, 100.0f, -5.0f);

         //타이어9
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1900, 1955), 200.0f, 100.0f, -20.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2000, 1885), 300.0f, 100.0f, -45.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2080, 1780), 300.0f, 100.0f, -65.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2090, 1685), 300.0f, 100.0f, -90.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2075, 1585), 300.0f, 100.0f, -110.0f);

         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1750, 1665), 70.0f, 220.0f, 110.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1752, 1650), 50.0f, 220.0f, 100.0f);

         //대각8
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1645, 1435), 600.0f, 80.0f, 50.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1400, 1000), 400.0f, 80.0f, 75.0f);

         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1860, 1270), 480.0f, 80.0f, -130.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1665, 950), 300.0f, 80.0f, -108.0f);

         //타이어10
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1680, 770), 130.0f, 180.0f, -80.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1695, 750), 110.0f, 180.0f, -30.0f);

         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1330, 700), 300.0f, 100.0f, 90.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1380, 500), 200.0f, 100.0f, 120.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1400, 500), 200.0f, 100.0f, 140.0f);

         //X축2
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2050, 435), 1200.0f, 100.0f, 180.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2080, 685), 800.0f, 70.0f, 0.0f);

         //타이어3
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2730, 495), 200.0f, 100.0f, 210.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2790, 555), 200.0f, 100.0f, -110.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2810, 680), 200.0f, 80.0f, -100.0f);

         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2470, 770), 100.0f, 200.0f, 30.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2470, 770), 100.0f, 200.0f, 85.0f);

         //대각5
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2510, 850), 130.0f, 100.0f, 105.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2785, 930), 400.0f, 100.0f, -75.0f);

         //타이어4
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2670, 1150), 300.0f, 100.0f, -45.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2520, 1230), 300.0f, 100.0f, -20.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2405, 1230), 300.0f, 100.0f, 20.0f);

         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2445, 890), 70.0f, 200.0f, 130.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2475, 880), 40.0f, 200.0f, 180.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2465, 875), 80.0f, 200.0f, -155.0f);

         //대각4
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2345, 825), 250.0f, 100.0f, -134.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2205, 1100), 300.0f, 100.0f, 45.0f);

         //타이어6
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2070, 980), 100.0f, 100.0f, 25.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2065, 980), 80.0f, 100.0f, -55.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2057, 1030), 80.0f, 100.0f, -100.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2057, 1030), 50.0f, 100.0f, -150.0f);

         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2165, 725), 250.0f, 60.0f, -155.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2000, 700), 250.0f, 60.0f, 180.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1900, 730), 250.0f, 60.0f, 150.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1820, 810), 250.0f, 60.0f, 125.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1760, 950), 250.0f, 60.0f, 90.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1785, 1110), 250.0f, 60.0f, 70.0f);

         //대각3
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1990, 1320), 400.0f, 60.0f, 35.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2170, 1450), 80.0f, 60.0f, 42.0f);

         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2200, 1170), 400.0f, 60.0f, -143.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2365, 1300), 200.0f, 100.0f, -130.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2430, 1400), 200.0f, 100.0f, -100.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2450, 1520), 100.0f, 100.0f, -80.0f);

         //타이어5
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2448, 1578), 50.0f, 100.0f, -100.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2450, 1570), 70.0f, 100.0f, -165.0f);

         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2170, 1480), 50.0f, 100.0f, 90.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2168, 1530), 150.0f, 100.0f, 97.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2180, 1700), 200.0f, 100.0f, 70.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2230, 1800), 200.0f, 100.0f, 45.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2320, 1860), 200.0f, 100.0f, 30.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2400, 1870), 200.0f, 100.0f, 0.0f);

         //대각2
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2550, 1550), 200.0f, 100.0f, 165.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2620, 1830), 350.0f, 100.0f, -15.0f);

         //타이어2
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2920, 1715), 300.0f, 100.0f, -30.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2990, 1630), 300.0f, 100.0f, -50.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(3015, 1580), 300.0f, 100.0f, -80.0f);

         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2673, 1500), 100.0f, 100.0f, 145.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2730, 1430), 120.0f, 100.0f, 125.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2770, 1340), 120.0f, 100.0f, 105.0f);

         //대각1
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2810, 920), 830.0f, 100.0f, 95.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(3085, 940), 1000.0f, 100.0f, -85.0f);

         //타이어1
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(3090, 350), 200.0f, 100.0f, -120.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2970, 215), 300.0f, 100.0f, -150.0f);

         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2845, 500), 80.0f, 100.0f, 90.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2825, 470), 80.0f, 100.0f, 45.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2790, 450), 80.0f, 100.0f, 25.0f);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2755, 435), 40.0f, 100.0f, 10.0f);

         m_pObjManager->CreateModelObjectByName(D2D1::Vector2F(350, 1200), eObject1Name::FOREST);


         break;
      }


      case eTrack2:
      {
         //2트랙 Colliders
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1250, 550), 500, 100, 90);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1520, 845), 500, 100, 0);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1500, 320), 500, 100, 180);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1900, 385), 350, 100, 180);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1900, 780), 350, 100, 0);

         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1770, 400), 100, 100, -90);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1770, 760), 100, 100, -90);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2130, 450), 60, 100, -90);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2130, 700), 60, 100, -90);

         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2620, 585), 200, 100, -90);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2450, 840), 460, 100, -40);

         //중단 벽
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1570, 1200), 60, 50, 50);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1740, 1400), 60, 50, 50);

         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2110, 760), 60, 50, -130);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2270, 950), 60, 50, -130);


         //하단
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1400, 1320), 460, 100, 140);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1220, 1560), 250, 100, 90);

         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2060, 1400), 100, 100, 90);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2060, 1760), 100, 100, 90);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1710, 1470), 60, 100, 90);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1710, 1700), 60, 100, 90);

         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2350, 1330), 500, 100, 180);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2600, 1600), 500, 100, -90);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2350, 1850), 500, 100, 0);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1950, 1770), 350, 100, 0);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1950, 1400), 350, 100, 180);

         //사슬
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2340, 435), 500, 100, 180);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2170, 730), 200, 100, 0);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2150, 700), 200, 100, 140);


         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1525, 1720), 500, 100, 0);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1685, 1450), 200, 100, -180);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1690, 1470), 200, 100, -40);

         //동상
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1500, 585), 50, 80, 0);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1500, 585), 50, 80, 60);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1500, 585), 50, 80, 120);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1500, 585), 50, 80, 180);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1500, 585), 50, 80, 240);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1500, 585), 50, 80, 300);


         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2335, 1595), 50, 80, 0);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2335, 1595), 50, 80, 60);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2335, 1595), 50, 80, 120);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2335, 1595), 50, 80, 180);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2335, 1595), 50, 80, 240);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2335, 1595), 50, 80, 300);
         //센터 워
         {float w = 40;
         float h = 20;


         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1740, 1240), w, h, 0);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1740, 1240), w, h, 60);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1740, 1240), w, h, 120);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1740, 1240), w, h, 180);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1740, 1240), w, h, 240);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1740, 1240), w, h, 300);

         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1780, 1120), w, h, 0);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1780, 1120), w, h, 60);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1780, 1120), w, h, 120);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1780, 1120), w, h, 180);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1780, 1120), w, h, 240);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1780, 1120), w, h, 300);

         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1860, 1200), w, h, 0);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1860, 1200), w, h, 60);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1860, 1200), w, h, 120);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1860, 1200), w, h, 180);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1860, 1200), w, h, 240);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1860, 1200), w, h, 300);

         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1910, 1090), w, h, 0);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1910, 1090), w, h, 60);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1910, 1090), w, h, 120);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1910, 1090), w, h, 180);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1910, 1090), w, h, 240);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1910, 1090), w, h, 300);

         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1980, 965), w, h, 0);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1980, 965), w, h, 60);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1980, 965), w, h, 120);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1980, 965), w, h, 180);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1980, 965), w, h, 240);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(1980, 965), w, h, 300);

         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2055, 1050), w, h, 0);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2055, 1050), w, h, 60);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2055, 1050), w, h, 120);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2055, 1050), w, h, 180);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2055, 1050), w, h, 240);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2055, 1050), w, h, 300);

         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2110, 920), w, h, 0);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2110, 920), w, h, 60);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2110, 920), w, h, 120);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2110, 920), w, h, 180);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2110, 920), w, h, 240);
         m_pObjManager->CreateDummyObject(D2D1::Vector2F(2110, 920), w, h, 300);	break;
         }
         break;
      }
   }


}


void GameManager::CameraUpdate(CarMK3* target)
{
   /// 카메라가 따라다닐 타겟(자동차) 
   // 어떻게 위치를 가져와야할지 모르겠다...
   D2D1_VECTOR_2F _cameraCenter = m_pObjManager->GetCars(0)->GetTransform()->GetLocalPosition();

   //m_pCamera->SetNowCenter(_cameraCenter);

   /// 개발 도중, 탑뷰냐 아이소매트릭이냐
   /*
   if (m_MakeIsomatric)
   {
      //m_pCamera->MakeIsometricTM(D2D1::Vector2F(1000, 0), 0.5f, 45.0f);

   }
   else
   {
      m_pCamera->MakeTopViewTM();
   }
   */

   m_pCamera->MakeIsometricTM(_cameraCenter, 0.5f, 45.0f);
   //m_pCamera->MoveHorizontal(_cameraCenter.x);
   //m_pCamera->MoveVertical(_cameraCenter.y);

   /// 선형 보간 lerp 곡선
   // 3가지 변수가 필요하다 	 
   //D2D1_VECTOR_2F _pos1;		// 처음 위치
   //D2D1_VECTOR_2F _pos2;		// 나중 위치
   //float factor = 0.5f;		// 중간에 보간될 위치

   //D2D1_VECTOR_2F _resultPos;	// 결과 위치
   //_resultPos = CVector2::VectorPlus(
   //	CVector2::VectorScalarMultiply(_pos1, 1.0f - factor),
   //	CVector2::VectorScalarMultiply(_pos2, factor));
}

void GameManager::Looping()
{
   MSG msg;

   // 기본 메시지 루프입니다:
   while (true)
   {
      if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
      {
         TranslateMessage(&msg);
         DispatchMessage(&msg);

         // WM_QUIT 메시지가 입력되면, while문을 벗어난다.
         if (msg.message == WM_QUIT)
         {
            break;
         }
      }
      else
      {
         /*
         if (m_pTimeController->FPSCheck() == false)
         {
            continue;
         }
         */
         // 처리할 메시지가 없다면 GameProc을 돌리자
         // 실제 게임의 구현이 들어가있다.
         m_pSceneManager->FiniteStateMachine();
      }
   }
}

void GameManager::Finish()
{
   delete m_pEngine;
}

LRESULT GameManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   switch (message)
   {
      case WM_PAINT:
      {
         PAINTSTRUCT ps;
         HDC hdc = BeginPaint(hWnd, &ps);
         // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
         EndPaint(hWnd, &ps);
      }
      break;
      case WM_DESTROY:
         PostQuitMessage(0);
         break;
      default:
         return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

