//======================================================
//
//チュートリアルシーン[tutorial.cpp]
//Author:石原颯馬
//
//======================================================
#include "precompile.h"
#include "manager.h"
#include "texture.h"
#include "input.h"
#include "tutorial.h"
#include "object.h"
#include "bg.h"

//=================================
//コンストラクタ
//=================================
CTutorial::CTutorial()
{
}

//=================================
//デストラクタ
//=================================
CTutorial::~CTutorial()
{
}

//=================================
//初期化
//=================================
HRESULT CTutorial::Init(void)
{
	CBG::Create()->BindTexture(9);
	
	return S_OK;
}

//=================================
//終了
//=================================
void CTutorial::Uninit(void)
{
	CObject::ReleaseAll();
}

//=================================
//更新
//=================================
void CTutorial::Update(void)
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
void CTutorial::Draw(void)
{
	//勝手にやってくれる
}
