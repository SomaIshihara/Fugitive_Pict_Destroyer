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
class CBuilding : public CObject
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
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	float GetDepth(void) { return m_fDepth; }
	int GetModelIdx(void) { return m_nIdx; }

	//�_���[�W�t�^
	void AddDamage(int nDamage);

private:
	static CBuilding* m_apBuilding[MAX_OBJ];
	static int m_nNumAll;			//�����N���X���ł̌�
	int m_nID;						//�����N���X���ł�ID
	int m_nEndurance;				//�ϋv�l
	//const int m_nEnduranceMax;	//�����l
	//const int m_nValue;			//�����̉��l
	D3DXMATRIX mtxWorld;			//���[���h�}�g��
	int m_nIdx;						//���f���ԍ�

	//�ʒu��
	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXVECTOR3 m_rot;	//����
	float m_fWidth;		//��(X)
	float m_fHeight;	//����(Y)
	float m_fDepth;		//���s(Z)
};

#endif // !_BUILDING_H_
