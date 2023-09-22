//======================================================
//
//警察マネージャ処理[policemanager.cpp]
//Author:石原颯馬
//
//======================================================
#include "policemanager.h"
#include "manager.h"
#include "picto.h"
#include "building.h"
#include "koban.h"
#include "score.h"
#include "game.h"
#include "picto.h"

//マクロ
#define KOBAN_BUILDING_SEARCH_NUM	(5)		//建物探索回数（これを過ぎると沸かない）
#define KOBAN_NOBUILDING_TIME_PERCE	(0.5f)	//建物が見つからなかった場合の探索時間割合

//前方宣言
int CPoliceManager::m_disPatchCT[] = {};
std::vector<int> CPoliceManager::m_AttackList;
int CPoliceManager::m_nCounterSpawn = CManager::INT_ZERO;
int CPoliceManager::m_nSpawnSpan = CManager::INT_ZERO;
int CPoliceManager::m_nPoliceMax = CManager::INT_ZERO;
int CPoliceManager::m_waitingPolice = CManager::INT_ZERO;
int CPoliceManager::m_nPatrollNum = CManager::INT_ZERO;

//=================================
//コンストラクタ（デフォルト）
//=================================
CPoliceManager::CPoliceManager(int nPriority)
{
	
}

//=================================
//デストラクタ
//=================================
CPoliceManager::~CPoliceManager()
{
}

//=================================
//初期化
//=================================
HRESULT CPoliceManager::Init(void)
{
	m_waitingPolice = CManager::INT_ZERO;
	m_nCounterSpawn = CManager::INT_ZERO;
	m_nSpawnSpan = CManager::INT_ZERO;
	m_nPoliceMax = CManager::INT_ZERO;
	m_nLv = CManager::INT_ZERO;

	//できた
	return S_OK;
}

//=================================
//終了
//=================================
void CPoliceManager::Uninit(void)
{
	//管理オブジェクト破棄
	Release();
}

//=================================
//更新
//=================================
void CPoliceManager::Update(void)
{
	int nNumAll = CKoban::GetNumAll();	//交番数取得
	CKoban** ppKoban = CKoban::GetKoban();

	//強化
	CScene::MODE mode = CManager::GetMode();

	if (mode == CScene::MODE_GAME)
	{//ゲームはスコアにより変動
		long long nScore = CGame::GetScoreObj()->GetScore();
		while (HAVE_VALUE(m_nLv) <= nScore) m_nLv++;	//上げられるだけ上げる
	}
	else if (mode == CScene::MODE_TUTORIAL)
	{//チュートリアルは固定1レベ
		m_nLv = 1;
	}

	//出動
	if (CPictoPolice::GetNumAll() + m_waitingPolice < m_nPoliceMax + m_nLv)
	{//人手不足
		m_nCounterSpawn++;	//沸きカウンタ増やす
		if (m_nCounterSpawn >= m_nSpawnSpan)
		{//雇う
			m_waitingPolice++;
		}
	}

	//建物のCT減らす
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{
		m_disPatchCT[cnt]--;
	}

	if (m_waitingPolice > 0)
	{//待機中の警察が1人以上いる
		if (CPictoPolice::GetNumAll() < m_nPatrollNum)
		{//パトロール用に沸かす
			int nSpawnKoban = rand() % nNumAll;	//適当に決める
			int nAssignBuilding;

			for (int cnt = 0; cnt < KOBAN_BUILDING_SEARCH_NUM; cnt++)
			{
				nAssignBuilding = rand() % CBuilding::GetNumAll();
				if (CBuilding::GetBuilding(nAssignBuilding)->GetEndurance() > 0)
				{//耐久値が残っている
					CPictoPolice* pPolice = CPictoPolice::Create(ppKoban[nSpawnKoban]->GetPos(), m_nLv);	//適当に決めた交番から沸かす
					pPolice->SetTargetObj(CBuilding::GetBuilding(nAssignBuilding));					//適当に決めた建物に配属
					m_nCounterSpawn = 0;	//カウンタリセット
					m_waitingPolice--;		//待機中警察を減らす
					break;
				}
			}

			//いや見つからないんですが->沸き時間の何割かたったらまた探す
			m_nCounterSpawn = (int)((float)m_nSpawnSpan * KOBAN_NOBUILDING_TIME_PERCE);	//カウンタリセット
		}

		//建物ダメージ報告リストに応じて追加出動
		for (int cntList = 0; cntList < m_AttackList.size(); cntList++)
		{
			if (m_disPatchCT[m_AttackList[cntList]] <= 0)
			{//CT終了している
				CBuilding* pBuilding = CBuilding::GetBuilding(m_AttackList[cntList]);

				//被害を受けている建物に近い交番を探す
				int nNearKoban = -1;
				float fLengthNear = -1.0f;
				for (int cntKoban = 0; cntKoban < nNumAll; cntKoban++)
				{
					float fLength = D3DXVec3Length(&(ppKoban[cntKoban]->GetPos() - CBuilding::GetBuilding(m_AttackList[cntList])->GetPos()));
					if (nNearKoban == -1 || fLengthNear > fLength)
					{
						nNearKoban = cntKoban;		//一番近い交番番号とする
						fLengthNear = fLength;	//近い値を入れる
					}
				}

				//攻撃している最寄りのピクトを取得
				int nNearPicto = -1;
				float fLenNearPicto = -1.0f;
				for (int cnt = 0; cnt < MAX_OBJ; cnt++)
				{
					CPictoDestroyer* pPicto = CPictoDestroyer::GetPicto(cnt);
					if (pPicto != nullptr)
					{
						float fLenPicto = D3DXVec3Length(&(pPicto->GetPos() - pBuilding->GetPos()));
						if (nNearPicto == -1 || fLenNearPicto > fLenPicto)
						{
							nNearPicto = cnt;			//一番近いピクトとする
							fLenNearPicto = fLenPicto;	//距離入れる
						}
					}
				}

				CPictoPolice* pPolice = CPictoPolice::Create(ppKoban[nNearKoban]->GetPos(), m_nLv);	//近い交番から沸かす
				pPolice->SetTargetObj(CPictoDestroyer::GetPicto(nNearPicto));					//建物の近くで攻撃しているピクトをターゲットにする
				m_waitingPolice--;																//待機中警察を減らす
				m_disPatchCT[m_AttackList[cntList]] = m_nSpawnSpan;								//雇う間隔と同じ間隔でCTを設定
			}
		}
	}

	//報告リストをリセット
	m_AttackList.clear();
}

//=================================
//描画
//=================================
void CPoliceManager::Draw(void)
{
	
}

//=================================
//生成処理
//=================================
CPoliceManager* CPoliceManager::Create(void)
{
	CPoliceManager* pObjPoliceManager = NULL;

	if (pObjPoliceManager == NULL)
	{
		//管理オブジェクト生成
		pObjPoliceManager = new CPoliceManager;

		//管理オブジェクト初期化
		pObjPoliceManager->Init();

		return pObjPoliceManager;
	}
	else
	{
		return NULL;
	}
}

//=================================
//交番設定
//=================================
void CPoliceManager::SetKobanParam(const int nSpawnSpan, const int nPoliceMax, const int nPatrollNum)
{//代入
	m_nSpawnSpan = nSpawnSpan;
	m_nPoliceMax = nPoliceMax;
	m_nPatrollNum = nPatrollNum;
}
