//======================================================
//
//�s�N�g���񏈗�[pict.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "pict.h"
#include "debugproc.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"	//��
#include "camera.h"	//��
#include "block.h"	//��
#include "player.h"
#include "bullet.h"
#include "motion.h"
#include "building.h"
#include "shadow.h"
#include "meshField.h"
#include "file.h"
#include "Culc.h"

//�}�N��
#define PICT_WALK_SPEED				(6.0f)		//�s�N�g����̕��s���x
#define PICT_BUIDING_STOP_LENGTH	(120.0f)	//�s�N�g���񂪌������痣��鋗��
#define PICT_POLICE_STOP_LENGTH		(30.0f)		//�s�N�g���񂪌x�@���痣��鋗��
#define PICT_ATTACK_TIME			(60)		//�U�����s���Ԋu

//�ÓI�����o�ϐ�
CPict* CPict::m_apPict[MAX_OBJ];
int CPict::m_nNumAll = 0;
CPictDestroyer* CPictDestroyer::m_apPict[MAX_OBJ];
int CPictDestroyer::m_nNumAll = 0;
CPictBlocker* CPictBlocker::m_apPict[MAX_OBJ];
int CPictBlocker::m_nNumAll = 0;
CPictPolice* CPictPolice::m_apPict[MAX_OBJ];
int CPictPolice::m_nNumAll = 0;

//******************************************************
//�����̃s�N�g�N���X
//******************************************************
//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CPict::CPict()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apPict[cnt] == NULL)
		{//�����
			m_apPict[cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;	//ID�o����
			m_nNumAll++;	//�������₷
			break;
		}
	}
	//�l�N���A
	m_pos = VEC3_ZERO;
	m_rot = VEC3_ZERO;
	m_pMotion = NULL;
	m_fWidth = FLOAT_ZERO;
	m_fHeight = FLOAT_ZERO;
	m_fDepth = FLOAT_ZERO;
	m_nCounterJumpTime = 0;
	m_bJump = false;
	m_bControll = false;
	nLife = INT_ZERO;
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h�j
//=================================
CPict::CPict(const D3DXVECTOR3 pos)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apPict[cnt] == NULL)
		{//�����
			m_apPict[cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;	//ID�o����
			m_nNumAll++;	//�������₷
			break;
		}
	}
	//�l�N���A
	m_pos = pos;
	m_rot = VEC3_ZERO;
	m_fWidth = FLOAT_ZERO;
	m_fHeight = FLOAT_ZERO;
	m_fDepth = FLOAT_ZERO;
	m_nCounterJumpTime = 0;
	m_bJump = false;
	m_bControll = false;
	nLife = 1000;
}

//=================================
//�f�X�g���N�^
//=================================
CPict::~CPict()
{
}

//========================
//����������
//========================
HRESULT CPict::Init(void)
{
	//���[�V���������E������
	m_pMotion = new CMotion;
	m_pMotion->Init();

	//���[�V�����r���[�A�̃t�@�C����ǂݍ���
	LoadMotionViewerFile("data\\motion_exithuman.txt", &m_apModel[0], m_pMotion, &m_nNumModel);

	//���[�V�����ݒ�
	m_pMotion->Set(0);

	//�R���W�����ݒ�
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(30.0f, 30.0f, 30.0f);
	m_collision.SetVtx(vtxMax, -vtxMax);

	//�e�ݒ�
	m_pShadow = CShadow::Create();

	//�T�C�Y�ݒ�
	m_fWidth = 30.0f;
	m_fHeight = 120.0f;
	m_fDepth = 30.0f;

	//�W�����v���Z�b�g
	m_nCounterJumpTime = 0;
	m_bJump = false;

	//���c���Ȃ��ݒ�
	m_bControll = false;

	//�ł���
	return S_OK;
}

//========================
//�I������
//========================
void CPict::Uninit(void)
{
	//���[�V�����j��
	if (m_pMotion != NULL)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = NULL;
	}

	m_apPict[m_nID] = NULL;
	for (int cnt = 0; cnt < PICT_MODEL_NUM; cnt++)
	{//�������
		m_apModel[cnt]->Uninit();
		delete m_apModel[cnt];
		m_apModel[cnt] = NULL;
	}

	//�e����
	if (m_pShadow != NULL)
	{
		m_pShadow->Release();
	}

	//�������g�j��
	Release();
}

