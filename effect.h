//======================================================
//
//�G�t�F�N�g�����̃w�b�_[effect.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_
#include "main.h"
#include "object2D.h"

//�D�揇��
#define EFFECT_PRIORITY	(2)

class CEffect : public CObject2D
{
public:
	//�G�t�F�N�g�\���̒�`
	typedef struct
	{
		D3DXVECTOR3 move;	//�ړ���
		D3DXCOLOR col;		//�F
		int nLife;			//����
	} Effect;

	//�R���X�g���N�^�E�f�X�g���N�^
	CEffect(int nPriority = EFFECT_PRIORITY);			//�f�t�H���g
	CEffect(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fWidth, const float fHeight,
		const D3DXCOLOR col, const int nLife, int nPriority = EFFECT_PRIORITY);		//�I�[�o�[���[�h�i�G�t�F�N�g���낢��ݒ�j
	~CEffect();

	//�ǂݍ���
	static HRESULT Load(const char* pPath);
	static void Unload(void);

	//����
	static CEffect* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fWidth, const float fHeight,
		const D3DXCOLOR col, const int nLife);

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	//�e�N�X�`��
	Effect m_effect;						//�G�t�F�N�g�\����
	const int m_nDefLife;					//�ݒ����
};

#endif // !_BG_H_