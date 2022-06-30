//************************************************************************************
// 
// �����\��[number.h]
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#ifndef __NUMBER_H__
#define __NUMBER_H__
//-------------------- �C���N���[�h�� --------------------
#include "main.h"

//-------------------- �萔��` --------------------
#define NUMBER_SIZE_X	(400.0f / 10)	// ������
#define NUMBER_SIZE_Y	(70.0f)			// ��������

#define NUMBER_COUNT_X	10
#define NUMBER_COUNT_Y	2


//-------------------- �v���g�^�C�v�錾 --------------------
HRESULT InitNumber();
void UninitNumber();
//void UpdateNumber();	����͒�`���Ȃ�
void DrawNumber(XMFLOAT2 vPos,					// �\���ʒu�i����j
				unsigned nNumber,				// �\�����l
				int nWidth,						// �\������
				float fSizeX = NUMBER_SIZE_X,	// ������
				float fSizeY = NUMBER_SIZE_Y);	// ��������

#endif // !__NUMBER_H__