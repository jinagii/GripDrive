#pragma once

#include "GlobalDefine.h"

using namespace std;

class CASprite;
class CGameObject;
class Character;
class ICollisionObject;

// 프리셋으로 추가하려는 색상이 있다면, enum에도 해당 항목을 추가하자.
enum eBRUSHCOLOR
{
	BLACK_COLOR = 0,
	WHITE_COLOR,
	RED_COLOR,
	GREEN_COLOR,
	BLUE_COLOR,
	MAX_COLOR
};


class D2DEngine
{

	///--------------------------------------------------------------------------
	/// 멤버 변수
private:
	HWND m_hWnd;

	// 팩토리
	//ID2D1Factory* m_pFactory;				// 함수들의 집합
	ID2D1Factory1* m_pD2D1Factory1;				// 기존 팩토리를 상속받은 D2D1_1 버전 팩토리
	IWICImagingFactory* m_pWICFactory;

	// 텍스트 관련
	IDWriteFactory* m_pWriteFactory;
	IDWriteTextFormat* m_pTextFormat;

	// 이미지 관련
	IWICBitmapDecoder*		m_pWICDecoder;
	IWICFormatConverter*	m_pWICConverter;
	IWICBitmapFrameDecode*	m_pFrame;

	// 렌더 타겟
	//ID2D1HwndRenderTarget*	m_pRenderTarget;		// 그릴 대상
	ID2D1DeviceContext* m_pD21DDeviceContext;			// 렌더 타겟 대신에 사용할 DC
	D3D_FEATURE_LEVEL	m_featureLevel;				// enumeration, 다이렉트의 기능을 어디까지 사용할 것인지
	ID2D1Device* m_pD2D1Device1;					// 뭔지 잘 모르겠다

	IDXGISwapChain1* m_pSwapChain;					
	// 스왑 체인은 우리가 알고 있는 더블 버퍼링을 의미한다.
	// 더블 버퍼링이 전면과 후면을 스왑(교체)해주면서 스크린에 보여주는 방식이
	// 마치 사슬과 같은 모양으로 연결된다고 해서 붙여진 이름이다.
	// 이에 따라 SwapChain은 GetBuffer나 Present와 같이 버퍼를 가져오고 보여주는 기능을 갖고 있다.

	ID2D1Bitmap1* m_pD2DTargetBitmap;

	float m_dpi;

	ID2D1Bitmap* m_pBitmap;							// 테스트용 비트맵

	// 브러쉬
	ID2D1SolidColorBrush*	m_pBlackBrush;			// 검정 브러쉬
	ID2D1SolidColorBrush*	m_pWhiteBrush;			// 하얀 브러쉬
	ID2D1SolidColorBrush*	m_pRedBrush;			// R 브러쉬
	ID2D1SolidColorBrush*	m_pGreenBrush;			// G 브러쉬
	ID2D1SolidColorBrush*	m_pBlueBrush;			// B 브러쉬
	ID2D1SolidColorBrush*	m_pTempBrush;			// Temp 브러쉬
	ID2D1SolidColorBrush*	m_pNowBrush;			// Now 브러쉬
	ID2D1SolidColorBrush*	arr_MyBrush[MAX_COLOR];

	ID2D1SolidColorBrush* m_pSkidMarkBrush;			// 스키드마크 브러쉬

	// 오류 판단
	HRESULT m_IsReady;		// 다음으로 진행할지 말지 결정한다

	D2D1_SIZE_F m_RenderTargetSize;

public:
	///--------------------------------------------------------------------------
	/// 애니메이션 관련 현재는 m_Player만 활용
	std::vector<CASprite*> v_Sprite;

	// 스프라이트 관련
	int m_SpriteCount;			// 스프라이트의 갯수 (인덱스는 0부터)
	int m_SpriteSheetCount;		// 스프라이트 시트의 갯수 (인덱스는 0부터)
	std::map<std::string, ID2D1Bitmap*> m_SpriteSheets;	// 스프라이트 시트

public:
	///--------------------------------------------------------------------------
	/// 싱글톤 관련

