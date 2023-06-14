//======================================================
//
//爆発処理[explosion.cpp]
//Author:石原颯馬
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "explosion.h"
#include "input.h"
#include <assert.h>

//=================================
//コンストラクタ（デフォルト）
//=================================
CExplosion::CExplosion()
{
	m_nIdxTexture = -1;
}

//=================================
//コンストラクタ（オーバーロード 位置向きandパターン幅高さ）
//=================================
CExplosion::CExplosion(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const int nAnimSpeed) 
	: CObjectAnim2D(pos, rot, fWidth, fHeight, nAnimSpeed, false)
{
	m_nIdxTexture = -1;
}

//=================================
//デストラクタ
//=================================
CExplosion::~CExplosion()
{
}

//=================================
//初期化
//=================================
HRESULT CExplosion::Init(void)
{
	//テクスチャ読み込み
	CTexture* pTexture = CManager::GetTexture();
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\bomb0.png", 8, 2);

	//親クラス処理
	if (FAILED(CObjectAnim2D::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}
//=================================
//終了
//=================================
void CExplosion::Uninit(void)
{
	//親クラス処理
	CObjectAnim2D::Uninit();
}

//=================================
//更新
//=================================
void CExplosion::Update(void)
{
	//親クラス処理
	CObjectAnim2D::Update();
}

//=================================
//描画
//=================================
void CExplosion::Draw(void)
{
	//親クラス処理
	CObjectAnim2D::Draw();
}

//=================================
//生成処理
//=================================
CExplosion* CExplosion::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const int nAnimSpeed)
{
	CExplosion* pExplosion = NULL;

	if (pExplosion == NULL)
	{
		//オブジェクトアニメーション2Dの生成
		pExplosion = new CExplosion(pos, rot, fWidth, fHeight, nAnimSpeed);

		//初期化
		pExplosion->Init();

		//テクスチャ割り当て
		pExplosion->BindTexture(pExplosion->m_nIdxTexture);

		return pExplosion;
	}
	else
	{
		return NULL;
	}
}