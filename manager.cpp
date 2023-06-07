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

//�}�N��
#define FPS_SPEED	(500)	//FPS�v������

//�ÓI�����o�ϐ�
CRenderer* CManager::m_pRenderer = NULL;
CInputKeyboard* CManager::m_pInputKeyboard = NULL;
CInputMouse* CManager::m_pInputMouse = NULL;
CDebugProc* CManager::m_pDebProc = NULL;
CSound* CManager::m_pSound = NULL;
int CManager::m_nFPS = 0;
DWORD CManager::m_dwFrameCount = 0;

//�e�N�X�`���p�X
const char* c_apTexturePathMultiBG[MAX_EFFECT] =
{
	"data\\TEXTURE\\bg100.png",
	"data\\TEXTURE\\bg101.png",
	"data\\TEXTURE\\bg102.png"
};

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

	//�e�N�X�`���ǂݍ���
	CPlayer::Load("data\\TEXTURE\\runningman000.png");	//�v���C���[
	CBG::Load("data\\TEXTURE\\wasitu01_.jpg");			//1���w�i
	CBullet::Load("data\\TEXTURE\\EnergyBullet_01.png");//�e
	CEnemy::Load("data\\TEXTURE\\Enemy_01.png");		//�G
	CExplosion::Load("data\\TEXTURE\\bomb0.png");		//����
	for (int cnt = 0; cnt < MAX_EFFECT; cnt++)		//���d�w�i
	{//1�����ǂݍ���
		CMultipleBG::Load(c_apTexturePathMultiBG[cnt], cnt);
	}
	CEffect::Load("data\\TEXTURE\\effect000.jpg");				//�G�t�F�N�g
	CParticle::Load("data\\TEXTURE\\effect000.jpg");			//�p�[�e�B�N��
	CScore::Load("data\\TEXTURE\\Number_Rank_01.png", 10, 1);	//�X�R�A
	CTimer::Load("data\\TEXTURE\\Number_Rank_01.png", 10, 1);	//�^�C�}�[
	CBlock::Load("data\\TEXTURE\\Block_R_01.png");				//�u���b�N
	CItem::Load("data\\TEXTURE\\Item_05.png");					//�A�C�e��

	//�I�u�W�F�N�g����+������
	//CBG::Create();
	CMultipleBG::Create(0.0075f,0.01f,0.02f);
	CPlayer::Create(D3DXVECTOR3(640.0f, 420.0f, 0.0f), VEC3_ZERO,100.0f, 200.0f, 8, 1, 2);
	CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - 24.0f, 32.0f, 0.0f), VEC3_ZERO, 48.0f, 64.0f);
	CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 24.0f, 32.0f, 0.0f), VEC3_ZERO, 48.0f, 64.0f);
	//�j��\�I�u�W�F�N�g
	ResetObj();

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
	//�e�N�X�`���j��
	CItem::Unload();			//�A�C�e��
	CBlock::Unload();			//�u���b�N
	CTimer::Unload();			//�^�C�}�[
	CScore::Unload();			//�X�R�A
	CParticle::Unload();		//�p�[�e�B�N��
	CEffect::Unload();			//�G�t�F�N�g
	CMultipleBG::Unload();		//���d�w�i�i�܂Ƃ߂Ĕj�������j
	CExplosion::Unload();		//����
	CEnemy::Unload();			//�G
	CBullet::Unload();			//�e
	CBG::Unload();				//1���w�i
	CPlayer::Unload();			//�v���C���[

	//�I�u�W�F�N�g�I��+�j��
	CObject2D::ReleaseAll();

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

	//�Ĕz�u�{�^���������ꂽ
	if (m_pInputKeyboard->GetTrigger(DIK_F5) == true)
	{//�Ĕz�u���s
		ResetObj();
	}

	//FPS�v����̏���
	m_dwFrameCount++;

	//�f�o�b�O�\��
	m_pDebProc->Print("FPS:%d\n", m_nFPS);
	m_pDebProc->Print("[������@]\n");
	m_pDebProc->Print("�}�E�X���N���b�N:�e����\n");
	m_pDebProc->Print("Space:�W�����v\n");
	m_pDebProc->Print("F5:�Ĕz�u\n");
	m_pDebProc->Print("[Debug]F1:�X�R�A�ݒ�(12345678)\n");
	m_pDebProc->Print("[Debug]F2:�X�R�A���Z(+100)\n");
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

				if (type == CObject::TYPE_ENEMY || type == CObject::TYPE_BLOCK || type == CObject::TYPE_ITEM)
				{//�j��Ώ�
					pObj->Uninit();	//�f�X�g���C
				}
			}
		}
	}

	//�Ĕz�u
	CEnemy::Create(D3DXVECTOR3(500.0f, 300.0f, 0.0f), VEC3_ZERO, 84.0f, 60.0f, 2, 1, 60, 1);
	CEnemy::Create(D3DXVECTOR3(300.0f, 300.0f, 0.0f), VEC3_ZERO, 84.0f, 60.0f, 2, 1, 60, 1);
	CEnemy::Create(D3DXVECTOR3(700.0f, 300.0f, 0.0f), VEC3_ZERO, 84.0f, 60.0f, 2, 1, 60, 1);
	CBlock::Create(D3DXVECTOR3(964.0f, 450.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(900.0f, 450.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(836.0f, 450.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(1100.0f, 300.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(1100.0f, 236.0f, 0.0f), 64.0f, 64.0f);
	CItem::Create(D3DXVECTOR3(900.0f, 350.0f, 0.0f), 48.0f, 20.0f);
	CItem::Create(D3DXVECTOR3(1100.0f, 150.0f, 0.0f), 48.0f, 20.0f);
}