	// 엔진 포인터를 반환해주는 함수를 만든다.
	static D2DEngine* Instance();

protected:
	// 기본 생성자
	// 싱글톤을 사용하게 되면, 하나의 인스턴스만이 존재해야하므로 기본 생성자를 public으로 둘 수 없다.
	D2DEngine();

public:
	~D2DEngine();

private:
	// 싱글톤을 통해 받을 포인터
	static D2DEngine* _instance;


public:
	///--------------------------------------------------------------------------
	/// 초기화 관련

	// 초기화
	// D2D 및 D3D, 텍스트 관련 팩토리를 생성한다.
	void Initialize(HWND hWnd);
	
private:
	// 디바이스 컨텍스트, 스왑체인, 기본 브러쉬 등을 생성하는 함수.
	// Initialize 함수에 의해 호출된다.
	HRESULT CreateDeviceResources();

public:
	void	BeginRender();				// 렌더링 시작 전 초기화 함수
	void	EndRender();				// EndDraw와 스왑 체인을 호출한다.
										// 오류 발생 시 DiscardDeviceResources를 호출한다.
	void    DiscardDeviceResources();	// Device Context를 삭제하는 함수
										// 오류에 의해 삭제된 Device Context는 BeginRender에서 다시 생성된다.				
	void	DiscardFactory();			// 게임 종료 시 호출된다.	
										// 대부분의 포인터형 자원들이 여기서 삭제된다.
	///--------------------------------------------------------------------------
	/// 이미지 생성 관련
public:

	// 파일 경로를 통해 이미지를 불러오는 함수
	// 리턴 값이 HRESULT이므로 단순히 사용할 수는 없고, 래핑해서 사용하는 것을 권장.
	HRESULT LoadBitmapFromFile(
		ID2D1RenderTarget* pRenderTarget,	// 렌더 타겟
		IWICImagingFactory* pIWICFactory,	// Initialize에서 생성된 팩토리
		PCWSTR path,						// 파일명 (이미지의 경로)
		UINT destinationWidth,				// 이미지의 가로 크기
		UINT destinationHeight,				// 이미지의 세로 크기	
		ID2D1Bitmap** ppBitmap);			// 이미지를 넣어줄 비트맵 인터페이스

	// ID2D1Bitmap을 즉시 반환해서 쓸 수 있게 래핑했다.
	// LoadBitmapFromFile과는 다르게 게임 게임 로직 내에서도 사용 가능하다.
	ID2D1Bitmap* LoadBitmapByPath(PCWSTR path);

	// 파일 이름을 통해 로드한 이미지를 string과 pair해서 STL Map에 넣는다.
	// 나중에 해당하는 string으로 다시 불러올 수 있다.
	int LoadSpriteSheet(const wchar_t* filePath, std::string str);

	// 현재 이 코드는 테스트용도로 만들어진 코드.
	// 스프라이트 시트가 꽉 차있다는 것을 전제로 만들어졌다.
	void MakingSpriteSheet(
		ID2D1Bitmap* bitmap,		// 애니메이션을 만들 스프라이트가 담긴 비트맵 이미지
		int rowCount,				// 스프라이트 시트에서 애니메이션 행의 개수
		int columnCount,			// 스프라이트 시트에서 애니메이션 열의 개수
		int verticalPos,			// 세로 몇 번째 줄의 스프라이트들을 애니메이션으로 쓸지
		Character* character,	// 어떤 오브젝트에서 사용할지
		const char* motionName		// 애니메이션의 이름은 무엇인지
	);

	///--------------------------------------------------------------------------
	/// 애니메이션 스프라이트 로드
public:
	CA_Animation* LoadSpriteSheetByPath(PCWSTR path, int spriteCount, float frameRate, bool isLoop);


