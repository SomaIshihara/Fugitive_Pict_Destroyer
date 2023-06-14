//======================================================
//
//�e�N�X�`���ۑ��N���X[texture.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//�ÓI�����o�ϐ�
int CTexture::m_nNumAll = 0;

//=================================
//�R���X�g���N�^
//=================================
CTexture::CTexture()
{
	for (int cnt = 0; cnt < MAX_TEXTURE; cnt++)
	{//�S������ۂɂ���
		m_apTexture[cnt].pTexture = NULL;
		m_apTexture[cnt].nPatternWidth = 0;
		m_apTexture[cnt].nPatternHeight = 0;
	}
}

//=================================
//�f�X�g���N�^
//=================================
CTexture::~CTexture()
{
}

//=================================
//���炩���ߓǂݍ���
//=================================
HRESULT CTexture::Load(void)
{
	return S_OK;
}

//=================================
//�S���j��
//=================================
void CTexture::Unload(void)
{
	for (int cnt = 0; cnt < m_nNumAll; cnt++)
	{//�S���j��
		if (m_apTexture[cnt].pTexture != NULL)
		{
			m_apTexture[cnt].pTexture->Release();
			m_apTexture[cnt].pTexture = NULL;
		}
	}
	m_nNumAll = 0;	//�����Z�b�g
}

//=================================
//�ǉ����o�^
//=================================
int CTexture::Regist(const char * pPath, const int nPatWidth, const int nPatHeight)
{
	//�d���`�F�b�N
	for (int cnt = 0; cnt < m_nNumAll; cnt++)
	{//���镪������
		if (strcmp(&m_aFilepath[cnt][0], pPath) == 0)
		{//��v����
			return cnt;
		}
	}

	//�V�K�o�^
	if (m_nNumAll < MAX_TEXTURE)
	{//�e�N�X�`���ۑ����E�ȓ�
		 //�f�o�C�X�擾
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		//�e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(pDevice, pPath,
			&m_apTexture[m_nNumAll].pTexture);

		//�t�@�C���p�X�R�s�[
		strcpy(&m_aFilepath[m_nNumAll][0], pPath);

		//�p�^�[�����������
		m_apTexture[m_nNumAll].nPatternWidth = nPatWidth;
		m_apTexture[m_nNumAll].nPatternHeight = nPatHeight;

		//�������₷
		m_nNumAll++;

		return m_nNumAll - 1;
	}
	else
	{//��������Ȃ�
		return -1;
	}
}

//=================================
//�e�N�X�`���擾
//=================================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	if (m_nNumAll > nIdx && nIdx >= 0)
	{//�e�N�X�`�������ȓ�
		return m_apTexture[nIdx].pTexture;	//�e�N�X�`���|�C���^�Ԃ�
	}
	else
	{//����Ȃ��̂͂Ȃ�
		return NULL;	//�Ԃ����̂Ȃ�ĂȂ�
	}
}

//=================================
//�e�N�X�`���p�^�[�����擾
//=================================
int CTexture::GetPatWidth(int nIdx)
{
	if (m_nNumAll > nIdx && nIdx >= 0)
	{//�e�N�X�`�������ȓ�
		return m_apTexture[nIdx].nPatternWidth;	//�e�N�X�`���|�C���^�Ԃ�
	}
	else
	{//����Ȃ��̂͂Ȃ�
		return 0;	//�Ԃ����̂Ȃ�ĂȂ�
	}
}

//=================================
//�e�N�X�`���p�^�[�������擾
//=================================
int CTexture::GetPatHeight(int nIdx)
{
	if (m_nNumAll > nIdx && nIdx >= 0)
	{//�e�N�X�`�������ȓ�
		return m_apTexture[nIdx].nPatternHeight;	//�e�N�X�`���|�C���^�Ԃ�
	}
	else
	{//����Ȃ��̂͂Ȃ�
		return 0;	//�Ԃ����̂Ȃ�ĂȂ�
	}
}

