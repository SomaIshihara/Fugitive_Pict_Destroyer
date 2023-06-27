//======================================================
//
//�v���C���[�����̃w�b�_[player.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//�v���C���[�N���X
class CPlayer
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CPlayer();
	~CPlayer();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	//�Ǐ]�ݒ�
	void Chace(int nPictNum) { m_nChacePictNum = nPictNum; }
	void Unchace(void) { m_nChacePictNum = -1; }

private:
	//�֐�
	void Move(void);								//�J�����ړ�����
	void Rotate(void);								//�J������]����
	void Select(void);								//�I�u�W�F�N�g�I��

	//�ϐ�
	int m_nChacePictNum;	//�Ǐ]����s�N�g����̔ԍ�
	bool m_bControllPict;	//�s�N�g����𑀏c���邩�ǂ���
};

#endif