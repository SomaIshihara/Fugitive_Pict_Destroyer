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
#include "objectX.h"
#include "collision.h"

#define PICT_MODEL_NUM	(10)	//�s�N�g����Ɏg�����f����
#define PICT_MAX_LEVEL	(30)	//�s�N�g�̍ő僌�x��

//�O���錾
class CModel;
class CMotion;
class CBuilding;
class CShadow;
class CPictPolice;
class CPictNormal;
class CPictTaxi;

//�s�N�g����N���X
class CPict : public CObject
{
public:
	//�s�N�g���ʃp�����[�^�\����
	typedef struct
	{
		int nReqExp;		//�v��EXP
		float fATKPow;		//�U���͔{��
		int nUsePictNormal;	//��ʐl�s�N�g�̎g�p�\�l��
	} PictParam;

	//�s�N�g�����ޗ�
	typedef enum
	{
		TYPE_NORMAL = 0,
		TYPE_DESTROYER,
		TYPE_BLOCKER,
		TYPE_TAXI,
		TYPE_MAX
	} TYPE;

	//�s�N�g�����ԗ�
	typedef enum
	{
		STATE_FACE = 0,	//�ΏۂɌ������Ă���
		STATE_ATTACK,	//�U����
		STATE_LEAVE,	//�A���Ă���
		STATE_MAX
	} STATE;

	//���[�V�������
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,
		MOTIONTYPE_MOVE
	} MOTIONTYPE;

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
	STATE GetState(void) { return m_state; }
	int GetLife(void) { return m_nLife; }
	static CObject* GetAgit(void) { return m_pAgitObj; }
	static D3DXVECTOR3 GetAgitPos(void) { return m_pAgitObj->GetPos(); }
	static PictParam GetPictParam(int nLv) { return m_pictParam[nLv]; }
	CObject* GetTargetObj(void) { return m_targetObj; }
	

	//�ݒ�
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	void AddDamage(int nDamage);
	void SetState(STATE state) { m_state = state; }
	static void SetAgit(CObjectX* pAgit) { m_pAgitObj = pAgit; }
	void SetTargetObj(CObject* pObj) { m_targetObj = pObj; }
	void UnsetTargetObj(void) { m_targetObj = GetAgit(); m_state = STATE_LEAVE; }

	//�����蔻��
	//void CollisionBlockX(D3DXVECTOR3* pPosNew);
	bool CollisionBlockY(D3DXVECTOR3* pPosNew);
	//void CollisionBlockZ(D3DXVECTOR3* pPosNew);

	//�s�N�g���c
	void SetControll(void) { m_bControll = true; }
	void Controll(D3DXVECTOR3 move);
	void Uncontroll(void) { m_bControll = false; }
	bool IsControll(void) { return m_bControll; }

	//�ǂݍ���
	void LoadPictParam(const char* pPath);

	//���
	virtual void TakeTaxi(CPictTaxi* taxi) = 0;

private:
	//�֐�
	void Search(void);

	//�ϐ�
	static CPict* m_apPict[MAX_OBJ];	//�s�N�g����|�C���^
	static int m_nNumAll;				//�s�N�g���񑍐�
	int m_nID;							//�s�N�g����ID
	CMotion* m_pMotion;					//���[�V�����|�C���^

	D3DXMATRIX m_mtxWorld;				//���[���h�}�g��
	CModel* m_apModel[PICT_MODEL_NUM];	//�s�N�g���񃂃f��
	int m_nNumModel;					//���f����
	CCollision m_collision;				//�R���W����

	D3DXVECTOR3 m_pos;					//�ʒu
	D3DXVECTOR3 m_move;					//�ړ���
	D3DXVECTOR3 m_rot;					//����
	float m_fWidth, m_fHeight, m_fDepth;//�T�C�Y

	bool m_bJump;						//�W�����v����
	int m_nCounterJumpTime;				//�W�����v����

	bool m_bControll;					//���c�ł��邩
	CShadow* m_pShadow;					//�e�I�u�W�F�N�g�|�C���^

	static PictParam m_pictParam[PICT_MAX_LEVEL];	//�s�N�g�̃p�����[�^

	int m_nLife;						//�̗�
	float m_fRedAlpha;					//�Ԃ����銄��
	STATE m_state;						//���
	static CObjectX* m_pAgitObj;		//�A�W�g�̃|�C���^

	CObject* m_targetObj;				//�ړI�̃I�u�W�F
	D3DXVECTOR3 m_PointPos;				//�ړI�|�C���g�̈ʒu
};

