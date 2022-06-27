#include "SelectMode.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
#include "Scene.h"
#include "Sound.h"

#define TITLE_POS_X			(0.0f)
#define TITLE_POS_Y			(0.0f)
#define TITLE_WIDTH			(SCREEN_WIDTH)
#define TITLE_HEIGHT		(SCREEN_HEIGHT)

#define LOGOS_POS_X			(0.0f)			
#define LOGOS_POS_Y			(300.0f)
#define LOGOS_WIDTH			(340)
#define LOGOS_HEIGHT		(120)

#define LOGOT_POS_X			(0.0f)			
#define LOGOT_POS_Y			(180.0f)
#define LOGOT_WIDTH			(255)
#define LOGOT_HEIGHT		(115)

#define LOGOSN_POS_X		(0.0f)			
#define LOGOSN_POS_Y		(65.0f)
#define LOGOSN_WIDTH		(255)
#define LOGOSN_HEIGHT		(115)

#define LOGOR_POS_X			(0.0f)			
#define LOGOR_POS_Y			(-55.0f)
#define LOGOR_WIDTH			(340)
#define LOGOR_HEIGHT		(120)

#define LOGOH_POS_X			(0.0f)			
#define LOGOH_POS_Y			(-175.0f)
#define LOGOH_WIDTH			(340)
#define LOGOH_HEIGHT		(120)



enum GAMEMODE {
	TURTLE = 0,
	SNAKE,
};

enum TEXNUM {
	TEX_BG = 0,
	TEX_LOGO_GAME,
	TEX_LOGO_T,
	TEX_LOGO_S,
	TEX_RANKING,
	TEX_HOWTOPLAY,

	MAX_TEXTURE,
};

static LPCWSTR g_pszTexFName[MAX_TEXTURE] = {
	L"data/texture/bg000.jpg",
	L"data/texture/shanghaibutton.png",
	L"data/texture/turtle.png",
	L"data/texture/snake.png",
	L"data/texture/rankingbutton.png",
	L"data/texture/HTPbutton.png",
};

static ID3D11ShaderResourceView *g_pTexture[MAX_TEXTURE];
static int g_nMode;

HRESULT	InitMode() {
	HRESULT hr = S_OK;
	ID3D11Device *pDevice = GetDevice();
	for (int i = 0; i < MAX_TEXTURE; i++) {
		hr = CreateTextureFromFile(pDevice, g_pszTexFName[i], &g_pTexture[i]);
		if (FAILED(hr)) {
			return hr;
		}
	}
	
	g_nMode = TURTLE;


	// BGM再生
	CSound::Play(BGM_000);

	return hr;
}

// 終了
void	UninitMode() {
	// BGM再生停止
	CSound::Stop(BGM_000);
	// 背景テクスチャ開放
	for (int i = 0; i < MAX_TEXTURE; i++) {
		SAFE_RELEASE(g_pTexture[i]);
	}

}

