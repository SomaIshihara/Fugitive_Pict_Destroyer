//======================================================
//
//当たり判定処理の[collision.cpp]
//Author:石原颯馬
//
//======================================================
#include "collision.h"
#include "Culc.h"
#include "manager.h"
#include "objectBillboard.h"

//=================================
//コンストラクタ
//=================================
CCollision::CCollision()
{
	m_vtxMin = CManager::VEC3_ZERO;
	m_vtxMax = CManager::VEC3_ZERO;
}

//=================================
//デストラクタ
//=================================
CCollision::~CCollision()
{
}

//=================================
//当たり判定チェック
//=================================
bool CCollision::CollisionCheck(const D3DXVECTOR3 posOld, const D3DXVECTOR3 posNew, const D3DXVECTOR3 posCenter, const D3DXVECTOR3 rot)
{
	//前側面
	if (CollisionPolygon(posOld, posNew, posCenter, rot, D3DXVECTOR3(0.0f, 0.0f, -1.0f),
		D3DXVECTOR3(m_vtxMin.x, m_vtxMin.y, m_vtxMin.z), D3DXVECTOR3(m_vtxMax.x, m_vtxMin.y, m_vtxMin.z),
		D3DXVECTOR3(m_vtxMax.x, m_vtxMax.y, m_vtxMin.z), D3DXVECTOR3(m_vtxMin.x, m_vtxMax.y, m_vtxMin.z)) == true)
	{
		return true;
	}

	//右側面
	if (CollisionPolygon(posOld, posNew, posCenter, rot, D3DXVECTOR3(1.0f, 0.0f, 0.0f),
		D3DXVECTOR3(m_vtxMin.x, m_vtxMin.y, m_vtxMin.z), D3DXVECTOR3(m_vtxMax.x, m_vtxMin.y, m_vtxMin.z),
		D3DXVECTOR3(m_vtxMax.x, m_vtxMax.y, m_vtxMin.z), D3DXVECTOR3(m_vtxMin.x, m_vtxMax.y, m_vtxMin.z)) == true)
	{
		return true;
	}

	//奥側面
	if (CollisionPolygon(posOld, posNew, posCenter, rot, D3DXVECTOR3(0.0f, 0.0f, 1.0f),
		D3DXVECTOR3(m_vtxMax.x, m_vtxMin.y, m_vtxMax.z), D3DXVECTOR3(m_vtxMin.x, m_vtxMin.y, m_vtxMax.z),
		D3DXVECTOR3(m_vtxMin.x, m_vtxMax.y, m_vtxMax.z), D3DXVECTOR3(m_vtxMax.x, m_vtxMax.y, m_vtxMax.z)) == true)
	{
		return true;
	}

	//左側面
	if (CollisionPolygon(posOld, posNew, posCenter, rot, D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
		D3DXVECTOR3(m_vtxMin.x, m_vtxMin.y, m_vtxMax.z), D3DXVECTOR3(m_vtxMin.x, m_vtxMin.y, m_vtxMin.z),
		D3DXVECTOR3(m_vtxMin.x, m_vtxMax.y, m_vtxMin.z), D3DXVECTOR3(m_vtxMin.x, m_vtxMax.y, m_vtxMax.z)) == true)
	{
		return true;
	}

	//上側面
	if (CollisionPolygon(posOld, posNew, posCenter, rot, D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		D3DXVECTOR3(m_vtxMin.x, m_vtxMax.y, m_vtxMin.z), D3DXVECTOR3(m_vtxMax.x, m_vtxMax.y, m_vtxMin.z),
		D3DXVECTOR3(m_vtxMax.x, m_vtxMax.y, m_vtxMax.z), D3DXVECTOR3(m_vtxMin.x, m_vtxMax.y, m_vtxMax.z)) == true)
	{
		return true;
	}

	//下側面
	if (CollisionPolygon(posOld, posNew, posCenter, rot, D3DXVECTOR3(0.0f, -1.0f, 0.0f),
		D3DXVECTOR3(m_vtxMin.x, m_vtxMin.y, m_vtxMin.z), D3DXVECTOR3(m_vtxMax.x, m_vtxMin.y, m_vtxMin.z),
		D3DXVECTOR3(m_vtxMax.x, m_vtxMin.y, m_vtxMax.z), D3DXVECTOR3(m_vtxMin.x, m_vtxMin.y, m_vtxMax.z)) == true)
	{
		return true;
	}

	return false;
}

//=================================
//ポリゴンごとの当たり判定チェック
//=================================
bool CCollision::CollisionPolygon(const D3DXVECTOR3 posOld, const D3DXVECTOR3 posNew, const D3DXVECTOR3 posCenter, const D3DXVECTOR3 rot,
	const D3DXVECTOR3 vecNor, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 pos3)
{
	//頂点の回転に必要な変数
	//各頂点を求めるのに必要な変数
	D3DXMATRIX mtxWorld;	//回転のベースとなるワールドマトリ
	D3DXMATRIX mtxRot;		//回転行列
	D3DXMATRIX mtxTrans;	//すべて変換後の行列
	D3DXVECTOR3 vtxTrans;	//変換後の点

	//頂点の回転
	pos0 = RotateVtx(pos0, rot, posCenter);	//pos0
	pos1 = RotateVtx(pos1, rot, posCenter);	//pos1
	pos2 = RotateVtx(pos2, rot, posCenter);	//pos2
	pos3 = RotateVtx(pos3, rot, posCenter);	//pos3

	//当たり判定
	if (D3DXVec3Dot(&posOld, &vecNor) * D3DXVec3Dot(&posNew, &vecNor) <= 0)
	{
		//ポリゴン貫通しているか
		D3DXVECTOR3 ray = posNew - posOld;			//レイ
		D3DXVec3Normalize(&ray, &ray);				//レイを正規化
		float fU, fV, fD;							//ごみ箱

		if (D3DXIntersectTri(&pos0, &pos1, &pos3, &posOld, &ray, &fU, &fV, &fD) == true || D3DXIntersectTri(&pos1, &pos2, &pos3, &posOld, &ray, &fU, &fV, &fD) == true)
		{//四角形を三角形に分割し、どちらかtrue帰ってきたら選択できた
			return true;
		}
	}
	
	return false;
}
