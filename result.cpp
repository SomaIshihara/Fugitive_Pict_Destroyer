//======================================================
//
//リザルトシーン[result.cpp]
//Author:石原颯馬
//
//======================================================
#include "precompile.h"
#include "manager.h"
#include "result.h"
#include "game.h"
#include "fade.h"
#include "texture.h"
#include "bg.h"
#include "number.h"
#include "score.h"
#include "button.h"
#include "sound.h"

//静的メンバ変数
const int CResult::HEADLINE_TIME = 60;
const int CResult::ATK_TIME = HEADLINE_TIME + 40;
const int CResult::DEST_TIME = ATK_TIME + 40;
const int CResult::VALUE_TIME = DEST_TIME + 60;
const int CResult::RANK_TIME = VALUE_TIME + 60;
const float CResult::FADE_ALPHA = 0.4f;
const float CResult::FADE_SPEED = 0.03f;

//=================================
//コンストラクタ
//=================================
CResult::CResult()
{
	m_pBG = nullptr;
	m_fAlpha = CManager::FLOAT_ZERO;
	m_pButton = nullptr;
	m_nCounter = CManager::INT_ZERO;
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
	//背景を生成（その後透明にする）
	m_pBG = CBG::Create(PRIORITY_UI);	//ゲーム画面UIとかぶらないようにする
	m_pBG->BindTexture(-1);
	m_pBG->SetCol(D3DXCOLOR((DWORD)0x00000000));
	
	m_fAlpha = 0.0f;
	m_nCounter = 0;
	return S_OK;
}

//=================================
//終了
//=================================
void CResult::Uninit(void)
{
	
}

//=================================
//更新
//=================================
void CResult::Update(void)
{
	CSound* pSound = CManager::GetSound();
	m_fAlpha += FADE_SPEED;

	if (m_fAlpha >= FADE_ALPHA)
	{//ある程度暗くした
		m_fAlpha = FADE_ALPHA;
		m_nCounter++;

		if (m_nCounter == HEADLINE_TIME)
		{//見出し生成
			CObject2D* pObject = nullptr;
			pObject = CObject2D::Create(D3DXVECTOR3(300.0f, 122.0f, 0.0f), CManager::VEC3_ZERO, 500.0f, 144.0f, PRIORITY_PAUSE);
			pObject->BindTexture(CTexture::PRELOAD_RESULT_HEADLINE);
			pSound->Play(CSound::SOUND_LABEL_SE_POPRESULT);
		}
		else if (m_nCounter == ATK_TIME)
		{//攻撃数表示
			//文字
			CObject2D* pObject = nullptr;
			pObject = CObject2D::Create(D3DXVECTOR3(300.0f, 272.0f, 0.0f), CManager::VEC3_ZERO, 250.0f, 72.0f, PRIORITY_PAUSE);
			pObject->BindTexture(CTexture::PRELOAD_RESULT_01);

			//数字
			int nNum = CGame::GetATKNum();
			//十
			CNumber* pNumber = nullptr;
			pNumber = CNumber::Create(D3DXVECTOR3(465.0f, 272.0f, 0.0f), CManager::VEC3_ZERO, 40.0f, 72.0f);
			pNumber->BindTexture(CTexture::PRELOAD_NUMBER);
			pNumber->SetNumber(nNum, 1);

			//一
			pNumber = CNumber::Create(D3DXVECTOR3(505.0f, 272.0f, 0.0f), CManager::VEC3_ZERO, 40.0f, 72.0f);
			pNumber->BindTexture(CTexture::PRELOAD_NUMBER);
			pNumber->SetNumber(nNum, 0);

			pSound->Play(CSound::SOUND_LABEL_SE_POPRESULT);
		}
		else if (m_nCounter == DEST_TIME)
		{//全壊数表示
			//文字
			CObject2D* pObject = nullptr;
			pObject = CObject2D::Create(D3DXVECTOR3(800.0f, 272.0f, 0.0f), CManager::VEC3_ZERO, 250.0f, 72.0f, PRIORITY_PAUSE);
			pObject->BindTexture(CTexture::PRELOAD_RESULT_02); 

			//数字
			int nNum = CGame::GetDestNum();
			//十
			CNumber* pNumber = nullptr;
			pNumber = CNumber::Create(D3DXVECTOR3(965.0f, 272.0f, 0.0f), CManager::VEC3_ZERO, 40.0f, 72.0f);
			pNumber->BindTexture(CTexture::PRELOAD_NUMBER);
			pNumber->SetNumber(nNum, 1);

			//一
			pNumber = CNumber::Create(D3DXVECTOR3(1005.0f, 272.0f, 0.0f), CManager::VEC3_ZERO, 40.0f, 72.0f);
			pNumber->BindTexture(CTexture::PRELOAD_NUMBER);
			pNumber->SetNumber(nNum, 0);

			pSound->Play(CSound::SOUND_LABEL_SE_POPRESULT);
		}
		else if (m_nCounter == VALUE_TIME)
		{//被害総額表示
			//文字
			CObject2D* pObject = nullptr;
			pObject = CObject2D::Create(D3DXVECTOR3(250.0f, 462.0f, 0.0f), CManager::VEC3_ZERO, 375.0f, 108.0f, PRIORITY_PAUSE);
			pObject->BindTexture(CTexture::PRELOAD_RESULT_03);

			//数字
			long long nScore = CGame::GetScoreObj()->GetScore();
			CScore* pScore = nullptr;
			pScore = CScore::Create(D3DXVECTOR3(1097.5f, 462.0f, 0.0f), CManager::VEC3_ZERO, 60.0f, 108.0f);
			pScore->Set(nScore);

			pSound->Play(CSound::SOUND_LABEL_SE_POPRESULT);
		}
		else if (m_nCounter == RANK_TIME)
		{//ランキング遷移ボタン表示
			m_pButton = CButton2D::Create(D3DXVECTOR3(1100.0f, 650.0f, 0.0f), CManager::VEC3_ZERO, 311.0f, 111.0f);
			m_pButton->BindTexture(CTexture::PRELOAD_TITLE_RANK);
		}

		if (m_pButton != nullptr && m_pFade == nullptr && m_pButton->IsClickTrigger() == true)
		{//ランキング遷移
			m_pFade = CFade::Create(CScene::MODE_RANKING);
			pSound->Play(CSound::SOUND_LABEL_SE_BUTTON);
			return;
		}
	}

	m_pBG->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_fAlpha));
}

//=================================
//描画
//=================================
void CResult::Draw(void)
{
	//勝手にやってくれる
}

//=================================
//生成
//=================================
CResult* CResult::Create(void)
{
	CResult* pResult = nullptr;

	if (pResult == nullptr)
	{
		//オブジェクト2Dの生成
		pResult = new CResult;

		//初期化
		pResult->Init();

		return pResult;
	}
	else
	{
		return nullptr;
	}
}
