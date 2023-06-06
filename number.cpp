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

//=================================
//コンストラクタ（デフォルト）
//=================================
CNumber::CNumber(int nPriority) :CObject2D(nPriority)
{
}

//=================================
//コンストラクタ（オーバーロード 位置向きandパターン幅高さ）
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
	//値初期化

	//テクスチャ設定
	D3DXVECTOR2 tex0, tex3;
	tex0 = D3DXVECTOR2((float)(INT_ZERO % GetPatWidth()) / GetPatWidth(),
		(float)(INT_ZERO / GetPatWidth()) / GetPatHeight());
	tex3 = D3DXVECTOR2((float)(INT_ZERO % GetPatWidth() + 1) / GetPatWidth(),
		(float)(INT_ZERO / GetPatWidth() + 1) / GetPatHeight());

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

		return pObjAnim2D;
	}
	else
	{
		return NULL;
	}
}

//=================================
//数字分割処理（nDigitは0以上で入力すること）
//=================================
void CNumber::SetNumber(const int nSource, const int nDigit)
{
	int nNumber = 0;	//1桁分の数字

	if (nDigit < 0)
	{//不正
		assert(false);
	}

	//桁ごとの数字を入れる
	nNumber = nSource % (int)pow(10, nDigit + 1) / (int)pow(10, nDigit);

	//テクスチャ設定
	D3DXVECTOR2 tex0, tex3;
	tex0 = D3DXVECTOR2((float)(nNumber % GetPatWidth()) / GetPatWidth(),
		(float)(nNumber / GetPatWidth()) / GetPatHeight());
	tex3 = D3DXVECTOR2((float)(nNumber % GetPatWidth() + 1) / GetPatWidth(),
		(float)(nNumber / GetPatWidth() + 1) / GetPatHeight());

	if (FAILED(SetTex(tex0, tex3)))
	{
		assert(false);
	}
}