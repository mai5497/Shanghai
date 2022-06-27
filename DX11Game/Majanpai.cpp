//----- �C���N���[�h�� -----
#include "polygon.h"
#include "Texture.h"
#include "Majanpai.h"
#include "Gamedata.h"
#include "debugproc.h"
#include "main.h"
#include "input.h"
#include "Scene.h"
#include "ranking.h"
#include "timer.h"
#include "Sound.h"
#include <stdlib.h>
#include <time.h>


//----- �萔��` -----
#define PATH_TILES			L"data/texture/mahjongtile.png"				// �v�摜�p�X
#define PATH_GAMEOVER		L"data/texture/cantplay.png"

#define TILES_SIZERATE		(1/3.5f)									// �v�̏k���䗦
#define TILES_SHIFT			(20.0f)										// �d�Ȃ��Ă���v���������炷�i�v�̍����Ɠ����j
#define TILES_WIDTH			(1578 / 9 * TILES_SIZERATE)					// �v�̉���
#define TILES_FULLHEIGHT	(1109 / 4 * TILES_SIZERATE)					// �v�̏c���i�����̕������܂ށj
#define TILES_HEIGHT		(1109 / 4 * TILES_SIZERATE - TILES_SHIFT)	// �v�̏c���i���̂��镔�������j

#define TILES_COUNT_X		(9)											// �e�X�N�`���̉��̐�
#define TILES_COUNT_Y		(4)											// �e�N�X�`���̏c�̐�

#define MAX_TILES			(136)										// �v�̐��i�G�ߔv�����j

#define TILES_POS_CENTER	(0.0f)															// �`�撆��
#define TILES_POS_LEFT		(TILES_POS_CENTER - (float)TILES_WIDTH  * 7 + TILES_WIDTH / 2)	// �`�捶�[
#define TILES_POS_RIGHT		(TILES_POS_CENTER + (float)TILES_WIDTH  * 8 - TILES_WIDTH / 2)	// �`��E�[
#define TILES_POS_UP		(TILES_POS_CENTER + (float)TILES_HEIGHT * 4 - TILES_HEIGHT / 2)	// �`���[
#define TILES_POS_DOWN		(TILES_POS_CENTER - (float)TILES_HEIGHT * 4 + TILES_HEIGHT / 2)	// �`�扺�[

#define TILES_POS_CNT_X		(31)										// �v�̔z�u�z��̉���
#define TILES_POS_CNT_Y		(17)										// �v�̔z�u�z��̏c��
#define	TILES_POS_CNT_Z		(5)											// �v�̔z�u�z��̐[�x

#define GAMEOVER_SIZE_X		(614.0f)									// �Q�[���I�[�o�[����
#define GAMEOVER_SIZE_Y		(192.0f)									// �Q�[���I�[�o�[�c��

enum TEXNUM {
	TEX_TILES = 0,
	TEX_GO,

	MAX_TEXTURE,
};


//----- �O���[�o���ϐ��錾 -----
int g_FreeCnt;
int g_TilesSum;
int g_DeltilesSum;

Mahjongtile *g_pMahjongtile; // �v�̏��
int ***g_pSelectPos;

static LPCWSTR g_pszTexFName[MAX_TEXTURE] = {
	L"data/texture/mahjongtile.png",
	L"data/texture/cantplay.png",
};

static ID3D11ShaderResourceView *g_pTexture[MAX_TEXTURE];

//----- �v���g�^�C�v�錾 -----
int Compare(const void *n1, const void *n2);
bool SetBoard();
bool SearchFree(Mahjongtile* tmp);
void SearchDelTile();

