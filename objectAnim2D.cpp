//======================================================
//
//アニメーションありオブジェクト（2D）処理[objectAnim2D.cpp]
//Author:石原颯馬
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "objectAnim2D.h"
#include "input.h"
#include <assert.h>

//静的メンバ変数
LPDIRECT3DTEXTURE9 CObjectAnim2D::m_pTexture = NULL;

//=================================
//コンストラクタ（デフォルト）
//=================================
CObjectAnim2D::CObjectAnim2D() : m_nPatternWidth(1) , m_nPatternHeight(1)
{
	//値クリア
	m_nCounterAnim = INT_ZERO;
	m_nPatternAnim = INT_ZERO;
	m_nAnimSpeed = INT_ZERO;
	m_bLoop = true;
}

//=================================
//コンストラクタ（オーバーロード 位置向きandパターン幅高さ）
//=================================
CObjectAnim2D::CObjectAnim2D(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,
	const float fWidth, const float fHeight, const int nPatWidth, const int nPatHeight, const int nAnimSpeed, const bool bLoop)
	: CObject2D(pos,rot, fWidth, fHeight), m_nPatternWidth(nPatWidth), m_nPatternHeight(nPatHeight)
{
	//値クリア
	m_nCounterAnim = INT_ZERO;
	m_nPatternAnim = INT_ZERO;

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
	m_nCounterAnim = INT_ZERO;
	m_nPatternAnim = INT_ZERO;

	//テクスチャ設定
	D3DXVECTOR2 tex0, tex3;
	tex0 = D3DXVECTOR2((float)(m_nPatternAnim % m_nPatternWidth) / m_nPatternWidth,
		(float)(m_nPatternAnim / m_nPatternWidth) / m_nPatternHeight);
	tex3 = D3DXVECTOR2((float)(m_nPatternAnim % m_nPatternWidth + 1) / m_nPatternWidth,
		(float)(m_nPatternAnim / m_nPatternWidth + 1) / m_nPatternHeight);

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

		//パターンNo更新
		if (m_bLoop == true)
		{//ループする設定の場合
			m_nPatternAnim = (m_nPatternAnim + 1) % (m_nPatternHeight * m_nPatternWidth);
		}
		else
		{//ループしない設定の場合
			m_nPatternAnim++;
			if (m_nPatternAnim >= (m_nPatternHeight * m_nPatternWidth))
			{//アニメーション終了
				Uninit();
				return;
			}
		}

		//テクスチャ設定
		D3DXVECTOR2 tex0, tex3;
		tex0 = D3DXVECTOR2((float)(m_nPatternAnim % m_nPatternWidth) / m_nPatternWidth,
			(float)(m_nPatternAnim / m_nPatternWidth) / m_nPatternHeight);
		tex3 = D3DXVECTOR2((float)(m_nPatternAnim % m_nPatternWidth + 1) / m_nPatternWidth,
			(float)(m_nPatternAnim / m_nPatternWidth + 1) / m_nPatternHeight);

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
CObjectAnim2D* CObjectAnim2D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight,
	const int nPatWidth, const int nPatHeight, const int nAnimSpeed, const bool bLoop)
{
	CObjectAnim2D* pObjAnim2D = NULL;

	if (pObjAnim2D == NULL)
	{
		//オブジェクトアニメーション2Dの生成
		pObjAnim2D = new CObjectAnim2D(pos, rot, fWidth, fHeight, nPatWidth, nPatHeight, nAnimSpeed, bLoop);

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
HRESULT CObjectAnim2D::Load(const char* pPath)
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
void CObjectAnim2D::Unload(void)
{
	//テクスチャ破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}