//========================
//�X�V����
//========================
void CPict::Update(void)
{
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();	//�L�[�{�[�h�擾
	D3DXVECTOR3 pos = m_pos;

	//�W�����v�J�E���^���₷
	m_nCounterJumpTime++;

	//���f���ݒ�
	for (int cnt = 0; cnt < PICT_MODEL_NUM; cnt++)
	{
		m_apModel[cnt]->Update();
	}
	//���[�V����������
	if (m_pMotion != NULL)
	{
		//���[�V�����X�V
		m_pMotion->Update();
	}

	//�����蔻��
	pos.x += m_move.x;
	CollisionBlockX(&pos);

	pos.y += m_move.y - (ACCELERATION_GRAVITY * m_nCounterJumpTime / MAX_FPS);
	if (CollisionBlockY(&pos) == true)
	{
		m_bJump = false;

		//�W�����v
		if (pKeyboard->GetRepeate(DIK_J))
		{//�W�����v����
			m_bJump = true;
			m_nCounterJumpTime = 0;
			m_move.y = 5.0f;
		}
	}

	pos.z += m_move.z;
	CollisionBlockZ(&pos);

	m_pos = pos;

#if 0 
	if (pKeyboard->GetRepeate(DIK_SPACE) == true)
	{
		CBulletBillboard::Create(m_pos, m_rot, 16.0f, 16.0f, 10.0f, CObject::TYPE_PLAYER);
	}

	//�Ǐ]�ݒ�
	if (pKeyboard->GetTrigger(DIK_C) == true)
	{
		CManager::GetPlayer()->Chace(m_nID);
	}
	else if (pKeyboard->GetTrigger(DIK_U) == true)
	{
		CManager::GetPlayer()->Unchace();
	}
#endif

	//�e�ݒ�
	m_pShadow->Set(m_pos, m_rot);
	CManager::GetDebProc()->Print("pos = (x = %f, y = %f, z = %f)\n", m_pos.x, m_pos.y, m_pos.z);
}

//========================
//�`�揈��
//========================
void CPict::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//�f�o�C�X�擾
	CTexture* pTexture = CManager::GetTexture();						//�e�N�X�`���I�u�W�F�N�g�擾
	D3DXMATRIX mtxRot, mtxTrans, mtxTexture;							//�v�Z�p
	D3DMATERIAL9 matDef;												//���݂̃}�e���A���ۑ��p

	//���݂̃}�e���A���擾
	pDevice->GetMaterial(&matDef);

	//���f���擾
	//���[���h�}�g���b�N�X������
	D3DXMatrixIdentity(&m_mtxWorld);

	//�g�k�𔽉f
	//D3DXMatrixScaling(&mtxScall, FENCE_SCALE, FENCE_SCALE, FENCE_SCALE);
	//D3DXMatrixMultiply(&g_aFence[nCount].mtxWorld, &g_aFence[nCount].mtxWorld, &mtxScall);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu���f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���f���`��
	for (int cnt = 0; cnt < PICT_MODEL_NUM; cnt++)
	{
		m_apModel[cnt]->Draw();
	}

	//�}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//========================
//�_���[�W�t�^����
//========================
void CPict::AddDamage(int nDamage)
{
	nLife -= nDamage;	//�t�^

	//0�ɂȂ��������
	if (nLife <= INT_ZERO)
	{
		for (int cnt = 0; cnt < MAX_OBJ; cnt++)
		{//�S�I�u�W�F�N�g����
			CPictBlocker* pPict = CPictBlocker::GetPict(cnt);	//�I�u�W�F�N�g�擾

			if (pPict != NULL)	//�k���`�F
			{//�Ȃ񂩂���
				if (pPict->GetTarget() == this)
				{//�������^�[�Q�b�g
					pPict->UnsetTarget();	//�^�[�Q�b�g�O��
				}
			}
		}

		//���U
		Uninit();
	}
}

