//======================================================
//
//�u���b�N�����̃w�b�_[block.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_
#include "main.h"
#include "object2D.h"

//�D�揇��
#define BLOCK_PRIORITY	(1)

//�u���b�N�N���X
class CBlock : public CObject2D
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CBlock(int nPriority = BLOCK_PRIORITY);	//�f�t�H���g
	CBlock(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, int nPriority = BLOCK_PRIORITY);	//�I�[�o�[���[�h
	~CBlock();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�ǂݍ���
	static HRESULT Load(const char* pPath);
	static void Unload(void);

	//����
	static CBlock* Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight);
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	//�e�N�X�`��
};

#endif // !_BLOCK_H_
