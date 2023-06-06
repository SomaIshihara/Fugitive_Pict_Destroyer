//======================================================
//
//�u���b�N����[block.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "block.h"

//�ÓI�����o�ϐ�
LPDIRECT3DTEXTURE9 CBlock::m_pTexture = NULL;

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CBlock::CBlock(int nPriority) : CObject2D(nPriority)
{
	SetType(TYPE_BLOCK);	//�^�C�v�ݒ�
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h �ʒu�������j
//=================================
CBlock::CBlock(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, int nPriority) :CObject2D(pos, VEC3_ZERO, fWidth, fHeight, nPriority)
{
	SetType(TYPE_BLOCK);	//�^�C�v�ݒ�
}

//=================================
//�f�X�g���N�^
//=================================
CBlock::~CBlock()
{
}

//=================================
//������
//=================================
HRESULT CBlock::Init(void)
{
	return CObject2D::Init();
}

//=================================
//�I��
//=================================
void CBlock::Uninit(void)
{
	CObject2D::Uninit();
}

//=================================
//�X�V
//=================================
void CBlock::Update(void)
{
	CObject2D::Update();
}

//=================================
//�`��
//=================================
void CBlock::Draw(void)
{
	CObject2D::Draw();
}

//=================================
//��������
//=================================
CBlock* CBlock::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
	CBlock* pBlock = NULL;

	if (pBlock == NULL)
	{
		//�I�u�W�F�N�g�A�j���[�V����2D�̐���
		pBlock = new CBlock(pos, fWidth, fHeight);

		//������
		pBlock->Init();

		//�e�N�X�`���ݒ�
		pBlock->BindTexture(m_pTexture);

		return pBlock;
	}
	else
	{
		return NULL;
	}
}

//=================================
//�e�N�X�`���ǂݍ��ݏ���
//=================================
HRESULT CBlock::Load(const char* pPath)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//�f�o�C�X�擾

	//�e�N�X�`���ǂݍ���
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		pPath,
		&m_pTexture)))
	{//���s
		return E_FAIL;
	}

	//����
	return S_OK;
}

//=================================
//�e�N�X�`���j������
//=================================
void CBlock::Unload(void)
{
	//�e�N�X�`���j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}