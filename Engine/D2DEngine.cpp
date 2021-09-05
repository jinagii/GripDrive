#include "D2DEngine.h"

#include "Character.h"
#include "CAABB.h"
#include "CTransform.h"
#include "Smoke.h"

// 생성된 자원들을 Release해 주는 함수
// pFactory 같은 자원은 게임 종료 시 돌려준다.
template <class T> void SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = nullptr;
	}
}

// 멤버 변수들을 초기화해주는 함수
D2DEngine::D2DEngine() :
	m_hWnd(nullptr),

	// 팩토리 초기화
	m_pD2D1Factory1(nullptr),
	m_pWICFactory(nullptr),
	m_pWriteFactory(nullptr),

	// 이미지 관련 초기화
	m_pWICDecoder(nullptr),
	m_pWICConverter(nullptr),
	m_pFrame(nullptr),

	// 렌더 타겟 초기화
	//m_pRenderTarget(nullptr),
	m_pD21DDeviceContext(nullptr),

	// 테스트용 이미지 초기화
	m_pBitmap(nullptr),

	// 브러쉬들 초기화
	m_pBlackBrush(nullptr), m_pWhiteBrush(nullptr),
	m_pRedBrush(nullptr), m_pGreenBrush(nullptr), m_pBlueBrush(nullptr),
	m_pTempBrush(nullptr), m_pNowBrush(nullptr), m_pSkidMarkBrush(nullptr),
	// 텍스트 관련 멤버 초기화
	m_pTextFormat(nullptr),
	// 오류 판단 멤버 초기화
	m_IsReady(S_FALSE),
	// 렌더링 타겟 사이즈 0으로 초기화
	m_RenderTargetSize(D2D1_SIZE_F())
{

}

D2DEngine::~D2DEngine()
{

}

D2DEngine* D2DEngine::_instance = 0;

D2DEngine* D2DEngine::Instance()
{
	if (_instance == 0)
	{
		_instance = new D2DEngine;
	}

	return _instance;
}

void D2DEngine::Initialize(HWND hWnd)
{
	// 현재 스레드에서 COM 라이브러리를 초기화해주는 함수다.
	// 이미지를 로드할 WIC를 활용하기 위해서 거쳐야하는 단계이다.
	// 단, COM 라이브러리가 정확히 어떤 일을 하는지는 조금 더 알아봐야한다.
	HRESULT _hResult = CoInitialize(NULL);


	// 앞으로 함수 사용중에 필요할 어플리케이션(윈도우) 핸들을 가져와 멤버변수로 관리한다.
	// 필요할 때마다 파라미터로 가져오기 귀찮기 때문.
	m_hWnd = hWnd;

	// hr을 먼저 S_OK로 초기화해준다.
	HRESULT hr = S_OK;

	// 함수의 집합체인 팩토리를 만든다.
	// Demo에서도 만들 수 있지만, 엔진에서 만드는 게 낫다.
	// 이유는 Demo에도 Direct2D 관련 헤더를 넣고, lib까지 추가해야 하기 때문이다.
	/// D2D1_1 버전의 팩토리를 초기화한다.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2D1Factory1);

	// 이미지 로드에 쓰이는 WIC를 활용하기 위한 초기화 단계를 실행한다.
	// CoCreateInstance는 그 첫 번째로 m_pWICFactory를 만드는 기능을 수행한다.
	// CLSID란, 클래스 ID라는 의미인데, Windows의 모든 응용 프로그램 구성 요소의 고유 ID를 나태난다.
	if (SUCCEEDED(hr))
	{
		// Create WIC factory.
		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory,
			reinterpret_cast<void**>(&m_pWICFactory)
		);
	}

	// 아래에서는 DirectWrite를 위한 초기화를 진행한다.
	// DirectWrite는 텍스트를 보다 다양하게 표현할 수 있도록 도와주는 DirectX API다.
	// 그림을 그릴 때와 마찬가지로 텍스트 출력에 필요한 Factory가 따로 존재하며, 이를 먼저 선언해줘야 한다.
	// 물론, 텍스트도 결국 렌더링이 필요하므로 D2DFacrtory와 RenderTarget을 필요로 한다.
	// 참고로 관련 함수와 클래스들은 dwrite.h에 포함되어있다.
	// MSDN : https://docs.microsoft.com/en-us/windows/win32/directwrite/direct-write-portal

	// 어떤 폰트를 사용할 것인지, 얼마나 크게 사용할 것인지를 정한다.
	static const WCHAR msc_fontName[] = L"굴림";			// 폰트
	static const float msc_fontSize = 12;				// 크기

	// D2DFactory가 만들어졌다면, D2D와 같은 느낌으로 Write Factory를 만든다.
	if (SUCCEEDED(hr))
	{

		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,							// 팩토리 타입 설정
			__uuidof(m_pWriteFactory),							// 팩토리 인터페이스를 식별하는 값
			reinterpret_cast<IUnknown**>(&m_pWriteFactory)		// 팩토리 개체에 대한 포인터 주소
		);
	}

	if (SUCCEEDED(hr))
	{
		// DirectWrite를 통해 출력한 텍스트의 포멧(스타일 등)을 결정한다.
		hr = m_pWriteFactory->CreateTextFormat(
			msc_fontName,							// 폰트 
			NULL,									// 폰트 컬렉션, NULL로 두면 system font 컬렉션으로 설정된다.
			DWRITE_FONT_WEIGHT_NORMAL,				// 텍스트의 두께
			DWRITE_FONT_STYLE_NORMAL,				// 텍스트의 글꼴 스타일
			DWRITE_FONT_STRETCH_NORMAL,				// 텍스트의 늘임 정도
			msc_fontSize,							// 폰트 사이즈 (장치 독립 픽셀)
			L"",									// 로케일 이름 L"en-us"로 표현 가능, 공백일 경우 자동으로 지정되는듯
			&m_pTextFormat							// 포멧 포인터에 정보들을 담는다.
		);
	}

	// 텍스트 정렬 방식
	if (SUCCEEDED(hr))
	{
		// Left-Top the text horizontally and vertically.
		// 텍스트의 레이아웃 상자를 기준으로 앞쪽 가장자리와 가장 상단 가장자리에 정렬되도록함
		m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	}

	m_IsReady = CreateDeviceResources();
}

