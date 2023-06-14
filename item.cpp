//======================================================
//
//�A�C�e������[item.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "item.h"

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CItem::CItem(int nPriority) : CObject2D(nPriority)
{
	//�l�N���A
	m_nIdxTexture = -1;
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h �ʒu�������j
//=================================
CItem::CItem(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, int nPriority) :CObject2D(pos, VEC3_ZERO, fWidth, fHeight, nPriority)
{
	//�l�N���A
	m_nIdxTexture = -1;
}

//=================================
//�f�X�g���N�^
//=================================
CItem::~CItem()
{
}

//=================================
//������
//=================================
HRESULT CItem::Init(void)
{
	//�e�N�X�`���ǂݍ���
	CTexture* pTexture = CManager::GetTexture();
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\Item_05.png");

	SetType(TYPE_ITEM);	//�^�C�v�ݒ�

	return CObject2D::Init();
}

//=================================
//�I��
//=================================
void CItem::Uninit(void)
{
	CObject2D::Uninit();
}

//=================================
//�X�V
//=================================
void CItem::Update(void)
{
	CObject2D::Update();
}

//=================================
//�`��
//=================================
void CItem::Draw(void)
{
	CObject2D::Draw();
}

//=================================
//��������
//=================================
CItem* CItem::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
	CItem* pItem = NULL;

	if (pItem == NULL)
	{
		//�I�u�W�F�N�g�A�j���[�V����2D�̐���
		pItem = new CItem(pos, fWidth, fHeight);

		//������
		pItem->Init();

		//�e�N�X�`���ݒ�
		pItem->BindTexture(pItem->m_nIdxTexture);

		return pItem;
	}
	else
	{
		return NULL;
	}
}