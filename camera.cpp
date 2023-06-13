//==========================================
//
//�J�����֌W�v���O����[camera.cpp]
//Author:�Ό��D�n
//
//==========================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "Culc.h"

//�}�N��
#define CAMERA_MOVE_SPEED		(1.0f)		//�J�����ړ����x
#define CAMERA_KEY_ROT_SPEED	(0.005f)	//�L�[�{�[�h���͂ł̉�]���x
#define CAMERA_MOU_ROT_SPEED	(0.0012f)	//�}�E�X�ړ��ł̉�]���x
#define CAMERA_LENGTH			(1000.0f)	//�J������������ő勗��
#define CAMERA_ROT_X_MIN		(-0.45f)	//�J������X�p�x�̍Œ�l[rad]
#define CAMERA_ROT_X_MAX		(-0.2f)	//�J������X�p�x�̍Œ�l[rad]

//========================
//�R���X�g���N�^
//========================
CCamera::CCamera()
{
	//�l�N���A
	m_posV = VEC3_ZERO;
	m_posR = VEC3_ZERO;
	m_vecU = VEC3_ZERO;
	m_rot = VEC3_ZERO;
}

//========================
//�f�X�g���N�^
//========================
CCamera::~CCamera()
{

}

//========================
//����������
//========================
HRESULT CCamera::Init(void)
{
	//�l�ݒ�
	m_posV = D3DXVECTOR3(0.0f,50.0f,-100.0f);
	m_posR = VEC3_ZERO;
	m_vecU = D3DXVECTOR3(0.0f,1.0f,0.0f);
	m_rot = D3DXVECTOR3(-0.5f,0.0f,0.0f);
	FixRot();
	FixPosV();

	//�ł���
	return S_OK;
}

//========================
//�I������
//========================
void CCamera::Uninit(void)
{
	
}

//========================
//�X�V����
//========================
void CCamera::Update(void)
{
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();
	CInputMouse* pMouse = CManager::GetInputMouse();
#if 1
	if (pMouse->GetPress(MOUSE_CLICK_LEFT) == true)
	{
		//���_
		m_rot.y -= pMouse->GetMove().x * CAMERA_MOU_ROT_SPEED;
		m_rot.x -= pMouse->GetMove().y * CAMERA_MOU_ROT_SPEED;
		FixRot();
		m_vecU.x = sinf(m_rot.x) * sinf(m_rot.y);
		m_vecU.y = cosf(m_rot.x);
		m_vecU.z = sinf(m_rot.x) * cosf(m_rot.y) * -1;
		D3DXVec3Normalize(&m_vecU, &m_vecU);
		FixPosV();
	}
#endif

#if 0
	//�����_
	if (pKeyboard->GetPress(DIK_Q) == true)
	{//-
		m_rot.y -= CAMERA_KEY_ROT_SPEED * D3DX_PI;
		FixRot();
		FixPosR();
	}
	else if (pKeyboard->GetPress(DIK_E) == true)
	{//+
		m_rot.y += CAMERA_KEY_ROT_SPEED * D3DX_PI;
		FixRot();
		FixPosR();
	}

	//���_
	if (pKeyboard->GetPress(DIK_Z) == true)
	{//-
		m_rot.y += CAMERA_KEY_ROT_SPEED * D3DX_PI;
		FixRot();
		FixPosV();
	}
	else if (pKeyboard->GetPress(DIK_C) == true)
	{//+
		m_rot.y -= CAMERA_KEY_ROT_SPEED * D3DX_PI;
		FixRot();
		FixPosV();
	}

	//�ړ�
	if (pKeyboard->GetPress(DIK_A) == true)
	{
		m_posV.x += -cosf(m_rot.y) * CAMERA_MOVE_SPEED;
		m_posR.x += -cosf(m_rot.y) * CAMERA_MOVE_SPEED;
		m_posV.z += -sinf(m_rot.y) * CAMERA_MOVE_SPEED;
		m_posR.z += -sinf(m_rot.y) * CAMERA_MOVE_SPEED;
	}
	else if (pKeyboard->GetPress(DIK_D) == true)
	{
		m_posV.x += cosf(m_rot.y) * CAMERA_MOVE_SPEED;
		m_posR.x += cosf(m_rot.y) * CAMERA_MOVE_SPEED;
		m_posV.z += sinf(m_rot.y) * CAMERA_MOVE_SPEED;
		m_posR.z += sinf(m_rot.y) * CAMERA_MOVE_SPEED;
	}

	if (pKeyboard->GetPress(DIK_W) == true)
	{
		m_posV.x += -sinf(m_rot.y) * CAMERA_MOVE_SPEED;
		m_posR.x += -sinf(m_rot.y) * CAMERA_MOVE_SPEED;
		m_posV.z += cosf(m_rot.y) * CAMERA_MOVE_SPEED;
		m_posR.z += cosf(m_rot.y) * CAMERA_MOVE_SPEED;
	}
	else if (pKeyboard->GetPress(DIK_S) == true)
	{
		m_posV.x += sinf(m_rot.y) * CAMERA_MOVE_SPEED;
		m_posR.x += sinf(m_rot.y) * CAMERA_MOVE_SPEED;
		m_posV.z += -cosf(m_rot.y) * CAMERA_MOVE_SPEED;
		m_posR.z += -cosf(m_rot.y) * CAMERA_MOVE_SPEED;
	}
#endif
}

