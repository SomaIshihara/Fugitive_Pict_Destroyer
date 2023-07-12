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
#include "enemy.h"
#include "block.h"
#include "bullet.h"
#include "building.h"
#include "pict.h"
#include "input.h"
#include "explosion.h"
#include "particle.h"
#include "effect.h"
#include "Culc.h"

//****************************************
//2D�e
//****************************************
//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CBullet2D::CBullet2D(int nPriority) : CObject2D(nPriority)
{
	m_nIdxTexture = -1;
	m_move = D3DXVECTOR3(3.0f, 0.0f, 0.0f);
	CObject::SetType(TYPE_BULLET);
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h �ʒu����and�p�^�[���������j
//=================================
CBullet2D::CBullet2D(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fSpeed, int nPriority) 
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
CBullet2D::~CBullet2D()
{
}

//=================================
//������
//=================================
HRESULT CBullet2D::Init(void)
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
void CBullet2D::Uninit(void)
{
	CObject2D::Uninit();
}

//=================================
//�X�V
//=================================
void CBullet2D::Update(void)
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
void CBullet2D::Draw(void)
{
	CObject2D::Draw();
}

//=================================
//��������
//=================================
CBullet2D* CBullet2D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fSpeed, const TYPE type)
{
	CBullet2D* pBullet = NULL;

	if (pBullet == NULL)
	{
		//�I�u�W�F�N�g�A�j���[�V����2D�̐���
		pBullet = new CBullet2D(pos, rot, fWidth, fHeight, fSpeed);

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
bool CBullet2D::CollisionEnemy(void)
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

//=================================
//�u���b�N�Ƃ̏Փ˔���
//=================================
bool CBullet2D::CollisionBlock(void)
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

//****************************************
//�r���{�[�h�e
//****************************************
//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CBulletBillboard::CBulletBillboard(int nPriority) : CObjectBillboard(nPriority)
{
	m_nIdxTexture = -1;
	m_move = D3DXVECTOR3(3.0f, 0.0f, 0.0f);
	CObject::SetType(TYPE_BULLET);
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h �ʒu����and�p�^�[���������j
//=================================
CBulletBillboard::CBulletBillboard(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fSpeed, 
	const int nPower, CPict* firePict, int nPriority) : CObjectBillboard(pos, rot, fWidth, fHeight, nPriority)
{
	m_nIdxTexture = -1;
	m_move.x = sinf(FIX_ROT(rot.y + D3DX_PI)) * fSpeed;
	m_move.y = sinf(FIX_ROT(rot.x + D3DX_PI)) * fSpeed;
	m_move.z = -cosf(FIX_ROT(rot.y + D3DX_PI)) * fSpeed;
	m_nPower = nPower;
	m_pfirePict = firePict;
	CObject::SetType(TYPE_BULLET);
}

//=================================
//�f�X�g���N�^
//=================================
CBulletBillboard::~CBulletBillboard()
{
}

//=================================
//������
//=================================
HRESULT CBulletBillboard::Init(void)
{
	CObjectBillboard::Init();

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
void CBulletBillboard::Uninit(void)
{
	CObjectBillboard::Uninit();
}

//=================================
//�X�V
//=================================
void CBulletBillboard::Update(void)
{
	//�G�t�F�N�g����
	CEffectBillboard::Create(GetPos(), VEC3_ZERO, 30.0f, 30.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 30);

	//���[�J��
	D3DXVECTOR3 pos = GetPos();

	//�ړ�
	pos += m_move;

	//�͈͊O����
	if (pos.x > SCREEN_WIDTH || pos.x < -SCREEN_WIDTH || pos.z > SCREEN_HEIGHT || pos.z < -SCREEN_WIDTH)
	{
		Uninit();
		return;
	}

	//�ʒu�ݒ�
	SetPos(pos);

	//�����Ƃ̏Փ˔���
	if (CollisionBuilding() == true)
	{
		return;
	}

	//�s�N�g�Ƃ̔���
	if (CollisionPict() == true)
	{
		return;
	}

	//�e
	CObjectBillboard::Update();
}

//=================================
//�`��
//=================================
void CBulletBillboard::Draw(void)
{
	CObjectBillboard::Draw();
}

//=================================
//��������
//=================================
CBulletBillboard* CBulletBillboard::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, 
	const float fSpeed, const int nPower, const TYPE type, CPict* firePict)
{
	CBulletBillboard* pBullet = NULL;

	if (pBullet == NULL)
	{
		//�I�u�W�F�N�g�A�j���[�V����2D�̐���
		pBullet = new CBulletBillboard(pos, rot, fWidth, fHeight, fSpeed, nPower, firePict);

		//������
		pBullet->Init();

		//�^�C�v�ݒ�
		pBullet->SetType(type);

		//���u��
		pBullet->SetCol(D3DXCOLOR(1.0f, 0.5f, 0.16f, 1.0f));

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
//�����Ƃ̏Փ˔���
//=================================
bool CBulletBillboard::CollisionBuilding(void)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//�S�I�u�W�F�N�g����
		CBuilding* pBuilding = CBuilding::GetBuilding(cnt);

		if (pBuilding != NULL)	//�k���`�F
		{//�Ȃ񂩂���
			D3DXVECTOR3 buildPos = pBuilding->GetPos();
			float buildWidth = pBuilding->GetWidth();
			float buildDepth = pBuilding->GetDepth();

			if (GetPos().x > buildPos.x - buildWidth * 0.5f &&
				GetPos().x < buildPos.x + buildWidth * 0.5f &&
				GetPos().z > buildPos.z - buildDepth * 0.5f &&
				GetPos().z < buildPos.z + buildDepth * 0.5f)
			{
				//��������
				CParticleBillboard::Create(GetPos(), 30, 32, 0.2f, 3, D3DXCOLOR(1.0f, 0.5f, 0.14f, 1.0f), 20.0f, 20.0f);
				//CParticleBillboard::Create(GetPos(), 120, 16, 0.2f, 3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20.0f, 20.0f);

				//�U��
				pBuilding->AddDamage(m_nPower);

				//�����I��
				Uninit();

				//�e�͓G�ɓ�������
				return true;
			}
		}
	}
	//�e�͓G�ɓ������Ă��Ȃ�����
	return false;
}

//=================================
//�s�N�g�Ƃ̏Փ˔���
//=================================
bool CBulletBillboard::CollisionPict(void)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//�S�I�u�W�F�N�g����
		CPict* pPict = CPict::GetPict(cnt);

		if (pPict != NULL && pPict != m_pfirePict)	//�k���`�F
		{//�Ȃ񂩂���
			D3DXVECTOR3 pictPos = pPict->GetPos();
			float pictWidth = pPict->GetWidth();
			float pictDepth = pPict->GetDepth();

			if (GetPos().x > pictPos.x - pictWidth * 0.5f &&
				GetPos().x < pictPos.x + pictWidth * 0.5f &&
				GetPos().z > pictPos.z - pictDepth * 0.5f &&
				GetPos().z < pictPos.z + pictDepth * 0.5f)
			{
				//��������
				CParticleBillboard::Create(GetPos(), 10, 4, 1, 2, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 20.0f, 20.0f);

				//�_���[�W
				pPict->AddDamage(m_nPower);

				//�����I��
				Uninit();

				//�e�͓G�ɓ�������
				return true;
			}
		}
	}
	//�e�͓G�ɓ������Ă��Ȃ�����
	return false;
}