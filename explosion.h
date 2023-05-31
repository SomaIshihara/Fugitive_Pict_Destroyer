//======================================================
//
//���������̃w�b�_[explosion.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "main.h"
#include "objectAnim2D.h"

class CExplosion : public CObjectAnim2D
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CExplosion();																												//�f�t�H���g
	CExplosion(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,
		const float fWidth, const float fHeight, const int nPatWidth, const int nPatHeight,
		const int nAnimSpeed);	//�I�[�o�[���[�h�i�ʒu����and�A�j���[�V�����j
	virtual ~CExplosion();

	//�ǂݍ���
	static HRESULT Load(const char* pPath);
	static void Unload(void);

	//��{����
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//����
	static CExplosion* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight,
		const int nPatWidth, const int nPatHeight, const int nAnimSpeed);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	//�e�N�X�`��
};

#endif // !_OBJ_ANIM2D_H_