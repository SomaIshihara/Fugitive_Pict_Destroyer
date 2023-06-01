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

class CEffect : public CObject2D
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CEffect();
	~CEffect();

	//�ǂݍ���
	static HRESULT Load(const char* pPath);
	static void Unload(void);

	//����
	static CEffect* Create(void);

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	//�e�N�X�`��
};

#endif // !_BG_H_