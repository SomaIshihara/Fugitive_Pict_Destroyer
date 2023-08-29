//======================================================
//
//�w�i�i2D�j����[bg.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "bg.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//=================================
//�R���X�g���N�^
//=================================
CBG::CBG(int nPriority) : CObject2D(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, FLOAT_ZERO), VEC3_ZERO, SCREEN_WIDTH, SCREEN_HEIGHT, nPriority)
{
	m_nIdxTexture = -1;
}

//=================================
//�f�X�g���N�^
//=================================
CBG::~CBG()
{
}

//=================================
//������
//=================================
HRESULT CBG::Init(void)
{
	//�e�N���X����
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=================================
//�I��
//=================================
void CBG::Uninit(void)
{
	//�e�N���X����
	CObject2D::Uninit();
}

//=================================
//�X�V
//=================================
void CBG::Update(void)
{
	//�e�N���X����
	CObject2D::Update();
}

//=================================
//�`��
//=================================
void CBG::Draw(void)
{
	//�e�N���X����
	CObject2D::Draw();
}

//=================================
//��������
//=================================
CBG* CBG::Create(void)
{
	CBG* pObjBG = NULL;

	if (pObjBG == NULL)
	{
		//�w�i�̐���
		pObjBG = new CBG();

		//������
		pObjBG->Init();

		return pObjBG;
	}
	else
	{
		return NULL;
	}
}