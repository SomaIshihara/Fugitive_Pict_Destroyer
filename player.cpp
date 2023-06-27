//======================================================
//
//�v���C���[����[player.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "player.h"
#include "manager.h"
#include "renderer.h"
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
//�N���b�N�������W���烏�[���h���W�ɕϊ�
//=================================
D3DXVECTOR3 CPlayer::ConvertClickPosToWorld(float fZ)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//�f�o�C�X�̎擾
	CCamera* pCamera = CManager::GetCamera();
	CInputMouse* pMouse = CManager::GetInputMouse();					//�}�E�X�擾

	D3DXMATRIX mtxView, mtxProj;
	D3DXMATRIX mtxViewPort;
	D3DXMATRIX mtx;
	D3DXVECTOR3 posClick = pMouse->GetPos();
	posClick.z = fZ;	//Z���W�w��
	D3DXVECTOR3 posWorld;

	//�r���[�}�g���擾�E�t�s��
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	D3DXMatrixInverse(&mtxView, NULL, &mtxView);

	//�v���W�F�N�V�����}�g���擾�E�t�s��
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);
	D3DXMatrixInverse(&mtxProj, NULL, &mtxProj);

	//�r���[�|�[�g�}�g���ݒ�E�t�s��
	D3DXMatrixIdentity(&mtxViewPort);
	mtxViewPort._11 = SCREEN_WIDTH * 0.5;
	mtxViewPort._22 = -SCREEN_HEIGHT * 0.5;
	mtxViewPort._41 = SCREEN_WIDTH * 0.5;
	mtxViewPort._42 = SCREEN_HEIGHT * 0.5;
	D3DXMatrixInverse(&mtxViewPort, NULL, &mtxViewPort);

	//�S���|����
	mtx = mtxViewPort * mtxProj * mtxView;	//������D3DXMatrixMultiply����Ă�݂���

	//����Ń��[���h���W�ɕϊ��ł���
	D3DXVec3TransformCoord(&posWorld, &posClick, &mtx);

	return posWorld;
}

//=================================
//�I��
//=================================
void CPlayer::Select(void)
{
	//�I�u�W�F�N�g�I���i0.0�`1.0�j
	D3DXVECTOR3 posNear = ConvertClickPosToWorld(0.0f);
	D3DXVECTOR3 posFar = ConvertClickPosToWorld(1.0f);
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//�S�I�u�W�F�N�g����
		CBuilding* pBuilding = CBuilding::GetBuilding(cnt);	//�I�u�W�F�N�g�擾

		if (pBuilding != NULL)	//�k���`�F
		{//�Ȃ񂩂���
			if (CObjectX::GetModel(pBuilding->GetModelIdx())->m_collision.CollisionCheck(posNear, posFar, pBuilding->GetPos(), pBuilding->GetRot()) == true)
			{//�����������
				CPict* pPict = CPict::GetPict(0);	//�I�u�W�F�N�g�擾
				pPict->SetTarget(pBuilding);
				return;
			}
		}
	}

	////�s�N�g����ړ������i0.0�`0.5�j
	//posNear = ConvertClickPosToWorld(0.0f);
	//posFar = ConvertClickPosToWorld(0.2f);
	//D3DXVECTOR3 posPlane = VEC3_ZERO;
	//D3DXVECTOR3 vecNorPlane = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//D3DXVECTOR3 posSelect;
	//D3DXPLANE plane;

	//D3DXPlaneFromPointNormal(&plane, &posPlane, &vecNorPlane);
	//D3DXPlaneIntersectLine(&posSelect, &plane, &posNear, &posFar);

	//if (D3DXVec3Length(&posSelect) <= PICT_MOVE_LENGTH)
	//{
	//	CPict* pPict = CPict::GetPict(0);	//�I�u�W�F�N�g�擾

	//	if (pPict != NULL)	//�k���`�F
	//	{//�s�N�g����
	//		pPict->SetTargetPos(posSelect);
	//	}
	//}
}
