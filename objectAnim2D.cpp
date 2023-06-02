//======================================================
//
//�A�j���[�V��������I�u�W�F�N�g�i2D�j����[objectAnim2D.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "objectAnim2D.h"
#include "input.h"
#include <assert.h>

//�ÓI�����o�ϐ�
LPDIRECT3DTEXTURE9 CObjectAnim2D::m_pTexture = NULL;

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CObjectAnim2D::CObjectAnim2D() : m_nPatternWidth(1) , m_nPatternHeight(1)
{
	//�l�N���A
	m_nCounterAnim = INT_ZERO;
	m_nPatternAnim = INT_ZERO;
	m_nAnimSpeed = INT_ZERO;
	m_bLoop = true;
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h �ʒu����and�p�^�[���������j
//=================================
CObjectAnim2D::CObjectAnim2D(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,
	const float fWidth, const float fHeight, const int nPatWidth, const int nPatHeight, const int nAnimSpeed, const bool bLoop)
	: CObject2D(pos,rot, fWidth, fHeight), m_nPatternWidth(nPatWidth), m_nPatternHeight(nPatHeight)
{
	//�l�N���A
	m_nCounterAnim = INT_ZERO;
	m_nPatternAnim = INT_ZERO;

	//�l�ݒ�
	m_nAnimSpeed = nAnimSpeed;
	m_bLoop = bLoop;
}

//=================================
//�f�X�g���N�^
//=================================
CObjectAnim2D::~CObjectAnim2D()
{
}

//=================================
//������
//=================================
HRESULT CObjectAnim2D::Init(void)
{
	//�e�N���X����
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}
	//�l������
	m_nCounterAnim = INT_ZERO;
	m_nPatternAnim = INT_ZERO;

	//�e�N�X�`���ݒ�
	D3DXVECTOR2 tex0, tex3;
	tex0 = D3DXVECTOR2((float)(m_nPatternAnim % m_nPatternWidth) / m_nPatternWidth,
		(float)(m_nPatternAnim / m_nPatternWidth) / m_nPatternHeight);
	tex3 = D3DXVECTOR2((float)(m_nPatternAnim % m_nPatternWidth + 1) / m_nPatternWidth,
		(float)(m_nPatternAnim / m_nPatternWidth + 1) / m_nPatternHeight);

	if (FAILED(SetTex(tex0, tex3)))
	{
		return E_FAIL;
	}

	return S_OK;
}
//=================================
//�I��
//=================================
void CObjectAnim2D::Uninit(void)
{
	//�e�N���X����
	CObject2D::Uninit();
}

//=================================
//�X�V
//=================================
void CObjectAnim2D::Update(void)
{
	//�e�N���X����
	CObject2D::Update();
	
	m_nCounterAnim++;

	if ((m_nCounterAnim % m_nAnimSpeed) == 0)
	{
		m_nCounterAnim = 0;	//�J�E���^�����l�ɖ߂�

		//�p�^�[��No�X�V
		if (m_bLoop == true)
		{//���[�v����ݒ�̏ꍇ
			m_nPatternAnim = (m_nPatternAnim + 1) % (m_nPatternHeight * m_nPatternWidth);
		}
		else
		{//���[�v���Ȃ��ݒ�̏ꍇ
			m_nPatternAnim++;
			if (m_nPatternAnim >= (m_nPatternHeight * m_nPatternWidth))
			{//�A�j���[�V�����I��
				Uninit();
				return;
			}
		}

		//�e�N�X�`���ݒ�
		D3DXVECTOR2 tex0, tex3;
		tex0 = D3DXVECTOR2((float)(m_nPatternAnim % m_nPatternWidth) / m_nPatternWidth,
			(float)(m_nPatternAnim / m_nPatternWidth) / m_nPatternHeight);
		tex3 = D3DXVECTOR2((float)(m_nPatternAnim % m_nPatternWidth + 1) / m_nPatternWidth,
			(float)(m_nPatternAnim / m_nPatternWidth + 1) / m_nPatternHeight);

		if (FAILED(SetTex(tex0, tex3)))
		{
			assert(false);
		}
	}
}

//=================================
//�`��
//=================================
void CObjectAnim2D::Draw(void)
{
	//�e�N���X����
	CObject2D::Draw();
}

//=================================
//��������
//=================================
CObjectAnim2D* CObjectAnim2D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight,
	const int nPatWidth, const int nPatHeight, const int nAnimSpeed, const bool bLoop)
{
	CObjectAnim2D* pObjAnim2D = NULL;

	if (pObjAnim2D == NULL)
	{
		//�I�u�W�F�N�g�A�j���[�V����2D�̐���
		pObjAnim2D = new CObjectAnim2D(pos, rot, fWidth, fHeight, nPatWidth, nPatHeight, nAnimSpeed, bLoop);

		//������
		pObjAnim2D->Init();

		//�e�N�X�`�����蓖��
		pObjAnim2D->BindTexture(m_pTexture);

		return pObjAnim2D;
	}
	else
	{
		return NULL;
	}
}

//=================================
//�e�N�X�`���ǂݍ��ݏ���
//=================================
HRESULT CObjectAnim2D::Load(const char* pPath)
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
void CObjectAnim2D::Unload(void)
{
	//�e�N�X�`���j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}