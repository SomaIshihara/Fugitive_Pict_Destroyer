//======================================================
//
//�s�N�g���񏈗�[picto.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "picto.h"
#include "debugproc.h"
#include "model.h"
#include "manager.h"
#include "game.h"
#include "tutorial.h"
#include "renderer.h"
#include "input.h"	//��
#include "camera.h"	//��
#include "player.h"
#include "bullet.h"
#include "motion.h"
#include "building.h"
#include "shadow.h"
#include "meshField.h"
#include "slider.h"
#include "file.h"
#include "Culc.h"
#include "point.h"
#include "havenum.h"
#include "level.h"

//�}�N��
#define PICTO_WALK_SPEED			(6.0f)		//�s�N�g����̕��s���x
#define PICTO_POINT_RESEARCH_LENGTH	(35.0f)		//�s�N�g���񂪃|�C���g�ɓ����������Ƃɂ��鋗��
#define PICTO_AGIT_STOP_LENGTH		(20.0f)		//�s�N�g���񂪃A�W�g���痣��鋗��
#define PICTO_BUIDING_STOP_LENGTH	(120.0f)	//�s�N�g���񂪌������痣��鋗��
#define PICTO_POLICE_STOP_LENGTH	(55.0f)		//�s�N�g���񂪌x�@���痣��鋗��
#define PICTO_POLICE_SEARCH_LENGTH	(60.0f)		//�s�N�g�x�@�̃T�[�`�͈�
#define PICTO_ATTACK_TIME			(60)		//�U�����s���Ԋu
#define PICTO_DAMAGE_TIME			(120)		//�Ԃ����鎞��
#define PICTO_LIFE					(1000)		//�̗�
#define PICTO_SOCIAL_DISTANCE		(30.0f)		//�\�[�V�����f�B�X�^���X�͈�

#define PICTO_FORCEDRETURN_NUM		(2)			//�����A���܂ł̐l��

//�v�Z
#define PICTO_POWER(lv,hpct)	((int)ceil((50 + (50 * ((float)hpct / PICTO_HAVENPICTO(lv)) * 1.2f) + hpct) * PICTO_ATK(lv)))

//�ÓI�����o�ϐ�
CPicto* CPicto::m_apPicto[MAX_OBJ];
int CPicto::m_nNumAll = 0;
CObjectX* CPicto::m_pAgitObj = nullptr;
const float CPicto::LOOSE_LENGTH = 100.0f;

CPictoDestroyer* CPictoDestroyer::m_apPicto[MAX_OBJ];
int CPictoDestroyer::m_nNumAll = 0;
int CPictoDestroyer::m_nLv = 1;
int CPictoDestroyer::m_nExp = 0;

CPictoBlocker* CPictoBlocker::m_apPicto[MAX_OBJ];
int CPictoBlocker::m_nNumAll = 0;
int CPictoBlocker::m_nLv = 1;
int CPictoBlocker::m_nExp = 0;

CPictoTaxi* CPictoTaxi::m_apPicto[MAX_OBJ];
int CPictoTaxi::m_nNumAll = 0;
const float CPictoTaxi::RESCUE_LIFE = 0.5f;

CPictoNormal* CPictoNormal::m_apPicto[MAX_OBJ];
int CPictoNormal::m_nNumAll = 0;
const int CPictoNormal::NORMAL_D_PERCENT = 15;
const int CPictoNormal::NORMAL_B_PERCENT = 20;
const int CPictoNormal::NORMAL_N_PERCENT = 65;
const int CPictoNormal::NORMAL_NUM_MIN = 500;
const int CPictoNormal::NORMAL_NUM_DEGREE = 2500;

CPictoPolice* CPictoPolice::m_apPicto[MAX_OBJ];
int CPictoPolice::m_nNumAll = 0;

//******************************************************
//�����̃s�N�g�N���X
//******************************************************
//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CPicto::CPicto()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apPicto[cnt] == nullptr)
		{//�����
			m_apPicto[cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;	//ID�o����
			m_nNumAll++;	//�������₷
			break;
		}
	}
	//�l�N���A
	m_pos = CManager::VEC3_ZERO;
	m_rot = CManager::VEC3_ZERO;
	m_pMotion = nullptr;
	m_fWidth = CManager::FLOAT_ZERO;
	m_fHeight = CManager::FLOAT_ZERO;
	m_fDepth = CManager::FLOAT_ZERO;
	m_nCounterJumpTime = 0;
	m_bJump = false;
	m_fRedAlpha = CManager::FLOAT_ZERO;
	m_state = STATE_MAX;
	m_type = TYPE_MAX;
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h�j
//=================================
CPicto::CPicto(const D3DXVECTOR3 pos, const TYPE type)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apPicto[cnt] == nullptr)
		{//�����
			m_apPicto[cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;	//ID�o����
			m_nNumAll++;	//�������₷
			break;
		}
	}
	//�l�N���A
	m_pos = pos;
	m_rot = CManager::VEC3_ZERO;
	m_pMotion = nullptr;
	m_fWidth = CManager::FLOAT_ZERO;
	m_fHeight = CManager::FLOAT_ZERO;
	m_fDepth = CManager::FLOAT_ZERO;
	m_nCounterJumpTime = 0;
	m_bJump = false;
	m_fRedAlpha = CManager::FLOAT_ZERO;
	m_state = STATE_MAX;
	m_type = type;
}

//=================================
//�f�X�g���N�^
//=================================
CPicto::~CPicto()
{
}

//========================
//����������
//========================
HRESULT CPicto::Init(void)
{
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

	//��Ԑݒ�
	m_state = STATE_FACE;

	//�s�N�g����ł���
	SetType(CObject::TYPE_PICTO);

	//�ł���
	return S_OK;
}

//========================
//�I������
//========================
void CPicto::Uninit(void)
{
	//�x�@�^�[�Q�b�g����
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//�S�I�u�W�F�N�g����
		CPictoPolice* pPicto = CPictoPolice::GetPicto(cnt);	//�I�u�W�F�N�g�擾

		if (pPicto != nullptr)	//�k���`�F
		{//�Ȃ񂩂���
			pPicto->UnsetTarget();	//�^�[�Q�b�g����
		}
	}

	//�^�N�V�[�^�[�Q�b�g����
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//�S�I�u�W�F�N�g����
		CPictoTaxi* pPicto = CPictoTaxi::GetPicto(cnt);	//�I�u�W�F�N�g�擾

		if (pPicto != nullptr)	//�k���`�F
		{//�Ȃ񂩂���
			pPicto->UnsetTargetPicto();	//�^�[�Q�b�g����
		}
	}

	//���[�V�����j��
	if (m_pMotion != nullptr)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	m_apPicto[m_nID] = nullptr;
	if (m_ppModel != nullptr)
	{
		for (int cnt = 0; cnt < m_nNumModel; cnt++)
		{//�������
			if (m_ppModel[cnt] != nullptr)
			{
				m_ppModel[cnt]->Uninit();
				delete m_ppModel[cnt];
				m_ppModel[cnt] = nullptr;
			}
		}
		delete[] m_ppModel;	//�z�񂻂̂��̂�j��
	}

	//�e����
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
	}

	//�l�����炷
	m_nNumAll--;

	//�������g�j��
	Release();
}

