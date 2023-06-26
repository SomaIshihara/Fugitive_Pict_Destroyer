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
#define PICT_WALK_SPEED		(6.0f)	//�s�N�g����̕��s���x
#define PICT_STOP_LENGTH	(60.0f)	//�s�N�g���񂪌������痣��鋗��
#define PICT_DESTRUCTION_TIME	(60)//�j��H����s���Ԋu

//�ÓI�����o�ϐ�
CPict* CPict::m_apPict[MAX_OBJ];
int CPict::m_nNumAll = 0;

//���F���f���p�X
const char* c_apModelPath[PICT_MODEL_NUM] =
{
	"data/MODEL/exithuman_x/01_Body.x",
	"data/MODEL/exithuman_x/02_Head.x",
	"data/MODEL/exithuman_x/03_Left_Arm.x",
	"data/MODEL/exithuman_x/04_Left_Hand.x",
	"data/MODEL/exithuman_x/05_Right_Arm.x",
	"data/MODEL/exithuman_x/06_Right_Hand.x",
	"data/MODEL/exithuman_x/07_Left_Leg.x",
	"data/MODEL/exithuman_x/08_Left_Foot.x",
	"data/MODEL/exithuman_x/09_Right_Leg.x",
	"data/MODEL/exithuman_x/10_Right_Foot.x"
};

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
	m_pTargetBuilding= NULL;
	m_pMotion = NULL;
	m_nCounterDestruction = INT_ZERO;
	m_fWidth = FLOAT_ZERO;
	m_fHeight = FLOAT_ZERO;
	m_fDepth = FLOAT_ZERO;
	m_nCounterJumpTime = 0;
	m_bJump = false;
	m_bControll = false;
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
	m_pTargetBuilding = NULL;
	m_nCounterDestruction = INT_ZERO;
	m_fWidth = FLOAT_ZERO;
	m_fHeight = FLOAT_ZERO;
	m_fDepth = FLOAT_ZERO;
	m_nCounterJumpTime = 0;
	m_bJump = false;
	m_bControll = false;
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
	m_pTargetBuilding = NULL;
	m_nCounterDestruction = INT_ZERO;

	//���[�V���������E������
	m_pMotion = new CMotion;
	m_pMotion->Init();

	//���[�V�����r���[�A�̃t�@�C����ǂݍ���
	LoadMotionViewerFile("data\\motion_exithuman.txt", &m_apModel[0], m_pMotion, &m_nNumModel);

	//���[�V�����ݒ�
	m_pMotion->Set(0);

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
}

//========================
//�X�V����
//========================
void CPict::Update(void)
{
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();	//�L�[�{�[�h�擾
	D3DXVECTOR3 targetPos = VEC3_ZERO; 
	float targetWidthHalf = FLOAT_ZERO;
	float targetDepthHalf = FLOAT_ZERO;
	D3DXVECTOR3 pos = m_pos;

	//�W�����v�J�E���^���₷
	m_nCounterJumpTime++;

	if (m_pTargetBuilding != NULL)
	{
		if (m_bControll == false)
		{
			m_move.x = FLOAT_ZERO;
			m_move.z = FLOAT_ZERO;
			targetPos = m_pTargetBuilding->GetPos();
			targetWidthHalf = m_pTargetBuilding->GetWidth() * 0.5f;
			targetDepthHalf = m_pTargetBuilding->GetDepth() * 0.5f;

			if (targetPos.x - targetWidthHalf - PICT_STOP_LENGTH > m_pos.x || targetPos.x + targetWidthHalf + PICT_STOP_LENGTH < m_pos.x ||
				targetPos.z - targetDepthHalf - PICT_STOP_LENGTH > m_pos.z || targetPos.z + targetDepthHalf + PICT_STOP_LENGTH < m_pos.z)
			{
				float fTargetLenWidth, fTargetLenDepth;
				float fTargetRot;

				fTargetLenWidth = targetPos.x - m_pos.x;
				fTargetLenDepth = targetPos.z - m_pos.z;

				fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);

				m_move.x = sinf(fTargetRot) * PICT_WALK_SPEED;
				m_move.z = cosf(fTargetRot) * PICT_WALK_SPEED;

				m_rot.y = FIX_ROT(fTargetRot + D3DX_PI);

				if (m_pMotion->GetType() != 1)
				{
					m_pMotion->Set(1);
				}

				//�j��J�E���^�[���Z�b�g
				m_nCounterDestruction = INT_ZERO;
			}
			else
			{
				m_nCounterDestruction++;
				if (m_nCounterDestruction > PICT_DESTRUCTION_TIME)
				{
					//�j��H��
					m_pTargetBuilding->AddDamage(1000);

					//�j��J�E���^�[���Z�b�g
					m_nCounterDestruction = INT_ZERO;
				}

				if (m_pMotion->GetType() != 0)
				{
					m_pMotion->Set(0);
				}
			}
		}
	}

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
	D3DXMATERIAL *pMat;													//�}�e���A���f�[�^�ւ̃|�C���^

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
//��������
//========================
CPict* CPict::Create(D3DXVECTOR3 pos)
{
	CPict* pPict = NULL;

	if (pPict == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pPict = new CPict(pos);

		//������
		pPict->Init();

		return pPict;
	}
	else
	{
		return NULL;
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
