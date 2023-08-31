//======================================================
//
//�Q�[���V�[��[game.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "game.h"
#include "pause.h"
#include "player.h"
#include "meshField.h"
#include "slider.h"
#include "building.h"
#include "score.h"
#include "timer.h"
#include "objectX.h"
#include "pict.h"
#include "koban.h"
#include "item.h"
#include "meshsky.h"
#include "point.h"
#include "input.h"
#include "havenum.h"

//�ÓI�����o�ϐ�
CPlayer* CGame::m_pPlayer = nullptr;
CSlider* CGame::m_pSlider = nullptr;
CMeshField* CGame::m_pMeshField = nullptr;
CTimer* CGame::m_pTimer = nullptr;
CScore* CGame::m_pScore = nullptr;
CHaveNum* CGame::m_pHaveNum[];

//=================================
//�R���X�g���N�^
//=================================
CGame::CGame()
{
	m_pPause = nullptr;
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
	m_pSlider = CSlider::Create(D3DXVECTOR3(100.0f, SCREEN_HEIGHT - 20.0f, 0.0f), 40.0f, 40.0f, 3);
	m_pPlayer = new CPlayer;

	//�v���C���[������
	if (FAILED(m_pPlayer->Init()))
	{
		return E_FAIL;
	}

	//���I�u�W�F����
	m_pMeshField = CMeshField::Create(D3DXVECTOR3(-1280.0f, 0.0f, 1280.0f), VEC3_ZERO, 64.0f, 64.0f, 40, 40);

	//�����p�����[�^�ǂݍ���
	CBuilding::LoadParam("data\\CSV\\BuildingParam.csv");

	//�I�u�W�F�N�g����+������
	m_pTimer = CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f + 24.0f, 32.0f, 0.0f), VEC3_ZERO, 48.0f, 72.0f);
	m_pTimer->Set(120, CTimer::COUNT_DOWN);
	m_pScore = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - 24.0f, 32.0f, 0.0f), VEC3_ZERO, 40.0f, 64.0f);
	CObjectX* pAgit = CObjectX::Create(D3DXVECTOR3(600.0f,0.0f,0.0f), VEC3_ZERO, CManager::GetAgitModel());
	CPict::SetAgit(pAgit);
	CMeshSky::Create(VEC3_ZERO, VEC3_ZERO, 10000.0f, 8, 8);
	m_pHaveNum[0] = CHaveNum::Create(D3DXVECTOR3(SCREEN_WIDTH - 30.0f, 100.0f, 0.0f), VEC3_ZERO, 60.0f, 72.0f, 2, 0);//�A�C�R���ԍ���
	m_pHaveNum[1] = CHaveNum::Create(D3DXVECTOR3(SCREEN_WIDTH - 30.0f, 172.0f, 0.0f), VEC3_ZERO, 60.0f, 72.0f, 2, 0);//�A�C�R���ԍ���
	m_pHaveNum[2] = CHaveNum::Create(D3DXVECTOR3(SCREEN_WIDTH - 30.0f, 244.0f, 0.0f), VEC3_ZERO, 60.0f, 72.0f, 5,0);//�A�C�R���ԍ���

	//��
	CItemBullet::Create(D3DXVECTOR3(0.0f, 0.0f, 10.0f), VEC3_ZERO);

	//�}�b�v�f�[�^�ǂݍ��݂Ɣz�u
	CObjectX::LoadData("data\\Fugitive_Pict_MapData_v111.ismd");

	//�|�C���g����
	CPoint::Update();

	//��ԃp�����[�^�ݒ�
	CKoban::SetKobanParam(300, 10);	//���ݒ�

	return S_OK;
}

//=================================
//�I��
//=================================
void CGame::Uninit(void)
{
	CObject::ReleaseAll();

	//�v���C���[�j��
	if (m_pPlayer != NULL)
	{//�v���C���[�I��
		m_pPlayer->Uninit();
		delete m_pPlayer;
		m_pPlayer = NULL;
	}

	m_pPlayer = nullptr;
	m_pSlider = nullptr;
	m_pMeshField = nullptr;
	m_pTimer = nullptr;
	m_pScore = nullptr;
}

//=================================
//�X�V
//=================================
void CGame::Update(void)
{
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();	//�L�[�{�[�h�擾

	if (pKeyboard->GetTrigger(DIK_P) == true)
	{//�|�[�Y�؂�ւ�
		CManager::SetPause((CManager::GetPause() == true ? false : true));
	}

	if (CManager::GetPause() == true)
	{//�|�[�Y���Ă�
		if (m_pPause == nullptr)
		{//�|�[�Y���ʂ��
			m_pPause = new CPause;		//�|�[�Y����
			m_pPause->Init();			//�|�[�Y������
		}

		//�|�[�Y���̏���
		m_pPause->Update();
	}
	else
	{//�|�[�Y���ĂȂ�
		if (m_pPause != nullptr)
		{//�Ȃ񂩓����Ă�
			m_pPause->Uninit();	//�I��
			delete m_pPause;	//�j��
			m_pPause = nullptr;	//�ʂ�ۓ����
		}

		//���i�̏���
		CKoban::CommonUpdate();	//��ԋ��ʍX�V����
		m_pPlayer->Update();

		//���ԊǗ�
		if (m_pTimer->GetTime() <= 0)
		{//���Ԑ؂�
			CManager::SetMode(CScene::MODE_RESULT);
		}
	}
}

//=================================
//�`��
//=================================
void CGame::Draw(void)
{
	//���i�͂��ׂ�CObject�N���X�p�����Ă���̂������Ă���̂Ŏ����`��
	//�|�[�Y�̒��g�����ׂ�CObject�N���X�p�����Ă�̂Ŏ����`��
	//����Ă����ł��邱�Ƃ͂Ȃ�
}