HRESULT D2DEngine::CreateDeviceResources()
{
	// 렌더 타겟 역할을 할 DeviceContext를 만든다.
	// 이전 버전의 엔진에서는 Initialize에서 사전 렌더 타겟을 생성했는데,
	// 이번 버전에서는 해당 부분이 구조적으로 구현되었다.
	// BeginRender를 할때마다 구현해주지 않아도 된다.
	// 단, BeginRender에도 이 함수가 들어가 있긴 하지만,
	// m_pD2DDeviceContext의 상태를 보기 때문에 괜찮다.
	HRESULT hr = S_OK;

	if (m_pD21DDeviceContext == nullptr)
	{

		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;


		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		};

		ID3D11Device* _D3D11Device;
		ID3D11DeviceContext* _D3D11DeviceContext;

		/// 잘 모르겠다 아래는
		D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			0,
			creationFlags,
			featureLevels,
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION,
			&_D3D11Device,
			&m_featureLevel,
			&_D3D11DeviceContext
		);

		IDXGIDevice* _dexgiDevice;

		_D3D11Device->QueryInterface(&_dexgiDevice);

		m_pD2D1Factory1->CreateDevice(_dexgiDevice, &m_pD2D1Device1);

		hr = m_pD2D1Device1->CreateDeviceContext(
			D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
			&m_pD21DDeviceContext);

		if (SUCCEEDED(hr))
		{
			hr = m_pD21DDeviceContext->CreateSolidColorBrush(D2D1::ColorF((D2D1::ColorF::Black)), &m_pBlackBrush);
			hr = m_pD21DDeviceContext->CreateSolidColorBrush(D2D1::ColorF((D2D1::ColorF::White)), &m_pWhiteBrush);
			hr = m_pD21DDeviceContext->CreateSolidColorBrush(D2D1::ColorF((D2D1::ColorF::Red)), &m_pRedBrush);
			hr = m_pD21DDeviceContext->CreateSolidColorBrush(D2D1::ColorF((D2D1::ColorF::LightGreen)), &m_pGreenBrush);
			hr = m_pD21DDeviceContext->CreateSolidColorBrush(D2D1::ColorF((D2D1::ColorF::Blue)), &m_pBlueBrush);

			hr = m_pD21DDeviceContext->CreateSolidColorBrush(D2D1::ColorF((D2D1::ColorF::Black)), &m_pSkidMarkBrush);

		}

		arr_MyBrush[0] = m_pBlackBrush;
		arr_MyBrush[1] = m_pWhiteBrush;
		arr_MyBrush[2] = m_pRedBrush;
		arr_MyBrush[3] = m_pGreenBrush;
		arr_MyBrush[4] = m_pBlueBrush;


		DXGI_SWAP_CHAIN_DESC1 _swapChainDesc = { 0 };
		_swapChainDesc.Width = 0;
		_swapChainDesc.Height = 0;
		_swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		_swapChainDesc.Stereo = false;
		_swapChainDesc.SampleDesc.Count = 1;
		_swapChainDesc.SampleDesc.Quality = 0;
		_swapChainDesc.BufferUsage =
			DXGI_USAGE_RENDER_TARGET_OUTPUT;
		_swapChainDesc.BufferCount = 2;
		_swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		_swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		_swapChainDesc.Flags = 0;


		IDXGIAdapter* _dxgiAdapter;
		_dexgiDevice->GetAdapter(&_dxgiAdapter);

		IDXGIFactory2* _dxgiFactory;
		_dxgiAdapter->GetParent(IID_PPV_ARGS(&_dxgiFactory));

		DXGI_RATIONAL rate;
		rate.Denominator = 60;
		rate.Numerator = 1;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC _swapChainDescFull;
		_swapChainDescFull.RefreshRate = rate;
		_swapChainDescFull.Scaling = DXGI_MODE_SCALING_STRETCHED;
		_swapChainDescFull.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST;
		_swapChainDescFull.Windowed = true;

		_dxgiFactory->CreateSwapChainForHwnd(_D3D11Device,
			m_hWnd,
			&_swapChainDesc,
			&_swapChainDescFull, // 전체화면 스왑체인 설정
			nullptr,
			&m_pSwapChain);

		ID3D11Texture2D* _backBuffer;
		m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&_backBuffer));


		D2D1_BITMAP_PROPERTIES1 _bitmapProperties =
			BitmapProperties1(
				D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
				PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
				m_dpi,
				m_dpi
			);

		IDXGISurface* _dxgiBackBuffer;
		m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&_dxgiBackBuffer));


		m_pD21DDeviceContext->CreateBitmapFromDxgiSurface(
			_dxgiBackBuffer,
			&_bitmapProperties,
			&m_pD2DTargetBitmap
		);

		m_pD21DDeviceContext->SetTarget(m_pD2DTargetBitmap);

		// 이펙트 생성을 여기서 함께 해준다.
		CreateEffects();

	}
	return hr;
}

HRESULT D2DEngine::LoadBitmapFromFile(
	ID2D1RenderTarget* pRenderTarget,
	IWICImagingFactory* pIWICFactory,
	PCWSTR path,
	UINT destinationWidth,
	UINT destinationHeight,
	ID2D1Bitmap** ppBitmap)
{
	// 방어적으로 코드를 짜기 위해 매번 hr을 return값으로 받겠다.
	// 없어도 프로그램은 돌아간다.
	HRESULT hr = S_OK;

	// IWIC라는 접두어에서 알 수 있듯, 이들은 모두 Interface를 의미한다.
	// Interface란, 쉽게 생각해서 함수의 모음을 의미한다.
	IWICBitmapDecoder* pDecoder = nullptr;		// 압축된 이미지 포멧을 디코딩해주는 인터페이스다.
	IWICBitmapFrameDecode* pSource = nullptr;	// 디코딩된 데이터를 넘겨줄 인터페이스다.
	IWICFormatConverter* pConverter = nullptr;	// 디코딩된 데이터를 비트맵으로 바꿔주는 인터페이스다.
	IWICBitmapScaler* pScaler = nullptr;		// 비트맵의 스케일러 값을 초기화해주는 인터페이스다.


	// 이제부터 길고도 긴 비트맵 로딩이 시작된다.
	// 1. 가장 먼저 하게될 순서는 WICFactory를 통해 이미지를 로딩하는 일이다.
	// 1-2. 당연히 Factory 인터페이스가 필요하며, PCWSTR 형식의 경로를 함수의 매개변수로 넘겨준다.
	// 1-3. 이후 읽기 옵션과 파일 처리를 위한 옵션을 준다.
	// 1-4. 이제 해당 파일에 접근 가능한 Decoder 객체가 생성된다.
	// 1-5. Decoder는 PNG를 디코딩해주는 역할을 한다.
	hr = pIWICFactory->CreateDecoderFromFilename(
		path,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);

	// 2. 디코더로부터 몇 번째 프레임의 이미지를 가져갈지 결정한다.
	// 2-1. 일반적인 이미지는 첫번째 프레임에 이미지가 저장된다. 따라서 0을 넣어 객체를 구성한다.
	// 2-2. 이때 객체는 pSorce 즉, FrameDecode가 된다.
	if (SUCCEEDED(hr))
	{
		hr = pDecoder->GetFrame(0, &pSource);
	}

	// 3. 이미지 형식을 비트맵 형식으로 변환해주는 과정을 진행한다.
	// 3-1. 이미지 형식을 변환해주는 인터페이스인 Converter를 생성한다.
	if (SUCCEEDED(hr))
	{
		hr = pIWICFactory->CreateFormatConverter(&pConverter);
	}

	// 4. 비트맵 형식으로 바뀐 데이터를 Bitmap객체에 넣어주기 전에 추가 작업을 진행한다.

	// 3-2. 비트맵 형식으로 바뀐 데이터를 Bitmap객체에 넣어주기 전에 추가 작업을 진행한다.
	if (SUCCEEDED(hr))
	{
		// 그릴 이미지에 가로/세로 사이즈를 특정하고 싶다면,
		// BitmapScaler를 통해 이미지를 재조정할 수 있다.
		// 아래는 이를 위한 if구문으로, 조정할 가로/세로 사이즈가 0이 아니라면,
		// 즉, 재조정이 필요하다면 진행하게 된다.
		if (destinationWidth != 0 || destinationHeight != 0)
		{
			// 일단, 우리가 목표로하는 가로/세로 사이즈와는 상관없이, 원래의 크기를 구해서 저장해둔다.
			UINT originalWidth, originalHeight;
			hr = pSource->GetSize(&originalWidth, &originalHeight);
			if (SUCCEEDED(hr))
			{
				if (destinationWidth == 0)
				{
					// 목표 크기 / 원래 크기로 비율을 구한다음, 이를 다시 원래 크기에 곱하는 방식을 취한다.
					float scalar = static_cast<float>(destinationHeight) / static_cast<float>(originalHeight);
					destinationWidth = static_cast<UINT>(scalar * static_cast<float>(originalWidth));
				}
				else if (destinationHeight == 0)
				{
					float scalar = static_cast<float>(destinationWidth) / static_cast<float>(originalWidth);
					destinationHeight = static_cast<UINT>(scalar * static_cast<float>(originalHeight));
				}

				// 크기가 정해졌다면, 이들을 매개변수로 pScaler의 함수에 전달한다.
				// 어떤 소스인지, 크기는 얼마인지, 크기 변화에 따른 픽셀 처리 알고리즘은 어떤 것을 사용할지 결정한다.
				hr = pIWICFactory->CreateBitmapScaler(&pScaler);

				if (SUCCEEDED(hr))
				{
					hr = pScaler->Initialize(
						pSource,
						destinationWidth,
						destinationHeight,
						WICBitmapInterpolationModeCubic
					);
				}

				// 이제 pScaler 인터페이스로부터 데이터를 가져와 pConverter를 초기화한다.
				// 컨버팅할 비트맵을 넣어주고, 사용할 픽셀 형식과 디더링 알고리즘을 설정한다.
				// 픽셀 형식은 잘 모르겠으나, 디더링은 나무위키 등에 잘 정리 되어있으니 확인해보자.
				// https://namu.wiki/w/%EB%94%94%EB%8D%94%EB%A7%81 >> 과거에는 점묘법 표현을 위한 알고리즘이었으나,
				// 현재에는 색상 표현의 한계를 극복하기 위해 사용된다고 한다.
				// 이외에 알파값이나 팔레트 등을 선택한다.
				if (SUCCEEDED(hr))
				{
					hr = pConverter->Initialize(
						pScaler,
						GUID_WICPixelFormat32bppPBGRA,
						WICBitmapDitherTypeNone,
						NULL,
						0.f,
						WICBitmapPaletteTypeMedianCut
					);
				}
			}
		}

		// 크기가 변경되지 않는 경우 위와 같은 과정을 거치지 않는다.
		// 원래 크기를 쓰려면 destinySize를 0을 떄리자.
		// 어차피 Transform으로도 크기 조절이 가능할텐데, 이렇게 하는 이유는 무엇일까?
		else
		{
			hr = pConverter->Initialize(
				pSource,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeMedianCut
			);
		}
	}

	// 마지막으로 우리가 직접 사용할 bitmap에 포인터에 비트맵을 넣어준다.
	// 컨버팅까지 끝났으므로, bitmap 인터페이스에서 드로우를 해주면 된다.
	if (SUCCEEDED(hr))
	{
		hr = pRenderTarget->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			ppBitmap
		);
	}

	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pConverter);
	SafeRelease(&pScaler);

	return hr;
}



