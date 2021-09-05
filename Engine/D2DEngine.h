#pragma once

#include "GlobalDefine.h"

using namespace std;

class CASprite;
class CGameObject;
class Character;
class ICollisionObject;

// ���������� �߰��Ϸ��� ������ �ִٸ�, enum���� �ش� �׸��� �߰�����.
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
	/// ��� ����
private:
	HWND m_hWnd;

	// ���丮
	//ID2D1Factory* m_pFactory;				// �Լ����� ����
	ID2D1Factory1* m_pD2D1Factory1;				// ���� ���丮�� ��ӹ��� D2D1_1 ���� ���丮
	IWICImagingFactory* m_pWICFactory;

	// �ؽ�Ʈ ����
	IDWriteFactory* m_pWriteFactory;
	IDWriteTextFormat* m_pTextFormat;

	// �̹��� ����
	IWICBitmapDecoder*		m_pWICDecoder;
	IWICFormatConverter*	m_pWICConverter;
	IWICBitmapFrameDecode*	m_pFrame;

	// ���� Ÿ��
	//ID2D1HwndRenderTarget*	m_pRenderTarget;		// �׸� ���
	ID2D1DeviceContext* m_pD21DDeviceContext;			// ���� Ÿ�� ��ſ� ����� DC
	D3D_FEATURE_LEVEL	m_featureLevel;				// enumeration, ���̷�Ʈ�� ����� ������ ����� ������
	ID2D1Device* m_pD2D1Device1;					// ���� �� �𸣰ڴ�

	IDXGISwapChain1* m_pSwapChain;					
	// ���� ü���� �츮�� �˰� �ִ� ���� ���۸��� �ǹ��Ѵ�.
	// ���� ���۸��� ����� �ĸ��� ����(��ü)���ָ鼭 ��ũ���� �����ִ� �����
	// ��ġ �罽�� ���� ������� ����ȴٰ� �ؼ� �ٿ��� �̸��̴�.
	// �̿� ���� SwapChain�� GetBuffer�� Present�� ���� ���۸� �������� �����ִ� ����� ���� �ִ�.

	ID2D1Bitmap1* m_pD2DTargetBitmap;

	float m_dpi;

	ID2D1Bitmap* m_pBitmap;							// �׽�Ʈ�� ��Ʈ��

	// �귯��
	ID2D1SolidColorBrush*	m_pBlackBrush;			// ���� �귯��
	ID2D1SolidColorBrush*	m_pWhiteBrush;			// �Ͼ� �귯��
	ID2D1SolidColorBrush*	m_pRedBrush;			// R �귯��
	ID2D1SolidColorBrush*	m_pGreenBrush;			// G �귯��
	ID2D1SolidColorBrush*	m_pBlueBrush;			// B �귯��
	ID2D1SolidColorBrush*	m_pTempBrush;			// Temp �귯��
	ID2D1SolidColorBrush*	m_pNowBrush;			// Now �귯��
	ID2D1SolidColorBrush*	arr_MyBrush[MAX_COLOR];

	ID2D1SolidColorBrush* m_pSkidMarkBrush;			// ��Ű�帶ũ �귯��

	// ���� �Ǵ�
	HRESULT m_IsReady;		// �������� �������� ���� �����Ѵ�

	D2D1_SIZE_F m_RenderTargetSize;

public:
	///--------------------------------------------------------------------------
	/// �ִϸ��̼� ���� ����� m_Player�� Ȱ��
	std::vector<CASprite*> v_Sprite;

	// ��������Ʈ ����
	int m_SpriteCount;			// ��������Ʈ�� ���� (�ε����� 0����)
	int m_SpriteSheetCount;		// ��������Ʈ ��Ʈ�� ���� (�ε����� 0����)
	std::map<std::string, ID2D1Bitmap*> m_SpriteSheets;	// ��������Ʈ ��Ʈ

public:
	///--------------------------------------------------------------------------
	/// �̱��� ����

	// ���� �����͸� ��ȯ���ִ� �Լ��� �����.
	static D2DEngine* Instance();