//========================
//�X�V����
//========================
void CPicto::Update(void)
{
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();	//�L�[�{�[�h�擾
	D3DXVECTOR3 pos = m_pos;
	CMotion* pMotion = GetMotion();

	//�s�N�g����:�|�C���g�ړ�����
	if (m_state != STATE_ATTACK)
	{
		if (m_targetObj != nullptr)
		{//�ړI�n������
			D3DXVECTOR3 targetPos = m_targetObj->GetPos();
			float targetWidthHalf = m_targetObj->GetWidth() * 0.5f;
			float targetDepthHalf = m_targetObj->GetDepth() * 0.5f;

			if (targetPos.x - (targetWidthHalf + 50.0f) < pos.x && targetPos.x + (targetWidthHalf + 50.0f) > pos.x &&
				targetPos.z - (targetDepthHalf + 50.0f) < pos.z && targetPos.z + (targetDepthHalf + 50.0f) > pos.z)
			{//����
				m_move.x = CManager::FLOAT_ZERO;
				m_move.z = CManager::FLOAT_ZERO;
				switch (m_state)
				{
				case STATE_FACE:
					m_state = STATE_ATTACK;
					break;
				case STATE_LEAVE:
					Return();
					return;
					break;
				}
			}
			else
			{//�ړI�n�ł͂Ȃ�
				//�|�C���g�ړ�
				D3DXVECTOR3 pointPos = m_pPoint->GetPos();
				if (pointPos.x - PICTO_POINT_RESEARCH_LENGTH < pos.x && pointPos.x + PICTO_POINT_RESEARCH_LENGTH > pos.x &&
					pointPos.z - PICTO_POINT_RESEARCH_LENGTH < pos.z && pointPos.z + PICTO_POINT_RESEARCH_LENGTH > pos.z)
				{//����
					if (targetPos.x - (targetWidthHalf + 150.0f) < pos.x && targetPos.x + (targetWidthHalf + 150.0f) > pos.x &&
						targetPos.z - (targetDepthHalf + 150.0f) < pos.z && targetPos.z + (targetDepthHalf + 150.0f) > pos.z)
					{//�������߂�
						pointPos = targetPos;
					}
					else
					{//�܂�����
						Search();	//�|�C���g����
						pointPos = m_pPoint->GetPos();	//�V�����|�C���g�̈ʒu�擾
					}
				}

				float fTargetLenWidth, fTargetLenDepth;
				float fTargetRot;
				m_move.x = CManager::FLOAT_ZERO;
				m_move.z = CManager::FLOAT_ZERO;

				fTargetLenWidth = pointPos.x - pos.x;
				fTargetLenDepth = pointPos.z - pos.z;

				fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);

				m_move.x = sinf(fTargetRot) * PICTO_WALK_SPEED;
				m_move.z = cosf(fTargetRot) * PICTO_WALK_SPEED;

				m_rot.y = FIX_ROT(fTargetRot + D3DX_PI);

				if (pMotion->GetType() != MOTIONTYPE_MOVE)
				{
					pMotion->Set(MOTIONTYPE_MOVE);
				}
			}
		}

		//�\�[�V�����f�B�X�^���X����
		float fLengthNear = CManager::FLOAT_ZERO;
		D3DXVECTOR3 posAfter = pos + m_move;
		CPicto* pPictoNear = nullptr;

		//�߂��̒��ׂ�
		for (int cnt = 0; cnt < MAX_OBJ; cnt++)
		{
			if (m_apPicto[cnt] != nullptr && m_apPicto[cnt] != this)
			{//���X�g�ɂ���
				float fLength = D3DXVec3Length(&(m_apPicto[cnt]->GetPos() - posAfter));
				if (pPictoNear == nullptr || fLengthNear > fLength)
				{//�Ȃ�����ĂȂ�
					pPictoNear = m_apPicto[cnt];
					fLengthNear = fLength;
				}
			}
		}

		//�߂����Ȃ��H
		if (pPictoNear != nullptr && fLengthNear <= PICTO_SOCIAL_DISTANCE)
		{//���ł�
			float fTargetLenWidth, fTargetLenDepth;
			float fTargetRot;
			D3DXVECTOR3 posNearPicto = pPictoNear->GetPos();

			//���������ė����
			fTargetLenWidth = posNearPicto.x - posAfter.x;
			fTargetLenDepth = posNearPicto.z - posAfter.z;

			fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);
			fTargetRot = FIX_ROT(fTargetRot + (1.1f * D3DX_PI));

			posAfter.x = posNearPicto.x + (sinf(fTargetRot) * PICTO_SOCIAL_DISTANCE * 2.0f);
			posAfter.z = posNearPicto.z + (cosf(fTargetRot) * PICTO_SOCIAL_DISTANCE * 2.0f);

			//�������ړ��ʂɂ���
			m_move = posAfter - m_pos;
		}
	}

	//�W�����v�J�E���^���₷
	m_nCounterJumpTime++;

	//���f���ݒ�
	if (m_ppModel != nullptr)
	{
		for (int cnt = 0; cnt < m_nNumModel; cnt++)
		{
			if (m_ppModel[cnt] != nullptr)
			{
				m_ppModel[cnt]->Update();
			}
		}
	}

	//���[�V����������
	if (m_pMotion != nullptr)
	{
		//���[�V�����X�V
		m_pMotion->Update();
	}

	//�����蔻��
	pos.x += m_move.x;
	pos.y += m_move.y - (ACCELERATION_GRAVITY * m_nCounterJumpTime / MAX_FPS);

	CollisionField(&pos);

	pos.z += m_move.z;
	m_pos = pos;

	if (m_fRedAlpha >= CManager::FLOAT_ZERO)
	{//�܂��Ԃ�
	 //�ԐF������炷
		m_fRedAlpha -= PICTO_DAMAGE_ALPHA / PICTO_DAMAGE_TIME;

		if (m_fRedAlpha < CManager::FLOAT_ZERO)
		{//�Ԃ��Ȃ��Ȃ���
			m_fRedAlpha = CManager::FLOAT_ZERO;
		}
	}

	//�e�ݒ�
	m_pShadow->Set(m_pos, m_rot);

	m_move.x = CManager::FLOAT_ZERO;
	m_move.z = CManager::FLOAT_ZERO;
}

//========================
//�`�揈��
//========================
void CPicto::Draw(void)
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
	if (m_ppModel != nullptr)
	{
		for (int cnt = 0; cnt < m_nNumModel; cnt++)
		{
			if (m_ppModel[cnt] != nullptr)
			{
				m_ppModel[cnt]->SetMainColor(D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f));		//��X�ώ��ɕύX
				m_ppModel[cnt]->SetSubColor(D3DXCOLOR(m_fRedAlpha, 0.0f, 0.0f, 0.0f));
				m_ppModel[cnt]->Draw();
			}
		}
	}

	//�}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//=================================