void D2DEngine::BeginRender()
{
	m_IsReady = CreateDeviceResources();

	// 디바이스 종속적 자원들이 문제 없고, 랜더링을 할 수 있는 상황일 때
	// CheckWindowState는 현재 렌더 타겟과 관련된 HWND(이 윈도우)가 가려졌는지 판단하는 함수다.
	// D2D1_WINDOW_STATE enum의 요소 중 하나를 반환한다.
	// 여기서 D2D1_WINDOW_STATE_OCCLUDED는 가려졌음을 의미하므로, 그렇지 않아야 하기 때문에 !로 조건을 걸었다.
	if (SUCCEEDED(m_IsReady))
	{
		m_RenderTargetSize = m_pD21DDeviceContext->GetSize();

		// 그리기는 BegineDraw()와 EndDraw()사이에
		// BeginDraw와 EndDraw는 Direct2D 시스템에서 렌더링 대상을 사용하고 있음을 알려주는 용도다.
		m_pD21DDeviceContext->BeginDraw();

		// 행렬을 Identity로 하겠다는 의미는 기저벡터를 처음으로 돌려주겠다는 의미이다.
		// 바꿔말하면, 아무리 기저벡터를 옮기거나 돌려도 BeginRender에서 초기화하겠다는 것이다.
		m_pD21DDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());

		// 렌더 타겟의 그림 그리는 공간을 black으로 채우겠다.
		m_pD21DDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	}
}

void D2DEngine::EndRender()
{
	// m_IsReady는 그리기 시작과 함께 매번 값이 할당되므로, 여기서는 해주지 않아도 된다.
	// 이미 BeginRender를 지나왔을 것이므로
	// 마찬가지로 현재 윈도우가 가려져 있는지 판단해준다.
	if (SUCCEEDED(m_IsReady))
	{
		HRESULT hr = m_pD21DDeviceContext->EndDraw();

		DXGI_PRESENT_PARAMETERS parameters;
		parameters.DirtyRectsCount = 0;
		parameters.pDirtyRects = nullptr;
		parameters.pScrollOffset = nullptr;
		parameters.pScrollRect = nullptr;

		m_pSwapChain->Present1(1, 0, &parameters);

		// EndDraw에서는 그리기 도중 발생한 오류 등에 대해 판별해준다.
		// 이때 D2DERR_RECREATE_TARGET이 발생한 경우에는 렌더링 대상을 다시 만든다음 프레임 렌더링을 다시 시도한다.
		// 잘모르겟다.
		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = S_OK;
			DiscardDeviceResources();
		}
	}

}

void D2DEngine::DiscardDeviceResources()
{
	SafeRelease(&m_pD21DDeviceContext);

}

void D2DEngine::DiscardFactory()
{
	// 단, Factory의 경우는 게임이 종료되는 시점에서 자원을 돌려준다.
	SafeRelease(&m_pD21DDeviceContext);
	SafeRelease(&m_pD2D1Factory1);
	SafeRelease(&m_pTextFormat);

	SafeRelease(&m_pBlackBrush);
	SafeRelease(&m_pWhiteBrush);
	SafeRelease(&m_pRedBrush);
	SafeRelease(&m_pGreenBrush);
	SafeRelease(&m_pBlueBrush);

	SafeRelease(&m_pSwapChain);

	SafeRelease(&m_pGaussianBlurEffect);
	SafeRelease(&m_pOpacity);
	SafeRelease(&m_pBlendEffect);

	CoUninitialize();
}



ID2D1Bitmap* D2DEngine::LoadBitmapByPath(PCWSTR path)
{
	HRESULT hr;
	ID2D1Bitmap* pNewBitmap;

	// Create a bitmap by loading it from a file.
	hr = LoadBitmapFromFile(
		m_pD21DDeviceContext,
		m_pWICFactory,
		path,
		0,
		0,
		&pNewBitmap
	);

	return pNewBitmap;
}

int D2DEngine::LoadSpriteSheet(const wchar_t* filePath, std::string str)
{
	HRESULT hr;
	ID2D1Bitmap* pNewBitmap;

	// Create a bitmap by loading it from a file.
	hr = LoadBitmapFromFile(
		m_pD21DDeviceContext,
		m_pWICFactory,
		filePath,
		0,
		0,
		&pNewBitmap
	);

	if (SUCCEEDED(hr))
	{
		m_SpriteSheets.insert(std::pair<std::string, ID2D1Bitmap*>(str, pNewBitmap));
		m_SpriteSheetCount++;

		return (m_SpriteSheetCount - 1);
	}

	return -1;
}


void D2DEngine::DrawSprite(CASprite* sprite, std::string motionName, int dstX, int dstY)
{
	CASprite* _D2DSprite = dynamic_cast<CASprite*>(sprite);

	if (_D2DSprite != NULL)
	{
		ID2D1Bitmap* _sheet = m_SpriteSheets[motionName];
		// 인덱스 접근 시 실패체크도 해야 함
		if (_sheet != NULL)
		{
			D2D1_SIZE_F _size;

			_size = _D2DSprite->m_Size;

			m_pD21DDeviceContext->DrawBitmap(_sheet,
				D2D1::RectF((float)dstX, (float)dstY, (float)(dstX + _size.width), (float)(dstY + _size.height)),
				1.0f,
				D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
				D2D1::RectF(
					(float)_D2DSprite->m_SpritePos.left,
					(float)_D2DSprite->m_SpritePos.top,
					(float)(_D2DSprite->m_SpritePos.left + _size.width),
					(float)(_D2DSprite->m_SpritePos.top + _size.height)));
		}
	}
}

