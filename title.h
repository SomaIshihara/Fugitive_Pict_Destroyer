//======================================================
//
//�^�C�g���V�[���̃w�b�_[title.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _TITLE_H_
#define _TITLE_H_
#include "manager.h"

//�O���錾
class CButton3D;
class CPlayer;

//�^�C�g���N���X
class CTitle : public CScene
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CTitle();
	~CTitle();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	D3DXVECTOR3 m_cursorPos;	//�N���b�N�O�̃J�[�\���ʒu
	static CButton3D* m_pButton;
};

#endif // !_TITLE_H_
