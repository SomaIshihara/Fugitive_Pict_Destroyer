//======================================================
//
//移動ポイント[point.cpp]
//Author:石原颯馬
//
//======================================================
#include "point.h"
#include "building.h"
#include "pict.h"
#include "objectX.h"

//静的メンバ変数
CPoint* CPoint::m_pTop = NULL;
CPoint* CPoint::m_pCur = NULL;
int CPoint::m_nNumAll = 0;

#define POINT_INTERVAL	(1.2f)	//間隔

//=================================
//コンストラクタ
//=================================
CPoint::CPoint()
{
	if (m_pCur == NULL)
	{//最後尾がいない（すなわち先頭もいない）
		m_pTop = this;		//俺が先頭
		m_pPrev = NULL;		//前後誰もいない
		m_pNext = NULL;
	}
	else
	{//最後尾がいる
		m_pPrev = m_pCur;		//最後尾が自分の前のオブジェ
		m_pCur->m_pNext = this;	//最後尾の次のオブジェが自分
		m_pNext = NULL;			//自分の次のオブジェはいない
	}
	m_pCur = this;	//俺が最後尾
	m_nNumAll++;
}

//=================================
//デストラクタ（デフォルト）
//=================================
CPoint::~CPoint()
{
}

//=================================
//初期化
//=================================
HRESULT CPoint::Init(void)
{
	return S_OK;
}

//=================================
//すべて終了
//=================================
void CPoint::ReleaseAll(void)
{
	CPoint* pPoint = m_pTop;	//先頭を入れる

	while (pPoint != NULL)
	{//最後尾まで回し続ける
		CPoint* pPointNext = pPoint->m_pNext;	//次のオブジェ保存

		if (pPoint->m_pPrev != NULL)
		{//前にオブジェがいる
			pPoint->m_pPrev->m_pNext = pPoint->m_pNext;	//前のオブジェの次のオブジェは自分の次のオブジェ
		}
		if (pPoint->m_pNext != NULL)
		{
			pPoint->m_pNext->m_pPrev = pPoint->m_pPrev;	//次のオブジェの前のオブジェは自分の前のオブジェ
		}

		if (m_pCur == pPoint)
		{//最後尾でした
			m_pCur = pPoint->m_pPrev;	//最後尾を自分の前のオブジェにする
		}
		if (m_pTop == pPoint)
		{
			m_pTop = pPoint->m_pNext;	//先頭を自分の次のオブジェにする
		}

		//成仏	
		delete pPoint;	//自分自身破棄
		m_nNumAll--;	//総数減らす

		pPoint = pPointNext;	//次を入れる
	}
}

//=================================
//更新
//=================================
void CPoint::Update(void)
{
	ReleaseAll();	//いったん消す

	//建物に応じて生成
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{
		CBuilding* pBuilding = CBuilding::GetBuilding(cnt);

		if (pBuilding != NULL)
		{//建物ある
			D3DXVECTOR3 pos = pBuilding->GetPos();				//位置取得
			float fWidthHalf = pBuilding->GetWidth() * 0.5f;	//幅取得
			float fDepthHalf = pBuilding->GetDepth() * 0.5f;	//奥行取得

			CPoint::Create(pos + D3DXVECTOR3(-fWidthHalf * POINT_INTERVAL, 0.0f, -fDepthHalf * POINT_INTERVAL));
			CPoint::Create(pos + D3DXVECTOR3(fWidthHalf * POINT_INTERVAL, 0.0f, -fDepthHalf * POINT_INTERVAL));
			CPoint::Create(pos + D3DXVECTOR3(-fWidthHalf * POINT_INTERVAL, 0.0f, fDepthHalf * POINT_INTERVAL));
			CPoint::Create(pos + D3DXVECTOR3(fWidthHalf * POINT_INTERVAL, 0.0f, fDepthHalf * POINT_INTERVAL));
		}
	}

	//アジト
	CObject* pBuilding = CPict::GetAgit();

	if (pBuilding != NULL)
	{//建物ある
		D3DXVECTOR3 pos = pBuilding->GetPos();				//位置取得
		float fWidthHalf = pBuilding->GetWidth() * 0.5f;	//幅取得
		float fDepthHalf = pBuilding->GetDepth() * 0.5f;	//奥行取得

		CPoint::Create(pos + D3DXVECTOR3(-fWidthHalf * POINT_INTERVAL, 0.0f, -fDepthHalf * POINT_INTERVAL));
		CPoint::Create(pos + D3DXVECTOR3(fWidthHalf * POINT_INTERVAL, 0.0f, -fDepthHalf * POINT_INTERVAL));
		CPoint::Create(pos + D3DXVECTOR3(-fWidthHalf * POINT_INTERVAL, 0.0f, fDepthHalf * POINT_INTERVAL));
		CPoint::Create(pos + D3DXVECTOR3(fWidthHalf * POINT_INTERVAL, 0.0f, fDepthHalf * POINT_INTERVAL));
	}
}

//=================================
//生成
//=================================
CPoint* CPoint::Create(const D3DXVECTOR3 pos)
{
	CPoint* pPoint = NULL;

	if (pPoint == NULL)
	{
		//オブジェクトアニメーション2Dの生成
		pPoint = new CPoint;

		//初期化
		pPoint->Init();
		pPoint->m_pos = pos;

		return pPoint;
	}
	else
	{
		return NULL;
	}
}

//=================================
//探索
//=================================
CPoint* CPoint::Search(const D3DXVECTOR3 pos, const CPoint* pNowPoint)
{
	CPoint* pPoint = m_pTop;	//先頭を入れる
	CPoint* pPointNear = NULL;
	float fLength = 0.0f;

	while (pPoint != NULL)
	{//最後尾まで回し続ける
		CPoint* pPointNext = pPoint->m_pNext;	//次のオブジェ保存
		
		//距離測る
		float fLengthNow = D3DXVec3Length(&(pPoint->m_pos - pos));
		if (pPointNear == NULL || fLength > fLengthNow)
		{//まっさら
			pPointNear = pPoint;
			fLength = fLengthNow;
		}

		pPoint = pPointNext;	//次を入れる
	}
	return pPointNear;
}