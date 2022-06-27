//ランキング
#pragma once
#include "main.h"

#define MAX_RANKING			(5)


//===========================
//
//	プロトタイプ宣言
//
//===========================
HRESULT	InitRanking();			// 初期化
void	InitRankData();			// データの読み込み
void	UninitRanking();		// 終了
void	UpdateRanking();		// 更新
void	DrawRanking();			// 描画

void SetRanking(int nScore);	// ランキング更新
int* GetRanking();				// ランキング取得