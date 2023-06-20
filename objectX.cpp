//======================================================
//
//�I�u�W�F�N�g�iX�j����[object3D.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "objectX.h"
#include <assert.h>

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

	//�T�C�Y�ݒ�
	D3DXVECTOR3 vtxMin, vtxMax;
	m_aModel[m_nIdx].m_collision.GetVtx(&vtxMin, &vtxMax);
	m_fWidth = vtxMax.x - vtxMin.x;
	m_fHeight = vtxMax.y - vtxMin.y;
	m_fDepth = vtxMax.z - vtxMin.z;
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
	SetType(TYPE_BUILDING);	//�ꎞ�I�Ɍ����Ƃ���i�����蔻��`�F�b�N�̂��߁j
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
	CTexture* pTexture = CManager::GetTexture();						//�e�N�X�`���I�u�W�F�N�g�擾
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
		pDevice->SetTexture(0, pTexture->GetAddress(m_aModel[m_nIdx].m_pIdxtexture[nCntMat]));

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
	CObjectX* pObjX = NULL;

	if (pObjX == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pObjX = new CObjectX(pos, rot, nIdx);

		//������
		pObjX->Init();

		return pObjX;
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
	CTexture* pTexture = CManager::GetTexture();						//�e�N�X�`���I�u�W�F�N�g�擾
	m_aModel[nIdx].m_pIdxtexture = NULL;	//�e�N�X�`���ԍ��|�C���^��NULL�ɂ���

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
		//�e�N�X�`���|�C���^�m��
		if (m_aModel[nIdx].m_pIdxtexture == NULL)
		{//NULL
			//�e�N�X�`���ԍ��z��m��
			m_aModel[nIdx].m_pIdxtexture = new int[(int)m_aModel[nIdx].m_dwNumMatModel];

			//�����蔻�萶��
			int nNumVtx;		//���_��
			DWORD dwSizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y
			BYTE *pVtxBuff;		//���_�o�b�t�@�|�C���^

			//���_�����擾
			nNumVtx = m_aModel[nIdx].m_pMesh->GetNumVertices();

			//���_�t�H�[�}�b�g
			dwSizeFVF = D3DXGetFVFVertexSize(m_aModel[nIdx].m_pMesh->GetFVF());

			//���_�o�b�t�@���b�N
			m_aModel[nIdx].m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void **)&pVtxBuff);

			//�ŏ������S�������
			D3DXVECTOR3 vtx = *(D3DXVECTOR3 *)pVtxBuff;

			D3DXVECTOR3 vtxMax = vtx;
			D3DXVECTOR3 vtxMin = vtx;

			pVtxBuff += dwSizeFVF;

			for (int nCntVtx = 1; nCntVtx < nNumVtx; nCntVtx++, pVtxBuff += dwSizeFVF)
			{
				D3DXVECTOR3 vtx = *(D3DXVECTOR3 *)pVtxBuff;

				if (vtxMax.x < vtx.x)
				{
					vtxMax.x = vtx.x;
				}
				if (vtxMax.y < vtx.y)
				{
					vtxMax.y = vtx.y;
				}
				if (vtxMax.z < vtx.z)
				{
					vtxMax.z = vtx.z;
				}
				if (vtxMin.x > vtx.x)
				{
					vtxMin.x = vtx.x;
				}
				if (vtxMin.y > vtx.y)
				{
					vtxMin.y = vtx.y;
				}
				if (vtxMin.z > vtx.z)
				{
					vtxMin.z = vtx.z;
				}
			}

			//�ݒ�
			m_aModel[nIdx].m_collision.SetVtx(vtxMin, vtxMax);

			//���_�o�b�t�@�A�����b�N
			m_aModel[nIdx].m_pMesh->UnlockVertexBuffer();

			//�e�N�X�`���ǂݍ���
			D3DXMATERIAL* pMat;	//�}�e���A���|�C���^

			//�}�e���A�����ɑ΂���|�C���^�擾
			pMat = (D3DXMATERIAL*)m_aModel[nIdx].m_pBuffMat->GetBufferPointer();

			//�e�N�X�`���ǂݍ���
			for (int nCntTex = 0; nCntTex < (int)m_aModel[nIdx].m_dwNumMatModel; nCntTex++)
			{
				m_aModel[nIdx].m_pIdxtexture[nCntTex] = NULL;
				if (pMat[nCntTex].pTextureFilename != NULL)
				{//�e�N�X�`�������
					//�e�N�X�`���ǂݍ���
					m_aModel[nIdx].m_pIdxtexture[nCntTex] = pTexture->Regist(pMat[nCntTex].pTextureFilename);
				}
				else
				{//�Ȃ���
					m_aModel[nIdx].m_pIdxtexture[nCntTex] = -1;	//�e�N�X�`���擾����NULL�ɂȂ�悤�ɂ���
				}
			}
		}
		else
		{//��������
			assert(false);
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

		//�e�N�X�`���ԍ��j��
		if (m_aModel[cntModel].m_pIdxtexture != NULL)
		{
			delete[] m_aModel[cntModel].m_pIdxtexture;
			m_aModel[cntModel].m_pIdxtexture = NULL;
		}
	}
}