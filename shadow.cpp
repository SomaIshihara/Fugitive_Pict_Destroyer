//======================================================
//
//影の処理[shadow.cpp]
//Author:石原颯馬
//
//======================================================
#include "shadow.h"
#include "renderer.h"
#include "texture.h"

//=================================
//コンストラクタ
//=================================
CShadow::CShadow() : CObject3D(CManager::VEC3_ZERO, CManager::VEC3_ZERO, 20.0f, 20.0f)
{
}

//=================================
//デストラクタ
//=================================
CShadow::~CShadow()
{
}

//========================
//初期化処理
//========================
HRESULT CShadow::Init(void)
{
	//親処理
	CObject3D::Init();

	//仮：テクスチャ読み込み
	CTexture* pTexture = CManager::GetTexture();
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\effect000.jpg");

	//できた
	return S_OK;
}

//========================
//終了処理
//========================
void CShadow::Uninit(void)
{
	//親処理
	CObject3D::Uninit();
}

//========================
//更新処理
//========================
void CShadow::Update(void)
{
	//親処理
	CObject3D::Update();
}

//========================
//描画処理
//========================
void CShadow::Draw(void)
{
	//ブレンディング種類を加算合成に変更
	CManager::GetRenderer()->SetBlendType(CRenderer::BLENDTYPE_SUB);

	//親処理
	CObject3D::Draw();

	//ブレンディング種類を通常状態に戻す
	CManager::GetRenderer()->SetBlendType(CRenderer::BLENDTYPE_NORMAL);
}

//========================
//生成処理
//========================
CShadow* CShadow::Create(void)
{
	CShadow* pShadow = nullptr;

	if (pShadow == nullptr)
	{
		//オブジェクト2Dの生成
		pShadow = new CShadow();

		//初期化
		pShadow->Init();

		//テクスチャ設定
		pShadow->BindTexture(pShadow->m_nIdxTexture);

		return pShadow;
	}
	else
	{
		return nullptr;
	}
}

//========================
//破棄処理
//========================
void CShadow::Release(void)
{
	CObject3D::Release();
}

//========================
//位置向き設定処理
//========================
void CShadow::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	//位置向き設定
	SetPos(pos);
	SetRot(rot);
}
