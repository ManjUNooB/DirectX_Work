#include "Camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	POS_X_CAM		(0.0f)										// カメラの初期位置(X座標)
#define	POS_Y_CAM		(200.0f)									// カメラの初期位置(Y座標)
#define	POS_Z_CAM		(-700.0f)									// カメラの初期位置(Z座標)
#define	VIEW_ANGLE		(45.0f)										// ビュー平面の視野角
#define	VIEW_ASPECT		((float)SCREEN_WIDTH/(float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比	
#define	VIEW_NEAR_Z		(10.0f)										// ビュー平面のNearZ値
#define	VIEW_FAR_Z		(30000.0f)									// ビュー平面のFarZ値

// コンストラクタ
CCamera::CCamera()
{
	Init();
}

// 初期化
void CCamera::Init()
{
	m_vPos = XMFLOAT3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);	// 視点
	m_vTarget = XMFLOAT3(0.0f, 50.0f, 0.0f);			// 注視点
	m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);					// 上方ベクトル

	m_fAspectRatio = VIEW_ASPECT;			// 縦横比
	m_fFovY = VIEW_ANGLE;					// 視野角(Degree)
	m_fNearZ = VIEW_NEAR_Z;					// 前方クリップ距離
	m_fFarZ = VIEW_FAR_Z;					// 後方クリップ距離
}

// ビュー/プロジェクション マトリックス更新
void CCamera::Update()
{
	XMStoreFloat4x4(&m_mtxView, XMMatrixLookAtLH(
		XMLoadFloat3(&m_vPos), XMLoadFloat3(&m_vTarget), XMLoadFloat3(&m_vUp)));
	XMStoreFloat4x4(&m_mtxProj, XMMatrixPerspectiveFovLH(
		XMConvertToRadians(m_fFovY), m_fAspectRatio, m_fNearZ, m_fFarZ));
}

// ワールド マトリックス設定
void CCamera::SetWorldMatrix(XMFLOAT4X4& mtxWorld)
{
	m_mtxWorld = mtxWorld;
	m_vPos = XMFLOAT3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
	m_vTarget = XMFLOAT3(mtxWorld._41 + mtxWorld._31, mtxWorld._42 + mtxWorld._32, mtxWorld._43 + mtxWorld._33);
	m_vUp = XMFLOAT3(mtxWorld._21, mtxWorld._22, mtxWorld._23);
}

// 視点/注視点/上方ベクトルからワールド マトリックス算出
XMFLOAT4X4& CCamera::CalcWorldMatrix()
{
	XMVECTOR vecZ = XMVectorSet(m_vTarget.x - m_vPos.x, m_vTarget.y - m_vPos.y, m_vTarget.z - m_vPos.z, 0.0f);
	XMFLOAT3 vZ;
	XMStoreFloat3(&vZ, XMVector3Normalize(vecZ));
	XMVECTOR vecY = XMLoadFloat3(&m_vUp);
	XMVECTOR vecX = XMVector3Normalize(XMVector3Cross(vecY, vecZ));
	XMFLOAT3 vX;
	XMStoreFloat3(&vX, vecX);
	vecY = XMVector3Normalize(XMVector3Cross(vecZ, vecX));
	XMFLOAT3 vY;
	XMStoreFloat3(&vY, vecY);

	m_mtxWorld._11 = vX.x;
	m_mtxWorld._12 = vX.y;
	m_mtxWorld._13 = vX.z;
	m_mtxWorld._14 = 0.0f;
	m_mtxWorld._21 = vY.x;
	m_mtxWorld._22 = vY.y;
	m_mtxWorld._23 = vY.z;
	m_mtxWorld._24 = 0.0f;
	m_mtxWorld._31 = vZ.x;
	m_mtxWorld._32 = vZ.y;
	m_mtxWorld._33 = vZ.z;
	m_mtxWorld._34 = 0.0f;
	m_mtxWorld._41 = m_vPos.x;
	m_mtxWorld._42 = m_vPos.y;
	m_mtxWorld._43 = m_vPos.z;
	m_mtxWorld._44 = 1.0f;

	return m_mtxWorld;
}
