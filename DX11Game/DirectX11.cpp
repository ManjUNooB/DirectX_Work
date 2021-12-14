#include "DirectX11.h"

//--�萔��`--//
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
	//	���딻�菉����
	HRESULT hr = S_OK;

	//	�f�o�C�X�E�X���b�v�`�F�[���̍쐬
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

	//	�@�\���x����`
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

	//	�o�b�N�o�b�t�@�쐬
	hr = CreateBackBuffer(width, height);
	if (FAILED(hr)) {
		return hr;
	}

	//	���X�^���C�Y�ݒ�
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE;		// �J�����O����(���ʕ`��)
	m_pDevice->CreateRasterizerState(&rd, &m_pRs[0]);
	rd.CullMode = D3D11_CULL_FRONT;		//	�O�ʃJ�����O(���ʕ`��)
	m_pDevice->CreateRasterizerState(&rd, &m_pRs[1]);
	rd.CullMode = D3D11_CULL_BACK;		//	�w�ʃJ�����O(�\�ʕ`��)
	m_pDevice->CreateRasterizerState(&rd, &m_pRs[2]);

	//	�u�����h�X�e�[�g�쐬
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;
	BlendDesc.RenderTarget[0].BlendEnable = FALSE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_pDevice->CreateBlendState(&BlendDesc, &m_pBs[0]);

	// �u�����h �X�e�[�g���� (�A���t�@ �u�����h�p)
	//BlendDesc.AlphaToCoverageEnable = TRUE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	m_pDevice->CreateBlendState(&BlendDesc, &m_pBs[1]);
	// �u�����h �X�e�[�g���� (���Z�����p)
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	m_pDevice->CreateBlendState(&BlendDesc, &m_pBs[2]);
	// �u�����h �X�e�[�g���� (���Z�����p)
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	m_pDevice->CreateBlendState(&BlendDesc, &m_pBs[3]);
	SetBlendState(BS_ALPHABLEND);

	//	�[�x�X�e���V���X�e�[�g�쐬
	CD3D11_DEFAULT def;
	CD3D11_DEPTH_STENCIL_DESC dsd(def);
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	m_pDevice->CreateDepthStencilState(&dsd, &m_pDSS[0]);
	CD3D11_DEPTH_STENCIL_DESC dsd2(def);
	dsd2.DepthEnable = FALSE;
	m_pDevice->CreateDepthStencilState(&dsd2, &m_pDSS[1]);


	return hr;
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

//--------------------------------------//
//	�o�b�N�o�b�t�@�쐬
//--------------------------------------//
HRESULT DirectX11::CreateBackBuffer(UINT width,UINT height)
{	
	HRESULT hr = E_FAIL;

	//	�����_�[�^�[�Q�b�g�r���[�̍쐬
	ID3D11Texture2D* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
	SAFE_RELEASE(pBackBuffer);

	//	Z�o�b�t�@�p�e�N�X�`������
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

	//	Z�o�b�t�@�^�[�Q�b�g�r���[���쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	hr = m_pDevice->CreateDepthStencilView(m_pDepthStencilTexture,
		&dsvd, &m_pDepthStencilView);
	if (FAILED(hr)) {
		return hr;
	}

	//	�e�^�[�Q�b�g�r���[�������_�[�^�[�Q�b�g�ɐݒ�
	m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	//	�r���[�|�[�g�ݒ�
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

//--------------------------------------//
//	�f�o�C�X�擾
//--------------------------------------//
ID3D11Device* DirectX11::GetDevice()
{
	return m_pDevice;
}

//--------------------------------------//
//	�f�o�C�X�R���e�L�X�g�擾
//--------------------------------------//
ID3D11DeviceContext* DirectX11::GetContext()
{
	return m_pContext;
}

//--------------------------------------//
//	�u�����h�X�e�[�g�̐ݒ�
//--------------------------------------//
void DirectX11::SetBlendState(int nBlendState)
{
	if (nBlendState >= 0 && nBlendState < MAX_BLENDSTATE) {
		float blendFactor[4] = { 0.0f,0.0f,0.0f,0.0f };
		m_pContext->OMSetBlendState(m_pBs[nBlendState],blendFactor,0xffffffff);
	}
}

