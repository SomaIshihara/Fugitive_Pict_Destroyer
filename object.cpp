//======================================================
//
//�I�u�W�F�N�g����[object.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "object.h"

//�ÓI�ϐ�������
CObject* CObject::m_apObject[] = {};
int CObject::m_nNumAll = 0;

//=================================
//�R���X�g���N�^
//=================================
CObject::CObject()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apObject[cnt] == NULL)
		{//�����
			m_apObject[cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;	//ID�o����
			m_Type = TYPE_NONE;	//�������񉽂ł��Ȃ����̂Ƃ���
			m_nNumAll++;	//�������₷
			break;
		}
	}
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
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apObject[cnt] != NULL)
		{//����ۂł͂Ȃ�
			m_apObject[cnt]->Uninit();
		}
	}
}

//=================================
//�I�u�W�F�N�g���ׂčX�V
//=================================
void CObject::UpdateAll(void)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apObject[cnt] != NULL)
		{//����ۂł͂Ȃ�
			m_apObject[cnt]->Update();
		}
	}
}

//=================================
//�I�u�W�F�N�g���ׂĕ`��
//=================================
void CObject::DrawAll(void)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apObject[cnt] != NULL)
		{//����ۂł͂Ȃ�
			m_apObject[cnt]->Draw();
		}
	}
}

//=================================
//�I�u�W�F�N�g�j��
//=================================
void CObject::Release(void)
{
	int nID = m_nID;
	if (m_apObject[nID] != NULL)
	{//����ۂł͂Ȃ�
		//�������g�j��
		delete m_apObject[nID];
		m_apObject[nID] = NULL;
		m_nNumAll--;	//�������炷
	}
}