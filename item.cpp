//======================================================
//
//�A�C�e������[item.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "item.h"

//�ÓI�����o�ϐ�
LPDIRECT3DTEXTURE9 CItem::m_pTexture = NULL;

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CItem::CItem(int nPriority) : CObject2D(nPriority)
{
	SetType(TYPE_ITEM);	//�^�C�v�ݒ�
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h �ʒu�������j
//=================================
CItem::CItem(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, int nPriority) :CObject2D(pos, VEC3_ZERO, fWidth, fHeight, nPriority)
{
	SetType(TYPE_ITEM);	//�^�C�v�ݒ�
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
		pItem->BindTexture(m_pTexture);

		return pItem;
	}
	else
	{
		return NULL;
	}
}

//=================================
//�e�N�X�`���ǂݍ��ݏ���
//=================================
HRESULT CItem::Load(const char* pPath)
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
void CItem::Unload(void)
{
	//�e�N�X�`���j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}