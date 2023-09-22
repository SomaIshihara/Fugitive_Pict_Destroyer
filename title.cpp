//======================================================
//
//タイトルシーン[title.cpp]
//Author:石原颯馬
//
//======================================================
#include "precompile.h"
#include "texture.h"
#include "title.h"
#include "fade.h"
#include "input.h"
#include "camera.h"
#include "button.h"
#include "sound.h"

//マクロ
#define TITLE_CAMERA_ROT	(CAMERA_MOU_ROT_SPEED * 0.15f)

//静的メンバ変数
CButton3D* CTitle::m_pButtonStart = nullptr;
CButton3D* CTitle::m_pButtonRank = nullptr;

//=================================
//コンストラクタ
//=================================
CTitle::CTitle()
{
	m_pFade = nullptr;
}

//=================================
//デストラクタ
//=================================
CTitle::~CTitle()
{
}

//=================================
//初期化
//=================================
HRESULT CTitle::Init(void)
{
	//カメラ位置リセット
	CManager::GetCamera()->ResetPos();
	CManager::GetCamera()->SetLength(700.0f);

	CObject3D::Create(D3DXVECTOR3(-300.0f, 80.0f, 0.0f), D3DXVECTOR3(-0.5f * D3DX_PI, 0.0f, 0.0f), 365.0f, 171.0f)->BindTexture(CTexture::PRELOAD_TITLELOGO);

	m_pButtonStart = CButton3D::Create(D3DXVECTOR3(300.0f, 80.0f,0.0f), D3DXVECTOR3(-0.5f * D3DX_PI,0.0f,0.0f), 311.0f, 111.0f);
	m_pButtonStart->BindTexture(CTexture::PRELOAD_TITLE_START);

	m_pButtonRank = CButton3D::Create(D3DXVECTOR3(300.0f, -31.0f, 0.0f), D3DXVECTOR3(-0.5f * D3DX_PI, 0.0f, 0.0f), 311.0f, 111.0f);
	m_pButtonRank->BindTexture(CTexture::PRELOAD_TITLE_RANK);

	return S_OK;
}

//=================================
//終了
//=================================
void CTitle::Uninit(void)
{
	CObject::ReleaseAll();
	m_pButtonStart = nullptr;
}

//=================================
//更新
//=================================
void CTitle::Update(void)
{
	CSound* pSound = CManager::GetSound();
	CCamera* pCamera = CManager::GetCamera();
	CInputMouse* pMouse = CManager::GetInputMouse();
	D3DXVECTOR3 rot = CManager::VEC3_ZERO;

	D3DXVECTOR3 move = CManager::VEC3_ZERO;
	move.x = pMouse->GetMove().x;
	move.y = pMouse->GetMove().y;

	rot.y += move.x * TITLE_CAMERA_ROT;

	pCamera->SetRot(rot);

	if (m_pFade == nullptr)
	{
		if (m_pButtonStart->IsClickTrigger() == true)
		{//スタートボタンが押された
			m_pFade = CFade::Create(CScene::MODE_TUTORIAL);
			pSound->Play(CSound::SOUND_LABEL_SE_BUTTON);
		}
		else if (m_pButtonRank->IsClickTrigger() == true)
		{//ランキングボタンが押された
			m_pFade = CFade::Create(CScene::MODE_RANKING);
			pSound->Play(CSound::SOUND_LABEL_SE_BUTTON);
		}
	}
}

//=================================
//描画
//=================================
void CTitle::Draw(void)
{
}
