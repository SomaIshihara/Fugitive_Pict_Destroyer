//======================================================
//
//�Q�[���V�[��[game.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "game.h"
#include "pause.h"
#include "result.h"
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
int CGame::m_nATKBuilding = CManager::INT_ZERO;
int CGame::m_nDestBuilding = CManager::INT_ZERO;

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
	m_pMeshField = CMeshField::Create(D3DXVECTOR3(-1280.0f, 0.0f, 1280.0f), CManager::VEC3_ZERO, 64.0f, 64.0f, 40, 40);

	//�I�u�W�F�N�g����+������
	m_pTimer = CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f + 24.0f, 32.0f, 0.0f), CManager::VEC3_ZERO, 48.0f, 72.0f);
	m_pTimer->Set(2, CTimer::COUNT_DOWN);
	m_pScore = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - 24.0f, 32.0f, 0.0f), CManager::VEC3_ZERO, 40.0f, 64.0f);
	CObjectX* pAgit = CObjectX::Create(D3DXVECTOR3(600.0f,0.0f,0.0f), CManager::VEC3_ZERO, CManager::GetAgitModel());
	CPict::SetAgit(pAgit);
	CMeshSky::Create(CManager::VEC3_ZERO, CManager::VEC3_ZERO, 10000.0f, 8, 8);
	m_pHaveNum[0] = CHaveNum::Create(D3DXVECTOR3(SCREEN_WIDTH - 30.0f, 100.0f, 0.0f), CManager::VEC3_ZERO, 60.0f, 72.0f, 2, 0);//�A�C�R���ԍ���
	m_pHaveNum[1] = CHaveNum::Create(D3DXVECTOR3(SCREEN_WIDTH - 30.0f, 172.0f, 0.0f), CManager::VEC3_ZERO, 60.0f, 72.0f, 2, 0);//�A�C�R���ԍ���
	m_pHaveNum[2] = CHaveNum::Create(D3DXVECTOR3(SCREEN_WIDTH - 30.0f, 244.0f, 0.0f), CManager::VEC3_ZERO, 60.0f, 72.0f, 5,0);//�A�C�R���ԍ���

	//��
	CItemBullet::Create(D3DXVECTOR3(0.0f, 0.0f, 10.0f), CManager::VEC3_ZERO);

	//�}�b�v�f�[�^�ǂݍ��݂Ɣz�u
	CObjectX::LoadData("data\\Fugitive_Pict_MapData_v120.ismd");

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
	if (m_pPlayer != nullptr)
	{//�v���C���[�I��
		m_pPlayer->Uninit();
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}
	//���U���g�j��
	if (m_pResult != nullptr)
	{//�v���C���[�I��
		m_pResult->Uninit();
		delete m_pResult;
		m_pResult = nullptr;
	}
	//�|�[�Y�j���i�ꉞ�j
	if (m_pPause != nullptr)
	{//�v���C���[�I��
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = nullptr;
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

	if (m_pResult != nullptr)
	{//���U���g
		m_pResult->Update();
	}
	else
	{//�Q�[��
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

			//�X�R�A�Z�o
			CulcScore();

			//���ԊǗ�
			if (m_pTimer->GetTime() <= 0)
			{//���Ԑ؂�
				if (m_pResult == nullptr)
				{
					m_pResult = CResult::Create();
				}
			}
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

//=================================
//�X�R�A�v�Z
//=================================
void CGame::CulcScore(void)
{
	//�X�R�A�ރ��Z�b�g
	m_pScore->Set(0);
	m_nATKBuilding = 0;
	m_nDestBuilding = 0;

	//�����I�u�W�F�N�g�S����
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

			//�X�R�A�Z�o
			float fParcent;
			long long nScore;

			if (pBuilding->GetUnique() == false)
			{//�v�Z�Z�o
				int nEndurance = pBuilding->GetEndurance();
				int nMaxEndurance = HAVE_LIFE(pBuilding->GetLv());

				if (nEndurance < nMaxEndurance)
				{//�����Ă���
					m_nATKBuilding++;	//�U������

					if (nEndurance <= 0)
					{//�S��
						m_nDestBuilding++;
					}

					//��Q�z�v�Z
					fParcent = ((float)nEndurance / nMaxEndurance);
					nScore = (1.0f - fParcent) * HAVE_VALUE(pBuilding->GetLv());
					m_pScore->Add(nScore);
				}
			}
			else
			{//��
				int nMaxEndurance = pBuilding->GetSigEndurance() * pow(10, pBuilding->GetPowEndurance());
				int nEndurance = pBuilding->GetEndurance();

				if (nEndurance < nMaxEndurance)
				{//�����Ă���
					m_nATKBuilding++;	//�U������

					if (nEndurance <= 0)
					{//�S��
						m_nDestBuilding++;
					}

					//��Q�z�v�Z
					fParcent = ((float)nEndurance / nMaxEndurance);
					long long nValue = pBuilding->GetSigValue() * pow(10, pBuilding->GetPowValue());
					nScore = (1.0f - fParcent) * nValue;
					m_pScore->Add(nScore);
				}
			}
		}
		else
		{//�����Ȃ�
			break;
		}
	}
}