	///--------------------------------------------------------------------------
	/// 이미지 그려주기 관련
public:
	// 넣어준 비트맵을 그려준다.
	void DrawUIImage(ID2D1Bitmap* pBitmap);
	void DrawGOImage(ID2D1Bitmap* pBitmap);

	void ImageDrawByPoint(ID2D1Bitmap* pBitmap, D2D1_POINT_2F pos);
	void ImageDrawByPoint(ID2D1Bitmap* pBitmap, D2D1_POINT_2F pos, float opcity);
	void ImageDrawSmoke(ID2D1Bitmap* pBitmap, D2D1_POINT_2F pos, D2D1_SIZE_F size, float opcity);
	void ImageDrawSmokeFromSheet(ID2D1Bitmap* pBitmap, D2D1_RECT_F source, D2D1_POINT_2F pos, float size, float opcity);


	void ImageDrawByScale(ID2D1Bitmap* pBitmap, D2D1_POINT_2F pos, D2D1_SIZE_F);

	void DrawSprite(CASprite* sprite, std::string motionName, int dstX, int dstY);

	// 방향에 따라 이미지를 바꿔주는 함수
	void DrawSpriteByRotation(ID2D1Bitmap* carBitmap, const float angle, const int spriteCount);
	void DrawSpriteByRotation(vector<ID2D1Bitmap*> bitmap, D2D1_MATRIX_3X2_F projTM, const float angle, const int spriteCount, bool isIsometric);
	void DrawShadowByRotation(vector<ID2D1Bitmap*> bitmap, D2D1_MATRIX_3X2_F projTM, const float angle, const int spriteCount, bool isIsometric);


	///--------------------------------------------------------------------------
	/// 애니메이션 그려주기 관련
public:
	void DrawSpriteAnimation(CA_Animation* animation, float dTime);
	void DrawSpriteAnimation(CA_Animation* animation, D2D1_VECTOR_2F pos, float dTime);



	///--------------------------------------------------------------------------
	/// 이펙트 관련 함수
private:
	// 멤버 변수
	// 각 멤버 변수마다 어떤 이펙트를 넣을 것인지 생각하면서 명명한다.
	ID2D1Effect* m_pGaussianBlurEffect;
	ID2D1Effect* m_pBlendEffect;
	ID2D1Effect* m_pOpacity;
	ID2D1Effect* m_pShadow;
	ID2D1Effect* m_pPerspectiveTransform;

	// Shadow 이펙트를 쓰기 위한 멤버변수
	ID2D1Effect* m_pFloodEffect;				// 배경을 덮어버리는 효과
	ID2D1Effect* m_pAffineTransformEffect;		// 이미지의 위치를 변환
	ID2D1Effect* m_pCompositeEffect;			// 두 개의 이미지를 함께 출력시켜주는 효과

	// 위 변수들에 이펙트를 생성해 넣는다.
	void CreateEffects();

public:
	// 게임 오브젝트를 가져와 각각의 비트맵에 이펙트를 넣어준다.
	void ApplyGaussianBlur(CGameObject* obj);									// 블러 이펙트
	void ApplyOpacity(CGameObject* obj, float val);								// 투명도 이펙트
	void ApplyBlend(CGameObject* obj, ID2D1Bitmap* bitmap2);					// 블렌드 이펙트
	void ApplyShadow(CGameObject* obj, D2D1_POINT_2F point);					// 그림자 이펙트

	ID2D1Effect* ApplyShadowByBitmap(ID2D1Bitmap* bitmap);					// 그림자 이펙트
	ID2D1Effect* ApplyShadowByBitmap(ID2D1Effect* effect);					// 그림자 이펙트


	// 맵을 Perspective하게 보여줄 수도 있지만, 
	// 한번에 모두 뒤집어야 하므로 독립적으로 사용해선 의미가 없다.
	void ApplyPerspectiveTransform(ID2D1Bitmap* bitmap, D2D1_VECTOR_3F vec);	// Perspective 이펙트(3D처럼 보임)

