//======================================================
//
//X�t�@�C�����f���̃w�b�_[xmodel.h]
//Author:�Ό��D�n
//
//======================================================
#include "xmodel.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//�ÓI�����o�ϐ�
CXModel* CXModel::m_pTop = NULL;
CXModel* CXModel::m_pCur = NULL;
int CXModel::m_nNumAll = 0;

//=================================
//�R���X�g���N�^
//=================================
CXModel::CXModel()
{
	if (m_pCur == NULL)
	{//�Ō�������Ȃ��i���Ȃ킿�擪�����Ȃ��j
		m_pTop = this;		//�����擪
		m_pPrev = NULL;		//�O��N�����Ȃ�
		m_pNext = NULL;
	}
	else
	{//�Ō��������
		m_pPrev = m_pCur;		//�Ō���������̑O�̃I�u�W�F
		m_pCur->m_pNext = this;	//�Ō���̎��̃I�u�W�F������
		m_pNext = NULL;			//�����̎��̃I�u�W�F�͂��Ȃ�
	}
	m_pCur = this;				//�����Ō��
	m_bExclusion = false;		//�����Ă�
	m_nNumAll++;
}

//=================================
//�f�X�g���N�^
//=================================
CXModel::~CXModel()
{
}

//=================================
//�ǂݍ���
//=================================
CXModel* CXModel::Load(const char* pPath)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//�f�o�C�X�擾
	CTexture* pTexture = CManager::GetTexture();						//�e�N�X�`���I�u�W�F�N�g�擾

	CXModel* pObject = m_pTop;	//�擪������

	while (pObject != NULL)
	{//���O���m
		CXModel* pObjectNext = pObject->m_pNext;	//���̃I�u�W�F�ۑ�

		if (pObject->m_pFilePath != NULL && strcmp(pObject->m_pFilePath, pPath) == 0)
		{
			return pObject;
		}

		pObject = pObjectNext;	//��������
	}

	pObject = new CXModel;
	pObject->m_pIdxtexture = NULL;	//�e�N�X�`���ԍ��|�C���^��NULL�ɂ���

	if (SUCCEEDED(D3DXLoadMeshFromX(
		pPath,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&pObject->m_pBuffMat,
		NULL,
		&pObject->m_dwNumMatModel,
		&pObject->m_pMesh)))
	{
		//�e�N�X�`���|�C���^�m��
		if (pObject->m_pIdxtexture == NULL)
		{//NULL
		 //�e�N�X�`���ԍ��z��m��
			pObject->m_pIdxtexture = new int[(int)pObject->m_dwNumMatModel];

			//�����蔻�萶��
			int nNumVtx;		//���_��
			DWORD dwSizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y

			//���_�����擾
			nNumVtx = pObject->m_pMesh->GetNumVertices();

			//���_�t�H�[�}�b�g
			dwSizeFVF = D3DXGetFVFVertexSize(pObject->m_pMesh->GetFVF());

			//���_�o�b�t�@���b�N
			BYTE *pVtxBuff;		//���_�o�b�t�@�|�C���^
			pObject->m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void **)&pVtxBuff);

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
			pObject->m_collision.SetVtx(vtxMin, vtxMax);

			//���_�o�b�t�@�A�����b�N
			pObject->m_pMesh->UnlockVertexBuffer();

			//�e�N�X�`���ǂݍ���
			D3DXMATERIAL* pMat;	//�}�e���A���|�C���^

			//�}�e���A�����ɑ΂���|�C���^�擾
			pMat = (D3DXMATERIAL*)pObject->m_pBuffMat->GetBufferPointer();

			//�e�N�X�`���ǂݍ���
			for (int nCntTex = 0; nCntTex < (int)pObject->m_dwNumMatModel; nCntTex++)
			{
				pObject->m_pIdxtexture[nCntTex] = NULL;
				if (pMat[nCntTex].pTextureFilename != NULL)
				{//�e�N�X�`�������
				 //�e�N�X�`���ǂݍ���
					pObject->m_pIdxtexture[nCntTex] = pTexture->Regist(pMat[nCntTex].pTextureFilename);
				}
				else
				{//�Ȃ���
					pObject->m_pIdxtexture[nCntTex] = -1;	//�e�N�X�`���擾����NULL�ɂȂ�悤�ɂ���
				}
			}
		}
		else
		{//��������
			assert(false);
		}

		//�t�@�C���p�X�ۑ�
		pObject->m_pFilePath = new char[strlen(pPath)+1];
		strcpy(pObject->m_pFilePath, pPath);

		//�ԍ��Ԃ�
		return pObject;
	}
	else
	{
		delete pObject;
		m_nNumAll--;
		return NULL;
	}

	return NULL;
}

