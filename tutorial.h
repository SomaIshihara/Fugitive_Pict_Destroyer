//======================================================
//
//�`���[�g���A���V�[���̃w�b�_[tutorial.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//�^�C�g���N���X
class CTutorial : public CScene
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CTutorial();
	~CTutorial();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:

};

#endif // !_TUTORIAL_H_