void D2DEngine::DrawSpriteByRotation(ID2D1Bitmap* carBitmap, const float angle, const int spriteCount)
{

	// 이 방식으로 처리할 경우 좌회전을 처리할 수 없다.
	int _angle = abs(angle);


	if (_angle >= 360)
	{
		_angle = _angle % 360;
	}

	int _width = carBitmap->GetPixelSize().width;
	int _height = carBitmap->GetPixelSize().height;

	// 개개의 스프라이트의 크기를 나타낸다.
	// 하나의 스프라이트 크기가 100픽셀이라면, _individual에는 100이 들어간다.
	int _individual = _width / spriteCount;

	int _measure = 360 / spriteCount;

	float _targetSize = 0.25f;
	// 0, 0 좌표에 스프라이트 그리기
	D2D1_RECT_F _rect;
	_rect.left = -(_individual * _targetSize);
	_rect.top = -(_height * _targetSize);
	_rect.right = _individual * _targetSize;
	_rect.bottom = _height * _targetSize;

	int _index = 0;

	// 양수일때
	if (angle >= 0)
	{
		_index = _angle / _measure;
	}
	else
	{
		_index = _angle / _measure;
		_index = (spriteCount - _index) - 1;
	}

	/// 잘라쓰는 방법을 모르겠따.
	//ID2D1Effect* _effected = ApplyShadowByBitmap(carBitmap);

	m_pD21DDeviceContext->DrawBitmap(
		carBitmap,
		//D2D1::RectF(-(_individual * 0.5f), -(_height * 0.5f), _individual, _height),
		_rect,
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1::RectF(
			_index * _individual,
			0,
			(_index * _individual) + _individual,
			_height));

}

void D2DEngine::DrawSpriteByRotation(vector<ID2D1Bitmap*> bitmap, D2D1_MATRIX_3X2_F projTM, const float angle, const int spriteCount, bool isIsometric)
{
	int _angle = abs(angle);

	if (isIsometric)
	{
		_angle = abs(angle + 45);
	}

	if (_angle >= 360)
	{
		_angle = _angle % 360;
	}

	int _measure = 360 / spriteCount;

	int _index = 0;

	if (angle >= 0)
	{
		_index = _angle / _measure;
	}
	else
	{
		_index = _angle / _measure;
		_index = (spriteCount - _index) - 1;
	}

	ID2D1Bitmap* _sprite = bitmap[_index];

	/// 아핀 트랜스폼 이펙트를 통해 차체의 이미지를 변환
	D2D1_VECTOR_2F _center = D2D1::Vector2F(-23, -25);

	D2D1_MATRIX_3X2_F _matrix = CTransform::MatrixTranslation(_center);
	D2D1_MATRIX_3X2_F _scale = CTransform::MakeScaleTransform(0.5, 0.5, _center);
	D2D1_MATRIX_3X2_F _resultMT = _scale * _matrix;

	ID2D1Effect* _effected = AddAffinTransform(_sprite, _resultMT);
	m_pD21DDeviceContext->DrawImage(_effected);
}

void D2DEngine::DrawShadowByRotation(vector<ID2D1Bitmap*> bitmap, D2D1_MATRIX_3X2_F projTM, const float angle, const int spriteCount,
	bool isIsometric)
{
	int _angle = abs(angle);

	if (isIsometric)
	{
		_angle = abs(angle + 45);
	}

	if (_angle >= 360)
	{
		_angle = _angle % 360;
	}

	int _measure = 360 / spriteCount;

	int _index = 0;

	if (angle >= 0)
	{
		_index = _angle / _measure;
	}
	else
	{
		_index = _angle / _measure;
		_index = (spriteCount - _index) - 1;
	}

	ID2D1Bitmap* _sprite = bitmap[_index];


	/// 아핀 트랜스폼 이펙트는 안쪽에서 한다.

	// 그림자 그리기
	ID2D1Effect* _effected = ApplyShadowByBitmap(_sprite);
	m_pD21DDeviceContext->DrawImage(_effected);
}


void D2DEngine::DrawSpriteAnimation(CA_Animation* animation, float dTime)
{

	D2D1_RECT_F _sourceRect;

	_sourceRect.left = animation->m_Index * animation->m_Width;
	_sourceRect.top = 0;
	_sourceRect.right = _sourceRect.left + animation->m_Width;
	_sourceRect.bottom = animation->m_Height;

	m_pD21DDeviceContext->DrawBitmap(
		animation->m_pBitmap,
		D2D1::RectF(0, 0, animation->m_Width, animation->m_Height),
		animation->m_Opacity,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		_sourceRect
	);

	animation->m_FrameCount++;

	if (animation->m_FrameCount > animation->m_FrameRate)
	{
		animation->m_FrameCount = 0;

		animation->m_Index++;

		if (animation->m_Index >= animation->m_MaxIndex)
		{
			animation->m_Index = 0;
		}
	}
}

void D2DEngine::DrawSpriteAnimation(CA_Animation* animation, D2D1_VECTOR_2F pos, float dTime)
{
	D2D1_RECT_F _sourceRect;

	_sourceRect.left = animation->m_Index * animation->m_Width;
	_sourceRect.top = 0;
	_sourceRect.right = _sourceRect.left + animation->m_Width;
	_sourceRect.bottom = animation->m_Height;

	m_pD21DDeviceContext->DrawBitmap(
		animation->m_pBitmap,
		D2D1::RectF(pos.x, pos.y, pos.x + animation->m_Width, pos.y + animation->m_Height),
		animation->m_Opacity,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		_sourceRect
	);

	animation->m_FrameCount++;

	if (animation->m_FrameCount > animation->m_FrameRate)
	{
		animation->m_FrameCount = 0;

		animation->m_Index++;

		if (animation->m_Index >= animation->m_MaxIndex)
		{
			animation->m_Index = 0;
		}
	}
}

void D2DEngine::CreateEffects()
{
	// 이펙트는 DeviceContext로부터 작성할 수 있다.
	m_pD21DDeviceContext->CreateEffect(CLSID_D2D1GaussianBlur, &m_pGaussianBlurEffect);
	m_pD21DDeviceContext->CreateEffect(CLSID_D2D1Opacity, &m_pOpacity);
	m_pD21DDeviceContext->CreateEffect(CLSID_D2D1Blend, &m_pBlendEffect);
	m_pD21DDeviceContext->CreateEffect(CLSID_D2D1Shadow, &m_pShadow);
	m_pD21DDeviceContext->CreateEffect(CLSID_D2D13DPerspectiveTransform, &m_pPerspectiveTransform);

	// Shadow를 위한 이펙트 생성
	m_pD21DDeviceContext->CreateEffect(CLSID_D2D1Flood, &m_pFloodEffect);
	m_pD21DDeviceContext->CreateEffect(CLSID_D2D12DAffineTransform, &m_pAffineTransformEffect);
	m_pD21DDeviceContext->CreateEffect(CLSID_D2D1Composite, &m_pCompositeEffect);

}

void D2DEngine::ApplyGaussianBlur(CGameObject* obj)
{
	ID2D1Bitmap* _bitmap = obj->GetBitmap();

	// SetInput에 들어가는 첫번째 파라미터는 
	// 이펙트 사용을 위해 들어갈 비트맵의 순서를 의미한다.
	// 한 개라면 0이면 되고, 2개 이상부터 차례로 숫자를 늘려서 넣어준다.
	m_pGaussianBlurEffect->SetInput(0, _bitmap);
	m_pGaussianBlurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, 3.0f);

	m_pD21DDeviceContext->DrawImage(m_pGaussianBlurEffect);
}

