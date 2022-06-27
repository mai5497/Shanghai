//----- インクルード部 -----
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


//----- 定数定義 -----
#define PATH_TILES			L"data/texture/mahjongtile.png"				// 牌画像パス
#define PATH_GAMEOVER		L"data/texture/cantplay.png"

#define TILES_SIZERATE		(1/3.5f)									// 牌の縮小比率
#define TILES_SHIFT			(20.0f)										// 重なっている牌を少しずらす（牌の高さと同じ）
#define TILES_WIDTH			(1578 / 9 * TILES_SIZERATE)					// 牌の横幅
#define TILES_FULLHEIGHT	(1109 / 4 * TILES_SIZERATE)					// 牌の縦幅（高さの部分も含む）
#define TILES_HEIGHT		(1109 / 4 * TILES_SIZERATE - TILES_SHIFT)	// 牌の縦幅（柄のある部分だけ）

#define TILES_COUNT_X		(9)											// テスクチャの横の数
#define TILES_COUNT_Y		(4)											// テクスチャの縦の数

#define MAX_TILES			(136)										// 牌の数（季節牌抜き）

#define TILES_POS_CENTER	(0.0f)															// 描画中央
#define TILES_POS_LEFT		(TILES_POS_CENTER - (float)TILES_WIDTH  * 7 + TILES_WIDTH / 2)	// 描画左端
#define TILES_POS_RIGHT		(TILES_POS_CENTER + (float)TILES_WIDTH  * 8 - TILES_WIDTH / 2)	// 描画右端
#define TILES_POS_UP		(TILES_POS_CENTER + (float)TILES_HEIGHT * 4 - TILES_HEIGHT / 2)	// 描画上端
#define TILES_POS_DOWN		(TILES_POS_CENTER - (float)TILES_HEIGHT * 4 + TILES_HEIGHT / 2)	// 描画下端

#define TILES_POS_CNT_X		(31)										// 牌の配置配列の横幅
#define TILES_POS_CNT_Y		(17)										// 牌の配置配列の縦幅
#define	TILES_POS_CNT_Z		(5)											// 牌の配置配列の深度

#define GAMEOVER_SIZE_X		(614.0f)									// ゲームオーバー横幅
#define GAMEOVER_SIZE_Y		(192.0f)									// ゲームオーバー縦幅

enum TEXNUM {
	TEX_TILES = 0,
	TEX_GO,

	MAX_TEXTURE,
};


//----- グローバル変数宣言 -----
int g_FreeCnt;
int g_TilesSum;
int g_DeltilesSum;

Mahjongtile *g_pMahjongtile; // 牌の情報
int ***g_pSelectPos;

static LPCWSTR g_pszTexFName[MAX_TEXTURE] = {
	L"data/texture/mahjongtile.png",
	L"data/texture/cantplay.png",
};

static ID3D11ShaderResourceView *g_pTexture[MAX_TEXTURE];

//----- プロトタイプ宣言 -----
int Compare(const void *n1, const void *n2);
bool SetBoard();
bool SearchFree(Mahjongtile* tmp);
void SearchDelTile();

