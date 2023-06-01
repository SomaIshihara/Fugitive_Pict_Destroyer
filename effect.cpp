//======================================================
//
//�G�t�F�N�g����[effect.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "effect.h"
#include "manager.h"
#include "renderer.h"

//�ÓI�����o�ϐ�
LPDIRECT3DTEXTURE9 CEffect::m_pTexture = NULL;

//=================================
//�R���X�g���N�^
//=================================
CEffect::CEffect() : CObject2D(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT)
{
}

//=================================
//�f�X�g���N�^
//=================================
CEffect::~CEffect()
{
}

//=================================
//������
//=================================
HRESULT CEffect::Init(void)
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
void CEffect::Uninit(void)
{
	//�e�N���X����
	CObject2D::Uninit();
}

//=================================
//�X�V
//=================================
void CEffect::Update(void)
{
	//�e�N���X����
	CObject2D::Update();
}

//=================================
//�`��
//=================================
void CEffect::Draw(void)
{
	//�e�N���X����
	CObject2D::Draw();
}

//=================================
//��������
//=================================
CEffect* CEffect::Create(void)
{
	CEffect* pObjBG = NULL;

	if (pObjBG == NULL)
	{
		//�w�i�̐���
		pObjBG = new CEffect();

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
HRESULT CEffect::Load(const char* pPath)
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
void CEffect::Unload(void)
{
	//�e�N�X�`���j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}