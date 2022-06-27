#include "polygon.h"
#include "Texture.h"

#include "UI.h"
#include "Majanpai.h"
#include "number.h"


#define PATH_BOX			L"data/texture/UIbox.png"				// 画像パス
#define BOX_SIZE_X			(309.0f * 0.95f)
#define BOX_SIZE_Y			(380.0f)
#define BOX_POS_X			(0 - SCREEN_WIDTH / 2 + BOX_SIZE_X / 2)
#define BOX_POS_Y			(SCREEN_HEIGHT / 2 - BOX_SIZE_Y / 2)

#define NUMBER_CHRCNT		(3)
#define NUMBER_POS_X		(BOX_POS_X - NUMBER_CHRCNT * NUMBER_SIZE_X * 0.5)
#define TILESUM_POS_Y		(290.0f)
#define FREECNT_POS_Y		(170.0f)
#define DELTILESUM_POS_Y	(60.0f)


//----- グローバル変数宣言 -----
static ID3D11ShaderResourceView *g_pTexture;


//------------------------------------------------------------------------------------
//
//				初期化
//
//------------------------------------------------------------------------------------
HRESULT	InitUI() {
	HRESULT hr = S_OK;
	ID3D11Device *pDevice = GetDevice();
	hr = CreateTextureFromFile(pDevice, PATH_BOX, &g_pTexture);

	return hr;
}


//------------------------------------------------------------------------------------
//
//				終了
//
//------------------------------------------------------------------------------------
void	UninitUI() {
	// テクスチャ開放
	SAFE_RELEASE(g_pTexture);
}


//------------------------------------------------------------------------------------
//
//				　描画
//
//------------------------------------------------------------------------------------
void	DrawUI() {
	ID3D11DeviceContext *pDC = GetDeviceContext();

	SetPolygonSize(BOX_SIZE_X, BOX_SIZE_Y);
	SetPolygonPos(BOX_POS_X, BOX_POS_Y);
	SetPolygonTexture(g_pTexture);
	DrawPolygon(pDC);

	DrawNumber(XMFLOAT2(NUMBER_POS_X, FREECNT_POS_Y), (unsigned)GetFreeCnt() / 2, NUMBER_CHRCNT);
	DrawNumber(XMFLOAT2(NUMBER_POS_X, TILESUM_POS_Y), (unsigned)GetTilesSum(), NUMBER_CHRCNT);
	DrawNumber(XMFLOAT2(NUMBER_POS_X, DELTILESUM_POS_Y), (unsigned)GetDelTilesSum(), NUMBER_CHRCNT);
}