//========================
//�ݒ菈��
//========================
void CCamera::SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//�f�o�C�X�̎擾

	//�v���W�F�N�V�����}�g���b�N�X������
	D3DXMatrixIdentity(&m_mtxProjection);

	//�쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection, D3DXToRadian(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, CAMERA_LENGTH);

	//�ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	
	//�r���[�}�g���b�N�X������
	D3DXMatrixIdentity(&m_mtxView);

	//�쐬
	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);

	//�ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//========================
//���_�C���i�����_���S�j
//========================
void CCamera::FixPosV(void)
{
#if 1
	m_posV.x = m_posR.x + cosf(m_rot.x) * sinf(m_rot.y) * 100.0f;
	m_posV.y = m_posR.y - sinf(m_rot.x) * 100.0f;
	m_posV.z = m_posR.z - cosf(m_rot.x) * cosf(m_rot.y) * 100.0f;
#endif
}

//========================
//�����_�C���i���_���S�j
//========================
void CCamera::FixPosR(void)
{
#if 0
	m_posR.x = m_posV.x - sinf(m_rot.y) * cosf(m_rot.x) * m_fLength;
	m_posR.y = m_posV.y - sinf(m_rot.x) * cosf(m_rot.y) * m_fLength;
	m_posR.z = m_posV.z + cosf(m_rot.y) * cosf(m_rot.x) * m_fLength;

#endif
	m_posR.x = m_posV.x - sinf(m_rot.y) * cosf(m_rot.x) * 100.0f;
	m_posR.z = m_posV.z + cosf(m_rot.y) * cosf(m_rot.x) * 100.0f;
}

//========================
//�p�x�C��
//========================
void CCamera::FixRot(void)
{
	//�J����1���v�Z
	m_rot.x = FIX_ROT(m_rot.x);
	m_rot.y = FIX_ROT(m_rot.y);
	m_rot.z = FIX_ROT(m_rot.z);

#if 1
	//[�J��������]x��]�̐���
	if (m_rot.x >= CAMERA_ROT_X_MAX * D3DX_PI)
	{
		m_rot.x = CAMERA_ROT_X_MAX * D3DX_PI;
	}
	else if (m_rot.x <= CAMERA_ROT_X_MIN * D3DX_PI)
	{
		m_rot.x = CAMERA_ROT_X_MIN * D3DX_PI;
	}
#endif
}