//======================================================
//
//ブロック処理[block.cpp]
//Author:石原颯馬
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "block.h"

//=================================
//コンストラクタ（デフォルト）
//=================================
CBlock::CBlock(int nPriority) : CObject2D(nPriority)
{
	//値クリア
	m_nIdxTexture = -1;
}

//=================================
//コンストラクタ（オーバーロード 位置幅高さ）
//=================================
CBlock::CBlock(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, int nPriority) :CObject2D(pos, VEC3_ZERO, fWidth, fHeight, nPriority)
{
	//値クリア
	m_nIdxTexture = -1;
}

//=================================
//デストラクタ
//=================================
CBlock::~CBlock()
{
}

//=================================
//初期化
//=================================
HRESULT CBlock::Init(void)
{
	//テクスチャ読み込み
	CTexture* pTexture = CManager::GetTexture();
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\Block_R_01.png");

	SetType(TYPE_BLOCK);	//タイプ設定

	return CObject2D::Init();
}

//=================================
//終了
//=================================
void CBlock::Uninit(void)
{
	CObject2D::Uninit();
}

//=================================
//更新
//=================================
void CBlock::Update(void)
{
	CObject2D::Update();
}

//=================================
//描画
//=================================
void CBlock::Draw(void)
{
	CObject2D::Draw();
}

//=================================
//生成処理
//=================================
CBlock* CBlock::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
	CBlock* pBlock = NULL;

	if (pBlock == NULL)
	{
		//オブジェクトアニメーション2Dの生成
		pBlock = new CBlock(pos, fWidth, fHeight);

		//初期化
		pBlock->Init();

		//テクスチャ設定
		pBlock->BindTexture(pBlock->m_nIdxTexture);

		return pBlock;
	}
	else
	{
		return NULL;
	}
}