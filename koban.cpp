//======================================================
//
//交番プログラム[koban.cpp]
//Author:石原颯馬
//
//======================================================
#include "koban.h"
#include "picto.h"
#include "building.h"

//マクロ
#define KOBAN_BUILDING_SEARCH_NUM	(5)		//建物探索回数（これを過ぎると沸かない）
#define KOBAN_NOBUILDING_TIME_PERCE	(0.5f)	//建物が見つからなかった場合の探索時間割合

//静的メンバ変数
CKoban* CKoban::m_apKoban[];
int CKoban::m_disPatchCT[] = {};
std::vector<int> CKoban::m_AttackList;
int CKoban::m_nNumAll = CManager::INT_ZERO;
int CKoban::m_nCounterSpawn = CManager::INT_ZERO;
int CKoban::m_nSpawnSpan = CManager::INT_ZERO;
int CKoban::m_nPoliceMax = CManager::INT_ZERO;
int CKoban::m_waitingPolice = CManager::INT_ZERO;
int CKoban::m_nPatrollNum = CManager::INT_ZERO;

//=================================
//コンストラクタ（デフォルト）
//=================================
CKoban::CKoban()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apKoban[cnt] == nullptr)
		{//空っぽ
			m_apKoban[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;
			m_nNumAll++;	//総数増やす
			break;
		}
	}
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CKoban::CKoban(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel) : CObjectX(pos, rot, pModel)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apKoban[cnt] == nullptr)
		{//空っぽ
			m_apKoban[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;
			m_nNumAll++;	//総数増やす
			break;
		}
	}
}

//=================================
//デストラクタ
//=================================
CKoban::~CKoban()
{
}

//=================================
//初期化
//=================================
HRESULT CKoban::Init(void)
{
	CObjectX::Init();
	return S_OK;
}

//=================================
//終了
//=================================
void CKoban::Uninit(void)
{
	m_apKoban[m_nID] = nullptr;
	m_nNumAll--;

	//なくなったら静的メンバ変数初期化
	if (m_nNumAll <= 0)
	{
		m_nCounterSpawn = CManager::INT_ZERO;
		m_nSpawnSpan = CManager::INT_ZERO;
		m_nPoliceMax = CManager::INT_ZERO;
	}

	CObjectX::Uninit();
}

//=================================
//更新
//=================================
void CKoban::Update(void)
{
	CObjectX::Update();
}

//=================================
//描画
//=================================
void CKoban::Draw(void)
{
	CObjectX::Draw();
}

//=================================
//交番共通更新処理
//=================================
void CKoban::CommonUpdate(void)
{
	if (CPictoPolice::GetNumAll() + m_waitingPolice < m_nPoliceMax)
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
			int nSpawnKoban = rand() % m_nNumAll;	//適当に決める
			int nAssignBuilding;

			for (int cnt = 0; cnt < KOBAN_BUILDING_SEARCH_NUM; cnt++)
			{
				nAssignBuilding = rand() % CBuilding::GetNumAll();
				if (CBuilding::GetBuilding(nAssignBuilding)->GetEndurance() > 0)
				{//耐久値が残っている
					CPictoPolice* pPolice = CPictoPolice::Create(m_apKoban[nSpawnKoban]->GetPos());	//適当に決めた交番から沸かす
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
		for (int cnt = 0; cnt < m_AttackList.size(); cnt++)
		{
			if (m_disPatchCT[m_AttackList[cnt]] <= 0)
			{//CT終了している
				CBuilding* pBuilding = CBuilding::GetBuilding(m_AttackList[cnt]);

				//被害を受けている建物に近い交番を探す
				int nNearKoban = -1;
				float fLengthNear = -1.0f;
				for (int cnt = 0; cnt < m_nNumAll; cnt++) 
				{
					float fLength = D3DXVec3Length(&(m_apKoban[cnt]->GetPos() - CBuilding::GetBuilding(m_AttackList[cnt])->GetPos()));
					if (nNearKoban == -1 || fLengthNear > fLength)
					{
						nNearKoban = cnt;		//一番近い交番番号とする
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
				
				CPictoPolice* pPolice = CPictoPolice::Create(m_apKoban[nNearKoban]->GetPos());	//近い交番から沸かす
				pPolice->SetTargetObj(pBuilding);												//攻撃を受けている建物に出動
				pPolice->SetTargetPicto(CPictoDestroyer::GetPicto(nNearPicto));					//建物の近くで攻撃しているピクトをターゲットにする
				m_waitingPolice--;																//待機中警察を減らす
				m_disPatchCT[m_AttackList[cnt]] = m_nSpawnSpan;									//雇う間隔と同じ間隔でCTを設定
			}
		}
	}

	//報告リストをリセット
	m_AttackList.clear();
}

//=================================
//生成
//=================================
CKoban* CKoban::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel)
{
	CKoban* pKoban = nullptr;

	if (pKoban == nullptr)
	{
		//交番の生成
		pKoban = new CKoban(pos, rot, pModel);

		//初期化
		pKoban->Init();

		return pKoban;
	}
	else
	{
		return nullptr;
	}
}

//=================================
//交番設定
//=================================
void CKoban::SetKobanParam(const int nSpawnSpan, const int nPoliceMax, const int nPatrollNum)
{//代入
	m_nSpawnSpan = nSpawnSpan;
	m_nPoliceMax = nPoliceMax;
	m_nPatrollNum = nPatrollNum;
}