//=================================
//�u���b�N�Ƃ̏Փ˔���(X)
//=================================
void CPict::CollisionBlockX(D3DXVECTOR3* pPosNew)
{
	float fPlayerWidth = GetWidth() * 0.5f, fPlayerHeight = GetHeight() * 0.5f, fPlayerDepth = GetDepth() * 0.5f;

	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//�S�I�u�W�F�N�g����
		CObject* pObj = GetObject(BLOCK_PRIORITY, cnt);	//�I�u�W�F�N�g�擾

		if (pObj != NULL)	//�k���`�F
		{//�Ȃ񂩂���
			TYPE type = pObj->GetType();	//��ގ擾

			if (type == TYPE_BLOCK)
			{//�u���b�N
				float fOtherWidth = pObj->GetWidth() * 0.5f, fOtherHeight = pObj->GetHeight() * 0.5f, fOtherDepth = pObj->GetDepth() * 0.5f;
				D3DXVECTOR3 otherPos = pObj->GetPos();
				if (pPosNew->y - fPlayerHeight < otherPos.y + fOtherHeight &&
					pPosNew->y + fPlayerHeight > otherPos.y - fOtherHeight && 
					pPosNew->z - fPlayerDepth < otherPos.z + fOtherDepth &&
					pPosNew->z + fPlayerDepth > otherPos.z - fOtherDepth)
				{
					if (GetPos().x + fPlayerWidth <= otherPos.x - fOtherWidth &&
						pPosNew->x + fPlayerWidth > otherPos.x - fOtherWidth)
					{
						pPosNew->x = otherPos.x - fOtherWidth - fPlayerWidth;
						m_move.x = 0.0f;
					}
					else if (GetPos().x - fPlayerWidth >= otherPos.x + fOtherWidth &&
						pPosNew->x - fPlayerWidth < otherPos.x + fOtherWidth)
					{
						pPosNew->x = otherPos.x + fOtherWidth + fPlayerWidth;
						m_move.x = 0.0f;
					}
				}
			}
		}
	}
}

//=================================
//�u���b�N�Ƃ̏Փ˔���(Y)
//=================================
bool CPict::CollisionBlockY(D3DXVECTOR3* pPosNew)
{
	float fPlayerWidth = GetWidth() * 0.5f, fPlayerHeight = GetHeight() * 0.5f, fPlayerDepth = GetDepth() * 0.5f;
	bool bLand = false;

	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//�S�I�u�W�F�N�g����
		CObject* pObj = GetObject(BLOCK_PRIORITY, cnt);	//�I�u�W�F�N�g�擾

		if (pObj != NULL)	//�k���`�F
		{//�Ȃ񂩂���
			TYPE type = pObj->GetType();	//��ގ擾

			if (type == TYPE_BLOCK)
			{//�u���b�N
				float fOtherWidth = pObj->GetWidth() * 0.5f, fOtherHeight = pObj->GetHeight() * 0.5f, fOtherDepth = pObj->GetDepth() * 0.5f;
				D3DXVECTOR3 otherPos = pObj->GetPos();
				if (pPosNew->x - fPlayerWidth < otherPos.x + fOtherWidth &&
					pPosNew->x + fPlayerWidth > otherPos.x - fOtherWidth &&
					pPosNew->z - fPlayerDepth < otherPos.z + fOtherDepth &&
					pPosNew->z + fPlayerDepth > otherPos.z - fOtherDepth)
				{
					if (GetPos().y + fPlayerHeight <= otherPos.y - fOtherHeight &&
						pPosNew->y + fPlayerHeight > otherPos.y - fOtherHeight)
					{
						pPosNew->y = otherPos.y - fOtherHeight - fPlayerHeight;
						m_move.y = 0.0f;
						m_nCounterJumpTime = 0;
						bLand = true;
					}
					else if (GetPos().y - fPlayerHeight >= otherPos.y + fOtherHeight &&
						pPosNew->y - fPlayerHeight < otherPos.y + fOtherHeight)
					{
						pPosNew->y = otherPos.y + fOtherHeight + fPlayerHeight;
						m_move.x = 0.0f;
						m_nCounterJumpTime = 0;
					}
				}
			}
		}
	}

	//�����擾
	float fLandHeight = CManager::GetMeshField()->GetHeight(*pPosNew);
	if (pPosNew->y < fLandHeight)
	{
		pPosNew->y = fLandHeight;
		bLand = true;
		m_move.y = 0.0f;
		m_nCounterJumpTime = 0;
	}

	return bLand;
}