//------------------------------------------------------------------------------------
//
//				初期化
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


	//----- 変数初期化 -----
	g_pMahjongtile = new Mahjongtile[MAX_TILES];
	g_pSelectPos = new int**[TILES_POS_CNT_Z];
	for (int i = 0; i < TILES_POS_CNT_Z; i++) {	// メモリ確保
		g_pSelectPos[i] = new int*[TILES_POS_CNT_Y];
		for (int j = 0; j < TILES_POS_CNT_Y; j++) {
			g_pSelectPos[i][j] = new int[TILES_POS_CNT_X];
		}
	}
	g_FreeCnt = 0;
	g_TilesSum = MAX_TILES;
	g_DeltilesSum = 0;

	do {
		//----- 選択された配置を読み込む -----
		for (int i = 0; i < TILES_POS_CNT_Z; i++) {
			for (int j = 0; j < TILES_POS_CNT_Y; j++) {
				for (int k = 0; k < TILES_POS_CNT_X; k++) {
					g_pSelectPos[i][j][k] = SelectPos[i][j][k];
				}
			}
		}

		//----- 盤面の生成（g_pMahjongtileのtilePosの初期化も行っている）-----
		IsBoardSet = SetBoard();
	} while (IsBoardSet);

	//----- 盤面を作った時に中身を変えているのでもう一度初期化 -----
	for (int i = 0; i < TILES_POS_CNT_Z; i++) {
		for (int j = 0; j < TILES_POS_CNT_Y; j++) {
			for (int k = 0; k < TILES_POS_CNT_X; k++) {
				g_pSelectPos[i][j][k] = SelectPos[i][j][k];
			}
		}
	}

	//----- g_pMahjongtileの初期化 -----
	for (int i = 0; i < MAX_TILES; i++) {
		g_pMahjongtile[i].IsSelect = false;

		g_pMahjongtile[i].tileNum = i;

		g_pMahjongtile[i].IsFree = SearchFree(&g_pMahjongtile[i]);
	}
	SearchDelTile();

	//----- ランダムで決まった情報を並び替え -----
	// ｚ軸の昇順ー＞ｙ軸の昇順ー＞ｘ軸の昇順にする
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
//				終了
//
//------------------------------------------------------------------------------------
void UninitMajanpai() {
	//----- メモリの開放 -----
	for (int i = 0; i < TILES_POS_CNT_Z; i++) {
		for (int j = 0; j < TILES_POS_CNT_Y; j++) {
			delete[] g_pSelectPos[i][j];
		}
		delete[] g_pSelectPos[i];
	}
	delete[] g_pSelectPos;
	delete[] g_pMahjongtile;

	// テクスチャ開放
	for (int i = 0; i < MAX_TEXTURE; i++) {
		SAFE_RELEASE(g_pTexture[i]);
	}
}


//------------------------------------------------------------------------------------
//
//				更新
//
//------------------------------------------------------------------------------------
void UpdateMajanpai() {
	//----- 初期化 -----
	static Mahjongtile *pSelect[2] = { nullptr,nullptr };
	static int selectCnt = 0;
	float BasePos_X = 0.0f;
	float BasePos_Y = 0.0f;

	for (int i = 0; i < MAX_TILES; i++) {
		if (g_pMahjongtile[i].tilePos[1] % 2 == 1) {// 0から数えて偶数列にある牌は少しずらす
			BasePos_Y = (float)(TILES_SHIFT * g_pMahjongtile[i].tilePos[0]);
		} else {
			BasePos_Y = (float)(TILES_SHIFT * g_pMahjongtile[i].tilePos[0] + TILES_SHIFT);
		}

		if (g_pMahjongtile[i].tilePos[2] % 2 == 0) {
			BasePos_X = TILES_WIDTH / 2.0f;
		} else {
			BasePos_X = 0;
		}


		//----- カーソルとの当たり判定 -----
		if (TILES_POS_LEFT + TILES_WIDTH * (g_pMahjongtile[i].tilePos[2] / 2) - BasePos_X - TILES_WIDTH / 2 <= GetMousePosition()->x - SCREEN_WIDTH / 2 && GetMousePosition()->x - SCREEN_WIDTH / 2 <= TILES_POS_LEFT + TILES_WIDTH * (g_pMahjongtile[i].tilePos[2] / 2) - BasePos_X + TILES_WIDTH / 2) {
			if (BasePos_Y + TILES_POS_UP - TILES_HEIGHT * (g_pMahjongtile[i].tilePos[1] / 2) + TILES_HEIGHT / 2 >= (GetMousePosition()->y - SCREEN_HEIGHT / 2) * -1 && (GetMousePosition()->y - SCREEN_HEIGHT / 2)*-1 >= BasePos_Y + TILES_POS_UP - TILES_HEIGHT * (g_pMahjongtile[i].tilePos[1] / 2) - TILES_HEIGHT / 2) {
				// カーソルが牌の上にあって左クリックされたとき
				if (g_pMahjongtile[i].IsSelect == false && GetMouseRelease(MOUSEBUTTON_L) && g_pMahjongtile[i].IsFree == true) {
					// SE再生
					CSound::Play(SE_TILESELECT_1);
					g_pMahjongtile[i].IsSelect = true;	// 選択された
					selectCnt++;						// 選択数カウントアップ

					pSelect[1] = pSelect[0];			// 入っていたデータを押し出す
					pSelect[0] = &g_pMahjongtile[i];	// 新しく保存
				} else if (g_pMahjongtile[i].IsSelect == true && GetMouseRelease(MOUSEBUTTON_L)) {
					g_pMahjongtile[i].IsSelect = false;	// 選択解除
					selectCnt--;						// 選択数カウントダウン
				}
			}
		}

		//----- 牌の選択数が２だったら -----
		if (selectCnt == 2) {
			// 牌の柄が同じだったら
			if (pSelect[0]->tileNum / 4 == pSelect[1]->tileNum / 4) {
				// 牌を消す（配置に0）
				g_pSelectPos[pSelect[0]->tilePos[0]][pSelect[0]->tilePos[1]][pSelect[0]->tilePos[2]] = 0;
				g_pSelectPos[pSelect[1]->tilePos[0]][pSelect[1]->tilePos[1]][pSelect[1]->tilePos[2]] = 0;
				// 選択を解除
				pSelect[0]->IsSelect = false;
				pSelect[1]->IsSelect = false;
				selectCnt = 0;		// 選択数をリセット
				g_TilesSum -= 2;	// 牌の合計を二つ減らす
				// 自由牌再決定
				g_FreeCnt = 0;
				for (int j = 0; j < MAX_TILES; j++) {
					g_pMahjongtile[j].IsFree = SearchFree(&g_pMahjongtile[j]);
				}
				SearchDelTile();
			} else { // 選択された牌が同じ柄ではない
				pSelect[0]->IsSelect = false;	// 選択を解除
				pSelect[1]->IsSelect = false;
				selectCnt = 0;					// 選択数リセット
			}
		}
	}


	//----- 牌がなくなったら -----
	if (g_TilesSum < 1) {
	//if (g_TilesSum < 134) {
		SetRanking(GetTimer()); // ランキング更新
		SetScene(SCENE_RESULT);	// リザルトへ移行
		return;
	}


	if (g_DeltilesSum < 1) {
		if (GetKeyRelease(VK_SPACE)) {	// スペースが押されたら
			//タイトル画面へ
			SetScene(SCENE_TITLE);
			return;
		}
		if (GetKeyRelease(VK_RETURN)) {	// エンターが押されたら
			UninitMajanpai();
			InitMajanpai();
			resetTimer();
		}
	}
}


