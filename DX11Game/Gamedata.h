#pragma once
//----- �C���N���[�h�� -----
#include "main.h"


//===========================
//
//	�v���g�^�C�v�錾
//
//===========================
HRESULT	InitData();			// ������
void	UninitData();		// �I��
void	SaveRankData();		// �����L���O�Z�[�u
int*	LoadRankData();		// �����L���O���[�h
int*	LoadBoardDate();	// �{�[�h�f�[�^���[�h