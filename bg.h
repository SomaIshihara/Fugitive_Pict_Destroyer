//======================================================
//
//�w�i�i2D�j�����̃w�b�_[bg.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _BG_H_
#define _BG_H_
#include "main.h"
#include "object2D.h"

class CBG : public CObject2D
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CBG(int nPriority = BG_PRIORITY);
	~CBG();

	//�ǂݍ���
	static HRESULT Load(const char* pPath);
	static void Unload(void);

	//����
	static CBG* Create(void);

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	//�e�N�X�`��
};

#endif // !_BG_H_