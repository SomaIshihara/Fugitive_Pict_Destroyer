//==========================================
//
//�t�F�[�h�\���v���O�����̃w�b�_[fade.h]
//Author:�Ό��D�n
//
//==========================================
#ifndef _FADE_H_
#define _FADE_H_

#include "manager.h"

//�t�F�[�h�N���X
class CFade
{
public:
	//�t�F�[�h�̏��
	typedef enum
	{
		FADE_NONE = 0,	//�������Ȃ�
		FADE_IN,		//�C��
		FADE_OUT,		//�A�E�g
		FADE_MAX
	} FADE;

	//�R���X�g���N�^�E�f�X�g���N�^
	CFade();
	~CFade();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//����
	static CFade* Create(void);

	//�擾
	FADE Get(void) { return m_fade; }

private:
	void SetNext(CScene::MODE sceneNext);

	LPDIRECT3DVERTEXBUFFER9 m_pVtxbuff;		//���_�o�b�t�@
	FADE m_fade;							//���
	CScene::MODE m_modeNext;				//���̃V�[��
	float m_fAlpha;							//�s�����x
};

#endif // !_FADE_H_
