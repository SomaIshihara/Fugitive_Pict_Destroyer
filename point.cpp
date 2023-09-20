//======================================================
//
//移動ポイント[point.cpp]
//Author:石原颯馬
//
//======================================================
#include "point.h"
#include "building.h"
#include "picto.h"
#include "objectX.h"
#include "koban.h"
#include "Culc.h"
#include <vector>

//静的メンバ変数
CPoint* CPoint::m_pTop = nullptr;
CPoint* CPoint::m_pCur = nullptr;
int CPoint::m_nNumAll = 0;
const float CPoint::MERGE_LENGTH = 40.0f;
const float CPoint::POINT_INTERVAL = 20.0f;

//=================================
//コンストラクタ
//=================================
CPoint::CPoint()
{
	if (m_pCur == nullptr)
	{//最後尾がいない（すなわち先頭もいない）
		m_pTop = this;		//俺が先頭
		m_pPrev = nullptr;		//前後誰もいない
		m_pNext = nullptr;
	}
	else
	{//最後尾がいる
		m_pPrev = m_pCur;		//最後尾が自分の前のオブジェ
		m_pCur->m_pNext = this;	//最後尾の次のオブジェが自分
		m_pNext = nullptr;			//自分の次のオブジェはいない
	}
	m_bDeath = false;
	m_pCur = this;	//俺が最後尾
	m_nNumAll++;
}

//=================================
//デストラクタ
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

	while (pPoint != nullptr)
	{//最後尾まで回し続ける
		CPoint* pPointNext = pPoint->m_pNext;	//次のオブジェ保存

		pPoint->Release();		//破棄

		pPoint = pPointNext;	//次を入れる
	}

	Death();	//全部へし折る
}

//=================================
//終了
//=================================
void CPoint::Release(void)
{
	m_bDeath = true;	//死亡フラグにする
}

