//======================================================
//
//�}�l�[�W��[manager.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "object.h"
#include "object2D.h"
#include "objectAnim2D.h"
#include "player.h"
#include "bg.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"

//�}�N��
#define FPS_SPEED	(500)	//FPS�v������

//�ÓI�����o�ϐ�
CRenderer* CManager::m_pRenderer = NULL;
CInputKeyboard* CManager::m_pInputKeyboard = NULL;
CDebugProc* CManager::m_pDebProc = NULL;
int CManager::m_nFPS = 0;
DWORD CManager::m_dwFrameCount = 0;

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
	m_pRenderer = new CRenderer;
	m_pDebProc = new CDebugProc;

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
	//�f�o�b�O������
	m_pDebProc->Init();

	//�e�N�X�`���ǂݍ���
	CPlayer::Load("data\\TEXTURE\\runningman000.png");
	CBG::Load("data\\TEXTURE\\wasitu01_.jpg");
	CBullet::Load("data\\TEXTURE\\EnergyBullet_01.png");
	CEnemy::Load("data\\TEXTURE\\Enemy_01.png");
	CExplosion::Load("data\\TEXTURE\\bomb0.png");

	//�I�u�W�F�N�g����+������
	CBG::Create();
	CPlayer::Create(D3DXVECTOR3(640.0f, 420.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),200.0f, 400.0f, 8, 1, 2);
	CEnemy::Create(D3DXVECTOR3(500.0f, 300.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 84.0f, 60.0f, 2, 1, 60,1);
	CEnemy::Create(D3DXVECTOR3(300.0f, 300.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 84.0f, 60.0f, 2, 1, 60,1);
	CEnemy::Create(D3DXVECTOR3(700.0f, 300.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 84.0f, 60.0f, 2, 1, 60,1);

	//FPS�v���평����
	m_nFPS = 0;
	m_dwFrameCount = 0;

	//�ł���
	return S_OK;
}

//=================================
//�I��
//=================================
void CManager::Uninit(void)
{
	//�e�N�X�`���j��
	CBullet::Unload();
	CBG::Unload();
	CObjectAnim2D::Unload();
	CPlayer::Unload();

	//�I�u�W�F�N�g�I��+�j��
	CObject2D::ReleaseAll();

	//�f�o�b�O�j��
	if (m_pDebProc != NULL)
	{//�f�o�b�O�I��
		m_pDebProc->Uninit();
		delete m_pDebProc;
		m_pDebProc = NULL;
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
	m_pRenderer->Update();

	//FPS�v����̏���
	m_dwFrameCount++;

	//�f�o�b�O�\��
	m_pDebProc->Print("FPS:%d\n", m_nFPS);
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