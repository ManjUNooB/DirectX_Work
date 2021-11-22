//=============================================================================
//
// �n�ʏ��� [field.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "field.h"
#include "Shader.h"
#include "Texture.h"
#include "input.h"
#include "mesh.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_FIELD	L"data/texture/field000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����

#define M_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR		XMFLOAT4(0.1f,0.1f,0.1f,1.0f)
#define M_AMBIENT		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define	VALUE_MOVE		(5.0f)											// �ړ���
#define	VALUE_ROTATE	(XM_PI*0.02f)									// ��]��

#define	SIZE_X			(800.0f)										// �n�ʂ̃T�C�Y(X����)
#define	SIZE_Z			(800.0f)										// �n�ʂ̃T�C�Y(Z����)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
static HRESULT MakeVertexField(ID3D11Device* pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture;				// �e�N�X�`���ւ̃|�C���^
static MESH							g_mesh;					// �\����
static MATERIAL						g_material;				// �}�e���A��

//=============================================================================
// ����������
//=============================================================================
HRESULT InitField(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	// �ʒu�E��]�̏����ݒ�
	g_mesh.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_mesh.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// �}�e���A���̏����ݒ�
	g_material.Diffuse = M_DIFFUSE;
	g_material.Ambient = M_AMBIENT;
	g_material.Specular = M_SPECULAR;
	g_material.Power = 1.0f;
	g_material.Emissive = M_EMISSIVE;
	g_mesh.pMaterial = &g_material;

	// �e�N�X�`���̓ǂݍ���
	hr = CreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
							   TEXTURE_FIELD,		// �t�@�C���̖��O
							   &g_mesh.pTexture);	// �ǂݍ��ރ������[
	if (FAILED(hr))
		return hr;
	XMStoreFloat4x4(&g_mesh.mtxTexture, XMMatrixIdentity());

	// ���_���̍쐬
	hr = MakeVertexField(pDevice);

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitField(void)
{
	// ���b�V�����
	ReleaseMesh(&g_mesh);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateField(void)
{
	// ���b�V���X�V
	UpdateMesh(&g_mesh);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawField(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// ���b�V���̕`��
	DrawMesh(pDeviceContext, &g_mesh);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexField(ID3D11Device* pDevice)
{
	VERTEX_3D	vertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	int			indexWk[NUM_VERTEX];	// �C���f�b�N�X�i�[���[�N

	// ���_���W�̐ݒ�
	g_mesh.nNumVertex = NUM_VERTEX;
	vertexWk[0].vtx = XMFLOAT3(-SIZE_X, 0.0f,  SIZE_Z);
	vertexWk[1].vtx = XMFLOAT3( SIZE_X, 0.0f,  SIZE_Z);
	vertexWk[2].vtx = XMFLOAT3(-SIZE_X, 0.0f, -SIZE_Z);
	vertexWk[3].vtx = XMFLOAT3( SIZE_X, 0.0f, -SIZE_Z);

	// �@���x�N�g���̐ݒ�
	vertexWk[0].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertexWk[1].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertexWk[2].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertexWk[3].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);

	// �g�U���ˌ��̐ݒ�
	vertexWk[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertexWk[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertexWk[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertexWk[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	vertexWk[0].tex = XMFLOAT2(0.0f, 0.0f);
	vertexWk[1].tex = XMFLOAT2(8.0f, 0.0f);
	vertexWk[2].tex = XMFLOAT2(0.0f, 8.0f);
	vertexWk[3].tex = XMFLOAT2(8.0f, 8.0f);

	// �C���f�b�N�X�z��̐ݒ�
	g_mesh.nNumIndex = NUM_VERTEX;
	indexWk[0] = 0;
	indexWk[1] = 1;
	indexWk[2] = 2;
	indexWk[3] = 3;

	return MakeMeshVertex(pDevice, &g_mesh, vertexWk, indexWk);
}
