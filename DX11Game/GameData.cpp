//************************************************************************************
// 
// ゲームデータ管理[GameData.cpp]
// 編集者：伊地田真衣
// 
//************************************************************************************

//-------------------- インクルード部 --------------------
#include <stdio.h>
#include "Gamedata.h"
#include "ranking.h"
#include "Majanpai.h"
#include "SelectMode.h"

//-------------------- 定数定義 --------------------
#define RANKFILENAME		("ranking.bin")
#define BOARDFILENAME		("board.bin")
#define MAXBOARD			(31*17*5)

//-------------------- グローバル変数 --------------------
static 	int rankTime[MAX_RANKING] = {
			60, 120, 180, 240, 300,
};


//====================================================================================
//
//				初期化
//
//====================================================================================
HRESULT	InitData() {
	HRESULT hr = S_OK;
	FILE *fp = NULL;
	
	fopen_s(&fp,RANKFILENAME, "rb");
	if (fp) {
		fread(rankTime, sizeof(int), MAX_RANKING, fp);

	}else{
		fopen_s(&fp, RANKFILENAME, "wb");
		fwrite(rankTime, sizeof(int), MAX_RANKING, fp);
		fclose(fp);
	}
	fclose(fp);

	InitRankData();

	return hr;
}


//====================================================================================
//
//				終了
//
//====================================================================================
void	UninitData() {

}


//====================================================================================
//
//				セーブ
//
//====================================================================================
void	SaveRankData() {
	FILE *fp;
	int *savedata;
	fopen_s(&fp, RANKFILENAME, "wb");
	if (fp) {
		savedata = GetRanking();
		fwrite(savedata, sizeof(int), MAX_RANKING, fp);
	
	}

	fclose(fp);
}

//====================================================================================
//
//				セーブデータロード
//
//====================================================================================
int*	LoadRankData() {
	FILE *fp;
	fopen_s(&fp, RANKFILENAME, "rb");
	if (fp) {
		fread(rankTime, sizeof(int), MAX_RANKING, fp);
	}

	fclose(fp);

	return &rankTime[0];
}


//====================================================================================
//
//				盤面読込
//
//====================================================================================
int*	LoadBoardDate() {
	int mode = GetMode();
	bool bitshift = false;
	int SelsectTilesPos[TILES_POS_CNT_Z][TILES_POS_CNT_Y][TILES_POS_CNT_X];
	FILE *fp;
	if (mode == 0) {
		bitshift = false;
	} else {
		bitshift = true;
	}
	fopen_s(&fp, BOARDFILENAME, "rb");
	if (fp) {
		if (bitshift) {
			fseek(fp, MAXBOARD * sizeof(int), SEEK_SET);
		}
		fread(SelsectTilesPos, sizeof(int), MAXBOARD, fp);
	}

	fclose(fp);


	return &SelsectTilesPos[0][0][0];
}