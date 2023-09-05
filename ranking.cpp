//======================================================
//
//ランキングシーン[ranking.cpp]
//Author:石原颯馬
//
//======================================================
#include "precompile.h"
#include "manager.h"
#include "ranking.h"
#include "input.h"
#include "fade.h"

//=================================
//コンストラクタ
//=================================
CRanking::CRanking()
{
}

//=================================
//デストラクタ
//=================================
CRanking::~CRanking()
{
}

//=================================
//初期化
//=================================
HRESULT CRanking::Init(void)
{
	//見出し生成
	CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 100.0f, 0.0f), CManager::VEC3_ZERO, 311.0f, 111.0f);

	return S_OK;
}

//=================================
//終了
//=================================
void CRanking::Uninit(void)
{
}

//=================================
//更新
//=================================
void CRanking::Update(void)
{
	CInputMouse* pMouse = CManager::GetInputMouse();

	if (pMouse->GetTrigger(MOUSE_CLICK_LEFT) == true)
	{//タイトルに遷移
		CFade::Create(CScene::MODE_TITLE);
	}
}

//=================================
//描画
//=================================
void CRanking::Draw(void)
{
}
