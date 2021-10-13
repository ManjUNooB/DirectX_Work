#include "Light.h"

using namespace DirectX;

// マクロ定義
#define LIGHT0_DIRECTION	XMFLOAT3(-1.0f, -1.0f, 1.0f)
#define LIGHT0_DIFFUSE		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
#define LIGHT0_AMBIENT		XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f)
#define LIGHT0_SPECULAR		XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f)

// コンストラクタ
CLight::CLight()
{
	Init();
}

// 初期化
void CLight::Init()
{
	XMFLOAT3 vDir = LIGHT0_DIRECTION;
	XMStoreFloat3(&m_direction, XMVector3Normalize(XMLoadFloat3(&vDir)));
	m_diffuse = LIGHT0_DIFFUSE;
	m_ambient = LIGHT0_AMBIENT;
	m_specular = LIGHT0_SPECULAR;
}
