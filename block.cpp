//======================================================
//
//�u���b�N����[block.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "block.h"

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CBlock::CBlock(int nPriority) : CObject2D(nPriority)
{
	//�l�N���A
	m_nIdxTexture = -1;
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h �ʒu�������j
//=================================
CBlock::CBlock(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, int nPriority) :CObject2D(pos, VEC3_ZERO, fWidth, fHeight, nPriority)
{
	//�l�N���A
	m_nIdxTexture = -1;
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
	//�e�N�X�`���ǂݍ���
	CTexture* pTexture = CManager::GetTexture();
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\Block_R_01.png");

	SetType(TYPE_BLOCK);	//�^�C�v�ݒ�

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
		pBlock->BindTexture(pBlock->m_nIdxTexture);

		return pBlock;
	}
	else
	{
		return NULL;
	}
}