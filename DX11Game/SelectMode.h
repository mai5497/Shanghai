#ifndef __SELECTMODE_H__
#define __SELECTMODE_H__

#include "main.h"

//===========================
//
//	プロトタイプ宣言
//
//===========================
HRESULT	InitMode();		// 初期化
void	UninitMode();	// 終了
void	UpdateMode();	// 更新
void	DrawMode();		// 描画
int		GetMode();		// モード番号返却

#endif // !__SELECTMODE_H__

