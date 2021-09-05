#include "D2DEngine.h"

#include "Character.h"
#include "CAABB.h"
#include "CTransform.h"
#include "Smoke.h"

// ������ �ڿ����� Release�� �ִ� �Լ�
// pFactory ���� �ڿ��� ���� ���� �� �����ش�.
template <class T> void SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = nullptr;
	}
}

// ��� �������� �ʱ�ȭ���ִ� �Լ�
D2DEngine::D2DEngine() :
	m_hWnd(nullptr),

	// ���丮 �ʱ�ȭ
	m_pD2D1Factory1(nullptr),
	m_pWICFactory(nullptr),
	m_pWriteFactory(nullptr),

	// �̹��� ���� �ʱ�ȭ
	m_pWICDecoder(nullptr),
	m_pWICConverter(nullptr),
	m_pFrame(nullptr),

	// ���� Ÿ�� �ʱ�ȭ
	//m_pRenderTarget(nullptr),
	m_pD21DDeviceContext(nullptr),

	// �׽�Ʈ�� �̹��� �ʱ�ȭ
	m_pBitmap(nullptr),

	// �귯���� �ʱ�ȭ
	m_pBlackBrush(nullptr), m_pWhiteBrush(nullptr),
	m_pRedBrush(nullptr), m_pGreenBrush(nullptr), m_pBlueBrush(nullptr),
	m_pTempBrush(nullptr), m_pNowBrush(nullptr), m_pSkidMarkBrush(nullptr),
	// �ؽ�Ʈ ���� ��� �ʱ�ȭ
	m_pTextFormat(nullptr),
	// ���� �Ǵ� ��� �ʱ�ȭ
	m_IsReady(S_FALSE),
	// ������ Ÿ�� ������ 0���� �ʱ�ȭ
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
	// ���� �����忡�� COM ���̺귯���� �ʱ�ȭ���ִ� �Լ���.
	// �̹����� �ε��� WIC�� Ȱ���ϱ� ���ؼ� ���ľ��ϴ� �ܰ��̴�.
	// ��, COM ���̺귯���� ��Ȯ�� � ���� �ϴ����� ���� �� �˾ƺ����Ѵ�.
	HRESULT _hResult = CoInitialize(NULL);


	// ������ �Լ� ����߿� �ʿ��� ���ø����̼�(������) �ڵ��� ������ ��������� �����Ѵ�.
	// �ʿ��� ������ �Ķ���ͷ� �������� ������ ����.
	m_hWnd = hWnd;

	// hr�� ���� S_OK�� �ʱ�ȭ���ش�.
	HRESULT hr = S_OK;

	// �Լ��� ����ü�� ���丮�� �����.
	// Demo������ ���� �� ������, �������� ����� �� ����.
	// ������ Demo���� Direct2D ���� ����� �ְ�, lib���� �߰��ؾ� �ϱ� �����̴�.
	/// D2D1_1 ������ ���丮�� �ʱ�ȭ�Ѵ�.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2D1Factory1);

	// �̹��� �ε忡 ���̴� WIC�� Ȱ���ϱ� ���� �ʱ�ȭ �ܰ踦 �����Ѵ�.
	// CoCreateInstance�� �� ù ��°�� m_pWICFactory�� ����� ����� �����Ѵ�.
	// CLSID��, Ŭ���� ID��� �ǹ��ε�, Windows�� ��� ���� ���α׷� ���� ����� ���� ID�� ���³���.
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

	// �Ʒ������� DirectWrite�� ���� �ʱ�ȭ�� �����Ѵ�.
	// DirectWrite�� �ؽ�Ʈ�� ���� �پ��ϰ� ǥ���� �� �ֵ��� �����ִ� DirectX API��.
	// �׸��� �׸� ���� ���������� �ؽ�Ʈ ��¿� �ʿ��� Factory�� ���� �����ϸ�, �̸� ���� ��������� �Ѵ�.
	// ����, �ؽ�Ʈ�� �ᱹ �������� �ʿ��ϹǷ� D2DFacrtory�� RenderTarget�� �ʿ�� �Ѵ�.
	// ����� ���� �Լ��� Ŭ�������� dwrite.h�� ���ԵǾ��ִ�.
	// MSDN : https://docs.microsoft.com/en-us/windows/win32/directwrite/direct-write-portal

	// � ��Ʈ�� ����� ������, �󸶳� ũ�� ����� �������� ���Ѵ�.
	static const WCHAR msc_fontName[] = L"����";			// ��Ʈ
	static const float msc_fontSize = 12;				// ũ��

	// D2DFactory�� ��������ٸ�, D2D�� ���� �������� Write Factory�� �����.
	if (SUCCEEDED(hr))
	{

		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,							// ���丮 Ÿ�� ����
			__uuidof(m_pWriteFactory),							// ���丮 �������̽��� �ĺ��ϴ� ��
			reinterpret_cast<IUnknown**>(&m_pWriteFactory)		// ���丮 ��ü�� ���� ������ �ּ�
		);
	}

	if (SUCCEEDED(hr))
	{
		// DirectWrite�� ���� ����� �ؽ�Ʈ�� ����(��Ÿ�� ��)�� �����Ѵ�.
		hr = m_pWriteFactory->CreateTextFormat(
			msc_fontName,							// ��Ʈ 
			NULL,									// ��Ʈ �÷���, NULL�� �θ� system font �÷������� �����ȴ�.
			DWRITE_FONT_WEIGHT_NORMAL,				// �ؽ�Ʈ�� �β�
			DWRITE_FONT_STYLE_NORMAL,				// �ؽ�Ʈ�� �۲� ��Ÿ��
			DWRITE_FONT_STRETCH_NORMAL,				// �ؽ�Ʈ�� ���� ����
			msc_fontSize,							// ��Ʈ ������ (��ġ ���� �ȼ�)
			L"",									// ������ �̸� L"en-us"�� ǥ�� ����, ������ ��� �ڵ����� �����Ǵµ�
			&m_pTextFormat							// ���� �����Ϳ� �������� ��´�.
		);
	}

	// �ؽ�Ʈ ���� ���
	if (SUCCEEDED(hr))
	{
		// Left-Top the text horizontally and vertically.
		// �ؽ�Ʈ�� ���̾ƿ� ���ڸ� �������� ���� �����ڸ��� ���� ��� �����ڸ��� ���ĵǵ�����
		m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	}

	m_IsReady = CreateDeviceResources();
}