	// 이펙트를 되돌려 받는다. 
	// 바로 그려버릴 게 아니라, 이펙트를 받아서 다른 이펙트를 적용해야하기 때문
	// 가장 먼저 아핀트랜스폼을 하는 이유는 어찌됐든 이펙트가 어디에 출력되어야할지는 알아야하기 때문.
	ID2D1Effect* AddAffinTransform(ID2D1Bitmap* bitmap, D2D1_MATRIX_3X2_F matrix);
	ID2D1Effect* AddAffinTransform(ID2D1Effect* effect, D2D1_MATRIX_3X2_F matrix);
	ID2D1Effect* AddOpacity(ID2D1Effect* effect, float val);

	// 이펙트가 붙은 이미지 출력함수
	void DrawEffectBitmap(ID2D1Effect* effect);

	///--------------------------------------------------------------------------
	/// 자체 이펙트 관련 함수

	// 트레일 그리기 함수
	void DrawLineForTrail(D2D1_POINT_2F pos1, D2D1_POINT_2F pos2);
	void DrawLineForTrail(D2D1_POINT_2F pos1, D2D1_POINT_2F pos2, float opacity);


	// 화면 비틀기 함수 (2D로 연산이 끝난 상태에서 렌더링을 Isometric으로 변환 시켜준다.)
	//void MakeIsometric(D2D1_VECTOR_2F center, float size);
	D2D1_MATRIX_3X2_F MakeIsometric(D2D1_VECTOR_2F center, float size, float rot);

	///--------------------------------------------------------------------------
	/// SetTransform 관련 함수들
	void SetTransform(D2D1_MATRIX_3X2_F transformMatrix);

	void RotateDraw(ICollisionObject* pObject);

	void TransformRotation(float angle, int x, int y);
	void TransformEnd();

	///--------------------------------------------------------------------------
	/// 현재 브러쉬 컬러값 조정
	// 브러쉬 컬러값 조정 함수
	void SetBrushColor(float r, float g, float b, float a);
	void SetBrushColor(D2D1::ColorF color);

	///--------------------------------------------------------------------------
	/// D2 도형 그리기 함수들
	// 사각형 stroke 그리기 함수
	void DrawRectangle_PreSetCol (int x, int y, int x1, int y1, eBRUSHCOLOR eBrushCol);
	void DrawRectangle_PickCol (int x, int y, int x1, int y1, D2D1::ColorF color);
	void DrawRectangle_NowCol (int x, int y, int x1, int y1);
	void DrawRectangle_Debug (int x, int y, int x1, int y1, bool isCollide);

	// 사각형 fill 그리기 함수
	void DrawFillRect_NowCol (D2D1_RECT_F rect);
	void DrawFillRect_PickCol (D2D1_RECT_F _rect, D2D1::ColorF color);
	void DrawFillAlphaRect(D2D1_RECT_F _rect, float alpha);

	// 원형 그리기 함수
	void DrawEllipse_PreSetCol (float x, float y, float radius, eBRUSHCOLOR eBrushCol);
	void DrawEllipse_PickCol (float x, float y, float radius, D2D1::ColorF color);
	void DrawEllipse_NowCol (float x, float y, float radius);
	void DrawEllipse_Debug (float x, float y, float radius, bool isCollide);

	// 선 그리기 함수
	void DrawLine_PreSetCol	(float x1, float y1, float x2, float y2, eBRUSHCOLOR eBrushCol);
	void DrawLine_PickCol (float x1, float y1, float x2, float y2, D2D1::ColorF color);
	void DrawLine_NowCol (float x1, float y1, float x2, float y2);

	void DrawLineByVector(D2D1_VECTOR_2F origin, D2D1_VECTOR_2F target, D2D1::ColorF color);

	// 텍스트 관련 함수
	bool	DrawText(int x, int y, const WCHAR* pch, ...);
	bool	DrawText(D2D1_POINT_2F pos, const WCHAR* pch, ...);
	bool	DrawText(D2D1_POINT_2F pos, const WCHAR* pch, D2D1::ColorF color...);

};

