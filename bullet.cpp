//======================================================
//
//�e����[bullet.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "object.h"
//#include "player.h"
#include "enemy.h"
#include "block.h"
#include "bullet.h"
#include "input.h"
#include "explosion.h"
#include "particle.h"
#include "effect.h"
#include "Culc.h"

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CBullet::CBullet(int nPriority) : CObject2D(nPriority)
{
	m_nIdxTexture = -1;
	m_move = D3DXVECTOR3(3.0f, 0.0f, 0.0f);
	CObject::SetType(TYPE_BULLET);
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h �ʒu����and�p�^�[���������j
//=================================
CBullet::CBullet(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fSpeed, int nPriority) 
	: CObject2D(pos, rot, fWidth, fHeight, nPriority)
{
	m_nIdxTexture = -1;
	m_move.x = sinf(FIX_ROT(rot.z * D3DX_PI + D3DX_PI)) * fSpeed;
	m_move.y = cosf(FIX_ROT(rot.z * D3DX_PI + D3DX_PI)) * fSpeed;
	CObject::SetType(TYPE_BULLET);
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

	//�e�N�X�`���ǂݍ���
	CTexture* pTexture = CManager::GetTexture();
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\EnergyBullet_01.png");

	//��ސݒ�
	m_Type = TYPE_PLAYER;

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
	//�G�t�F�N�g����
	CEffect2D::Create(GetPos(), VEC3_ZERO, 30.0f, 30.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 30);

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
	if (m_Type == TYPE_PLAYER && CollisionEnemy() == true)
	{//�e���G�ɓ�������
		return;
	}
#if 0
	if (m_Type == TYPE_ENEMY && CollisionPlayer() == true)
	{//�e���v���C���[�ɓ�������
		return;
	}
#endif
	if (m_Type == TYPE_PLAYER && CollisionBlock() == true)
	{//�e���u���b�N�ɓ�������
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
CBullet* CBullet::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fSpeed, const TYPE type)
{
	CBullet* pBullet = NULL;

	if (pBullet == NULL)
	{
		//�I�u�W�F�N�g�A�j���[�V����2D�̐���
		pBullet = new CBullet(pos, rot, fWidth, fHeight, fSpeed);

		//������
		pBullet->Init();

		//�^�C�v�ݒ�
		pBullet->SetType(type);

		//���u��
		pBullet->SetCol(D3DXCOLOR(1.0f, 0.5f, 0.16f,1.0f));

		//�e�N�X�`�����蓖��
		pBullet->BindTexture(pBullet->m_nIdxTexture);

		return pBullet;
	}
	else
	{
		return NULL;
	}
}

//=================================
//�G�Ƃ̏Փ˔���
//=================================
bool CBullet::CollisionEnemy(void)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//�S�I�u�W�F�N�g����
		CObject* pObj = GetObject(ENEMY_PRIORITY,cnt);	//�I�u�W�F�N�g�擾

		if (pObj != NULL)	//�k���`�F
		{//�Ȃ񂩂���
			TYPE type = pObj->GetType();	//��ގ擾

			if (type == TYPE_ENEMY)
			{//�G
				if (GetPos().x > pObj->GetPos().x - pObj->GetWidth() * 0.5f &&
					GetPos().x < pObj->GetPos().x + pObj->GetWidth() * 0.5f &&
					GetPos().y > pObj->GetPos().y - pObj->GetHeight() * 0.5f &&
					GetPos().y < pObj->GetPos().y + pObj->GetHeight() * 0.5f)
				{
					//��������
					//CParticle2D::Create(GetPos(), 48, 16, 2, 3, D3DXCOLOR(1.0f, 0.5f, 0.14f, 1.0f), 20.0f, 20.0f);
					CParticleBillboard::Create(D3DXVECTOR3(0.0f,20.0f,0.0f), 48, 48, 2, 3, D3DXCOLOR(1.0f, 0.5f, 0.14f, 1.0f), 8.0f, 8.0f);
					//CExplosion::Create(GetPos(), GetRot(), 80.0f, 80.0f, 8, 2, 3);

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

#if 0
//=================================
//�G�Ƃ̏Փ˔���
//=================================
bool CBullet::CollisionPlayer(void)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//�S�I�u�W�F�N�g����
		CObject* pObj = GetObject(PLAYER_PRIORITY,cnt);	//�I�u�W�F�N�g�擾

		if (pObj != NULL)	//�k���`�F
		{//�Ȃ񂩂���
			TYPE type = pObj->GetType();	//��ގ擾

			if (type == TYPE_PLAYER)
			{//�G
				if (GetPos().x > pObj->GetPos().x - pObj->GetWidth() * 0.5f &&
					GetPos().x < pObj->GetPos().x + pObj->GetWidth() * 0.5f &&
					GetPos().y > pObj->GetPos().y - pObj->GetHeight() * 0.5f &&
					GetPos().y < pObj->GetPos().y + pObj->GetHeight() * 0.5f)
				{
					//��������
					CParticle2D::Create(GetPos(), 48, 16, 2, 3, D3DXCOLOR(1.0f, 0.5f, 0.14f, 1.0f), 20.0f, 20.0f);

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
#endif

//=================================
//�u���b�N�Ƃ̏Փ˔���
//=================================
bool CBullet::CollisionBlock(void)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//�S�I�u�W�F�N�g����
		CObject* pObj = GetObject(BLOCK_PRIORITY, cnt);	//�I�u�W�F�N�g�擾

		if (pObj != NULL)	//�k���`�F
		{//�Ȃ񂩂���
			TYPE type = pObj->GetType();	//��ގ擾

			if (type == TYPE_BLOCK)
			{//�G
				if (GetPos().x > pObj->GetPos().x - pObj->GetWidth() * 0.5f &&
					GetPos().x < pObj->GetPos().x + pObj->GetWidth() * 0.5f &&
					GetPos().y > pObj->GetPos().y - pObj->GetHeight() * 0.5f &&
					GetPos().y < pObj->GetPos().y + pObj->GetHeight() * 0.5f)
				{
					//��������
					CParticle2D::Create(GetPos(), 48, 16, 2, 3, D3DXCOLOR(1.0f, 0.5f, 0.14f, 1.0f), 20.0f, 20.0f);

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