//�����L���O
#pragma once
#include "main.h"

#define MAX_RANKING			(5)


//===========================
//
//	�v���g�^�C�v�錾
//
//===========================
HRESULT	InitRanking();			// ������
void	InitRankData();			// �f�[�^�̓ǂݍ���
void	UninitRanking();		// �I��
void	UpdateRanking();		// �X�V
void	DrawRanking();			// �`��

void SetRanking(int nScore);	// �����L���O�X�V
int* GetRanking();				// �����L���O�擾