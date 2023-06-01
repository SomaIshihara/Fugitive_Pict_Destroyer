//======================================================
//
//エフェクト処理[effect.cpp]
//Author:石原颯馬
//
//======================================================
#include "effect.h"
#include "manager.h"
#include "renderer.h"

//静的メンバ変数
LPDIRECT3DTEXTURE9 CEffect::m_pTexture = NULL;

//=================================
//コンストラクタ（デフォルト）
//=================================
CEffect::CEffect() : m_nDefLife(0)
{
	//値クリア
	m_effect.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_effect.nLife = 0;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CEffect::CEffect(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, 
	const D3DXCOLOR col, const int nLife) : CObject2D(pos, rot, fWidth, fHeight), m_nDefLife(nLife)
{
	//値設定
	m_effect.col = col;
	m_effect.nLife = nLife;
}

//=================================
//デストラクタ
//=================================
CEffect::~CEffect()
{
}

//=================================
//初期化
//=================================
HRESULT CEffect::Init(void)
{
	//親クラス処理
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=================================
//終了
//=================================
void CEffect::Uninit(void)
{
	//親クラス処理
	CObject2D::Uninit();
}

//=================================
//更新
//=================================
void CEffect::Update(void)
{
	//親クラス処理
	CObject2D::Update();

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
void CEffect::Draw(void)
{
	//ブレンディング種類を加算合成に変更
	CManager::GetRenderer()->SetBlendType(CRenderer::BLENDTYPE_ADD);

	//親クラス処理
	CObject2D::Draw();

	//ブレンディング種類を通常状態に戻す
	CManager::GetRenderer()->SetBlendType(CRenderer::BLENDTYPE_NORMAL);
}

//=================================
//生成処理
//=================================
CEffect* CEffect::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight,
	const D3DXCOLOR col, const int nLife)
{
	CEffect* pObjEffect = NULL;

	if (pObjEffect == NULL)
	{
		//背景の生成
		pObjEffect = new CEffect(pos, rot, fWidth, fHeight, col, nLife);

		//初期化
		pObjEffect->Init();

		//テクスチャ設定
		pObjEffect->BindTexture(m_pTexture);

		return pObjEffect;
	}
	else
	{
		return NULL;
	}
}

//=================================
//テクスチャ読み込み処理
//=================================
HRESULT CEffect::Load(const char* pPath)
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
void CEffect::Unload(void)
{
	//テクスチャ破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}