//======================================================
//
//�v���C���[����[player.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "Culc.h"

//�}�N��
#define PLAYER_SPEED	(5.0f)	//���̈ړ����x

//�ÓI�����o�ϐ�
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CPlayer::CPlayer()
{
	m_move = VEC3_ZERO;
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h �ʒu����and�p�^�[���������j
//=================================
CPlayer::CPlayer(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight,
	const int nPatWidth, const int nPatHeight, const int nAnimSpeed) : CObjectAnim2D(pos, rot, fWidth, fHeight, nPatWidth, nPatHeight, nAnimSpeed, true)
{

}

//=================================
//�f�X�g���N�^
//=================================
CPlayer::~CPlayer()
{
}

//=================================
//������
//=================================
HRESULT CPlayer::Init(void)
{
	CObjectAnim2D::Init();

	SetType(TYPE_PLAYER);	//��ސݒ�

	return S_OK;
}

//=================================
//�I��
//=================================
void CPlayer::Uninit(void)
{
	CObjectAnim2D::Uninit();
}

//=================================
//�X�V
//=================================
void CPlayer::Update(void)
{
	//���[�J��
	D3DXVECTOR3 pos = GetPos();
	m_move = VEC3_ZERO;

	//�|�C���^�擾
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	//���擾�ƈړ�
	if (pKeyboard->GetPress(DIK_W))
	{
		if (pKeyboard->GetPress(DIK_A))
		{//����
			m_move.x = sinf(LEFT_UP) * PLAYER_SPEED;
			m_move.y = cosf(LEFT_UP) * PLAYER_SPEED;
		}
		else if (pKeyboard->GetPress(DIK_D))
		{//�E��
			m_move.x = sinf(RIGHT_UP) * PLAYER_SPEED;
			m_move.y = cosf(RIGHT_UP) * PLAYER_SPEED;
		}
		else
		{//�^��
			m_move.x = sinf(UP) * PLAYER_SPEED;
			m_move.y = cosf(UP) * PLAYER_SPEED;
		}
	}
	else if (pKeyboard->GetPress(DIK_S))
	{
		if (pKeyboard->GetPress(DIK_A))
		{//����
			m_move.x = sinf(LEFT_DOWN) * PLAYER_SPEED;
			m_move.y = cosf(LEFT_DOWN) * PLAYER_SPEED;
		}
		else if (pKeyboard->GetPress(DIK_D))
		{//�E��
			m_move.x = sinf(RIGHT_DOWN) * PLAYER_SPEED;
			m_move.y = cosf(RIGHT_DOWN) * PLAYER_SPEED;
		}
		else
		{//�^��
			m_move.x = sinf(DOWN) * PLAYER_SPEED;
			m_move.y = cosf(DOWN) * PLAYER_SPEED;
		}
	}
	else if (pKeyboard->GetPress(DIK_A))
	{//��
		m_move.x = sinf(LEFT) * PLAYER_SPEED;
		m_move.y = cosf(LEFT) * PLAYER_SPEED;
	}
	else if (pKeyboard->GetPress(DIK_D))
	{//�E
		m_move.x = sinf(RIGHT) * PLAYER_SPEED;
		m_move.y = cosf(RIGHT) * PLAYER_SPEED;
	}

	//�ʒu�ݒ�
	pos += m_move;
	SetPos(pos);

	//�e
	if (pKeyboard->GetRepeate(DIK_SPACE))
	{
		D3DXVECTOR3 bulletPos = pos + D3DXVECTOR3(10.0f, 0.0f, 0.0f);
		CBullet::Create(bulletPos, GetRot(), 16.0f, 16.0f, 13.0f);
	}

	//�A�j���[�V����
	CObjectAnim2D::Update();
}

//=================================
//�`��
//=================================
void CPlayer::Draw(void)
{
	CObjectAnim2D::Draw();
}

//=================================
//��������
//=================================
CPlayer* CPlayer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight,
	const int nPatWidth, const int nPatHeight, const int nAnimSpeed)
{
	CPlayer* pPlayer = NULL;

	if (pPlayer == NULL)
	{
		//�I�u�W�F�N�g�A�j���[�V����2D�̐���
		pPlayer = new CPlayer(pos, rot, fWidth, fHeight, nPatWidth, nPatHeight, nAnimSpeed);

		//������
		pPlayer->Init();

		//�e�N�X�`�����蓖��
		pPlayer->BindTexture(m_pTexture);

		return pPlayer;
	}
	else
	{
		return NULL;
	}
}

//=================================
//�e�N�X�`���ǂݍ��ݏ���
//=================================
HRESULT CPlayer::Load(const char* pPath)
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
void CPlayer::Unload(void)
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
void CPlayer::AddDamage(int nDamage)
{
	m_nLife -= nDamage;	//�_���[�W�t�^

	//���S����
	if (m_nLife <= DEATH_LIFE)
	{
		Uninit();	//�I������
	}
}