//=================================
//�j��
//=================================
void CXModel::Unload(void)
{
	if (m_pPrev != NULL)
	{//�O�ɃI�u�W�F������
		m_pPrev->m_pNext = m_pNext;	//�O�̃I�u�W�F�̎��̃I�u�W�F�͎����̎��̃I�u�W�F
	}
	if (m_pNext != NULL)
	{
		m_pNext->m_pPrev = m_pPrev;	//���̃I�u�W�F�̑O�̃I�u�W�F�͎����̑O�̃I�u�W�F
	}

	if (m_pCur == this)
	{//�Ō���ł���
		m_pCur = m_pPrev;	//�Ō���������̑O�̃I�u�W�F�ɂ���
	}
	if (m_pTop == this)
	{
		m_pTop = m_pNext;	//�擪�������̎��̃I�u�W�F�ɂ���
	}

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

	//�t�@�C���p�X�j��
	if (m_pFilePath != NULL)
	{
		delete[] m_pFilePath;
		m_pFilePath = NULL;
	}

	//����	
	delete this;	//�������g�j��
	m_nNumAll--;	//�������炷
}

//=================================
//�S�j��
//=================================
void CXModel::UnloadAll(void)
{
	CXModel* pObject = m_pTop;	//�擪������

	while (pObject != NULL)
	{//�Ō���܂ŉ񂵑�����
		CXModel* pObjectNext = pObject->m_pNext;	//���̃I�u�W�F�ۑ�

		pObject->Unload();	//�����

		pObject = pObjectNext;	//��������
	}
}

//=================================
//���X�g�ǂݍ���
//=================================
CXModel::LOADRESULT CXModel::LoadList(const char * pPath)
{
	FILE* pFile;

	pFile = fopen(pPath, "rb");

	if (pFile != NULL)
	{//�J����
		int nNumAll = 0;
		fread(&nNumAll, sizeof(int), 1, pFile);	//����������

		char aPath[256];

		for (int cnt = 0; cnt < nNumAll; cnt++)
		{
			fread(&aPath[0], sizeof(char), 256, pFile);	//�t�@�C���p�X�ǂݍ���
			CXModel::Load(&aPath[0]);
		}

		fclose(pFile);
		return RES_OK;
	}
	else
	{//�J���Ȃ������i�t�@�C���Ȃ��񂶂�ˁH�j
		return RES_ERR_LIST_NOTFOUND;
	}
}

//=================================
//���X�g�����o��
//=================================
CXModel::LOADRESULT CXModel::SaveList(const char * pPath)
{
	FILE* pFile;

	pFile = fopen(pPath, "wb");

	if (pFile != NULL)
	{//���ʂɊJ����
		int nNumAll = CXModel::GetNumAll();
		fwrite(&nNumAll, sizeof(int), 1, pFile);	//����������

		CXModel* pObject = m_pTop;	//���X�g�������ݑ̐�
		while (pObject != NULL)
		{
			CXModel* pObjectNext = pObject->m_pNext;

			fwrite(pObject->m_pFilePath, sizeof(char), 256, pFile);

			pObject = pObjectNext;
		}

		fclose(pFile);
		return RES_OK;
	}
	else
	{//�Ȃ����J���Ȃ������i�Ȃ�ŁH�j
		return RES_ERR_LIST_NOTFOUND;
	}
}