//�^�[�Q�b�g�ݒ�
//=================================
void CPicto::SetTargetObj(CObject * pObj)
{
	m_targetObj = pObj;		//�ړI�n�ݒ�
	Search();				//�o�H�T��
}

//=================================
//�^�[�Q�b�g����
//=================================
void CPicto::UnsetTargetObj(void)
{
	m_targetObj = GetAgit();	//�ړI�n���A�W�g�ɂ���
	m_state = STATE_LEAVE;		//�A����
	Search();					//�o�H�T��
}

//=================================
//���f���ݒ�
//=================================
void CPicto::SetModel(const char * pPath)
{
	//���[�V���������E������
	m_pMotion = new CMotion;
	m_pMotion->Init();

	//���[�V�����r���[�A�̃t�@�C����ǂݍ���
	LoadMotionViewerFile(pPath, &m_ppModel, m_pMotion, &m_nNumModel);

	if (m_ppModel != nullptr)
	{
		for (int cnt = 0; cnt < m_nNumModel - 2; cnt++)
		{//�̂̕����̂ݍs��
			m_ppModel[cnt]->SetChangeColor(true);
		}
	}

	//���[�V�����ݒ�
	m_pMotion->Set(0);
}

//=================================
//�t�B�[���h�̓����蔻��
//=================================
bool CPicto::CollisionField(D3DXVECTOR3* pPosNew)
{
	bool bLand = false;

	//�����擾
	float fLandHeight = CManager::FLOAT_ZERO;
	CScene::MODE mode = CManager::GetMode();
	if (mode == CScene::MODE_GAME)
	{//�Q�[��
		fLandHeight = CGame::GetMeshField()->GetHeight(*pPosNew);
	}
	else if (mode == CScene::MODE_TUTORIAL)
	{//�`���[�g���A��
		fLandHeight = CTutorial::GetMeshField()->GetHeight(*pPosNew);
	}

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
//�A��
//=================================
void CPicto::Return(void)
{
	Uninit();
}

//=================================
//�|�C���g��������
//=================================
void CPicto::Search(void)
{
	D3DXVECTOR3 posTarget = m_targetObj->GetPos();
	CPoint* pPointNear = nullptr;
	float fLenNear = 0.0f;
	float fRadNear = 0.0f;
	CPoint* pPoint = CPoint::GetTop();

	//���ݒn�ƌ����܂ł̊p�x���v�Z
	float fTargetLenWidth = posTarget.x - m_pos.x;
	float fTargetLenDepth = posTarget.z - m_pos.z;
	float fRadiusBuilding = atan2f(fTargetLenWidth, fTargetLenDepth);

	while (pPoint != nullptr)
	{//���X�g�I���܂ł��
		if (pPoint != m_pPoint)
		{//���ݐݒ肵�Ă���|�C���g�ł͂Ȃ�
			D3DXVECTOR3 vecPoint = pPoint->GetPos() - m_pos;

			bool bCollision = false;
			for (int cnt = 0; cnt < MAX_OBJ; cnt++)
			{
				CBuilding* pBuilding = CBuilding::GetBuilding(cnt);
				if (pBuilding != nullptr)
				{
					D3DXVECTOR3 posBuilding = pBuilding->GetPos();
					float fWidthHalf = pBuilding->GetWidth() * 0.5f;
					float fDepthHalf = pBuilding->GetDepth() * 0.5f;

					//4���_���
					D3DXVECTOR3 posBuild[4];
					posBuild[0] = posBuilding + D3DXVECTOR3(-fWidthHalf, 0.0f, -fDepthHalf);
					posBuild[1] = posBuilding + D3DXVECTOR3(fWidthHalf, 0.0f, -fDepthHalf);
					posBuild[2] = posBuilding + D3DXVECTOR3(fWidthHalf, 0.0f, fDepthHalf);
					posBuild[3] = posBuilding + D3DXVECTOR3(-fWidthHalf, 0.0f, fDepthHalf);

					//�v���X�}�C�i�X�����������̃t���b�O
					bool bPlus = false;
					bool bMinus = false;
					for (int cntPos = 0; cntPos < 4; cntPos++)
					{
						D3DXVECTOR3 vecLine = (posBuild[(cntPos + 1) % 4] - posBuild[cntPos]);
						D3DXVECTOR3 vecToPosOld = m_pos - posBuild[cntPos];
						D3DXVECTOR3 vecToPos = pPoint->GetPos() - posBuild[cntPos];
						if (TASUKIGAKE(vecLine.x, vecLine.z, vecToPosOld.x, vecToPosOld.z) >= 0.0f && TASUKIGAKE(vecLine.x, vecLine.z, vecToPos.x, vecToPos.z) < 0.0f)
						{//��������
							float fAreaA = (vecToPos.z * vecPoint.x) - (vecToPos.x * vecPoint.z);
							float fAreaB = (vecLine.z * vecPoint.x) - (vecLine.x * vecPoint.z);
							if (fAreaA / fAreaB >= 0.0f && fAreaA / fAreaB <= 1.0f)
							{//������
								bCollision = true;	//�Փ˂���
								break;	//�������������̂ŏI��
							}
						}
					}

					if (bCollision == true)
					{
						break;
					}
				}
			}

			if (bCollision == false)
			{//�������ĂȂ�
				float fLength = D3DXVec3Length(&vecPoint);	//�|�C���g�ƌ��ݒn�̋���
															//�|�C���g�ƌ��ݒn�̊p�x
				float fRadius = fabsf(acosf(D3DXVec3Dot(&vecPoint, &(posTarget - m_pos)) / (fLength * D3DXVec3Length(&(posTarget - m_pos)))));

				if (fLength > PICTO_POINT_RESEARCH_LENGTH + 2.0f)
				{//���������Ă��Ȃ��E�p�x��������
					if (pPointNear == nullptr)
					{
						pPointNear = pPoint;
						fLenNear = fLength;
						fRadNear = fRadius;
					}
					else if (fRadius < 0.5f * D3DX_PI && fRadius < fRadNear)
					{
						pPointNear = pPoint;
						fLenNear = fLength;
						fRadNear = fRadius;
					}
				}
			}
		}
		
		//���̃|�C���g��
		pPoint = pPoint->GetNext();
	}

	m_pPoint = pPointNear;	//�V�����|�C���g��ݒ�
}

//******************************************************
//�f�X�g���C���[�s�N�g�N���X
//******************************************************
//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CPictoDestroyer::CPictoDestroyer()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apPicto[cnt] == nullptr)
		{//�����
			m_apPicto[cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;	//ID�o����
			m_nNumAll++;	//�������₷
			break;
		}
	}
	m_nCounterDestruction = CManager::INT_ZERO;
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h�j
//=================================
CPictoDestroyer::CPictoDestroyer(const D3DXVECTOR3 pos, const TYPE type) : CPicto(pos,type)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apPicto[cnt] == nullptr)
		{//�����
			m_apPicto[cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;	//ID�o����
			m_nNumAll++;	//�������₷
			break;
		}
	}
	m_nCounterDestruction = CManager::INT_ZERO;
}

