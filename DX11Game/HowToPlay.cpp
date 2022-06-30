//************************************************************************************
// 
// �V�ѕ�����[HowToPlay.cpp]
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************

//-------------------- �C���N���[�h�� --------------------
#include "HowToPlay.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
#include "Scene.h"
#include "Sound.h"


//-------------------- �萔��` --------------------
#define BG_POS_X		0.0f
#define BG_POS_Y		0.0f
#define BG_WIDTH		SCREEN_WIDTH
#define BG_HEIGHT		SCREEN_HEIGHT

#define HTP_POS_X		0.0f
#define HTP_POS_Y		0.0f
#define HTP_WIDTH		SCREEN_WIDTH
#define HTP_HEIGHT		SCREEN_HEIGHT

enum TEXNUM {
	TEX_BG = 0,
	TEX_HOWTOPLAY,

	MAX_TEXTURE,
};

//-------------------- �O���[�o���ϐ���` --------------------
static LPCWSTR g_pszTexFName[MAX_TEXTURE] = {
	L"data/texture/bg000.jpg",
	L"data/texture/howtoplay.png",
};

static ID3D11ShaderResourceView *g_pTexture[MAX_TEXTURE];


//====================================================================================
//
//				������
//
//====================================================================================
HRESULT	InitHowToPlay() {
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
void	UninitHowToPlay() {
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
void	UpdateHowToPlay() {
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
void	DrawHowToPlay() {
	ID3D11DeviceContext *pDC = GetDeviceContext();
	SetPolygonSize(BG_WIDTH, BG_HEIGHT);
	SetPolygonPos(BG_POS_X, BG_POS_Y);
	SetPolygonTexture(g_pTexture[TEX_BG]);
	DrawPolygon(pDC);

	SetPolygonSize(HTP_WIDTH, HTP_HEIGHT);
	SetPolygonPos(HTP_POS_X, HTP_POS_Y);
	SetPolygonTexture(g_pTexture[TEX_HOWTOPLAY]);
	DrawPolygon(pDC);
}
