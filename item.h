//======================================================
//
//�A�C�e�������̃w�b�_[item.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _ITEM_H_
#define _ITEM_H_
#include "main.h"
#include "object2D.h"

//�D�揇��
#define ITEM_PRIORITY	(PRIORITY_DEFAULT)

//�u���b�N�N���X
class CItem : public CObject2D
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CItem(int nPriority = ITEM_PRIORITY);	//�f�t�H���g
	CItem(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, int nPriority = ITEM_PRIORITY);	//�I�[�o�[���[�h
	~CItem();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�ǂݍ���
	static HRESULT Load(const char* pPath);
	static void Unload(void);

	//����
	static CItem* Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight);
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	//�e�N�X�`��
};

#endif // !_BLOCK_H_
