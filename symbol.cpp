//======================================================
//
//記号処理[symbol.cpp]
//Author:石原颯馬
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "object2D.h"
#include "symbol.h"
#include "input.h"
#include <assert.h>

//=================================
//コンストラクタ（デフォルト）
//=================================
CSymbol::CSymbol(int nPriority) :CObject2D(nPriority)
{
}

//=================================
//コンストラクタ（オーバーロード 位置向きand幅高さ）
//=================================
CSymbol::CSymbol(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, int nPriority)
	: CObject2D(pos,rot, fWidth, fHeight, nPriority)
{
}

//=================================
//デストラクタ
//=================================
CSymbol::~CSymbol()
{
}

//=================================
//初期化
//=================================
HRESULT CSymbol::Init(void)
{
	//親クラス処理
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}
	//パターン幅高さ取得
	CTexture* pTexture = CManager::GetTexture();
	int nPatWidth = pTexture->GetPatWidth(m_nIdxTexture);
	int nPatHeight = pTexture->GetPatHeight(m_nIdxTexture);

	//テクスチャ設定
	D3DXVECTOR2 tex0, tex3;
	tex0 = D3DXVECTOR2((float)(CManager::INT_ZERO % nPatWidth) / nPatWidth,
		(float)(CManager::INT_ZERO / nPatWidth) / nPatHeight);
	tex3 = D3DXVECTOR2((float)(CManager::INT_ZERO % nPatWidth + 1) / nPatWidth,
		(float)(CManager::INT_ZERO / nPatWidth + 1) / nPatHeight);

	if (FAILED(SetTex(tex0, tex3)))
	{
		return E_FAIL;
	}

	return S_OK;
}
//=================================
//終了
//=================================
void CSymbol::Uninit(void)
{
	//親クラス処理
	CObject2D::Uninit();
}

//=================================
//更新
//=================================
void CSymbol::Update(void)
{
	//親クラス処理
	CObject2D::Update();
}

//=================================
//描画
//=================================
void CSymbol::Draw(void)
{
	//親クラス処理
	CObject2D::Draw();
}

//=================================
//生成処理
//=================================
CSymbol* CSymbol::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight)
{
	CSymbol* pSymbol = nullptr;

	if (pSymbol == nullptr)
	{
		//記号オブジェの生成
		pSymbol = new CSymbol(pos, rot, fWidth, fHeight);

		//初期化
		pSymbol->Init();

		return pSymbol;
	}
	else
	{
		return nullptr;
	}
}

//=================================
//数字分割処理（nNumは0以上で入力すること）
//=================================
void CSymbol::SetSymbol(const int nNum)
{
	if (nNum < 0)
	{//不正
		assert(false);
	}

	//パターン幅高さ取得
	CTexture* pTexture = CManager::GetTexture();
	int nPatWidth = pTexture->GetPatWidth(m_nIdxTexture);
	int nPatHeight = pTexture->GetPatHeight(m_nIdxTexture);

	//テクスチャ設定
	D3DXVECTOR2 tex0, tex3;
	tex0 = D3DXVECTOR2((float)(nNum % nPatWidth) / nPatWidth,
		(float)(nNum / nPatWidth) / nPatHeight);
	tex3 = D3DXVECTOR2((float)(nNum % nPatWidth + 1) / nPatWidth,
		(float)(nNum / nPatWidth + 1) / nPatHeight);

	if (FAILED(SetTex(tex0, tex3)))
	{
		assert(false);
	}
}

//=================================
//数字専用テクスチャ設定処理
//=================================
void CSymbol::BindTexture(int nIdx)
{
	//テクスチャ設定
	m_nIdxTexture = nIdx;

	//親にも伝える
	CObject2D::BindTexture(nIdx);
}