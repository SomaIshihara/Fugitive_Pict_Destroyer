//======================================================
//
//数字処理[number.cpp]
//Author:石原颯馬
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "number.h"
#include "input.h"
#include <assert.h>

//静的メンバ変数
CObject2D::PatternTexture CNumber::m_patternTexture = { NULL,0,0 };

//=================================
//コンストラクタ（デフォルト）
//=================================
CNumber::CNumber()
{
	//値クリア
	m_patternTexture.pTexture = NULL;
	m_patternTexture.nPatternWidth = 0;
	m_patternTexture.nPatternHeight = 0;
}

//=================================
//コンストラクタ（オーバーロード 位置向きandパターン幅高さ）
//=================================
CNumber::CNumber(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight) : CObject2D(pos,rot, fWidth, fHeight)
{
	//値クリア
	m_patternTexture.pTexture = NULL;
	m_patternTexture.nPatternWidth = 0;
	m_patternTexture.nPatternHeight = 0;
}

//=================================
//デストラクタ
//=================================
CNumber::~CNumber()
{
}

//=================================
//初期化
//=================================
HRESULT CNumber::Init(void)
{
	//親クラス処理
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}
	//値初期化

	//テクスチャ設定
	D3DXVECTOR2 tex0, tex3;
	tex0 = D3DXVECTOR2((float)(INT_ZERO % m_patternTexture.nPatternWidth) / m_patternTexture.nPatternWidth,
		(float)(INT_ZERO / m_patternTexture.nPatternWidth) / m_patternTexture.nPatternHeight);
	tex3 = D3DXVECTOR2((float)(INT_ZERO % m_patternTexture.nPatternWidth + 1) / m_patternTexture.nPatternWidth,
		(float)(INT_ZERO / m_patternTexture.nPatternWidth + 1) / m_patternTexture.nPatternHeight);

	if (FAILED(SetTex(tex0, tex3)))
	{
		return E_FAIL;
	}

	return S_OK;
}
//=================================
//終了
//=================================
void CNumber::Uninit(void)
{
	//親クラス処理
	CObject2D::Uninit();
}

//=================================
//更新
//=================================
void CNumber::Update(void)
{
	//親クラス処理
	CObject2D::Update();
}

//=================================
//描画
//=================================
void CNumber::Draw(void)
{
	//親クラス処理
	CObject2D::Draw();
}

//=================================
//生成処理
//=================================
CNumber* CNumber::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight)
{
	CNumber* pObjAnim2D = NULL;

	if (pObjAnim2D == NULL)
	{
		//オブジェクトアニメーション2Dの生成
		pObjAnim2D = new CNumber(pos, rot, fWidth, fHeight);

		//初期化
		pObjAnim2D->Init();

		//テクスチャ割り当て
		pObjAnim2D->BindTexture(m_patternTexture.pTexture);

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
HRESULT CNumber::Load(const char* pPath, int nPatWidth, int nPatHeight)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイス取得

	//テクスチャ読み込み
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		pPath,
		&m_patternTexture.pTexture)))
	{//失敗
		return E_FAIL;
	}

	//パターン幅高さ代入
	m_patternTexture.nPatternWidth = nPatWidth;
	m_patternTexture.nPatternHeight = nPatHeight;

	//成功
	return S_OK;
}

//=================================
//テクスチャ破棄処理
//=================================
void CNumber::Unload(void)
{
	//テクスチャ破棄
	if (m_patternTexture.pTexture != NULL)
	{
		m_patternTexture.pTexture->Release();
		m_patternTexture.pTexture = NULL;
	}
}

//=================================
//数字分割処理（nDigitは1以上で入力すること）
//=================================
void CNumber::SetNumber(const int nSource, const int nDigit)
{
	int nNumber = 0;	//1桁分の数字

	if (nDigit <= 0)
	{//不正
		assert(false);
	}

	//桁ごとの数字を入れる
	nNumber = nSource % (int)pow(10, nSource + 1) / (int)pow(10, nSource);

	//テクスチャ設定
	D3DXVECTOR2 tex0, tex3;
	tex0 = D3DXVECTOR2((float)(nNumber % m_patternTexture.nPatternWidth) / m_patternTexture.nPatternWidth,
		(float)(nNumber / m_patternTexture.nPatternWidth) / m_patternTexture.nPatternHeight);
	tex3 = D3DXVECTOR2((float)(nNumber % m_patternTexture.nPatternWidth + 1) / m_patternTexture.nPatternWidth,
		(float)(nNumber / m_patternTexture.nPatternWidth + 1) / m_patternTexture.nPatternHeight);

	if (FAILED(SetTex(tex0, tex3)))
	{
		assert(false);
	}
}