//======================================================
//
//背景（2D）処理[bg.cpp]
//Author:石原颯馬
//
//======================================================
#include "bg.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//=================================
//コンストラクタ
//=================================
CBG::CBG(int nPriority) : CObject2D(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, CManager::FLOAT_ZERO), CManager::VEC3_ZERO, SCREEN_WIDTH, SCREEN_HEIGHT, nPriority)
{
	m_nIdxTexture = -1;
}

//=================================
//デストラクタ
//=================================
CBG::~CBG()
{
}

//=================================
//初期化
//=================================
HRESULT CBG::Init(void)
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
void CBG::Uninit(void)
{
	//親クラス処理
	CObject2D::Uninit();
}

//=================================
//更新
//=================================
void CBG::Update(void)
{
	//親クラス処理
	CObject2D::Update();
}

//=================================
//描画
//=================================
void CBG::Draw(void)
{
	//Zバッファを使用しない
	CManager::GetRenderer()->SetZEnable(false);

	//親クラス処理
	CObject2D::Draw();
	
	//Zバッファを使用する
	CManager::GetRenderer()->SetZEnable(true);
}

//=================================
//生成処理
//=================================
CBG* CBG::Create(const int nPriority)
{
	CBG* pObjBG = nullptr;

	if (pObjBG == nullptr)
	{
		//背景の生成
		pObjBG = new CBG(nPriority);

		//初期化
		pObjBG->Init();

		return pObjBG;
	}
	else
	{
		return nullptr;
	}
}