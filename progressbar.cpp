//======================================================
//
//進捗バー処理[bg.cpp]
//Author:石原颯馬
//
//======================================================
#include "progressbar.h"
#include "manager.h"
#include "object2D.h"

//=================================
//コンストラクタ（デフォルト）
//=================================
CProgressBar::CProgressBar(int nPriority)
{
	
}

//=================================
//デストラクタ
//=================================
CProgressBar::~CProgressBar()
{
}

//=================================
//初期化
//=================================
HRESULT CProgressBar::Init(void)
{
	//できた
	return S_OK;
}

//=================================
//終了
//=================================
void CProgressBar::Uninit(void)
{
	m_pFrame->Uninit();
	m_pBar->Uninit();

	//管理オブジェクト破棄
	Release();
}

//=================================
//更新
//=================================
void CProgressBar::Update(void)
{
	
}

//=================================
//描画
//=================================
void CProgressBar::Draw(void)
{
	
}

//=================================
//生成処理
//=================================
CProgressBar* CProgressBar::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
	CProgressBar* pObjProgressBar = NULL;

	if (pObjProgressBar == NULL)
	{
		//管理オブジェクト生成・初期化
		pObjProgressBar = new CProgressBar;
		pObjProgressBar->Init();
		pObjProgressBar->m_fDefWidth = fWidth * 0.95f;

		//枠生成・初期化
		pObjProgressBar->m_pFrame = CObject2D::Create(pos, CManager::VEC3_ZERO, fWidth, fHeight, PRIORITY_UI);
		pObjProgressBar->m_pFrame->Init();
		pObjProgressBar->m_pFrame->BindTexture(-1);

		//バー生成・初期化
		pObjProgressBar->m_pBar = CObject2D::Create(pos, CManager::VEC3_ZERO, fWidth * 0.95f, fHeight * 0.95f, PRIORITY_UI);
		pObjProgressBar->m_pBar->Init();
		pObjProgressBar->m_pBar->BindTexture(-1);
		pObjProgressBar->m_pBar->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		return pObjProgressBar;
	}
	else
	{
		return NULL;
	}
}

//=================================
//割合設定
//=================================
void CProgressBar::SetPercent(const float fParcent)
{
	m_fParcent = fParcent;	//値変更
	m_pBar->SetSize(m_fDefWidth * fParcent, m_pBar->GetHeight());
}
