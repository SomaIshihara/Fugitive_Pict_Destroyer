//======================================================
//
//���U���g�V�[��[result.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "precompile.h"
#include "manager.h"
#include "result.h"
#include "number.h"
#include "button.h"

//�ÓI�����o�ϐ�
const int CResult::HEADLINE_TIME = 60;
const int CResult::ATK_TIME = HEADLINE_TIME + 40;
const int CResult::DEST_TIME = ATK_TIME + 40;
const int CResult::VALUE_TIME = DEST_TIME + 60;
const int CResult::RANK_TIME = VALUE_TIME + 60;

//=================================
//�R���X�g���N�^
//=================================
CResult::CResult()
{
	m_pButton = nullptr;
	m_nCounter = INT_ZERO;
}

//=================================
//�f�X�g���N�^
//=================================
CResult::~CResult()
{
}

//=================================
//������
//=================================
HRESULT CResult::Init(void)
{
	m_nCounter = 0;
	return S_OK;
}

//=================================
//�I��
//=================================
void CResult::Uninit(void)
{
	//�������ŊǗ����Ă�����̂����j��
	m_pButton->Uninit();
	m_pButton = nullptr;
}

//=================================
//�X�V
//=================================
void CResult::Update(void)
{
	m_nCounter++;

	if (m_nCounter >= HEADLINE_TIME)
	{//���o������

	}
	else if (m_nCounter >= ATK_TIME)
	{//�U�����\��

	}
	else if (m_nCounter >= DEST_TIME)
	{//�S�󐔕\��

	}
	else if (m_nCounter >= VALUE_TIME)
	{//��Q���z�\��

	}
	else if (m_nCounter >= RANK_TIME)
	{//�����L���O�J�ڃ{�^���\��
		m_pButton = CButton2D::Create(D3DXVECTOR3(1000.0f, 600.0f, 0.0f), VEC3_ZERO, 311.0f, 111.0f);
	}

	if (m_pButton != nullptr && m_pButton->IsClickTrigger() == true)
	{//�����L���O�J��
		CManager::SetMode(CScene::MODE_RANKING);
	}
}

//=================================
//�`��
//=================================
void CResult::Draw(void)
{
	//����ɂ���Ă����
}
