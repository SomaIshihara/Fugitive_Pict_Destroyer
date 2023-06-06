//======================================================
//
//アイテム処理[item.cpp]
//Author:石原颯馬
//
//======================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "item.h"

//静的メンバ変数
LPDIRECT3DTEXTURE9 CItem::m_pTexture = NULL;

//=================================
//コンストラクタ（デフォルト）
//=================================
CItem::CItem(int nPriority) : CObject2D(nPriority)
{
	SetType(TYPE_ITEM);	//タイプ設定
}

//=================================
//コンストラクタ（オーバーロード 位置幅高さ）
//=================================
CItem::CItem(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, int nPriority) :CObject2D(pos, VEC3_ZERO, fWidth, fHeight, nPriority)
{
	SetType(TYPE_ITEM);	//タイプ設定
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
		pItem->BindTexture(m_pTexture);

		return pItem;
	}
	else
	{
		return NULL;
	}
}

//=================================
//テクスチャ読み込み処理
//=================================
HRESULT CItem::Load(const char* pPath)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイス取得

	//テクスチャ読み込み
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		pPath,
		&m_pTexture)))
	{//失敗
		return E_FAIL;
	}

	//成功
	return S_OK;
}

//=================================
//テクスチャ破棄処理
//=================================
void CItem::Unload(void)
{
	//テクスチャ破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}