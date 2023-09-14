//======================================================
//
//アニメーションありオブジェクト（2D）処理[objectAnim2D.cpp]
//Author:石原颯馬
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "objectAnim2D.h"
#include "input.h"
#include <assert.h>

//=================================
//コンストラクタ（デフォルト）
//=================================
CObjectAnim2D::CObjectAnim2D(int nPriority) : CObject2D(nPriority)
{
	//値クリア
	m_nCounterAnim = CManager::INT_ZERO;
	m_nPatternAnim = CManager::INT_ZERO;
	m_nAnimSpeed = CManager::INT_ZERO;
	m_bLoop = true;
}

//=================================
//コンストラクタ（オーバーロード 位置向きandパターン幅高さ）
//=================================
CObjectAnim2D::CObjectAnim2D(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,
	const float fWidth, const float fHeight, const int nAnimSpeed, const bool bLoop, int nPriority)
	: CObject2D(pos,rot, fWidth, fHeight, nPriority)
{
	//値クリア
	m_nCounterAnim = CManager::INT_ZERO;
	m_nPatternAnim = CManager::INT_ZERO;

	//値設定
	m_nAnimSpeed = nAnimSpeed;
	m_bLoop = bLoop;
}

//=================================
//デストラクタ
//=================================
CObjectAnim2D::~CObjectAnim2D()
{
}

//=================================
//初期化
//=================================
HRESULT CObjectAnim2D::Init(void)
{
	//親クラス処理
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}
	//値初期化
	m_nCounterAnim = CManager::INT_ZERO;
	m_nPatternAnim = CManager::INT_ZERO;

	//パターン幅高さ取得
	CTexture* pTexture = CManager::GetTexture();
	int nPatWidth = pTexture->GetPatWidth(m_nIdxTexture);
	int nPatHeight = pTexture->GetPatHeight(m_nIdxTexture);

	//テクスチャ設定
	D3DXVECTOR2 tex0, tex3;
	tex0 = D3DXVECTOR2((float)(m_nPatternAnim % nPatWidth) / nPatWidth,
		(float)(m_nPatternAnim / nPatWidth) / nPatHeight);
	tex3 = D3DXVECTOR2((float)(m_nPatternAnim % nPatWidth + 1) / nPatWidth,
		(float)(m_nPatternAnim / nPatWidth + 1) / nPatHeight);

	if (FAILED(SetTex(tex0, tex3)))
	{
		return E_FAIL;
	}

	return S_OK;
}
//=================================
//終了
//=================================
void CObjectAnim2D::Uninit(void)
{
	//親クラス処理
	CObject2D::Uninit();
}

//=================================
//更新
//=================================
void CObjectAnim2D::Update(void)
{
	//親クラス処理
	CObject2D::Update();
	
	m_nCounterAnim++;

	if ((m_nCounterAnim % m_nAnimSpeed) == 0)
	{
		m_nCounterAnim = 0;	//カウンタ初期値に戻す

		//パターン幅高さ取得
		CTexture* pTexture = CManager::GetTexture();
		int nPatWidth = pTexture->GetPatWidth(m_nIdxTexture);
		int nPatHeight = pTexture->GetPatHeight(m_nIdxTexture);

		//パターンNo更新
		if (m_bLoop == true)
		{//ループする設定の場合
			m_nPatternAnim = (m_nPatternAnim + 1) % (nPatHeight * nPatWidth);
		}
		else
		{//ループしない設定の場合
			m_nPatternAnim++;
			if (m_nPatternAnim >= (nPatHeight * nPatWidth))
			{//アニメーション終了
				Uninit();
				return;
			}
		}

		//テクスチャ設定
		D3DXVECTOR2 tex0, tex3;
		tex0 = D3DXVECTOR2((float)(m_nPatternAnim % nPatWidth) / nPatWidth,
			(float)(m_nPatternAnim / nPatWidth) / nPatHeight);
		tex3 = D3DXVECTOR2((float)(m_nPatternAnim % nPatWidth + 1) / nPatWidth,
			(float)(m_nPatternAnim / nPatWidth + 1) / nPatHeight);

		if (FAILED(SetTex(tex0, tex3)))
		{
			assert(false);
		}
	}
}

//=================================
//描画
//=================================
void CObjectAnim2D::Draw(void)
{
	//親クラス処理
	CObject2D::Draw();
}

//=================================
//生成処理
//=================================
CObjectAnim2D* CObjectAnim2D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const int nAnimSpeed, const bool bLoop)
{
	CObjectAnim2D* pObjAnim2D = nullptr;

	if (pObjAnim2D == nullptr)
	{
		//オブジェクトアニメーション2Dの生成
		pObjAnim2D = new CObjectAnim2D(pos, rot, fWidth, fHeight, nAnimSpeed, bLoop);

		//初期化
		pObjAnim2D->Init();

		//テクスチャ割り当て
		pObjAnim2D->BindTexture(pObjAnim2D->m_nIdxTexture);

		return pObjAnim2D;
	}
	else
	{
		return nullptr;
	}
}

//=================================
//テクスチャ設定処理
//=================================
void CObjectAnim2D::BindTexture(int nIdx)
{
	//アニメーション側の番号設定
	m_nIdxTexture = nIdx;

	//テクスチャ座標再設定
	CTexture* pTexture = CManager::GetTexture();
	int nPatWidth = pTexture->GetPatWidth(m_nIdxTexture);
	int nPatHeight = pTexture->GetPatHeight(m_nIdxTexture);

	//テクスチャ設定
	D3DXVECTOR2 tex0, tex3;
	tex0 = D3DXVECTOR2((float)(m_nPatternAnim % nPatWidth) / nPatWidth,
		(float)(m_nPatternAnim / nPatWidth) / nPatHeight);
	tex3 = D3DXVECTOR2((float)(m_nPatternAnim % nPatWidth + 1) / nPatWidth,
		(float)(m_nPatternAnim / nPatWidth + 1) / nPatHeight);

	if (FAILED(SetTex(tex0, tex3)))
	{
		assert(false);
	}

	//親にも伝える
	CObject2D::BindTexture(nIdx);
}