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
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CEffect::CEffect() : m_nDefLife(0)
{
	//�l�N���A
	m_effect.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_effect.nLife = 0;
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h�j
//=================================
CEffect::CEffect(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, 
	const D3DXCOLOR col, const int nLife) : CObject2D(pos, rot, fWidth, fHeight), m_nDefLife(nLife)
{
	//�l�ݒ�
	m_effect.col = col;
	m_effect.nLife = nLife;
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

	//�����Ǘ�
	m_effect.nLife--;	//���炷

	//�s�����x�ݒ�
	m_effect.col.a = (float)m_effect.nLife / m_nDefLife;
	SetCol(m_effect.col);

	if (m_effect.nLife <= 0)
	{//����
		Uninit();	//�I��
	}
}

//=================================
//�`��
//=================================
void CEffect::Draw(void)
{
	//�u�����f�B���O��ނ����Z�����ɕύX
	CManager::GetRenderer()->SetBlendType(CRenderer::BLENDTYPE_ADD);

	//�e�N���X����
	CObject2D::Draw();

	//�u�����f�B���O��ނ�ʏ��Ԃɖ߂�
	CManager::GetRenderer()->SetBlendType(CRenderer::BLENDTYPE_NORMAL);
}

//=================================
//��������
//=================================
CEffect* CEffect::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight,
	const D3DXCOLOR col, const int nLife)
{
	CEffect* pObjEffect = NULL;

	if (pObjEffect == NULL)
	{
		//�w�i�̐���
		pObjEffect = new CEffect(pos, rot, fWidth, fHeight, col, nLife);

		//������
		pObjEffect->Init();

		//�e�N�X�`���ݒ�
		pObjEffect->BindTexture(m_pTexture);

		return pObjEffect;
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