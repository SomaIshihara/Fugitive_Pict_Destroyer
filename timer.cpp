//======================================================
//
//�^�C�}�[����[timer.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "object.h"
#include "timer.h"
#include "number.h"

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CTimer::CTimer(int nPriority) : CObject(nPriority)
{
	//�l�N���A
	m_nIdxTexture = -1;
	m_pos = VEC3_ZERO;
	m_rot = VEC3_ZERO;
	m_fOneWidth = FLOAT_ZERO;
	m_fOneHeight = FLOAT_ZERO;
	m_nCounter = 0;
	m_count = COUNT_DOWN;
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h�j
//=================================
CTimer::CTimer(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fOneWidth, const float fOneHeight, int nPriority) : CObject(nPriority)
{
	//�l�ݒ�
	m_nIdxTexture = -1;
	m_pos = pos;
	m_rot = rot;
	m_fOneWidth = fOneWidth;
	m_fOneHeight = fOneHeight;
	m_nCounter = 0;
	m_count = COUNT_DOWN;
}

//=================================
//�f�X�g���N�^
//=================================
CTimer::~CTimer()
{
}

//=================================
//������
//=================================
HRESULT CTimer::Init(void)
{
	for (int cnt = 0; cnt < TIME_DIGIT; cnt++)
	{//�����I�u�W�F�N�g������
		m_pNumber[cnt] = NULL;
	}

	//�e�N�X�`���ǂݍ���
	CTexture* pTexture = CManager::GetTexture();
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\Number_Rank_01.png", 10, 1);

	//�^�C�}�[�J�E���^���Z�b�g
	m_nCounter = 0;
	m_count = COUNT_DOWN;

	//�^�C���ݒ�
	Set(0, COUNT_UP);

	//�ł���
	return S_OK;
}

//=================================
//�I��
//=================================
void CTimer::Uninit(void)
{
	for (int cnt = 0; cnt < TIME_DIGIT; cnt++)
	{//�����I�u�W�F�N�g�I��
		if (m_pNumber[cnt] != NULL)
		{//���v�B���g�͂���
			m_pNumber[cnt]->Uninit();
		}
	}

	//�^�C���I�u�W�F�N�g�j��
	Release();
}

//=================================
//�X�V
//=================================
void CTimer::Update(void)
{
//#ifdef _DEBUG
	//[debug]�����ݒ�
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F3) == true)
	{
		Set(120,COUNT_DOWN);	//�f�o�b�O�p�����炱��ł������
	}
//#endif // DEBUG
	//�^�C�}�[�X�V
	m_nCounter++;

	if (m_nCounter >= MAX_FPS)
	{
		//�J�E���^���Z�b�g
		m_nCounter = 0;

		switch (m_count)
		{
		case COUNT_UP:
			m_nTime++;	//���Z
			if (m_nTime >= (int)pow(10, TIME_DIGIT))
			{//�����̌��E�𒴂���
				m_nTime = (int)pow(10, TIME_DIGIT) - 1;
			}
			break;

		case COUNT_DOWN:
			m_nTime--;	//���Z
			if (m_nTime < INT_ZERO)
			{//0���������
				m_nTime = INT_ZERO;
			}
			break;
		}
	}

	for (int cnt = 0; cnt < TIME_DIGIT; cnt++)
	{//�����I�u�W�F�N�g�X�V
		if (m_pNumber[cnt] != NULL)
		{//���v�B���g�͂���
			//�I�u�W�F�N�g2D�̍X�V����
			m_pNumber[cnt]->Update();
		}
	}

	//��������
	CutNumber();
}

//=================================
//�`��
//=================================
void CTimer::Draw(void)
{
	for (int cnt = 0; cnt < TIME_DIGIT; cnt++)
	{//�����I�u�W�F�N�g�`��
		if (m_pNumber[cnt] != NULL)
		{//���v�B���g�͂���
			m_pNumber[cnt]->Draw();
		}
	}
}

//=================================
//��������
//=================================
CTimer* CTimer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fOneWidth, const float fOneHeight)
{
	CTimer* pScore = NULL;

	if (pScore == NULL)
	{
		//���d�w�i�Ǘ��I�u�W�F�N�g����
		pScore = new CTimer;

		//���d�w�i�Ǘ��I�u�W�F�N�g������
		pScore->Init();

		//�w�i�p�I�u�W�F�N�g2D����
		for (int cnt = 0; cnt < TIME_DIGIT; cnt++)
		{//1���������`�e�N�X�`���ݒ�
			//����
			pScore->m_pNumber[cnt] = pScore->m_pNumber[cnt]->Create(pos + D3DXVECTOR3(-fOneWidth * cnt,0.0f,0.0f), rot, fOneWidth, fOneHeight);

			//������
			pScore->m_pNumber[cnt]->Init();

			//�e�N�X�`���ݒ�
			pScore->m_pNumber[cnt]->BindTexture(pScore->m_nIdxTexture);
		}

		return pScore;
	}
	else
	{
		return NULL;
	}
}

//=================================
//�^�C���ݒ菈��
//=================================
void CTimer::Set(const int nScore,COUNT type)
{
	//�����ݒ�
	m_nTime = nScore;

	//��������
	CutNumber();
}

//=================================
//������������
//=================================
void CTimer::CutNumber(void)
{
	for (int cnt = 0; cnt < TIME_DIGIT; cnt++)
	{//�����I�u�W�F�N�g�ɓn��
		if (m_pNumber[cnt] != NULL)
		{//���v�B���g�͂���
			m_pNumber[cnt]->SetNumber(m_nTime, cnt);
		}
	}
}