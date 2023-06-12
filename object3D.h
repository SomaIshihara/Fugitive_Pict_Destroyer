//======================================================
//
//�I�u�W�F�N�g�i3D�j�����̃w�b�_[object3D.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_
#include "main.h"
#include "manager.h"
#include "object.h"

//�I�u�W�F�N�g�N���X
class CObject3D : public CObject
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CObject3D(int nPriority = PRIORITY_DEFAULT);																						//�f�t�H���g
	CObject3D(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, int nPriority = PRIORITY_DEFAULT);	//�I�[�o�[���[�h�i�ʒu�����T�C�Y�j
	virtual ~CObject3D();

	//��{����
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//����
	static CObject3D* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight);

	//�擾
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return FLOAT_ZERO; }
	float GetDepth(void) { return m_fDepth; }

private:
	LPDIRECT3DTEXTURE9 m_pTexture;		//�e�N�X�`���|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxbuff;	//���_�o�b�t�@
	D3DXMATRIX m_mtxWorld;				//���[���h�}�g��

	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXVECTOR3 m_rot;	//����
	float m_fWidth;		//��(X)
	float m_fDepth;		//���s(Z)
};

#endif // !_OBJECT_H_
