#include "DirectX11.h"

//--定数定義--//
constexpr int BufferCount = 1;
constexpr int RefreshRate_Numerator = 60;
constexpr int RefreshRate_Denominator = 1;
constexpr int SampleDesc_Count = 1;
constexpr int SampleDesc_Quility = 0;

DirectX11::DirectX11()
{
}

DirectX11::~DirectX11()
{
}



HRESULT DirectX11::InitDX(HWND hwnd, UINT width, UINT height, bool fullscreen)
{
	//	正誤判定初期化
	HRESULT hr = E_FAIL;

	//	デバイス・スワップチェーンの作成
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));
	scd.BufferCount = BufferCount;
	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = RefreshRate_Numerator;
	scd.BufferDesc.RefreshRate.Denominator = RefreshRate_Denominator;
	scd.OutputWindow = hwnd;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.Windowed = fullscreen;

	//	機能レベル定義
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, 0, featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, &scd,
		&m_pSwapChain, &m_pDevice, nullptr, &m_pContext);
	if (FAILED(hr)) {
		return hr;
	}

	//	バックバッファ作成
	hr = CreateBackBuffer(width, height);
	if (FAILED(hr)) {
		return hr;
	}



	return E_NOTIMPL;
}

void DirectX11::UninitDX()
{
}

void DirectX11::DrawBeginDX()
{
}

void DirectX11::DrawEndDX()
{
}

HRESULT DirectX11::CreateBackBuffer(UINT width,UINT height)
{	
	HRESULT hr = E_FAIL;

	//	レンダーターゲットビューの作成
	ID3D11Texture2D* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
	SAFE_RELEASE(pBackBuffer);

	//	Zバッファ用テクスチャ生成
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = width;
	td.Height = height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc.Count = SampleDesc_Count;
	td.SampleDesc.Quality = SampleDesc_Quility;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	hr = m_pDevice->CreateTexture2D(&td, nullptr, &m_pDepthStencilTexture);
	if (FAILED(hr)) {
		return hr;
	}

	//	Zバッファターゲットビューを作成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	hr = m_pDevice->CreateDepthStencilView(m_pDepthStencilTexture,
		&dsvd, &m_pDepthStencilView);
	if (FAILED(hr)) {
		return hr;
	}

	//	各ターゲットビューをレンダーターゲットに設定
	m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	//	ビューポート設定
	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pContext->RSSetViewports(1, &vp);

	return S_OK;
}

ID3D11Device* DirectX11::GetDevice()
{
	return m_pDevice;
}

ID3D11DeviceContext* DirectX11::GetContext()
{
	return m_pContext;
}