protected:
	// �⺻ ������
	// �̱����� ����ϰ� �Ǹ�, �ϳ��� �ν��Ͻ����� �����ؾ��ϹǷ� �⺻ �����ڸ� public���� �� �� ����.
	D2DEngine();

public:
	~D2DEngine();

private:
	// �̱����� ���� ���� ������
	static D2DEngine* _instance;


public:
	///--------------------------------------------------------------------------
	/// �ʱ�ȭ ����

	// �ʱ�ȭ
	// D2D �� D3D, �ؽ�Ʈ ���� ���丮�� �����Ѵ�.
	void Initialize(HWND hWnd);
	
private:
	// ����̽� ���ؽ�Ʈ, ����ü��, �⺻ �귯�� ���� �����ϴ� �Լ�.
	// Initialize �Լ��� ���� ȣ��ȴ�.
	HRESULT CreateDeviceResources();

public:
	void	BeginRender();				// ������ ���� �� �ʱ�ȭ �Լ�
	void	EndRender();				// EndDraw�� ���� ü���� ȣ���Ѵ�.
										// ���� �߻� �� DiscardDeviceResources�� ȣ���Ѵ�.
	void    DiscardDeviceResources();	// Device Context�� �����ϴ� �Լ�
										// ������ ���� ������ Device Context�� BeginRender���� �ٽ� �����ȴ�.				
	void	DiscardFactory();			// ���� ���� �� ȣ��ȴ�.	
										// ��κ��� �������� �ڿ����� ���⼭ �����ȴ�.
	///--------------------------------------------------------------------------
	/// �̹��� ���� ����
public:

	// ���� ��θ� ���� �̹����� �ҷ����� �Լ�
	// ���� ���� HRESULT�̹Ƿ� �ܼ��� ����� ���� ����, �����ؼ� ����ϴ� ���� ����.
	HRESULT LoadBitmapFromFile(
		ID2D1RenderTarget* pRenderTarget,	// ���� Ÿ��
		IWICImagingFactory* pIWICFactory,	// Initialize���� ������ ���丮
		PCWSTR path,						// ���ϸ� (�̹����� ���)
		UINT destinationWidth,				// �̹����� ���� ũ��
		UINT destinationHeight,				// �̹����� ���� ũ��	
		ID2D1Bitmap** ppBitmap);			// �̹����� �־��� ��Ʈ�� �������̽�

	// ID2D1Bitmap�� ��� ��ȯ�ؼ� �� �� �ְ� �����ߴ�.
	// LoadBitmapFromFile���� �ٸ��� ���� ���� ���� �������� ��� �����ϴ�.
	ID2D1Bitmap* LoadBitmapByPath(PCWSTR path);

	// ���� �̸��� ���� �ε��� �̹����� string�� pair�ؼ� STL Map�� �ִ´�.
	// ���߿� �ش��ϴ� string���� �ٽ� �ҷ��� �� �ִ�.
	int LoadSpriteSheet(const wchar_t* filePath, std::string str);

	// ���� �� �ڵ�� �׽�Ʈ�뵵�� ������� �ڵ�.
	// ��������Ʈ ��Ʈ�� �� ���ִٴ� ���� ������ ���������.
	void MakingSpriteSheet(
		ID2D1Bitmap* bitmap,		// �ִϸ��̼��� ���� ��������Ʈ�� ��� ��Ʈ�� �̹���
		int rowCount,				// ��������Ʈ ��Ʈ���� �ִϸ��̼� ���� ����
		int columnCount,			// ��������Ʈ ��Ʈ���� �ִϸ��̼� ���� ����
		int verticalPos,			// ���� �� ��° ���� ��������Ʈ���� �ִϸ��̼����� ����
		Character* character,	// � ������Ʈ���� �������
		const char* motionName		// �ִϸ��̼��� �̸��� ��������
	);

	///--------------------------------------------------------------------------
	/// �ִϸ��̼� ��������Ʈ �ε�
public:
	CA_Animation* LoadSpriteSheetByPath(PCWSTR path, int spriteCount, float frameRate, bool isLoop);


	///--------------------------------------------------------------------------
	/// �̹��� �׷��ֱ� ����
public:
	// �־��� ��Ʈ���� �׷��ش�.
	void DrawUIImage(ID2D1Bitmap* pBitmap);
	void DrawGOImage(ID2D1Bitmap* pBitmap);

	void ImageDrawByPoint(ID2D1Bitmap* pBitmap, D2D1_POINT_2F pos);
	void ImageDrawByPoint(ID2D1Bitmap* pBitmap, D2D1_POINT_2F pos, float opcity);
	void ImageDrawSmoke(ID2D1Bitmap* pBitmap, D2D1_POINT_2F pos, D2D1_SIZE_F size, float opcity);
	void ImageDrawSmokeFromSheet(ID2D1Bitmap* pBitmap, D2D1_RECT_F source, D2D1_POINT_2F pos, float size, float opcity);


	void ImageDrawByScale(ID2D1Bitmap* pBitmap, D2D1_POINT_2F pos, D2D1_SIZE_F);

	void DrawSprite(CASprite* sprite, std::string motionName, int dstX, int dstY);

	// ���⿡ ���� �̹����� �ٲ��ִ� �Լ�
	void DrawSpriteByRotation(ID2D1Bitmap* carBitmap, const float angle, const int spriteCount);
	void DrawSpriteByRotation(vector<ID2D1Bitmap*> bitmap, D2D1_MATRIX_3X2_F projTM, const float angle, const int spriteCount, bool isIsometric);
	void DrawShadowByRotation(vector<ID2D1Bitmap*> bitmap, D2D1_MATRIX_3X2_F projTM, const float angle, const int spriteCount, bool isIsometric);


	///--------------------------------------------------------------------------
	/// �ִϸ��̼� �׷��ֱ� ����
public:
	void DrawSpriteAnimation(CA_Animation* animation, float dTime);
	void DrawSpriteAnimation(CA_Animation* animation, D2D1_VECTOR_2F pos, float dTime);



	///--------------------------------------------------------------------------
	/// ����Ʈ ���� �Լ�
private:
	// ��� ����
	// �� ��� �������� � ����Ʈ�� ���� ������ �����ϸ鼭 ����Ѵ�.
	ID2D1Effect* m_pGaussianBlurEffect;
	ID2D1Effect* m_pBlendEffect;
	ID2D1Effect* m_pOpacity;
	ID2D1Effect* m_pShadow;
	ID2D1Effect* m_pPerspectiveTransform;

	// Shadow ����Ʈ�� ���� ���� �������
	ID2D1Effect* m_pFloodEffect;				// ����� ��������� ȿ��
	ID2D1Effect* m_pAffineTransformEffect;		// �̹����� ��ġ�� ��ȯ
	ID2D1Effect* m_pCompositeEffect;			// �� ���� �̹����� �Բ� ��½����ִ� ȿ��

	// �� �����鿡 ����Ʈ�� ������ �ִ´�.
	void CreateEffects();

