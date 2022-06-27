//インクルードガード
#ifndef __MAJANPAI_H__
#define __MAJANPAI_H__

// インクルード部
#include "main.h"

// 定数定義
#define TILES_POS_CNT_X		(31)										// 牌の配置配列の横幅
#define TILES_POS_CNT_Y		(17)										// 牌の配置配列の縦幅
#define	TILES_POS_CNT_Z		(5)											// 牌の配置配列の深度


// 構造体定義
struct Mahjongtile {
	int tileNum;
	int tilePos[3];	// 座標を直接入れる
	bool IsFree;	// 自由牌か否か
	bool IsSelect;	// 選択されているか（選択されていればtrue）
};

// プロトタイプ宣言
HRESULT InitMajanpai();
void UninitMajanpai();
void UpdateMajanpai();
void DrawMajanpai();

int GetFreeCnt();
int GetTilesSum();
int GetDelTilesSum();


#endif