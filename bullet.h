//======================================================
//
//�e�����̃w�b�_[bullet.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"
#include "objectAnim2D.h"

//�D�揇��
#define BULLET_PRIORITY	(PRIORITY_DEFAULT)

class CBullet : public CObject2D
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CBullet(int nPriority = BULLET_PRIORITY);	//�f�t�H���g
	CBullet(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fSpeed,
		int nPriority = BULLET_PRIORITY);	//�I�[�o�[���[�h�i�ʒu����and�A�j���[�V�����j
	~CBullet();

	//����
	static CBullet* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fSpeed, const TYPE type);

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�Փ˔���
	bool CollisionEnemy(void);	//�G
#if 0
	bool CollisionPlayer(void);	//�v���C���[
#endif
	bool CollisionBlock(void);	//�u���b�N

	//�ݒ菈��
	void SetType(TYPE type) { m_Type = type; }	//�^�C�v�ݒ�

private:
	int m_nIdxTexture;		//�e�N�X�`���ԍ�
	D3DXVECTOR3 m_move;		//�ړ���
	TYPE m_Type;			//�N�̒e��
};

#endif // !_PLAYER_H_