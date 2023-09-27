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
#include "objectX.h"
#include "meshsky.h"
#include "meshField.h"

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

	CObject3D::Create(D3DXVECTOR3(-300.0f, 180.0f, 0.0f), D3DXVECTOR3(-0.5f * D3DX_PI, 0.0f, 0.0f), 365.0f, 171.0f, PRIORITY_UI)->BindTexture(CTexture::PRELOAD_TITLELOGO);

	m_pButtonStart = CButton3D::Create(D3DXVECTOR3(300.0f, 180.0f, -50.0f), D3DXVECTOR3(-0.5f * D3DX_PI,0.0f,0.0f), 311.0f, 111.0f);
	m_pButtonStart->BindTexture(CTexture::PRELOAD_TITLE_START);

	m_pButtonRank = CButton3D::Create(D3DXVECTOR3(300.0f, 69.0f, -50.0f), D3DXVECTOR3(-0.5f * D3DX_PI, 0.0f, 0.0f), 311.0f, 111.0f);
	m_pButtonRank->BindTexture(CTexture::PRELOAD_TITLE_RANK);

	//マップデータ読み込みと配置
	CObjectX::LoadData("data\\Fugitive_Picto_MapData_v120.ismd");
	CMeshField::Create(D3DXVECTOR3(-1280.0f, 0.0f, 1280.0f), CManager::VEC3_ZERO, 64.0f, 64.0f, 40, 40);
	CMeshSky::Create(CManager::VEC3_ZERO, CManager::VEC3_ZERO, 10000.0f, 8, 8);

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
