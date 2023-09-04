//======================================================
//
//リザルトシーン[result.cpp]
//Author:石原颯馬
//
//======================================================
#include "precompile.h"
#include "manager.h"
#include "result.h"
#include "number.h"
#include "button.h"

//静的メンバ変数
const int CResult::HEADLINE_TIME = 60;
const int CResult::ATK_TIME = HEADLINE_TIME + 40;
const int CResult::DEST_TIME = ATK_TIME + 40;
const int CResult::VALUE_TIME = DEST_TIME + 60;
const int CResult::RANK_TIME = VALUE_TIME + 60;

//=================================
//コンストラクタ
//=================================
CResult::CResult()
{
	m_pButton = nullptr;
	m_nCounter = INT_ZERO;
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
	m_nCounter = 0;
	return S_OK;
}

//=================================
//終了
//=================================
void CResult::Uninit(void)
{
	//こっちで管理しているものだけ破棄
	m_pButton->Uninit();
	m_pButton = nullptr;
}

//=================================
//更新
//=================================
void CResult::Update(void)
{
	m_nCounter++;

	if (m_nCounter >= HEADLINE_TIME)
	{//見出し生成

	}
	else if (m_nCounter >= ATK_TIME)
	{//攻撃数表示

	}
	else if (m_nCounter >= DEST_TIME)
	{//全壊数表示

	}
	else if (m_nCounter >= VALUE_TIME)
	{//被害総額表示

	}
	else if (m_nCounter >= RANK_TIME)
	{//ランキング遷移ボタン表示
		m_pButton = CButton2D::Create(D3DXVECTOR3(1000.0f, 600.0f, 0.0f), VEC3_ZERO, 311.0f, 111.0f);
	}

	if (m_pButton != nullptr && m_pButton->IsClickTrigger() == true)
	{//ランキング遷移
		CManager::SetMode(CScene::MODE_RANKING);
	}
}

//=================================
//描画
//=================================
void CResult::Draw(void)
{
	//勝手にやってくれる
}
