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
#include "object2D.h"
#include "objectAnim2D.h"
#include "player.h"
#include "bg.h"
#include "multiplebg.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "block.h"
#include "item.h"
#include "effect.h"
#include "particle.h"
#include "score.h"
#include "timer.h"
#include "object3D.h"
#include "objectX.h"
#include "building.h"
#include "objectBillboard.h"
#include "pict.h"
#include "meshField.h"
#include "button.h"
#include "slider.h"
#include "meshsky.h"

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
CPlayer* CManager::m_pPlayer = NULL;
CSlider* CManager::m_pSlider = NULL;
int CManager::m_nFPS = 0;
DWORD CManager::m_dwFrameCount = 0;

//��
CMeshField* CManager::m_pMeshField = NULL;

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
	m_pPlayer = new CPlayer;

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

	//�v���C���[������
	if (FAILED(m_pPlayer->Init()))
	{
		return E_FAIL;
	}

	//3D���f���ǂݍ���
	CObjectX::Load("data\\MODEL\\jobi.x");
	CObjectX::Load("data\\MODEL\\zahyoukanban002.x");
	CObjectX::Load("data\\MODEL\\DoshinBill_01.x");
	CObjectX::Load("data\\MODEL\\hako.x");
	CObjectX::Load("data\\MODEL\\Agit.x");

	//�����p�����[�^�ǂݍ���
	CBuilding::LoadParam("data\\CSV\\BuildingParam.csv");

	//�I�u�W�F�N�g����+������
	CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - 24.0f, 32.0f, 0.0f), VEC3_ZERO, 40.0f, 64.0f);
	CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f + 24.0f, 32.0f, 0.0f), VEC3_ZERO, 48.0f, 72.0f);
	m_pMeshField = CMeshField::Create(D3DXVECTOR3(-1280.0f, 0.0f, 1280.0f), VEC3_ZERO, 64.0f, 64.0f, 40, 40);
	CBuilding* pTarget = CBuilding::Create(D3DXVECTOR3(64.0f, 0.0f, 580.0f), VEC3_ZERO, 0);
	CBuilding::Create(D3DXVECTOR3(-440.0f,0.0f,-453.0f), VEC3_ZERO, 2);
	CPictPolice::Create(D3DXVECTOR3(-300.0f, 0.0f, 0.0f))->SetBuilding(pTarget);
	CObjectX* pAgit = CObjectX::Create(VEC3_ZERO, VEC3_ZERO, 4);
	CPict::SetAgit(pAgit);
	m_pSlider = CSlider::Create(D3DXVECTOR3(100.0f, SCREEN_HEIGHT - 20.0f, 0.0f), 40.0f, 40.0f, 3);

	CPictNormal::Create(D3DXVECTOR3(500.0f, 0.0f, 200.0f));
	CPictNormal::Create(D3DXVECTOR3(300.0f, 0.0f, 300.0f));
	CPictNormal::Create(D3DXVECTOR3(100.0f, 0.0f, 600.0f));

	CMeshSky::Create(VEC3_ZERO, VEC3_ZERO, 10000.0f, 8, 8);

	//FPS�v���평����
	m_nFPS = 0;
	m_dwFrameCount = 0;

	//�T�E���h����
	m_pSound->Play(CSound::SOUND_LABEL_BGM_TITLE);

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

	//�v���C���[�j��
	if (m_pPlayer != NULL)
	{//�v���C���[�I��
		m_pPlayer->Uninit();
		delete m_pPlayer;
		m_pPlayer = NULL;
	}

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
	m_pPlayer->Update();

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

	m_pDebProc->Print("SLIDER_NUM = %d\n", m_pSlider->GetSelectIdx());
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