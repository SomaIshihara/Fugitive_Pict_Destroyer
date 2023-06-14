//======================================================
//
//�X�R�A����[score.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "object.h"
#include "score.h"
#include "number.h"

//�ÓI�����o�ϐ�
int CScore::m_nScore = 0;

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CScore::CScore(int nPriority) : CObject(nPriority)
{
	//�l�N���A
	m_nIdxTexture = -1;
	m_pos = VEC3_ZERO;
	m_rot = VEC3_ZERO;
	m_fOneWidth = FLOAT_ZERO;
	m_fOneHeight = FLOAT_ZERO;
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h�j
//=================================
CScore::CScore(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fOneWidth, const float fOneHeight, int nPriority) : CObject(nPriority)
{
	//�l�ݒ�
	m_nIdxTexture = -1;
	m_pos = pos;
	m_rot = rot;
	m_fOneWidth = fOneWidth;
	m_fOneHeight = fOneHeight;
}

//=================================
//�f�X�g���N�^
//=================================
CScore::~CScore()
{
}

//=================================
//������
//=================================
HRESULT CScore::Init(void)
{
	for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
	{//�����I�u�W�F�N�g������
		m_pNumber[cnt] = NULL;
	}

	//�e�N�X�`���ǂݍ���
	CTexture* pTexture = CManager::GetTexture();
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\Number_Rank_01.png", 10, 1);

	//�X�R�A�ݒ�
	Set(0);

	//�ł���
	return S_OK;
}

//=================================
//�I��
//=================================
void CScore::Uninit(void)
{
	for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
	{//�����I�u�W�F�N�g�I��
		if (m_pNumber[cnt] != NULL)
		{//���v�B���g�͂���
			m_pNumber[cnt]->Uninit();
		}
	}

	//�X�R�A�I�u�W�F�N�g�j��
	Release();
}

//=================================
//�X�V
//=================================
void CScore::Update(void)
{
//#ifdef _DEBUG
	//[debug]�����ݒ�
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F1) == true)
	{
		Set(12345678);	//�f�o�b�O�p�����炱��ł������
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F2) == true)
	{
		Add(100);	//�f�o�b�O�p�����炱��ł������
	}
//#endif // DEBUG
	for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
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
void CScore::Draw(void)
{
	for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
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
CScore* CScore::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fOneWidth, const float fOneHeight)
{
	CScore* pScore = NULL;

	if (pScore == NULL)
	{
		//���d�w�i�Ǘ��I�u�W�F�N�g����
		pScore = new CScore;

		//���d�w�i�Ǘ��I�u�W�F�N�g������
		pScore->Init();

		//�w�i�p�I�u�W�F�N�g2D����
		for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
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
//�X�R�A�ݒ菈��
//=================================
void CScore::Set(const int nScore)
{
	//�����ݒ�
	m_nScore = nScore;
}

//=================================
//�X�R�A���Z����
//=================================
void CScore::Add(const int nAdd)
{
	//�������Z
	m_nScore += nAdd;
}

//=================================
//������������
//=================================
void CScore::CutNumber(void)
{
	for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
	{//�����I�u�W�F�N�g�ɓn��
		if (m_pNumber[cnt] != NULL)
		{//���v�B���g�͂���
			m_pNumber[cnt]->SetNumber(m_nScore, cnt);
		}
	}
}