//------------------------------------------------------------------------------------
//
//				������
//
//------------------------------------------------------------------------------------
HRESULT InitMajanpai() {
	bool IsBoardSet = true;
	int *pLoad;
	pLoad = LoadBoardDate();

	int SelectPos[TILES_POS_CNT_Z][TILES_POS_CNT_Y][TILES_POS_CNT_X];

	for (int i = 0, cnt = 0; i < TILES_POS_CNT_Z; i++) {
		for (int j = 0; j < TILES_POS_CNT_Y; j++) {
			for (int k = 0; k < TILES_POS_CNT_X; k++, cnt++) {
				SelectPos[i][j][k] = *(pLoad + cnt);
			}
		}
	}


	//----- �ϐ������� -----
	g_pMahjongtile = new Mahjongtile[MAX_TILES];
	g_pSelectPos = new int**[TILES_POS_CNT_Z];
	for (int i = 0; i < TILES_POS_CNT_Z; i++) {	// �������m��
		g_pSelectPos[i] = new int*[TILES_POS_CNT_Y];
		for (int j = 0; j < TILES_POS_CNT_Y; j++) {
			g_pSelectPos[i][j] = new int[TILES_POS_CNT_X];
		}
	}
	g_FreeCnt = 0;
	g_TilesSum = MAX_TILES;
	g_DeltilesSum = 0;

	do {
		//----- �I�����ꂽ�z�u��ǂݍ��� -----
		for (int i = 0; i < TILES_POS_CNT_Z; i++) {
			for (int j = 0; j < TILES_POS_CNT_Y; j++) {
				for (int k = 0; k < TILES_POS_CNT_X; k++) {
					g_pSelectPos[i][j][k] = SelectPos[i][j][k];
				}
			}
		}

		//----- �Ֆʂ̐����ig_pMahjongtile��tilePos�̏��������s���Ă���j-----
		IsBoardSet = SetBoard();
	} while (IsBoardSet);

	//----- �Ֆʂ���������ɒ��g��ς��Ă���̂ł�����x������ -----
	for (int i = 0; i < TILES_POS_CNT_Z; i++) {
		for (int j = 0; j < TILES_POS_CNT_Y; j++) {
			for (int k = 0; k < TILES_POS_CNT_X; k++) {
				g_pSelectPos[i][j][k] = SelectPos[i][j][k];
			}
		}
	}

	//----- g_pMahjongtile�̏����� -----
	for (int i = 0; i < MAX_TILES; i++) {
		g_pMahjongtile[i].IsSelect = false;

		g_pMahjongtile[i].tileNum = i;

		g_pMahjongtile[i].IsFree = SearchFree(&g_pMahjongtile[i]);
	}
	SearchDelTile();

	//----- �����_���Ō��܂���������ёւ� -----
	// �����̏����[�������̏����[�������̏����ɂ���
	qsort(g_pMahjongtile, MAX_TILES, sizeof(Mahjongtile), Compare);

	HRESULT hr = S_OK;

	ID3D11Device *pDevice = GetDevice();
	for (int i = 0; i < MAX_TEXTURE; i++) {
		hr = CreateTextureFromFile(pDevice, g_pszTexFName[i], &g_pTexture[i]);
		if (FAILED(hr)) {
			return hr;
		}
	}


	return hr;
}

//------------------------------------------------------------------------------------
//
//				�I��
//
//------------------------------------------------------------------------------------
void UninitMajanpai() {
	//----- �������̊J�� -----
	for (int i = 0; i < TILES_POS_CNT_Z; i++) {
		for (int j = 0; j < TILES_POS_CNT_Y; j++) {
			delete[] g_pSelectPos[i][j];
		}
		delete[] g_pSelectPos[i];
	}
	delete[] g_pSelectPos;
	delete[] g_pMahjongtile;

	// �e�N�X�`���J��
	for (int i = 0; i < MAX_TEXTURE; i++) {
		SAFE_RELEASE(g_pTexture[i]);
	}
}


