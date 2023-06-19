//======================================================
//
//���f�������̃w�b�_[model.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//���f���i�K�w�\������j�N���X
class CModel
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CModel();
	~CModel();

	//��{����
	HRESULT Init(const char* pPath, const D3DXVECTOR3 posOffset, const D3DXVECTOR3 rotOffset);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//����
	static CModel* Create(const char* pPath, const D3DXVECTOR3 posOffset, const D3DXVECTOR3 rotOffset);
	
	//�e�q�ݒ�
	void SetParent(CModel* pModel) { m_pParent = pModel; }

	//�}�g���擾
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }

private:
	//���f��
	LPD3DXMESH m_pMesh;			//���b�V��
	LPD3DXBUFFER m_pBuffMat;	//�}�e���A���|�C���^
	DWORD m_dwNumMatModel;		//�}�e��
	int* m_pIdxtexture;			//�e�N�X�`���ԍ��i���I�m�ہj

	//�ʒu��
	D3DXVECTOR3 m_pos;			//�ʒu
	D3DXVECTOR3 m_rot;			//����
	D3DXMATRIX m_mtxWorld;		//���[���h�}�g��
	CModel* m_pParent;			//�e���f���|�C���^
};

#endif // !_MODEL_H_