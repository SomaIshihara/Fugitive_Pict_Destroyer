//======================================================
//
//�v���C���[����[player.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "player.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "objectX.h"
#include "pict.h"
#include "building.h"
#include "button.h"
#include "slider.h"
#include "building.h"
#include "xmodel.h"

//=================================
//�R���X�g���N�^
//=================================
CPlayer::CPlayer()
{
	m_bControllPict = false;

	m_nHaveDestroyer = CManager::INT_ZERO;
	m_nHaveBlocker = CManager::INT_ZERO;
	m_nHaveNormal = CManager::INT_ZERO;

	m_cursorPos = CManager::VEC3_ZERO;
	m_pButtonATK = NULL;
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
	m_bControllPict = false;

	//�X���C�_�[�����ݒ�
	CGame::GetSlider()->SetSelectIdx(CPictTaxi::MODE_SABO);

	//���F�l���ݒ�
	m_nHaveDestroyer = 1;
	m_nHaveBlocker = 2;
	m_nHaveNormal = 1500;

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
	}
	else
	{//�}�E�X�J�[�\���ʒu�ۑ�
		m_cursorPos = pMouse->GetPos();
	}
	//�{�^���������ꂽ�����m
	if (m_pButtonATK != NULL && m_pButtonATK->IsClickTrigger() == true)
	{
		Attack();
		m_pButtonATK->Uninit();
		m_pButtonATK = NULL;
	}
	else if (pMouse->GetTrigger(MOUSE_CLICK_LEFT) == true)
	{//�ʒu����
		Select();
	}

	CSlider* slider = CGame::GetSlider();
	slider->SetSelectIdx(slider->GetSelectIdx() - (pMouse->GetWheel() / 120));

	//�^�N�V�[���[�h
	CPictTaxi* pTaxi = CPictTaxi::GetPict(0);
	int nIdxSlider = CGame::GetSlider()->GetSelectIdx();

	//�������Č����Ă�̂Ƀ^�N�V�[���Ȃ�
	if (nIdxSlider != CPictTaxi::MODE_SABO && pTaxi == NULL)
	{//��������\�o��
		CPictTaxi::Create(CPict::GetAgitPos());
	}

	if (pTaxi != NULL)
	{//�^�N�V�[����
		pTaxi->SetMode((CPictTaxi::MODE)nIdxSlider);
	}
}

//=================================
//�U��
//=================================
void CPlayer::Attack(void)
{
	if (m_pSelectBuilding != NULL)
	{//�������I������Ă���
		CPictDestroyer* pict = CPictDestroyer::Create(CPict::GetAgitPos());
		pict->SetTargetObj((CObject*)m_pSelectBuilding);
		pict->SetState(CPict::STATE_FACE);
	}
	else if (m_pSelectPict != NULL)
	{//�s�N�g�i�Ȃ�ł��j���I������Ă���
		//�x�@���ׂ�
		for (int cnt = 0; cnt < MAX_OBJ; cnt++)
		{//�S�I�u�W�F�N�g����
			CPictPolice* pPict = CPictPolice::GetPict(cnt);	//�I�u�W�F�N�g�擾

			if (m_pSelectPict == pPict)
			{//�I�����Ă���s�N�g����ƌx�@���X�g�̃|�C���^����v
				CPictBlocker::Create(CPict::GetAgitPos())->SetTargetObj(pPict);
			}
		}
	}
}

//=================================
//���e
//=================================
void CPlayer::AddPict(const int nDestroyer, const int nBlocker, const int nNormal)
{//�ǉ�
	m_nHaveDestroyer += nDestroyer;
	m_nHaveBlocker += nBlocker;
	m_nHaveNormal += nNormal;
}

//=================================
//�ړ�
//=================================
void CPlayer::Move(void)
{
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();	//�L�[�{�[�h�擾
	CCamera* pCamera = CManager::GetCamera();					//�J�����擾
	D3DXVECTOR3 move = CManager::VEC3_ZERO;
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
	D3DXVECTOR3 rot = CManager::VEC3_ZERO;

	D3DXVECTOR3 move = CManager::VEC3_ZERO;
	move.x = pMouse->GetMove().x;
	move.y = pMouse->GetMove().y;

	rot.y -= move.x * CAMERA_MOU_ROT_SPEED;
	rot.x -= move.y * CAMERA_MOU_ROT_SPEED;

	pCamera->SetCameraRot(rot);

	//�J�[�\�������̈ʒu�ɖ߂�
	POINT setCursorpos;
	setCursorpos.x = m_cursorPos.x;
	setCursorpos.y = m_cursorPos.y;
	ClientToScreen(FindWindowA(CLASS_NAME, nullptr), &setCursorpos);
	SetCursorPos(setCursorpos.x, setCursorpos.y);
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

	//�{�^���폜
	if (m_pButtonATK != NULL)
	{
		m_pButtonATK->Uninit();
		m_pButtonATK = NULL;
	}

	//����
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//�S�I�u�W�F�N�g����
		CBuilding* pBuilding = CBuilding::GetBuilding(cnt);	//�I�u�W�F�N�g�擾

		if (pBuilding != NULL)	//�k���`�F
		{//�Ȃ񂩂���
			if (pBuilding->GetModel()->GetCollision().CollisionCheck(posNear, posFar, pBuilding->GetPos(), pBuilding->GetRot()) == true &&
				pBuilding->GetEndurance() > 0)
			{//�����I��
				m_pSelectPict = NULL;
				m_pSelectBuilding = pBuilding;
				
				//�{�^������
				m_pButtonATK = CButton2D::Create(mouse->GetPos(), CManager::VEC3_ZERO, 40.0f, 40.0f);
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

				//�{�^������
				m_pButtonATK = CButton2D::Create(mouse->GetPos(), CManager::VEC3_ZERO, 40.0f, 40.0f);
				return;
			}
		}
	}

	//�����I�����Ă��Ȃ��̂őI������
	m_pSelectBuilding = NULL;
	m_pSelectPict = NULL;
}
