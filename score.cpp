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
#include "symbol.h"
#include "building.h"
#include "xmodel.h"

//�}�N��
#define SCORE_INVISIBLE	(4)	//�����������\���ɂ���

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CScore::CScore(int nPriority) : CObject(nPriority)
{
	//�l�N���A
	m_nIdxTextureNumber = -1;
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
	m_nIdxTextureNumber = -1;
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
		m_pNumberTenHundred[cnt] = NULL;
	}

	//�e�N�X�`���ǂݍ���
	CTexture* pTexture = CManager::GetTexture();
	m_nIdxTextureNumber = pTexture->Regist("data\\TEXTURE\\tex_Score_01.png", 10, 1);
	m_nIdxtextureSymbol = pTexture->Regist("data\\TEXTURE\\tex_Symbol_01.png", 12, 1);

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
		if (m_pNumberTenHundred[cnt] != NULL)
		{//���v�B���g�͂���
			m_pNumberTenHundred[cnt]->Uninit();
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
	//�����I�u�W�F�N�g�S����
	Set(0);
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{
		CBuilding* pBuilding = CBuilding::GetBuilding(cnt);	//�����I�u�W�F�N�g�擾
		if (pBuilding != NULL)
		{//�Ȃ񂩂���
			CXModel* pModel = CXModel::GetTop();
			int nModelNum = 0;
			while (pModel != NULL && pModel != pBuilding->GetModel())
			{
				pModel = pModel->GetNext();
				nModelNum++;
			}
			CBuilding::BuildingParam buildingParam = CBuilding::GetBuildingParam(nModelNum);		//�����p�����[�^�擾
			float fParcent = ((float)pBuilding->GetEndurance() / buildingParam.nEndurance);
			long long nScore = (1.0f - fParcent) * buildingParam.nValue;	//�X�R�A�Z�o
			Add(nScore);
		}
		else
		{//�����Ȃ�
			break;
		}
	}

	for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
	{//�����I�u�W�F�N�g�X�V
		if (m_pNumberHMillion[cnt] != NULL)
		{//���v�B���g�͂���
			//�I�u�W�F�N�g2D�̍X�V����
			m_pNumberHMillion[cnt]->Update();
		}
	}
	for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
	{//�����I�u�W�F�N�g�X�V
		if (m_pNumberTenHundred[cnt] != NULL)
		{//���v�B���g�͂���
			//�I�u�W�F�N�g2D�̍X�V����
			m_pNumberTenHundred[cnt]->Update();
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
	m_pSymbolPic->Draw();
	for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
	{//�����I�u�W�F�N�g�X�V
		if (m_pNumberHMillion[cnt] != NULL)
		{//���v�B���g�͂���
			//�I�u�W�F�N�g2D�̍X�V����
			m_pNumberHMillion[cnt]->Draw();
		}
	}
	m_pSymbolHMillion->Draw();
	for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
	{//�����I�u�W�F�N�g�`��
		if (m_pNumberTenHundred[cnt] != NULL)
		{//���v�B���g�͂���
			m_pNumberTenHundred[cnt]->Draw();
		}
	}
	m_pSymbolTenHundred->Draw();
}

//=================================
//��������
//=================================
CScore* CScore::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fOneWidth, const float fOneHeight)
{
	CScore* pScore = NULL;

	if (pScore == NULL)
	{
		//�X�R�A�Ǘ��I�u�W�F�N�g����
		pScore = new CScore;

		//�X�R�A�Ǘ��I�u�W�F�N�g������
		pScore->Init();

		//����

		//��
		pScore->m_pSymbolTenHundred = pScore->m_pSymbolPic->Create(pos, rot, fOneWidth, fOneHeight);	//����8��+�L��2
		pScore->m_pSymbolTenHundred->Init();
		pScore->m_pSymbolTenHundred->BindTexture(pScore->m_nIdxtextureSymbol);
		pScore->m_pSymbolTenHundred->SetSymbol(CScore::TYPE_TEN_HUNDRED);

		for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
		{
			//����
			pScore->m_pNumberTenHundred[cnt] = pScore->m_pNumberTenHundred[cnt]->Create(pos + D3DXVECTOR3(-fOneWidth * (cnt + 1), 0.0f, 0.0f), rot, fOneWidth, fOneHeight);	//����4��+�L��2

			//������
			pScore->m_pNumberTenHundred[cnt]->Init();

			//�e�N�X�`���ݒ�
			pScore->m_pNumberTenHundred[cnt]->BindTexture(pScore->m_nIdxTextureNumber);
		}
		
		//��
		pScore->m_pSymbolHMillion = pScore->m_pSymbolPic->Create(pos + D3DXVECTOR3(-fOneWidth * 5, 0.0f, 0.0f), rot, fOneWidth, fOneHeight);	//����4��+�L��
		pScore->m_pSymbolHMillion->Init();
		pScore->m_pSymbolHMillion->BindTexture(pScore->m_nIdxtextureSymbol);
		pScore->m_pSymbolHMillion->SetSymbol(CScore::TYPE_HMILLION);

		for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
		{
			//����
			pScore->m_pNumberHMillion[cnt] = CNumber::Create(pos + D3DXVECTOR3(-fOneWidth * (cnt + 6), 0.0f, 0.0f), rot, fOneWidth, fOneHeight);

			//������
			pScore->m_pNumberHMillion[cnt]->Init();

			//�e�N�X�`���ݒ�
			pScore->m_pNumberHMillion[cnt]->BindTexture(pScore->m_nIdxTextureNumber);
		}

		//�ʉ݋L��
		pScore->m_pSymbolPic = CSymbol::Create(pos + D3DXVECTOR3(-fOneWidth * 10, 0.0f, 0.0f), rot, fOneWidth, fOneHeight);
		pScore->m_pSymbolPic->Init();
		pScore->m_pSymbolPic->BindTexture(pScore->m_nIdxtextureSymbol);
		pScore->m_pSymbolPic->SetSymbol(CScore::TYPE_PIC);

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
		if (m_pNumberTenHundred[cnt] != NULL)
		{//���v�B���g�͂���
			m_pNumberHMillion[cnt]->SetNumber(m_nScore, cnt + SCORE_INVISIBLE + 4);
		}
	}
	for (int cnt = 0; cnt < SCORE_DIGIT; cnt++)
	{//�����I�u�W�F�N�g�ɓn��
		if (m_pNumberTenHundred[cnt] != NULL)
		{//���v�B���g�͂���
			m_pNumberTenHundred[cnt]->SetNumber(m_nScore, cnt + SCORE_INVISIBLE);
		}
	}
}