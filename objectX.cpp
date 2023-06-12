//======================================================
//
//�I�u�W�F�N�g�i3D�j����[object3D.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "objectX.h"

//�ÓI�����o�ϐ�
CObjectX::Model CObjectX::m_aModel[];

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	//�N���A
	m_pos = VEC3_ZERO;
	m_rot = VEC3_ZERO;
	m_fWidth = FLOAT_ZERO;
	m_fHeight = FLOAT_ZERO;
	m_fDepth = FLOAT_ZERO;
	m_nIdx = 0;
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h �ʒu�����j
//=================================
CObjectX::CObjectX(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nIdx, int nPriority) : CObject(nPriority)
{
	//�N���A
	m_pos = pos;
	m_rot = rot;
	m_fWidth = FLOAT_ZERO;
	m_fHeight = FLOAT_ZERO;
	m_fDepth = FLOAT_ZERO;
	m_nIdx = nIdx;
}

//=================================
//�f�X�g���N�^
//=================================
CObjectX::~CObjectX()
{
}

//========================
//����������
//========================
HRESULT CObjectX::Init(void)
{
	return S_OK;
}

//========================
//�I������
//========================
void CObjectX::Uninit(void)
{
	//�������g�j��
	Release();
}

//========================
//�X�V����
//========================
void CObjectX::Update(void)
{
	
}

//========================
//�`�揈��
//========================
void CObjectX::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//�f�o�C�X�擾
	D3DXMATRIX mtxRot, mtxTrans, mtxTexture;							//�v�Z�p
	D3DMATERIAL9 matDef;												//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;													//�}�e���A���f�[�^�ւ̃|�C���^

	//���݂̃}�e���A���擾
	pDevice->GetMaterial(&matDef);

	//���f���擾
	//���[���h�}�g���b�N�X������
	D3DXMatrixIdentity(&mtxWorld);

	//�g�k�𔽉f
	//D3DXMatrixScaling(&mtxScall, FENCE_SCALE, FENCE_SCALE, FENCE_SCALE);
	//D3DXMatrixMultiply(&g_aFence[nCount].mtxWorld, &g_aFence[nCount].mtxWorld, &mtxScall);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	//�ʒu���f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//�}�e���A���f�[�^�ւ̃|�C���^�擾
	pMat = (D3DXMATERIAL*)m_aModel[m_nIdx].m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_aModel[m_nIdx].m_dwNumMatModel; nCntMat++)
	{
		//�}�e���A���ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`���ݒ�
		pDevice->SetTexture(0, m_aModel[m_nIdx].m_apTexture[nCntMat]);

		//���f���`��
		m_aModel[m_nIdx].m_pMesh->DrawSubset(nCntMat);
	}

	//�}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//========================
//��������
//========================
CObjectX* CObjectX::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nIdx)
{
	CObjectX* pObj2D = NULL;

	if (pObj2D == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pObj2D = new CObjectX(pos, rot, nIdx);

		//������
		pObj2D->Init();

		return pObj2D;
	}
	else
	{
		return NULL;
	}
}

//========================
//�t�@�C���ǂݍ��ݏ���
//========================
void CObjectX::Load(const char * pPath, const int nIdx)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//�f�o�C�X�擾

	if (SUCCEEDED(D3DXLoadMeshFromX(
		pPath,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_aModel[nIdx].m_pBuffMat,
		NULL,
		&m_aModel[nIdx].m_dwNumMatModel,
		&m_aModel[nIdx].m_pMesh)))
	{
		//�e�N�X�`���ǂݍ���
		D3DXMATERIAL* pMat;	//�}�e���A���|�C���^

		//�}�e���A�����ɑ΂���|�C���^�擾
		pMat = (D3DXMATERIAL*)m_aModel[nIdx].m_pBuffMat->GetBufferPointer();

		//�e�N�X�`���ǂݍ���
		for (int nCntTex = 0; nCntTex < (int)m_aModel[nIdx].m_dwNumMatModel; nCntTex++)
		{
			if (pMat[nCntTex].pTextureFilename != NULL)
			{
				//�e�N�X�`���ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntTex].pTextureFilename,
					&m_aModel[nIdx].m_apTexture[nCntTex]);
			}
		}
	}
}

//========================
//�t�@�C���j������
//========================
void CObjectX::Unload(void)
{
	for (int cntModel = 0; cntModel < X_MODEL_NUM; cntModel++)
	{
		//���b�V���̔j��
		if (m_aModel[cntModel].m_pMesh != NULL)
		{
			m_aModel[cntModel].m_pMesh->Release();
			m_aModel[cntModel].m_pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (m_aModel[cntModel].m_pBuffMat != NULL)
		{
			m_aModel[cntModel].m_pBuffMat->Release();
			m_aModel[cntModel].m_pBuffMat = NULL;
		}

		//���u���F�e�N�X�`���j��
		for (int cntTexture = 0; cntTexture < X_TEXTURE_NUM; cntTexture++)
		{
			if (m_aModel[cntModel].m_apTexture[cntTexture] != NULL)
			{
				m_aModel[cntModel].m_apTexture[cntTexture]->Release();
				m_aModel[cntModel].m_apTexture[cntTexture] = NULL;
			}
		}
	}
	
}