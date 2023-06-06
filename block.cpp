//======================================================
//
//ブロック処理[block.cpp]
//Author:石原颯馬
//
//======================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "block.h"

//静的メンバ変数
LPDIRECT3DTEXTURE9 CBlock::m_pTexture = NULL;

//=================================
//コンストラクタ（デフォルト）
//=================================
CBlock::CBlock(int nPriority) : CObject2D(nPriority)
{
	SetType(TYPE_BLOCK);	//タイプ設定
}

//=================================
//コンストラクタ（オーバーロード 位置幅高さ）
//=================================
CBlock::CBlock(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, int nPriority) :CObject2D(pos, VEC3_ZERO, fWidth, fHeight, nPriority)
{
	SetType(TYPE_BLOCK);	//タイプ設定
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
		pBlock->BindTexture(m_pTexture);

		return pBlock;
	}
	else
	{
		return NULL;
	}
}

//=================================
//テクスチャ読み込み処理
//=================================
HRESULT CBlock::Load(const char* pPath)
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
void CBlock::Unload(void)
{
	//テクスチャ破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}