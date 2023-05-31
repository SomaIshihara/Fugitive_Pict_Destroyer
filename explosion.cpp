//======================================================
//
//爆発処理[explosion.cpp]
//Author:石原颯馬
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "explosion.h"
#include "input.h"
#include <assert.h>

//静的メンバ変数
LPDIRECT3DTEXTURE9 CExplosion::m_pTexture = NULL;

//=================================
//コンストラクタ（デフォルト）
//=================================
CExplosion::CExplosion()
{

}

//=================================
//コンストラクタ（オーバーロード 位置向きandパターン幅高さ）
//=================================
CExplosion::CExplosion(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,
	const float fWidth, const float fHeight, const int nPatWidth, const int nPatHeight, const int nAnimSpeed)
	: CObjectAnim2D(pos, rot, fWidth, fHeight, nPatWidth, nPatHeight, nAnimSpeed, false)
{
	
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
CExplosion* CExplosion::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight,
	const int nPatWidth, const int nPatHeight, const int nAnimSpeed)
{
	CExplosion* pObjAnim2D = NULL;

	if (pObjAnim2D == NULL)
	{
		//オブジェクトアニメーション2Dの生成
		pObjAnim2D = new CExplosion(pos, rot, fWidth, fHeight, nPatWidth, nPatHeight, nAnimSpeed);

		//初期化
		pObjAnim2D->Init();

		//テクスチャ割り当て
		pObjAnim2D->BindTexture(m_pTexture);

		return pObjAnim2D;
	}
	else
	{
		return NULL;
	}
}

//=================================
//テクスチャ読み込み処理
//=================================
HRESULT CExplosion::Load(const char* pPath)
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
void CExplosion::Unload(void)
{
	//テクスチャ破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}