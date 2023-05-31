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

class CBullet : public CObject2D
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CBullet();	//�f�t�H���g
	CBullet(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fSpeed);	//�I�[�o�[���[�h�i�ʒu����and�A�j���[�V�����j
	~CBullet();

	//�ǂݍ���
	static HRESULT Load(const char* pPath);
	static void Unload(void);

	//����
	static CBullet* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fSpeed);

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�Փ˔���
	bool CollisionEnemy(void);	//�G
	bool CollisionPlayer(void);	//�v���C���[

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	//�e�N�X�`��
	D3DXVECTOR3 m_move;						//�ړ���
	TYPE m_Type;							//�N�̒e��
};

#endif // !_PLAYER_H_