//�f�X�g���C���[�s�N�g�N���X
class CPictDestroyer : public CPict
{
public:
	//���[�V�������
	typedef enum
	{
		MOTIONTYPE_DESTROY = 2
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
	void UnsetTarget(void);

	//���
	void TakeTaxi(CPictTaxi* taxi);

private:
	static CPictDestroyer* m_apPict[MAX_OBJ];	//�s�N�g����|�C���^
	static int m_nNumAll;				//�s�N�g���񑍐�
	int m_nID;							//�s�N�g����ID
	CBuilding* m_pTargetBuilding;		//�ړI�n
	int m_nCounterDestruction;			//�j��J�E���^�[

	static int m_nLevel;				//���x��
	static int m_nExp;					//�o���l
};

//�u���b�J�[�s�N�g�N���X
class CPictBlocker : public CPict
{
public:
	//���[�V�������
	typedef enum
	{
		MOTIONTYPE_ATTACK = 3
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
	void UnsetTarget(void);

	//���
	void TakeTaxi(CPictTaxi* taxi);

private:
	static CPictBlocker* m_apPict[MAX_OBJ];	//�s�N�g����|�C���^
	static int m_nNumAll;				//�s�N�g���񑍐�
	int m_nID;							//�s�N�g����ID

	int m_nCounterAttack;				//�U���J�E���^�[
	CPictPolice* m_pTargetPolice;		//�^�[�Q�b�g�x�@

	static int m_nLevel;				//���x��
	static int m_nExp;					//�o���l
};

//�s�N�^�N�V�[�N���X
class CPictTaxi : public CPict
{
public:
	//�s�N�^�N�V�[���[�h��
	typedef enum
	{
		MODE_PICK = 0,	//���W
		MODE_SABO,		//�T�{��
		MODE_RESCUE,	//�~��
		MODE_MAX
	} MODE;

	//�R���X�g���N�^�E�f�X�g���N�^
	CPictTaxi();
	CPictTaxi(const D3DXVECTOR3 pos);
	~CPictTaxi();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//����
	static CPictTaxi* Create(const D3DXVECTOR3 pos);

	//�擾
	static CPictTaxi* GetPict(int nID) { return m_apPict[nID]; }

	//���
	void SetTakeTaxi(const CPict::TYPE type, const int nTakeNum);

	//�ݒ�
	void SetMode(const MODE mode) { m_mode = mode; }
	void UnsetTarget(void) { m_ptargetPict = NULL; }

	//���
	void TakeTaxi(CPictTaxi* taxi) {};

private:
	//�֐�
	void SearchBullet(void);	//�e�T��
	CPictNormal* SearchNormal(void);	//��ʐl�s�N�g�T��
	CPict* SearchBattler(void);		//�퓬�v���T��

	//�ϐ�
	static CPictTaxi* m_apPict[MAX_OBJ];	//�s�N�g����|�C���^
	static int m_nNumAll;					//�s�N�g���񑍐�
	int m_nID;								//�s�N�g����ID

	MODE m_mode;			//�^�N�V�[���[�h

	//�^�[�Q�b�g
	/*�ق��ق�* pBulletObj;*/
	CPict* m_ptargetPict;	//�s�N�g�i�ڍוs���j

	int m_nTakeDestroyer;	//�f�X�g���C���[������Ă���l��
	int m_nTakeBlocker;		//�u���b�J�[������Ă���l��
	int m_nTakeNormal;		//��ʐl������Ă���l��
};

//��ʐl�s�N�g�N���X
class CPictNormal : public CPict
{
public:
	//���[�V�������
	typedef enum
	{
		MOTIONTYPE_APPERL = 4
	} MOTIONTYPE;

	//�R���X�g���N�^�E�f�X�g���N�^
	CPictNormal();
	CPictNormal(const D3DXVECTOR3 pos);
	~CPictNormal();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//����
	static CPictNormal* Create(const D3DXVECTOR3 pos);
	
	//�擾
	static CPictNormal* GetPict(int nID) { return m_apPict[nID]; }

	//���
	void TakeTaxi(CPictTaxi* taxi);

private:
	static CPictNormal* m_apPict[MAX_OBJ];	//�s�N�g����|�C���^
	static int m_nNumAll;					//�s�N�g���񑍐�
	int m_nID;								//�s�N�g����ID
};

//�s�N�g�x�@�N���X
class CPictPolice : public CPict
{
public:
	//���[�V�������
	typedef enum
	{
		MOTIONTYPE_ATTACK = 3
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

	//���ݒ�
	void SetBuilding(CBuilding* p) { m_pTargetBuilding = p; }

	//���
	void TakeTaxi(CPictTaxi* taxi) {}

private:
	static CPictPolice* m_apPict[MAX_OBJ];	//�s�N�g����|�C���^
	static int m_nNumAll;				//�s�N�g���񑍐�
	int m_nID;							//�s�N�g����ID
	CBuilding* m_pTargetBuilding;		//�ړI�n
	int m_nCounterAttack;				//�U���J�E���^�[
	CPict* m_pTargetPict;				//�^�[�Q�b�g�s�N�g�i�����j
};

#endif // !_PICT_H_