//------------------------------------------------------------------------------------
//
//				�X�V
//
//------------------------------------------------------------------------------------
void UpdateMajanpai() {
	//----- ������ -----
	static Mahjongtile *pSelect[2] = { nullptr,nullptr };
	static int selectCnt = 0;
	float BasePos_X = 0.0f;
	float BasePos_Y = 0.0f;

	for (int i = 0; i < MAX_TILES; i++) {
		if (g_pMahjongtile[i].tilePos[1] % 2 == 1) {// 0���琔���ċ�����ɂ���v�͏������炷
			BasePos_Y = (float)(TILES_SHIFT * g_pMahjongtile[i].tilePos[0]);
		} else {
			BasePos_Y = (float)(TILES_SHIFT * g_pMahjongtile[i].tilePos[0] + TILES_SHIFT);
		}

		if (g_pMahjongtile[i].tilePos[2] % 2 == 0) {
			BasePos_X = TILES_WIDTH / 2.0f;
		} else {
			BasePos_X = 0;
		}


		//----- �J�[�\���Ƃ̓����蔻�� -----
		if (TILES_POS_LEFT + TILES_WIDTH * (g_pMahjongtile[i].tilePos[2] / 2) - BasePos_X - TILES_WIDTH / 2 <= GetMousePosition()->x - SCREEN_WIDTH / 2 && GetMousePosition()->x - SCREEN_WIDTH / 2 <= TILES_POS_LEFT + TILES_WIDTH * (g_pMahjongtile[i].tilePos[2] / 2) - BasePos_X + TILES_WIDTH / 2) {
			if (BasePos_Y + TILES_POS_UP - TILES_HEIGHT * (g_pMahjongtile[i].tilePos[1] / 2) + TILES_HEIGHT / 2 >= (GetMousePosition()->y - SCREEN_HEIGHT / 2) * -1 && (GetMousePosition()->y - SCREEN_HEIGHT / 2)*-1 >= BasePos_Y + TILES_POS_UP - TILES_HEIGHT * (g_pMahjongtile[i].tilePos[1] / 2) - TILES_HEIGHT / 2) {
				// �J�[�\�����v�̏�ɂ����č��N���b�N���ꂽ�Ƃ�
				if (g_pMahjongtile[i].IsSelect == false && GetMouseRelease(MOUSEBUTTON_L) && g_pMahjongtile[i].IsFree == true) {
					// SE�Đ�
					CSound::Play(SE_TILESELECT_1);
					g_pMahjongtile[i].IsSelect = true;	// �I�����ꂽ
					selectCnt++;						// �I�𐔃J�E���g�A�b�v

					pSelect[1] = pSelect[0];			// �����Ă����f�[�^�������o��
					pSelect[0] = &g_pMahjongtile[i];	// �V�����ۑ�
				} else if (g_pMahjongtile[i].IsSelect == true && GetMouseRelease(MOUSEBUTTON_L)) {
					g_pMahjongtile[i].IsSelect = false;	// �I������
					selectCnt--;						// �I�𐔃J�E���g�_�E��
				}
			}
		}

		//----- �v�̑I�𐔂��Q�������� -----
		if (selectCnt == 2) {
			// �v�̕���������������
			if (pSelect[0]->tileNum / 4 == pSelect[1]->tileNum / 4) {
				// �v�������i�z�u��0�j
				g_pSelectPos[pSelect[0]->tilePos[0]][pSelect[0]->tilePos[1]][pSelect[0]->tilePos[2]] = 0;
				g_pSelectPos[pSelect[1]->tilePos[0]][pSelect[1]->tilePos[1]][pSelect[1]->tilePos[2]] = 0;
				// �I��������
				pSelect[0]->IsSelect = false;
				pSelect[1]->IsSelect = false;
				selectCnt = 0;		// �I�𐔂����Z�b�g
				g_TilesSum -= 2;	// �v�̍��v�����炷
				// ���R�v�Č���
				g_FreeCnt = 0;
				for (int j = 0; j < MAX_TILES; j++) {
					g_pMahjongtile[j].IsFree = SearchFree(&g_pMahjongtile[j]);
				}
				SearchDelTile();
			} else { // �I�����ꂽ�v���������ł͂Ȃ�
				pSelect[0]->IsSelect = false;	// �I��������
				pSelect[1]->IsSelect = false;
				selectCnt = 0;					// �I�𐔃��Z�b�g
			}
		}
	}


	//----- �v���Ȃ��Ȃ����� -----
	if (g_TilesSum < 1) {
	//if (g_TilesSum < 134) {
		SetRanking(GetTimer()); // �����L���O�X�V
		SetScene(SCENE_RESULT);	// ���U���g�ֈڍs
		return;
	}


	if (g_DeltilesSum < 1) {
		if (GetKeyRelease(VK_SPACE)) {	// �X�y�[�X�������ꂽ��
			//�^�C�g����ʂ�
			SetScene(SCENE_TITLE);
			return;
		}
		if (GetKeyRelease(VK_RETURN)) {	// �G���^�[�������ꂽ��
			UninitMajanpai();
			InitMajanpai();
			resetTimer();
		}
	}
}


