//==========================================
//
//計算プログラムのヘッダ[Culc.h]
//Author:石原颯馬
//
//==========================================
#ifndef _CULC_H_
#define _CULC_H_
#include "main.h"

//マクロ
#define ACCELERATION_GRAVITY	(9.8f)	//重力加速度
#define TWO_PI		(2 * D3DX_PI)		//360度（2π[rad]）

//方向
#define LEFT_UP		(-0.75f * D3DX_PI)	//左上
#define RIGHT_UP	(0.75f * D3DX_PI)	//右上
#define LEFT_DOWN	(-0.25f * D3DX_PI)	//左下
#define RIGHT_DOWN	(0.25f * D3DX_PI)	//右下
#define UP			(1.0f * D3DX_PI)	//真上
#define DOWN		(0.0f * D3DX_PI)	//真下
#define LEFT		(-0.5f * D3DX_PI)	//左
#define RIGHT		(0.5f * D3DX_PI)	//右

//便利マクロ関数
#define FIX_ROT(x)				(fmodf(x + (D3DX_PI * 3), D3DX_PI * 2) - D3DX_PI)	//角度を-PI~PIに修正
#define fatof(x)				(float)atof(x)										//通常のatofだとdouble型で返してくるのでfloatキャストも行うようにしたもの
#define TASUKIGAKE(ax,az,bx,bz)	((az * bx) - (ax * bz))								//名前の通り。それ以上の説明はない。
#define PYTHAGORAS(a,b)			sqrtf(powf(a,2) + powf(b,2))						//三平方の定理

//計算関数
D3DXVECTOR3 RotateVtx(const D3DXVECTOR3 vtx, const D3DXVECTOR3 rot, const D3DXVECTOR3 posCenter);

#endif // !_BILLBOARD_H_