//=================================
//更新
//=================================
void CPoint::Update(void)
{
	ReleaseAll();	//いったん消す

	//建物に応じて生成
	for (int cnt = 0; cnt < CBuilding::GetNumAll(); cnt++)
	{
		CBuilding* pBuilding = CBuilding::GetBuilding(cnt);

		if (pBuilding != nullptr)
		{//建物ある
			D3DXVECTOR3 pos = pBuilding->GetPos();				//位置取得
			float fWidthHalf = pBuilding->GetWidth() * 0.5f;	//幅取得
			float fDepthHalf = pBuilding->GetDepth() * 0.5f;	//奥行取得

			CPoint::Create(pos + D3DXVECTOR3(-(fWidthHalf + POINT_INTERVAL), 0.0f, -(fDepthHalf + POINT_INTERVAL)));
			CPoint::Create(pos + D3DXVECTOR3((fWidthHalf + POINT_INTERVAL), 0.0f, -(fDepthHalf + POINT_INTERVAL)));
			CPoint::Create(pos + D3DXVECTOR3(-(fWidthHalf + POINT_INTERVAL), 0.0f, (fDepthHalf + POINT_INTERVAL)));
			CPoint::Create(pos + D3DXVECTOR3((fWidthHalf + POINT_INTERVAL), 0.0f, (fDepthHalf + POINT_INTERVAL)));
		}
	}

	//アジト
	CObject* pBuilding = CPicto::GetAgit();

	if (pBuilding != nullptr)
	{//建物ある
		D3DXVECTOR3 pos = pBuilding->GetPos();				//位置取得
		float fWidthHalf = pBuilding->GetWidth() * 0.5f;	//幅取得
		float fDepthHalf = pBuilding->GetDepth() * 0.5f;	//奥行取得

		CPoint::Create(pos + D3DXVECTOR3(-(fWidthHalf + POINT_INTERVAL), 0.0f, -(fDepthHalf + POINT_INTERVAL)));
		CPoint::Create(pos + D3DXVECTOR3((fWidthHalf + POINT_INTERVAL), 0.0f, -(fDepthHalf + POINT_INTERVAL)));
		CPoint::Create(pos + D3DXVECTOR3(-(fWidthHalf + POINT_INTERVAL), 0.0f, (fDepthHalf + POINT_INTERVAL)));
		CPoint::Create(pos + D3DXVECTOR3((fWidthHalf + POINT_INTERVAL), 0.0f, (fDepthHalf + POINT_INTERVAL)));
	}

	//交番
	for (int cnt = 0; cnt < CKoban::GetNumAll(); cnt++)
	{
		CKoban* pKoban = CKoban::GetKoban(cnt);

		if (pKoban != nullptr)
		{//建物ある
			D3DXVECTOR3 pos = pKoban->GetPos();				//位置取得
			float fWidthHalf = pKoban->GetWidth() * 0.5f;	//幅取得
			float fDepthHalf = pKoban->GetDepth() * 0.5f;	//奥行取得

			CPoint::Create(pos + D3DXVECTOR3(-(fWidthHalf + POINT_INTERVAL), 0.0f, -(fDepthHalf + POINT_INTERVAL)));
			CPoint::Create(pos + D3DXVECTOR3((fWidthHalf + POINT_INTERVAL), 0.0f, -(fDepthHalf + POINT_INTERVAL)));
			CPoint::Create(pos + D3DXVECTOR3(-(fWidthHalf + POINT_INTERVAL), 0.0f, (fDepthHalf + POINT_INTERVAL)));
			CPoint::Create(pos + D3DXVECTOR3((fWidthHalf + POINT_INTERVAL), 0.0f, (fDepthHalf + POINT_INTERVAL)));
		}
	}

	//すべてのポイントに対して、建物に埋もれていないか確認
	CPoint* pPoint = CPoint::GetTop();	//先頭取得
	while (pPoint != nullptr)
	{
		CPoint* pPointNext = pPoint->GetNext();	//次取得

		//建物の分だけ確認
		for (int cnt = 0; cnt < CBuilding::GetNumAll(); cnt++)
		{
			CBuilding* pBuilding = CBuilding::GetBuilding(cnt);

			if (pBuilding != nullptr)
			{//ある
				D3DXVECTOR3 posBuild = pBuilding->GetPos();			//建物位置取得
				D3DXVECTOR3 posPoint = pPoint->m_pos;				//ポイント位置取得
				float fWidthHalf = pBuilding->GetWidth() * 0.5f;	//サイズ取得
				float fDepthHalf = pBuilding->GetDepth() * 0.5f;

				if (posPoint.x > posBuild.x - fWidthHalf &&
					posPoint.x < posBuild.x + fWidthHalf &&
					posPoint.z > posBuild.z - fDepthHalf &&
					posPoint.z < posBuild.z + fDepthHalf)
				{//オボボボボ（埋もれてる）
					pPoint->m_pFrag->Uninit();	//旗をへし折る
					pPoint->Release();			//死
					break;						//for文終了
				}
			}
		}

		pPoint = pPointNext;	//次入れる
	}

	//まとめてへし折る
	Death();

	//近すぎるポイントをマージ（その際、死亡フラグと化したポイントは無視すること）
	pPoint = CPoint::GetTop();	//先頭取得
	while (pPoint != nullptr)
	{
		CPoint* pPointNext = pPoint->GetNext();	//次取得

		if (pPoint->m_bDeath == false)
		{//普通の旗
			//2つ目のポイント
			std::vector<CPoint*> vector;
			CPoint* pPointSecond = CPoint::GetTop();	//先頭取得
			while (pPointSecond != nullptr)
			{
				CPoint* pPointSecondNext = pPointSecond->GetNext();	//次取得

				if (pPointSecond != pPoint && pPointSecond->m_bDeath == false && D3DXVec3Length(&(pPoint->GetPos() - pPointSecond->GetPos())) < MERGE_LENGTH)
				{//範囲内で普通の旗
					vector.emplace_back(pPointSecond);	//マージリストに入れる
				}

				pPointSecond = pPointSecondNext;	//次入れる
			}

			if (vector.size() > 0)
			{//近いのが1つでもある
				//マージ処理（近いポイント同士すべて死亡フラグにしてその間に新しくポイントを置く）
				D3DXVECTOR3 posAverage = pPoint->GetPos();	//位置もらう
				pPoint->m_pFrag->Uninit();					//旗をへし折る
				pPoint->Release();							//死亡フラグにする
				for (int cnt = 0; cnt < vector.size(); cnt++)
				{
					posAverage += vector[cnt]->GetPos();	//頂点を入れる
					vector[cnt]->m_pFrag->Uninit();			//旗をへし折る
					vector[cnt]->Release();					//死亡フラグにする
				}
				posAverage /= (vector.size() + 1);	//平均取る
				CPoint::Create(posAverage);			//新しく置く
			}
		}

		pPoint = pPointNext;	//次入れる
	}

	//まとめてへし折る
	Death();

	//ポイント接続
	pPoint = CPoint::GetTop();	//先頭取得
	while (pPoint != nullptr)
	{//設定
		CPoint* pPointSecond = CPoint::GetTop();

		while (pPointSecond != nullptr)
		{
			if (pPoint != pPointSecond)
			{//今格納している奴じゃない
				D3DXVECTOR3 pos = pPoint->GetPos();
				D3DXVECTOR3 posSecond = pPointSecond->GetPos();
				D3DXVECTOR3 vecPoint = posSecond - pos;

				bool bCollision = false;
				for (int cnt = 0; cnt < CBuilding::GetNumAll(); cnt++)
				{
					CBuilding* pBuilding = CBuilding::GetBuilding(cnt);
					if (pBuilding != nullptr)
					{
						D3DXVECTOR3 posBuilding = pBuilding->GetPos();
						float fWidthHalf = pBuilding->GetWidth() * 0.5f;
						float fDepthHalf = pBuilding->GetDepth() * 0.5f;

						//4頂点作る
						D3DXVECTOR3 posBuild[4];
						posBuild[0] = posBuilding + D3DXVECTOR3(-fWidthHalf, 0.0f, -fDepthHalf);
						posBuild[1] = posBuilding + D3DXVECTOR3(fWidthHalf, 0.0f, -fDepthHalf);
						posBuild[2] = posBuilding + D3DXVECTOR3(fWidthHalf, 0.0f, fDepthHalf);
						posBuild[3] = posBuilding + D3DXVECTOR3(-fWidthHalf, 0.0f, fDepthHalf);

						//プラスマイナスがあったかのフラッグ
						for (int cntPos = 0; cntPos < 4; cntPos++)
						{
							D3DXVECTOR3 vecLine = (posBuild[(cntPos + 1) % 4] - posBuild[cntPos]);
							D3DXVECTOR3 vecToPosOld = pos - posBuild[cntPos];
							D3DXVECTOR3 vecToPos = posSecond - posBuild[cntPos];
							if (TASUKIGAKE(vecLine.x, vecLine.z, vecToPosOld.x, vecToPosOld.z) >= 0.0f && TASUKIGAKE(vecLine.x, vecLine.z, vecToPos.x, vecToPos.z) < 0.0f)
							{//当たった
								float fAreaA = (vecToPos.z * vecPoint.x) - (vecToPos.x * vecPoint.z);
								float fAreaB = (vecLine.z * vecPoint.x) - (vecLine.x * vecPoint.z);
								if (fAreaA / fAreaB >= 0.0f && fAreaA / fAreaB <= 1.0f)
								{//ごっつん
									bCollision = true;	//衝突した
									break;	//もう当たったので終了
								}
							}
						}

						if (bCollision == true)
						{
							break;
						}
					}
				}

				if (bCollision == false)
				{//当たってない
					pPoint->m_connectPoint.emplace_back(pPointSecond);
				}
			}
			pPointSecond = pPointSecond->GetNext();	//次入れる
		}

		pPoint = pPoint->GetNext();	//次入れる
	}
}

