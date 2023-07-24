//======================================================
//
//�^�C�g���V�[��[title.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "precompile.h"
#include "texture.h"
#include "title.h"
#include "input.h"
#include "camera.h"
#include "button.h"

//�ÓI�����o�ϐ�
CButton3D* CTitle::m_pButton = NULL;

//=================================
//�R���X�g���N�^
//=================================
CTitle::CTitle()
{
	m_cursorPos = VEC3_ZERO;
}

//=================================
//�f�X�g���N�^
//=================================
CTitle::~CTitle()
{
}

//=================================
//������
//=================================
HRESULT CTitle::Init(void)
{
	m_pButton = CButton3D::Create(VEC3_ZERO, D3DXVECTOR3(-0.5f * D3DX_PI,0.0f,0.0f), 311.0f, 111.0f);
	m_pButton->BindTexture(CTexture::PRELOAD_TITLE_START);

	return S_OK;
}

//=================================
//�I��
//=================================
void CTitle::Uninit(void)
{
	CObject::ReleaseAll();
}

//=================================
//�X�V
//=================================
void CTitle::Update(void)
{
	CCamera* pCamera = CManager::GetCamera();
	CInputMouse* pMouse = CManager::GetInputMouse();
	D3DXVECTOR3 rot = VEC3_ZERO;

	if (pMouse->GetPress(MOUSE_CLICK_LEFT) == true)
	{//��]
		D3DXVECTOR3 move = VEC3_ZERO;
		move.x = pMouse->GetMove().x;
		move.y = pMouse->GetMove().y;

		rot.y -= move.x * CAMERA_MOU_ROT_SPEED;
		rot.x -= move.y * CAMERA_MOU_ROT_SPEED;

		pCamera->SetCameraRot(rot);

		//�J�[�\�������̈ʒu�ɖ߂�
		POINT setCursorpos;
		setCursorpos.x = m_cursorPos.x;
		setCursorpos.y = m_cursorPos.y;
		ClientToScreen(FindWindowA(CLASS_NAME, nullptr), &setCursorpos);
		SetCursorPos(setCursorpos.x, setCursorpos.y);
	}
	else
	{//�}�E�X�J�[�\���ʒu�ۑ�
		m_cursorPos = pMouse->GetPos();
	}

	if (m_pButton->IsClickTrigger() == true)
	{//�{�^���������ꂽ
		CManager::SetMode(CScene::MODE_GAME);
	}
}

//=================================
//�`��
//=================================
void CTitle::Draw(void)
{
}
