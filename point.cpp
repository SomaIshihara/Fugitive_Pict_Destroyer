//======================================================
//
//�ړ��|�C���g[point.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "point.h"
#include "building.h"
#include "pict.h"
#include "objectX.h"

//�ÓI�����o�ϐ�
CPoint* CPoint::m_pTop = NULL;
CPoint* CPoint::m_pCur = NULL;
int CPoint::m_nNumAll = 0;

#define POINT_INTERVAL	(1.2f)	//�Ԋu

//=================================
//�R���X�g���N�^
//=================================
CPoint::CPoint()
{
	if (m_pCur == NULL)
	{//�Ō�������Ȃ��i���Ȃ킿�擪�����Ȃ��j
		m_pTop = this;		//�����擪
		m_pPrev = NULL;		//�O��N�����Ȃ�
		m_pNext = NULL;
	}
	else
	{//�Ō��������
		m_pPrev = m_pCur;		//�Ō���������̑O�̃I�u�W�F
		m_pCur->m_pNext = this;	//�Ō���̎��̃I�u�W�F������
		m_pNext = NULL;			//�����̎��̃I�u�W�F�͂��Ȃ�
	}
	m_pCur = this;	//�����Ō��
	m_nNumAll++;
}

//=================================
//�f�X�g���N�^�i�f�t�H���g�j
//=================================
CPoint::~CPoint()
{
}

//=================================
//������
//=================================
HRESULT CPoint::Init(void)
{
	return S_OK;
}

//=================================
//���ׂďI��
//=================================
void CPoint::ReleaseAll(void)
{
	CPoint* pPoint = m_pTop;	//�擪������

	while (pPoint != NULL)
	{//�Ō���܂ŉ񂵑�����
		CPoint* pPointNext = pPoint->m_pNext;	//���̃I�u�W�F�ۑ�

		if (pPoint->m_pPrev != NULL)
		{//�O�ɃI�u�W�F������
			pPoint->m_pPrev->m_pNext = pPoint->m_pNext;	//�O�̃I�u�W�F�̎��̃I�u�W�F�͎����̎��̃I�u�W�F
		}
		if (pPoint->m_pNext != NULL)
		{
			pPoint->m_pNext->m_pPrev = pPoint->m_pPrev;	//���̃I�u�W�F�̑O�̃I�u�W�F�͎����̑O�̃I�u�W�F
		}

		if (m_pCur == pPoint)
		{//�Ō���ł���
			m_pCur = pPoint->m_pPrev;	//�Ō���������̑O�̃I�u�W�F�ɂ���
		}
		if (m_pTop == pPoint)
		{
			m_pTop = pPoint->m_pNext;	//�擪�������̎��̃I�u�W�F�ɂ���
		}

		//����	
		delete pPoint;	//�������g�j��
		m_nNumAll--;	//�������炷

		pPoint = pPointNext;	//��������
	}
}

//=================================
//�X�V
//=================================
void CPoint::Update(void)
{
	ReleaseAll();	//�����������

	//�����ɉ����Đ���
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{
		CBuilding* pBuilding = CBuilding::GetBuilding(cnt);

		if (pBuilding != NULL)
		{//��������
			D3DXVECTOR3 pos = pBuilding->GetPos();				//�ʒu�擾
			float fWidthHalf = pBuilding->GetWidth() * 0.5f;	//���擾
			float fDepthHalf = pBuilding->GetDepth() * 0.5f;	//���s�擾

			CPoint::Create(pos + D3DXVECTOR3(-fWidthHalf * POINT_INTERVAL, 0.0f, -fDepthHalf * POINT_INTERVAL));
			CPoint::Create(pos + D3DXVECTOR3(fWidthHalf * POINT_INTERVAL, 0.0f, -fDepthHalf * POINT_INTERVAL));
			CPoint::Create(pos + D3DXVECTOR3(-fWidthHalf * POINT_INTERVAL, 0.0f, fDepthHalf * POINT_INTERVAL));
			CPoint::Create(pos + D3DXVECTOR3(fWidthHalf * POINT_INTERVAL, 0.0f, fDepthHalf * POINT_INTERVAL));
		}
	}

	//�A�W�g
	CObject* pBuilding = CPict::GetAgit();

	if (pBuilding != NULL)
	{//��������
		D3DXVECTOR3 pos = pBuilding->GetPos();				//�ʒu�擾
		float fWidthHalf = pBuilding->GetWidth() * 0.5f;	//���擾
		float fDepthHalf = pBuilding->GetDepth() * 0.5f;	//���s�擾

		CPoint::Create(pos + D3DXVECTOR3(-fWidthHalf * POINT_INTERVAL, 0.0f, -fDepthHalf * POINT_INTERVAL));
		CPoint::Create(pos + D3DXVECTOR3(fWidthHalf * POINT_INTERVAL, 0.0f, -fDepthHalf * POINT_INTERVAL));
		CPoint::Create(pos + D3DXVECTOR3(-fWidthHalf * POINT_INTERVAL, 0.0f, fDepthHalf * POINT_INTERVAL));
		CPoint::Create(pos + D3DXVECTOR3(fWidthHalf * POINT_INTERVAL, 0.0f, fDepthHalf * POINT_INTERVAL));
	}
}

//=================================
//����
//=================================
CPoint* CPoint::Create(const D3DXVECTOR3 pos)
{
	CPoint* pPoint = NULL;

	if (pPoint == NULL)
	{
		//�I�u�W�F�N�g�A�j���[�V����2D�̐���
		pPoint = new CPoint;

		//������
		pPoint->Init();
		pPoint->m_pos = pos;

		return pPoint;
	}
	else
	{
		return NULL;
	}
}

//=================================
//�T��
//=================================
CPoint* CPoint::Search(const D3DXVECTOR3 pos, const CPoint* pNowPoint)
{
	CPoint* pPoint = m_pTop;	//�擪������
	CPoint* pPointNear = NULL;
	float fLength = 0.0f;

	while (pPoint != NULL)
	{//�Ō���܂ŉ񂵑�����
		CPoint* pPointNext = pPoint->m_pNext;	//���̃I�u�W�F�ۑ�
		
		//��������
		float fLengthNow = D3DXVec3Length(&(pPoint->m_pos - pos));
		if (pPointNear == NULL || fLength > fLengthNow)
		{//�܂�����
			pPointNear = pPoint;
			fLength = fLengthNow;
		}

		pPoint = pPointNext;	//��������
	}
	return pPointNear;
}