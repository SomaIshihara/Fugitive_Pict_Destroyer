//======================================================
//
//カウントダウン処理[countdown.cpp]
//Author:石原颯馬
//
//======================================================
#include "countdown.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "object2D.h"
#include "sound.h"

//=================================
//コンストラクタ
//=================================
CCountDown::CCountDown(const int startCount) : m_startCount(startCount)
{
	m_pos = CManager::VEC3_ZERO;
	m_pFrame = nullptr;
	m_ppCount = nullptr;
	m_count = startCount + 1;
	m_counterSec = CManager::INT_ZERO;
	m_shouldCount = false;
}

//=================================
//デストラクタ
//=================================
CCountDown::~CCountDown()
{
}

//=================================
//初期化
//=================================
HRESULT CCountDown::Init(void)
{
	return S_OK;
}

//=================================
//終了
//=================================
void CCountDown::Uninit(void)
{
	//枠破棄
	if (m_pFrame != nullptr)
	{
		m_pFrame->Uninit();
		m_pFrame = nullptr;
	}

	//カウント
	for (int cnt = 0; cnt < m_startCount + 1; cnt++)
	{
		if (m_ppCount[cnt] != nullptr)
		{
			m_ppCount[cnt]->Uninit();
			m_ppCount[cnt] = nullptr;
		}
	}

	delete[] m_ppCount;
}

//=================================
//更新
//=================================
void CCountDown::Update(void)
{
	if (m_shouldCount == true)
	{//スタートしている
		m_counterSec++;
		if (m_counterSec >= MAX_FPS)
		{//1秒分のフレーム数回した
			m_counterSec = 0;
			m_count--;
		}
	}

	//表示設定
	//いったん元に戻す
	for (int cnt = 0; cnt < m_startCount + 1; cnt++)
	{
		m_ppCount[cnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	//暗くする
	for (int cnt = 0; cnt < m_count; cnt++)
	{
		m_ppCount[cnt]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	}
}

//=================================
//描画
//=================================
void CCountDown::Draw(void)
{
	//手動描画
	m_pFrame->Draw();
	for (int cnt = 0; cnt < m_startCount + 1; cnt++)
	{
		m_ppCount[cnt]->Draw();
	}
}

//=================================
//生成処理
//=================================
CCountDown* CCountDown::Create(const D3DXVECTOR3 pos, const float fOneWidth, const float fHeight, const int nCount)
{
	CCountDown* pObjCountDown = nullptr;

	if (pObjCountDown == nullptr)
	{
		//背景の生成
		pObjCountDown = new CCountDown(nCount);

		//初期化
		pObjCountDown->Init();
		pObjCountDown->SetPos(pos);

		//枠
		pObjCountDown->m_pFrame = CObject2D::Create(pos, CManager::VEC3_ZERO, fOneWidth * (nCount + 1), fHeight,PRIORITY_UI);
		pObjCountDown->m_pFrame->Init();
		pObjCountDown->m_pFrame->BindTexture(CTexture::PRELOAD_COUNTDOWN_01);

		//カウント
		//パターン幅高さ取得
		CTexture* pTexture = CManager::GetTexture();
		int nPatWidth = pTexture->GetPatWidth(CTexture::PRELOAD_COUNTDOWN_02);
		int nPatHeight = pTexture->GetPatHeight(CTexture::PRELOAD_COUNTDOWN_02);

		//配列生成
		pObjCountDown->m_ppCount = new CObject2D*[nCount + 1];

		//オブジェクト生成
		for (int cnt = 0; cnt < nCount + 1; cnt++)
		{
			pObjCountDown->m_ppCount[cnt] = CObject2D::Create(pos + D3DXVECTOR3(((float)cnt - (0.5f * nCount)) * fOneWidth, 0.0f, 0.0f),
				CManager::VEC3_ZERO, fOneWidth, fHeight, PRIORITY_UI);
			pObjCountDown->m_ppCount[cnt]->Init();
			pObjCountDown->m_ppCount[cnt]->BindTexture(CTexture::PRELOAD_COUNTDOWN_02);

			//テクスチャ設定
			D3DXVECTOR2 tex0, tex3;
			tex0 = D3DXVECTOR2((float)(cnt % nPatWidth) / nPatWidth,
				(float)(cnt / nPatWidth) / nPatHeight);
			tex3 = D3DXVECTOR2((float)(cnt % nPatWidth + 1) / nPatWidth,
				(float)(cnt / nPatWidth + 1) / nPatHeight);

			//設定
			pObjCountDown->m_ppCount[cnt]->SetTex(tex0, tex3);
		}

		return pObjCountDown;
	}
	else
	{
		return nullptr;
	}
}

//=================================
//カウントダウン開始
//=================================
void CCountDown::Start(void)
{
	m_shouldCount = true;
	m_count--;
}