//------------------------------------------------------------------------------------
//
//				�`��
//
//------------------------------------------------------------------------------------
void DrawMajanpai() {
	//----- ������ -----
	ID3D11DeviceContext *pDC = GetDeviceContext();
	float BasePos_Y = 0.0f;
	float BasePos_X = 0.0f;


	for (int i = 0; i < MAX_TILES; i++) {
		if (g_pSelectPos[g_pMahjongtile[i].tilePos[0]][g_pMahjongtile[i].tilePos[1]][g_pMahjongtile[i].tilePos[2]] == 0) {
			continue;	// �z�u��0�������Ă���Ƃ���͏��������Ȃ�
		}

		if (g_pMahjongtile[i].tilePos[1] % 2 == 1) {// 0���琔���ċ�����ɂ���v�͏������炷
			BasePos_Y = (float)(TILES_SHIFT * g_pMahjongtile[i].tilePos[0]);
		} else {
			BasePos_Y = (float)(TILES_SHIFT * g_pMahjongtile[i].tilePos[0] + TILES_SHIFT);
		}

		if (g_pMahjongtile[i].tilePos[2] % 2 == 0) {
			BasePos_X = TILES_WIDTH / 2.0f;
		} else {
			BasePos_X = 0;
		}

		//----- �`��ݒ� -----
		SetPolygonPos(TILES_POS_LEFT + TILES_WIDTH * (g_pMahjongtile[i].tilePos[2] / 2) - BasePos_X, BasePos_Y + TILES_POS_UP - TILES_HEIGHT * (g_pMahjongtile[i].tilePos[1] / 2));	// �|���S���`��ʒu�̕ύX
		SetPolygonSize(TILES_WIDTH, TILES_FULLHEIGHT);
		SetPolygonFrameSize(1.0f / (float)TILES_COUNT_X, 1.0f / (float)TILES_COUNT_Y);
		SetPolygonUV(((g_pMahjongtile[i].tileNum / 4) % TILES_COUNT_X) / (float)TILES_COUNT_X,
			((g_pMahjongtile[i].tileNum / 4) / TILES_COUNT_X) / (float)TILES_COUNT_Y);

		if (g_pMahjongtile[i].IsSelect) {	// �I�����ꂽ�v�Ȃ物�F�ŕ\��
			SetPolygonColor(1.0f, 1.0f, 0.0f);
		} else {
			SetPolygonColor(1.0f, 1.0f, 1.0f);

		}
		SetPolygonTexture(g_pTexture[TEX_TILES]);
		DrawPolygon(pDC);
	}

	if (g_DeltilesSum < 1) {	// �Q�[���I�[�o�[����
		SetPolygonPos(0.0f, 0.0f);
		SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);
		SetPolygonUV(0.0f, 0.0f);
		SetPolygonFrameSize(1.0f, 1.0f);
		SetPolygonTexture(nullptr);
		SetPolygonColor(1.0f, 1.0f, 1.0f);
		SetPolygonAlpha(0.5f);
		DrawPolygon(GetDeviceContext());

		ID3D11DeviceContext *pDC = GetDeviceContext();
		SetPolygonSize(GAMEOVER_SIZE_X, GAMEOVER_SIZE_Y);
		SetPolygonPos(0.0f, 0.0f);
		SetPolygonAlpha(1.0f);
		SetPolygonTexture(g_pTexture[TEX_GO]);
		DrawPolygon(pDC);
	}

	//----- �`��ݒ��߂� -----
	SetPolygonColor(1.0f, 1.0f, 1.0f);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);

}


