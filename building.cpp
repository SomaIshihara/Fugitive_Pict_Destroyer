//======================================================
//
//��������[building.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "building.h"
#include "renderer.h"
#include "texture.h"
#include "objectX.h"
#include "pict.h"
#include "score.h"
#include <stdio.h>
#include <assert.h>

//�}�N��
#define BUILDING_DAMAGE_ALPHA	(0.65f)	//�������_���[�W��H��������̐ԐF�̋�ő�l
#define BUILDING_DAMAGE_TIME	(60)	//�Ԃ����鎞��

//�ÓI�����o�ϐ�
CBuilding* CBuilding::m_apBuilding[MAX_OBJ];
CBuilding::BuildingParam CBuilding::m_aBuildingParam[MAX_OBJ];
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
	m_pos = VEC3_ZERO;
	m_rot = VEC3_ZERO;
	m_fWidth = FLOAT_ZERO;
	m_fHeight = FLOAT_ZERO;
	m_fDepth = FLOAT_ZERO;
	m_nIdx = 0;
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
	m_nIdx = nIdx;
	m_nEndurance = m_aBuildingParam[m_nIdx].nEndurance;
	m_pos = pos;
	m_rot = rot;
	m_fWidth = FLOAT_ZERO;
	m_fHeight = FLOAT_ZERO;
	m_fDepth = FLOAT_ZERO;

	//�T�C�Y�ݒ�
	D3DXVECTOR3 vtxMin, vtxMax;
	CObjectX::Model* model = CObjectX::GetModel(m_nIdx);
	model->m_collision.GetVtx(&vtxMin, &vtxMax);
	m_fWidth = vtxMax.x - vtxMin.x;
	m_fHeight = vtxMax.y - vtxMin.y;
	m_fDepth = vtxMax.z - vtxMin.z;
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
	SetType(TYPE_BUILDING);	//�����Ƃ���

	return S_OK;
}

//========================
//�I������
//========================
void CBuilding::Uninit(void)
{
	//�����N���X���ł̏���
	m_apBuilding[m_nID] = NULL;

	//�������g�j��
	Release();
}

//========================
//�X�V����
//========================
void CBuilding::Update(void)
{
	if (m_fRedAlpha >= FLOAT_ZERO)
	{//�܂��Ԃ�
		//�ԐF������炷
		m_fRedAlpha -= BUILDING_DAMAGE_ALPHA / BUILDING_DAMAGE_TIME;

		if (m_fRedAlpha < FLOAT_ZERO)
		{//�Ԃ��Ȃ��Ȃ���
			m_fRedAlpha = FLOAT_ZERO;
		}
	}
}

//========================
//�`�揈��
//========================
void CBuilding::Draw(void)
{
	if (m_nEndurance > 0)
	{//�ϋv���c���Ă���
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//�f�o�C�X�擾
		CTexture* pTexture = CManager::GetTexture();						//�e�N�X�`���I�u�W�F�N�g�擾
		D3DXMATRIX mtxRot, mtxTrans, mtxTexture;							//�v�Z�p
		D3DMATERIAL9 matDef;												//���݂̃}�e���A���ۑ��p
		D3DXMATERIAL *pMat;													//�}�e���A���f�[�^�ւ̃|�C���^

		//���݂̃}�e���A���擾
		pDevice->GetMaterial(&matDef);

		//���f���擾
		//���[���h�}�g���b�N�X������
		D3DXMatrixIdentity(&mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		//�ʒu���f
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		//�}�e���A���f�[�^�ւ̃|�C���^�擾
		CObjectX::Model* model = CObjectX::GetModel(m_nIdx);
		pMat = (D3DXMATERIAL*)model->m_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)model->m_dwNumMatModel; nCntMat++)
		{
			//�}�e���A���ݒ�
			D3DMATERIAL9 changeMat = pMat[nCntMat].MatD3D;

			//�_���[�W��ԂȂ�Ԓǉ�
			if (m_fRedAlpha > FLOAT_ZERO)
			{
				changeMat.Diffuse.r = 1.0f * m_fRedAlpha + changeMat.Diffuse.r * (1.0f - m_fRedAlpha);
				changeMat.Diffuse.g = 0.0f * m_fRedAlpha + changeMat.Diffuse.g * (1.0f - m_fRedAlpha);
				changeMat.Diffuse.b = 0.0f * m_fRedAlpha + changeMat.Diffuse.b * (1.0f - m_fRedAlpha);
			}

			pDevice->SetMaterial(&changeMat);

			//�e�N�X�`���ݒ�
			pDevice->SetTexture(0, pTexture->GetAddress(model->m_pIdxtexture[nCntMat]));

			//���f���`��
			model->m_pMesh->DrawSubset(nCntMat);
		}

		//�}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
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
	{//�S��
		m_nEndurance = 0;	//�ϋv0�ɂ���

		for (int cnt = 0; cnt < MAX_OBJ; cnt++)
		{//�S�I�u�W�F�N�g����
			CPictDestroyer* pPict = CPictDestroyer::GetPict(cnt);	//�I�u�W�F�N�g�擾

			if (pPict != NULL)	//�k���`�F
			{//�Ȃ񂩂���
				if (pPict->GetTarget() == this)
				{//�������^�[�Q�b�g
					pPict->UnsetTarget();	//�^�[�Q�b�g�O��
				}
			}
		}
	}

	//�Ԃ�����
	m_fRedAlpha = BUILDING_DAMAGE_ALPHA;
}

//========================
//�p�����[�^�ǂݍ���
//========================
void CBuilding::LoadParam(const char* pPath)
{
	FILE* pFile;	//�t�@�C���|�C���^

	pFile = fopen(pPath, "r");

	if (pFile != NULL)
	{//�ǂݍ��߂�
		int cnt = 0;		//�J�E���^
		char aData[128];	//�ǂݎ�����f�[�^
		char* pSprit;		//������������
		int nSignificant;	//�L������
		int nPower;			//�ׂ���

		fgets(&aData[0],128,pFile);	//���ݓǂݎ��

		//�f�[�^�ǂݎ��
		while (1)
		{
			fgets(&aData[0], 128, pFile);	//�S���ǂݎ��

			if (feof(pFile))
			{//����A�����EOF��
				break;
			}

			//������
			pSprit = strtok(&aData[0], ",");
			strcpy(&m_aBuildingParam[cnt].aName[0], pSprit);

			//���l
			pSprit = strtok(NULL, ",");
			nSignificant = atoi(pSprit);
			pSprit = strtok(NULL, ",");
			nPower = atoi(pSprit);
			m_aBuildingParam[cnt].nValue = (long long)nSignificant * pow(10, nPower);

			//�ϋv
			pSprit = strtok(NULL, ",");
			nSignificant = atoi(pSprit);
			pSprit = strtok(NULL, ",");
			nPower = atoi(pSprit);
			m_aBuildingParam[cnt].nEndurance = nSignificant * (int)pow(10, nPower);
			
			cnt++;	//�J�E���^���₷
		}
	}
	else
	{
		assert(false);
	}
}