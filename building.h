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

//�O���錾
class CXModel;

//�����N���X
class CBuilding : public CObject
{
public:
	//�����p�����[�^�\����
	typedef struct
	{
		char aName[64];
		int nEndurance;
		long long nValue;
	} BuildingParam;

	//�R���X�g���N�^�E�f�X�g���N�^
	CBuilding();
	CBuilding(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel);
	~CBuilding();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�p�����[�^�ǂݍ���
	static void LoadParam(const char* pPath);

	//����
	static CBuilding* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel);

	//�擾
	static CBuilding* GetBuilding(int nIdx) { return m_apBuilding[nIdx]; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	float GetDepth(void) { return m_fDepth; }
	CXModel* GetModel(void) { return m_pModel; }
	int GetEndurance(void) { return m_nEndurance; }
	static BuildingParam GetBuildingParam(int nIdx) { return m_aBuildingParam[nIdx]; }

	//�_���[�W�t�^
	void AddDamage(int nDamage);

private:
	static CBuilding* m_apBuilding[MAX_OBJ];
	static BuildingParam m_aBuildingParam[MAX_OBJ];
	static int m_nNumAll;			//�����N���X���ł̌�
	int m_nID;						//�����N���X���ł�ID
	int m_nEndurance;				//�ϋv�l
	D3DXMATRIX mtxWorld;			//���[���h�}�g��
	CXModel* m_pModel;				//���f���|�C���^

	//�ʒu��
	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXVECTOR3 m_rot;	//����
	float m_fWidth;		//��(X)
	float m_fHeight;	//����(Y)
	float m_fDepth;		//���s(Z)

	//������
	float m_fRedAlpha;	//�_���[�W��H��������̐ԐF�
};

#endif // !_BUILDING_H_
