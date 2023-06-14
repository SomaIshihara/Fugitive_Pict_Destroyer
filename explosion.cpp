//======================================================
//
//��������[explosion.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "explosion.h"
#include "input.h"
#include <assert.h>

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CExplosion::CExplosion()
{
	m_nIdxTexture = -1;
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h �ʒu����and�p�^�[���������j
//=================================
CExplosion::CExplosion(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const int nAnimSpeed) 
	: CObjectAnim2D(pos, rot, fWidth, fHeight, nAnimSpeed, false)
{
	m_nIdxTexture = -1;
}

//=================================
//�f�X�g���N�^
//=================================
CExplosion::~CExplosion()
{
}

//=================================
//������
//=================================
HRESULT CExplosion::Init(void)
{
	//�e�N�X�`���ǂݍ���
	CTexture* pTexture = CManager::GetTexture();
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\bomb0.png", 8, 2);

	//�e�N���X����
	if (FAILED(CObjectAnim2D::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}
//=================================
//�I��
//=================================
void CExplosion::Uninit(void)
{
	//�e�N���X����
	CObjectAnim2D::Uninit();
}

//=================================
//�X�V
//=================================
void CExplosion::Update(void)
{
	//�e�N���X����
	CObjectAnim2D::Update();
}

//=================================
//�`��
//=================================
void CExplosion::Draw(void)
{
	//�e�N���X����
	CObjectAnim2D::Draw();
}

//=================================
//��������
//=================================
CExplosion* CExplosion::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const int nAnimSpeed)
{
	CExplosion* pExplosion = NULL;

	if (pExplosion == NULL)
	{
		//�I�u�W�F�N�g�A�j���[�V����2D�̐���
		pExplosion = new CExplosion(pos, rot, fWidth, fHeight, nAnimSpeed);

		//������
		pExplosion->Init();

		//�e�N�X�`�����蓖��
		pExplosion->BindTexture(pExplosion->m_nIdxTexture);

		return pExplosion;
	}
	else
	{
		return NULL;
	}
}