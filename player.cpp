//======================================================
//
//�v���C���[����[player.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "object.h"
#include "input.h"
#include "camera.h"
#include "objectX.h"
#include "pict.h"
#include "building.h"

#define CAMERA_MOVE_SPEED		(10.0f)		//�J�����ړ����x
#define CAMERA_MOU_ROT_SPEED	(0.0012f)	//�}�E�X�ړ��ł̉�]���x
#define PICT_MOVE_LENGTH		(1000.0f)	//�s�N�g����ړ������I�𔼌a

//=================================
//�R���X�g���N�^
//=================================
CPlayer::CPlayer()
{
	m_nChacePictNum = -1;
	m_bControllPict = false;
}

//=================================
//�f�X�g���N�^
//=================================
CPlayer::~CPlayer()
{}

//=================================
//������
//=================================
HRESULT CPlayer::Init(void)
{
	m_nChacePictNum = -1;
	m_bControllPict = false;
	return S_OK;
}

//=================================
//�I��
//=================================
void CPlayer::Uninit(void)
{
	
}

//=================================
//�X�V
//=================================
void CPlayer::Update(void)
{
	CInputMouse* pMouse = CManager::GetInputMouse();	//�}�E�X�擾
	Move();

	if (pMouse->GetPress(MOUSE_CLICK_RIGHT) == true)
	{//��]
		Rotate();
		//�J�[�\�������̈ʒu�ɖ߂�
		POINT setCursorpos;
		setCursorpos.x = SCREEN_WIDTH / 2;
		setCursorpos.y = SCREEN_HEIGHT / 2;
		ClientToScreen(FindWindowA(CLASS_NAME, nullptr), &setCursorpos);
		SetCursorPos(setCursorpos.x, setCursorpos.y);
	}
	if (pMouse->GetPress(MOUSE_CLICK_LEFT) == true)
	{//�ʒu����
		Select();
	}

	//�f�o�b�O
	if (m_pSelectBuilding != NULL)
	{
		CManager::GetDebProc()->Print("[BUILDING]\n");
	}
	if (m_pSelectPict != NULL)
	{
		CManager::GetDebProc()->Print("[PICT]\n");
	}
}

//=================================
//�U��
//=================================
void CPlayer::Attack(void)
{
	if (m_pSelectBuilding != NULL)
	{//�������I������Ă���
		CPictDestroyer::GetPict(0)->SetTarget(m_pSelectBuilding);
	}
	else if (m_pSelectPict != NULL)
	{//�s�N�g�i�Ȃ�ł��j���I������Ă���
		//�x�@���ׂ�
		for (int cnt = 0; cnt < MAX_OBJ; cnt++)
		{//�S�I�u�W�F�N�g����
			CPictPolice* pPict = CPictPolice::GetPict(cnt);	//�I�u�W�F�N�g�擾

			if (m_pSelectPict == pPict)
			{//�I�����Ă���s�N�g����ƌx�@���X�g�̃|�C���^����v
				CPictBlocker::GetPict(0)->SetTarget(pPict);
			}
		}
	}
}

//=================================
//�ړ�
//=================================
void CPlayer::Move(void)
{
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();	//�L�[�{�[�h�擾
	CCamera* pCamera = CManager::GetCamera();					//�J�����擾
	D3DXVECTOR3 move = VEC3_ZERO;
	D3DXVECTOR3 rot = pCamera->GetRot();
	//�ړ�
	if (pKeyboard->GetPress(DIK_A) == true)
	{
		move.x += -cosf(rot.y) * CAMERA_MOVE_SPEED;
		move.z += -sinf(rot.y) * CAMERA_MOVE_SPEED;
	}
	else if (pKeyboard->GetPress(DIK_D) == true)
	{
		move.x += cosf(rot.y) * CAMERA_MOVE_SPEED;
		move.z += sinf(rot.y) * CAMERA_MOVE_SPEED;
	}

	if (pKeyboard->GetPress(DIK_W) == true)
	{
		move.x += -sinf(rot.y) * CAMERA_MOVE_SPEED;
		move.z += cosf(rot.y) * CAMERA_MOVE_SPEED;
	}
	else if (pKeyboard->GetPress(DIK_S) == true)
	{
		move.x += sinf(rot.y) * CAMERA_MOVE_SPEED;
		move.z += -cosf(rot.y) * CAMERA_MOVE_SPEED;
	}

	if (m_bControllPict == true)
	{//���c�ݒ�
		CPict::GetPict(0)->Controll(move);
	}
	else if (m_nChacePictNum != -1)
	{//�Ǐ]�ݒ�
		move = CPict::GetPict(m_nChacePictNum)->GetMove();
	}

	//�ړ�
	pCamera->SetCameraPos(move);

	//�s�N�g���񑀏c�ݒ�
	if (pKeyboard->GetTrigger(DIK_H) == true)
	{
		CPict::GetPict(0)->SetControll();
		m_bControllPict = true;
	}
	else if (pKeyboard->GetTrigger(DIK_J) == true)
	{
		CPict::GetPict(0)->Uncontroll();
		m_bControllPict = false;
	}
}

//=================================
//��]
//=================================
void CPlayer::Rotate(void)
{
	CCamera* pCamera = CManager::GetCamera();
	CInputMouse* pMouse = CManager::GetInputMouse();
	D3DXVECTOR3 rot = VEC3_ZERO;

	rot.y -= pMouse->GetMove().x * CAMERA_MOU_ROT_SPEED;
	rot.x -= pMouse->GetMove().y * CAMERA_MOU_ROT_SPEED;

	pCamera->SetCameraRot(rot);
}

//=================================
//�I��
//=================================
void CPlayer::Select(void)
{
	CInputMouse* mouse = CManager::GetInputMouse();	//�}�E�X�擾

	//�I�u�W�F�N�g�I���i0.0�`1.0�j
	D3DXVECTOR3 posNear = mouse->ConvertClickPosToWorld(0.0f);
	D3DXVECTOR3 posFar = mouse->ConvertClickPosToWorld(1.0f);

	//����
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//�S�I�u�W�F�N�g����
		CBuilding* pBuilding = CBuilding::GetBuilding(cnt);	//�I�u�W�F�N�g�擾

		if (pBuilding != NULL)	//�k���`�F
		{//�Ȃ񂩂���
			if (CObjectX::GetModel(pBuilding->GetModelIdx())->m_collision.CollisionCheck(posNear, posFar, pBuilding->GetPos(), pBuilding->GetRot()) == true)
			{//�����I��
				m_pSelectPict = NULL;
				m_pSelectBuilding = pBuilding;
				return;
			}
		}
	}

	//�s�N�g����
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//�S�I�u�W�F�N�g����
		CPict* pPict = CPict::GetPict(cnt);	//�I�u�W�F�N�g�擾

		if (pPict != NULL)	//�k���`�F
		{//�Ȃ񂩂���
			if (pPict->GetCollision().CollisionCheck(posNear, posFar, pPict->GetPos(), pPict->GetRot()) == true)
			{//�s�N�g�I��
				m_pSelectBuilding = NULL;
				m_pSelectPict = pPict;
				return;
			}
		}
	}

	//�����I�����Ă��Ȃ��̂őI������
	m_pSelectBuilding = NULL;
	m_pSelectPict = NULL;
}
