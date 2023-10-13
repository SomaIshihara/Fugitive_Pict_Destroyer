//======================================================
//
//数字処理[number.cpp]
//Author:石原颯馬
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "object2D.h"
#include "number.h"
#include "input.h"
#include <assert.h>

//=================================
//コンストラクタ（デフォルト）
//=================================
CNumber::CNumber(int nPriority) :CObject2D(nPriority)
{
}

//=================================
//コンストラクタ（オーバーロード 位置向きand幅高さ）
//=================================
CNumber::CNumber(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, int nPriority) 
	: CObject2D(pos,rot, fWidth, fHeight, nPriority)
{
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
	CNumber* pNumber = nullptr;

	if (pNumber == nullptr)
	{
		//オブジェクトアニメーション2Dの生成
		pNumber = new CNumber(pos, rot, fWidth, fHeight);

		//初期化
		pNumber->Init();

		return pNumber;
	}
	else
	{
		return nullptr;
	}
}

//=================================
//数字分割処理（nDigitは0以上で入力すること）
//=================================
void CNumber::SetNumber(const long long nSource, const int nDigit)
{
	int nNumber = 0;	//1桁分の数字

	if (nDigit < 0)
	{//不正
		assert(false);
	}

	//桁ごとの数字を入れる
	nNumber = nSource % (long long)pow(10, nDigit + 1) / (long long)pow(10, nDigit);

	//パターン幅高さ取得
	CTexture* pTexture = CManager::GetTexture();
	int nPatWidth = pTexture->GetPatWidth(m_nIdxTexture);
	int nPatHeight = pTexture->GetPatHeight(m_nIdxTexture);

	//テクスチャ設定
	D3DXVECTOR2 tex0, tex3;
	tex0 = D3DXVECTOR2((float)(nNumber % nPatWidth) / nPatWidth,
		(float)(nNumber / nPatWidth) / nPatHeight);
	tex3 = D3DXVECTOR2((float)(nNumber % nPatWidth + 1) / nPatWidth,
		(float)(nNumber / nPatWidth + 1) / nPatHeight);

	if (FAILED(SetTex(tex0, tex3)))
	{
		assert(false);
	}
}

//=================================
//数字専用テクスチャ設定処理
//=================================
void CNumber::BindTexture(int nIdx)
{
	//テクスチャ設定
	m_nIdxTexture = nIdx;

	//親にも伝える
	CObject2D::BindTexture(nIdx);
}