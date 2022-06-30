//************************************************************************************
// 
// �^�C�g������[Title.cpp]
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

//-------------------- �C���N���[�h�� --------------------
#include "title.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
#include "Scene.h"
#include "Sound.h"

//-------------------- �萔��` --------------------
#define TITLE_POS_X			0.0f
#define TITLE_POS_Y			0.0f
#define TITLE_WIDTH			SCREEN_WIDTH
#define TITLE_HEIGHT		SCREEN_HEIGHT

#define LOGO_POS_X			0.0f			
#define LOGO_POS_Y			0.0f
#define LOGO_WIDTH			647
#define LOGO_HEIGHT			640

#define CS_POS_X			(0.0f)
#define CS_POS_Y			(-250.0f)
#define CS_WIDTH			(900 / 2)
#define CS_HEIGHT			(160 / 2)


enum TEXNUM {
	TEX_BG = 0,
	TEX_LOGO,
	TEX_CLICKSTART,

	MAX_TEXTURE,
};

//-------------------- �O���[�o���ϐ���` --------------------
static LPCWSTR g_pszTexFName[MAX_TEXTURE] = {
	L"data/texture/bg000.jpg",
	L"data/texture/Titlerogo.png",
	L"data/texture/ClickToStart.png",
};

static ID3D11ShaderResourceView *g_pTexture[MAX_TEXTURE];

//====================================================================================
//
//				������
//
//====================================================================================
HRESULT	InitTitle() {
	HRESULT hr = S_OK;
	ID3D11Device *pDevice = GetDevice();
	for (int i = 0; i < MAX_TEXTURE; i++) {
		hr = CreateTextureFromFile(pDevice, g_pszTexFName[i], &g_pTexture[i]);
		if (FAILED(hr)) {
			return hr;
		}
	}


	// BGM�Đ�
	CSound::Play(BGM_000);

	return hr;
}

//====================================================================================
//
//				�I��
//
//====================================================================================
void UninitTitle() {

	// BGM�Đ���~
	CSound::Stop(BGM_000);
	// �w�i�e�N�X�`���J��
	for (int i = 0; i < MAX_TEXTURE; i++) {
		SAFE_RELEASE(g_pTexture[i]);
	}
}

//====================================================================================
//
//				�X�V
//
//====================================================================================
void UpdateTitle() {
	//�N���b�N�܂���[Enter]����
	if (GetMouseRelease(MOUSEBUTTON_L) || GetKeyRelease(VK_RETURN)) {
		CSound::Play(SE_DECIDE_1);
		//���[�h�I����ʂ�
		SetScene(SCENE_MODE);
		return;
	}
}

//====================================================================================
//
//				�`��
//
//====================================================================================
void DrawTitle() {
	ID3D11DeviceContext *pDC = GetDeviceContext();
	SetPolygonSize(TITLE_WIDTH, TITLE_HEIGHT);
	SetPolygonPos(TITLE_POS_X, TITLE_POS_Y);
	SetPolygonTexture(g_pTexture[TEX_BG]);
	DrawPolygon(pDC);

	SetPolygonSize(LOGO_WIDTH, LOGO_HEIGHT);
	SetPolygonPos(LOGO_POS_X, LOGO_POS_Y);
	SetPolygonTexture(g_pTexture[TEX_LOGO]);
	DrawPolygon(pDC);

	SetPolygonSize(CS_WIDTH, CS_HEIGHT);
	SetPolygonPos(CS_POS_X, CS_POS_Y);
	SetPolygonTexture(g_pTexture[TEX_CLICKSTART]);
	DrawPolygon(pDC);
}