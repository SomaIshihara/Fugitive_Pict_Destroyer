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
	CExplosion();														//�f�t�H���g
	CExplosion(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,
		const float fWidth, const float fHeight, const int nAnimSpeed);	//�I�[�o�[���[�h�i�ʒu����and�A�j���[�V�����j
	virtual ~CExplosion();

	//��{����
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//����
	static CExplosion* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const int nAnimSpeed);

private:
	int m_nIdxTexture;	//�e�N�X�`���ԍ�
};

#endif // !_OBJ_ANIM2D_H_