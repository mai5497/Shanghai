//************************************************************************************
// 
// �f�o�b�O�\��[debugproc.h]
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#ifndef __DEBUGPROC_H__
#define __DEBUGPROC_H__
//-------------------- �C���N���[�h�� --------------------
#include "main.h"

//-------------------- �v���g�^�C�v�錾 --------------------
HRESULT InitDebugProc(void);
void UninitDebugProc(void);
void UpdateDebugProc(void);
void DrawDebugProc(void);
void StartDebugProc(bool hiragana = false);
void PrintDebugProc(const char *fmt, ...);
bool SetHiragana(bool hiragana = true);

#endif