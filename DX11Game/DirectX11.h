#ifndef __DIRECT_X_11_H_
#define __DIRECT_X_11_H_


//--include--//
#include <d3d11.h>

//--ライブラリリンク--//
#pragma comment(lib,"d3d11")

//--マクロ定義--//
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) do{if(p){p->Release(); p = nullptr;}}while(0)
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(p) do{if(p){delete p; p = nullptr;}}while(0)
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) do{if(p){delete[] p; p = nullptr;}}while(0)
#endif

//--定数定義--//
constexpr int DssNum = 2;

enum EBlendState {
	BS_NONE = 0,		//半透明合成無し
	BS_ALPHABLEND,		//半透明合成
	BS_ADDITIVE,		//加算合成
	BS_SUBTRACTION,		//減算合成

	MAX_BLENDSTATE
	
};

enum ECullMode {
	CULLMODE_NONE = 0,	//	カリングしない
	CULLMODE_CW,		//	前面カリング
	CULLMODE_CCW,		//	背面カリング

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
	ID3D11Device* m_pDevice;						//	デバイス
	ID3D11DeviceContext* m_pContext;				//	デバイスコンテキスト
	IDXGISwapChain* m_pSwapChain;					//	スワップチェーン
	ID3D11RenderTargetView* m_pRenderTargetView;	//	フレームバッファ
	ID3D11Texture2D* m_pDepthStencilTexture;		//	Zバッファ用メモリ
	ID3D11DepthStencilView* m_pDepthStencilView;	//	Zバッファ
	UINT m_uSyncInterval = 0;						//	垂直同期(0:無,1:有)
	ID3D11RasterizerState* m_pRs[MAX_CULLMODE];		//	ラスタライズステート
	ID3D11BlendState* m_pBs[MAX_BLENDSTATE];		//	ブレンドステート
	ID3D11DepthStencilState* m_pDSS[DssNum];		//	Z/ステンシルステート

};

#endif	//DIRECT_X_11_H