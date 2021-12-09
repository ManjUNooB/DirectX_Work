#ifndef __DIRECT_X_11_H_
#define __DIRECT_X_11_H_


//--include--//
#include <d3d11.h>

//--���C�u���������N--//
#pragma comment(lib,"d3d11")

//--�}�N����`--//
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) do{if(p){p->Release(); p = nullptr;}}while(0)
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(p) do{if(p){delete p; p = nullptr;}}while(0)
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) do{if(p){delete[] p; p = nullptr;}}while(0)
#endif

//--�萔��`--//
constexpr int DssNum = 2;

enum EBlendState {
	BS_NONE = 0,		//��������������
	BS_ALPHABLEND,		//����������
	BS_ADDITIVE,		//���Z����
	BS_SUBTRACTION,		//���Z����

	MAX_BLENDSTATE
	
};

enum ECullMode {
	CULLMODE_NONE = 0,	//	�J�����O���Ȃ�
	CULLMODE_CW,		//	�O�ʃJ�����O
	CULLMODE_CCW,		//	�w�ʃJ�����O

	MAX_CULLMODE
};

class DirectX11
{
public:
	DirectX11();
	~DirectX11();

	HRESULT InitDX(HWND hwnd,UINT width,UINT height,bool fullscreen);
	void UninitDX();
	void DrawBeginDX();
	void DrawEndDX();

	HRESULT CreateBackBuffer(UINT width,UINT height);
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();

private:
	ID3D11Device* m_pDevice;						//	�f�o�C�X
	ID3D11DeviceContext* m_pContext;				//	�f�o�C�X�R���e�L�X�g
	IDXGISwapChain* m_pSwapChain;					//	�X���b�v�`�F�[��
	ID3D11RenderTargetView* m_pRenderTargetView;	//	�t���[���o�b�t�@
	ID3D11Texture2D* m_pDepthStencilTexture;		//	Z�o�b�t�@�p������
	ID3D11DepthStencilView* m_pDepthStencilView;	//	Z�o�b�t�@
	UINT m_uSyncInterval = 0;						//	��������(0:��,1:�L)
	ID3D11RasterizerState* m_pRs[MAX_CULLMODE];		//	���X�^���C�Y�X�e�[�g
	ID3D11BlendState* m_pBs[MAX_BLENDSTATE];		//	�u�����h�X�e�[�g
	ID3D11DepthStencilState* m_pDSS[DssNum];		//	Z/�X�e���V���X�e�[�g

};

#endif	//DIRECT_X_11_H