//=================================
//�u���b�N�Ƃ̏Փ˔���(Z)
//=================================
void CPict::CollisionBlockZ(D3DXVECTOR3* pPosNew)
{
	float fPlayerWidth = GetWidth() * 0.5f, fPlayerHeight = GetHeight() * 0.5f, fPlayerDepth = GetDepth() * 0.5f;

	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//�S�I�u�W�F�N�g����
		CObject* pObj = GetObject(BLOCK_PRIORITY, cnt);	//�I�u�W�F�N�g�擾

		if (pObj != NULL)	//�k���`�F
		{//�Ȃ񂩂���
			TYPE type = pObj->GetType();	//��ގ擾

			if (type == TYPE_BLOCK)
			{//�u���b�N
				float fOtherWidth = pObj->GetWidth() * 0.5f, fOtherHeight = pObj->GetHeight() * 0.5f, fOtherDepth = pObj->GetDepth() * 0.5f;
				D3DXVECTOR3 otherPos = pObj->GetPos();
				if (pPosNew->x - fPlayerWidth < otherPos.x + fOtherWidth &&
					pPosNew->x + fPlayerWidth > otherPos.x - fOtherWidth &&
					pPosNew->y - fPlayerHeight < otherPos.y + fOtherHeight &&
					pPosNew->y + fPlayerHeight > otherPos.y - fOtherHeight
					)
				{
					if (GetPos().z + fPlayerDepth <= otherPos.z - fOtherDepth &&
						pPosNew->z + fPlayerDepth > otherPos.z - fOtherDepth)
					{
						pPosNew->z = otherPos.z - fOtherDepth - fPlayerDepth;
						m_move.z = 0.0f;
					}
					else if (GetPos().z - fPlayerDepth >= otherPos.z + fOtherDepth &&
						pPosNew->z - fPlayerDepth < otherPos.z + fOtherDepth)
					{
						pPosNew->z = otherPos.z + fOtherDepth + fPlayerDepth;
						m_move.z = 0.0f;
					}
				}
			}
		}
	}
}

//=================================
//�s�N�g����̑��c
//=================================
void CPict::Controll(D3DXVECTOR3 move)
{
	if (m_bControll == true)
	{//���c�\
		m_move = move;
	}
}

//******************************************************
//�f�X�g���C���[�s�N�g�N���X
//******************************************************
//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CPictDestroyer::CPictDestroyer()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apPict[cnt] == NULL)
		{//�����
			m_apPict[cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;	//ID�o����
			m_nNumAll++;	//�������₷
			break;
		}
	}
	m_pTargetBuilding = NULL;
	m_nCounterDestruction = INT_ZERO;
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h�j
//=================================
CPictDestroyer::CPictDestroyer(const D3DXVECTOR3 pos) : CPict(pos)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apPict[cnt] == NULL)
		{//�����
			m_apPict[cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;	//ID�o����
			m_nNumAll++;	//�������₷
			break;
		}
	}
	m_pTargetBuilding = NULL;
	m_nCounterDestruction = INT_ZERO;
}

//=================================
//�f�X�g���N�^
//=================================
CPictDestroyer::~CPictDestroyer()
{
}

//========================
//����������
//========================
HRESULT CPictDestroyer::Init(void)
{
	//�e����
	CPict::Init();

	return S_OK;
}

//========================
//�I������
//========================
void CPictDestroyer::Uninit(void)
{
	m_apPict[m_nID] = NULL;

	//�e����
	CPict::Uninit();
}

