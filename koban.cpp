//======================================================
//
//��ԃv���O����[koban.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "koban.h"
#include "pict.h"
#include "building.h"

//�ÓI�����o�ϐ�
CKoban* CKoban::m_apKoban[];
int CKoban::m_nNumAll = INT_ZERO;
int CKoban::m_nCounterSpawn = INT_ZERO;
int CKoban::m_nSpawnSpan = INT_ZERO;
int CKoban::m_nPoliceMax = INT_ZERO;

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CKoban::CKoban()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apKoban[cnt] == NULL)
		{//�����
			m_apKoban[cnt] = this;	//�������g�̃|�C���^��o�^
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
		if (m_apKoban[cnt] == NULL)
		{//�����
			m_apKoban[cnt] = this;	//�������g�̃|�C���^��o�^
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
	if (CPictPolice::GetNumAll() < m_nPoliceMax)
	{//�l��s��
		m_nCounterSpawn++;	//�����J�E���^���₷
		if (m_nCounterSpawn >= m_nSpawnSpan)
		{//������
			int nSpawnKoban = rand() % m_nNumAll;	//�K���Ɍ��߂�
			int nAssignBuilding = rand() % CBuilding::GetNumAll();

			CPictPolice* pPolice = CPictPolice::Create(m_apKoban[nSpawnKoban]->GetPos());	//�K���Ɍ��߂���Ԃ��畦����
			pPolice->SetTargetObj(CBuilding::GetBuilding(nAssignBuilding));					//�K���Ɍ��߂������ɔz��

			m_nCounterSpawn = 0;	//�J�E���^���Z�b�g
		}
	}
}

//=================================
//����
//=================================
CKoban* CKoban::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel)
{
	CKoban* pKoban = NULL;

	if (pKoban == NULL)
	{
		//��Ԃ̐���
		pKoban = new CKoban(pos, rot, pModel);

		//������
		pKoban->Init();

		return pKoban;
	}
	else
	{
		return NULL;
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
