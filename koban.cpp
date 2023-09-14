//======================================================
//
//��ԃv���O����[koban.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "koban.h"
#include "picto.h"
#include "building.h"

//�}�N��
#define KOBAN_BUILDING_SEARCH_NUM	(5)		//�����T���񐔁i������߂���ƕ����Ȃ��j
#define KOBAN_NOBUILDING_TIME_PERCE	(0.5f)	//������������Ȃ������ꍇ�̒T�����Ԋ���

//�ÓI�����o�ϐ�
CKoban* CKoban::m_apKoban[];
int CKoban::m_nNumAll = CManager::INT_ZERO;
int CKoban::m_nCounterSpawn = CManager::INT_ZERO;
int CKoban::m_nSpawnSpan = CManager::INT_ZERO;
int CKoban::m_nPoliceMax = CManager::INT_ZERO;

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CKoban::CKoban()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apKoban[cnt] == nullptr)
		{//�����
			m_apKoban[cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;
			m_nNumAll++;	//�������₷
			break;
		}
	}
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h�j
//=================================
CKoban::CKoban(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel) : CObjectX(pos, rot, pModel)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apKoban[cnt] == nullptr)
		{//�����
			m_apKoban[cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;
			m_nNumAll++;	//�������₷
			break;
		}
	}
}

//=================================
//�f�X�g���N�^
//=================================
CKoban::~CKoban()
{
}

//=================================
//������
//=================================
HRESULT CKoban::Init(void)
{
	CObjectX::Init();
	return S_OK;
}

//=================================
//�I��
//=================================
void CKoban::Uninit(void)
{
	m_apKoban[m_nID] = nullptr;
	m_nNumAll--;

	//�Ȃ��Ȃ�����ÓI�����o�ϐ�������
	if (m_nNumAll <= 0)
	{
		m_nCounterSpawn = CManager::INT_ZERO;
		m_nSpawnSpan = CManager::INT_ZERO;
		m_nPoliceMax = CManager::INT_ZERO;
	}

	CObjectX::Uninit();
}

//=================================
//�X�V
//=================================
void CKoban::Update(void)
{
	CObjectX::Update();
}

//=================================
//�`��
//=================================
void CKoban::Draw(void)
{
	CObjectX::Draw();
}

//=================================
//��ԋ��ʍX�V����
//=================================
void CKoban::CommonUpdate(void)
{
	if (CPictoPolice::GetNumAll() < m_nPoliceMax)
	{//�l��s��
		m_nCounterSpawn++;	//�����J�E���^���₷
		if (m_nCounterSpawn >= m_nSpawnSpan)
		{//������
			int nSpawnKoban = rand() % m_nNumAll;	//�K���Ɍ��߂�
			int nAssignBuilding;

			for (int cnt = 0; cnt < KOBAN_BUILDING_SEARCH_NUM; cnt++)
			{
				nAssignBuilding = rand() % CBuilding::GetNumAll();
				if (CBuilding::GetBuilding(nAssignBuilding)->GetEndurance() > 0)
				{//�ϋv�l���c���Ă���
					CPictoPolice* pPolice = CPictoPolice::Create(m_apKoban[nSpawnKoban]->GetPos());	//�K���Ɍ��߂���Ԃ��畦����
					pPolice->SetTargetObj(CBuilding::GetBuilding(nAssignBuilding));					//�K���Ɍ��߂������ɔz��
					m_nCounterSpawn = 0;	//�J�E���^���Z�b�g
					break;
				}
			}

			//���〈����Ȃ���ł���->�������Ԃ̉�������������܂��T��
			m_nCounterSpawn = (int)((float)m_nSpawnSpan * KOBAN_NOBUILDING_TIME_PERCE);	//�J�E���^���Z�b�g
		}
	}
}

//=================================
//����
//=================================
CKoban* CKoban::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel)
{
	CKoban* pKoban = nullptr;

	if (pKoban == nullptr)
	{
		//��Ԃ̐���
		pKoban = new CKoban(pos, rot, pModel);

		//������
		pKoban->Init();

		return pKoban;
	}
	else
	{
		return nullptr;
	}
}

//=================================
//��Ԑݒ�
//=================================
void CKoban::SetKobanParam(const int nSpawnSpan, const int nPoliceMax)
{//���
	m_nSpawnSpan = nSpawnSpan;
	m_nPoliceMax = nPoliceMax;
}
