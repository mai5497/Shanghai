//----- �C���N���[�h�� -----
#include "ranking.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
#include "Sound.h"
#include "number.h"
#include "bg.h"
#include "Scene.h"
#include "Gamedata.h"


//----- �}�N����` -----
#define RANKING_POS_X		(0.0f)
#define RANKING_POS_Y		(0.0f)
#define RANKING_WIDTH		(SCREEN_WIDTH)
#define RANKING_HEIGHT		(SCREEN_HEIGHT)

#define LOGO_POS_X			(0.0f)			
#define LOGO_POS_Y			(260.0f)
#define LOGO_WIDTH			(1008 / 2)
#define LOGO_HEIGHT			(320 / 2)

#define CC_POS_X			(0.0f)
#define CC_POS_Y			(-300.0f)
#define CC_WIDTH			(900 / 2)
#define CC_HEIGHT			(160 / 2)

#define MIN_CHRCNT			(2)												// ��������
#define SEC_CHRCNT			(2)												// �b������

#define MINCHR_POS_X		(0.0f - NUMBER_SIZE_X * 0.5)															// �������ʒu
#define MINNUM_POS_X		(MINCHR_POS_X - MIN_CHRCNT * NUMBER_SIZE_X - NUMBER_SIZE_X / 2)	// �������ʒu
#define SECNUM_POS_X		(MINCHR_POS_X + NUMBER_SIZE_X - NUMBER_SIZE_X / 2)
#define SECCHR_POS_X		(SECNUM_POS_X + SEC_CHRCNT * NUMBER_SIZE_X + NUMBER_SIZE_X / 2)

enum TEXNUM {
	TEX_BG = 0,
	TEX_LOGO,
	TEX_TIMER,
	TEX_CLICKCONTINUE,

	MAX_TEXTURE,
};

enum TIMERROGONUM {
	MIN = 10,
	SEC,
};


//----- �O���[�o���ϐ��錾 -----
static int g_nTime[MAX_RANKING];

static LPCWSTR g_pszTexFName[MAX_TEXTURE] = {
	L"data/texture/bg000.jpg",
	L"data/texture/ranking.png",
	L"data/texture/number001.png",
	L"data/texture/ClickToContinue.png",
};

static ID3D11ShaderResourceView *g_pTexture[MAX_TEXTURE];



//------------------------------------------------------------------------------------
//
//				������
//
//------------------------------------------------------------------------------------
HRESULT	InitRanking() {
	HRESULT hr = S_OK;
	ID3D11Device *pDevice = GetDevice();

	for (int i = 0; i < MAX_TEXTURE; i++) {
		hr = CreateTextureFromFile(pDevice, g_pszTexFName[i], &g_pTexture[i]);
		if (FAILED(hr)) {
			return hr;
		}
	}

	// BGM�Đ�
	CSound::Play(BGM_002);

	return hr;
}
//------------------------------------------------------------------------------------
//
//				�f�[�^�̓ǂݍ���
//
//------------------------------------------------------------------------------------
void	InitRankData() {
	for (int i = 0; i < MAX_RANKING; i++) {
		g_nTime[i] = *(LoadRankData() + i);
	}
}



//------------------------------------------------------------------------------------
//
//				�I��
//
//------------------------------------------------------------------------------------
void UninitRanking() {
	// BGM�Đ���~
	CSound::Stop(BGM_002);

	for (int i = 0; i < MAX_TEXTURE; i++) {
		SAFE_RELEASE(g_pTexture[i]);
	}
}


//------------------------------------------------------------------------------------
//
//				�X�V
//
//------------------------------------------------------------------------------------
void UpdateRanking() {
	// �N���b�N�܂���[Enter]����
	if (GetMouseRelease(MOUSEBUTTON_L) || GetKeyRelease(VK_RETURN)) {
		CSound::Play(SE_DECIDE_2);
		//���[�h�I����ʂ�
		SetScene(SCENE_MODE);
		return;
	}

}


//------------------------------------------------------------------------------------
//
//				�`��
//
//------------------------------------------------------------------------------------
void DrawRanking() {
	int min;
	int sec;
	ID3D11DeviceContext *pDC = GetDeviceContext();

	SetPolygonSize(RANKING_WIDTH, RANKING_HEIGHT);
	SetPolygonPos(RANKING_POS_X, RANKING_POS_Y);
	SetPolygonTexture(g_pTexture[TEX_BG]);
	DrawPolygon(pDC);

	SetPolygonSize(LOGO_WIDTH, LOGO_HEIGHT);
	SetPolygonPos(LOGO_POS_X, LOGO_POS_Y);
	SetPolygonTexture(g_pTexture[TEX_LOGO]);
	DrawPolygon(pDC);


	for (int i = 0; i < MAX_RANKING; i++) {
		SetPolygonFrameSize(1.0f / NUMBER_COUNT_X, 1.0f / NUMBER_COUNT_Y);
		SetPolygonSize(NUMBER_SIZE_X, NUMBER_SIZE_Y);
		SetPolygonPos(MINCHR_POS_X, 170.0f - i * 85.0f - NUMBER_SIZE_Y * 0.5);
		SetPolygonUV((MIN % NUMBER_COUNT_X) / (float)NUMBER_COUNT_X,
					(MIN / NUMBER_COUNT_X) / (float)NUMBER_COUNT_Y);
		SetPolygonTexture(g_pTexture[TEX_TIMER]);
		DrawPolygon(pDC);

		SetPolygonFrameSize(1.0f / NUMBER_COUNT_X, 1.0f / NUMBER_COUNT_Y);
		SetPolygonSize(NUMBER_SIZE_X, NUMBER_SIZE_Y);
		SetPolygonPos(SECCHR_POS_X, 170.0f - i * 85.0f - NUMBER_SIZE_Y * 0.5);
		SetPolygonUV((SEC % NUMBER_COUNT_X) / (float)NUMBER_COUNT_X,
					(SEC / NUMBER_COUNT_X) / (float)NUMBER_COUNT_Y);
		SetPolygonTexture(g_pTexture[TEX_TIMER]);
		DrawPolygon(pDC);

		sec = g_nTime[i];
		min = sec / 60;
		if (sec > 59) {
			sec -= 60 * min;
		}

		DrawNumber(XMFLOAT2(MINNUM_POS_X, 170.0f - i * 85.0f), (unsigned)min, MIN_CHRCNT);
		DrawNumber(XMFLOAT2(SECNUM_POS_X, 170.0f - i * 85.0f), (unsigned)sec, SEC_CHRCNT);
	}

	SetPolygonSize(CC_WIDTH, CC_HEIGHT);
	SetPolygonPos(CC_POS_X, CC_POS_Y);
	SetPolygonTexture(g_pTexture[TEX_CLICKCONTINUE]);
	DrawPolygon(pDC);

}



//------------------------------------------------------------------------------------
//
//				�����L���O�ۑ�
//
//------------------------------------------------------------------------------------
void SetRanking(int nScore) {
	if (g_nTime[MAX_RANKING - 1] < nScore) {
		return;
	}
	for (int i = 0; i < MAX_RANKING; ++i) {
		if (g_nTime[i] > nScore) {
			for (int j = MAX_RANKING - 1; j > i; --j) {
				g_nTime[j] = g_nTime[j - 1];
			}
			g_nTime[i] = nScore;

			SaveRankData();

			return;
		}
		if (g_nTime[i] == nScore) {
			return;
		}
	}
}

int* GetRanking() {
	return &g_nTime[0];
}