//------------------------------------------------------------------------------------
//
//				描画
//
//------------------------------------------------------------------------------------
void DrawMajanpai() {
	//----- 初期化 -----
	ID3D11DeviceContext *pDC = GetDeviceContext();
	float BasePos_Y = 0.0f;
	float BasePos_X = 0.0f;


	for (int i = 0; i < MAX_TILES; i++) {
		if (g_pSelectPos[g_pMahjongtile[i].tilePos[0]][g_pMahjongtile[i].tilePos[1]][g_pMahjongtile[i].tilePos[2]] == 0) {
			continue;	// 配置に0が入っているところは処理をしない
		}

		if (g_pMahjongtile[i].tilePos[1] % 2 == 1) {// 0から数えて偶数列にある牌は少しずらす
			BasePos_Y = (float)(TILES_SHIFT * g_pMahjongtile[i].tilePos[0]);
		} else {
			BasePos_Y = (float)(TILES_SHIFT * g_pMahjongtile[i].tilePos[0] + TILES_SHIFT);
		}

		if (g_pMahjongtile[i].tilePos[2] % 2 == 0) {
			BasePos_X = TILES_WIDTH / 2.0f;
		} else {
			BasePos_X = 0;
		}

		//----- 描画設定 -----
		SetPolygonPos(TILES_POS_LEFT + TILES_WIDTH * (g_pMahjongtile[i].tilePos[2] / 2) - BasePos_X, BasePos_Y + TILES_POS_UP - TILES_HEIGHT * (g_pMahjongtile[i].tilePos[1] / 2));	// ポリゴン描画位置の変更
		SetPolygonSize(TILES_WIDTH, TILES_FULLHEIGHT);
		SetPolygonFrameSize(1.0f / (float)TILES_COUNT_X, 1.0f / (float)TILES_COUNT_Y);
		SetPolygonUV(((g_pMahjongtile[i].tileNum / 4) % TILES_COUNT_X) / (float)TILES_COUNT_X,
			((g_pMahjongtile[i].tileNum / 4) / TILES_COUNT_X) / (float)TILES_COUNT_Y);

		if (g_pMahjongtile[i].IsSelect) {	// 選択された牌なら黄色で表示
			SetPolygonColor(1.0f, 1.0f, 0.0f);
		} else {
			SetPolygonColor(1.0f, 1.0f, 1.0f);

		}
		SetPolygonTexture(g_pTexture[TEX_TILES]);
		DrawPolygon(pDC);
	}

	if (g_DeltilesSum < 1) {	// ゲームオーバー処理
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

	//----- 描画設定を戻す -----
	SetPolygonColor(1.0f, 1.0f, 1.0f);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);

}


