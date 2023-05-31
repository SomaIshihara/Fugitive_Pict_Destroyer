//======================================================
//
//�A�j���[�V��������I�u�W�F�N�g�i2D�j�����̃w�b�_[objectAnim2D.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _OBJ_ANIM2D_H_
#define _OBJ_ANIM2D_H_

#include "main.h"
#include "object2D.h"

class CObjectAnim2D : public CObject2D
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CObjectAnim2D();																												//�f�t�H���g
	CObjectAnim2D(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, 
		const float fWidth, const float fHeight, const int nPatWidth, const int nPatHeight,
		const int nAnimSpeed, const bool bLoop);	//�I�[�o�[���[�h�i�ʒu����and�A�j���[�V�����j
	virtual ~CObjectAnim2D();

	//�ǂݍ���
	static HRESULT Load(const char* pPath);
	static void Unload(void);

	//��{����
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//����
	static CObjectAnim2D* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight,
		const int nPatWidth, const int nPatHeight, const int nAnimSpeed, const bool bLoop);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	//�e�N�X�`��
	int m_nAnimSpeed;	//�A�j���[�V�������x
	int m_nCounterAnim;	//�A�j���[�V�����J�E���^
	int m_nPatternAnim;	//�A�j���[�V�����p�^�[��
	const int m_nPatternWidth;	//�p�^�[����
	const int m_nPatternHeight;	//�p�^�[������
	bool m_bLoop;				//���[�v���邩
};

#endif // !_OBJ_ANIM2D_H_