//======================================================
//
//�I�u�W�F�N�g�i2D�j�����̃w�b�_[object2D.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_
#include "main.h"
#include "object.h"

//�I�u�W�F�N�g�N���X
class CObject2D : public CObject
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CObject2D();																						//�f�t�H���g
	CObject2D(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight);	//�I�[�o�[���[�h�i�ʒu�����T�C�Y�j
	virtual ~CObject2D();

	//��{����
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//����
	static CObject2D* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight);

	//�ݒ�
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }

	//�擾
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }

	//�ݒ�
	HRESULT SetPos(const D3DXVECTOR3 pos);	//�ʒu
	HRESULT SetRot(const D3DXVECTOR3 rot);	//����
	HRESULT SetTex(const D3DXVECTOR2 tex0, const D3DXVECTOR2 tex3);	//�e�N�X�`�����W

private:
	//���_�ʒu�ݒ�
	HRESULT SetVtxPos(void);

	LPDIRECT3DVERTEXBUFFER9 m_pVtxbuff;		//���_�o�b�t�@
	LPDIRECT3DTEXTURE9 m_pTexture;			//�e�N�X�`���|�C���^
	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXVECTOR3 m_rot;	//����
	float m_fLength;	//����
	float m_fAngle;		//�p�x
	float m_fWidth;		//��
	float m_fHeight;	//����
};

#endif // !_OBJECT_H_
