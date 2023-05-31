//======================================================
//
//�e����[bullet.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "object.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "input.h"
#include "explosion.h"
#include "Culc.h"

//�ÓI�����o�ϐ�
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CBullet::CBullet()
{
	m_move = D3DXVECTOR3(3.0f, 0.0f, 0.0f);
	SetType(TYPE_BULLET);
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h �ʒu����and�p�^�[���������j
//=================================
CBullet::CBullet(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fSpeed) : CObject2D(pos, rot, fWidth, fHeight)
{
	m_move.x = sinf(FIX_ROT(rot.z * D3DX_PI + D3DX_PI)) * fSpeed;
	m_move.y = cosf(FIX_ROT(rot.z * D3DX_PI + D3DX_PI)) * fSpeed;
	SetType(TYPE_BULLET);
}

//=================================
//�f�X�g���N�^
//=================================
CBullet::~CBullet()
{
}

//=================================
//������
//=================================
HRESULT CBullet::Init(void)
{
	CObject2D::Init();

	//��ސݒ�
	m_Type = TYPE_ENEMY;

	return S_OK;
}

//=================================
//�I��
//=================================
void CBullet::Uninit(void)
{
	CObject2D::Uninit();
}

//=================================
//�X�V
//=================================
void CBullet::Update(void)
{
	//���[�J��
	D3DXVECTOR3 pos = GetPos();

	//�ړ�
	pos += m_move;

	//�͈͊O����
	if (pos.x > SCREEN_WIDTH || pos.x < 0.0f || pos.y > SCREEN_HEIGHT || pos.y < 0.0f)
	{
		Uninit();
		return;
	}

	//�ʒu�ݒ�
	SetPos(pos);

	//�G�Ƃ̏Փ˔���
	if (m_Type == TYPE_ENEMY && CollisionEnemy() == true)
	{//�e���G�ɓ�������GetType() == TYPE_ENEMY && 
		return;
	}
	if(m_Type == TYPE_PLAYER &&  CollisionPlayer() == true)
	{//�e���v���C���[�ɓ�������
		return;
	}

	//�e
	CObject2D::Update();
}

//=================================
//�`��
//=================================
void CBullet::Draw(void)
{
	CObject2D::Draw();
}

//=================================
//��������
//=================================
CBullet* CBullet::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fSpeed)
{
	CBullet* pPlayer = NULL;

	if (pPlayer == NULL)
	{
		//�I�u�W�F�N�g�A�j���[�V����2D�̐���
		pPlayer = new CBullet(pos, rot, fWidth, fHeight, fSpeed);

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
HRESULT CBullet::Load(const char* pPath)
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
void CBullet::Unload(void)
{
	//�e�N�X�`���j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=================================
//�G�Ƃ̏Փ˔���
//=================================
bool CBullet::CollisionEnemy(void)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//�S�I�u�W�F�N�g����
		CObject* pObj = GetObject(cnt);	//�I�u�W�F�N�g�擾

		if (pObj != NULL)	//�k���`�F
		{//�Ȃ񂩂���
			TYPE type = pObj->GetType();	//��ގ擾

			if (type == TYPE_ENEMY)
			{//�G
				if (GetPos().x > pObj->GetPos().x - pObj->GetWidth() / 2 &&
					GetPos().x < pObj->GetPos().x + pObj->GetWidth() / 2 &&
					GetPos().y > pObj->GetPos().y - pObj->GetHeight() / 2 &&
					GetPos().y < pObj->GetPos().y + pObj->GetHeight() / 2)
				{
					//��������
					CExplosion::Create(GetPos(), GetRot(), 80.0f, 80.0f, 8, 2, 3);

					//�G�Ƀ_���[�W
					pObj->Uninit();

					//�����I��
					Uninit();

					//�e�͓G�ɓ�������
					return true;
				}
			}
		}
	}
	//�e�͓G�ɓ������Ă��Ȃ�����
	return false;
}

//=================================
//�G�Ƃ̏Փ˔���
//=================================
bool CBullet::CollisionPlayer(void)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//�S�I�u�W�F�N�g����
		CObject* pObj = GetObject(cnt);	//�I�u�W�F�N�g�擾

		if (pObj != NULL)	//�k���`�F
		{//�Ȃ񂩂���
			TYPE type = pObj->GetType();	//��ގ擾

			if (type == TYPE_PLAYER)
			{//�G
				CPlayer* pPlayer = (CPlayer*)pObj;
				if (GetPos().x > pPlayer->GetPos().x - pPlayer->GetWidth() / 2 &&
					GetPos().x < pPlayer->GetPos().x + pPlayer->GetWidth() / 2 &&
					GetPos().y > pPlayer->GetPos().y - pPlayer->GetHeight() / 2 &&
					GetPos().y < pPlayer->GetPos().y + pPlayer->GetHeight() / 2)
				{
					//�i���������j
					//�G�Ƀ_���[�W
					pPlayer->AddDamage(1);

					//�����I��
					Uninit();

					//�e�͓G�ɓ�������
					return true;
				}
			}
		}
	}
	//�e�͓G�ɓ������Ă��Ȃ�����
	return false;
}