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
//コンストラクタ
//=================================
CEffect::CEffect() : CObject2D(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT)
{
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
}

//=================================
//描画
//=================================
void CEffect::Draw(void)
{
	//親クラス処理
	CObject2D::Draw();
}

//=================================
//生成処理
//=================================
CEffect* CEffect::Create(void)
{
	CEffect* pObjBG = NULL;

	if (pObjBG == NULL)
	{
		//背景の生成
		pObjBG = new CEffect();

		//初期化
		pObjBG->Init();

		//テクスチャ設定
		pObjBG->BindTexture(m_pTexture);

		return pObjBG;
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