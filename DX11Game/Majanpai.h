//�C���N���[�h�K�[�h
#ifndef __MAJANPAI_H__
#define __MAJANPAI_H__

// �C���N���[�h��
#include "main.h"

// �萔��`
#define TILES_POS_CNT_X		(31)										// �v�̔z�u�z��̉���
#define TILES_POS_CNT_Y		(17)										// �v�̔z�u�z��̏c��
#define	TILES_POS_CNT_Z		(5)											// �v�̔z�u�z��̐[�x


// �\���̒�`
struct Mahjongtile {
	int tileNum;
	int tilePos[3];	// ���W�𒼐ړ����
	bool IsFree;	// ���R�v���ۂ�
	bool IsSelect;	// �I������Ă��邩�i�I������Ă����true�j
};

// �v���g�^�C�v�錾
HRESULT InitMajanpai();
void UninitMajanpai();
void UpdateMajanpai();
void DrawMajanpai();

int GetFreeCnt();
int GetTilesSum();
int GetDelTilesSum();


#endif