//=================================
//�f�X�g���N�^
//=================================
CPictoDestroyer::~CPictoDestroyer()
{
}

//========================
//����������
//========================
HRESULT CPictoDestroyer::Init(void)
{
	//�e����
	CPicto::Init();

	//���f���ݒ�
	CPicto::SetModel("data\\motion_exithuman_dest.txt");

	//���������Z����
	CScene::MODE mode = CManager::GetMode();
	CHaveNum** ppHaveNumObj = nullptr;
	if (mode == CScene::MODE_GAME)
	{//�Q�[��
		ppHaveNumObj = CGame::GetHaveNumObj();
	}
	else if (mode == CScene::MODE_TUTORIAL)
	{//�`���[�g���A��
		ppHaveNumObj = CTutorial::GetHaveNumObj();
	}

	//���[�_�[���Z
	ppHaveNumObj[TYPE_DESTROYER]->AddNum(-1);

	//��ʐl�A��Ă�
	int nAgitNormal = ppHaveNumObj[TYPE_NORMAL]->GetHaveNum();
	int nMaxNormal = PICTO_HAVENPICTO(m_nLv);

	if (nAgitNormal >= nMaxNormal)
	{//���݂̃��x���Ŏ��Ă��ʐl�̐l���ȏ�
		ppHaveNumObj[TYPE_NORMAL]->AddNum(-nMaxNormal);
		m_nHaveNormalPicto = nMaxNormal;
	}
	else
	{//����Ȃ����ǂƂ肠�������邾���A��Ă�
		ppHaveNumObj[TYPE_NORMAL]->AddNum(-nAgitNormal);
		m_nHaveNormalPicto = nAgitNormal;
	}

	//�̗͐ݒ�
	m_nLife = HAVE_LIFE(m_nLv);

	return S_OK;
}

//========================
//�I������
//========================
void CPictoDestroyer::Uninit(void)
{
	m_apPicto[m_nID] = nullptr;

	//�e����
	CPicto::Uninit();
}

//========================
//�X�V����
//========================
void CPictoDestroyer::Update(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	CMotion* pMotion = GetMotion();

	if (GetState() == STATE_ATTACK)
	{
		D3DXVECTOR3 targetPos = GetTargetObj()->GetPos();
		float fTargetLenWidth, fTargetLenDepth;
		float fTargetRot;

		fTargetLenWidth = targetPos.x - pos.x;
		fTargetLenDepth = targetPos.z - pos.z;

		fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);
		rot.y = FIX_ROT(fTargetRot + D3DX_PI);

		m_nCounterDestruction++;

		if (m_nCounterDestruction > PICTO_ATTACK_TIME)
		{
			//�����ւ̊p�x�v�Z
			float fBuildingAngle = atan2f(GetTargetObj()->GetHeight(), D3DXVec3Length(&(targetPos - pos)));

			//�e����
			CBulletBillboard::Create(GetPos() + D3DXVECTOR3(0.0f,30.0f,0.0f), rot + D3DXVECTOR3(-fBuildingAngle, 0.0f, 0.0f), 10.0f, 10.0f, 3.0f, PICTO_POWER(m_nLv,m_nHaveNormalPicto), TYPE_DESTROYER, this);

			//�j��J�E���^�[���Z�b�g
			m_nCounterDestruction = CManager::INT_ZERO;
		}

		if (pMotion->GetType() != MOTIONTYPE_DESTROY)
		{
			pMotion->Set(MOTIONTYPE_DESTROY);
		}
	}
	else
	{
		//�j��J�E���^�[���Z�b�g
		m_nCounterDestruction = CManager::INT_ZERO;
	}
	SetMove(CManager::VEC3_ZERO);
	SetRot(rot);

	//�e����
	CPicto::Update();
}

//========================
//�`�揈��
//========================
void CPictoDestroyer::Draw(void)
{
	//�e����
	CPicto::Draw();
}

//========================
//��������
//========================
CPictoDestroyer* CPictoDestroyer::Create(const D3DXVECTOR3 pos)
{
	CPictoDestroyer* pPicto = nullptr;

	if (pPicto == nullptr)
	{
		//�s�N�g�̐���
		pPicto = new CPictoDestroyer(pos);

		//������
		pPicto->Init();

		return pPicto;
	}
	else
	{
		return nullptr;
	}
}

//=================================
//�^�N�V�[���
//=================================
void CPictoDestroyer::TakeTaxi(CPictoTaxi * taxi)
{
	taxi->SetTakeTaxi(CPicto::TYPE_DESTROYER, 1);
	//�����ɘA��Ă����ʐl���悹�鏈��
}

//========================
//�_���[�W�t�^����
//========================
void CPictoDestroyer::AddDamage(int nDamage)
{
	m_nLife -= nDamage;	//�t�^

						//0�ɂȂ��������
	if (m_nLife <= CManager::INT_ZERO)
	{
		for (int cnt = 0; cnt < MAX_OBJ; cnt++)
		{//�S�I�u�W�F�N�g����
			CPictoBlocker* pPicto = CPictoBlocker::GetPicto(cnt);	//�I�u�W�F�N�g�擾

			if (pPicto != nullptr)	//�k���`�F
			{//�Ȃ񂩂���
				if (pPicto->GetTargetObj() == this)
				{//�������^�[�Q�b�g
					pPicto->UnsetTargetObj();	//�^�[�Q�b�g�O��
				}
			}
		}

		//���U
		Uninit();
	}

	//�Ԃ�����
	SetRedAlpha();
}


//=================================
//�o���l�擾�i���x���A�b�v�����܂ށj
//=================================
void CPictoDestroyer::AddExp(const int nExp)
{
	//�o���l���Z
	m_nExp += nExp;

	//���ʒ������烌�x���A�b�v
	while (REQUIRE_EXP(m_nLv + 1) <= m_nExp)
	{//�グ����
		m_nExp -= REQUIRE_EXP(m_nLv + 1);	//�����o���l���Z
		m_nLv++;							//���x���A�b�v

		//��ʂɔ��f
		CScene::MODE mode = CManager::GetMode();
		if (mode == CScene::MODE_GAME)
		{//�Q�[��
			CGame::GetLevelObj()[TYPE_DESTROYER]->SetNum(m_nLv);
		}
	}
}

//=================================
//�A��
//=================================
void CPictoDestroyer::Return(void)
{
	CScene::MODE mode = CManager::GetMode();

	if (mode == CScene::MODE_GAME)
	{//�Q�[��
		CGame::GetPlayer()->AddPicto(1, 0, m_nHaveNormalPicto);
	}
	else if (mode == CScene::MODE_TUTORIAL)
	{//�`���[�g���A��
		CTutorial::GetPlayer()->AddPicto(1, 0, m_nHaveNormalPicto);
	}

	CPicto::Return();
}