//========================
//�X�V����
//========================
void CPictDestroyer::Update(void)
{
	D3DXVECTOR3 targetPos = VEC3_ZERO;
	float targetWidthHalf = FLOAT_ZERO;
	float targetDepthHalf = FLOAT_ZERO;
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();
	CMotion* pMotion = GetMotion();

	//XZ�̂ݏ���
	move.x = FLOAT_ZERO;
	move.z = FLOAT_ZERO;

	if (m_pTargetBuilding != NULL)
	{
		if (CPict::IsControll() == false)
		{
			move.x = FLOAT_ZERO;
			move.z = FLOAT_ZERO;
			targetPos = m_pTargetBuilding->GetPos();
			targetWidthHalf = m_pTargetBuilding->GetWidth() * 0.5f;
			targetDepthHalf = m_pTargetBuilding->GetDepth() * 0.5f;

			if (targetPos.x - targetWidthHalf - PICT_BUIDING_STOP_LENGTH > pos.x || targetPos.x + targetWidthHalf + PICT_BUIDING_STOP_LENGTH < pos.x ||
				targetPos.z - targetDepthHalf - PICT_BUIDING_STOP_LENGTH > pos.z || targetPos.z + targetDepthHalf + PICT_BUIDING_STOP_LENGTH < pos.z)
			{
				float fTargetLenWidth, fTargetLenDepth;
				float fTargetRot;

				fTargetLenWidth = targetPos.x - pos.x;
				fTargetLenDepth = targetPos.z - pos.z;

				fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);

				move.x = sinf(fTargetRot) * PICT_WALK_SPEED;
				move.z = cosf(fTargetRot) * PICT_WALK_SPEED;

				rot.y = FIX_ROT(fTargetRot + D3DX_PI);

				if (pMotion->GetType() != MOTIONTYPE_MOVE)
				{
					pMotion->Set(MOTIONTYPE_MOVE);
				}

				//�j��J�E���^�[���Z�b�g
				m_nCounterDestruction = INT_ZERO;
			}
			else
			{
				m_nCounterDestruction++;
				if (m_nCounterDestruction > PICT_ATTACK_TIME)
				{
					//�j��H��
					m_pTargetBuilding->AddDamage(1000);

					//�j��J�E���^�[���Z�b�g
					m_nCounterDestruction = INT_ZERO;
				}

				if (pMotion->GetType() != MOTIONTYPE_DESTROY)
				{
					pMotion->Set(MOTIONTYPE_DESTROY);
				}
			}
		}
	}
	else
	{
		if (pMotion->GetType() != MOTIONTYPE_NEUTRAL)
		{
			pMotion->Set(MOTIONTYPE_NEUTRAL);
		}
	}

	//�l�ݒ�
	SetRot(rot);
	SetMove(move);

	//�e����
	CPict::Update();
}

//========================
//�`�揈��
//========================
void CPictDestroyer::Draw(void)
{
	//�e����
	CPict::Draw();
}

//========================
//��������
//========================
CPictDestroyer* CPictDestroyer::Create(const D3DXVECTOR3 pos)
{
	CPictDestroyer* pPict = NULL;

	if (pPict == NULL)
	{
		//�s�N�g�̐���
		pPict = new CPictDestroyer(pos);

		//������
		pPict->Init();

		return pPict;
	}
	else
	{
		return NULL;
	}
}

//******************************************************
//�u���b�J�[�s�N�g�N���X
//******************************************************
//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CPictBlocker::CPictBlocker()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apPict[cnt] == NULL)
		{//�����
			m_apPict[cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;	//ID�o����
			m_nNumAll++;	//�������₷
			break;
		}
	}
	m_nCounterAttack = INT_ZERO;
	m_pTargetPolice = NULL;
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h�j
//=================================
CPictBlocker::CPictBlocker(const D3DXVECTOR3 pos) : CPict(pos)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apPict[cnt] == NULL)
		{//�����
			m_apPict[cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;	//ID�o����
			m_nNumAll++;	//�������₷
			break;
		}
	}
	m_nCounterAttack = INT_ZERO;
	m_pTargetPolice = NULL;
}

//=================================
//�f�X�g���N�^
//=================================
CPictBlocker::~CPictBlocker()
{
}

//========================
//����������
//========================
HRESULT CPictBlocker::Init(void)
{
	//�e����
	CPict::Init();

	return S_OK;
}

