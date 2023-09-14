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
int CKoban::m_nNumAll = CManager::INT_ZERO;
int CKoban::m_nCounterSpawn = CManager::INT_ZERO;
int CKoban::m_nSpawnSpan = CManager::INT_ZERO;
int CKoban::m_nPoliceMax = CManager::INT_ZERO;

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
	if (CPictoPolice::GetNumAll() < m_nPoliceMax)
	{//人手不足
		m_nCounterSpawn++;	//沸きカウンタ増やす
		if (m_nCounterSpawn >= m_nSpawnSpan)
		{//沸かす
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
					break;
				}
			}

			//いや見つからないんですが->沸き時間の何割かたったらまた探す
			m_nCounterSpawn = (int)((float)m_nSpawnSpan * KOBAN_NOBUILDING_TIME_PERCE);	//カウンタリセット
		}
	}
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
void CKoban::SetKobanParam(const int nSpawnSpan, const int nPoliceMax)
{//代入
	m_nSpawnSpan = nSpawnSpan;
	m_nPoliceMax = nPoliceMax;
}
