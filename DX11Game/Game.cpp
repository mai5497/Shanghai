//----- CN[h -----
#include "Game.h"
#include "Scene.h"

#include "Sound.h"

#include "bg.h"
#include "Majanpai.h"
#include "timer.h"
#include "UI.h"


//------------------------------------------------------------------------------------
//
//				ú»
//
//------------------------------------------------------------------------------------
HRESULT InitGame() {
	HRESULT hr = S_OK;

	// vú»
	hr = InitMajanpai();
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("vú»¸s"), NULL, MB_OK);
		return hr;
	}

	// wiú»
	hr = InitBG();
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("wiú»¸s"), NULL, MB_OK);
		return hr;
	}

	// ^C}[
	hr = InitTimer();
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("^C}[ú»¸s"), NULL, MB_OK);
		return hr;
	}

	// UI
	hr = InitUI();
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("UIú»¸s"), NULL, MB_OK);
		return hr;
	}

	// BGMÄ¶
	CSound::Play(BGM_001);


	return hr;
}


//------------------------------------------------------------------------------------
//
//				I¹
//
//------------------------------------------------------------------------------------
void UninitGame() {
	// BGMÄ¶â~
	CSound::Stop(BGM_001);

	// wiI¹
	UninitBG();

	// vI¹
	UninitMajanpai();

	// ^C}[I¹
	UninitTimer();

	// UII¹
	UninitUI();
}


//------------------------------------------------------------------------------------
//
//				XV
//
//------------------------------------------------------------------------------------
void UpdateGame() {
	// wi`æXV
	UpdateBG();

	// ^C}[XV
	UpdateTimer();

	// vXV
	UpdateMajanpai();
}


//------------------------------------------------------------------------------------
//
//				`æ
//
//------------------------------------------------------------------------------------
void DrawGame() {
	// wi`æ
	DrawBG();

	// ^C}[`æ
	DrawTimer();

	// UI`æ
	DrawUI();

	// v`æ
	DrawMajanpai();

}