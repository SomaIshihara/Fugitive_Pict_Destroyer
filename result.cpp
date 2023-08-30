//======================================================
//
//タイトルシーン[title.cpp]
//Author:石原颯馬
//
//======================================================
#include "precompile.h"
#include "manager.h"
#include "texture.h"
#include "input.h"
#include "result.h"
#include "object.h"
#include "bg.h"

//=================================
//コンストラクタ
//=================================
CResult::CResult()
{
}

//=================================
//デストラクタ
//=================================
CResult::~CResult()
{
}

//=================================
//初期化
//=================================
HRESULT CResult::Init(void)
{
	CBG::Create()->BindTexture(9);
	
	return S_OK;
}

//=================================
//終了
//=================================
void CResult::Uninit(void)
{
	CObject::ReleaseAll();
}

//=================================
//更新
//=================================
void CResult::Update(void)
{
	CInputMouse* pMouse = CManager::GetInputMouse();

	if (pMouse->GetTrigger(MOUSE_CLICK_LEFT) == true)
	{//仮
		CManager::SetMode(CScene::MODE_TITLE);
	}
}

//=================================
//描画
//=================================
void CResult::Draw(void)
{
	//勝手にやってくれる
}
