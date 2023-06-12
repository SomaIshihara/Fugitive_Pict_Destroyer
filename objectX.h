//======================================================
//
//�I�u�W�F�N�g�iX�j�����̃w�b�_[objectX.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_
#include "main.h"
#include "manager.h"
#include "object.h"

//�}�N��
#define X_MODEL_NUM		(64)	//���f���ۊǌ�
#define X_TEXTURE_NUM	(16)	//�e�N�X�`���g�p��

//�I�u�W�F�N�g�N���X
class CObjectX : public CObject
{
public:
	typedef struct
	{
		LPD3DXMESH m_pMesh;								//���b�V��
		LPD3DXBUFFER m_pBuffMat;						//�}�e���A���|�C���^
		DWORD m_dwNumMatModel;							//�}�e��
		LPDIRECT3DTEXTURE9 m_apTexture[X_TEXTURE_NUM];	//�e�N�X�`���|�C���^
	} Model;
	//�R���X�g���N�^�E�f�X�g���N�^
	CObjectX(int nPriority = PRIORITY_DEFAULT);																	//�f�t�H���g
	CObjectX(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nIdx, int nPriority = PRIORITY_DEFAULT);	//�I�[�o�[���[�h�i�ʒu�����T�C�Y�j
	virtual ~CObjectX();

	//��{����
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//�ǂݍ���
	static void Load(const char* pPath, const int nIdx);
	static void Unload(void);

	//����
	static CObjectX* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nIdx);

	//�擾
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	float GetDepth(void) { return m_fDepth; }

private:
	//���f��
	static Model m_aModel[X_MODEL_NUM];	//���f��
	D3DXMATRIX mtxWorld;				//���[���h�}�g��
	int m_nIdx;							//���f���ԍ�

	//�ʒu��
	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXVECTOR3 m_rot;	//����
	float m_fWidth;		//��(X)
	float m_fHeight;	//����(Y)
	float m_fDepth;		//���s(Z)
};

#endif // !_OBJECT_H_
