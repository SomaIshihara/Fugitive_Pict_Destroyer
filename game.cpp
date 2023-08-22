//======================================================
//
//�Q�[���V�[��[game.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "game.h"
#include "player.h"
#include "meshField.h"
#include "slider.h"
#include "building.h"
#include "score.h"
#include "timer.h"
#include "objectX.h"
#include "pict.h"
#include "item.h"
#include "meshsky.h"
#include "point.h"
#include "input.h"

//�ÓI�����o�ϐ�
CPlayer* CGame::m_pPlayer = NULL;
CSlider* CGame::m_pSlider = NULL;
CMeshField* CGame::m_pMeshField = NULL;

//=================================
//�R���X�g���N�^
//=================================
CGame::CGame()
{
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
	CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - 24.0f, 32.0f, 0.0f), VEC3_ZERO, 40.0f, 64.0f);
	CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f + 24.0f, 32.0f, 0.0f), VEC3_ZERO, 48.0f, 72.0f);
	CBuilding* pTarget = CBuilding::Create(D3DXVECTOR3(64.0f, 0.0f, 580.0f), VEC3_ZERO, 0);
	CBuilding::Create(D3DXVECTOR3(-440.0f, 0.0f, -453.0f), VEC3_ZERO, 2);
	CObjectX* pAgit = CObjectX::Create(VEC3_ZERO, VEC3_ZERO, 4);
	CPict::SetAgit(pAgit);
	CMeshSky::Create(VEC3_ZERO, VEC3_ZERO, 10000.0f, 8, 8);

	//��
	CPictPolice::Create(D3DXVECTOR3(-200.0f, 0.0f, 50.0f))->SetTargetObj(CBuilding::GetBuilding(0));
	CPictNormal::Create(D3DXVECTOR3(48.0f, 0.0f, 556.0f));
	CItemBullet::Create(D3DXVECTOR3(0.0f, 0.0f, 10.0f), VEC3_ZERO);

	//�|�C���g����
	CPoint::Update();

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
}

//=================================
//�X�V
//=================================
void CGame::Update(void)
{
	m_pPlayer->Update();

	if (CManager::GetInputKeyboard()->GetPress(DIK_F6))
	{
		CManager::SetMode(CScene::MODE_TITLE);
	}
}

//=================================
//�`��
//=================================
void CGame::Draw(void)
{
}
