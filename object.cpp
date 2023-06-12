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
CObject* CObject::m_apObject[PRIORITY_MAX][MAX_OBJ];
int CObject::m_nNumAll = 0;

//=================================
//�R���X�g���N�^
//=================================
CObject::CObject(int nPriority)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//���ׂĊm�F
		if (m_apObject[nPriority][cnt] == NULL)
		{//�����
			m_apObject[nPriority][cnt] = this;	//�������g�̃|�C���^��o�^
			m_nID = cnt;	//ID�o����
			m_nPriority = nPriority;	//�D�揇�ʊo����
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
	for (int cntPriority = 0; cntPriority < PRIORITY_MAX; cntPriority++)
	{
		for (int cntObj = 0; cntObj < MAX_OBJ; cntObj++)
		{//���ׂĊm�F
			if (m_apObject[cntPriority][cntObj] != NULL)
			{//����ۂł͂Ȃ�
				m_apObject[cntPriority][cntObj]->Uninit();
			}
		}
	}
}

//=================================
//�I�u�W�F�N�g���ׂčX�V
//=================================
void CObject::UpdateAll(void)
{
	for (int cntPriority = 0; cntPriority < PRIORITY_MAX; cntPriority++)
	{
		for (int cntObj = 0; cntObj < MAX_OBJ; cntObj++)
		{//���ׂĊm�F
			if (m_apObject[cntPriority][cntObj] != NULL)
			{//����ۂł͂Ȃ�
				m_apObject[cntPriority][cntObj]->Update();
			}
		}
	}
}

//=================================
//�I�u�W�F�N�g���ׂĕ`��
//=================================
void CObject::DrawAll(void)
{
	CCamera* pCamera = CManager::GetCamera();	//�J����
	pCamera->SetCamera();
	for (int cntPriority = 0; cntPriority < PRIORITY_MAX; cntPriority++)
	{
		for (int cntObj = 0; cntObj < MAX_OBJ; cntObj++)
		{//���ׂĊm�F
			if (m_apObject[cntPriority][cntObj] != NULL)
			{//����ۂł͂Ȃ�
				m_apObject[cntPriority][cntObj]->Draw();
			}
		}
	}
}

//=================================
//�I�u�W�F�N�g�j��
//=================================
void CObject::Release(void)
{
	int nID = m_nID;				//delete������g���Ȃ��Ȃ�̂ŕۊ�
	int nPriority = m_nPriority;
	if (m_apObject[nPriority][nID] != NULL)
	{//����ۂł͂Ȃ�
		//�������g�j��
		delete m_apObject[nPriority][nID];
		m_apObject[nPriority][nID] = NULL;
		m_nNumAll--;	//�������炷
	}
}