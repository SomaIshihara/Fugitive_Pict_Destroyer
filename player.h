//======================================================
//
//�v���C���[�����̃w�b�_[player.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//�O���錾
class CBuilding;
class CPict;
class CButton2D;

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

	//�U������
	void Attack(void);

	//���e����
	void AddPict(const int nDestroyer, const int nBlocker, const int nNormal);

private:
	//�֐�
	void Move(void);								//�J�����ړ�����
	void Rotate(void);								//�J������]����
	void Select(void);								//�I�u�W�F�N�g�I��

	//�ϐ�
	CButton2D* m_pButtonATK;		//�U���{�^��

	bool m_bControllPict;			//�s�N�g����𑀏c���邩�ǂ���
	CBuilding* m_pSelectBuilding;	//�I�𒆂̌���
	CPict* m_pSelectPict;			//�I�𒆂̃s�N�g����

	int m_nHaveDestroyer;	//�f�X�g���C���[������
	int m_nHaveBlocker;		//�u���b�J�[������
	int m_nHaveNormal;		//��ʐl������

	D3DXVECTOR3 m_cursorPos;	//�N���b�N�O�̃J�[�\���ʒu
};

#endif