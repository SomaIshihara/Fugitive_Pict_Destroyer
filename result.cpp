//======================================================
//
//�^�C�g���V�[��[title.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "precompile.h"
#include "manager.h"
#include "texture.h"
#include "input.h"
#include "result.h"
#include "object.h"
#include "bg.h"

//=================================
//�R���X�g���N�^
//=================================
CResult::CResult()
{
}

//=================================
//�f�X�g���N�^
//=================================
CResult::~CResult()
{
}

//=================================
//������
//=================================
HRESULT CResult::Init(void)
{
	CBG::Create()->BindTexture(9);
	
	return S_OK;
}

//=================================
//�I��
//=================================
void CResult::Uninit(void)
{
	CObject::ReleaseAll();
}

//=================================
//�X�V
//=================================
void CResult::Update(void)
{
	CInputMouse* pMouse = CManager::GetInputMouse();

	if (pMouse->GetTrigger(MOUSE_CLICK_LEFT) == true)
	{//��
		CManager::SetMode(CScene::MODE_TITLE);
	}
}

//=================================
//�`��
//=================================
void CResult::Draw(void)
{
	//����ɂ���Ă����
}