//=================================
//生成
//=================================
CPoint* CPoint::Create(const D3DXVECTOR3 pos)
{
	CPoint* pPoint = nullptr;

	if (pPoint == nullptr)
	{
		//ポイントの生成
		pPoint = new CPoint;

		//初期化
		pPoint->Init();
		pPoint->m_pos = pos;
		pPoint->m_pFrag = CObjectX::Create(pos, CManager::VEC3_ZERO, CManager::GetFragModel());

		return pPoint;
	}
	else
	{
		return nullptr;
	}
}

//=================================
//探索
//=================================
CPoint* CPoint::Search(const D3DXVECTOR3 pos, const CPoint* pNowPoint)
{
	CPoint* pPoint = m_pTop;	//先頭を入れる
	CPoint* pPointNear = nullptr;
	float fLength = 0.0f;

	while (pPoint != nullptr)
	{//最後尾まで回し続ける
		CPoint* pPointNext = pPoint->m_pNext;	//次のオブジェ保存
		
		//距離測る
		float fLengthNow = D3DXVec3Length(&(pPoint->m_pos - pos));
		if (pPointNear == nullptr || fLength > fLengthNow)
		{//まっさら
			pPointNear = pPoint;
			fLength = fLengthNow;
		}

		pPoint = pPointNext;	//次を入れる
	}
	return pPointNear;
}

//=================================
//死亡フラグと化した旗をへし折る
//=================================
void CPoint::Death(void)
{
	CPoint* pPoint = m_pTop;	//先頭を入れる
	while (pPoint != nullptr)
	{//最後尾まで回し続ける
		CPoint* pPointNext = pPoint->m_pNext;	//次のオブジェ保存

		if (pPoint->m_bDeath == true)
		{//これは死亡フラグです
			if (pPoint->m_pPrev != nullptr)
			{//前にオブジェがいる
				pPoint->m_pPrev->m_pNext = pPoint->m_pNext;	//前のオブジェの次のオブジェは自分の次のオブジェ
			}
			if (pPoint->m_pNext != nullptr)
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
		}

		pPoint = pPointNext;	//次を入れる
	}
}
