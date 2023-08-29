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

//静的メンバ変数


//=================================
//コンストラクタ
//=================================
CGame::CGame()
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
CGame::~CGame()
{
}

//=================================
//初期化
//=================================
HRESULT CGame::Init(void)
{
	//使うやつ生成
	m_pBG = CBG::Create();
	m_pPolygon = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 90.0f, 0.0f), VEC3_ZERO, 320.0f, 180.0f, PRIORITY_UI);
	m_pContinue = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 270.0f, 0.0f), VEC3_ZERO, 320.0f, 180.0f);
	m_pRestart = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 450.0f, 0.0f), VEC3_ZERO, 320.0f, 180.0f);
	m_pExit = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 630.0f, 0.0f), VEC3_ZERO, 320.0f, 180.0f);
	m_pScreenShot = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH - 80.0f, 80.0f, 0.0f), VEC3_ZERO, 160.0f, 160.0f);

	//テクスチャ貼り貼り（なおテクスチャはあらかじめpreloadで読み込んだものとする）
	CTexture* pTexture = CManager::GetTexture();
	

	return S_OK;
}

//=================================
//終了
//=================================
void CGame::Uninit(void)
{
	//使ったやつだけ破棄
	m_pBG->Uninit();
	m_pBG = nullptr;
	m_pPolygon->Uninit();
	m_pPolygon = nullptr;
	m_pContinue->Uninit();
	m_pContinue = nullptr;
	m_pRestart->Uninit();
	m_pRestart = nullptr;
	m_pExit->Uninit();
	m_pExit = nullptr;
	m_pScreenShot->Uninit();
	m_pScreenShot = nullptr;
}

//=================================
//更新
//=================================
void CGame::Update(void)
{
	//ボタン類はCObjectを親にしているから勝手に更新される

	//スクショモードか確認
	if (m_bScreenShotMode == false)
	{//普通に描画
		//ボタンに応じた処理
		if (m_pContinue->IsClickTrigger() == true)
		{//ゲームに戻る

		}
		else if (m_pRestart->IsClickTrigger() == true)
		{//リスタート

		}
		else if (m_pExit->IsClickTrigger() == true)
		{//終了

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
		}
	}
	else
	{//スクショモード
		CInputMouse* pMouse = CManager::GetInputMouse();	//マウス取得

		if (pMouse->GetTrigger(MOUSE_CLICK_LEFT) == true)
		{//左クリックがされた
			m_bScreenShotMode = false;	//スクショモード終了

			//いったん表示されてるやつ無効化
			m_pBG->SetEnable(true);
			m_pPolygon->SetEnable(true);
			m_pContinue->SetEnable(true);
			m_pRestart->SetEnable(true);
			m_pExit->SetEnable(true);
			m_pScreenShot->SetEnable(true);
		}
	}
}

//=================================
//描画
//=================================
void CGame::Draw(void)
{
	//CObjectを親にしているから勝手に描画される
}
