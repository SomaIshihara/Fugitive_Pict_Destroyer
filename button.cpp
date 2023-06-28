//======================================================
//
//�{�^������[button.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "button.h"
#include "input.h"
#include "Culc.h"
#include "manager.h"

//**************************************************
//2D�{�^��
//**************************************************
//========================
//�R���X�g���N�^
//========================
CButton2D::CButton2D()
{
	//�l�N���A
	m_bHold = false;
	m_bPress = false;
	m_bTrigger = false;
	m_bRelease = false;
	m_bRepeate = false;
}

//========================
//�R���X�g���N�^�i�I�[�o�[���[�h�j
//========================
CButton2D::CButton2D(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, int nPriority)
	: CObject2D(pos, rot, fWidth, fHeight, nPriority)
{
	//�l�N���A
	m_bHold = false;
	m_bPress = false;
	m_bTrigger = false;
	m_bRelease = false;
	m_bRepeate = false;
}

//========================
//�f�X�g���N�^
//========================
CButton2D::~CButton2D()
{
}

//========================
//������
//========================
HRESULT CButton2D::Init(void)
{
	//�e����
	CObject2D::Init();

	return S_OK;
}

//========================
//�I��
//========================
void CButton2D::Uninit(void)
{
	//�e����
	CObject2D::Uninit();
}

//========================
//�X�V
//========================
void CButton2D::Update(void)
{
	//�{�^������
	CInputMouse* mouse = CManager::GetInputMouse();	//�}�E�X�擾

	//�}�E�X�擾
	if (mouse != NULL)
	{
		D3DXVECTOR3 buttonPos = GetPos();			//�{�^���ʒu�擾
		float fWidth = GetWidth();					//�{�^�����擾
		float fHeight = GetHeight();				//�{�^�������擾
		D3DXVECTOR3 cursorPos = mouse->GetPos();	//�J�[�\���ʒu�擾

		if (cursorPos.x >= buttonPos.x - fWidth * 0.5f &&
			cursorPos.x <= buttonPos.x + fWidth * 0.5f &&
			cursorPos.y >= buttonPos.y - fHeight * 0.5f &&
			cursorPos.y <= buttonPos.y + fHeight * 0.5f)
		{//�͈͓�
			m_bHold = true;										//������
			m_bPress = mouse->GetPress(MOUSE_CLICK_LEFT);		//�v���X
			m_bTrigger = mouse->GetTrigger(MOUSE_CLICK_LEFT);	//�g���K�[
			m_bRelease = mouse->GetRelease(MOUSE_CLICK_LEFT);	//�����[�X
			m_bRepeate = mouse->GetRepeate(MOUSE_CLICK_LEFT);	//���s�[�g
		}
		else
		{//�͈͊O
			m_bHold = false;	//������
			m_bPress = false;	//�v���X
			m_bTrigger = false;	//�g���K�[
			m_bRelease = false;	//�����[�X
			m_bRepeate = false;	//���s�[�g
		}
	}
	else
	{//�}�E�X���擾�ł��Ȃ�
		m_bHold = false;	//������
		m_bPress = false;	//�v���X
		m_bTrigger = false;	//�g���K�[
		m_bRelease = false;	//�����[�X
		m_bRepeate = false;	//���s�[�g
	}

	//�e����
	CObject2D::Update();
}

//========================
//�`��
//========================
void CButton2D::Draw(void)
{
	//�e����
	CObject2D::Draw();
}

//========================
//����
//========================
CButton2D * CButton2D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight)
{
	CButton2D* pButton2D = NULL;

	if (pButton2D == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pButton2D = new CButton2D(pos, rot, fWidth, fHeight);

		//������
		pButton2D->Init();

		return pButton2D;
	}
	else
	{
		return NULL;
	}
}

//**************************************************
//3D�{�^��
//**************************************************
//========================
//�R���X�g���N�^
//========================
CButton3D::CButton3D()
{
	//�l�N���A
	m_bHold = false;
	m_bPress = false;
	m_bTrigger = false;
	m_bRelease = false;
	m_bRepeate = false;
}

//========================
//�R���X�g���N�^�i�I�[�o�[���[�h�j
//========================
CButton3D::CButton3D(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, int nPriority)
	: CObject3D(pos, rot, fWidth, fHeight, nPriority)
{
	//�l�N���A
	m_bHold = false;
	m_bPress = false;
	m_bTrigger = false;
	m_bRelease = false;
	m_bRepeate = false;
}