//========================
//�I������
//========================
void CPictBlocker::Uninit(void)
{
	m_apPict[m_nID] = NULL;

	//�e����
	CPict::Uninit();
}

//========================
//�X�V����
//========================
void CPictBlocker::Update(void)
{
	D3DXVECTOR3 targetPos = VEC3_ZERO;
	float targetWidthHalf = FLOAT_ZERO;
	float targetDepthHalf = FLOAT_ZERO;
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();
	CMotion* pMotion = GetMotion();

	if (m_pTargetPolice != NULL)
	{
		if (CPict::IsControll() == false)
		{
			move.x = FLOAT_ZERO;
			move.z = FLOAT_ZERO;
			targetPos = m_pTargetPolice->GetPos();
			targetWidthHalf = m_pTargetPolice->GetWidth() * 0.5f;
			targetDepthHalf = m_pTargetPolice->GetDepth() * 0.5f;

			if (targetPos.x - targetWidthHalf - PICT_POLICE_STOP_LENGTH > pos.x || targetPos.x + targetWidthHalf + PICT_POLICE_STOP_LENGTH < pos.x ||
				targetPos.z - targetDepthHalf - PICT_POLICE_STOP_LENGTH > pos.z || targetPos.z + targetDepthHalf + PICT_POLICE_STOP_LENGTH < pos.z)
			{
				float fTargetLenWidth, fTargetLenDepth;
				float fTargetRot;

				fTargetLenWidth = targetPos.x - pos.x;
				fTargetLenDepth = targetPos.z - pos.z;

				fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);

				move.x = sinf(fTargetRot) * PICT_WALK_SPEED;
				move.z = cosf(fTargetRot) * PICT_WALK_SPEED;

				rot.y = FIX_ROT(fTargetRot + D3DX_PI);

				if (pMotion->GetType() != MOTIONTYPE_MOVE)
				{
					pMotion->Set(MOTIONTYPE_MOVE);
				}

				//�U���J�E���^�[���Z�b�g
				m_nCounterAttack = INT_ZERO;
			}
			else
			{
				m_nCounterAttack++;
				if (m_nCounterAttack > PICT_ATTACK_TIME)
				{
					//�U��
					m_pTargetPolice->AddDamage(100);

					//�U���J�E���^�[���Z�b�g
					m_nCounterAttack = INT_ZERO;
				}

				if (pMotion->GetType() != MOTIONTYPE_ATTACK)
				{
					pMotion->Set(MOTIONTYPE_ATTACK);
				}
			}
		}
	}
	else
	{
		if (pMotion->GetType() != MOTIONTYPE_NEUTRAL)
		{
			pMotion->Set(MOTIONTYPE_NEUTRAL);
		}
	}

	//�l�ݒ�
	SetRot(rot);
	SetMove(move);

	//�e����
	CPict::Update();
}

//========================
//�`�揈��
//========================
void CPictBlocker::Draw(void)
{
	//�e����
	CPict::Draw();
}

//========================
//��������
//========================
CPictBlocker* CPictBlocker::Create(const D3DXVECTOR3 pos)
{
	CPictBlocker* pPict = NULL;

	if (pPict == NULL)
	{
		//�s�N�g�̐���
		pPict = new CPictBlocker(pos);

		//������
		pPict->Init();

		return pPict;
	}
	else
	{
		return NULL;
	}
}

//******************************************************
//�s�N�g�x�@�N���X
//******************************************************
//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CPictPolice::CPictPolice()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apPict[cnt] == NULL)
		{//�����
			m_apPict[cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;	//ID�o����
			m_nNumAll++;	//�������₷
			break;
		}
	}
	m_pTargetBuilding = NULL;
	m_nCounterAttack = INT_ZERO;
	m_pTargetPict = NULL;
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h�j
//=================================
CPictPolice::CPictPolice(const D3DXVECTOR3 pos) : CPict(pos)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apPict[cnt] == NULL)
		{//�����
			m_apPict[cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;	//ID�o����
			m_nNumAll++;	//�������₷
			break;
		}
	}
	m_pTargetBuilding = NULL;
	m_nCounterAttack = INT_ZERO;
	m_pTargetPict = NULL;
}

