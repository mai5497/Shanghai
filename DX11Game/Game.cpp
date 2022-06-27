//----- �C���N���[�h�� -----
#include "Game.h"
#include "Scene.h"

#include "Sound.h"

#include "bg.h"
#include "Majanpai.h"
#include "timer.h"
#include "UI.h"


//------------------------------------------------------------------------------------
//
//				������
//
//------------------------------------------------------------------------------------
HRESULT InitGame() {
	HRESULT hr = S_OK;

	// �����v������
	hr = InitMajanpai();
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("�����v���������s"), NULL, MB_OK);
		return hr;
	}

	// �w�i������
	hr = InitBG();
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("�w�i���������s"), NULL, MB_OK);
		return hr;
	}

	// �^�C�}�[
	hr = InitTimer();
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("�^�C�}�[���������s"), NULL, MB_OK);
		return hr;
	}

	// UI
	hr = InitUI();
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("UI���������s"), NULL, MB_OK);
		return hr;
	}

	// BGM�Đ�
	CSound::Play(BGM_001);


	return hr;
}


//------------------------------------------------------------------------------------
//
//				�I��
//
//------------------------------------------------------------------------------------
void UninitGame() {
	// BGM�Đ���~
	CSound::Stop(BGM_001);

	// �w�i�I������
	UninitBG();

	// �����v�I������
	UninitMajanpai();

	// �^�C�}�[�I������
	UninitTimer();

	// UI�I������
	UninitUI();
}


//------------------------------------------------------------------------------------
//
//				�X�V
//
//------------------------------------------------------------------------------------
void UpdateGame() {
	// �w�i�`��X�V
	UpdateBG();

	// �^�C�}�[�X�V
	UpdateTimer();

	// �����v�X�V
	UpdateMajanpai();
}


//------------------------------------------------------------------------------------
//
//				�`��
//
//------------------------------------------------------------------------------------
void DrawGame() {
	// �w�i�`��
	DrawBG();

	// �^�C�}�[�`��
	DrawTimer();

	// UI�`��
	DrawUI();

	// �����v�`��
	DrawMajanpai();

}