//******************************************************
//�u���b�J�[�s�N�g�N���X
//******************************************************
//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CPictoBlocker::CPictoBlocker()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apPicto[cnt] == nullptr)
		{//�����
			m_apPicto[cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;	//ID�o����
			m_nNumAll++;	//�������₷
			break;
		}
	}
	m_nCounterAttack = CManager::INT_ZERO;
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h�j
//=================================
CPictoBlocker::CPictoBlocker(const D3DXVECTOR3 pos, const TYPE type) : CPicto(pos,type)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apPicto[cnt] == nullptr)
		{//�����
			m_apPicto[cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;	//ID�o����
			m_nNumAll++;	//�������₷
			break;
		}
	}
	m_nCounterAttack = CManager::INT_ZERO;
}

//=================================
//�f�X�g���N�^
//=================================
CPictoBlocker::~CPictoBlocker()
{
}

//========================
//����������
//========================
HRESULT CPictoBlocker::Init(void)
{
	//�e����
	CPicto::Init();

	//���f���ݒ�
	CPicto::SetModel("data\\motion_exithuman_block.txt");

	//���������Z����
	CScene::MODE mode = CManager::GetMode();
	CHaveNum** ppHaveNumObj = nullptr;
	if (mode == CScene::MODE_GAME)
	{//�Q�[��
		ppHaveNumObj = CGame::GetHaveNumObj();
	}
	else if (mode == CScene::MODE_TUTORIAL)
	{//�`���[�g���A��
		ppHaveNumObj = CTutorial::GetHaveNumObj();
	}

	//���[�_�[���Z
	ppHaveNumObj[TYPE_BLOCKER]->AddNum(-1);

	//��ʐl�A��Ă�
	int nAgitNormal = ppHaveNumObj[TYPE_NORMAL]->GetHaveNum();
	int nMaxNormal = PICTO_HAVENPICTO(m_nLv);

	if (nAgitNormal >= nMaxNormal)
	{//���݂̃��x���Ŏ��Ă��ʐl�̐l���ȏ�
		ppHaveNumObj[TYPE_NORMAL]->AddNum(-nMaxNormal);
		m_nHaveNormalPicto = nMaxNormal;
	}
	else
	{//����Ȃ����ǂƂ肠�������邾���A��Ă�
		ppHaveNumObj[TYPE_NORMAL]->AddNum(-nAgitNormal);
		m_nHaveNormalPicto = nAgitNormal;
	}

	//�̗͐ݒ�
	m_nLife = HAVE_LIFE(m_nLv);

	return S_OK;
}

//========================
//�I������
//========================
void CPictoBlocker::Uninit(void)
{
	m_apPicto[m_nID] = nullptr;

	//�e����
	CPicto::Uninit();
}

//========================
//�X�V����
//========================
void CPictoBlocker::Update(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	CMotion* pMotion = GetMotion();

	if (GetState() == STATE_ATTACK)
	{
		D3DXVECTOR3 targetPos = GetTargetObj()->GetPos();
		float fTargetLenWidth, fTargetLenDepth;
		float fTargetRot;

		if (D3DXVec3Length(&(targetPos - pos)) > LOOSE_LENGTH)
		{//������
			UnsetTargetObj();
			if (pMotion->GetType() != MOTIONTYPE_NEUTRAL)
			{
				pMotion->Set(MOTIONTYPE_NEUTRAL);
			}
			SetState(STATE_LEAVE);
		}

		fTargetLenWidth = targetPos.x - pos.x;
		fTargetLenDepth = targetPos.z - pos.z;

		fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);
		rot.y = FIX_ROT(fTargetRot + D3DX_PI);

		m_nCounterAttack++;
		if (m_nCounterAttack > PICTO_ATTACK_TIME)
		{
			//�e����
			CBulletBillboard::Create(GetPos() + D3DXVECTOR3(0.0f, 30.0f, 0.0f), GetRot(), 10.0f, 10.0f, 10.0f, PICTO_POWER(m_nLv, m_nHaveNormalPicto), TYPE_BLOCKER, this);

			//�U���J�E���^�[���Z�b�g
			m_nCounterAttack = CManager::INT_ZERO;
		}

		if (pMotion->GetType() != MOTIONTYPE_ATTACK)
		{
			pMotion->Set(MOTIONTYPE_ATTACK);
		}
	}
	else
	{
		//�U���J�E���^�[���Z�b�g
		m_nCounterAttack = CManager::INT_ZERO;
	}
	SetMove(CManager::VEC3_ZERO);
	SetRot(rot);

	//�e����
	CPicto::Update();
}

//========================
//�`�揈��
//========================
void CPictoBlocker::Draw(void)
{
	//�e����
	CPicto::Draw();
}

//========================
//��������
//========================
CPictoBlocker* CPictoBlocker::Create(const D3DXVECTOR3 pos)
{
	CPictoBlocker* pPicto = nullptr;

	if (pPicto == nullptr)
	{
		//�s�N�g�̐���
		pPicto = new CPictoBlocker(pos);

		//������
		pPicto->Init();

		return pPicto;
	}
	else
	{
		return nullptr;
	}
}

//=================================
//�^�N�V�[���
//=================================
void CPictoBlocker::TakeTaxi(CPictoTaxi * taxi)
{
	taxi->SetTakeTaxi(CPicto::TYPE_BLOCKER, 1);
	//�����ɘA��Ă����ʐl���悹�鏈��
}

//========================
//�_���[�W�t�^����
//========================
void CPictoBlocker::AddDamage(int nDamage)
{
	m_nLife -= nDamage;	//�t�^

						//0�ɂȂ��������
	if (m_nLife <= CManager::INT_ZERO)
	{
		for (int cnt = 0; cnt < MAX_OBJ; cnt++)
		{//�S�I�u�W�F�N�g����
			CPictoBlocker* pPicto = CPictoBlocker::GetPicto(cnt);	//�I�u�W�F�N�g�擾

			if (pPicto != nullptr)	//�k���`�F
			{//�Ȃ񂩂���
				if (pPicto->GetTargetObj() == this)
				{//�������^�[�Q�b�g
					pPicto->UnsetTargetObj();	//�^�[�Q�b�g�O��
				}
			}
		}

		//���U
		Uninit();
	}

	//�Ԃ�����
	SetRedAlpha();
}

//=================================
//�o���l�擾�i���x���A�b�v�����܂ށj
//=================================
void CPictoBlocker::AddExp(const int nExp)
{
	//�o���l���Z
	m_nExp += nExp;

	//���ʒ������烌�x���A�b�v
	while (REQUIRE_EXP(m_nLv + 1) <= m_nExp)
	{//�グ����
		m_nExp -= REQUIRE_EXP(m_nLv + 1);	//�����o���l���Z
		m_nLv++;							//���x���A�b�v

		//��ʂɔ��f
		CScene::MODE mode = CManager::GetMode();
		if (mode == CScene::MODE_GAME)
		{//�Q�[��
			CGame::GetLevelObj()[TYPE_BLOCKER]->SetNum(m_nLv);
		}
	}
}

