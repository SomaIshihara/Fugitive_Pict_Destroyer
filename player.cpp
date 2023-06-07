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
#include "block.h"
#include "item.h"
#include "score.h"
#include "Culc.h"

//�}�N��
#define PLAYER_SPEED	(5.0f)			//���̈ړ����x
#define PLAYER_JUMP_HEIGHT	(10.0f)		//���̃W�����v��
#define BLOCKCOLLISION_ERRORNUM	(10)	//�u���b�N�����蔻��̌덷

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
	CInputMouse* pMouse = CManager::GetInputMouse();

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

	//�ʒu�ݒ�(���̎��_�ł͂܂��I�u�W�F�N�g�̈ʒu�͍X�V����ĂȂ�)
	pos.x += m_move.x;
	pos.y -= m_move.y - (ACCELERATION_GRAVITY * m_nCounterJumpTime / MAX_FPS);

	//�u���b�N�����蔻��
	if (CollisionBlock(&pos) == true)
	{
		m_bJump = false;
		//�W�����v
		if (pKeyboard->GetRepeate(DIK_SPACE))
		{//�W�����v����
			m_bJump = true;
			m_nCounterJumpTime = 0;
			m_move.y = PLAYER_JUMP_HEIGHT;
		}
	}
	
	//�A�C�e�������蔻��
	CollisionItem(&pos);

	//�ŏI�I�Ȉʒu�̐ݒ�
	SetPos(pos);

	//�e
	if (pMouse->GetRepeate(MOUSE_CLICK_LEFT) == true)
	{
		D3DXVECTOR3 bulletPos = pos + D3DXVECTOR3(10.0f, 0.0f, 0.0f);
		CBullet::Create(bulletPos, GetRot(), 16.0f, 16.0f, 13.0f, TYPE_PLAYER);
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_SHOT);
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

//=================================
//�u���b�N�Ƃ̏Փ˔���
//=================================
bool CPlayer::CollisionBlock(D3DXVECTOR3* pPosNew)
{
	float fPlayerWidth = GetWidth() / 2, fPlayerHeight = GetHeight() / 2;
	bool bLand = false;		//���n����
	bool bHitHead = false;	//���Ԃ���

	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//�S�I�u�W�F�N�g����
		CObject* pObj = GetObject(BLOCK_PRIORITY, cnt);	//�I�u�W�F�N�g�擾

		if (pObj != NULL)	//�k���`�F
		{//�Ȃ񂩂���
			TYPE type = pObj->GetType();	//��ގ擾

			if (type == TYPE_BLOCK)
			{//�u���b�N
				float fOtherWidth = pObj->GetWidth() / 2, fOtherHeight = pObj->GetHeight() / 2;
				D3DXVECTOR3 otherPos = pObj->GetPos();
				if (pPosNew->x + (GetWidth() / 2) > pObj->GetPos().x - pObj->GetWidth() / 2 &&
					pPosNew->x - (GetWidth() / 2) < pObj->GetPos().x + pObj->GetWidth() / 2 &&
					pPosNew->y + (GetHeight() / 2) > pObj->GetPos().y - pObj->GetHeight() / 2 &&
					pPosNew->y - (GetHeight() / 2) < pObj->GetPos().y + pObj->GetHeight() / 2)
				{//��������߂荞�� 
					if (GetPos().y + fPlayerHeight <= otherPos.y - fOtherHeight &&
						pPosNew->y + fPlayerHeight > otherPos.y - fOtherHeight)
					{
						bLand = true;
						m_move.y = 0.0f;
						m_nCounterJumpTime = 0;
						pPosNew->y = otherPos.y - fOtherHeight - fPlayerHeight;
					}
					else if (GetPos().y - fPlayerHeight >= otherPos.y + fOtherHeight &&
						pPosNew->y - fPlayerHeight < otherPos.y + fOtherHeight)
					{
						bHitHead = true;
					}

					if (pPosNew->y - fPlayerHeight < otherPos.y + fOtherHeight &&
						pPosNew->y + fPlayerHeight > otherPos.y - fOtherHeight)
					{
						if (GetPos().x + fPlayerWidth <= otherPos.x - fOtherWidth &&
							pPosNew->x + fPlayerWidth > otherPos.x - fOtherWidth)
						{
							pPosNew->x = otherPos.x - fOtherWidth - fPlayerWidth;
							m_move.x = 0.0f;
							bHitHead = false;
						}
						else if (GetPos().x - fPlayerWidth >= otherPos.x + fOtherWidth &&
							pPosNew->x - fPlayerWidth < otherPos.x + fOtherWidth)
						{
							pPosNew->x = otherPos.x + fOtherWidth + fPlayerWidth;
							m_move.x = 0.0f;
							bHitHead = false;
						}
					}
					if (bHitHead)
					{
						pPosNew->y = otherPos.y + fOtherHeight + fPlayerHeight;
						m_move.y = 0.0f;
						m_nCounterJumpTime = 0;
					}
					if ((pPosNew->y + fPlayerHeight) - (otherPos.y - fOtherHeight) < BLOCKCOLLISION_ERRORNUM)
					{
						pPosNew->y = otherPos.y - fOtherHeight - fPlayerHeight;
					}
				}
			}
		}
	}

	//�������蔻��
	if (pPosNew->y + GetHeight() / 2 > SCREEN_HEIGHT)
	{//�������񒅒n
		bLand = true;
		m_move.y = 0.0f;
		m_nCounterJumpTime = 0;
		pPosNew->y = SCREEN_HEIGHT - GetHeight() / 2;
	}

	return bLand;
}

//=================================
//�u���b�N�Ƃ̏Փ˔���
//=================================
void CPlayer::CollisionItem(D3DXVECTOR3* pPosNew)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//�S�I�u�W�F�N�g����
		CObject* pObj = GetObject(ITEM_PRIORITY, cnt);	//�I�u�W�F�N�g�擾

		if (pObj != NULL)	//�k���`�F
		{//�Ȃ񂩂���
			TYPE type = pObj->GetType();	//��ގ擾

			if (type == TYPE_ITEM)
			{//�A�C�e��
				if (pPosNew->x + (GetWidth() / 2) > pObj->GetPos().x - pObj->GetWidth() / 2 &&
					pPosNew->x - (GetWidth() / 2) < pObj->GetPos().x + pObj->GetWidth() / 2 &&
					pPosNew->y + (GetHeight() / 2) > pObj->GetPos().y - pObj->GetHeight() / 2 &&
					pPosNew->y - (GetHeight() / 2) < pObj->GetPos().y + pObj->GetHeight() / 2)
				{//��������߂荞�� 
					//�X�R�A���Z
					CScore::Add(10000);	//�������񌈂ߑł��ԃX�p

					//�A�C�e������
					pObj->Uninit();
				}
			}
		}
	}
}