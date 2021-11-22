//=============================================================================
//
// モデル処理 [model.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include <time.h>
#include "AssimpModel.h"
#include "model.h"
#include "main.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ANIM_IDLE			(0)
#define ANIM_JUMP			(1)
#define ANIM_KICK			(2)
#define ANIM_RUN			(3)
#define ANIM_SLASH			(4)
#define ANIM_WALK			(5)

#define	PLAYER_RADIUS		(50.0f)							// 半径
#define	VALUE_MOVE_PLAYER	(0.155f)						// 移動速度
#define	RATE_MOVE_PLAYER	(0.025f)						// 移動慣性係数
#define	VALUE_ROTATE_PLAYER	(0.079f)							// 回転速度(Degree)
#define	RATE_ROTATE_PLAYER	(0.10f)							// 回転慣性係数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CCamera*			g_pCamera;
static CLight*			g_pLight;

static XMFLOAT4X4		g_mtxWorld;

static XMFLOAT3			g_posModel;
static XMFLOAT3			g_moveModel;
static XMFLOAT3			g_rotModel;
static XMFLOAT3			g_rotDest;
static XMFLOAT3			g_sclModel;

static LPCSTR			g_pszModelPath = "data/model/Sword And Shield.fbx";
static CAssimpModel		g_model;	// FBXデータ

