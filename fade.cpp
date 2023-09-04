//==========================================
//
//�t�F�[�h�\���v���O����[fade.cpp]
//Author:�Ό��D�n
//
//==========================================
#include "manager.h"
#include "renderer.h"
#include "fade.h"

//�}�N��
#define FADE_SPEED	(0.032f)	//�t�F�[�h�̑���

//========================
//�R���X�g���N�^
//========================
CFade::CFade()
{
	m_fade = FADE_NONE;
	m_modeNext = CScene::MODE_MAX;
}

//========================
//�f�X�g���N�^
//========================
CFade::~CFade()
{
}

//========================
//�t�F�[�h����������
//========================
HRESULT CFade::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//�f�o�C�X�擾

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxbuff,
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	VERTEX_2D *pVtx;	//�ݒ�p�|�C���^
	if (FAILED(m_pVtxbuff->Lock(0, 0, (void **)&pVtx, 0)))
	{
		return E_FAIL;
	}

	//�ʒu�i�S�ʂȂ̂ŌŒ�j
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhw
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//�F
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);

	//�e�N�X�`�����W�i�ꉞ�j
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);

	//���_�o�b�t�@���A�����b�N
	if (FAILED(m_pVtxbuff->Unlock()))
	{
		return E_FAIL;
	}

	m_fade = FADE_NONE;	//�t�F�[�h�Ȃ�
}

//========================
//�t�F�[�h�I������
//========================
void CFade::Uninit(void)
{
	if (m_pVtxbuff != nullptr)
	{//�j��
		m_pVtxbuff->Release();
		m_pVtxbuff = nullptr;
	}

	delete this;
}

//========================
//�t�F�[�h�X�V����
//========================
void CFade::Update(void)
{
	VERTEX_2D *pVtx;					//�ݒ�p�|�C���^

	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)
		{
			m_fAlpha -= FADE_SPEED;
			if (m_fAlpha <= 0.0f)
			{
				m_fAlpha = 0.0f;
				m_fade = FADE_NONE;
			}
		}
		else if (m_fade == FADE_OUT)
		{
			m_fAlpha += FADE_SPEED;
			if (m_fAlpha >= 1.0f)
			{
				m_fAlpha = 1.0f;
				m_fade = FADE_IN;
				CManager::SetMode(m_modeNext);
			}
		}
	}

	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	if (FAILED(m_pVtxbuff->Lock(0, 0, (void **)&pVtx, 0)))
	{
		assert(false);
	}

	//���_�J���[
	pVtx[0].col = D3DXCOLOR(0.0f,0.0f,0.0f,m_fAlpha);
	pVtx[1].col = D3DXCOLOR(0.0f,0.0f,0.0f,m_fAlpha);
	pVtx[2].col = D3DXCOLOR(0.0f,0.0f,0.0f,m_fAlpha);
	pVtx[3].col = D3DXCOLOR(0.0f,0.0f,0.0f,m_fAlpha);

	//���_�o�b�t�@���A�����b�N
	if (FAILED(m_pVtxbuff->Unlock()))
	{
		assert(false);
	}
}

//========================
//�t�F�[�h�`�揈��
//========================
void CFade::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//�f�o�C�X�擾
	CTexture* pTexture = CManager::GetTexture();						//�e�N�X�`���I�u�W�F�N�g�擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxbuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, nullptr);

	//�`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================
//�t�F�[�h��������
//========================
CFade* CFade::Create(void)
{
	CFade* pFade = NULL;

	if (pFade == NULL)
	{
		//�t�F�[�h�̐���
		pFade = new CFade;

		//������
		pFade->Init();

		return pFade;
	}
	else
	{
		return NULL;
	}
}

//========================
//���̃V�[���ݒ菈��
//========================
void CFade::SetNext(CScene::MODE sceneNext)
{
	m_modeNext = sceneNext;
	m_fade = FADE_IN;
	m_fAlpha = 0.0f;
}