void D2DEngine::ApplyOpacity(CGameObject* obj, float val)
{
	ID2D1Bitmap* _bitmap = obj->GetBitmap();

	m_pOpacity->SetInput(0, _bitmap);
	m_pOpacity->SetValue(D2D1_OPACITY_PROP_OPACITY, val);

	m_pD21DDeviceContext->DrawImage(m_pOpacity);
}



void D2DEngine::ApplyBlend(CGameObject* obj, ID2D1Bitmap* bitmap2)
{
	ID2D1Bitmap* _bitmap = obj->GetBitmap();

	m_pBlendEffect->SetInput(0, _bitmap);

	// 현재 블렌드를 그냥 넣을 경우, 두 개의 비트맵이 동시에 출력된다.
	// image로 접근할 필요가 있겠다.
	m_pBlendEffect->SetValue(D2D1_BLEND_PROP_MODE, D2D1_BLEND_MODE_MULTIPLY);

	m_pD21DDeviceContext->DrawImage(m_pBlendEffect);
}

void D2DEngine::ApplyShadow(CGameObject* obj, D2D1_POINT_2F point)
{
	// 그림자 효과의 생성 과정
	// 1. Shadow 효과 포인터에 비트맵을 넣어, 출력할 그림자를 생성한다.
	// 사실 위 단계에서 이미 그림자는 생성된 상태지만, 다른 효과들의 도움이 필요하다.
	// 2. 아핀 변환을 통해 해당 그림자가 어디 생성될지 결정한다.
	// 3. 두 개의 이미지가 생겼으므로, Composite 효과를 통해 두 이미지를 묶어 출력한다.

	ID2D1Bitmap* _bitmap = obj->GetBitmap();

	m_pShadow->SetInput(0, _bitmap);

	m_pAffineTransformEffect->SetInputEffect(0, m_pShadow);

	// 원본을 기준으로 어디에 그림자가 질지 결정한다.
	D2D1_MATRIX_3X2_F _matrix = D2D1::Matrix3x2F::Translation(point.x, point.y);

	m_pAffineTransformEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, _matrix);

	// 두가지 이미지를 하나로 출력?
	m_pCompositeEffect->SetInputEffect(0, m_pAffineTransformEffect);
	m_pCompositeEffect->SetInput(1, _bitmap);

	m_pD21DDeviceContext->DrawImage(m_pCompositeEffect);
}

ID2D1Effect* D2DEngine::ApplyShadowByBitmap(ID2D1Bitmap* _bitmap)
{
	m_pShadow->SetInput(0, _bitmap);

	m_pAffineTransformEffect->SetInputEffect(0, m_pShadow);

	// 원본을 기준으로 어디에 그림자가 질지 결정한다.

	CTransform _transform;

	D2D1_VECTOR_2F _center = D2D1::Vector2F(-30, -35);
	D2D1_VECTOR_2F _scale = D2D1::Vector2F(0.5, 0.5);

	D2D1_MATRIX_3X2_F _matrix = _transform.GetTransformMatrix(_center, 0, _scale);

	//D2D1_MATRIX_3X2_F _matrix = _transform.MatrixTranslation(D2D1::Vector2F(-20, -20));
	//D2D1_MATRIX_3X2_F _scale = _transform.MakeScaleTransform(0.5, 0.5, D2D1::Vector2F(-20, -20));
	//D2D1_MATRIX_3X2_F _resultMT = _scale * _matrix;

	m_pAffineTransformEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, _matrix);
	/*
	// 두가지 이미지를 하나로 출력?
	m_pCompositeEffect->SetInputEffect(0, m_pAffineTransformEffect);
	m_pCompositeEffect->SetInput(1, _bitmap);
	*/
	//return m_pCompositeEffect;
	//return m_pShadow;
	return m_pAffineTransformEffect;
}

ID2D1Effect* D2DEngine::ApplyShadowByBitmap(ID2D1Effect* effect)
{
	m_pShadow->SetInputEffect(0, effect);

	m_pAffineTransformEffect->SetInputEffect(0, m_pShadow);

	// 원본을 기준으로 어디에 그림자가 질지 결정한다.
	D2D1_MATRIX_3X2_F _matrix = D2D1::Matrix3x2F::Translation(10, 10);


	m_pAffineTransformEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, _matrix);

	// 두가지 이미지를 하나로 출력?
	m_pCompositeEffect->SetInputEffect(0, m_pAffineTransformEffect);
	m_pCompositeEffect->SetInputEffect(1, effect);

	return m_pCompositeEffect;
}


void D2DEngine::ApplyPerspectiveTransform(ID2D1Bitmap* bitmap, D2D1_VECTOR_3F vec)
{
	m_pPerspectiveTransform->SetInput(0, bitmap);
	m_pPerspectiveTransform->SetValue(D2D1_3DPERSPECTIVETRANSFORM_PROP_PERSPECTIVE_ORIGIN, D2D1::Vector3F(0.0f, 192.0f, 0.0f));
	m_pPerspectiveTransform->SetValue(D2D1_3DPERSPECTIVETRANSFORM_PROP_ROTATION, vec);

	m_pD21DDeviceContext->DrawImage(m_pPerspectiveTransform);

}


ID2D1Effect* D2DEngine::AddAffinTransform(ID2D1Bitmap* bitmap, D2D1_MATRIX_3X2_F matrix)
{
	m_pAffineTransformEffect->SetInput(0, bitmap);
	m_pAffineTransformEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, matrix);

	return m_pAffineTransformEffect;
}

ID2D1Effect* D2DEngine::AddAffinTransform(ID2D1Effect* effect, D2D1_MATRIX_3X2_F matrix)
{
	m_pAffineTransformEffect->SetInputEffect(0, effect);
	m_pAffineTransformEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, matrix);

	return m_pAffineTransformEffect;
}

ID2D1Effect* D2DEngine::AddOpacity(ID2D1Effect* effect, float val)
{
	m_pOpacity->SetInputEffect(0, effect);
	m_pOpacity->SetValue(D2D1_OPACITY_PROP_OPACITY, val);

	return m_pOpacity;
}

void D2DEngine::DrawEffectBitmap(ID2D1Effect* effect)
{
	m_pD21DDeviceContext->DrawImage(effect);
}

