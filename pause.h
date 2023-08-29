//======================================================
//
//�|�[�Y��ʂ̃w�b�_[game.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _GAME_H_
#define _GAME_H_
#include "manager.h"

//�O���錾
class CObject2D;
class CButton2D;
class CBG;

//�^�C�g���N���X
class CGame
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CGame();
	~CGame();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�擾
	

private:
	CBG* m_pBG;					//�w�i
	CObject2D* m_pPolygon;		//����������Ȃ��|���S��
	CButton2D* m_pContinue;		//�ĊJ�{�^��
	CButton2D* m_pRestart;		//���X�^�[�g�{�^��
	CButton2D* m_pExit;			//�I���{�^��
	CButton2D* m_pScreenShot;	//�X�N�V�����[�h�{�^��
	bool m_bScreenShotMode;		//�X�N�V�����[�h��
};

#endif // !_GAME_H_
