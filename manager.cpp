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
#include "multiplebg.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "effect.h"
#include "particle.h"
#include "score.h"
#include "timer.h"

//�}�N��
#define FPS_SPEED	(500)	//FPS�v������

//�ÓI�����o�ϐ�
CRenderer* CManager::m_pRenderer = NULL;
CInputKeyboard* CManager::m_pInputKeyboard = NULL;
CDebugProc* CManager::m_pDebProc = NULL;
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

	//�I�u�W�F�N�g����+������
	//CBG::Create();
	CMultipleBG::Create(0.0075f,0.01f,0.02f);
	CPlayer::Create(D3DXVECTOR3(640.0f, 420.0f, 0.0f), VEC3_ZERO,200.0f, 400.0f, 8, 1, 2);
	CEnemy::Create(D3DXVECTOR3(500.0f, 300.0f, 0.0f), VEC3_ZERO, 84.0f, 60.0f, 2, 1, 60,1);
	CEnemy::Create(D3DXVECTOR3(300.0f, 300.0f, 0.0f), VEC3_ZERO, 84.0f, 60.0f, 2, 1, 60,1);
	CEnemy::Create(D3DXVECTOR3(700.0f, 300.0f, 0.0f), VEC3_ZERO, 84.0f, 60.0f, 2, 1, 60,1);
	CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - 24.0f, 32.0f, 0.0f), VEC3_ZERO, 48.0f, 64.0f);
	CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 24.0f, 32.0f, 0.0f), VEC3_ZERO, 48.0f, 64.0f);

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
	m_pDebProc->Print("[������@]\n");
	m_pDebProc->Print("Space:�e����\n");
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