//------------------------------------------------------------------------------------
//
//				ソート比較
//
//------------------------------------------------------------------------------------
int Compare(const void *n1, const void *n2) {
	int num = 0;

	// ｚ軸の昇順ー＞ｙ軸の昇順ー＞ｘ軸の昇順にする
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
//				自由牌数取得
//
//------------------------------------------------------------------------------------
int GetFreeCnt() {
	return g_FreeCnt;
}


//------------------------------------------------------------------------------------
//
//				総牌数取得
//
//------------------------------------------------------------------------------------
int GetTilesSum() {
	return g_TilesSum;
}


//------------------------------------------------------------------------------------
//
//				消せる牌数取得
//
//------------------------------------------------------------------------------------
int GetDelTilesSum() {
	return g_DeltilesSum;
}



//------------------------------------------------------------------------------------
//
//				盤面の生成
//
//------------------------------------------------------------------------------------
bool SetBoard() {
	//----- 初期化 -----
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

	//----- 牌があるところを記憶 -----
	for (i = 0, l = 0; i < TILES_POS_CNT_Z; i++) {
		for (j = 0; j < TILES_POS_CNT_Y; j++) {
			for (k = 0; k < TILES_POS_CNT_X; k++) {
				//--- 牌があるか ---
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

	//----- 一括で配置を決める -----
	for (i = 0; i < MAX_TILES; i++) {
		//----- 自由牌判定処理 -----
		for (j = 0; j < MAX_TILES; j++) {
			temporary[j].IsFree = SearchFree(&temporary[j]);
			if (g_FreeCnt < 1) {
				continue;
			}
			if (temporary[j].IsFree) {
				// 高さが高いほうが後ろに来る
				/*g_FreeCntが０で入ってくるのでー１の値を見ている*/
				lasttmp = g_FreeCnt - 1;
				freeTmp[lasttmp] = j;
			}
		}

		if (g_FreeCnt < 2) {	// ここに入るときは配置エラー
			// 呼び出し元に戻るのでメモリ開放
			delete[] freeTmp;
			delete[] temporary;

			return true;
		}

		// 高低差の算出
		differ = temporary[freeTmp[lasttmp]].tilePos[0] - temporary[freeTmp[lasttmp - 1]].tilePos[0];

		//----- 一つ目の牌をとる -----
		if (differ >= g_FreeCnt) {
			g_pMahjongtile[i].tilePos[0] = temporary[freeTmp[lasttmp]].tilePos[0];
			g_pMahjongtile[i].tilePos[1] = temporary[freeTmp[lasttmp]].tilePos[1];
			g_pMahjongtile[i].tilePos[2] = temporary[freeTmp[lasttmp]].tilePos[2];
		} else {
			// ランダムでとる
			random = rand() % (g_FreeCnt);
			g_pMahjongtile[i].tilePos[0] = temporary[freeTmp[random]].tilePos[0];
			g_pMahjongtile[i].tilePos[1] = temporary[freeTmp[random]].tilePos[1];
			g_pMahjongtile[i].tilePos[2] = temporary[freeTmp[random]].tilePos[2];
		}

		// 牌をとったことにする
		g_pSelectPos[g_pMahjongtile[i].tilePos[0]][g_pMahjongtile[i].tilePos[1]][g_pMahjongtile[i].tilePos[2]] = 0;


		//----- 二つ目の牌をとる -----
		// ループの準備
		loop = true;
		do {
			random = rand() % (g_FreeCnt);
			if (temporary[freeTmp[random]].tilePos[0] != g_pMahjongtile[i].tilePos[0] ||
				temporary[freeTmp[random]].tilePos[1] != g_pMahjongtile[i].tilePos[1] ||
				temporary[freeTmp[random]].tilePos[2] != g_pMahjongtile[i].tilePos[2]) {
				loop = false;
			}
		} while (loop);

		i++;	// 次の場所へ移行

		// 二つ目の場所に代入
		g_pMahjongtile[i].tilePos[0] = temporary[freeTmp[random]].tilePos[0];
		g_pMahjongtile[i].tilePos[1] = temporary[freeTmp[random]].tilePos[1];
		g_pMahjongtile[i].tilePos[2] = temporary[freeTmp[random]].tilePos[2];

		// 牌をとったことにする
		g_pSelectPos[g_pMahjongtile[i].tilePos[0]][g_pMahjongtile[i].tilePos[1]][g_pMahjongtile[i].tilePos[2]] = 0;

		//----- 次のループの準備 -----
		for (k = 0; k < g_FreeCnt; k++) {
			freeTmp[k] = -1;
		}
		g_FreeCnt = 0;
	}


	//----- メモリ開放 -----
	delete[] freeTmp;
	delete[] temporary;

	return false;
}



//------------------------------------------------------------------------------------
//
//				自由牌探索
//
//------------------------------------------------------------------------------------
bool SearchFree(Mahjongtile* tmp) {
	// 配列外を触らないように端確定してるところは牌があるかだけ確認
	if (tmp->tilePos[0] == 4 || tmp->tilePos[2] == 1 || tmp->tilePos[2] == (TILES_POS_CNT_X - 2)) {
		if (g_pSelectPos[tmp->tilePos[0]][tmp->tilePos[1]][tmp->tilePos[2]] == 1) {
			return true;
		}
		return false;
	}


	//--- 左側に牌があるか ---
	if (((g_pSelectPos[tmp->tilePos[0]][tmp->tilePos[1] - 1][tmp->tilePos[2] - 2] == 0 && // 上
		  g_pSelectPos[tmp->tilePos[0]][tmp->tilePos[1]][tmp->tilePos[2] - 2] == 0 && // 真ん中
		  g_pSelectPos[tmp->tilePos[0]][tmp->tilePos[1] + 1][tmp->tilePos[2] - 2] == 0) || // 下
	     //--- 右側に牌があるか ---
		(g_pSelectPos[tmp->tilePos[0]][tmp->tilePos[1] - 1][tmp->tilePos[2] + 2] == 0 && // 上
		 g_pSelectPos[tmp->tilePos[0]][tmp->tilePos[1]][tmp->tilePos[2] + 2] == 0 && // 真ん中
		 g_pSelectPos[tmp->tilePos[0]][tmp->tilePos[1] + 1][tmp->tilePos[2] + 2] == 0)) && // 下
	     //--- そこに牌はあるか ---
		(g_pSelectPos[tmp->tilePos[0]][tmp->tilePos[1]][tmp->tilePos[2]] == 1)) {
		//--- 上の段に牌があるか ---
		if ((g_pSelectPos[tmp->tilePos[0] + 1][tmp->tilePos[1] - 1][tmp->tilePos[2] - 1] == 0 && // 左上
			 g_pSelectPos[tmp->tilePos[0] + 1][tmp->tilePos[1]][tmp->tilePos[2] - 1] == 0 &&	// 左真ん中
			 g_pSelectPos[tmp->tilePos[0] + 1][tmp->tilePos[1] + 1][tmp->tilePos[2] - 1] == 0 && // 左下

			 g_pSelectPos[tmp->tilePos[0] + 1][tmp->tilePos[1] - 1][tmp->tilePos[2] + 1] == 0 && // 右上
			 g_pSelectPos[tmp->tilePos[0] + 1][tmp->tilePos[1]][tmp->tilePos[2] + 1] == 0 && // 右真ん中
			 g_pSelectPos[tmp->tilePos[0] + 1][tmp->tilePos[1] + 1][tmp->tilePos[2] + 1] == 0 && // 右下

			 g_pSelectPos[tmp->tilePos[0] + 1][tmp->tilePos[1] - 1][tmp->tilePos[2]] == 0 && // 中上
			 g_pSelectPos[tmp->tilePos[0] + 1][tmp->tilePos[1]][tmp->tilePos[2]] == 0 && // 中真ん中
			 g_pSelectPos[tmp->tilePos[0] + 1][tmp->tilePos[1] + 1][tmp->tilePos[2]] == 0)) { // 中下

			 g_FreeCnt = g_FreeCnt + 1;
			 return true;
		}
	}
	return false;
}


//------------------------------------------------------------------------------------
//
//				消せる牌探索
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

