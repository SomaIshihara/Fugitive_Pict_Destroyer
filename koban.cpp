//======================================================
//
//交番プログラム[koban.cpp]
//Author:石原颯馬
//
//======================================================
#include "koban.h"
#include "pict.h"
#include "building.h"

//静的メンバ変数
CKoban* CKoban::m_apKoban[];
CXModel* CKoban::m_pModel = nullptr;
int CKoban::m_nNumAll = INT_ZERO;
int CKoban::m_nCounterSpawn = INT_ZERO;
int CKoban::m_nSpawnSpan = INT_ZERO;
int CKoban::m_nPoliceMax = INT_ZERO;

//=================================
//コンストラクタ（デフォルト）
//=================================
CKoban::CKoban()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apKoban[cnt] == NULL)
		{//空っぽ
			m_apKoban[cnt] = this;	//自分自身のポインタを登録
			m_nNumAll++;	//総数増やす
			break;
		}
	}
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CKoban::CKoban(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot) : CObjectX(pos, rot, m_pModel)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apKoban[cnt] == NULL)
		{//空っぽ
			m_apKoban[cnt] = this;	//自分自身のポインタを登録
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
	if (CPictPolice::GetNumAll() < m_nPoliceMax)
	{//人手不足
		m_nCounterSpawn++;	//沸きカウンタ増やす
		if (m_nCounterSpawn >= m_nSpawnSpan)
		{//沸かす
			int nSpawnKoban = rand() % m_nNumAll;	//適当に決める
			int nAssignBuilding = rand() % CBuilding::GetNumAll();

			CPictPolice* pPolice = CPictPolice::Create(m_apKoban[nSpawnKoban]->GetPos());	//適当に決めた交番から沸かす
			pPolice->SetTargetObj(CBuilding::GetBuilding(nAssignBuilding));					//適当に決めた建物に配属

			m_nCounterSpawn = 0;	//カウンタリセット
		}
	}
}

//=================================
//生成
//=================================
CKoban* CKoban::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CKoban* pKoban = NULL;

	if (pKoban == NULL)
	{
		//交番の生成
		pKoban = new CKoban(pos, rot);

		//初期化
		pKoban->Init();

		return pKoban;
	}
	else
	{
		return NULL;
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