//------------------------------------------------------------------------------------
//
//				�\�[�g��r
//
//------------------------------------------------------------------------------------
int Compare(const void *n1, const void *n2) {
	int num = 0;

	// �����̏����[�������̏����[�������̏����ɂ���
	if (((Mahjongtile *)n1)->tilePos[0] > ((Mahjongtile *)n2)->tilePos[0]) {
		num = 1;
	} else if (((Mahjongtile *)n1)->tilePos[0] < ((Mahjongtile *)n2)->tilePos[0]) {
		num = -1;
	} else {
		if (((Mahjongtile *)n1)->tilePos[1] > ((Mahjongtile *)n2)->tilePos[1]) {
			num = 1;
		} else if (((Mahjongtile *)n1)->tilePos[1] < ((Mahjongtile *)n2)->tilePos[1]) {
			num = -1;
		} else {
			if (((Mahjongtile *)n1)->tilePos[2] > ((Mahjongtile *)n2)->tilePos[2]) {
				num = 1;
			} else {
				num = -1;
			}
		}
	}

	return num;
}

//------------------------------------------------------------------------------------
//
//				���R�v���擾
//
//------------------------------------------------------------------------------------
int GetFreeCnt() {
	return g_FreeCnt;
}


//------------------------------------------------------------------------------------
//
//				���v���擾
//
//------------------------------------------------------------------------------------
int GetTilesSum() {
	return g_TilesSum;
}


//------------------------------------------------------------------------------------
//
//				������v���擾
//
//------------------------------------------------------------------------------------
int GetDelTilesSum() {
	return g_DeltilesSum;
}



