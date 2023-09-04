//======================================================
//
//�`���[�g���A���V�[��[tutorial.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "precompile.h"
#include "manager.h"
#include "texture.h"
#include "input.h"
#include "tutorial.h"
#include "object.h"
#include "bg.h"

//=================================
//�R���X�g���N�^
//=================================
CTutorial::CTutorial()
{
}

//=================================
//�f�X�g���N�^
//=================================
CTutorial::~CTutorial()
{
}

//=================================
//������
//=================================
HRESULT CTutorial::Init(void)
{
	CBG::Create()->BindTexture(9);
	
	return S_OK;
}

//=================================
//�I��
//=================================
void CTutorial::Uninit(void)
{
	CObject::ReleaseAll();
}

//=================================
//�X�V
//=================================
void CTutorial::Update(void)
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
void CTutorial::Draw(void)
{
	//����ɂ���Ă����
}
