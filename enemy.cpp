//======================================================
//
//�G����[enemy.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "enemy.h"
#include "input.h"
#include "bullet.h"
#include "Culc.h"

//�}�N��
#define PLAYER_SPEED	(5.0f)	//���̈ړ����x

//�ÓI�����o�ϐ�
LPDIRECT3DTEXTURE9 CEnemy::m_pTexture = NULL;

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CEnemy::CEnemy()
{
	m_move = VEC3_ZERO;
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h �ʒu����and�p�^�[���������j
//=================================
CEnemy::CEnemy(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight,
	const int nPatWidth, const int nPatHeight, const int nAnimSpeed, const int nLife) : CObjectAnim2D(pos, rot, fWidth, fHeight, nPatWidth, nPatHeight, nAnimSpeed, true)
{
	m_nLife = nLife;
}

//=================================
//�f�X�g���N�^
//=================================
CEnemy::~CEnemy()
{
}

//=================================
//������
//=================================
HRESULT CEnemy::Init(void)
{
	CObjectAnim2D::Init();

	//��ސݒ�
	SetType(TYPE_ENEMY);

	return S_OK;
}

//=================================
//�I��
//=================================
void CEnemy::Uninit(void)
{
	CObjectAnim2D::Uninit();
}

//=================================
//�X�V
//=================================
void CEnemy::Update(void)
{
	//�A�j���[�V����
	CObjectAnim2D::Update();
}

//=================================
//�`��
//=================================
void CEnemy::Draw(void)
{
	CObjectAnim2D::Draw();
}

//=================================
//��������
//=================================
CEnemy* CEnemy::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight,
	const int nPatWidth, const int nPatHeight, const int nAnimSpeed, const int nLife)
{
	CEnemy* pEnemy = NULL;

	if (pEnemy == NULL)
	{
		//�I�u�W�F�N�g�A�j���[�V����2D�̐���
		pEnemy = new CEnemy(pos, rot, fWidth, fHeight, nPatWidth, nPatHeight, nAnimSpeed, nLife);

		//������
		pEnemy->Init();

		//�e�N�X�`�����蓖��
		pEnemy->BindTexture(m_pTexture);

		return pEnemy;
	}
	else
	{
		return NULL;
	}
}

//=================================
//�e�N�X�`���ǂݍ��ݏ���
//=================================
HRESULT CEnemy::Load(const char* pPath)
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
void CEnemy::Unload(void)
{
	//�e�N�X�`���j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=================================
//�_���[�W�ݒ肨��ю��S���菈��
//=================================
void CEnemy::AddDamage(int nDamage)
{
	m_nLife -= nDamage;	//�_���[�W�t�^

	//���S����
	if (m_nLife <= DEATH_LIFE)
	{
		Uninit();	//�I������
	}
}