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
#include <vector>

//��ԃN���X
class CKoban : public CObjectX
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
	static void SetKobanParam(const int nSpawnSpan, const int nPoliceMax, const int nPatrollNum);
	static void AddPolice(const int nNum) { m_waitingPolice += nNum; }

	//�_���[�W��
	static void ReportDamage(const int nID) { m_AttackList.emplace_back(nID); }

	//�擾
	static CKoban* GetKoban(const int nID) { return m_apKoban[nID]; }
	static int GetNumAll(void) { return m_nNumAll; }

private:
	static CKoban* m_apKoban[MAX_OBJ];	//��ԏ��
	static int m_disPatchCT[MAX_OBJ];	//�o�����CT�i�������ƂɊǗ��j
	static std::vector<int> m_AttackList;
	int m_nID;					//ID
	static int m_nNumAll;		//����

	static int m_nCounterSpawn;	//�����J�E���^
	static int m_nSpawnSpan;	//�����̑���

	static int m_waitingPolice;	//�ҋ@���̌x�@�̐l��

	static int m_nPatrollNum;	//�p�g���[���ɍs������l��
	static int m_nPoliceMax;	//�x�@�̍ő�l��
};

#endif // !_KOBAN_H_
