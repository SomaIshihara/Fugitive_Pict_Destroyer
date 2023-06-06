//======================================================
//
//��������[number.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "number.h"
#include "input.h"
#include <assert.h>

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CNumber::CNumber(int nPriority) :CObject2D(nPriority)
{
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h �ʒu����and�p�^�[���������j
//=================================
CNumber::CNumber(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, int nPriority) 
	: CObject2D(pos,rot, fWidth, fHeight, nPriority)
{
}

//=================================
//�f�X�g���N�^
//=================================
CNumber::~CNumber()
{
}

//=================================
//������
//=================================
HRESULT CNumber::Init(void)
{
	//�e�N���X����
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}
	//�l������

	//�e�N�X�`���ݒ�
	D3DXVECTOR2 tex0, tex3;
	tex0 = D3DXVECTOR2((float)(INT_ZERO % GetPatWidth()) / GetPatWidth(),
		(float)(INT_ZERO / GetPatWidth()) / GetPatHeight());
	tex3 = D3DXVECTOR2((float)(INT_ZERO % GetPatWidth() + 1) / GetPatWidth(),
		(float)(INT_ZERO / GetPatWidth() + 1) / GetPatHeight());

	if (FAILED(SetTex(tex0, tex3)))
	{
		return E_FAIL;
	}

	return S_OK;
}
//=================================
//�I��
//=================================
void CNumber::Uninit(void)
{
	//�e�N���X����
	CObject2D::Uninit();
}

//=================================
//�X�V
//=================================
void CNumber::Update(void)
{
	//�e�N���X����
	CObject2D::Update();
}

//=================================
//�`��
//=================================
void CNumber::Draw(void)
{
	//�e�N���X����
	CObject2D::Draw();
}

//=================================
//��������
//=================================
CNumber* CNumber::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight)
{
	CNumber* pObjAnim2D = NULL;

	if (pObjAnim2D == NULL)
	{
		//�I�u�W�F�N�g�A�j���[�V����2D�̐���
		pObjAnim2D = new CNumber(pos, rot, fWidth, fHeight);

		//������
		pObjAnim2D->Init();

		return pObjAnim2D;
	}
	else
	{
		return NULL;
	}
}

//=================================
//�������������inDigit��0�ȏ�œ��͂��邱�Ɓj
//=================================
void CNumber::SetNumber(const int nSource, const int nDigit)
{
	int nNumber = 0;	//1�����̐���

	if (nDigit < 0)
	{//�s��
		assert(false);
	}

	//�����Ƃ̐���������
	nNumber = nSource % (int)pow(10, nDigit + 1) / (int)pow(10, nDigit);

	//�e�N�X�`���ݒ�
	D3DXVECTOR2 tex0, tex3;
	tex0 = D3DXVECTOR2((float)(nNumber % GetPatWidth()) / GetPatWidth(),
		(float)(nNumber / GetPatWidth()) / GetPatHeight());
	tex3 = D3DXVECTOR2((float)(nNumber % GetPatWidth() + 1) / GetPatWidth(),
		(float)(nNumber / GetPatWidth() + 1) / GetPatHeight());

	if (FAILED(SetTex(tex0, tex3)))
	{
		assert(false);
	}
}