HRESULT D2DEngine::CreateDeviceResources()
{
	// ���� Ÿ�� ������ �� DeviceContext�� �����.
	// ���� ������ ���������� Initialize���� ���� ���� Ÿ���� �����ߴµ�,
	// �̹� ���������� �ش� �κ��� ���������� �����Ǿ���.
	// BeginRender�� �Ҷ����� ���������� �ʾƵ� �ȴ�.
	// ��, BeginRender���� �� �Լ��� �� �ֱ� ������,
	// m_pD2DDeviceContext�� ���¸� ���� ������ ������.
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

		/// �� �𸣰ڴ� �Ʒ���
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
			&_swapChainDescFull, // ��üȭ�� ����ü�� ����
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

		// ����Ʈ ������ ���⼭ �Բ� ���ش�.
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
	// ��������� �ڵ带 ¥�� ���� �Ź� hr�� return������ �ްڴ�.
	// ��� ���α׷��� ���ư���.
	HRESULT hr = S_OK;

	// IWIC��� ���ξ�� �� �� �ֵ�, �̵��� ��� Interface�� �ǹ��Ѵ�.
	// Interface��, ���� �����ؼ� �Լ��� ������ �ǹ��Ѵ�.
	IWICBitmapDecoder* pDecoder = nullptr;		// ����� �̹��� ������ ���ڵ����ִ� �������̽���.
	IWICBitmapFrameDecode* pSource = nullptr;	// ���ڵ��� �����͸� �Ѱ��� �������̽���.
	IWICFormatConverter* pConverter = nullptr;	// ���ڵ��� �����͸� ��Ʈ������ �ٲ��ִ� �������̽���.
	IWICBitmapScaler* pScaler = nullptr;		// ��Ʈ���� �����Ϸ� ���� �ʱ�ȭ���ִ� �������̽���.


	// �������� ��� �� ��Ʈ�� �ε��� ���۵ȴ�.
	// 1. ���� ���� �ϰԵ� ������ WICFactory�� ���� �̹����� �ε��ϴ� ���̴�.
	// 1-2. �翬�� Factory �������̽��� �ʿ��ϸ�, PCWSTR ������ ��θ� �Լ��� �Ű������� �Ѱ��ش�.
	// 1-3. ���� �б� �ɼǰ� ���� ó���� ���� �ɼ��� �ش�.
	// 1-4. ���� �ش� ���Ͽ� ���� ������ Decoder ��ü�� �����ȴ�.
	// 1-5. Decoder�� PNG�� ���ڵ����ִ� ������ �Ѵ�.
	hr = pIWICFactory->CreateDecoderFromFilename(
		path,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);

	// 2. ���ڴ��κ��� �� ��° �������� �̹����� �������� �����Ѵ�.
	// 2-1. �Ϲ����� �̹����� ù��° �����ӿ� �̹����� ����ȴ�. ���� 0�� �־� ��ü�� �����Ѵ�.
	// 2-2. �̶� ��ü�� pSorce ��, FrameDecode�� �ȴ�.
	if (SUCCEEDED(hr))
	{
		hr = pDecoder->GetFrame(0, &pSource);
	}

	// 3. �̹��� ������ ��Ʈ�� �������� ��ȯ���ִ� ������ �����Ѵ�.
	// 3-1. �̹��� ������ ��ȯ���ִ� �������̽��� Converter�� �����Ѵ�.
	if (SUCCEEDED(hr))
	{
		hr = pIWICFactory->CreateFormatConverter(&pConverter);
	}

	// 4. ��Ʈ�� �������� �ٲ� �����͸� Bitmap��ü�� �־��ֱ� ���� �߰� �۾��� �����Ѵ�.

	// 3-2. ��Ʈ�� �������� �ٲ� �����͸� Bitmap��ü�� �־��ֱ� ���� �߰� �۾��� �����Ѵ�.
	if (SUCCEEDED(hr))
	{
		// �׸� �̹����� ����/���� ����� Ư���ϰ� �ʹٸ�,
		// BitmapScaler�� ���� �̹����� �������� �� �ִ�.
		// �Ʒ��� �̸� ���� if��������, ������ ����/���� ����� 0�� �ƴ϶��,
		// ��, �������� �ʿ��ϴٸ� �����ϰ� �ȴ�.
		if (destinationWidth != 0 || destinationHeight != 0)
		{
			// �ϴ�, �츮�� ��ǥ���ϴ� ����/���� ������ʹ� �������, ������ ũ�⸦ ���ؼ� �����صд�.
			UINT originalWidth, originalHeight;
			hr = pSource->GetSize(&originalWidth, &originalHeight);
			if (SUCCEEDED(hr))
			{
				if (destinationWidth == 0)
				{
					// ��ǥ ũ�� / ���� ũ��� ������ ���Ѵ���, �̸� �ٽ� ���� ũ�⿡ ���ϴ� ����� ���Ѵ�.
					float scalar = static_cast<float>(destinationHeight) / static_cast<float>(originalHeight);
					destinationWidth = static_cast<UINT>(scalar * static_cast<float>(originalWidth));
				}
				else if (destinationHeight == 0)
				{
					float scalar = static_cast<float>(destinationWidth) / static_cast<float>(originalWidth);
					destinationHeight = static_cast<UINT>(scalar * static_cast<float>(originalHeight));
				}

				// ũ�Ⱑ �������ٸ�, �̵��� �Ű������� pScaler�� �Լ��� �����Ѵ�.
				// � �ҽ�����, ũ��� ������, ũ�� ��ȭ�� ���� �ȼ� ó�� �˰����� � ���� ������� �����Ѵ�.
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

				// ���� pScaler �������̽��κ��� �����͸� ������ pConverter�� �ʱ�ȭ�Ѵ�.
				// �������� ��Ʈ���� �־��ְ�, ����� �ȼ� ���İ� ����� �˰����� �����Ѵ�.
				// �ȼ� ������ �� �𸣰�����, ������� ������Ű � �� ���� �Ǿ������� Ȯ���غ���.
				// https://namu.wiki/w/%EB%94%94%EB%8D%94%EB%A7%81 >> ���ſ��� ������ ǥ���� ���� �˰����̾�����,
				// ���翡�� ���� ǥ���� �Ѱ踦 �غ��ϱ� ���� ���ȴٰ� �Ѵ�.
				// �̿ܿ� ���İ��̳� �ȷ�Ʈ ���� �����Ѵ�.
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

		// ũ�Ⱑ ������� �ʴ� ��� ���� ���� ������ ��ġ�� �ʴ´�.
		// ���� ũ�⸦ ������ destinySize�� 0�� ������.
		// ������ Transform���ε� ũ�� ������ �������ٵ�, �̷��� �ϴ� ������ �����ϱ�?
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

	// ���������� �츮�� ���� ����� bitmap�� �����Ϳ� ��Ʈ���� �־��ش�.
	// �����ñ��� �������Ƿ�, bitmap �������̽����� ��ο츦 ���ָ� �ȴ�.
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

	// ����̽� ������ �ڿ����� ���� ����, �������� �� �� �ִ� ��Ȳ�� ��
	// CheckWindowState�� ���� ���� Ÿ�ٰ� ���õ� HWND(�� ������)�� ���������� �Ǵ��ϴ� �Լ���.
	// D2D1_WINDOW_STATE enum�� ��� �� �ϳ��� ��ȯ�Ѵ�.
	// ���⼭ D2D1_WINDOW_STATE_OCCLUDED�� ���������� �ǹ��ϹǷ�, �׷��� �ʾƾ� �ϱ� ������ !�� ������ �ɾ���.
	if (SUCCEEDED(m_IsReady))
	{
		m_RenderTargetSize = m_pD21DDeviceContext->GetSize();

		// �׸���� BegineDraw()�� EndDraw()���̿�
		// BeginDraw�� EndDraw�� Direct2D �ý��ۿ��� ������ ����� ����ϰ� ������ �˷��ִ� �뵵��.
		m_pD21DDeviceContext->BeginDraw();

		// ����� Identity�� �ϰڴٴ� �ǹ̴� �������͸� ó������ �����ְڴٴ� �ǹ��̴�.
		// �ٲ㸻�ϸ�, �ƹ��� �������͸� �ű�ų� ������ BeginRender���� �ʱ�ȭ�ϰڴٴ� ���̴�.
		m_pD21DDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());

		// ���� Ÿ���� �׸� �׸��� ������ black���� ä��ڴ�.
		m_pD21DDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	}
}

void D2DEngine::EndRender()
{
	// m_IsReady�� �׸��� ���۰� �Բ� �Ź� ���� �Ҵ�ǹǷ�, ���⼭�� ������ �ʾƵ� �ȴ�.
	// �̹� BeginRender�� �������� ���̹Ƿ�
	// ���������� ���� �����찡 ������ �ִ��� �Ǵ����ش�.
	if (SUCCEEDED(m_IsReady))
	{
		HRESULT hr = m_pD21DDeviceContext->EndDraw();

		DXGI_PRESENT_PARAMETERS parameters;
		parameters.DirtyRectsCount = 0;
		parameters.pDirtyRects = nullptr;
		parameters.pScrollOffset = nullptr;
		parameters.pScrollRect = nullptr;

		m_pSwapChain->Present1(1, 0, &parameters);

		// EndDraw������ �׸��� ���� �߻��� ���� � ���� �Ǻ����ش�.
		// �̶� D2DERR_RECREATE_TARGET�� �߻��� ��쿡�� ������ ����� �ٽ� ������� ������ �������� �ٽ� �õ��Ѵ�.
		// �߸𸣰ٴ�.
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
	// ��, Factory�� ���� ������ ����Ǵ� �������� �ڿ��� �����ش�.
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
		// �ε��� ���� �� ����üũ�� �ؾ� ��
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

	// �� ������� ó���� ��� ��ȸ���� ó���� �� ����.
	int _angle = abs(angle);


	if (_angle >= 360)
	{
		_angle = _angle % 360;
	}

	int _width = carBitmap->GetPixelSize().width;
	int _height = carBitmap->GetPixelSize().height;

	// ������ ��������Ʈ�� ũ�⸦ ��Ÿ����.
	// �ϳ��� ��������Ʈ ũ�Ⱑ 100�ȼ��̶��, _individual���� 100�� ����.
	int _individual = _width / spriteCount;

	int _measure = 360 / spriteCount;

	float _targetSize = 0.25f;
	// 0, 0 ��ǥ�� ��������Ʈ �׸���
	D2D1_RECT_F _rect;
	_rect.left = -(_individual * _targetSize);
	_rect.top = -(_height * _targetSize);
	_rect.right = _individual * _targetSize;
	_rect.bottom = _height * _targetSize;

	int _index = 0;

	// ����϶�
	if (angle >= 0)
	{
		_index = _angle / _measure;
	}
	else
	{
		_index = _angle / _measure;
		_index = (spriteCount - _index) - 1;
	}

	/// �߶󾲴� ����� �𸣰ڵ�.
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

	/// ���� Ʈ������ ����Ʈ�� ���� ��ü�� �̹����� ��ȯ
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


	/// ���� Ʈ������ ����Ʈ�� ���ʿ��� �Ѵ�.

	// �׸��� �׸���
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
	// ����Ʈ�� DeviceContext�κ��� �ۼ��� �� �ִ�.
	m_pD21DDeviceContext->CreateEffect(CLSID_D2D1GaussianBlur, &m_pGaussianBlurEffect);
	m_pD21DDeviceContext->CreateEffect(CLSID_D2D1Opacity, &m_pOpacity);
	m_pD21DDeviceContext->CreateEffect(CLSID_D2D1Blend, &m_pBlendEffect);
	m_pD21DDeviceContext->CreateEffect(CLSID_D2D1Shadow, &m_pShadow);
	m_pD21DDeviceContext->CreateEffect(CLSID_D2D13DPerspectiveTransform, &m_pPerspectiveTransform);

	// Shadow�� ���� ����Ʈ ����
	m_pD21DDeviceContext->CreateEffect(CLSID_D2D1Flood, &m_pFloodEffect);
	m_pD21DDeviceContext->CreateEffect(CLSID_D2D12DAffineTransform, &m_pAffineTransformEffect);
	m_pD21DDeviceContext->CreateEffect(CLSID_D2D1Composite, &m_pCompositeEffect);

}

void D2DEngine::ApplyGaussianBlur(CGameObject* obj)
{
	ID2D1Bitmap* _bitmap = obj->GetBitmap();

	// SetInput�� ���� ù��° �Ķ���ʹ� 
	// ����Ʈ ����� ���� �� ��Ʈ���� ������ �ǹ��Ѵ�.
	// �� ����� 0�̸� �ǰ�, 2�� �̻���� ���ʷ� ���ڸ� �÷��� �־��ش�.
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

	// ���� ���带 �׳� ���� ���, �� ���� ��Ʈ���� ���ÿ� ��µȴ�.
	// image�� ������ �ʿ䰡 �ְڴ�.
	m_pBlendEffect->SetValue(D2D1_BLEND_PROP_MODE, D2D1_BLEND_MODE_MULTIPLY);

	m_pD21DDeviceContext->DrawImage(m_pBlendEffect);
}

void D2DEngine::ApplyShadow(CGameObject* obj, D2D1_POINT_2F point)
{
	// �׸��� ȿ���� ���� ����
	// 1. Shadow ȿ�� �����Ϳ� ��Ʈ���� �־�, ����� �׸��ڸ� �����Ѵ�.
	// ��� �� �ܰ迡�� �̹� �׸��ڴ� ������ ��������, �ٸ� ȿ������ ������ �ʿ��ϴ�.
	// 2. ���� ��ȯ�� ���� �ش� �׸��ڰ� ��� �������� �����Ѵ�.
	// 3. �� ���� �̹����� �������Ƿ�, Composite ȿ���� ���� �� �̹����� ���� ����Ѵ�.

	ID2D1Bitmap* _bitmap = obj->GetBitmap();

	m_pShadow->SetInput(0, _bitmap);

	m_pAffineTransformEffect->SetInputEffect(0, m_pShadow);

	// ������ �������� ��� �׸��ڰ� ���� �����Ѵ�.
	D2D1_MATRIX_3X2_F _matrix = D2D1::Matrix3x2F::Translation(point.x, point.y);

	m_pAffineTransformEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, _matrix);

	// �ΰ��� �̹����� �ϳ��� ���?
	m_pCompositeEffect->SetInputEffect(0, m_pAffineTransformEffect);
	m_pCompositeEffect->SetInput(1, _bitmap);

	m_pD21DDeviceContext->DrawImage(m_pCompositeEffect);
}

ID2D1Effect* D2DEngine::ApplyShadowByBitmap(ID2D1Bitmap* _bitmap)
{
	m_pShadow->SetInput(0, _bitmap);

	m_pAffineTransformEffect->SetInputEffect(0, m_pShadow);

	// ������ �������� ��� �׸��ڰ� ���� �����Ѵ�.

	CTransform _transform;

	D2D1_VECTOR_2F _center = D2D1::Vector2F(-30, -35);
	D2D1_VECTOR_2F _scale = D2D1::Vector2F(0.5, 0.5);

	D2D1_MATRIX_3X2_F _matrix = _transform.GetTransformMatrix(_center, 0, _scale);

	//D2D1_MATRIX_3X2_F _matrix = _transform.MatrixTranslation(D2D1::Vector2F(-20, -20));
	//D2D1_MATRIX_3X2_F _scale = _transform.MakeScaleTransform(0.5, 0.5, D2D1::Vector2F(-20, -20));
	//D2D1_MATRIX_3X2_F _resultMT = _scale * _matrix;

	m_pAffineTransformEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, _matrix);
	/*
	// �ΰ��� �̹����� �ϳ��� ���?
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

	// ������ �������� ��� �׸��ڰ� ���� �����Ѵ�.
	D2D1_MATRIX_3X2_F _matrix = D2D1::Matrix3x2F::Translation(10, 10);


	m_pAffineTransformEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, _matrix);

	// �ΰ��� �̹����� �ϳ��� ���?
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


	// rowCount��ŭ ��������Ʈ�� ���η� ���������ش�.
	for (int x = 0; x < rowCount; x++)
	{
		CASprite* _sprite = new CASprite();

		// �ҽ� �̹������� Ư�� �κ��� ����ų Rect ���̴�.
		D2D1_RECT_F _imageRect;

		_imageRect.left = x * _spriteWidth;
		_imageRect.right = _imageRect.left + _spriteWidth;
		_imageRect.top = verticalPos * _spriteHeight;
		_imageRect.bottom = _imageRect.top + _spriteHeight;

		// ��Ʈ�� ������ ������ �̾Ƴ� �����͸� m_SpritePos�� �ִ´�.
		_sprite->m_SpritePos = _imageRect;

		// ��Ʈ�� ������ ����/���� ũ�⸦ �־��ش�.
		_sprite->m_Size = D2D1::SizeF(_spriteWidth, _spriteHeight);

		// �ش� ��������Ʈ�� �󸶳� ���ӵ� �������� �־��ش�.
		_sprite->m_DelayFrame = 0.2f;

		// �浹ü�� ũ�⸦ �ǹ��Ѵ�.
		D2D1_RECT_F _aabbSize;

		// AABB�� ũ��� ��Ʈ�ʿ� ���� �����Ǵ°� �ƴ�, �������� ���� ���´�.
		// �浹ü�� ���� ��ġ�� CGameObject�� position ���� ���� �ٲ��. ���⼭�� 0, 0�� �������� �Ѵ�.
		// �Ʒ��� ���� �������ڸ� ��������Ʈ ũ�⸸ŭ �ϰڴٴ� ���̴�.
		_aabbSize.left = 0;
		_aabbSize.top = 0;
		_aabbSize.right = _spriteWidth;
		_aabbSize.bottom = _spriteHeight;

		CAABB* _aabb = new CAABB(_aabbSize);

		_sprite->m_HitBox = _aabb;

		_motion->AddSprite(_sprite);
	}

	// RTTI�� �������ش�.

	//Character* _realCharacter = dynamic_cast<Character*>(character);

	character->m_Motions.push_back(_motion);

	// ���� Ǫ���� �� �κ��� �ҷ��� �־ �̸��� �־��ش�.
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

	// �����θ� �̷���� ��������Ʈ��� ����
	_aniStruct->m_Width = _aniStruct->m_pBitmap->GetPixelSize().width / spriteCount;
	_aniStruct->m_Height = _aniStruct->m_pBitmap->GetPixelSize().height;

	_aniStruct->m_MaxIndex = spriteCount;

	_aniStruct->m_SourceRect = D2D1::RectF(0, 0, _aniStruct->m_Width, _aniStruct->m_Height);

	return _aniStruct;
}

// �׳� �»�ܿ� ��´�. UI���� �׸��� ���� �Լ�
void D2DEngine::DrawUIImage(ID2D1Bitmap* pBitmap)
{
	if (pBitmap != nullptr)
	{
		m_pD21DDeviceContext->DrawBitmap(pBitmap);
	}
}

// ������ �������� ��� �Լ�. �ΰ����� ���ӿ�����Ʈ� ���δ�.
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

// r, g, b, a���� ���� '����' �귯�� ������ �����Ѵ�.
void D2DEngine::SetBrushColor(float r, float g, float b, float a)
{
	// ���� �귯�� ������ ��� ����������.
	SafeRelease(&m_pNowBrush);

	m_pD21DDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(r, g, b, a)), &m_pNowBrush);
}

// D2D1::ColorF�� ���� '����' �귯�� ������ �����Ѵ�.
void D2DEngine::SetBrushColor(D2D1::ColorF color)
{
	// ���� �귯�� ������ ��� ����������.
	SafeRelease(&m_pNowBrush);

	m_pD21DDeviceContext->CreateSolidColorBrush(color, &m_pNowBrush);
}

void D2DEngine::DrawRectangle_PreSetCol(int x, int y, int x1, int y1, eBRUSHCOLOR eBrushCol /*eBRUSHCOLOR�� ������ ����*/)
{
	// ������ �귯���� ��ǥ�� �ִ��� ������ �˻��Ѵ�.
	if (m_pBlackBrush != NULL)
	{
		D2D1_RECT_F _rt;
		_rt.left = (float)x;
		_rt.top = (float)y;
		_rt.right = (float)x1;
		_rt.bottom = (float)y1;

		// arr_MyBrush �迭�� �ִ� �귯���� enum���� �����Ͽ� � �귯���� ���� ���Ѵ�.
		m_pD21DDeviceContext->DrawRectangle(_rt, arr_MyBrush[eBrushCol], 1.0f);
	}
}

void D2DEngine::DrawRectangle_PickCol(int x, int y, int x1, int y1, D2D1::ColorF color /*D2D1::ColorF�� ������ ����*/)
{
	// ���� �Ķ���͸� ���� color���� �޾�, ���ϴ� ������ �귯���� ������ش�.
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

	// m_pTempBrush�� ���� ���� ������ ���� �ڿ��� �����ش�.
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
	// �ε��������� �Ǵ��Ѵ�.
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
	// �ε����ٸ� ���� �귯���� �׷��ش�.
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
	// ���� �׷��� ������ ũ�⸦ �����ͼ� right  ������ �־��ָ�, ȭ�� ũ�⸸ŭ ���ڰ� �����.
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
	// �Ķ���͸� ���� �޾ƿ� FLOAT������ POINT�� �����.
	// �ش� ������ Ellipse�� Center Point�� �ȴ�.
	if (m_pBlackBrush != NULL)
	{
		D2D_POINT_2F point;
		point.x = x;
		point.y = y;

		m_pD21DDeviceContext->DrawEllipse(D2D1::Ellipse(point, radius, radius), arr_MyBrush[eBrushCol], 1);
	}
}

void D2DEngine::DrawEllipse_PickCol(float x, float y, float radius, D2D1::ColorF color /*D2D1::ColorF�� ������ ����*/)
{
	// ���� �Ķ���͸� ���� color���� �޾�, ���ϴ� ������ �귯���� ������ش�.
	m_pD21DDeviceContext->CreateSolidColorBrush(color, &m_pTempBrush);

	if (m_pTempBrush != NULL)
	{
		D2D_POINT_2F point;
		point.x = x;
		point.y = y;

		m_pD21DDeviceContext->DrawEllipse(D2D1::Ellipse(point, radius, radius), m_pTempBrush, 1);
	}

	// �� �׷ȴٸ� �ٽ� �ڿ��� ��������.
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
	// �ε��������� �Ǵ��Ѵ�.
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
	// �ε����ٸ� ���� �귯���� �׷��ش�.
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

// ���������� ����� �� �귯���� ���� ���ϴ� ����� ������ �׸���.
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


void D2DEngine::DrawLine_PickCol(float x1, float y1, float x2, float y2, D2D1::ColorF color /*D2D1::ColorF�� ������ ����*/)
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

// ���� ������ SetTransform�� ����� �� �ֵ��� �� �� �����ߴ�.
void D2DEngine::SetTransform(D2D1_MATRIX_3X2_F transformMatrix)
{
	m_pD21DDeviceContext->SetTransform(transformMatrix);
}


// ������ ��ü�� ���������� ȸ���� �� �ֵ��� �����ߴ�.
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
	// �ش� �Լ��� ȣ��� ������ �׸��� �׸��� ȸ���Ǿ� ���´�.
}

// ���� ���Ͱ� �ٸ� ���� �ִٸ� �ٽ� �ʱ�ȭ���ش�.
void D2DEngine::TransformEnd()
{
	m_pD21DDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
}

bool D2DEngine::DrawText(int x, int y, const WCHAR* pch, ...)
{
	// ���� ��Ʈ�� ���� ����, �������� ó��

	va_list ap;			// ���� �а� �ִ� ������ ���
	va_start(ap, pch);	// ap ������ ������ ù��° �����μ��� ����Ű���� �ʱ�ȭ

	int len = _vscwprintf(pch, ap) + 1;
	WCHAR* buffer = new WCHAR[len];

	vswprintf_s(buffer, len, pch, ap);
	va_end(ap);

	//int nLen = lstrlenA(pch);
	int nLen = lstrlen(buffer);

	D2D1::ColorF color = (D2D1::ColorF(0.0f, 1.0f, 0.0f, 1.0f));

	m_pD21DDeviceContext->CreateSolidColorBrush(color, &m_pTempBrush);

	// ��Ʈ�� �����ϴ� ���� ��� �ʿ�
	m_pD21DDeviceContext->DrawTextW(buffer, wcslen(buffer), m_pTextFormat,
		D2D1::RectF((float)x, (float)y, (float)(x + 500), (float)(y + 40)),
		m_pTempBrush);

	SafeRelease(&m_pTempBrush);

	// ���˰��� ���� ����
	delete[] buffer;

	return true;
	/// �ҽ� õ���� ��� �ҽ��� �ִ�.
}

bool D2DEngine::DrawText(D2D1_POINT_2F pos, const WCHAR* pch, ...)
{
	// ���� ��Ʈ�� ���� ����, �������� ó��

	va_list ap;			// ���� �а� �ִ� ������ ���
	va_start(ap, pch);	// ap ������ ������ ù��° �����μ��� ����Ű���� �ʱ�ȭ

	int len = _vscwprintf(pch, ap) + 1;
	WCHAR* buffer = new WCHAR[len];

	vswprintf_s(buffer, len, pch, ap);
	va_end(ap);

	//int nLen = lstrlenA(pch);
	int nLen = lstrlen(buffer);

	// ��Ʈ�� �����ϴ� ���� ��� �ʿ�

	m_pD21DDeviceContext->DrawTextW(buffer, wcslen(buffer), m_pTextFormat,
		D2D1::RectF(pos.x, pos.y, (pos.x + 500), (pos.y + 40)),
		m_pGreenBrush);

	// ���˰��� ���� ����
	delete[] buffer;

	return true;
}

bool D2DEngine::DrawText(D2D1_POINT_2F pos, const WCHAR* pch, D2D1::ColorF color...)
{
	// ���� ��Ʈ�� ���� ����, �������� ó��

	va_list ap;			// ���� �а� �ִ� ������ ���
	va_start(ap, pch);	// ap ������ ������ ù��° �����μ��� ����Ű���� �ʱ�ȭ

	int len = _vscwprintf(pch, ap) + 1;
	WCHAR* buffer = new WCHAR[len];

	vswprintf_s(buffer, len, pch, ap);
	va_end(ap);

	//int nLen = lstrlenA(pch);
	int nLen = lstrlen(buffer);

	// ��Ʈ�� �����ϴ� ���� ��� �ʿ�

	m_pD21DDeviceContext->CreateSolidColorBrush(color, &m_pTempBrush);

	// ��Ʈ�� �����ϴ� ���� ��� �ʿ�
	m_pD21DDeviceContext->DrawTextW(buffer, wcslen(buffer), m_pTextFormat,
		D2D1::RectF(pos.x, pos.y, (pos.x + 500), (pos.y + 40)),
		m_pTempBrush);

	SafeRelease(&m_pTempBrush);

	// ���˰��� ���� ����
	delete[] buffer;

	return true;
}
