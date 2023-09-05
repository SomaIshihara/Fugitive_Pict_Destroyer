//======================================================
//
//�����L���O�V�[��[ranking.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "precompile.h"
#include "manager.h"
#include "ranking.h"
#include "input.h"
#include "fade.h"

//=================================
//�R���X�g���N�^
//=================================
CRanking::CRanking()
{
}

//=================================
//�f�X�g���N�^
//=================================
CRanking::~CRanking()
{
}

//=================================
//������
//=================================
HRESULT CRanking::Init(void)
{
	//���o������
	CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 100.0f, 0.0f), CManager::VEC3_ZERO, 311.0f, 111.0f);

	return S_OK;
}

//=================================
//�I��
//=================================
void CRanking::Uninit(void)
{
}

//=================================
//�X�V
//=================================
void CRanking::Update(void)
{
	CInputMouse* pMouse = CManager::GetInputMouse();

	if (pMouse->GetTrigger(MOUSE_CLICK_LEFT) == true)
	{//�^�C�g���ɑJ��
		CFade::Create(CScene::MODE_TITLE);
	}
}

//=================================
//�`��
//=================================
void CRanking::Draw(void)
{
}
