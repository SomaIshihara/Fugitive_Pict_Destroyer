//======================================================
//
//���U���g�V�[���̃w�b�_[result.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _RESULT_H_
#define _RESULT_H_
#include "manager.h"

//�^�C�g���N���X
class CResult : public CScene
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CResult();
	~CResult();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:

};

#endif // !_RESULT_H_
