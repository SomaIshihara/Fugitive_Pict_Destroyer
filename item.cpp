//======================================================
//
//アイテム処理[item.cpp]
//Author:石原颯馬
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "item.h"

//=================================
//コンストラクタ（デフォルト）
//=================================
CItem::CItem(int nPriority) : CObject2D(nPriority)
{
	//値クリア
	m_nIdxTexture = -1;
}

//=================================
//コンストラクタ（オーバーロード 位置幅高さ）
//=================================
CItem::CItem(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, int nPriority) :CObject2D(pos, VEC3_ZERO, fWidth, fHeight, nPriority)
{
	//値クリア
	m_nIdxTexture = -1;
}

//=================================
//デストラクタ
//=================================
CItem::~CItem()
{
}

//=================================
//初期化
//=================================
HRESULT CItem::Init(void)
{
	//テクスチャ読み込み
	CTexture* pTexture = CManager::GetTexture();
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\Item_05.png");

	SetType(TYPE_ITEM);	//タイプ設定

	return CObject2D::Init();
}

//=================================
//終了
//=================================
void CItem::Uninit(void)
{
	CObject2D::Uninit();
}

//=================================
//更新
//=================================
void CItem::Update(void)
{
	CObject2D::Update();
}

//=================================
//描画
//=================================
void CItem::Draw(void)
{
	CObject2D::Draw();
}

//=================================
//生成処理
//=================================
CItem* CItem::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
	CItem* pItem = NULL;

	if (pItem == NULL)
	{
		//オブジェクトアニメーション2Dの生成
		pItem = new CItem(pos, fWidth, fHeight);

		//初期化
		pItem->Init();

		//テクスチャ設定
		pItem->BindTexture(pItem->m_nIdxTexture);

		return pItem;
	}
	else
	{
		return NULL;
	}
}