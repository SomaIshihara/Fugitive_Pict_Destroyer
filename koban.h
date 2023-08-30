//======================================================
//
//��ԃv���O�����̃w�b�_[koban.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _KOBAN_H_
#define _KOBAN_H_
#include "objectX.h"
#include "xmodel.h"

//��ԃN���X
class CKoban : CObjectX
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CKoban();
	CKoban(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel);
	~CKoban();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void CommonUpdate(void);	//��ԋ��ʍX�V����

	//����
	static CKoban* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel);

	//�ݒ�
	static void SetKobanParam(const int nSpawnSpan, const int nPoliceMax);

private:
	static CKoban* m_apKoban[MAX_OBJ];	//��ԏ��
	int m_nID;					//ID
	static int m_nNumAll;		//����
	static int m_nCounterSpawn;	//�����J�E���^
	static int m_nSpawnSpan;	//�����̑���
	static int m_nPoliceMax;	//�x�@�̍ő�l��
};

#endif // !_KOBAN_H_