//=================================
//�f�X�g���N�^
//=================================
CPictPolice::~CPictPolice()
{
}

//========================
//����������
//========================
HRESULT CPictPolice::Init(void)
{
	//�e����
	CPict::Init();

	return S_OK;
}

//========================
//�I������
//========================
void CPictPolice::Uninit(void)
{
	m_apPict[m_nID] = NULL;

	//�e����
	CPict::Uninit();
}

//========================
//�X�V����
//========================
void CPictPolice::Update(void)
{
	D3DXVECTOR3 targetPos = VEC3_ZERO;
	float targetWidthHalf = FLOAT_ZERO;
	float targetDepthHalf = FLOAT_ZERO;
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();
	CMotion* pMotion = GetMotion();

	if (m_pTargetPict != NULL)
	{
		if (CPict::IsControll() == false)
		{
			move.x = FLOAT_ZERO;
			move.z = FLOAT_ZERO;
			targetPos = m_pTargetPict->GetPos();
			targetWidthHalf = m_pTargetPict->GetWidth() * 0.5f;
			targetDepthHalf = m_pTargetPict->GetDepth() * 0.5f;

			if (targetPos.x - targetWidthHalf - PICT_POLICE_STOP_LENGTH > pos.x || targetPos.x + targetWidthHalf + PICT_POLICE_STOP_LENGTH < pos.x ||
				targetPos.z - targetDepthHalf - PICT_POLICE_STOP_LENGTH > pos.z || targetPos.z + targetDepthHalf + PICT_POLICE_STOP_LENGTH < pos.z)
			{
				float fTargetLenWidth, fTargetLenDepth;
				float fTargetRot;

				fTargetLenWidth = targetPos.x - pos.x;
				fTargetLenDepth = targetPos.z - pos.z;

				fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);

				move.x = sinf(fTargetRot) * PICT_WALK_SPEED;
				move.z = cosf(fTargetRot) * PICT_WALK_SPEED;

				rot.y = FIX_ROT(fTargetRot + D3DX_PI);

				if (pMotion->GetType() != MOTIONTYPE_MOVE)
				{
					pMotion->Set(MOTIONTYPE_MOVE);
				}

				//�U���J�E���^�[���Z�b�g
				m_nCounterAttack = INT_ZERO;
			}
			else
			{
				m_nCounterAttack++;
				if (m_nCounterAttack > PICT_ATTACK_TIME)
				{
					//�U��
					m_pTargetPict->AddDamage(10);

					//�U���J�E���^�[���Z�b�g
					m_nCounterAttack = INT_ZERO;
				}

				if (pMotion->GetType() != MOTIONTYPE_ATTACK)
				{
					pMotion->Set(MOTIONTYPE_ATTACK);
				}
			}
		}
	}
	else
	{
		for (int cnt = 0; cnt < MAX_OBJ; cnt++)
		{//�S�I�u�W�F�N�g����
			CPictBlocker* pPict = CPictBlocker::GetPict(cnt);	//�I�u�W�F�N�g�擾

			if (pPict != NULL)
			{
				if (D3DXVec3Length(&(pPict->GetPos() - this->GetPos())) <= 50.0f)
				{
					m_pTargetPict = pPict;
				}
			}
		}

		if (pMotion->GetType() != MOTIONTYPE_NEUTRAL)
		{
			pMotion->Set(MOTIONTYPE_NEUTRAL);
		}
	}

	//�l�ݒ�
	SetRot(rot);
	SetMove(move);

	//�e����
	CPict::Update();
}

//========================
//�`�揈��
//========================
void CPictPolice::Draw(void)
{
	//�e����
	CPict::Draw();
}

//========================
//��������
//========================
CPictPolice* CPictPolice::Create(const D3DXVECTOR3 pos)
{
	CPictPolice* pPict = NULL;

	if (pPict == NULL)
	{
		//�s�N�g�̐���
		pPict = new CPictPolice(pos);

		//������
		pPict->Init();

		return pPict;
	}
	else
	{
		return NULL;
	}
}