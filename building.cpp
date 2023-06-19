//======================================================
//
//建物処理[building.cpp]
//Author:石原颯馬
//
//======================================================
#include "building.h"

//静的メンバ変数
CBuilding* CBuilding::m_apBuilding[MAX_OBJ];
int CBuilding::m_nNumAll = 0;

//=================================
//コンストラクタ（デフォルト）
//=================================
CBuilding::CBuilding()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apBuilding[cnt] == NULL)
		{//空っぽ
			m_apBuilding[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
	m_nEndurance = INT_ZERO;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CBuilding::CBuilding(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nIdx)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apBuilding[cnt] == NULL)
		{//空っぽ
			m_apBuilding[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
	m_nEndurance = INT_ZERO;
}

//=================================
//デストラクタ
//=================================
CBuilding::~CBuilding()
{
}

//========================
//初期化処理
//========================
HRESULT CBuilding::Init(void)
{
	//親処理
	CObjectX::Init();

	return S_OK;
}

//========================
//終了処理
//========================
void CBuilding::Uninit(void)
{
	//建物クラス内での処理
	m_apBuilding[m_nID] = NULL;

	//親処理
	CObjectX::Uninit();
}

//========================
//更新処理
//========================
void CBuilding::Update(void)
{
	//親処理
	CObjectX::Update();
}

//========================
//描画処理
//========================
void CBuilding::Draw(void)
{
	//親処理
	CObjectX::Draw();
}

//========================
//生成処理
//========================
CBuilding* CBuilding::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nIdx)
{
	CBuilding* pBuilding = NULL;

	if (pBuilding == NULL)
	{
		//オブジェクト2Dの生成
		pBuilding = new CBuilding(pos, rot, nIdx);

		//初期化
		pBuilding->Init();

		return pBuilding;
	}
	else
	{
		return NULL;
	}
}

//========================
//ダメージ付与処理
//========================
void CBuilding::AddDamage(int nDamage)
{
	m_nEndurance -= nDamage;
	if (m_nEndurance <= 0)
	{
		Uninit();
	}
}