static int g_nAnimStack;
static double g_dAnimTime;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitModel(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// 位置・回転・スケールの初期設定
	g_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_sclModel = XMFLOAT3(1.0f, 1.0f, 1.0f);

	// モデルデータの読み込み
	if (!g_model.Load(pDevice, pDeviceContext, g_pszModelPath)) {
		hr = E_FAIL;
		MessageBoxA(GetMainWnd(), "モデルデータ読込エラー", "InitModel", MB_OK | MB_ICONEXCLAMATION);
	}
	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitModel(void)
{
	// モデルの解放
	g_model.Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateModel(void)
{
	XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

	// カメラの向き取得
	XMFLOAT3 rotCamera = XMFLOAT3(0, 0, 0);//GetCameraAngle();

	if (GetKeyPress(VK_LEFT)) {
		if (GetKeyPress(VK_UP)) {
			// 左前移動
			g_moveModel.x -= sinf(rotCamera.y + XM_PI * 0.75f) * VALUE_MOVE_PLAYER;
			g_moveModel.z -= cosf(rotCamera.y + XM_PI * 0.75f) * VALUE_MOVE_PLAYER;

			g_rotDest.y = rotCamera.y + XM_PI * 0.75f;
		} else if (GetKeyPress(VK_DOWN)) {
			// 左後移動
			g_moveModel.x -= sinf(rotCamera.y + XM_PI * 0.25f) * VALUE_MOVE_PLAYER;
			g_moveModel.z -= cosf(rotCamera.y + XM_PI * 0.25f) * VALUE_MOVE_PLAYER;

			g_rotDest.y = rotCamera.y + XM_PI * 0.25f;
		} else {
			// 左移動
			g_moveModel.x -= sinf(rotCamera.y + XM_PIDIV2) * VALUE_MOVE_PLAYER;
			g_moveModel.z -= cosf(rotCamera.y + XM_PIDIV2) * VALUE_MOVE_PLAYER;

			g_rotDest.y = rotCamera.y + XM_PIDIV2;
		}
	} else if (GetKeyPress(VK_RIGHT)) {
		if (GetKeyPress(VK_UP)) {
			// 右前移動
			g_moveModel.x -= sinf(rotCamera.y - XM_PI * 0.75f) * VALUE_MOVE_PLAYER;
			g_moveModel.z -= cosf(rotCamera.y - XM_PI * 0.75f) * VALUE_MOVE_PLAYER;

			g_rotDest.y = rotCamera.y - XM_PI * 0.75f;
		} else if (GetKeyPress(VK_DOWN)) {
			// 右後移動
			g_moveModel.x -= sinf(rotCamera.y - XM_PI * 0.25f) * VALUE_MOVE_PLAYER;
			g_moveModel.z -= cosf(rotCamera.y - XM_PI * 0.25f) * VALUE_MOVE_PLAYER;

			g_rotDest.y = rotCamera.y - XM_PI * 0.25f;
		} else {
			// 右移動
			g_moveModel.x -= sinf(rotCamera.y - XM_PIDIV2) * VALUE_MOVE_PLAYER;
			g_moveModel.z -= cosf(rotCamera.y - XM_PIDIV2) * VALUE_MOVE_PLAYER;

			g_rotDest.y = rotCamera.y - XM_PIDIV2;
		}
	} else if (GetKeyPress(VK_UP)) {
		// 前移動
		g_moveModel.x -= sinf(XM_PI + rotCamera.y) * VALUE_MOVE_PLAYER;
		g_moveModel.z -= cosf(XM_PI + rotCamera.y) * VALUE_MOVE_PLAYER;

		g_rotDest.y = XM_PI + rotCamera.y;
	} else if (GetKeyPress(VK_DOWN)) {
		// 後移動
		g_moveModel.x -= sinf(rotCamera.y) * VALUE_MOVE_PLAYER;
		g_moveModel.z -= cosf(rotCamera.y) * VALUE_MOVE_PLAYER;

		g_rotDest.y = rotCamera.y;
	}

	if (GetKeyPress(VK_J)) {
		// 左回転
		g_rotDest.y -= VALUE_ROTATE_PLAYER;
		if (g_rotDest.y < -XM_PI) {
			g_rotDest.y += XM_2PI;
		}
	}
	if (GetKeyPress(VK_L)) {
		// 右回転
		g_rotDest.y += VALUE_ROTATE_PLAYER;
		if (g_rotDest.y > XM_PI) {
			g_rotDest.y -= XM_2PI;
		}
	}

	switch (g_nAnimStack) {
	case ANIM_IDLE:
		if (GetKeyTrigger(VK_X)) {
			g_nAnimStack = ANIM_SLASH;
			g_dAnimTime = 0.0;
		}
		if (GetKeyTrigger(VK_C)) {
			g_nAnimStack = ANIM_KICK;
			g_dAnimTime = 0.0;
		}
		// THRU
	case ANIM_WALK:
	case ANIM_RUN:
		if (GetKeyTrigger(VK_Z)) {
			g_nAnimStack = ANIM_JUMP;
			g_dAnimTime = 0.0;
		}
		break;
	}

	// 目的の角度までの差分
	float fDiffRotY = g_rotDest.y - g_rotModel.y;
	if (fDiffRotY > XM_PI) {
		fDiffRotY -= XM_2PI;
	}
	if (fDiffRotY < -XM_PI) {
		fDiffRotY += XM_2PI;
	}

	// 目的の角度まで慣性をかける
	g_rotModel.y += fDiffRotY * RATE_ROTATE_PLAYER;
	if (g_rotModel.y > XM_PI) {
		g_rotModel.y -= XM_2PI;
	}
	if (g_rotModel.y < -XM_PI) {
		g_rotModel.y += XM_2PI;
	}

	// 位置移動
	g_posModel.x += g_moveModel.x;
	g_posModel.y += g_moveModel.y;
	g_posModel.z += g_moveModel.z;

	// 範囲チェック
	if (g_posModel.x < -630.0f) {
		g_posModel.x = -630.0f;
	}
	if (g_posModel.x > 630.0f) {
		g_posModel.x = 630.0f;
	}
	if (g_posModel.z > 630.0f) {
		g_posModel.z = 630.0f;
	}
	if (g_posModel.z < -630.0f) {
		g_posModel.z = -630.0f;
	}

	// 移動量に慣性をかける
	g_moveModel.x += (0.0f - g_moveModel.x) * RATE_MOVE_PLAYER;
	g_moveModel.y += (0.0f - g_moveModel.y) * RATE_MOVE_PLAYER;
	g_moveModel.z += (0.0f - g_moveModel.z) * RATE_MOVE_PLAYER;

	// 影の位置設定
	//SetPositionShadow(g_nIdxShadow, XMFLOAT3(g_posModel.x, 0.1f, g_posModel.z));

	// アニメーション更新
	float fSpeed = sqrtf(g_moveModel.x * g_moveModel.x + g_moveModel.z * g_moveModel.z);
	//PrintDebugProc("Speed=%f\n\n", fSpeed);
	int nStack = ANIM_IDLE;
	if (fSpeed > 3.0f)
		nStack = ANIM_RUN;
	else if (fSpeed > 0.2f)
		nStack = ANIM_WALK;
	switch (g_nAnimStack) {
	case ANIM_IDLE:
	case ANIM_WALK:
	case ANIM_RUN:
		if (g_nAnimStack != nStack) {
			g_nAnimStack = nStack;
			g_dAnimTime = 0.0;
		}
		break;
	}
	static double dLastTime = clock() / double(CLOCKS_PER_SEC);
	double dNowTime = clock() / double(CLOCKS_PER_SEC);
	double dSlice = dNowTime - dLastTime;
	dLastTime = dNowTime;
	g_dAnimTime += dSlice;
	if (g_dAnimTime >= g_model.GetAnimDuration(g_nAnimStack)) {
		switch (g_nAnimStack) {
		case ANIM_JUMP:
		case ANIM_SLASH:
		case ANIM_KICK:
			g_nAnimStack = nStack;
			break;
		}
		g_dAnimTime = 0.0;
	}

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// スケールを反映
	mtxScl = XMMatrixScaling(g_sclModel.x, g_sclModel.y, g_sclModel.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(g_rotModel.x, g_rotModel.y, g_rotModel.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(g_posModel.x, g_posModel.y, g_posModel.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックス設定
	XMStoreFloat4x4(&g_mtxWorld, mtxWorld);

	// カメラ、光源設定
	g_model.SetCamera(g_pCamera);
	g_model.SetLight(g_pLight);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawModel(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// モデル表示
	g_model.SetAnimIndex(g_nAnimStack);
	g_model.SetAnimTime(g_dAnimTime);
	g_model.Draw(pDeviceContext, g_mtxWorld);
}

//=============================================================================
// カメラの設定
//=============================================================================
void SetModelCamera(CCamera* pCamera)
{
	g_pCamera = pCamera;
}

//=============================================================================
// ライトの設定
//=============================================================================
void SetModelLight(CLight* pLight)
{
	g_pLight = pLight;
}