void D2DEngine::MakingSpriteSheet(
	ID2D1Bitmap* bitmap,
	int rowCount,
	int columnCount,
	int verticalPos,
	Character* character,
	const char* motionName
)
{
	D2D1_SIZE_U _spritePixel = bitmap->GetPixelSize();

	float _spriteWidth = _spritePixel.width / rowCount;
	float _spriteHeight = _spritePixel.height / columnCount;

	CAMotion* _motion = new CAMotion();


	// rowCount만큼 스프라이트를 가로로 전진시켜준다.
	for (int x = 0; x < rowCount; x++)
	{
		CASprite* _sprite = new CASprite();

		// 소스 이미지에서 특정 부분을 가르킬 Rect 값이다.
		D2D1_RECT_F _imageRect;

		_imageRect.left = x * _spriteWidth;
		_imageRect.right = _imageRect.left + _spriteWidth;
		_imageRect.top = verticalPos * _spriteHeight;
		_imageRect.bottom = _imageRect.top + _spriteHeight;

		// 비트맵 정보를 가지고 뽑아낸 데이터를 m_SpritePos에 넣는다.
		_sprite->m_SpritePos = _imageRect;

		// 비트맵 각각의 가로/세로 크기를 넣어준다.
		_sprite->m_Size = D2D1::SizeF(_spriteWidth, _spriteHeight);

		// 해당 스프라이트가 얼마나 지속될 것인지를 넣어준다.
		_sprite->m_DelayFrame = 0.2f;

		// 충돌체의 크기를 의미한다.
		D2D1_RECT_F _aabbSize;

		// AABB의 크기는 비트맵에 의해 결정되는게 아닌, 독립적인 값을 갖는다.
		// 충돌체의 현재 위치는 CGameObject의 position 값에 따라 바뀐다. 여기서는 0, 0을 기준으로 한다.
		// 아래는 쉽게 설명하자면 스프라이트 크기만큼 하겠다는 뜻이다.
		_aabbSize.left = 0;
		_aabbSize.top = 0;
		_aabbSize.right = _spriteWidth;
		_aabbSize.bottom = _spriteHeight;

		CAABB* _aabb = new CAABB(_aabbSize);

		_sprite->m_HitBox = _aabb;

		_motion->AddSprite(_sprite);
	}

	// RTTI로 변경해준다.

	//Character* _realCharacter = dynamic_cast<Character*>(character);

	character->m_Motions.push_back(_motion);

	// 현재 푸쉬백 된 부분을 불러서 넣어서 이름을 넣어준다.
	int _index = character->m_Motions.size() - 1;
	character->m_Motions[_index]->Name = motionName;
}


CA_Animation* D2DEngine::LoadSpriteSheetByPath(PCWSTR path, int spriteCount, float frameRate, bool isLoop)
{
	CA_Animation* _aniStruct = new CA_Animation();

	_aniStruct->m_pBitmap = LoadBitmapByPath(path);
	_aniStruct->m_Index = 0;
	_aniStruct->m_FrameRate = frameRate;
	_aniStruct->m_FrameCount = 0;
	_aniStruct->m_Opacity = 1.0f;
	_aniStruct->m_IsLoop = isLoop;

	// 행으로만 이루어진 스프라이트라고 가정
	_aniStruct->m_Width = _aniStruct->m_pBitmap->GetPixelSize().width / spriteCount;
	_aniStruct->m_Height = _aniStruct->m_pBitmap->GetPixelSize().height;

	_aniStruct->m_MaxIndex = spriteCount;

	_aniStruct->m_SourceRect = D2D1::RectF(0, 0, _aniStruct->m_Width, _aniStruct->m_Height);

	return _aniStruct;
}

// 그냥 좌상단에 찍는다. UI등을 그리기 위한 함수
void D2DEngine::DrawUIImage(ID2D1Bitmap* pBitmap)
{
	if (pBitmap != nullptr)
	{
		m_pD21DDeviceContext->DrawBitmap(pBitmap);
	}
}

// 중점을 기준으로 찍는 함수. 인게임의 게임오브젝트등에 쓰인다.
void D2DEngine::DrawGOImage(ID2D1Bitmap* pBitmap)
{
	if (pBitmap != nullptr)
	{
		D2D1_SIZE_F _size = pBitmap->GetSize();
		D2D1_RECT_F _offset = D2D1::RectF(-_size.width / 2, -_size.height / 2,
			_size.width / 2, _size.height / 2);

		m_pD21DDeviceContext->DrawBitmap(pBitmap, _offset);
	}
}

void D2DEngine::ImageDrawByPoint(ID2D1Bitmap* pBitmap, D2D1_POINT_2F pos)
{
	if (pBitmap != nullptr)
	{
		int _width = pBitmap->GetPixelSize().width;
		int _height = pBitmap->GetPixelSize().height;
		m_pD21DDeviceContext->DrawBitmap(
			pBitmap,
			D2D1::RectF(pos.x, pos.y, pos.x + _width, pos.y + _height),
			1.0f
		);
	}
}

void D2DEngine::ImageDrawByPoint(ID2D1Bitmap* pBitmap, D2D1_POINT_2F pos, float opcity)
{
	if (pBitmap != nullptr)
	{

		int _width = pBitmap->GetPixelSize().width;
		int _height = pBitmap->GetPixelSize().height;

		//D2D1_RECT_F _rect;
		//_rect.left = (pos.x - _width);
		//_rect.top = (pos.y - _height);
		//_rect.right = (pos.x + _width / 2);
		//_rect.bottom = (pos.y + _height / 2);



		m_pD21DDeviceContext->DrawBitmap(
			pBitmap,
			D2D1::RectF(pos.x, pos.y, pos.x + _width, pos.y + _height),
			opcity
		);
	}
}

void D2DEngine::ImageDrawSmoke(ID2D1Bitmap* pBitmap, D2D1_POINT_2F pos, D2D1_SIZE_F size, float opcity)
{
	if (pBitmap != nullptr)
	{

		int _width = pBitmap->GetPixelSize().width;
		int _height = pBitmap->GetPixelSize().height;

		D2D1_RECT_F _rect;
		_rect.left = (pos.x - _width);
		_rect.top = (pos.y - _height);
		_rect.right = (pos.x + _width / 2) + size.width;
		_rect.bottom = (pos.y + _height / 2) + size.height;


		m_pD21DDeviceContext->DrawBitmap(
			pBitmap,
			_rect,
			opcity
		);
	}
}

void D2DEngine::ImageDrawSmokeFromSheet(ID2D1Bitmap* pBitmap, D2D1_RECT_F source, D2D1_POINT_2F pos, float size, float opcity)
{
	if (pBitmap != nullptr)
	{

		//int _width = pBitmap->GetPixelSize().width;
		//int _height = pBitmap->GetPixelSize().height;

		int _dynamicSize = 10 * size;

		D2D1_RECT_F _rect;
		_rect.left = (pos.x - _dynamicSize);
		_rect.top = (pos.y - _dynamicSize);
		_rect.right = (pos.x + _dynamicSize / 2);
		_rect.bottom = (pos.y + _dynamicSize / 2);


		m_pD21DDeviceContext->DrawBitmap(
			pBitmap,
			_rect,
			opcity,
			D2D1_INTERPOLATION_MODE_LINEAR,
			source
		);
	}
}


void D2DEngine::ImageDrawByScale(ID2D1Bitmap* pBitmap, D2D1_POINT_2F pos, D2D1_SIZE_F scale)
{
	float _width = pBitmap->GetPixelSize().width;
	float _height = pBitmap->GetPixelSize().height;

	D2D1_RECT_F _rect;
	_rect.left = pos.x;
	_rect.top = pos.y;
	_rect.right = pos.x + _width * scale.width;
	_rect.bottom = pos.y + _height * scale.height;

	m_pD21DDeviceContext->DrawBitmap(
		pBitmap,
		_rect,
		1,
		D2D1_INTERPOLATION_MODE_LINEAR
	);
}

// r, g, b, a값을 통해 '현재' 브러쉬 색상을 지정한다.
void D2DEngine::SetBrushColor(float r, float g, float b, float a)
{
	// 현재 브러쉬 정보를 모두 날려버린다.
	SafeRelease(&m_pNowBrush);

	m_pD21DDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(r, g, b, a)), &m_pNowBrush);
}

// D2D1::ColorF를 통해 '현재' 브러쉬 색상을 지정한다.
void D2DEngine::SetBrushColor(D2D1::ColorF color)
{
	// 현재 브러쉬 정보를 모두 날려버린다.
	SafeRelease(&m_pNowBrush);

	m_pD21DDeviceContext->CreateSolidColorBrush(color, &m_pNowBrush);
}

