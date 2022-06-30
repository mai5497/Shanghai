//************************************************************************************
// 
// �V�[���Ǘ�[Scene.h]
// �ҏW�ҁF�ɒn�c�^��
// 
//************************************************************************************
#ifndef __SCENE_H__
#define __SCENE_H__
//-------------------- �C���N���[�h�� --------------------
#include "main.h"

//-------------------- �萔��` --------------------
enum EScene {
	SCENE_NONE = 0,	// ���ݒ�
	SCENE_TITLE,	// �^�C�g�����
	SCENE_MODE,		// ���[�h�I�����
	SCENE_HOWTOPLAY,// �V�ѕ� 
	SCENE_GAME,		// �Q�[�����
	SCENE_RESULT,	// ���ʔ��\
	SCENE_RANKING,	// �����L���O	

	MAX_SCENE		// �V�[������
};

//-------------------- �v���g�^�C�v�錾 --------------------
HRESULT InitScene();			// ������
void UninitScene();				// �I������
void UpdateScene();				// �X�V
void DrawScene();				// �`��

void SetScene(EScene Escene);	// �؂�ւ�
int GetScene();					// �V�[���ԍ��Ԃ�

#endif
