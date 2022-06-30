//************************************************************************************
// 
// ランキング[ranking.h]
// 編集者：伊地田真衣
// 
//************************************************************************************
#ifndef __RANKING_H__
#define __RANKING_H__
//-------------------- インクルード部 --------------------
#include "main.h"

//-------------------- 定数定義 --------------------
#define MAX_RANKING			(5)

//-------------------- プロトタイプ宣言 --------------------
HRESULT	InitRanking();			// 初期化
void	InitRankData();			// データの読み込み
void	UninitRanking();		// 終了
void	UpdateRanking();		// 更新
void	DrawRanking();			// 描画

void SetRanking(int nScore);	// ランキング更新
int* GetRanking();				// ランキング取得

#endif