//=================================
//�A��
//=================================
void CPictoBlocker::Return(void)
{
	CScene::MODE mode = CManager::GetMode();

	if (mode == CScene::MODE_GAME)
	{//�Q�[��
		CGame::GetPlayer()->AddPicto(0, 1, m_nHaveNormalPicto);
	}
	else if (mode == CScene::MODE_TUTORIAL)
	{//�`���[�g���A��
		CTutorial::GetPlayer()->AddPicto(0, 1, m_nHaveNormalPicto);
	}

	CPicto::Return();
}

//******************************************************
//�s�N�^�N�V�[�N���X
//******************************************************
//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CPictoTaxi::CPictoTaxi()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apPicto[cnt] == nullptr)
		{//�����
			m_apPicto[cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;	//ID�o����
			m_nNumAll++;	//�������₷
			break;
		}
	}
	m_nTakeDestroyer = CManager::INT_ZERO;
	m_nTakeBlocker = CManager::INT_ZERO;
	m_nTakeNormal = CManager::INT_ZERO;
	m_nHaveBullet = CManager::INT_ZERO;
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h�j
//=================================
CPictoTaxi::CPictoTaxi(const D3DXVECTOR3 pos, const TYPE type) : CPicto(pos,type)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apPicto[cnt] == nullptr)
		{//�����
			m_apPicto[cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;	//ID�o����
			m_nNumAll++;	//�������₷
			break;
		}
	}
	m_nTakeDestroyer = CManager::INT_ZERO;
	m_nTakeBlocker = CManager::INT_ZERO;
	m_nTakeNormal = CManager::INT_ZERO;
	m_nHaveBullet = CManager::INT_ZERO;
}

//=================================
//�f�X�g���N�^
//=================================
CPictoTaxi::~CPictoTaxi()
{
}

//========================
//����������
//========================
HRESULT CPictoTaxi::Init(void)
{
	//�ݒ肳��Ă������[�h���擾
	m_mode = (MODE)CManager::GetScene()->GetSlider()->GetSelectIdx();

	//���[�h�ɂ���ĕς�邩��
	switch (m_mode)
	{
	case MODE_PICK:
		//�A�C�e���ޒT��
		SearchPick();
		break;
	case MODE_RESCUE:
		break;
	}
	
	//�e����
	CPicto::Init();

	//���f���ݒ�
	CPicto::SetModel("data\\motion_exithuman_taxi.txt");

	return S_OK;
}

//========================
//�I������
//========================
void CPictoTaxi::Uninit(void)
{
	m_apPicto[m_nID] = nullptr;

	//�e����
	CPicto::Uninit();
}

//========================
//�X�V����
//========================
void CPictoTaxi::Update(void)
{
	if (m_mode == MODE_SABO)
	{//�T�{��
		SetState(STATE_LEAVE);
		UnsetTargetObj();
	}

	if (GetState() == STATE_ATTACK)
	{//�ړI�n����
		switch (m_mode)
		{
		case MODE_PICK:
			if (m_pTargetPicto != nullptr)
			{
				//�^�N�V�[�ɏ悹��
				m_pTargetPicto->TakeTaxi(this);
				m_pTargetPicto->Uninit();
				m_pTargetPicto = nullptr;

				//�^�[�Q�b�g����
				UnsetTargetObj();
			}

			//���̃A�C�e���ޒT��
			if (SearchPick() == false)
			{//�Ȃ���Ȃ�
				if (GetMotion()->GetType() != MOTIONTYPE_NEUTRAL)
				{
					GetMotion()->Set(MOTIONTYPE_NEUTRAL);
				}
			}
			SetState(STATE_FACE);
			break;
		case MODE_RESCUE:
			//�^�[�Q�b�g�O��Ă�����T��
			if (m_pTargetPicto == nullptr)
			{
				m_pTargetPicto = SearchBattler();
			}

			if (m_pTargetPicto != nullptr)
			{
				//�^�N�V�[�ɏ悹��
				m_pTargetPicto->TakeTaxi(this);
				m_pTargetPicto->Uninit();
				m_pTargetPicto = nullptr;

				//�^�[�Q�b�g����
				UnsetTargetObj();
			}
			break;
		}
	}

	//�e����
	CPicto::Update();
}

//========================
//�`�揈��
//========================
void CPictoTaxi::Draw(void)
{
	//�e����
	CPicto::Draw();
}

//========================
//��������
//========================
CPictoTaxi* CPictoTaxi::Create(const D3DXVECTOR3 pos)
{
	CPictoTaxi* pPicto = nullptr;

	if (pPicto == nullptr)
	{
		//�s�N�g�̐���
		pPicto = new CPictoTaxi(pos);

		//������
		pPicto->Init();

		return pPicto;
	}
	else
	{
		return nullptr;
	}
}

//========================
//�^�N�V�[��ԏ���
//========================
void CPictoTaxi::SetTakeTaxi(const CPicto::TYPE type, const int nTakeNum)
{
	switch (type)
	{
	case TYPE_DESTROYER:	//�f�X�g���C���[
		m_nTakeDestroyer += nTakeNum;
		break;
	case TYPE_BLOCKER:		//�u���b�J�[
		m_nTakeBlocker += nTakeNum;
		break;
	case TYPE_NORMAL:		//��ʐl
		m_nTakeNormal += nTakeNum;
		break;
	}

	if (m_nTakeDestroyer + m_nTakeBlocker >= PICTO_FORCEDRETURN_NUM)
	{//�����A���
		SetState(STATE_LEAVE);
	}
}

//========================
//�A�C�e���ޒT��
//========================
bool CPictoTaxi::SearchPick(void)
{
	//�^�[�Q�b�g�O��Ă�����T��
	if (m_pTargetPicto == nullptr)
	{
		//�擾
		m_pTargetPicto = SearchNormal();

		//�����ɒe�ƃs�N�g�̋��������Ĕ�r���鏈��
		float fLengthPicto = -1.0f;
		float fLengthItem = -1.0f;
		
		if(m_pTargetPicto != nullptr)
		{//�s�N�g�擾����
			SetTargetObj(m_pTargetPicto);
		}
		else
		{//��
			return false;
		}

		return true;
	}

	return false;
}

//========================
//��ʐl�s�N�g�T��
//========================
CPictoNormal* CPictoTaxi::SearchNormal(void)
{
	CPictoNormal* pPictoNear = nullptr;
	float fNearLength;

	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//�S�I�u�W�F�N�g����
		CPictoNormal* pPicto = CPictoNormal::GetPicto(cnt);	//�I�u�W�F�N�g�擾

		if (pPicto != nullptr)	//�k���`�F
		{//�Ȃ񂩂���
			float fLength = D3DXVec3Length(&(pPicto->GetPos() - this->GetPos()));

			if (pPictoNear == nullptr || fLength < fNearLength)
			{//�߂�����������1�̂����m���
				fNearLength = fLength;
				pPictoNear = pPicto;
			}
		}
	}

	//�߂��s�N�g�̃|�C���^�Ԃ�
	return pPictoNear;
}

