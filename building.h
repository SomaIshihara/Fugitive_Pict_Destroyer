//======================================================
//
//���������̃w�b�_[building.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _BUILDING_H_
#define _BUILDING_H_

#include "main.h"
#include "objectX.h"

//�����N���X
class CBuilding : public CObjectX
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CBuilding();
	CBuilding(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nIdx);
	~CBuilding();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//����
	static CBuilding* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nIdx);

	//�擾
	static CBuilding* GetBuilding(int nIdx) { return m_apBuilding[nIdx]; }

	//�_���[�W�t�^
	void AddDamage(int nDamage);

private:
	static CBuilding* m_apBuilding[MAX_OBJ];
	static int m_nNumAll;			//�����N���X���ł̌�
	int m_nID;						//�����N���X���ł�ID
	int m_nEndurance;				//�ϋv�l
	//const int m_nEnduranceMax;	//�����l
	//const int m_nValue;			//�����̉��l
};

#endif // !_BUILDING_H_
