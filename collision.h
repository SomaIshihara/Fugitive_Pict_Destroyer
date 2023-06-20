//======================================================
//
//当たり判定処理のヘッダ[collision.h]
//Author:石原颯馬
//
//======================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "main.h"

//当たり判定クラス
class CCollision
{
public:
	//コンストラクタ・デストラクタ
	CCollision();
	~CCollision();

	//当たり判定チェック
	bool CollisionCheck(const D3DXVECTOR3 posOld, const D3DXVECTOR3 posNew, const D3DXVECTOR3 posCenter, const D3DXVECTOR3 rot);

	//設定
	void SetVtx(const D3DXVECTOR3 vtxMin, const D3DXVECTOR3 vtxMax) { m_vtxMin = vtxMin, m_vtxMax = vtxMax; }

	//取得
	void GetVtx(D3DXVECTOR3* vtxMin, D3DXVECTOR3* vtxMax) { *vtxMin = m_vtxMin, *vtxMax = m_vtxMax; }

private:
	bool CollisionPolygon(const D3DXVECTOR3 posOld, const D3DXVECTOR3 posNew, const D3DXVECTOR3 posCenter, const D3DXVECTOR3 rot,
		const D3DXVECTOR3 vecNor, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 pos3);
	D3DXVECTOR3 m_vtxMin, m_vtxMax;	//当たり判定最低値・最高値
};

#endif // !_COLLISION_H_