void D2DEngine::DrawRectangle_PreSetCol(int x, int y, int x1, int y1, eBRUSHCOLOR eBrushCol /*eBRUSHCOLOR로 색깔을 선택*/)
{
	// 검정색 브러쉬를 대표로 있는지 없는지 검사한다.
	if (m_pBlackBrush != NULL)
	{
		D2D1_RECT_F _rt;
		_rt.left = (float)x;
		_rt.top = (float)y;
		_rt.right = (float)x1;
		_rt.bottom = (float)y1;

		// arr_MyBrush 배열에 있는 브러쉬에 enum으로 접근하여 어떤 브러쉬를 쓸지 정한다.
		m_pD21DDeviceContext->DrawRectangle(_rt, arr_MyBrush[eBrushCol], 1.0f);
	}
}

void D2DEngine::DrawRectangle_PickCol(int x, int y, int x1, int y1, D2D1::ColorF color /*D2D1::ColorF로 색깔을 선택*/)
{
	// 이후 파라미터를 통해 color값을 받아, 원하는 색깔의 브러쉬를 만들어준다.
	m_pD21DDeviceContext->CreateSolidColorBrush(color, &m_pTempBrush);

	if (m_pTempBrush != NULL)
	{
		D2D1_RECT_F _rt;
		_rt.left = (float)x;
		_rt.top = (float)y;
		_rt.right = (float)x1;
		_rt.bottom = (float)y1;

		m_pD21DDeviceContext->DrawRectangle(_rt, m_pTempBrush, 1.0f);
	}

	// m_pTempBrush에 다음 번에 재사용을 위해 자원을 돌려준다.
	SafeRelease(&m_pTempBrush);
}

void D2DEngine::DrawRectangle_NowCol(int x, int y, int x1, int y1)
{
	if (m_pNowBrush != NULL)
	{
		D2D1_RECT_F _rt;
		_rt.left = (float)x;
		_rt.top = (float)y;
		_rt.right = (float)x1;
		_rt.bottom = (float)y1;

		m_pD21DDeviceContext->DrawRectangle(_rt, m_pNowBrush, 1.0f);
	}
}

void D2DEngine::DrawRectangle_Debug(int posX, int posY, int width, int height, bool isCollide)
{
	// 부딪혔는지를 판단한다.
	if (isCollide)
	{
		if (m_pRedBrush != NULL)
		{
			D2D1_RECT_F _rt;
			_rt.left = (float)posX;
			_rt.top = (float)posY;
			_rt.right = (float)posX + width;
			_rt.bottom = (float)posY + height;

			m_pD21DDeviceContext->DrawRectangle(_rt, m_pRedBrush, 1.0f);
		}
	}
	// 부딪혔다면 현재 브러쉬로 그려준다.
	else
	{
		if (m_pNowBrush != NULL)
		{
			D2D1_RECT_F _rt;
			_rt.left = (float)posX;
			_rt.top = (float)posY;
			_rt.right = (float)posX + width;
			_rt.bottom = (float)posY + height;

			m_pD21DDeviceContext->DrawRectangle(_rt, m_pNowBrush, 1.0f);
		}
	}
}


void D2DEngine::DrawFillRect_NowCol(D2D1_RECT_F _rect)
{
	if (m_pNowBrush != NULL)
	{
		m_pD21DDeviceContext->FillRectangle(_rect, m_pNowBrush);
	}
}

void D2DEngine::DrawFillRect_PickCol(D2D1_RECT_F _rect, D2D1::ColorF color)
{
	m_pD21DDeviceContext->CreateSolidColorBrush(color, &m_pTempBrush);

	if (m_pTempBrush != NULL)
	{
		m_pD21DDeviceContext->FillRectangle(_rect, m_pTempBrush);
	}

	SafeRelease(&m_pTempBrush);

}


void D2DEngine::DrawFillAlphaRect(D2D1_RECT_F _rect, float alpha)
{
	// 현재 그려줄 공간의 크기를 가져와서 right  값으로 넣어주면, 화면 크기만큼 상자가 생긴다.
	_rect.right = m_pD21DDeviceContext->GetSize().width;


	D2D1::ColorF color = (D2D1::ColorF(0.0f, 0.0f, 0.0f, alpha));
	m_pD21DDeviceContext->CreateSolidColorBrush(color, &m_pTempBrush);

	if (m_pTempBrush != NULL)
	{
		m_pD21DDeviceContext->FillRectangle(_rect, m_pTempBrush);
	}
	SafeRelease(&m_pTempBrush);

}


void D2DEngine::DrawEllipse_PreSetCol(float x, float y, float radius, eBRUSHCOLOR eBrushCol)
{
	// 파라미터를 통해 받아온 FLOAT값으로 POINT를 만든다.
	// 해당 지점은 Ellipse의 Center Point가 된다.
	if (m_pBlackBrush != NULL)
	{
		D2D_POINT_2F point;
		point.x = x;
		point.y = y;

		m_pD21DDeviceContext->DrawEllipse(D2D1::Ellipse(point, radius, radius), arr_MyBrush[eBrushCol], 1);
	}
}

void D2DEngine::DrawEllipse_PickCol(float x, float y, float radius, D2D1::ColorF color /*D2D1::ColorF로 색깔을 선택*/)
{
	// 이후 파라미터를 통해 color값을 받아, 원하는 색깔의 브러쉬를 만들어준다.
	m_pD21DDeviceContext->CreateSolidColorBrush(color, &m_pTempBrush);

	if (m_pTempBrush != NULL)
	{
		D2D_POINT_2F point;
		point.x = x;
		point.y = y;

		m_pD21DDeviceContext->DrawEllipse(D2D1::Ellipse(point, radius, radius), m_pTempBrush, 1);
	}

	// 다 그렸다면 다시 자원을 돌려주자.
	SafeRelease(&m_pTempBrush);
}

void D2DEngine::DrawEllipse_NowCol(float x, float y, float radius)
{
	if (m_pNowBrush != NULL)
	{
		D2D_POINT_2F point;
		point.x = x;
		point.y = y;

		m_pD21DDeviceContext->DrawEllipse(D2D1::Ellipse(point, radius, radius), m_pNowBrush, 1);
	}
}

void D2DEngine::DrawEllipse_Debug(float x, float y, float radius, bool isCollide)
{
	// 부딪혔는지를 판단한다.
	if (isCollide)
	{
		if (m_pRedBrush != NULL)
		{
			D2D_POINT_2F point;
			point.x = x;
			point.y = y;

			m_pD21DDeviceContext->DrawEllipse(D2D1::Ellipse(point, radius, radius), m_pRedBrush, 1);
		}
	}
	// 부딪혔다면 현재 브러쉬로 그려준다.
	else
	{
		if (m_pNowBrush != NULL)
		{
			D2D_POINT_2F point;
			point.x = x;
			point.y = y;

			m_pD21DDeviceContext->DrawEllipse(D2D1::Ellipse(point, radius, radius), m_pNowBrush, 1);
		}
	}
}

// 프리셋으로 만들어 둔 브러쉬를 통해 원하는 색깔로 라인을 그린다.
void D2DEngine::DrawLine_PreSetCol(float x1, float y1, float x2, float y2, eBRUSHCOLOR eBrushCol)
{
	D2D1_POINT_2F _point;
	_point.x = x1;
	_point.y = y1;
	D2D1_POINT_2F _point2;
	_point2.x = (float)x2;
	_point2.y = (float)y2;

	m_pD21DDeviceContext->DrawLine(_point, _point2, arr_MyBrush[eBrushCol], 1.0f);
}