//------------------------------------------------------------------------------------
//
//				�Ֆʂ̐���
//
//------------------------------------------------------------------------------------
bool SetBoard() {
	//----- ������ -----
	int i, j, k, l;
	int random = -1;
	bool loop = true;
	int differ = 0;
	int *freeTmp;
	freeTmp = new int[MAX_TILES];

	Mahjongtile *temporary;
	temporary = new Mahjongtile[MAX_TILES];

	//int freeTmp[MAX_TILES];
	//Mahjongtile temporary[MAX_TILES];

	int lasttmp;

	srand((unsigned)time(NULL));

	//----- �v������Ƃ�����L�� -----
	for (i = 0, l = 0; i < TILES_POS_CNT_Z; i++) {
		for (j = 0; j < TILES_POS_CNT_Y; j++) {
			for (k = 0; k < TILES_POS_CNT_X; k++) {
				//--- �v�����邩 ---
				if (g_pSelectPos[i][j][k] == 1 && l < MAX_TILES) {
					temporary[l].tilePos[0] = i;
					temporary[l].tilePos[1] = j;
					temporary[l].tilePos[2] = k;

					temporary[l].IsFree = false;

					l++;
				}
			}
		}
	}

	//----- �ꊇ�Ŕz�u�����߂� -----
	for (i = 0; i < MAX_TILES; i++) {
		//----- ���R�v���菈�� -----
		for (j = 0; j < MAX_TILES; j++) {
			temporary[j].IsFree = SearchFree(&temporary[j]);
			if (g_FreeCnt < 1) {
				continue;
			}
			if (temporary[j].IsFree) {
				// �����������ق������ɗ���
				/*g_FreeCnt���O�œ����Ă���̂Ł[�P�̒l�����Ă���*/
				lasttmp = g_FreeCnt - 1;
				freeTmp[lasttmp] = j;
			}
		}

		if (g_FreeCnt < 2) {	// �����ɓ���Ƃ��͔z�u�G���[
			// �Ăяo�����ɖ߂�̂Ń������J��
			delete[] freeTmp;
			delete[] temporary;

			return true;
		}

		// ���፷�̎Z�o
		differ = temporary[freeTmp[lasttmp]].tilePos[0] - temporary[freeTmp[lasttmp - 1]].tilePos[0];

		//----- ��ڂ̔v���Ƃ� -----
		if (differ >= g_FreeCnt) {
			g_pMahjongtile[i].tilePos[0] = temporary[freeTmp[lasttmp]].tilePos[0];
			g_pMahjongtile[i].tilePos[1] = temporary[freeTmp[lasttmp]].tilePos[1];
			g_pMahjongtile[i].tilePos[2] = temporary[freeTmp[lasttmp]].tilePos[2];
		} else {
			// �����_���łƂ�
			random = rand() % (g_FreeCnt);
			g_pMahjongtile[i].tilePos[0] = temporary[freeTmp[random]].tilePos[0];
			g_pMahjongtile[i].tilePos[1] = temporary[freeTmp[random]].tilePos[1];
			g_pMahjongtile[i].tilePos[2] = temporary[freeTmp[random]].tilePos[2];
		}

		// �v���Ƃ������Ƃɂ���
		g_pSelectPos[g_pMahjongtile[i].tilePos[0]][g_pMahjongtile[i].tilePos[1]][g_pMahjongtile[i].tilePos[2]] = 0;


		//----- ��ڂ̔v���Ƃ� -----
		// ���[�v�̏���
		loop = true;
		do {
			random = rand() % (g_FreeCnt);
			if (temporary[freeTmp[random]].tilePos[0] != g_pMahjongtile[i].tilePos[0] ||
				temporary[freeTmp[random]].tilePos[1] != g_pMahjongtile[i].tilePos[1] ||
				temporary[freeTmp[random]].tilePos[2] != g_pMahjongtile[i].tilePos[2]) {
				loop = false;
			}
		} while (loop);

		i++;	// ���̏ꏊ�ֈڍs

		// ��ڂ̏ꏊ�ɑ��
		g_pMahjongtile[i].tilePos[0] = temporary[freeTmp[random]].tilePos[0];
		g_pMahjongtile[i].tilePos[1] = temporary[freeTmp[random]].tilePos[1];
		g_pMahjongtile[i].tilePos[2] = temporary[freeTmp[random]].tilePos[2];

		// �v���Ƃ������Ƃɂ���
		g_pSelectPos[g_pMahjongtile[i].tilePos[0]][g_pMahjongtile[i].tilePos[1]][g_pMahjongtile[i].tilePos[2]] = 0;

		//----- ���̃��[�v�̏��� -----
		for (k = 0; k < g_FreeCnt; k++) {
			freeTmp[k] = -1;
		}
		g_FreeCnt = 0;
	}


	//----- �������J�� -----
	delete[] freeTmp;
	delete[] temporary;

	return false;
}



