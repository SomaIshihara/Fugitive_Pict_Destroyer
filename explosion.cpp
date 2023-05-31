//======================================================
//
//��������[explosion.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "explosion.h"
#include "input.h"
#include <assert.h>

//�ÓI�����o�ϐ�
LPDIRECT3DTEXTURE9 CExplosion::m_pTexture = NULL;

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CExplosion::CExplosion()
{

}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h �ʒu����and�p�^�[���������j
//=================================
CExplosion::CExplosion(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,
	const float fWidth, const float fHeight, const int nPatWidth, const int nPatHeight, const int nAnimSpeed)
	: CObjectAnim2D(pos, rot, fWidth, fHeight, nPatWidth, nPatHeight, nAnimSpeed, false)
{
	
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
CExplosion* CExplosion::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight,
	const int nPatWidth, const int nPatHeight, const int nAnimSpeed)
{
	CExplosion* pObjAnim2D = NULL;

	if (pObjAnim2D == NULL)
	{
		//�I�u�W�F�N�g�A�j���[�V����2D�̐���
		pObjAnim2D = new CExplosion(pos, rot, fWidth, fHeight, nPatWidth, nPatHeight, nAnimSpeed);

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
HRESULT CExplosion::Load(const char* pPath)
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
void CExplosion::Unload(void)
{
	//�e�N�X�`���j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}