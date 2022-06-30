//************************************************************************************
// 
// デバッグ表示[debugproc.h]
// 編集者：伊地田真衣
// 
//************************************************************************************
#ifndef __DEBUGPROC_H__
#define __DEBUGPROC_H__
//-------------------- インクルード部 --------------------
#include "main.h"

//-------------------- プロトタイプ宣言 --------------------
HRESULT InitDebugProc(void);
void UninitDebugProc(void);
void UpdateDebugProc(void);
void DrawDebugProc(void);
void StartDebugProc(bool hiragana = false);
void PrintDebugProc(const char *fmt, ...);
bool SetHiragana(bool hiragana = true);

#endif