//========================
//�퓬�v���s�N�g�T��
//========================
CPicto* CPictoTaxi::SearchBattler(void)
{
	CPictoDestroyer* pPictoD = nullptr;
	CPictoBlocker* pPictoB = nullptr;
	int nLifeD,nLifeB;

	//�f�X�g���C���[�T��
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//�S�I�u�W�F�N�g����
		CPictoDestroyer* pPicto = CPictoDestroyer::GetPicto(cnt);	//�I�u�W�F�N�g�擾

		if (pPicto != nullptr)	//�k���`�F
		{//�Ȃ񂩂���
			int nLife = pPicto->GetLife();

			if ((((float)nLife / PICTO_LIFE) <= RESCUE_LIFE) && (pPictoD == nullptr || nLifeD > nLife))
			{//�~���Ώۂł���Ȃ����̗͂���ԏ��Ȃ�
				pPictoD = pPicto;
				nLifeD = nLife;
			}
		}
	}

	//�u���b�J�[�T��
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//�S�I�u�W�F�N�g����
		CPictoBlocker* pPicto = CPictoBlocker::GetPicto(cnt);	//�I�u�W�F�N�g�擾

		if (pPicto != nullptr)	//�k���`�F
		{//�Ȃ񂩂���
			int nLife = pPicto->GetLife();

			if ((((float)nLife / PICTO_LIFE) <= RESCUE_LIFE) && (pPictoB == nullptr || nLifeB > nLife))
			{//�~���Ώۂł���Ȃ����̗͂���ԏ��Ȃ�
				pPictoB = pPicto;
				nLifeB = nLife;
			}
		}
	}

	//�����������邩�ǂ���
	if (pPictoD != nullptr && pPictoB != nullptr)
	{//��������
		//�̗͏��Ȃ��ق���Ԃ�
		if (nLifeD <= nLifeB)
		{//�ǂ������̗͓����Ȃ�f�X�g���C���[�D��
			return pPictoD;
		}
		else
		{//�u���b�J�[
			return pPictoB;
		}
	}
	else if(pPictoD == nullptr && pPictoB != nullptr)
	{//�u���b�J�[�������Ȃ�
		return pPictoB;
	}
	else if (pPictoD != nullptr && pPictoB == nullptr)
	{//�f�X�g���C���[�������Ȃ�
		return pPictoD;
	}
	else
	{//���Ȃ�
		return nullptr;
	}
}

//========================
//�_���[�W�t�^����
//========================
void CPictoTaxi::AddDamage(int nDamage)
{
	m_nLife -= nDamage;	//�t�^

	//0�ɂȂ��������
	if (m_nLife <= CManager::INT_ZERO)
	{
		for (int cnt = 0; cnt < MAX_OBJ; cnt++)
		{//�S�I�u�W�F�N�g����
			CPictoBlocker* pPicto = CPictoBlocker::GetPicto(cnt);	//�I�u�W�F�N�g�擾

			if (pPicto != nullptr)	//�k���`�F
			{//�Ȃ񂩂���
				if (pPicto->GetTargetObj() == this)
				{//�������^�[�Q�b�g
					pPicto->UnsetTargetObj();	//�^�[�Q�b�g�O��
				}
			}
		}

		//���U
		Uninit();
	}

	//�Ԃ�����
	SetRedAlpha();
}

//=================================
//�A��
//=================================
void CPictoTaxi::Return(void)
{
	//�퓬�v���ǉ�����
	CScene::MODE mode = CManager::GetMode();
	CPlayer* pPlayer = nullptr;
	if (mode == CScene::MODE_GAME)
	{//�Q�[��
		pPlayer = CGame::GetPlayer();
	}
	else if (mode == CScene::MODE_TUTORIAL)
	{//�`���[�g���A��
		pPlayer = CTutorial::GetPlayer();
	}
	pPlayer->AddPicto(m_nTakeDestroyer, m_nTakeBlocker, m_nTakeNormal);

	CPicto::Return();
}


//******************************************************
//��ʐl�s�N�g�N���X
//******************************************************
//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CPictoNormal::CPictoNormal()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apPicto[cnt] == nullptr)
		{//�����
			m_apPicto[cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;	//ID�o����
			m_nNumAll++;	//�������₷
			break;
		}
	}
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h�j
//=================================
CPictoNormal::CPictoNormal(const D3DXVECTOR3 pos, const TYPE type) : CPicto(pos,type)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apPicto[cnt] == nullptr)
		{//�����
			m_apPicto[cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;	//ID�o����
			m_nNumAll++;	//�������₷
			break;
		}
	}
}

//=================================
//�f�X�g���N�^
//=================================
CPictoNormal::~CPictoNormal()
{
}

//========================
//����������
//========================
HRESULT CPictoNormal::Init(void)
{
	//�e����
	CPicto::Init();

	//���f���ݒ�
	CPicto::SetModel("data\\motion_exithuman_nor.txt");

	return S_OK;
}

//========================
//�I������
//========================
void CPictoNormal::Uninit(void)
{
	m_apPicto[m_nID] = nullptr;

	//�e����
	CPicto::Uninit();
}

//========================
//�X�V����
//========================
void CPictoNormal::Update(void)
{
	//�e����
	CPicto::Update();
}

//========================
//�`�揈��
//========================
void CPictoNormal::Draw(void)
{
	//�e����
	CPicto::Draw();
}

//========================
//��������
//========================
CPictoNormal* CPictoNormal::Create(const D3DXVECTOR3 pos)
{
	CPictoNormal* pPicto = nullptr;

	if (pPicto == nullptr)
	{
		//�s�N�g�̐���
		pPicto = new CPictoNormal(pos);

		//������
		pPicto->Init();

		return pPicto;
	}
	else
	{
		return nullptr;
	}
}

//========================
//�^�N�V�[�ɏ�鏈��
//========================
void CPictoNormal::TakeTaxi(CPictoTaxi* taxi)
{
	//���I
	int nRand = rand() % (NORMAL_D_PERCENT + NORMAL_B_PERCENT + NORMAL_N_PERCENT);

	//����
	if (nRand < NORMAL_D_PERCENT)
	{//�f�X�g���C���[
		taxi->SetTakeTaxi(CPicto::TYPE_DESTROYER, 1);
	}
	else if (nRand < NORMAL_D_PERCENT + NORMAL_B_PERCENT)
	{//�u���b�J�[
		taxi->SetTakeTaxi(CPicto::TYPE_BLOCKER, 1);
	}
	else
	{//��ʐl�s�N�g
		int nPictoNum = rand() % (NORMAL_NUM_DEGREE + 1) + NORMAL_NUM_MIN;	//�����Ől�����܂�
		taxi->SetTakeTaxi(CPicto::TYPE_NORMAL, nPictoNum);
	}
}

//=================================
//�A��
//=================================
void CPictoNormal::Return(void)
{
	CPicto::Return();
}

