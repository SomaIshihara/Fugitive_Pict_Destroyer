//======================================================
//
//交番プログラム[koban.cpp]
//Author:石原颯馬
//
//======================================================
#include "koban.h"
#include "picto.h"
#include "building.h"

//静的メンバ変数
CKoban* CKoban::m_apKoban[];
int CKoban::m_nNumAll = CManager::INT_ZERO;

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