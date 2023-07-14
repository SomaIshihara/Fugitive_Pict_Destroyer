//======================================================
//
//�I�u�W�F�N�g����[object.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "object.h"

//�ÓI�ϐ�������
CObject* CObject::m_pTop = NULL;
CObject* CObject::m_pCur = NULL;
CObject* CObject::m_pProcessNext = NULL;
int CObject::m_nNumAll = 0;

//=================================
//�R���X�g���N�^
//=================================
CObject::CObject(int nPriority)
{
	if (m_pCur == NULL)
	{//�Ō�������Ȃ��i���Ȃ킿�擪�����Ȃ��j
		m_pTop = this;	//�����擪
		m_pPrev = NULL;	//�O��N�����Ȃ�
		m_pNext = NULL;
	}
	else
	{//�Ō��������
		m_pPrev = m_pCur;		//�Ō���������̑O�̃I�u�W�F
		m_pCur->m_pNext = this;	//�Ō���̎��̃I�u�W�F������
		m_pNext = NULL;			//�����̎��̃I�u�W�F�͂��Ȃ�
	}
	m_pCur = this;	//�����Ō��
}

//=================================
//�f�X�g���N�^
//=================================
CObject::~CObject()
{}

//=================================
//�I�u�W�F�N�g���ׂĔj��
//=================================
void CObject::ReleaseAll(void)
{
	CObject* pObject = m_pTop;	//�擪������

#if 0
	while (pObject != NULL)
	{//�Ō���܂ŉ񂵑�����
		CObject* pObjectNext = pObject->m_pNext;	//���̃I�u�W�F�ۑ�
		pObject->Uninit();		//�j��
		pObject = pObjectNext;	//��������
	}
#endif
	while (pObject != NULL)
	{//�Ō���܂ŉ񂵑�����
		 //CObject* pObjectNext = pObject->m_pNext;	//���̃I�u�W�F�ۑ�
		m_pProcessNext = pObject->m_pNext;	//���̃I�u�W�F�ۑ�
		pObject->Uninit();		//�I��
		//pObject = pObjectNext;	//��������
		pObject = m_pProcessNext;	//��������
	}
}

//=================================
//�I�u�W�F�N�g���ׂčX�V
//=================================
void CObject::UpdateAll(void)
{
	CObject* pObject = m_pTop;	//�擪������
#if 0
	while (pObject != NULL)
	{//�Ō���܂ŉ񂵑�����
		CObject* pObjectNext = pObject->m_pNext;	//���̃I�u�W�F�ۑ�
		pObject->Update();		//�X�V
		pObject = pObjectNext;	//��������
	}
#endif
	while (pObject != NULL)
	{//�Ō���܂ŉ񂵑�����
		//CObject* pObjectNext = pObject->m_pNext;	//���̃I�u�W�F�ۑ�
		m_pProcessNext = pObject->m_pNext;	//���̃I�u�W�F�ۑ�
		pObject->Update();		//�X�V
		//pObject = pObjectNext;	//��������
		pObject = m_pProcessNext;	//��������
	}
}

//=================================
//�I�u�W�F�N�g���ׂĕ`��
//=================================
void CObject::DrawAll(void)
{
	CCamera* pCamera = CManager::GetCamera();	//�J����
	pCamera->SetCamera();

	CObject* pObject = m_pTop;	//�擪������

	while (pObject != NULL)
	{//�Ō���܂ŉ񂵑�����
		//CObject* pObjectNext = pObject->m_pNext;	//���̃I�u�W�F�ۑ�
		m_pProcessNext = pObject->m_pNext;	//���̃I�u�W�F�ۑ�
		pObject->Draw();		//�`��
		//pObject = pObjectNext;	//��������
		pObject = m_pProcessNext;	//��������
	}
}

//=================================
//�I�u�W�F�N�g�j��
//=================================
void CObject::Release(void)
{
	int nPriority = m_nPriority;

	if (m_pPrev != NULL)
	{//�O�ɃI�u�W�F������
		m_pPrev->m_pNext = m_pNext;	//�O�̃I�u�W�F�̎��̃I�u�W�F�͎����̎��̃I�u�W�F
	}
	if (m_pNext != NULL)
	{
		m_pNext->m_pPrev = m_pPrev;	//���̃I�u�W�F�̑O�̃I�u�W�F�͎����̑O�̃I�u�W�F
	}

	if (m_pCur == this)
	{//�Ō���ł���
		m_pCur = m_pPrev;	//�Ō���������̑O�̃I�u�W�F�ɂ���
	}
	if (m_pTop == this)
	{
		m_pTop = m_pNext;	//�擪�������̎��̃I�u�W�F�ɂ���
	}
	if (m_pProcessNext == this)
	{
		m_pProcessNext = m_pPrev->m_pNext;
	}

	//����	
	delete this;	//�������g�j��
	m_nNumAll--;	//�������炷
}