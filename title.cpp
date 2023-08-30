//======================================================
//
//タイトルシーン[title.cpp]
//Author:石原颯馬
//
//======================================================
#include "precompile.h"
#include "texture.h"
#include "title.h"
#include "input.h"
#include "camera.h"
#include "button.h"

//マクロ
#define TITLE_CAMERA_ROT	(CAMERA_MOU_ROT_SPEED * 0.15f)

//静的メンバ変数
CButton3D* CTitle::m_pButton = NULL;

//=================================
//コンストラクタ
//=================================
CTitle::CTitle()
{
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
	m_pButton = CButton3D::Create(D3DXVECTOR3(300.0f,-20.0f,0.0f), D3DXVECTOR3(-0.5f * D3DX_PI,0.0f,0.0f), 311.0f, 111.0f);
	m_pButton->BindTexture(CTexture::PRELOAD_TITLE_START);

	return S_OK;
}

//=================================
//終了
//=================================
void CTitle::Uninit(void)
{
	CObject::ReleaseAll();
	m_pButton = nullptr;
}

//=================================
//更新
//=================================
void CTitle::Update(void)
{
	CCamera* pCamera = CManager::GetCamera();
	CInputMouse* pMouse = CManager::GetInputMouse();
	D3DXVECTOR3 rot = VEC3_ZERO;

	D3DXVECTOR3 move = VEC3_ZERO;
	move.x = pMouse->GetMove().x;
	move.y = pMouse->GetMove().y;

	rot.y += move.x * TITLE_CAMERA_ROT;

	pCamera->SetCameraRot(rot);

	if (m_pButton->IsClickTrigger() == true)
	{//ボタンが押された
		CManager::SetMode(CScene::MODE_GAME);
	}
}

//=================================
//描画
//=================================
void CTitle::Draw(void)
{
}
