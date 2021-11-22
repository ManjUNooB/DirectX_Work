//=============================================================================
//
// ���f������ [model.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include <time.h>
#include "AssimpModel.h"
#include "model.h"
#include "main.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ANIM_IDLE			(0)
#define ANIM_JUMP			(1)
#define ANIM_KICK			(2)
#define ANIM_RUN			(3)
#define ANIM_SLASH			(4)
#define ANIM_WALK			(5)

#define	PLAYER_RADIUS		(50.0f)							// ���a
#define	VALUE_MOVE_PLAYER	(0.155f)						// �ړ����x
#define	RATE_MOVE_PLAYER	(0.025f)						// �ړ������W��
#define	VALUE_ROTATE_PLAYER	(0.079f)							// ��]���x(Degree)
#define	RATE_ROTATE_PLAYER	(0.10f)							// ��]�����W��

//*****************************************************************************
// �O���[�o���ϐ�
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
static CAssimpModel		g_model;	// FBX�f�[�^

static int g_nAnimStack;
static double g_dAnimTime;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitModel(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_sclModel = XMFLOAT3(1.0f, 1.0f, 1.0f);

	// ���f���f�[�^�̓ǂݍ���
	if (!g_model.Load(pDevice, pDeviceContext, g_pszModelPath)) {
		hr = E_FAIL;
		MessageBoxA(GetMainWnd(), "���f���f�[�^�Ǎ��G���[", "InitModel", MB_OK | MB_ICONEXCLAMATION);
	}
	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitModel(void)
{
	// ���f���̉��
	g_model.Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateModel(void)
{
	XMMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate;

	// �J�����̌����擾
	XMFLOAT3 rotCamera = XMFLOAT3(0, 0, 0);//GetCameraAngle();

	if (GetKeyPress(VK_LEFT)) {
		if (GetKeyPress(VK_UP)) {
			// ���O�ړ�
			g_moveModel.x -= sinf(rotCamera.y + XM_PI * 0.75f) * VALUE_MOVE_PLAYER;
			g_moveModel.z -= cosf(rotCamera.y + XM_PI * 0.75f) * VALUE_MOVE_PLAYER;

			g_rotDest.y = rotCamera.y + XM_PI * 0.75f;
		} else if (GetKeyPress(VK_DOWN)) {
			// ����ړ�
			g_moveModel.x -= sinf(rotCamera.y + XM_PI * 0.25f) * VALUE_MOVE_PLAYER;
			g_moveModel.z -= cosf(rotCamera.y + XM_PI * 0.25f) * VALUE_MOVE_PLAYER;

			g_rotDest.y = rotCamera.y + XM_PI * 0.25f;
		} else {
			// ���ړ�
			g_moveModel.x -= sinf(rotCamera.y + XM_PIDIV2) * VALUE_MOVE_PLAYER;
			g_moveModel.z -= cosf(rotCamera.y + XM_PIDIV2) * VALUE_MOVE_PLAYER;

			g_rotDest.y = rotCamera.y + XM_PIDIV2;
		}
	} else if (GetKeyPress(VK_RIGHT)) {
		if (GetKeyPress(VK_UP)) {
			// �E�O�ړ�
			g_moveModel.x -= sinf(rotCamera.y - XM_PI * 0.75f) * VALUE_MOVE_PLAYER;
			g_moveModel.z -= cosf(rotCamera.y - XM_PI * 0.75f) * VALUE_MOVE_PLAYER;

			g_rotDest.y = rotCamera.y - XM_PI * 0.75f;
		} else if (GetKeyPress(VK_DOWN)) {
			// �E��ړ�
			g_moveModel.x -= sinf(rotCamera.y - XM_PI * 0.25f) * VALUE_MOVE_PLAYER;
			g_moveModel.z -= cosf(rotCamera.y - XM_PI * 0.25f) * VALUE_MOVE_PLAYER;

			g_rotDest.y = rotCamera.y - XM_PI * 0.25f;
		} else {
			// �E�ړ�
			g_moveModel.x -= sinf(rotCamera.y - XM_PIDIV2) * VALUE_MOVE_PLAYER;
			g_moveModel.z -= cosf(rotCamera.y - XM_PIDIV2) * VALUE_MOVE_PLAYER;

			g_rotDest.y = rotCamera.y - XM_PIDIV2;
		}
	} else if (GetKeyPress(VK_UP)) {
		// �O�ړ�
		g_moveModel.x -= sinf(XM_PI + rotCamera.y) * VALUE_MOVE_PLAYER;
		g_moveModel.z -= cosf(XM_PI + rotCamera.y) * VALUE_MOVE_PLAYER;

		g_rotDest.y = XM_PI + rotCamera.y;
	} else if (GetKeyPress(VK_DOWN)) {
		// ��ړ�
		g_moveModel.x -= sinf(rotCamera.y) * VALUE_MOVE_PLAYER;
		g_moveModel.z -= cosf(rotCamera.y) * VALUE_MOVE_PLAYER;

		g_rotDest.y = rotCamera.y;
	}

	if (GetKeyPress(VK_J)) {
		// ����]
		g_rotDest.y -= VALUE_ROTATE_PLAYER;
		if (g_rotDest.y < -XM_PI) {
			g_rotDest.y += XM_2PI;
		}
	}
	if (GetKeyPress(VK_L)) {
		// �E��]
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

	// �ړI�̊p�x�܂ł̍���
	float fDiffRotY = g_rotDest.y - g_rotModel.y;
	if (fDiffRotY > XM_PI) {
		fDiffRotY -= XM_2PI;
	}
	if (fDiffRotY < -XM_PI) {
		fDiffRotY += XM_2PI;
	}

	// �ړI�̊p�x�܂Ŋ�����������
	g_rotModel.y += fDiffRotY * RATE_ROTATE_PLAYER;
	if (g_rotModel.y > XM_PI) {
		g_rotModel.y -= XM_2PI;
	}
	if (g_rotModel.y < -XM_PI) {
		g_rotModel.y += XM_2PI;
	}

	// �ʒu�ړ�
	g_posModel.x += g_moveModel.x;
	g_posModel.y += g_moveModel.y;
	g_posModel.z += g_moveModel.z;

	// �͈̓`�F�b�N
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

	// �ړ��ʂɊ�����������
	g_moveModel.x += (0.0f - g_moveModel.x) * RATE_MOVE_PLAYER;
	g_moveModel.y += (0.0f - g_moveModel.y) * RATE_MOVE_PLAYER;
	g_moveModel.z += (0.0f - g_moveModel.z) * RATE_MOVE_PLAYER;

	// �e�̈ʒu�ݒ�
	//SetPositionShadow(g_nIdxShadow, XMFLOAT3(g_posModel.x, 0.1f, g_posModel.z));

	// �A�j���[�V�����X�V
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

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(g_sclModel.x, g_sclModel.y, g_sclModel.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(g_rotModel.x, g_rotModel.y, g_rotModel.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(g_posModel.x, g_posModel.y, g_posModel.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�ݒ�
	XMStoreFloat4x4(&g_mtxWorld, mtxWorld);

	// �J�����A�����ݒ�
	g_model.SetCamera(g_pCamera);
	g_model.SetLight(g_pLight);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawModel(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// ���f���\��
	g_model.SetAnimIndex(g_nAnimStack);
	g_model.SetAnimTime(g_dAnimTime);
	g_model.Draw(pDeviceContext, g_mtxWorld);
}

//=============================================================================
// �J�����̐ݒ�
//=============================================================================
void SetModelCamera(CCamera* pCamera)
{
	g_pCamera = pCamera;
}

//=============================================================================
// ���C�g�̐ݒ�
//=============================================================================
void SetModelLight(CLight* pLight)
{
	g_pLight = pLight;
}