void	UpdateMode() {
	//----- 亀を選択 -----
	if (LOGOT_POS_X - LOGOT_WIDTH / 2 <= GetMousePosition()->x - SCREEN_WIDTH / 2 &&
		GetMousePosition()->x - SCREEN_WIDTH / 2 <= LOGOT_POS_X + LOGOT_WIDTH / 2) {
		if (LOGOT_POS_Y + LOGOT_HEIGHT / 2 >= (GetMousePosition()->y - SCREEN_HEIGHT / 2) * -1 && (GetMousePosition()->y - SCREEN_HEIGHT / 2)*-1 >= LOGOT_POS_Y - LOGOT_HEIGHT / 2) {
			// カーソルが牌の上にあって左クリックされたとき
			if (GetMouseRelease(MOUSEBUTTON_L)) {
				g_nMode = TURTLE;	// 亀を選択
				//g_nMode = SNAKE;
				CSound::Play(SE_DECIDE_1);
				//ゲーム画面へ
				SetScene(SCENE_GAME);
				return;
			}
		}
	}

	//----- 蛇を選択 -----
	if (LOGOSN_POS_X - LOGOSN_WIDTH / 2 <= GetMousePosition()->x - SCREEN_WIDTH / 2 &&
		GetMousePosition()->x - SCREEN_WIDTH / 2 <= LOGOSN_POS_X + LOGOSN_WIDTH / 2) {
		if (LOGOSN_POS_Y + LOGOSN_HEIGHT / 2 >= (GetMousePosition()->y - SCREEN_HEIGHT / 2) * -1 && (GetMousePosition()->y - SCREEN_HEIGHT / 2) * -1 >= LOGOSN_POS_Y - LOGOSN_HEIGHT / 2) {
			// カーソルが牌の上にあって左クリックされたとき
			if (GetMouseRelease(MOUSEBUTTON_L)) {
				g_nMode = SNAKE;	// 蛇を選択
				CSound::Play(SE_DECIDE_1);
				//ゲーム画面へ
				SetScene(SCENE_GAME);
				return;
			}
		}
	}

	//----- ランキングを選択 -----
	if (LOGOR_POS_X - LOGOR_WIDTH / 2 <= GetMousePosition()->x - SCREEN_WIDTH / 2 && GetMousePosition()->x - SCREEN_WIDTH / 2 <= LOGOR_POS_X + LOGOR_WIDTH / 2) {
		if (LOGOR_POS_Y + LOGOR_HEIGHT / 2 >= (GetMousePosition()->y - SCREEN_HEIGHT / 2) * -1 && (GetMousePosition()->y - SCREEN_HEIGHT / 2)*-1 >= LOGOR_POS_Y - LOGOR_HEIGHT / 2) {
			// カーソルが牌の上にあって左クリックされたとき
			if (GetMouseRelease(MOUSEBUTTON_L)) {
				CSound::Play(SE_DECIDE_1);
				//ゲーム画面へ
				SetScene(SCENE_RANKING);
				return;
			}
		}
	}

	//----- 遊び方を選択 -----
	if (LOGOH_POS_X - LOGOH_WIDTH / 2 <= GetMousePosition()->x - SCREEN_WIDTH / 2 && GetMousePosition()->x - SCREEN_WIDTH / 2 <= LOGOH_POS_X + LOGOH_WIDTH / 2) {
		if (LOGOH_POS_Y + LOGOH_HEIGHT / 2 >= (GetMousePosition()->y - SCREEN_HEIGHT / 2) * -1 && (GetMousePosition()->y - SCREEN_HEIGHT / 2)*-1 >= LOGOH_POS_Y - LOGOH_HEIGHT / 2) {
			// カーソルが牌の上にあって左クリックされたとき
			if (GetMouseRelease(MOUSEBUTTON_L)) {
				CSound::Play(SE_DECIDE_1);
				//ゲーム画面へ
				SetScene(SCENE_HOWTOPLAY);
				return;
			}
		}
	}


}

void	DrawMode() {
	ID3D11DeviceContext *pDC = GetDeviceContext();
	SetPolygonSize(TITLE_WIDTH, TITLE_HEIGHT);
	SetPolygonPos(TITLE_POS_X, TITLE_POS_Y);
	SetPolygonTexture(g_pTexture[TEX_BG]);
	DrawPolygon(pDC);

	SetPolygonSize(LOGOS_WIDTH, LOGOS_HEIGHT);
	SetPolygonPos(LOGOS_POS_X, LOGOS_POS_Y);
	SetPolygonTexture(g_pTexture[TEX_LOGO_GAME]);
	DrawPolygon(pDC);

	SetPolygonSize(LOGOT_WIDTH, LOGOT_HEIGHT);
	SetPolygonPos(LOGOT_POS_X, LOGOT_POS_Y);
	SetPolygonTexture(g_pTexture[TEX_LOGO_T]);
	DrawPolygon(pDC);

	SetPolygonSize(LOGOSN_WIDTH, LOGOSN_HEIGHT);
	SetPolygonPos(LOGOSN_POS_X, LOGOSN_POS_Y);
	SetPolygonTexture(g_pTexture[TEX_LOGO_S]);
	DrawPolygon(pDC);

	SetPolygonSize(LOGOR_WIDTH, LOGOR_HEIGHT);
	SetPolygonPos(LOGOR_POS_X, LOGOR_POS_Y);
	SetPolygonTexture(g_pTexture[TEX_RANKING]);
	DrawPolygon(pDC);

	SetPolygonSize(LOGOH_WIDTH, LOGOH_HEIGHT);
	SetPolygonPos(LOGOH_POS_X, LOGOH_POS_Y);
	SetPolygonTexture(g_pTexture[TEX_HOWTOPLAY]);
	DrawPolygon(pDC);


}

int	GetMode() {
	return g_nMode;
}
