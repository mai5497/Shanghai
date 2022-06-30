//************************************************************************************
// 
// ポリゴン[polygon.h]
// 編集者：伊地田真衣
// 
//************************************************************************************
#ifndef __POLYGON_H__
#define __POLYGON_H__
//-------------------- インクルード部 --------------------
#include "main.h"
//-------------------- プロトタイプ宣言 --------------------
HRESULT InitPolygon(ID3D11Device* pDevice);
void UninitPolygon(void);
void UpdatePolygon(void);
void DrawPolygon(ID3D11DeviceContext* pDeviceContext);
void SetPolygonTexture(ID3D11ShaderResourceView* pTexture);
void SetPolygonPos(float fX, float fY);
void SetPolygonSize(float fScaleX, float fScaleY);
void SetPolygonAngle(float fAngle);
void SetPolygonUV(float fU, float fV);
void SetPolygonFrameSize(float fWidth, float fHeight);
void SetPolygonColor(float fRed, float fGreen, float fBlue);
void SetPolygonAlpha(float fAlpha);

#endif