//========================
//�f�X�g���N�^
//========================
CButton3D::~CButton3D()
{
}

//========================
//������
//========================
HRESULT CButton3D::Init(void)
{
	//�e����
	CObject3D::Init();

	return S_OK;
}

//========================
//�I��
//========================
void CButton3D::Uninit(void)
{
	//�e����
	CObject3D::Uninit();
}

//========================
//�X�V
//========================
void CButton3D::Update(void)
{
	//�{�^������
	CInputMouse* mouse = CManager::GetInputMouse();	//�}�E�X�擾

	//�}�E�X�擾
	if (mouse != NULL)
	{
		D3DXVECTOR3 buttonPos = GetPos();			//�{�^���ʒu�擾
		D3DXVECTOR3 buttonRot = GetRot();			//�{�^���p�x�擾
		float fWidth = GetWidth();					//�{�^�����擾
		float fDepth = GetDepth();					//�{�^�������擾
		D3DXVECTOR3 cursorPos = mouse->GetPos();	//�J�[�\���ʒu�擾

		//�I�u�W�F�N�g�I���i0.0�`1.0�j
		D3DXVECTOR3 posNear = mouse->ConvertClickPosToWorld(0.0f);
		D3DXVECTOR3 posFar = mouse->ConvertClickPosToWorld(1.0f);

		//���_�v�Z
		D3DXVECTOR3 pos0 = D3DXVECTOR3(-fWidth * 0.5f, 0.0f, fDepth * 0.5f);
		D3DXVECTOR3 pos1 = D3DXVECTOR3(fWidth * 0.5f, 0.0f, fDepth * 0.5f);
		D3DXVECTOR3 pos2 = D3DXVECTOR3(-fWidth * 0.5f, 0.0f, -fDepth * 0.5f);
		D3DXVECTOR3 pos3 = D3DXVECTOR3(fWidth * 0.5f, 0.0f, -fDepth * 0.5f);
		pos0 = RotateVtx(pos0, buttonRot, buttonPos);
		pos1 = RotateVtx(pos1, buttonRot, buttonPos);
		pos2 = RotateVtx(pos2, buttonRot, buttonPos);
		pos3 = RotateVtx(pos3, buttonRot, buttonPos);

		float fDust;
		D3DXVECTOR3 nor = (posFar - posNear);
		D3DXVec3Normalize(&nor, &nor);
		if (D3DXIntersectTri(&pos2, &pos1, &pos0, &posNear, &nor, &fDust, &fDust, &fDust) == true ||
			D3DXIntersectTri(&pos1, &pos2, &pos3, &posNear, &nor, &fDust, &fDust, &fDust) == true)
		{//�͈͓�
			m_bHold = true;										//������
			m_bPress = mouse->GetPress(MOUSE_CLICK_LEFT);		//�v���X
			m_bTrigger = mouse->GetTrigger(MOUSE_CLICK_LEFT);	//�g���K�[
			m_bRelease = mouse->GetRelease(MOUSE_CLICK_LEFT);	//�����[�X
			m_bRepeate = mouse->GetRepeate(MOUSE_CLICK_LEFT);	//���s�[�g
		}
		else
		{//�͈͊O
			m_bHold = false;	//������
			m_bPress = false;	//�v���X
			m_bTrigger = false;	//�g���K�[
			m_bRelease = false;	//�����[�X
			m_bRepeate = false;	//���s�[�g
		}
	}
	else
	{//�}�E�X���擾�ł��Ȃ�
		m_bHold = false;	//������
		m_bPress = false;	//�v���X
		m_bTrigger = false;	//�g���K�[
		m_bRelease = false;	//�����[�X
		m_bRepeate = false;	//���s�[�g
	}

	//�e����
	CObject3D::Update();
}

//========================
//�`��
//========================
void CButton3D::Draw(void)
{
	//�e����
	CObject3D::Draw();
}

//========================
//����
//========================
CButton3D * CButton3D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight)
{
	CButton3D* pButton3D = NULL;

	if (pButton3D == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pButton3D = new CButton3D(pos, rot, fWidth, fHeight);

		//������
		pButton3D->Init();

		return pButton3D;
	}
	else
	{
		return NULL;
	}
}