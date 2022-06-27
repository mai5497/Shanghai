#pragma once
//----- インクルード部 -----
#include "main.h"


//===========================
//
//	プロトタイプ宣言
//
//===========================
HRESULT	InitData();			// 初期化
void	UninitData();		// 終了
void	SaveRankData();		// ランキングセーブ
int*	LoadRankData();		// ランキングロード
int*	LoadBoardDate();	// ボードデータロード