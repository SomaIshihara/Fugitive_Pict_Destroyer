//======================================================
//
//�����L���O�V�[���̃w�b�_[ranking.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _RANKING_H_
#define _RANKING_H_
#include "manager.h"

//�^�C�g���N���X
class CRanking : public CScene
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CRanking();
	~CRanking();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:

};

#endif // !_RANKING_H_
