//======================================================
//
//��������[building.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "building.h"

//�ÓI�����o�ϐ�
CBuilding* CBuilding::m_apBuilding[MAX_OBJ];
int CBuilding::m_nNumAll = 0;

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CBuilding::CBuilding()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apBuilding[cnt] == NULL)
		{//�����
			m_apBuilding[cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;	//ID�o����
			m_nNumAll++;	//�������₷
			break;
		}
	}
	m_nEndurance = INT_ZERO;
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h�j
//=================================
CBuilding::CBuilding(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nIdx)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apBuilding[cnt] == NULL)
		{//�����
			m_apBuilding[cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;	//ID�o����
			m_nNumAll++;	//�������₷
			break;
		}
	}
	m_nEndurance = INT_ZERO;
}

//=================================
//�f�X�g���N�^
//=================================
CBuilding::~CBuilding()
{
}

//========================
//����������
//========================
HRESULT CBuilding::Init(void)
{
	//�e����
	CObjectX::Init();

	return S_OK;
}

//========================
//�I������
//========================
void CBuilding::Uninit(void)
{
	//�����N���X���ł̏���
	m_apBuilding[m_nID] = NULL;

	//�e����
	CObjectX::Uninit();
}

//========================
//�X�V����
//========================
void CBuilding::Update(void)
{
	//�e����
	CObjectX::Update();
}

//========================
//�`�揈��
//========================
void CBuilding::Draw(void)
{
	//�e����
	CObjectX::Draw();
}

//========================
//��������
//========================
CBuilding* CBuilding::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nIdx)
{
	CBuilding* pBuilding = NULL;

	if (pBuilding == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pBuilding = new CBuilding(pos, rot, nIdx);

		//������
		pBuilding->Init();

		return pBuilding;
	}
	else
	{
		return NULL;
	}
}

//========================
//�_���[�W�t�^����
//========================
void CBuilding::AddDamage(int nDamage)
{
	m_nEndurance -= nDamage;
	if (m_nEndurance <= 0)
	{
		Uninit();
	}
}
