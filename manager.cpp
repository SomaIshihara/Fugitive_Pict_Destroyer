//======================================================
//
//�}�l�[�W��[manager.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "debugproc.h"
#include "camera.h"
#include "light.h"
#include "texture.h"
#include "object.h"
#include "objectX.h"

//�V�[��
#include "title.h"
#include "game.h"
#include "result.h"
#include "ranking.h"

//�}�N��
#define FPS_SPEED	(500)	//FPS�v������

//�ÓI�����o�ϐ�
CRenderer* CManager::m_pRenderer = NULL;
CInputKeyboard* CManager::m_pInputKeyboard = NULL;
CInputMouse* CManager::m_pInputMouse = NULL;
CDebugProc* CManager::m_pDebProc = NULL;
CSound* CManager::m_pSound = NULL;
CCamera* CManager::m_pCamera = NULL;
CLight* CManager::m_pLight = NULL;
CTexture* CManager::m_pTexture = NULL;
CScene* CManager::m_pScene = NULL;

int CManager::m_nFPS = 0;
DWORD CManager::m_dwFrameCount = 0;

//************************************************
//�}�l�[�W���N���X
//************************************************
//=================================
//�R���X�g���N�^
//=================================
CManager::CManager()
{
	
}

//=================================
//�f�X�g���N�^
//=================================
CManager::~CManager()
{}

//=================================
//������
//=================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//����
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputMouse = new CInputMouse;
	m_pSound = new CSound;
	m_pRenderer = new CRenderer;
	m_pDebProc = new CDebugProc;
	m_pCamera = new CCamera;
	m_pLight = new CLight;
	m_pTexture = new CTexture;

	//�����_���[������
	if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h������
	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//�}�E�X������
	if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//�T�E���h������
	if (FAILED(m_pSound->Init(hWnd)))
	{
		return E_FAIL;
	}

	//�f�o�b�O������
	m_pDebProc->Init();

	//�J����������
	if (FAILED(m_pCamera->Init()))
	{
		return E_FAIL;
	}

	//���C�g������
	if (FAILED(m_pLight->Init()))
	{
		return E_FAIL;
	}

	//�e�N�X�`��������
	if (FAILED(m_pTexture->Load("data\\preload.txt")))
	{
		return E_FAIL;
	}

	//3D���f���ǂݍ���
	CObjectX::Load("data\\MODEL\\jobi.x");
	CObjectX::Load("data\\MODEL\\zahyoukanban002.x");
	CObjectX::Load("data\\MODEL\\DoshinBill_01.x");
	CObjectX::Load("data\\MODEL\\hako.x");
	CObjectX::Load("data\\MODEL\\Agit.x");

	//FPS�v���평����
	m_nFPS = 0;
	m_dwFrameCount = 0;

	//�T�E���h����
	m_pSound->Play(CSound::SOUND_LABEL_BGM_TITLE);

	//���[�h�ݒ�
	SetMode(CScene::MODE_TITLE);

	//�ł���
	return S_OK;
}

