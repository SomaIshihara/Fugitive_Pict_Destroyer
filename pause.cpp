//======================================================
//
//ポーズ画面[game.cpp]
//Author:石原颯馬
//
//======================================================
#include "pause.h"
#include "texture.h"
#include "input.h"
#include "bg.h"
#include "object2D.h"
#include "button.h"
#include "fade.h"
#include "sound.h"

//=================================
//コンストラクタ
//=================================
CPause::CPause()
{
	m_pBG = nullptr;
	m_pPolygon = nullptr;
	m_pContinue = nullptr;
	m_pRestart = nullptr;
	m_pExit = nullptr;
	m_pScreenShot = nullptr;
	m_bScreenShotMode = false;
}

//=================================
//デストラクタ
//=================================
CPause::~CPause()
{
}

//=================================
//初期化
//=================================
HRESULT CPause::Init(void)
{
	//使うやつ生成
	m_pBG = CBG::Create(PRIORITY_UI);
	m_pPolygon = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 90.0f, 0.0f), CManager::VEC3_ZERO, 600.0f, 180.0f, PRIORITY_PAUSE);
	m_pContinue = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 270.0f, 0.0f), CManager::VEC3_ZERO, 600.0f, 180.0f, PRIORITY_PAUSE);
	m_pRestart = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 450.0f, 0.0f), CManager::VEC3_ZERO, 600.0f, 180.0f, PRIORITY_PAUSE);
	m_pExit = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 630.0f, 0.0f), CManager::VEC3_ZERO, 600.0f, 180.0f, PRIORITY_PAUSE);
	m_pScreenShot = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH - 80.0f, 80.0f, 0.0f), CManager::VEC3_ZERO, 160.0f, 160.0f, PRIORITY_PAUSE);

	//テクスチャ貼り貼り（なおテクスチャはあらかじめpreloadで読み込んだものとする）
	CTexture* pTexture = CManager::GetTexture();
	m_pBG->BindTexture(-1);
	m_pBG->SetCol(D3DXCOLOR(1.0f,1.0f,1.0f,0.5f));
	m_pPolygon->BindTexture(CTexture::PRELOAD_PAUSE);
	m_pContinue->BindTexture(CTexture::PRELOAD_PAUSEBUTTON_01);
	m_pRestart->BindTexture(CTexture::PRELOAD_PAUSEBUTTON_02);
	m_pExit->BindTexture(CTexture::PRELOAD_PAUSEBUTTON_03);
	m_pScreenShot->BindTexture(CTexture::PRELOAD_PAUSEBUTTON_04);

	return S_OK;
}

//=================================
//終了
//=================================
void CPause::Uninit(void)
{
	if (m_pBG != nullptr)
	{
		m_pBG->Uninit();
		m_pBG = nullptr;
	}
	if (m_pPolygon != nullptr)
	{
		m_pPolygon->Uninit();
		m_pPolygon = nullptr;
	}
	if (m_pContinue != nullptr)
	{
		m_pContinue->Uninit();
		m_pContinue = nullptr;
	}
	if (m_pRestart != nullptr)
	{
		m_pRestart->Uninit();
		m_pRestart = nullptr;
	}
	if (m_pExit != nullptr)
	{
		m_pExit->Uninit();
		m_pExit = nullptr;
	}
	if (m_pScreenShot != nullptr)
	{
		m_pScreenShot->Uninit();
		m_pScreenShot = nullptr;
	}
}

//=================================
//更新
//=================================
void CPause::Update(void)
{
	CSound* pSound = CManager::GetSound();

	//ボタン類はレンダラーの更新停止により手動で更新する
	m_pContinue->Update();
	m_pRestart->Update();
	m_pExit->Update();
	m_pScreenShot->Update();

	//スクショモードか確認
	if (m_bScreenShotMode == false)
	{//普通に描画
		//ボタンに応じた処理
		if (m_pContinue->IsClickTrigger() == true)
		{//ゲームに戻る
			ReleaseSome();
			CManager::SetPause(false);
			pSound->Play(CSound::SOUND_LABEL_SE_BUTTON);
		}
		else if (m_pRestart->IsClickTrigger() == true)
		{//リスタート
			CFade::Create(CScene::MODE_GAME);
			CManager::SetPause(false);
			pSound->Play(CSound::SOUND_LABEL_SE_BUTTON);
		}
		else if (m_pExit->IsClickTrigger() == true)
		{//終了
			CFade::Create(CScene::MODE_TITLE);
			CManager::SetPause(false);
			pSound->Play(CSound::SOUND_LABEL_SE_BUTTON);
		}
		else if (m_pScreenShot->IsClickTrigger() == true)
		{//スクショモード
			m_bScreenShotMode = true;

			//いったん表示されてるやつ無効化
			m_pBG->SetEnable(false);
			m_pPolygon->SetEnable(false);
			m_pContinue->SetEnable(false);
			m_pRestart->SetEnable(false);
			m_pExit->SetEnable(false);
			m_pScreenShot->SetEnable(false);

			pSound->Play(CSound::SOUND_LABEL_SE_BUTTON);
		}
	}
	else
	{//スクショモード
		CInputMouse* pMouse = CManager::GetInputMouse();	//マウス取得

		if (pMouse->GetTrigger(MOUSE_CLICK_LEFT) == true)
		{//左クリックがされた
			m_bScreenShotMode = false;	//スクショモード終了

			//有効化
			m_pBG->SetEnable(true);
			m_pPolygon->SetEnable(true);
			m_pContinue->SetEnable(true);
			m_pRestart->SetEnable(true);
			m_pExit->SetEnable(true);
			m_pScreenShot->SetEnable(true);

			pSound->Play(CSound::SOUND_LABEL_SE_BUTTON);
		}
	}
}

//=================================
//描画
//=================================
void CPause::Draw(void)
{
	//CObjectを親にしているから勝手に描画される
}

//=================================
//一部破棄
//=================================
void CPause::ReleaseSome(void)
{
	//使ったやつだけ破棄
	if (m_pBG != nullptr)
	{
		m_pBG->Uninit();
		m_pBG = nullptr;
	}
	
	if (m_pPolygon != nullptr)
	{
		m_pPolygon->Uninit();
		m_pPolygon = nullptr;
	}
	
	if (m_pContinue != nullptr)
	{
		m_pContinue->Uninit();
		m_pContinue = nullptr;
	}
	
	if (m_pRestart != nullptr)
	{
		m_pRestart->Uninit();
		m_pRestart = nullptr;
	}
	
	if (m_pExit != nullptr)
	{
		m_pExit->Uninit();
		m_pExit = nullptr;
	}
	
	if (m_pScreenShot != nullptr)
	{
		m_pScreenShot->Uninit();
		m_pScreenShot = nullptr;
	}
}
