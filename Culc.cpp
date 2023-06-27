//======================================================
//
//計算プログラム[Culc.cpp]
//Author:石原颯馬
//
//======================================================
#include "Culc.h"

D3DXVECTOR3 RotateVtx(const D3DXVECTOR3 vtx, const D3DXVECTOR3 rot, const D3DXVECTOR3 posCenter)
{
	//頂点の回転に必要な変数
	//各頂点を求めるのに必要な変数
	D3DXMATRIX mtxWorld;	//回転のベースとなるワールドマトリ
	D3DXMATRIX mtxRot;		//回転行列
	D3DXMATRIX mtxTrans;	//すべて変換後の行列
	D3DXVECTOR3 vtxTrans;	//変換後の点

	//頂点の回転
	//-mtx----------------------------------------------------------------------------------------------------------------------------
	//回転行列を作る
	D3DXMatrixIdentity(&mtxWorld);

	//向き反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxRot, &mtxWorld);

	//位置反映
	mtxWorld._41 = posCenter.x;
	mtxWorld._42 = posCenter.y;
	mtxWorld._43 = posCenter.z;

	//-mtx----------------------------------------------------------------------------------------------------------------------------

	//-pos0---------------------------------------------------------------------------------------------------------------------------
	//回転行列をもとに頂点を回転する
	//ベースをコピー
	mtxTrans = mtxWorld;

	//0度のときの点を置く
	D3DXVECTOR3 vtxPos = vtx;

	//回転行列とかけ合わせる
	D3DXVec3TransformCoord(&vtxTrans, &vtxPos, &mtxTrans);

	//-pos0---------------------------------------------------------------------------------------------------------------------------

	return vtxTrans;
}
