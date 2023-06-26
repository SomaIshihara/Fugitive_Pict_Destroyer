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

//****************************************
//2Dブロック
//****************************************
//=================================
//コンストラクタ（デフォルト）
//=================================
CBlock2D::CBlock2D(int nPriority) : CObject2D(nPriority)
{
	//値クリア
	m_nIdxTexture = -1;
}

//=================================
//コンストラクタ（オーバーロード 位置幅高さ）
//=================================
CBlock2D::CBlock2D(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, int nPriority) :CObject2D(pos, VEC3_ZERO, fWidth, fHeight, nPriority)
{
	//値クリア
	m_nIdxTexture = -1;
}

//=================================
//デストラクタ
//=================================
CBlock2D::~CBlock2D()
{
}

//=================================
//初期化
//=================================
HRESULT CBlock2D::Init(void)
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
void CBlock2D::Uninit(void)
{
	CObject2D::Uninit();
}

//=================================
//更新
//=================================
void CBlock2D::Update(void)
{
	CObject2D::Update();
}

//=================================
//描画
//=================================
void CBlock2D::Draw(void)
{
	CObject2D::Draw();
}

//=================================
//生成処理
//=================================
CBlock2D* CBlock2D::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
	CBlock2D* pBlock = NULL;

	if (pBlock == NULL)
	{
		//オブジェクトアニメーション2Dの生成
		pBlock = new CBlock2D(pos, fWidth, fHeight);

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

//****************************************
//3Dブロック
//****************************************
//=================================
//コンストラクタ（デフォルト）
//=================================
CBlock3D::CBlock3D(int nPriority) : CObjectX(nPriority)
{
	//値クリア
	m_nIdxTexture = -1;
}

//=================================
//コンストラクタ（オーバーロード 位置幅高さ）
//=================================
CBlock3D::CBlock3D(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nIdx, int nPriority) : CObjectX(pos, VEC3_ZERO, nIdx, nPriority)
{
	//値クリア
	m_nIdxTexture = -1;
}

//=================================
//デストラクタ
//=================================
CBlock3D::~CBlock3D()
{
}

//=================================
//初期化
//=================================
HRESULT CBlock3D::Init(void)
{
	SetType(TYPE_BLOCK);	//タイプ設定

	return CObjectX::Init();
}

//=================================
//終了
//=================================
void CBlock3D::Uninit(void)
{
	CObjectX::Uninit();
}

//=================================
//更新
//=================================
void CBlock3D::Update(void)
{
	CObjectX::Update();
}

//=================================
//描画
//=================================
void CBlock3D::Draw(void)
{
	CObjectX::Draw();
}

//=================================
//生成処理
//=================================
CBlock3D* CBlock3D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nIdx)
{
	CBlock3D* pBlock = NULL;

	if (pBlock == NULL)
	{
		//オブジェクトアニメーション2Dの生成
		pBlock = new CBlock3D(pos, rot, nIdx);

		//初期化
		pBlock->Init();

		return pBlock;
	}
	else
	{
		return NULL;
	}
}