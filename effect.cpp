//======================================================
//
//エフェクト処理[effect.cpp]
//Author:石原颯馬
//
//======================================================
#include "effect.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//****************************************
//2Dエフェクト
//****************************************
//=================================
//コンストラクタ（デフォルト）
//=================================
CEffect2D::CEffect2D(int nPriority) : CObject2D(nPriority), m_nDefLife(0)
{
	//値クリア
	m_nIdxTexture = -1;
	m_effect.move = CManager::VEC3_ZERO;
	m_effect.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_effect.nLife = 0;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CEffect2D::CEffect2D(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fWidth, const float fHeight,
	const D3DXCOLOR col, const int nLife, int nPriority) : CObject2D(pos, CManager::VEC3_ZERO, fWidth, fHeight, nPriority), m_nDefLife(nLife)
{
	//値設定
	m_nIdxTexture = -1;
	m_effect.move = move;
	m_effect.col = col;
	m_effect.nLife = nLife;
}

//=================================
//デストラクタ
//=================================
CEffect2D::~CEffect2D()
{
}

//=================================
//初期化
//=================================
HRESULT CEffect2D::Init(void)
{
	//親クラス処理
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}

	//テクスチャ読み込み
	CTexture* pTexture = CManager::GetTexture();
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\effect000.jpg");

	return S_OK;
}

//=================================
//終了
//=================================
void CEffect2D::Uninit(void)
{
	//親クラス処理
	CObject2D::Uninit();
}

//=================================
//更新
//=================================
void CEffect2D::Update(void)
{
	//親クラス処理
	CObject2D::Update();

	//移動
	D3DXVECTOR3 pos = GetPos();
	pos += m_effect.move;
	SetPos(pos);

	//寿命管理
	m_effect.nLife--;	//減らす

	//不透明度設定
	m_effect.col.a = (float)m_effect.nLife / m_nDefLife;
	SetCol(m_effect.col);

	if (m_effect.nLife <= 0)
	{//死んだ
		Uninit();	//終了
	}
}

//=================================
//描画
//=================================
void CEffect2D::Draw(void)
{
	//Zテスト無効化
	CManager::GetRenderer()->SetEnableZTest(false);

	//アルファテスト有効化
	CManager::GetRenderer()->SetEnableAlplaTest(true);

	//ブレンディング種類を加算合成に変更
	CManager::GetRenderer()->SetBlendType(CRenderer::BLENDTYPE_ADD);

	//親クラス処理
	CObject2D::Draw();

	//ブレンディング種類を通常状態に戻す
	CManager::GetRenderer()->SetBlendType(CRenderer::BLENDTYPE_NORMAL);

	//アルファテスト無効化
	CManager::GetRenderer()->SetEnableAlplaTest(false);

	//Zテスト無効化
	CManager::GetRenderer()->SetEnableZTest(true);
}

//=================================
//生成処理
//=================================
CEffect2D* CEffect2D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fWidth, const float fHeight,
	const D3DXCOLOR col, const int nLife)
{
	CEffect2D* pObjEffect = nullptr;

	if (pObjEffect == nullptr)
	{
		//背景の生成
		pObjEffect = new CEffect2D(pos, move, fWidth, fHeight, col, nLife);

		//初期化
		pObjEffect->Init();

		//テクスチャ設定
		pObjEffect->BindTexture(pObjEffect->m_nIdxTexture);

		return pObjEffect;
	}
	else
	{
		return nullptr;
	}
}

//****************************************
//ビルボードエフェクト
//****************************************
//=================================
//コンストラクタ（デフォルト）
//=================================
CEffectBillboard::CEffectBillboard(int nPriority) : CObjectBillboard(nPriority), m_nDefLife(0)
{
	//値クリア
	m_nIdxTexture = -1;
	m_effect.move = CManager::VEC3_ZERO;
	m_effect.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_effect.nLife = 0;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CEffectBillboard::CEffectBillboard(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fWidth, const float fHeight,
	const D3DXCOLOR col, const int nLife, int nPriority) : CObjectBillboard(pos, CManager::VEC3_ZERO, fWidth, fHeight, nPriority), m_nDefLife(nLife)
{
	//値設定
	m_nIdxTexture = -1;
	m_effect.move = move;
	m_effect.col = col;
	m_effect.nLife = nLife;
}

//=================================
//デストラクタ
//=================================
CEffectBillboard::~CEffectBillboard()
{
}

//=================================
//初期化
//=================================
HRESULT CEffectBillboard::Init(void)
{
	//親クラス処理
	if (FAILED(CObjectBillboard::Init()))
	{
		return E_FAIL;
	}

	//テクスチャ読み込み
	CTexture* pTexture = CManager::GetTexture();
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\effect000.jpg");

	return S_OK;
}

//=================================
//終了
//=================================
void CEffectBillboard::Uninit(void)
{
	//親クラス処理
	CObjectBillboard::Uninit();
}

//=================================
//更新
//=================================
void CEffectBillboard::Update(void)
{
	//親クラス処理
	CObjectBillboard::Update();

	//移動
	D3DXVECTOR3 pos = GetPos();
	pos += m_effect.move;
	SetPos(pos);

	//寿命管理
	m_effect.nLife--;	//減らす

	//不透明度設定
	m_effect.col.a = (float)m_effect.nLife / m_nDefLife;
	SetCol(m_effect.col);

	if (m_effect.nLife <= 0)
	{//死んだ
		Uninit();	//終了
	}
}

//=================================
//描画
//=================================
void CEffectBillboard::Draw(void)
{
	//Zテスト無効化
	CManager::GetRenderer()->SetEnableZTest(false);

	//アルファテスト有効化
	CManager::GetRenderer()->SetEnableAlplaTest(true);

	//ブレンディング種類を加算合成に変更
	CManager::GetRenderer()->SetBlendType(CRenderer::BLENDTYPE_ADD);

	//親クラス処理
	CObjectBillboard::Draw();

	//ブレンディング種類を通常状態に戻す
	CManager::GetRenderer()->SetBlendType(CRenderer::BLENDTYPE_NORMAL);

	//アルファテスト無効化
	CManager::GetRenderer()->SetEnableAlplaTest(false);

	//Zテスト無効化
	CManager::GetRenderer()->SetEnableZTest(true);
}

//=================================
//生成処理
//=================================
CEffectBillboard* CEffectBillboard::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fWidth, const float fHeight,
	const D3DXCOLOR col, const int nLife)
{
	CEffectBillboard* pObjEffect = nullptr;

	if (pObjEffect == nullptr)
	{
		//背景の生成
		pObjEffect = new CEffectBillboard(pos, move, fWidth, fHeight, col, nLife);

		//初期化
		pObjEffect->Init();

		//テクスチャ設定
		pObjEffect->BindTexture(pObjEffect->m_nIdxTexture);

		return pObjEffect;
	}
	else
	{
		return nullptr;
	}
}