public:
	// ���� ������Ʈ�� ������ ������ ��Ʈ�ʿ� ����Ʈ�� �־��ش�.
	void ApplyGaussianBlur(CGameObject* obj);									// �� ����Ʈ
	void ApplyOpacity(CGameObject* obj, float val);								// ���� ����Ʈ
	void ApplyBlend(CGameObject* obj, ID2D1Bitmap* bitmap2);					// ���� ����Ʈ
	void ApplyShadow(CGameObject* obj, D2D1_POINT_2F point);					// �׸��� ����Ʈ

	ID2D1Effect* ApplyShadowByBitmap(ID2D1Bitmap* bitmap);					// �׸��� ����Ʈ
	ID2D1Effect* ApplyShadowByBitmap(ID2D1Effect* effect);					// �׸��� ����Ʈ


	// ���� Perspective�ϰ� ������ ���� ������, 
	// �ѹ��� ��� ������� �ϹǷ� ���������� ����ؼ� �ǹ̰� ����.
	void ApplyPerspectiveTransform(ID2D1Bitmap* bitmap, D2D1_VECTOR_3F vec);	// Perspective ����Ʈ(3Dó�� ����)

	// ����Ʈ�� �ǵ��� �޴´�. 
	// �ٷ� �׷����� �� �ƴ϶�, ����Ʈ�� �޾Ƽ� �ٸ� ����Ʈ�� �����ؾ��ϱ� ����
	// ���� ���� ����Ʈ�������� �ϴ� ������ ����Ƶ� ����Ʈ�� ��� ��µǾ�������� �˾ƾ��ϱ� ����.
	ID2D1Effect* AddAffinTransform(ID2D1Bitmap* bitmap, D2D1_MATRIX_3X2_F matrix);
	ID2D1Effect* AddAffinTransform(ID2D1Effect* effect, D2D1_MATRIX_3X2_F matrix);
	ID2D1Effect* AddOpacity(ID2D1Effect* effect, float val);

	// ����Ʈ�� ���� �̹��� ����Լ�
	void DrawEffectBitmap(ID2D1Effect* effect);

	///--------------------------------------------------------------------------
	/// ��ü ����Ʈ ���� �Լ�

	// Ʈ���� �׸��� �Լ�
	void DrawLineForTrail(D2D1_POINT_2F pos1, D2D1_POINT_2F pos2);
	void DrawLineForTrail(D2D1_POINT_2F pos1, D2D1_POINT_2F pos2, float opacity);


	// ȭ�� ��Ʋ�� �Լ� (2D�� ������ ���� ���¿��� �������� Isometric���� ��ȯ �����ش�.)
	//void MakeIsometric(D2D1_VECTOR_2F center, float size);
	D2D1_MATRIX_3X2_F MakeIsometric(D2D1_VECTOR_2F center, float size, float rot);

	///--------------------------------------------------------------------------
	/// SetTransform ���� �Լ���
	void SetTransform(D2D1_MATRIX_3X2_F transformMatrix);

	void RotateDraw(ICollisionObject* pObject);

	void TransformRotation(float angle, int x, int y);
	void TransformEnd();

	///--------------------------------------------------------------------------
	/// ���� �귯�� �÷��� ����
	// �귯�� �÷��� ���� �Լ�
	void SetBrushColor(float r, float g, float b, float a);
	void SetBrushColor(D2D1::ColorF color);

	///--------------------------------------------------------------------------
	/// D2 ���� �׸��� �Լ���
	// �簢�� stroke �׸��� �Լ�
	void DrawRectangle_PreSetCol (int x, int y, int x1, int y1, eBRUSHCOLOR eBrushCol);
	void DrawRectangle_PickCol (int x, int y, int x1, int y1, D2D1::ColorF color);
	void DrawRectangle_NowCol (int x, int y, int x1, int y1);
	void DrawRectangle_Debug (int x, int y, int x1, int y1, bool isCollide);

	// �簢�� fill �׸��� �Լ�
	void DrawFillRect_NowCol (D2D1_RECT_F rect);
	void DrawFillRect_PickCol (D2D1_RECT_F _rect, D2D1::ColorF color);
	void DrawFillAlphaRect(D2D1_RECT_F _rect, float alpha);

	// ���� �׸��� �Լ�
	void DrawEllipse_PreSetCol (float x, float y, float radius, eBRUSHCOLOR eBrushCol);
	void DrawEllipse_PickCol (float x, float y, float radius, D2D1::ColorF color);
	void DrawEllipse_NowCol (float x, float y, float radius);
	void DrawEllipse_Debug (float x, float y, float radius, bool isCollide);

	// �� �׸��� �Լ�
	void DrawLine_PreSetCol	(float x1, float y1, float x2, float y2, eBRUSHCOLOR eBrushCol);
	void DrawLine_PickCol (float x1, float y1, float x2, float y2, D2D1::ColorF color);
	void DrawLine_NowCol (float x1, float y1, float x2, float y2);

	void DrawLineByVector(D2D1_VECTOR_2F origin, D2D1_VECTOR_2F target, D2D1::ColorF color);

	// �ؽ�Ʈ ���� �Լ�
	bool	DrawText(int x, int y, const WCHAR* pch, ...);
	bool	DrawText(D2D1_POINT_2F pos, const WCHAR* pch, ...);
	bool	DrawText(D2D1_POINT_2F pos, const WCHAR* pch, D2D1::ColorF color...);

};

