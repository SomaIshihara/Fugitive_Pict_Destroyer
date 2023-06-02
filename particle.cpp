//======================================================
//
//�p�[�e�B�N������[particle.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "particle.h"
#include "manager.h"
#include "renderer.h"
#include "effect.h"
#include "Culc.h"

//�}�N��
#define ROT_ACCU	(100)	//�p�x�̐��x
#define SPEED_ACCU	(100)	//���x�̐��x
#define PART_EFFECT_LIFE	(20)	//�G�t�F�N�g�p�p�[�e�B�N���̎���

//�ÓI�����o�ϐ�
LPDIRECT3DTEXTURE9 CParticle::m_pTexture = NULL;

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CParticle::CParticle()
{
	//�p�[�e�B�N���̃N���A
	m_particle.pos = VEC3_ZERO;
	m_particle.nLife = INT_ZERO;
	m_particle.nEffeceNum = INT_ZERO;
	m_particle.fSpeedBace = FLOAT_ZERO;
	m_particle.fSpeedDegree = FLOAT_ZERO;
	m_particle.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_particle.fWidth = FLOAT_ZERO;
	m_particle.fHeight = FLOAT_ZERO;
}

//=================================
//�R���X�g���N�^�i�I�[�o�[���[�h�j
//=================================
CParticle::CParticle(const D3DXVECTOR3 pos, const int nLife, const int nEffeceNum, const float fSpeedBace, const float fSpeedDegree,
	const D3DXCOLOR col, const float fWidth, const float fHeight)
{
	//�p�[�e�B�N���̏�����
	m_particle.pos = pos;
	m_particle.nLife = nLife;
	m_particle.nEffeceNum = nEffeceNum;
	m_particle.fSpeedBace = fSpeedBace;
	m_particle.fSpeedDegree = fSpeedDegree;
	m_particle.col = col;
	m_particle.fWidth = fWidth;
	m_particle.fHeight = fHeight;
}

//=================================
//�f�X�g���N�^
//=================================
CParticle::~CParticle()
{
}

//=================================
//������
//=================================
HRESULT CParticle::Init(void)
{
	//�ł���
	return S_OK;
}

//=================================
//�I��
//=================================
void CParticle::Uninit(void)
{
	//�p�[�e�B�N���Ǘ��I�u�W�F�N�g�j��
	Release();
}

//=================================
//�X�V
//=================================
void CParticle::Update(void)
{
	//�G�t�F�N�g�ǉ�����
	for (int cntEffectNum = 0; cntEffectNum < m_particle.nEffeceNum; cntEffectNum++)
	{
		//�ʒu�i�p�[�e�B�N���̈ʒu�Ɠ����j
		//�ړ���
		float fRot = (float)(rand() % (int)(ROT_ACCU * TWO_PI + 1) - (int)(D3DX_PI * ROT_ACCU)) / (float)ROT_ACCU;
		float fSpeed = (rand() % (int)(m_particle.fSpeedDegree * SPEED_ACCU)) / SPEED_ACCU + m_particle.fSpeedBace;
		D3DXVECTOR3 move;
		move.x = sinf(fRot) * fSpeed;	//X
		move.y = cosf(fRot) * fSpeed;	//Y
		move.z = 0.0f;

		//�������i�p�[�e�B�N���ɂ���j
		//�F�i�p�[�e�B�N���ɂ���j
		//�����i�}�N���j

		//�G�t�F�N�g���o��
		CEffect* pEffect;
		pEffect = pEffect->Create(m_particle.pos, move, m_particle.fWidth, m_particle.fHeight, m_particle.col, PART_EFFECT_LIFE);
		pEffect->Init();

		//�G�t�F�N�g�Ƀe�N�X�`���𒣂�
		pEffect->BindTexture(m_pTexture);

		//AllEffect�̂ق���for���𔲂���
		break;
	}

	//�G�t�F�N�g�����Ǘ�
	m_particle.nLife--;

	if (m_particle.nLife <= INT_ZERO)
	{
		Uninit();
	}
}

//=================================
//�`��
//=================================
void CParticle::Draw(void)
{
	
}

//=================================
//��������
//=================================
CParticle* CParticle::Create(const D3DXVECTOR3 pos, const int nLife, const int nEffeceNum, const float fSpeedBace, const float fSpeedDegree,
	const D3DXCOLOR col, const float fWidth, const float fHeight)
{
	CParticle* pParticle = NULL;

	if (pParticle == NULL)
	{
		//�p�[�e�B�N���Ǘ��I�u�W�F�N�g����
		pParticle = new CParticle(pos, nLife, nEffeceNum, fSpeedBace, fSpeedDegree, col, fWidth, fHeight);

		//�p�[�e�B�N���Ǘ��I�u�W�F�N�g������
		pParticle->Init();

		return pParticle;
	}
	else
	{
		return NULL;
	}
}

//=================================
//�e�N�X�`���ǂݍ��ݏ���
//=================================
HRESULT CParticle::Load(const char* pPath)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//�f�o�C�X�擾

	//�e�N�X�`���ǂݍ���
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		pPath,
		&m_pTexture)))
	{//���s
		return E_FAIL;
	}

	//����
	return S_OK;
}

//=================================
//�e�N�X�`���j������
//=================================
void CParticle::Unload(void)
{
	//�e�N�X�`���j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}