//=================================
//�I��
//=================================
void CManager::Uninit(void)
{
	//���f���j��
	CObjectX::Unload();			//X���f��

	//�I�u�W�F�N�g�I��+�j��
	CObject::ReleaseAll();

	//�e�N�X�`���j��
	if (m_pTexture != NULL)
	{//�e�N�X�`���I��
		m_pTexture->Unload();
		delete m_pTexture;
		m_pTexture = NULL;
	}

	//���C�g�j��
	if (m_pLight != NULL)
	{//���C�g�I��
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

	//�J�����j��
	if (m_pCamera != NULL)
	{//�J�����I��
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	//�f�o�b�O�j��
	if (m_pDebProc != NULL)
	{//�f�o�b�O�I��
		m_pDebProc->Uninit();
		delete m_pDebProc;
		m_pDebProc = NULL;
	}

	//�T�E���h�j��
	if (m_pSound != NULL)
	{//�T�E���h�I��
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}

	//�}�E�X�j��
	if (m_pInputMouse != NULL)
	{//�}�E�X�I��
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	//�L�[�{�[�h�j��
	if (m_pInputKeyboard != NULL)
	{//�L�[�{�[�h�I��
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	//�����_���[�j��
	if (m_pRenderer != NULL)
	{//�����_���[�I��
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
}

//=================================
//�X�V
//=================================
void CManager::Update(void)
{
	m_pInputKeyboard->Update();
	m_pInputMouse->Update();
	m_pRenderer->Update();
	m_pCamera->Update();
	m_pLight->Update();

	m_pScene->Update();

	//���̎��_�Ŏ��S�t���O�������Ă���I�u�W�F���E��
	CObject::Death();

	//FPS�v����̏���
	m_dwFrameCount++;

	//�f�o�b�O�\��
	m_pDebProc->Print("FPS:%d\n", m_nFPS);
	m_pDebProc->Print("[������@]\n");
	m_pDebProc->Print("���N���b�N+�ړ�:���_�ړ�\n");
	m_pDebProc->Print("WASD:�v���C���[�i�J�����j�ړ�\n");
	m_pDebProc->Print("Space:�e����\n");
	m_pDebProc->Print("[���f���E�s�N�g�Ɍ�������]�}�E�X���N���b�N:�I��\n");
	m_pDebProc->Print("[Debug]F3:�^�C�}�[�ݒ�(120�b�J�E���g�_�E��)\n");
}

//=================================
//�`��
//=================================
void CManager::Draw(void)
{
	m_pRenderer->Draw();
}

//=================================
//FPS�v��
//=================================
void CManager::CheckFPS(DWORD dwCurrentTime, DWORD dwExecLastTime)
{
	m_nFPS = (m_dwFrameCount * 1000) / (dwCurrentTime - dwExecLastTime);
	m_dwFrameCount = 0;
}

//=================================
//���[�h�ݒ�
//=================================
void CManager::SetMode(CScene::MODE mode)
{
	//���~�߂�
	m_pSound->Stop();

	//���݂̃��[�h�j��
	if (m_pScene != NULL)
	{//�Ȃ񂩓����Ă�
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = NULL;
	}

	if (m_pScene == NULL)
	{//�ʂ�ʂ�
		//�V���[�h����
		m_pScene = CScene::Create(mode);
	}
}

#if 0
//=================================
//�Ĕz�u
//=================================
void CManager::ResetObj(void)
{
	//�Ԃ���
	for (int cntPriority = 0; cntPriority < PRIORITY_MAX; cntPriority++)
	{
		for (int cntObj = 0; cntObj < MAX_OBJ; cntObj++)
		{//�S�I�u�W�F�N�g����
			CObject* pObj = CObject::GetObject(cntPriority, cntObj);	//�I�u�W�F�N�g�擾

			if (pObj != NULL)	//�k���`�F
			{//�Ȃ񂩂���
				CObject::TYPE type = pObj->GetType();	//��ގ擾

				if (type == CObject::TYPE_ENEMY || type == CObject::TYPE_ITEM)
				{//�j��Ώ�
					pObj->Uninit();	//�f�X�g���C
				}
			}
		}
	}
}
#endif

//************************************************
//�V�[���i���ہj�N���X
//************************************************
//=================================
//�R���X�g���N�^
//=================================
CScene::CScene()
{
}

//=================================
//�f�X�g���N�^
//=================================
CScene::~CScene()
{
}

//=================================
//����
//=================================
CScene* CScene::Create(MODE mode)
{
	CScene* pScene = NULL;

	if (pScene == NULL)
	{
		//�V�[���̐���
		switch (mode)
		{
		case MODE_TITLE:
			pScene = new CTitle;
			break;
		case MODE_GAME:
			pScene = new CGame;
			break;
		case MODE_RESULT:
			pScene = new CResult;
			break;
		case MODE_RANKING:
			pScene = new CRanking;
			break;
		default:	//��Ȃ���͂Ȃ�
			return NULL;
			break;
		}

		//������
		pScene->Init();

		//���[�h����Ƃ�
		pScene->m_mode = mode;

		return pScene;
	}
	else
	{
		return NULL;
	}
}