//------------------------------------------------------------------------------------
//
//				���R�v�T��
//
//------------------------------------------------------------------------------------
bool SearchFree(Mahjongtile* tmp) {
	// �z��O��G��Ȃ��悤�ɒ[�m�肵�Ă�Ƃ���͔v�����邩�����m�F
	if (tmp->tilePos[0] == 4 || tmp->tilePos[2] == 1 || tmp->tilePos[2] == (TILES_POS_CNT_X - 2)) {
		if (g_pSelectPos[tmp->tilePos[0]][tmp->tilePos[1]][tmp->tilePos[2]] == 1) {
			return true;
		}
		return false;
	}


	//--- �����ɔv�����邩 ---
	if (((g_pSelectPos[tmp->tilePos[0]][tmp->tilePos[1] - 1][tmp->tilePos[2] - 2] == 0 && // ��
		  g_pSelectPos[tmp->tilePos[0]][tmp->tilePos[1]][tmp->tilePos[2] - 2] == 0 && // �^��
		  g_pSelectPos[tmp->tilePos[0]][tmp->tilePos[1] + 1][tmp->tilePos[2] - 2] == 0) || // ��
	     //--- �E���ɔv�����邩 ---
		(g_pSelectPos[tmp->tilePos[0]][tmp->tilePos[1] - 1][tmp->tilePos[2] + 2] == 0 && // ��
		 g_pSelectPos[tmp->tilePos[0]][tmp->tilePos[1]][tmp->tilePos[2] + 2] == 0 && // �^��
		 g_pSelectPos[tmp->tilePos[0]][tmp->tilePos[1] + 1][tmp->tilePos[2] + 2] == 0)) && // ��
	     //--- �����ɔv�͂��邩 ---
		(g_pSelectPos[tmp->tilePos[0]][tmp->tilePos[1]][tmp->tilePos[2]] == 1)) {
		//--- ��̒i�ɔv�����邩 ---
		if ((g_pSelectPos[tmp->tilePos[0] + 1][tmp->tilePos[1] - 1][tmp->tilePos[2] - 1] == 0 && // ����
			 g_pSelectPos[tmp->tilePos[0] + 1][tmp->tilePos[1]][tmp->tilePos[2] - 1] == 0 &&	// ���^��
			 g_pSelectPos[tmp->tilePos[0] + 1][tmp->tilePos[1] + 1][tmp->tilePos[2] - 1] == 0 && // ����

			 g_pSelectPos[tmp->tilePos[0] + 1][tmp->tilePos[1] - 1][tmp->tilePos[2] + 1] == 0 && // �E��
			 g_pSelectPos[tmp->tilePos[0] + 1][tmp->tilePos[1]][tmp->tilePos[2] + 1] == 0 && // �E�^��
			 g_pSelectPos[tmp->tilePos[0] + 1][tmp->tilePos[1] + 1][tmp->tilePos[2] + 1] == 0 && // �E��

			 g_pSelectPos[tmp->tilePos[0] + 1][tmp->tilePos[1] - 1][tmp->tilePos[2]] == 0 && // ����
			 g_pSelectPos[tmp->tilePos[0] + 1][tmp->tilePos[1]][tmp->tilePos[2]] == 0 && // ���^��
			 g_pSelectPos[tmp->tilePos[0] + 1][tmp->tilePos[1] + 1][tmp->tilePos[2]] == 0)) { // ����

			 g_FreeCnt = g_FreeCnt + 1;
			 return true;
		}
	}
	return false;
}


//------------------------------------------------------------------------------------
//
//				������v�T��
//
//------------------------------------------------------------------------------------
void SearchDelTile() {
	int *isfree = new int[MAX_TILES];
	g_DeltilesSum = 0;

	for (int i = 0; i < MAX_TILES; i++) {
		if (g_pMahjongtile[i].IsFree) {
			isfree[g_pMahjongtile[i].tileNum] = 1;
		} else {
			isfree[g_pMahjongtile[i].tileNum] = 0;
		}
	}

	for (int i = 0; i < MAX_TILES; i += 4) {
		if (isfree[i] == 0 && isfree[i + 1] == 0 && isfree[i + 2] == 0 && isfree[i + 3] == 0) {
			continue;
		}

		if (isfree[i] == 1 && isfree[i + 1] == 1 && isfree[i + 2] == 1 && isfree[i + 3] == 1) {
			g_DeltilesSum = g_DeltilesSum + 2;
			continue;
		}

		if (isfree[i] == 1 && isfree[i + 1] == 1) {
			g_DeltilesSum = g_DeltilesSum + 1;
		}
		if (isfree[i] == 1 && isfree[i + 2] == 1) {
			g_DeltilesSum = g_DeltilesSum + 1;
		}
		if (isfree[i] == 1 && isfree[i + 3] == 1) {
			g_DeltilesSum = g_DeltilesSum + 1;
		}

		if (isfree[i + 1] == 1 && isfree[i + 2] == 1) {
			g_DeltilesSum = g_DeltilesSum + 1;
		}
		if (isfree[i + 1] == 1 && isfree[i + 3] == 1) {
			g_DeltilesSum = g_DeltilesSum + 1;
		}

		if (isfree[i + 2] == 1 && isfree[i + 3] == 1) {
			g_DeltilesSum = g_DeltilesSum + 1;
		}
	}

	delete[] isfree;
}

