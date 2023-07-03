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
#include "collision.h"

#define PICT_MODEL_NUM	(10)	//�s�N�g����Ɏg�����f����

//�O���錾
class CModel;
class CMotion;
class CBuilding;
class CShadow;
class CPictPolice;

//�s�N�g����N���X
class CPict : public CObject
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CPict();
	CPict(const D3DXVECTOR3 pos);
	virtual ~CPict();

	//��{����
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//�擾
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	float GetDepth(void) { return m_fDepth; }
	int GetModelIdx(void) { return -1; }
	static CPict* GetPict(int nID) { return m_apPict[nID]; }
	CMotion* GetMotion(void) { return m_pMotion; }
	CCollision GetCollision(void) { return m_collision; }

	//�ݒ�
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	void AddDamage(int nDamage);
	virtual void UnsetTarget(void) = 0;

	//�����蔻��
	void CollisionBlockX(D3DXVECTOR3* pPosNew);
	bool CollisionBlockY(D3DXVECTOR3* pPosNew);
	void CollisionBlockZ(D3DXVECTOR3* pPosNew);

	//�s�N�g���c
	void SetControll(void) { m_bControll = true; }
	void Controll(D3DXVECTOR3 move);
	void Uncontroll(void) { m_bControll = false; }
	bool IsControll(void) { return m_bControll; }

private:
	static CPict* m_apPict[MAX_OBJ];	//�s�N�g����|�C���^
	static int m_nNumAll;				//�s�N�g���񑍐�
	int m_nID;							//�s�N�g����ID
	CMotion* m_pMotion;					//���[�V�����|�C���^
	D3DXVECTOR3 m_pos;					//�ʒu
	D3DXVECTOR3 m_move;					//�ړ���
	float m_fWidth, m_fHeight, m_fDepth;//�T�C�Y
	int m_nCounterJumpTime;				//�W�����v����
	bool m_bJump;						//�W�����v����
	D3DXVECTOR3 m_rot;					//����
	D3DXMATRIX m_mtxWorld;				//���[���h�}�g��
	CModel* m_apModel[PICT_MODEL_NUM];	//�s�N�g���񃂃f��
	CCollision m_collision;				//�R���W����
	int m_nNumModel;					//���f����
	bool m_bControll;					//���c�ł��邩
	CShadow* m_pShadow;					//�e�I�u�W�F�N�g�|�C���^
	int nLife;							//�̗�
};

//�f�X�g���C���[�s�N�g�N���X
class CPictDestroyer : public CPict
{
public:
	//���[�V�������
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,
		MOTIONTYPE_MOVE,
		MOTIONTYPE_DESTROY,
		MOTIONTYPE_MAX
	} MOTIONTYPE;

	//�R���X�g���N�^�E�f�X�g���N�^
	CPictDestroyer();
	CPictDestroyer(const D3DXVECTOR3 pos);
	~CPictDestroyer();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//����
	static CPictDestroyer* Create(const D3DXVECTOR3 pos);

	//�擾
	CBuilding* GetTarget(void) { return m_pTargetBuilding; }
	static CPictDestroyer* GetPict(int nID) { return m_apPict[nID]; }

	//�ݒ�
	void SetTarget(CBuilding* target) { m_pTargetBuilding = target; }
	void UnsetTarget(void) { m_pTargetBuilding = NULL; }

private:
	static CPictDestroyer* m_apPict[MAX_OBJ];	//�s�N�g����|�C���^
	static int m_nNumAll;				//�s�N�g���񑍐�
	int m_nID;							//�s�N�g����ID
	CBuilding* m_pTargetBuilding;		//�ړI�n
	int m_nCounterDestruction;			//�j��J�E���^�[
};

//�u���b�J�[�s�N�g�N���X
class CPictBlocker : public CPict
{
public:
	//���[�V�������
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,
		MOTIONTYPE_MOVE,
		MOTIONTYPE_ATTACK = 3,
		MOTIONTYPE_MAX
	} MOTIONTYPE;

	//�R���X�g���N�^�E�f�X�g���N�^
	CPictBlocker();
	CPictBlocker(const D3DXVECTOR3 pos);
	~CPictBlocker();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//����
	static CPictBlocker* Create(const D3DXVECTOR3 pos);

	//�擾
	CPictPolice* GetTarget(void) { return m_pTargetPolice; }
	static CPictBlocker* GetPict(int nID) { return m_apPict[nID]; }

	//�ݒ�
	void SetTarget(CPictPolice* target) { m_pTargetPolice = target; }
	void UnsetTarget(void) { m_pTargetPolice = NULL; }

private:
	static CPictBlocker* m_apPict[MAX_OBJ];	//�s�N�g����|�C���^
	static int m_nNumAll;				//�s�N�g���񑍐�
	int m_nID;							//�s�N�g����ID

	int m_nCounterAttack;				//�U���J�E���^�[
	CPictPolice* m_pTargetPolice;		//�^�[�Q�b�g�x�@
};

//�s�N�g�x�@�N���X
class CPictPolice : public CPict
{
public:
	//���[�V�������
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,
		MOTIONTYPE_MOVE,
		MOTIONTYPE_ATTACK = 3,
		MOTIONTYPE_MAX
	} MOTIONTYPE;

	//�R���X�g���N�^�E�f�X�g���N�^
	CPictPolice();
	CPictPolice(const D3DXVECTOR3 pos);
	~CPictPolice();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//����
	static CPictPolice* Create(const D3DXVECTOR3 pos);

	//�擾
	CPict* GetTarget(void) { return m_pTargetPict; }
	static CPictPolice* GetPict(int nID) { return m_apPict[nID]; }

	//�ݒ�
	void SetTarget(CPict* target) { m_pTargetPict = target; }
	void UnsetTarget(void) { m_pTargetPict = NULL; }

private:
	static CPictPolice* m_apPict[MAX_OBJ];	//�s�N�g����|�C���^
	static int m_nNumAll;				//�s�N�g���񑍐�
	int m_nID;							//�s�N�g����ID
	CBuilding* m_pTargetBuilding;		//�ړI�n
	int m_nCounterAttack;				//�U���J�E���^�[
	CPict* m_pTargetPict;				//�^�[�Q�b�g�s�N�g�i�����j
};

#endif // !_PICT_H_
