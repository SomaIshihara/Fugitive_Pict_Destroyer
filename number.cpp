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

//�ÓI�����o�ϐ�
CObject2D::PatternTexture CNumber::m_patternTexture = { NULL,0,0 };

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CNumber::CNumber()
{
	//�l�N���A
	m_patternTexture.pTexture = NULL;
	m_patternTexture.nPatternWidth = 0;
	m_patternTexture.nPatternHeight = 0;
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h �ʒu����and�p�^�[���������j
//=================================
CNumber::CNumber(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight) : CObject2D(pos,rot, fWidth, fHeight)
{
	//�l�N���A
	m_patternTexture.pTexture = NULL;
	m_patternTexture.nPatternWidth = 0;
	m_patternTexture.nPatternHeight = 0;
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
	tex0 = D3DXVECTOR2((float)(INT_ZERO % m_patternTexture.nPatternWidth) / m_patternTexture.nPatternWidth,
		(float)(INT_ZERO / m_patternTexture.nPatternWidth) / m_patternTexture.nPatternHeight);
	tex3 = D3DXVECTOR2((float)(INT_ZERO % m_patternTexture.nPatternWidth + 1) / m_patternTexture.nPatternWidth,
		(float)(INT_ZERO / m_patternTexture.nPatternWidth + 1) / m_patternTexture.nPatternHeight);

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

		//�e�N�X�`�����蓖��
		pObjAnim2D->BindTexture(m_patternTexture.pTexture);

		return pObjAnim2D;
	}
	else
	{
		return NULL;
	}
}

//=================================
//�e�N�X�`���ǂݍ��ݏ���
//=================================
HRESULT CNumber::Load(const char* pPath, int nPatWidth, int nPatHeight)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//�f�o�C�X�擾

	//�e�N�X�`���ǂݍ���
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		pPath,
		&m_patternTexture.pTexture)))
	{//���s
		return E_FAIL;
	}

	//�p�^�[�����������
	m_patternTexture.nPatternWidth = nPatWidth;
	m_patternTexture.nPatternHeight = nPatHeight;

	//����
	return S_OK;
}

//=================================
//�e�N�X�`���j������
//=================================
void CNumber::Unload(void)
{
	//�e�N�X�`���j��
	if (m_patternTexture.pTexture != NULL)
	{
		m_patternTexture.pTexture->Release();
		m_patternTexture.pTexture = NULL;
	}
}

//=================================
//�������������inDigit��1�ȏ�œ��͂��邱�Ɓj
//=================================
void CNumber::SetNumber(const int nSource, const int nDigit)
{
	int nNumber = 0;	//1�����̐���

	if (nDigit <= 0)
	{//�s��
		assert(false);
	}

	//�����Ƃ̐���������
	nNumber = nSource % (int)pow(10, nSource + 1) / (int)pow(10, nSource);

	//�e�N�X�`���ݒ�
	D3DXVECTOR2 tex0, tex3;
	tex0 = D3DXVECTOR2((float)(nNumber % m_patternTexture.nPatternWidth) / m_patternTexture.nPatternWidth,
		(float)(nNumber / m_patternTexture.nPatternWidth) / m_patternTexture.nPatternHeight);
	tex3 = D3DXVECTOR2((float)(nNumber % m_patternTexture.nPatternWidth + 1) / m_patternTexture.nPatternWidth,
		(float)(nNumber / m_patternTexture.nPatternWidth + 1) / m_patternTexture.nPatternHeight);

	if (FAILED(SetTex(tex0, tex3)))
	{
		assert(false);
	}
}