//======================================================
//
//�s�N�g���񏈗��̃w�b�_[pict.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _PICT_H_
#define _PICT_H_

#include "main.h"
#include "manager.h"
#include "object.h"

#define PICT_MODEL_NUM	(10)	//�s�N�g����Ɏg�����f����

//�O���錾
class CModel;
class CMotion;
class CBuilding;
class CShadow;

//�s�N�g����N���X
class CPict : public CObject
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CPict();
	CPict(const D3DXVECTOR3 pos);
	~CPict();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//����
	static CPict* Create(const D3DXVECTOR3 pos);

	//�擾
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return FLOAT_ZERO; }
	float GetHeight(void) { return FLOAT_ZERO; }
	float GetDepth(void) { return FLOAT_ZERO; }
	int GetModelIdx(void) { return -1; }
	static CPict* GetPict(int nID) { return m_apPict[nID]; }
	CBuilding* GetTarget(void) { return m_pTargetBuilding; }

	//�ݒ�
	void SetTarget(CBuilding* target) { m_pTargetBuilding = target; }
	void UnsetTarget(void) { m_pTargetBuilding = NULL; }

private:
	static CPict* m_apPict[MAX_OBJ];	//�s�N�g����|�C���^
	static int m_nNumAll;				//�s�N�g���񑍐�
	int m_nID;							//�s�N�g����ID
	CMotion* m_pMotion;					//���[�V�����|�C���^
	D3DXVECTOR3 m_pos;					//�ʒu
	D3DXVECTOR3 m_rot;					//����
	D3DXMATRIX m_mtxWorld;				//���[���h�}�g��
	CModel* m_apModel[PICT_MODEL_NUM];	//�s�N�g���񃂃f��
	int m_nNumModel;					//���f����

	CBuilding* m_pTargetBuilding;		//�ړI�n
	int m_nCounterDestruction;			//�j��J�E���^�[

	CShadow* m_pShadow;					//�e�I�u�W�F�N�g�|�C���^
};

#endif // !_PICT_H_
