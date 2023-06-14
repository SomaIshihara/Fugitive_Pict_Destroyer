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

private:
	//�֐�
	void Move(void);								//�J�����ړ�����
	void Rotate(void);								//�J������]����
	D3DXVECTOR3 GenerateClickRay(void);				//�N���b�N�������W�̃��C����
	D3DXVECTOR3 ConvertClickPosToWorld(float fZ);	//�N���b�N���W�����[���h���W�ɕϊ�
	void Select(void);								//�I�u�W�F�N�g�I��

	//�ϐ�
};

#endif