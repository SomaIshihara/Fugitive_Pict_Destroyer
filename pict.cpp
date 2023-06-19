//======================================================
//
//�s�N�g���񏈗�[pict.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "pict.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"

//�ÓI�����o�ϐ�
CPict* CPict::m_apPict[MAX_OBJ];
int CPict::m_nNumAll = 0;

//���F���f���p�X
const char* c_apModelPath[PICT_MODEL_NUM] =
{
	"data/MODEL/exithuman_x/01_Body.x",
	"data/MODEL/exithuman_x/02_Head.x",
	"data/MODEL/exithuman_x/03_Left_Arm.x",
	"data/MODEL/exithuman_x/04_Left_Hand.x",
	"data/MODEL/exithuman_x/05_Right_Arm.x",
	"data/MODEL/exithuman_x/06_Right_Hand.x",
	"data/MODEL/exithuman_x/07_Left_Leg.x",
	"data/MODEL/exithuman_x/08_Left_Foot.x",
	"data/MODEL/exithuman_x/09_Right_Leg.x",
	"data/MODEL/exithuman_x/10_Right_Foot.x"
};

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CPict::CPict()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apPict[cnt] == NULL)
		{//�����
			m_apPict[cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;	//ID�o����
			m_nNumAll++;	//�������₷
			break;
		}
	}
	//�l�N���A
	m_pos = VEC3_ZERO;
	m_rot = VEC3_ZERO;
	m_targetPos = VEC3_ZERO;
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h�j
//=================================
CPict::CPict(const D3DXVECTOR3 pos)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apPict[cnt] == NULL)
		{//�����
			m_apPict[cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;	//ID�o����
			m_nNumAll++;	//�������₷
			break;
		}
	}
	//�l�N���A
	m_pos = pos;
	m_rot = VEC3_ZERO;
	m_targetPos = VEC3_ZERO;
}

//=================================
//�f�X�g���N�^
//=================================
CPict::~CPict()
{
}

//========================
//����������
//========================
HRESULT CPict::Init(void)
{
	//���f������
	m_apModel[0] = CModel::Create(c_apModelPath[0], D3DXVECTOR3(0.0f, 35.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_apModel[1] = CModel::Create(c_apModelPath[1], D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_apModel[2] = CModel::Create(c_apModelPath[2], D3DXVECTOR3(-5.0f, 7.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_apModel[3] = CModel::Create(c_apModelPath[3], D3DXVECTOR3(-10.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_apModel[4] = CModel::Create(c_apModelPath[4], D3DXVECTOR3(5.0f, 7.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_apModel[5] = CModel::Create(c_apModelPath[5], D3DXVECTOR3(10.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_apModel[6] = CModel::Create(c_apModelPath[6], D3DXVECTOR3(-3.0f, -8.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_apModel[7] = CModel::Create(c_apModelPath[7], D3DXVECTOR3(0.0f, -12.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_apModel[8] = CModel::Create(c_apModelPath[8], D3DXVECTOR3(3.0f, -8.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_apModel[9] = CModel::Create(c_apModelPath[9], D3DXVECTOR3(0.0f, -12.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//���f���e�q�ݒ�
	m_apModel[0]->SetParent(NULL);
	m_apModel[1]->SetParent(m_apModel[0]);
	m_apModel[2]->SetParent(m_apModel[0]);
	m_apModel[3]->SetParent(m_apModel[2]);
	m_apModel[4]->SetParent(m_apModel[0]);
	m_apModel[5]->SetParent(m_apModel[4]);
	m_apModel[6]->SetParent(m_apModel[0]);
	m_apModel[7]->SetParent(m_apModel[6]);
	m_apModel[8]->SetParent(m_apModel[0]);
	m_apModel[9]->SetParent(m_apModel[8]);

	m_targetPos = D3DXVECTOR3(-100.0f, 0.0f, -100.0f);

	//�ł���
	return S_OK;
}

//========================
//�I������
//========================
void CPict::Uninit(void)
{
	m_apPict[m_nID] = NULL;
	for (int cnt = 0; cnt < PICT_MODEL_NUM; cnt++)
	{//�������
		m_apModel[cnt]->Uninit();
		delete m_apModel[cnt];
		m_apModel[cnt] = NULL;
	}
}

//========================
//�X�V����
//========================
void CPict::Update(void)
{
	D3DXVECTOR3 targetLength = m_targetPos - m_pos;
	if (D3DXVec3Length(&targetLength) >= 10.0f)
	{
		float fTargetWidth, fTargetDepth;
		float fTargetRot;

		fTargetWidth = m_targetPos.x - m_pos.x;
		fTargetDepth = m_targetPos.z - m_pos.z;

		fTargetRot = atan2f(fTargetWidth, fTargetDepth);

		m_pos.x += sinf(fTargetRot) * 1.2f;
		m_pos.z += cosf(fTargetRot) * 1.2f;
	}
}

//========================
//�`�揈��
//========================
void CPict::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//�f�o�C�X�擾
	CTexture* pTexture = CManager::GetTexture();						//�e�N�X�`���I�u�W�F�N�g�擾
	D3DXMATRIX mtxRot, mtxTrans, mtxTexture;							//�v�Z�p
	D3DMATERIAL9 matDef;												//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;													//�}�e���A���f�[�^�ւ̃|�C���^

	//���݂̃}�e���A���擾
	pDevice->GetMaterial(&matDef);

	//���f���擾
	//���[���h�}�g���b�N�X������
	D3DXMatrixIdentity(&m_mtxWorld);

	//�g�k�𔽉f
	//D3DXMatrixScaling(&mtxScall, FENCE_SCALE, FENCE_SCALE, FENCE_SCALE);
	//D3DXMatrixMultiply(&g_aFence[nCount].mtxWorld, &g_aFence[nCount].mtxWorld, &mtxScall);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu���f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���f���`��
	for (int cnt = 0; cnt < PICT_MODEL_NUM; cnt++)
	{
		m_apModel[cnt]->Draw();
	}

	//�}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//========================
//��������
//========================
CPict* CPict::Create(D3DXVECTOR3 pos)
{
	CPict* pPict = NULL;

	if (pPict == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pPict = new CPict(pos);

		//������
		pPict->Init();

		return pPict;
	}
	else
	{
		return NULL;
	}
}