//******************************************************
//�s�N�g�x�@�N���X
//******************************************************
//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CPictoPolice::CPictoPolice()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apPicto[cnt] == nullptr)
		{//�����
			m_apPicto[cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;	//ID�o����
			m_nNumAll++;	//�������₷
			break;
		}
	}
	m_nCounterAttack = CManager::INT_ZERO;
	m_pTargetPicto = nullptr;
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h�j
//=================================
CPictoPolice::CPictoPolice(const D3DXVECTOR3 pos, const TYPE type) : CPicto(pos,type)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apPicto[cnt] == nullptr)
		{//�����
			m_apPicto[cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;	//ID�o����
			m_nNumAll++;	//�������₷
			break;
		}
	}
	m_nCounterAttack = CManager::INT_ZERO;
	m_pTargetPicto = nullptr;
}

//=================================
//�f�X�g���N�^
//=================================
CPictoPolice::~CPictoPolice()
{
	
}

//========================
//����������
//========================
HRESULT CPictoPolice::Init(void)
{
	//�e����
	CPicto::Init();

	//���f���ݒ�
	CPicto::SetModel("data\\motion_exithuman_police.txt");

	//�̗͐ݒ�
	m_nLife = HAVE_LIFE(m_nLv);

	return S_OK;
}

//========================
//�I������
//========================
void CPictoPolice::Uninit(void)
{
	m_apPicto[m_nID] = nullptr;
	m_nNumAll--;

	//�e����
	CPicto::Uninit();
}

//========================
//�X�V����
//========================
void CPictoPolice::Update(void)
{
	D3DXVECTOR3 targetPos = CManager::VEC3_ZERO;
	float targetWidthHalf = CManager::FLOAT_ZERO;
	float targetDepthHalf = CManager::FLOAT_ZERO;
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();
	CMotion* pMotion = GetMotion();

	move.x = CManager::FLOAT_ZERO;
	move.z = CManager::FLOAT_ZERO;

	if (m_pTargetPicto != nullptr)
	{//�_�����߂Ă���
		SetState(STATE_ATTACK);	//�U������C�}���}��

		targetPos = m_pTargetPicto->GetPos();
		targetWidthHalf = m_pTargetPicto->GetWidth() * 0.5f;
		targetDepthHalf = m_pTargetPicto->GetDepth() * 0.5f;

		if (targetPos.x - targetWidthHalf - PICTO_POLICE_STOP_LENGTH > pos.x || targetPos.x + targetWidthHalf + PICTO_POLICE_STOP_LENGTH < pos.x ||
			targetPos.z - targetDepthHalf - PICTO_POLICE_STOP_LENGTH > pos.z || targetPos.z + targetDepthHalf + PICTO_POLICE_STOP_LENGTH < pos.z)
		{//�������ĂȂ�
			float fTargetLenWidth, fTargetLenDepth;
			float fTargetRot;

			fTargetLenWidth = targetPos.x - pos.x;
			fTargetLenDepth = targetPos.z - pos.z;

			fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);

			move.x = sinf(fTargetRot) * PICTO_WALK_SPEED;
			move.z = cosf(fTargetRot) * PICTO_WALK_SPEED;

			rot.y = FIX_ROT(fTargetRot + D3DX_PI);

			if (pMotion->GetType() != MOTIONTYPE_MOVE)
			{
				pMotion->Set(MOTIONTYPE_MOVE);
			}

			//�U���J�E���^�[���Z�b�g
			m_nCounterAttack = CManager::INT_ZERO;
		}
		else
		{//����
			//��������
			float fTargetLenWidth, fTargetLenDepth;
			fTargetLenWidth = targetPos.x - pos.x;
			fTargetLenDepth = targetPos.z - pos.z;

			float fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);
			rot.y = FIX_ROT(fTargetRot + D3DX_PI);

			m_nCounterAttack++;
			if (m_nCounterAttack > PICTO_ATTACK_TIME)
			{
				//�e����
				CBulletBillboard::Create(GetPos() + D3DXVECTOR3(0.0f, 30.0f, 0.0f), GetRot(), 10.0f, 10.0f, 10.0f, PICTO_POWER(m_nLv, m_nHaveNormalPicto), TYPE_POLICE, this);

				//�U���J�E���^�[���Z�b�g
				m_nCounterAttack = CManager::INT_ZERO;
			}

			if (pMotion->GetType() != MOTIONTYPE_ATTACK)
			{
				pMotion->Set(MOTIONTYPE_ATTACK);
			}
		}

		//�ʒu�ړ��ʐݒ�
		SetRot(rot);
		SetMove(move);
	}
	else
	{//������Ȃ�
		move.x = CManager::FLOAT_ZERO;
		move.z = CManager::FLOAT_ZERO;
		for (int cnt = 0; cnt < MAX_OBJ; cnt++)
		{//�S�I�u�W�F�N�g����
			CPicto* pPicto = CPicto::GetPicto(cnt);	//�I�u�W�F�N�g�擾

			if (pPicto != nullptr)
			{
				CPicto::TYPE type = pPicto->GetType();
				if (pPicto != this && type != TYPE_NORMAL && type != TYPE_POLICE)
				{
					if (D3DXVec3Length(&(pPicto->GetPos() - this->GetPos())) <= PICTO_POLICE_SEARCH_LENGTH)
					{
						m_pTargetPicto = pPicto;
					}
				}
			}
		}
		if (GetState() == STATE_ATTACK && pMotion->GetType() != MOTIONTYPE_NEUTRAL)
		{
			pMotion->Set(MOTIONTYPE_NEUTRAL);
		}
		//�ʒu�ړ��ʐݒ�
		SetRot(rot);
		SetMove(move);
	}

	//�e����
	CPicto::Update();
}

//========================
//�`�揈��
//========================
void CPictoPolice::Draw(void)
{
	//�e����
	CPicto::Draw();
}

//========================
//��������
//========================
CPictoPolice* CPictoPolice::Create(const D3DXVECTOR3 pos)
{
	CPictoPolice* pPicto = nullptr;

	if (pPicto == nullptr)
	{
		//�s�N�g�̐���
		pPicto = new CPictoPolice(pos);

		//������
		pPicto->Init();

		return pPicto;
	}
	else
	{
		return nullptr;
	}
}

//========================
//�_���[�W�t�^����
//========================
void CPictoPolice::AddDamage(int nDamage)
{
	m_nLife -= nDamage;	//�t�^

	//0�ɂȂ��������
	if (m_nLife <= CManager::INT_ZERO)
	{
		for (int cnt = 0; cnt < MAX_OBJ; cnt++)
		{//�S�I�u�W�F�N�g����
			CPictoBlocker* pPicto = CPictoBlocker::GetPicto(cnt);	//�I�u�W�F�N�g�擾

			if (pPicto != nullptr)	//�k���`�F
			{//�Ȃ񂩂���
				if (pPicto->GetTargetObj() == this)
				{//�������^�[�Q�b�g
					pPicto->UnsetTargetObj();	//�^�[�Q�b�g�O��
				}
			}
		}

		//�o���l�t�^
		CPictoBlocker::AddExp(DROP_EXP(9));	//�������񃌃x��9�Ƃ��Ĉ���

		//���U
		Uninit();
	}

	//�Ԃ�����
	SetRedAlpha();
}

//=================================
//�A��
//=================================
void CPictoPolice::Return(void)
{
	CPicto::Return();
}
