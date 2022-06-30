//************************************************************************************
// 
// 遊び方処理[HowToPlay.cpp]
// 編集者：伊地田真衣
// 
//************************************************************************************

//-------------------- インクルード部 --------------------
#include "HowToPlay.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
#include "Scene.h"
#include "Sound.h"


//-------------------- 定数定義 --------------------
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

//-------------------- グローバル変数定義 --------------------
static LPCWSTR g_pszTexFName[MAX_TEXTURE] = {
	L"data/texture/bg000.jpg",
	L"data/texture/howtoplay.png",
};

static ID3D11ShaderResourceView *g_pTexture[MAX_TEXTURE];


//====================================================================================
//
//				初期化
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

	// BGM再生
	CSound::Play(BGM_000);

	return hr;
}

//====================================================================================
//
//				終了
//
//====================================================================================
void	UninitHowToPlay() {
	// BGM再生停止
	CSound::Stop(BGM_000);
	// 背景テクスチャ開放
	for (int i = 0; i < MAX_TEXTURE; i++) {
		SAFE_RELEASE(g_pTexture[i]);
	}
}

//====================================================================================
//
//				更新
//
//====================================================================================
void	UpdateHowToPlay() {
	//クリックまたは[Enter]押下
	if (GetMouseRelease(MOUSEBUTTON_L) || GetKeyRelease(VK_RETURN)) {
		CSound::Play(SE_DECIDE_1);
		//モード選択画面へ
		SetScene(SCENE_MODE);
		return;
	}
}

//====================================================================================
//
//				描画
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
