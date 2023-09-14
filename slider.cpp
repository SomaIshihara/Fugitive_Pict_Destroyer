//======================================================
//
//スライダー処理[slider.cpp]
//Author:石原颯馬
//
//======================================================
#include "slider.h"
#include "picto.h"
#include "manager.h"
#include "object2D.h"
#include "texture.h"
#include "input.h"

//=================================
//コンストラクタ（デフォルト）
//=================================
CSlider::CSlider()
{
	m_pObjFrame = nullptr;
	m_pObjMove = nullptr;
	m_ppObjIcon = nullptr;
	m_pos = CManager::VEC3_ZERO;
	m_fOneWidth = CManager::FLOAT_ZERO;
	m_fOneHeight = CManager::FLOAT_ZERO;
	m_nIdxNum = CManager::INT_ZERO;
	m_nSelect = CManager::INT_ZERO;
	m_bClick = false;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CSlider::CSlider(const D3DXVECTOR3 pos, const float fOneWidth, const float fOneHeight, const int nIdxNum)
{
	m_pObjFrame = nullptr;
	m_pObjMove = nullptr;
	m_ppObjIcon = nullptr;
	m_pos = pos;
	m_fOneWidth = fOneWidth;
	m_fOneHeight = fOneHeight;
	m_nIdxNum = nIdxNum;
	m_nSelect = CManager::INT_ZERO;
	m_bClick = false;
}

//=================================
//デストラクタ
//=================================
CSlider::~CSlider()
{
}

//========================
//初期化処理
//========================
HRESULT CSlider::Init(void)
{
	//枠
	m_pObjFrame = CObject2D::Create(m_pos, CManager::VEC3_ZERO, m_fOneWidth * m_nIdxNum, m_fOneHeight, PRIORITY_UI);
	m_pObjFrame->BindTexture(-1);
	m_pObjFrame->SetCol(D3DXCOLOR(0.0f, 0.7f, 0.0f, 1.0f));

	//移動部
	m_pObjMove = CObject2D::Create(m_pos, CManager::VEC3_ZERO, m_fOneWidth, m_fOneHeight, PRIORITY_UI);
	m_pObjMove->BindTexture(-1);

	//アイコン
	m_ppObjIcon = new CObject2D*[m_nIdxNum];
	for (int cnt = 0; cnt < m_nIdxNum; cnt++)
	{
		float fPosX = (-((float)m_nIdxNum * 0.5f) + (0.5f + (float)cnt)) * m_fOneWidth;
		m_ppObjIcon[cnt] = CObject2D::Create(m_pos + D3DXVECTOR3(fPosX, 0.0f, 0.0f), CManager::VEC3_ZERO, m_fOneWidth, m_fOneHeight, PRIORITY_UI);
	}
	m_ppObjIcon[0]->BindTexture(CTexture::PRELOAD_SLIDER_01);
	m_ppObjIcon[1]->BindTexture(CTexture::PRELOAD_SLIDER_02);
	m_ppObjIcon[2]->BindTexture(CTexture::PRELOAD_SLIDER_03);

	m_nSelect = CPictoTaxi::MODE_SABO;

	return S_OK;
}

//========================
//終了処理
//========================
void CSlider::Uninit(void)
{
	if (m_ppObjIcon != nullptr)
	{
		delete[] m_ppObjIcon;
		m_ppObjIcon = nullptr;
	}

	//自分自身破棄
	Release();
}

//========================
//更新処理
//========================
void CSlider::Update(void)
{
	CInputMouse* mouse =  CManager::GetInputMouse();	//マウス取得
	D3DXVECTOR3 mousePos = mouse->GetPos();				//マウス位置取得
	D3DXVECTOR3 movePos = m_pObjMove->GetPos();

	//クリック
	if (mouse->GetTrigger(MOUSE_CLICK_LEFT) == true &&
		mousePos.x >= movePos.x - m_fOneWidth * 0.5f && mousePos.x <= movePos.x + m_fOneWidth * 0.5f &&
		mousePos.y >= movePos.y - m_fOneHeight * 0.5f && mousePos.y <= movePos.y + m_fOneHeight * 0.5f)
	{//クリックした
		m_bClick = true;
	}
	if(mouse->GetRelease(MOUSE_CLICK_LEFT) == true)
	{//離した
		m_bClick = false;
	}

	//選択
	if (m_bClick == true)
	{
		for (int cnt = 0; cnt < m_nIdxNum; cnt++)
		{
			D3DXVECTOR3 iconPos = m_ppObjIcon[cnt]->GetPos();
			if (mousePos.x >= iconPos.x - m_fOneWidth * 0.5f && mousePos.x <= iconPos.x + m_fOneWidth * 0.5f &&
				mousePos.y >= iconPos.y - m_fOneHeight * 0.5f && mousePos.y <= iconPos.y + m_fOneHeight * 0.5f)
			{//移動した
				m_nSelect = cnt;
			}
		}
	}
	else
	{
		for (int cnt = 0; cnt < m_nIdxNum; cnt++)
		{
			D3DXVECTOR3 iconPos = m_ppObjIcon[cnt]->GetPos();
			if (mouse->GetTrigger(MOUSE_CLICK_LEFT) == true &&
				mousePos.x >= iconPos.x - m_fOneWidth * 0.5f && mousePos.x <= iconPos.x + m_fOneWidth * 0.5f &&
				mousePos.y >= iconPos.y - m_fOneHeight * 0.5f && mousePos.y <= iconPos.y + m_fOneHeight * 0.5f)
			{//移動した
				m_bClick = true;
				m_nSelect = cnt;
			}
		}
	}

	//移動部移動
	float fPosX = (-((float)m_nIdxNum * 0.5f) + (0.5f + (float)m_nSelect)) * m_fOneWidth;
	movePos = m_pos + D3DXVECTOR3(fPosX, 0.0f, 0.0f);
	m_pObjMove->SetPos(movePos);
}

//========================
//描画処理
//========================
void CSlider::Draw(void)
{
}

//========================
//生成処理
//========================
CSlider* CSlider::Create(const D3DXVECTOR3 pos, const float fOneWidth, const float fOneHeight, const int nIdxNum)
{
	CSlider* pSlider = nullptr;

	if (pSlider == nullptr)
	{
		//ピクトの生成
		pSlider = new CSlider(pos, fOneWidth, fOneHeight, nIdxNum);

		//初期化
		pSlider->Init();

		return pSlider;
	}
	else
	{
		return nullptr;
	}
}
