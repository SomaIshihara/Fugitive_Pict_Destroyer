//======================================================
//
//�|�[�Y���[game.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "pause.h"
#include "texture.h"
#include "input.h"
#include "bg.h"
#include "object2D.h"
#include "button.h"

//�ÓI�����o�ϐ�


//=================================
//�R���X�g���N�^
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
//�f�X�g���N�^
//=================================
CGame::~CGame()
{
}

//=================================
//������
//=================================
HRESULT CGame::Init(void)
{
	//�g�������
	m_pBG = CBG::Create();
	m_pPolygon = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 90.0f, 0.0f), VEC3_ZERO, 320.0f, 180.0f, PRIORITY_UI);
	m_pContinue = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 270.0f, 0.0f), VEC3_ZERO, 320.0f, 180.0f);
	m_pRestart = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 450.0f, 0.0f), VEC3_ZERO, 320.0f, 180.0f);
	m_pExit = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 630.0f, 0.0f), VEC3_ZERO, 320.0f, 180.0f);
	m_pScreenShot = CButton2D::Create(D3DXVECTOR3(SCREEN_WIDTH - 80.0f, 80.0f, 0.0f), VEC3_ZERO, 160.0f, 160.0f);

	//�e�N�X�`���\��\��i�Ȃ��e�N�X�`���͂��炩����preload�œǂݍ��񂾂��̂Ƃ���j
	CTexture* pTexture = CManager::GetTexture();
	

	return S_OK;
}

//=================================
//�I��
//=================================
void CGame::Uninit(void)
{
	//�g����������j��
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
//�X�V
//=================================
void CGame::Update(void)
{
	//�{�^���ނ�CObject��e�ɂ��Ă��邩�珟��ɍX�V�����

	//�X�N�V�����[�h���m�F
	if (m_bScreenShotMode == false)
	{//���ʂɕ`��
		//�{�^���ɉ���������
		if (m_pContinue->IsClickTrigger() == true)
		{//�Q�[���ɖ߂�

		}
		else if (m_pRestart->IsClickTrigger() == true)
		{//���X�^�[�g

		}
		else if (m_pExit->IsClickTrigger() == true)
		{//�I��

		}
		else if (m_pScreenShot->IsClickTrigger() == true)
		{//�X�N�V�����[�h
			m_bScreenShotMode = true;

			//��������\������Ă�������
			m_pBG->SetEnable(false);
			m_pPolygon->SetEnable(false);
			m_pContinue->SetEnable(false);
			m_pRestart->SetEnable(false);
			m_pExit->SetEnable(false);
			m_pScreenShot->SetEnable(false);
		}
	}
	else
	{//�X�N�V�����[�h
		CInputMouse* pMouse = CManager::GetInputMouse();	//�}�E�X�擾

		if (pMouse->GetTrigger(MOUSE_CLICK_LEFT) == true)
		{//���N���b�N�����ꂽ
			m_bScreenShotMode = false;	//�X�N�V�����[�h�I��

			//��������\������Ă�������
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
//�`��
//=================================
void CGame::Draw(void)
{
	//CObject��e�ɂ��Ă��邩�珟��ɕ`�悳���
}
