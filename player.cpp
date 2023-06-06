//======================================================
//
//�v���C���[����[player.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "manager.h"
#include "sound.h"
#include "renderer.h"
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "Culc.h"

//�}�N��
#define PLAYER_SPEED	(5.0f)	//���̈ړ����x
#define PLAYER_JUMP_HEIGHT	(10.0f)	//���̃W�����v��

//�ÓI�����o�ϐ�
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CPlayer::CPlayer(int nPriority) : CObjectAnim2D(nPriority)
{
	m_nCounterJumpTime = 0;
	m_move = VEC3_ZERO;
	m_bJump = false;
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h �ʒu����and�p�^�[���������j
//=================================
CPlayer::CPlayer(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight,
	const int nPatWidth, const int nPatHeight, const int nAnimSpeed, int nPriority) 
	: CObjectAnim2D(pos, rot, fWidth, fHeight, nPatWidth, nPatHeight, nAnimSpeed, true, nPriority)
{
	m_nCounterJumpTime = 0;
	m_move = VEC3_ZERO;
	m_bJump = false;
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
	m_move.x = 0.0f;

	//�W�����v�J�E���^���₷
	m_nCounterJumpTime++;

	//�|�C���^�擾
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	//���擾�ƈړ�
	//if (pKeyboard->GetPress(DIK_W))
	//{
	//	if (pKeyboard->GetPress(DIK_A))
	//	{//����
	//		m_move.x = sinf(LEFT_UP) * PLAYER_SPEED;
	//		m_move.y = cosf(LEFT_UP) * PLAYER_SPEED;
	//	}
	//	else if (pKeyboard->GetPress(DIK_D))
	//	{//�E��
	//		m_move.x = sinf(RIGHT_UP) * PLAYER_SPEED;
	//		m_move.y = cosf(RIGHT_UP) * PLAYER_SPEED;
	//	}
	//	else
	//	{//�^��
	//		m_move.x = sinf(UP) * PLAYER_SPEED;
	//		m_move.y = cosf(UP) * PLAYER_SPEED;
	//	}
	//}
	//else if (pKeyboard->GetPress(DIK_S))
	//{
	//	if (pKeyboard->GetPress(DIK_A))
	//	{//����
	//		m_move.x = sinf(LEFT_DOWN) * PLAYER_SPEED;
	//		m_move.y = cosf(LEFT_DOWN) * PLAYER_SPEED;
	//	}
	//	else if (pKeyboard->GetPress(DIK_D))
	//	{//�E��
	//		m_move.x = sinf(RIGHT_DOWN) * PLAYER_SPEED;
	//		m_move.y = cosf(RIGHT_DOWN) * PLAYER_SPEED;
	//	}
	//	else
	//	{//�^��
	//		m_move.x = sinf(DOWN) * PLAYER_SPEED;
	//		m_move.y = cosf(DOWN) * PLAYER_SPEED;
	//	}
	//}
	//else 
	if (pKeyboard->GetPress(DIK_A))
	{//��
		m_move.x = sinf(LEFT) * PLAYER_SPEED;
		//m_move.y = cosf(LEFT) * PLAYER_SPEED;
	}
	else if (pKeyboard->GetPress(DIK_D))
	{//�E
		m_move.x = sinf(RIGHT) * PLAYER_SPEED;
		//m_move.y = cosf(RIGHT) * PLAYER_SPEED;
	}

	//�W�����v
	if (m_bJump == false && pKeyboard->GetRepeate(DIK_SPACE))
	{//�W�����v����
		m_bJump = true;
		m_nCounterJumpTime = 0;
		m_move.y = PLAYER_JUMP_HEIGHT;
	}

	//�ʒu�ݒ�
	//pos += m_move;	//���̎��_�ł͂܂��I�u�W�F�N�g�̈ʒu�͍X�V����ĂȂ�
	pos.x += m_move.x;
	pos.y -= m_move.y - (ACCELERATION_GRAVITY * m_nCounterJumpTime / MAX_FPS);

	//�u���b�N�Ƃ̓����蔻��ƃW�����v
	if (pos.y + GetHeight() / 2 - m_move.y - (ACCELERATION_GRAVITY * m_nCounterJumpTime / MAX_FPS) > SCREEN_HEIGHT)
	{//�������񒅒n
		m_bJump = false;
		m_move.y = 0.0f;
		m_nCounterJumpTime = 0;
		pos.y = SCREEN_HEIGHT - GetHeight() / 2;
	}

	//�ŏI�I�Ȉʒu�̐ݒ�
	SetPos(pos);

	////�e
	//if (pKeyboard->GetRepeate(DIK_SPACE))
	//{
	//	D3DXVECTOR3 bulletPos = pos + D3DXVECTOR3(10.0f, 0.0f, 0.0f);
	//	CBullet::Create(bulletPos, GetRot(), 16.0f, 16.0f, 13.0f);
	//	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_SHOT);
	//}

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