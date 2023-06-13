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
LPDIRECT3DTEXTURE9 CEffect2D::m_pTexture = NULL;
LPDIRECT3DTEXTURE9 CEffectBillboard::m_pTexture = NULL;

//****************************************
//2D�G�t�F�N�g
//****************************************
//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CEffect2D::CEffect2D(int nPriority) : CObject2D(nPriority), m_nDefLife(0)
{
	//�l�N���A
	m_effect.move = VEC3_ZERO;
	m_effect.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_effect.nLife = 0;
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h�j
//=================================
CEffect2D::CEffect2D(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fWidth, const float fHeight,
	const D3DXCOLOR col, const int nLife, int nPriority) : CObject2D(pos, VEC3_ZERO, fWidth, fHeight, nPriority), m_nDefLife(nLife)
{
	//�l�ݒ�
	m_effect.move = move;
	m_effect.col = col;
	m_effect.nLife = nLife;
}

//=================================
//�f�X�g���N�^
//=================================
CEffect2D::~CEffect2D()
{
}

//=================================
//������
//=================================
HRESULT CEffect2D::Init(void)
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
void CEffect2D::Uninit(void)
{
	//�e�N���X����
	CObject2D::Uninit();
}

//=================================
//�X�V
//=================================
void CEffect2D::Update(void)
{
	//�e�N���X����
	CObject2D::Update();

	//�ړ�
	D3DXVECTOR3 pos = GetPos();
	pos += m_effect.move;
	SetPos(pos);

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
void CEffect2D::Draw(void)
{
	//Z�e�X�g������
	CManager::GetRenderer()->SetEnableZTest(false);

	//�A���t�@�e�X�g�L����
	CManager::GetRenderer()->SetEnableAlplaTest(true);

	//�u�����f�B���O��ނ����Z�����ɕύX
	CManager::GetRenderer()->SetBlendType(CRenderer::BLENDTYPE_ADD);

	//�e�N���X����
	CObject2D::Draw();

	//�u�����f�B���O��ނ�ʏ��Ԃɖ߂�
	CManager::GetRenderer()->SetBlendType(CRenderer::BLENDTYPE_NORMAL);

	//�A���t�@�e�X�g������
	CManager::GetRenderer()->SetEnableAlplaTest(false);

	//Z�e�X�g������
	CManager::GetRenderer()->SetEnableZTest(true);
}

//=================================
//��������
//=================================
CEffect2D* CEffect2D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fWidth, const float fHeight,
	const D3DXCOLOR col, const int nLife)
{
	CEffect2D* pObjEffect = NULL;

	if (pObjEffect == NULL)
	{
		//�w�i�̐���
		pObjEffect = new CEffect2D(pos, move, fWidth, fHeight, col, nLife);

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
HRESULT CEffect2D::Load(const char* pPath)
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
void CEffect2D::Unload(void)
{
	//�e�N�X�`���j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//****************************************
//�r���{�[�h�G�t�F�N�g
//****************************************
//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CEffectBillboard::CEffectBillboard(int nPriority) : CObjectBillboard(nPriority), m_nDefLife(0)
{
	//�l�N���A
	m_effect.move = VEC3_ZERO;
	m_effect.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_effect.nLife = 0;
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h�j
//=================================
CEffectBillboard::CEffectBillboard(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fWidth, const float fHeight,
	const D3DXCOLOR col, const int nLife, int nPriority) : CObjectBillboard(pos, VEC3_ZERO, fWidth, fHeight, nPriority), m_nDefLife(nLife)
{
	//�l�ݒ�
	m_effect.move = move;
	m_effect.col = col;
	m_effect.nLife = nLife;
}

//=================================
//�f�X�g���N�^
//=================================
CEffectBillboard::~CEffectBillboard()
{
}

//=================================
//������
//=================================
HRESULT CEffectBillboard::Init(void)
{
	//�e�N���X����
	if (FAILED(CObjectBillboard::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=================================
//�I��
//=================================
void CEffectBillboard::Uninit(void)
{
	//�e�N���X����
	CObjectBillboard::Uninit();
}

//=================================
//�X�V
//=================================
void CEffectBillboard::Update(void)
{
	//�e�N���X����
	CObjectBillboard::Update();

	//�ړ�
	D3DXVECTOR3 pos = GetPos();
	pos += m_effect.move;
	SetPos(pos);

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
void CEffectBillboard::Draw(void)
{
	//Z�e�X�g������
	CManager::GetRenderer()->SetEnableZTest(false);

	//�A���t�@�e�X�g�L����
	CManager::GetRenderer()->SetEnableAlplaTest(true);

	//�u�����f�B���O��ނ����Z�����ɕύX
	CManager::GetRenderer()->SetBlendType(CRenderer::BLENDTYPE_ADD);

	//�e�N���X����
	CObjectBillboard::Draw();

	//�u�����f�B���O��ނ�ʏ��Ԃɖ߂�
	CManager::GetRenderer()->SetBlendType(CRenderer::BLENDTYPE_NORMAL);

	//�A���t�@�e�X�g������
	CManager::GetRenderer()->SetEnableAlplaTest(false);

	//Z�e�X�g������
	CManager::GetRenderer()->SetEnableZTest(true);
}

//=================================
//��������
//=================================
CEffectBillboard* CEffectBillboard::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fWidth, const float fHeight,
	const D3DXCOLOR col, const int nLife)
{
	CEffectBillboard* pObjEffect = NULL;

	if (pObjEffect == NULL)
	{
		//�w�i�̐���
		pObjEffect = new CEffectBillboard(pos, move, fWidth, fHeight, col, nLife);

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
HRESULT CEffectBillboard::Load(const char* pPath)
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
void CEffectBillboard::Unload(void)
{
	//�e�N�X�`���j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}