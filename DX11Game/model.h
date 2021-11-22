//=============================================================================
//
// ���f������ [model.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
void SetModelCamera(CCamera* pCamera);
void SetModelLight(CLight* pLight);