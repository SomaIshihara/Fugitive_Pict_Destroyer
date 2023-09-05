//======================================================
//
//���f������[model.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "main.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include <assert.h>

//=================================
//�R���X�g���N�^
//=================================
CModel::CModel()
{
	m_pos = CManager::VEC3_ZERO;
	m_rot = CManager::VEC3_ZERO;
	m_posMotioned = CManager::VEC3_ZERO;
	m_rotMotioned = CManager::VEC3_ZERO;
	m_pParent = NULL;
}

//=================================
//�f�X�g���N�^
//=================================
CModel::~CModel()
{
}

//========================
//����������
//========================
HRESULT CModel::Init(const char * pPath, const D3DXVECTOR3 posOffset, const D3DXVECTOR3 rotOffset)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//�f�o�C�X�擾
	CTexture* pTexture = CManager::GetTexture();						//�e�N�X�`���I�u�W�F�N�g�擾
	m_pIdxtexture = NULL;	//�e�N�X�`���ԍ��|�C���^��NULL�ɂ���

	if (SUCCEEDED(D3DXLoadMeshFromX(
		pPath,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMatModel,
		&m_pMesh)))
	{
		//�e�N�X�`���|�C���^�m��
		if (m_pIdxtexture == NULL)
		{//NULL
		 //�e�N�X�`���ԍ��z��m��
			m_pIdxtexture = new int[(int)m_dwNumMatModel];

			//�����蔻��ݒ���
			////�����蔻�萶��
			//int nNumVtx;		//���_��
			//DWORD dwSizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y
			//BYTE *pVtxBuff;		//���_�o�b�t�@�|�C���^
			//
			////���_�����擾
			//nNumVtx = m_pMesh->GetNumVertices();
			//
			////���_�t�H�[�}�b�g
			//dwSizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());
			//
			////���_�o�b�t�@���b�N
			//m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void **)&pVtxBuff);
			//
			////�ŏ������S�������
			//D3DXVECTOR3 vtx = *(D3DXVECTOR3 *)pVtxBuff;
			//
			//D3DXVECTOR3 vtxMax = vtx;
			//D3DXVECTOR3 vtxMin = vtx;
			//
			//pVtxBuff += dwSizeFVF;
			//
			//for (int nCntVtx = 1; nCntVtx < nNumVtx; nCntVtx++, pVtxBuff += dwSizeFVF)
			//{
			//	D3DXVECTOR3 vtx = *(D3DXVECTOR3 *)pVtxBuff;
			//
			//	if (vtxMax.x < vtx.x)
			//	{
			//		vtxMax.x = vtx.x;
			//	}
			//	if (vtxMax.y < vtx.y)
			//	{
			//		vtxMax.y = vtx.y;
			//	}
			//	if (vtxMax.z < vtx.z)
			//	{
			//		vtxMax.z = vtx.z;
			//	}
			//	if (vtxMin.x > vtx.x)
			//	{
			//		vtxMin.x = vtx.x;
			//	}
			//	if (vtxMin.y > vtx.y)
			//	{
			//		vtxMin.y = vtx.y;
			//	}
			//	if (vtxMin.z > vtx.z)
			//	{
			//		vtxMin.z = vtx.z;
			//	}
			//}
			//
			////�ݒ�
			//m_collision.SetVtx(vtxMin, vtxMax);
			//
			////���_�o�b�t�@�A�����b�N
			//m_pMesh->UnlockVertexBuffer();

			//�e�N�X�`���ǂݍ���
			D3DXMATERIAL* pMat;	//�}�e���A���|�C���^

			//�}�e���A�����ɑ΂���|�C���^�擾
			pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

			//�e�N�X�`���ǂݍ���
			for (int nCntTex = 0; nCntTex < (int)m_dwNumMatModel; nCntTex++)
			{
				m_pIdxtexture[nCntTex] = NULL;
				if (pMat[nCntTex].pTextureFilename != NULL)
				{//�e�N�X�`�������
				 //�e�N�X�`���ǂݍ���
					m_pIdxtexture[nCntTex] = pTexture->Regist(pMat[nCntTex].pTextureFilename);
				}
				else
				{//�Ȃ���
					m_pIdxtexture[nCntTex] = -1;	//�e�N�X�`���擾����NULL�ɂȂ�悤�ɂ���
				}
			}
		}
		else
		{//��������
			assert(false);
		}
	}

	//�ʒu�����ݒ�
	m_pos = posOffset;
	m_rot = rotOffset;

	return S_OK;
}

//========================
//�I������
//========================
void CModel::Uninit(void)
{
	//���b�V���̔j��
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	//�}�e���A���̔j��
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	//�e�N�X�`���ԍ��j��
	if (m_pIdxtexture != NULL)
	{
		delete[] m_pIdxtexture;
		m_pIdxtexture = NULL;
	}
}

//========================
//�X�V����
//========================
void CModel::Update(void)
{
	//�������񉼂̒l������
	m_posMotioned = m_pos;
	m_rotMotioned = m_rot;
}

//========================
//�`�揈��
//========================
void CModel::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//�f�o�C�X�擾
	CTexture* pTexture = CManager::GetTexture();						//�e�N�X�`���I�u�W�F�N�g�擾
	D3DXMATRIX mtxRot, mtxTrans, mtxTexture;							//�v�Z�p
	D3DXMATRIX mtxParent;												//�e�̃}�g��
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
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rotMotioned.y, m_rotMotioned.x, m_rotMotioned.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu���f
	D3DXMatrixTranslation(&mtxTrans, m_posMotioned.x, m_posMotioned.y, m_posMotioned.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//�p�[�c�̐e�}�g���ݒ�
	if (m_pParent != NULL)
	{
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	//�p�[�c�̃}�g���Ɛe�}�g�����������킹��
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	//���[���h�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//�}�e���A���f�[�^�ւ̃|�C���^�擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMatModel; nCntMat++)
	{
		//�}�e���A���ύX
		D3DMATERIAL9 changeMat = pMat[nCntMat].MatD3D;

		//���C���F�ύX
		changeMat.Diffuse = m_changeMainColor;

		//�T�u�F�ύX
		changeMat.Diffuse.r = 1.0f * m_changeSubColor.r + changeMat.Diffuse.r * (1.0f - m_changeSubColor.r);
		changeMat.Diffuse.g = 1.0f * m_changeSubColor.g + changeMat.Diffuse.g * (1.0f - m_changeSubColor.g);
		changeMat.Diffuse.b = 1.0f * m_changeSubColor.b + changeMat.Diffuse.b * (1.0f - m_changeSubColor.b);

		//�}�e���A���ݒ�
		pDevice->SetMaterial(&changeMat);

		//�e�N�X�`���ݒ�
		pDevice->SetTexture(0, pTexture->GetAddress(m_pIdxtexture[nCntMat]));

		//���f���`��
		m_pMesh->DrawSubset(nCntMat);
	}

	//�}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//========================
//��������
//========================
CModel* CModel::Create(const char * pPath, const D3DXVECTOR3 posOffset, const D3DXVECTOR3 rotOffset)
{
	CModel* pModel = NULL;

	if (pModel == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pModel = new CModel();

		//������
		pModel->Init(pPath, posOffset, rotOffset);

		return pModel;
	}
	else
	{
		return NULL;
	}
}