void D2DEngine::DrawLine_PickCol(float x1, float y1, float x2, float y2, D2D1::ColorF color /*D2D1::ColorF로 색깔을 선택*/)
{
	m_pD21DDeviceContext->CreateSolidColorBrush(color, &m_pTempBrush);

	if (m_pTempBrush != NULL)
	{
		D2D1_POINT_2F _point;
		_point.x = x1;
		_point.y = y1;
		D2D1_POINT_2F _point2;
		_point2.x = x2;
		_point2.y = y2;

		m_pD21DDeviceContext->DrawLine(_point, _point2, m_pTempBrush, 1.0f);
	}

	SafeRelease(&m_pTempBrush);
}

void D2DEngine::DrawLine_NowCol(float x1, float y1, float x2, float y2)
{
	if (m_pNowBrush != NULL)
	{
		D2D1_POINT_2F _point;
		_point.x = x1;
		_point.y = y1;
		D2D1_POINT_2F _point2;
		_point2.x = x2;
		_point2.y = y2;

		m_pD21DDeviceContext->DrawLine(_point, _point2, m_pNowBrush, 1.0f);
	}
}

void D2DEngine::DrawLineByVector(D2D1_VECTOR_2F origin, D2D1_VECTOR_2F target, D2D1::ColorF color)
{
	m_pD21DDeviceContext->CreateSolidColorBrush(color, &m_pTempBrush);

	if (m_pTempBrush != NULL)
	{
		m_pD21DDeviceContext->DrawLine(
			D2D1::Point2F(origin.x, origin.y),
			D2D1::Point2F(target.x, target.y),
			m_pTempBrush);
	}

	SafeRelease(&m_pTempBrush);
}

void D2DEngine::DrawLineForTrail(D2D1_POINT_2F pos1, D2D1_POINT_2F pos2)
{
	m_pD21DDeviceContext->DrawLine(
		pos1,
		pos2,
		m_pBlackBrush,
		3.0f);
}

void D2DEngine::DrawLineForTrail(D2D1_POINT_2F pos1, D2D1_POINT_2F pos2, float opacity)
{
	m_pSkidMarkBrush->SetOpacity(opacity);

	m_pD21DDeviceContext->DrawLine(
		pos1,
		pos2,
		m_pSkidMarkBrush,
		4.0f);
}


D2D1_MATRIX_3X2_F D2DEngine::MakeIsometric(D2D1_VECTOR_2F center, float size, float rot)
{
	CTransform transform;

	D2D1_VECTOR_2F _sizeVec;
	_sizeVec.x = 1.0f;
	_sizeVec.y = 1.0f * size;

	D2D1_MATRIX_3X2_F transMT = transform.MakeComplexTransform(rot, _sizeVec, center);

	return transMT;
}

// 엔진 딴에서 SetTransform을 사용할 수 있도록 한 번 래핑했다.
void D2DEngine::SetTransform(D2D1_MATRIX_3X2_F transformMatrix)
{
	m_pD21DDeviceContext->SetTransform(transformMatrix);
}


// 각각의 객체가 독립적으로 회전할 수 있도록 래핑했다.
void D2DEngine::RotateDraw(ICollisionObject* pObject)
{
	CAABB* _AABB = dynamic_cast<CAABB*>(pObject);

	if (_AABB != nullptr)
	{

		D2D_VECTOR_2F _center;

		float _angle = _AABB->GetAngle();
		_center.x = _AABB->GetRectPosition().left + ((_AABB->GetRectPosition().right - _AABB->GetRectPosition().left) * 0.5f);
		_center.y = _AABB->GetRectPosition().top + ((_AABB->GetRectPosition().bottom - _AABB->GetRectPosition().top) * 0.5f);

		CTransform _rotate;
		D2D1_MATRIX_3X2_F _rotateTM = _rotate.MakeRotateTransform(_angle, _center);

		m_pD21DDeviceContext->SetTransform(_rotateTM);

		pObject->Draw(this);

		m_pD21DDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());

	}
	else
	{
		return;
	}

}

void D2DEngine::TransformRotation(float angle, int x, int y)
{
	m_pD21DDeviceContext->SetTransform(D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(x, y)));
	// 해당 함수가 호출된 다음에 그림을 그리면 회전되어 나온다.
}

// 기저 벡터가 다른 곳에 있다면 다시 초기화해준다.
void D2DEngine::TransformEnd()
{
	m_pD21DDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
}

bool D2DEngine::DrawText(int x, int y, const WCHAR* pch, ...)
{
	// 포맷 스트링 관련 버퍼, 가변인자 처리

	va_list ap;			// 현재 읽고 있는 번지를 기억
	va_start(ap, pch);	// ap 포인터 변수가 첫번째 가변인수를 가리키도록 초기화

	int len = _vscwprintf(pch, ap) + 1;
	WCHAR* buffer = new WCHAR[len];

	vswprintf_s(buffer, len, pch, ap);
	va_end(ap);

	//int nLen = lstrlenA(pch);
	int nLen = lstrlen(buffer);

	D2D1::ColorF color = (D2D1::ColorF(0.0f, 1.0f, 0.0f, 1.0f));

	m_pD21DDeviceContext->CreateSolidColorBrush(color, &m_pTempBrush);

	// 폰트별 차지하는 영역 계산 필요
	m_pD21DDeviceContext->DrawTextW(buffer, wcslen(buffer), m_pTextFormat,
		D2D1::RectF((float)x, (float)y, (float)(x + 500), (float)(y + 40)),
		m_pTempBrush);

	SafeRelease(&m_pTempBrush);

	// 포맷관련 버퍼 삭제
	delete[] buffer;

	return true;
	/// 소스 천국에 덜어낸 소스가 있다.
}

bool D2DEngine::DrawText(D2D1_POINT_2F pos, const WCHAR* pch, ...)
{
	// 포맷 스트링 관련 버퍼, 가변인자 처리

	va_list ap;			// 현재 읽고 있는 번지를 기억
	va_start(ap, pch);	// ap 포인터 변수가 첫번째 가변인수를 가리키도록 초기화

	int len = _vscwprintf(pch, ap) + 1;
	WCHAR* buffer = new WCHAR[len];

	vswprintf_s(buffer, len, pch, ap);
	va_end(ap);

	//int nLen = lstrlenA(pch);
	int nLen = lstrlen(buffer);

	// 폰트별 차지하는 영역 계산 필요

	m_pD21DDeviceContext->DrawTextW(buffer, wcslen(buffer), m_pTextFormat,
		D2D1::RectF(pos.x, pos.y, (pos.x + 500), (pos.y + 40)),
		m_pGreenBrush);

	// 포맷관련 버퍼 삭제
	delete[] buffer;

	return true;
}

bool D2DEngine::DrawText(D2D1_POINT_2F pos, const WCHAR* pch, D2D1::ColorF color...)
{
	// 포맷 스트링 관련 버퍼, 가변인자 처리

	va_list ap;			// 현재 읽고 있는 번지를 기억
	va_start(ap, pch);	// ap 포인터 변수가 첫번째 가변인수를 가리키도록 초기화

	int len = _vscwprintf(pch, ap) + 1;
	WCHAR* buffer = new WCHAR[len];

	vswprintf_s(buffer, len, pch, ap);
	va_end(ap);

	//int nLen = lstrlenA(pch);
	int nLen = lstrlen(buffer);

	// 폰트별 차지하는 영역 계산 필요

	m_pD21DDeviceContext->CreateSolidColorBrush(color, &m_pTempBrush);

	// 폰트별 차지하는 영역 계산 필요
	m_pD21DDeviceContext->DrawTextW(buffer, wcslen(buffer), m_pTextFormat,
		D2D1::RectF(pos.x, pos.y, (pos.x + 500), (pos.y + 40)),
		m_pTempBrush);

	SafeRelease(&m_pTempBrush);

	// 포맷관련 버퍼 삭제
	delete[] buffer;

	return true;
}
