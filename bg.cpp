//======================================================
//
//�w�i�i2D�j����[bg.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "bg.h"
#include "manager.h"
#include "renderer.h"

//�ÓI�����o�ϐ�
LPDIRECT3DTEXTURE9 CBG::m_pTexture = NULL;

//=================================
//�R���X�g���N�^
//=================================
CBG::CBG() : CObject2D(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT)
{
}

//=================================
//�f�X�g���N�^
//=================================
CBG::~CBG()
{
}

//=================================
//������
//=================================
HRESULT CBG::Init(void)
{
	//�e�N���X����
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=================================
//�I��
//=================================
void CBG::Uninit(void)
{
	//�e�N���X����
	CObject2D::Uninit();
}

//=================================
//�X�V
//=================================
void CBG::Update(void)
{
	//�e�N���X����
	CObject2D::Update();
}

//=================================
//�`��
//=================================
void CBG::Draw(void)
{
	//�e�N���X����
	CObject2D::Draw();
}

//=================================
//��������
//=================================
CBG* CBG::Create(void)
{
	CBG* pObjBG = NULL;

	if (pObjBG == NULL)
	{
		//�w�i�̐���
		pObjBG = new CBG();

		//������
		pObjBG->Init();

		//�e�N�X�`���ݒ�
		pObjBG->BindTexture(m_pTexture);

		return pObjBG;
	}
	else
	{
		return NULL;
	}
}

//=================================
//�e�N�X�`���ǂݍ��ݏ���
//=================================
HRESULT CBG::Load(const char* pPath)
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
void CBG::Unload(void)
{
	//�e�N�X�`���j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}