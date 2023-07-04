//======================================================
//
//ピクトさん処理のヘッダ[pict.h]
//Author:石原颯馬
//
//======================================================
#ifndef _PICT_H_
#define _PICT_H_

#include "main.h"
#include "manager.h"
#include "object.h"
#include "objectX.h"
#include "collision.h"

#define PICT_MODEL_NUM	(10)	//ピクトさんに使うモデル数

//前方宣言
class CModel;
class CMotion;
class CBuilding;
class CShadow;
class CPictPolice;

//ピクトさんクラス
class CPict : public CObject
{
public:
	//ピクトさん状態列挙
	typedef enum
	{
		STATE_FACE = 0,	//対象に向かっている
		STATE_ATTACK,	//攻撃中
		STATE_LEAVE,		//帰っている
		STATE_MAX
	} STATE;

	//モーション種類
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,
		MOTIONTYPE_MOVE
	} MOTIONTYPE;

	//コンストラクタ・デストラクタ
	CPict();
	CPict(const D3DXVECTOR3 pos);
	virtual ~CPict();

	//基本処理
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//取得
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
	static D3DXVECTOR3 GetAgitPos(void) { return m_pAgitObj->GetPos(); }

	//設定
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	void AddDamage(int nDamage);
	void SetState(STATE state) { m_state = state; }
	static void SetAgit(CObjectX* pAgit) { m_pAgitObj = pAgit; }
	virtual void UnsetTarget(void) = 0;

	//当たり判定
	void CollisionBlockX(D3DXVECTOR3* pPosNew);
	bool CollisionBlockY(D3DXVECTOR3* pPosNew);
	void CollisionBlockZ(D3DXVECTOR3* pPosNew);

	//ピクト操縦
	void SetControll(void) { m_bControll = true; }
	void Controll(D3DXVECTOR3 move);
	void Uncontroll(void) { m_bControll = false; }
	bool IsControll(void) { return m_bControll; }

private:
	static CPict* m_apPict[MAX_OBJ];	//ピクトさんポインタ
	static int m_nNumAll;				//ピクトさん総数
	int m_nID;							//ピクトさんID
	CMotion* m_pMotion;					//モーションポインタ

	D3DXMATRIX m_mtxWorld;				//ワールドマトリ
	CModel* m_apModel[PICT_MODEL_NUM];	//ピクトさんモデル
	int m_nNumModel;					//モデル数
	CCollision m_collision;				//コリジョン

	D3DXVECTOR3 m_pos;					//位置
	D3DXVECTOR3 m_move;					//移動量
	D3DXVECTOR3 m_rot;					//向き
	float m_fWidth, m_fHeight, m_fDepth;//サイズ

	bool m_bJump;						//ジャンプ中か
	int m_nCounterJumpTime;				//ジャンプ時間

	bool m_bControll;					//操縦できるか
	CShadow* m_pShadow;					//影オブジェクトポインタ

	int nLife;							//体力
	float m_fRedAlpha;					//赤くする割合
	STATE m_state;						//状態
	static CObjectX* m_pAgitObj;		//アジトのポインタ
};

//デストロイヤーピクトクラス
class CPictDestroyer : public CPict
{
public:
	//モーション種類
	typedef enum
	{
		MOTIONTYPE_DESTROY = 2
	} MOTIONTYPE;

	//コンストラクタ・デストラクタ
	CPictDestroyer();
	CPictDestroyer(const D3DXVECTOR3 pos);
	~CPictDestroyer();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CPictDestroyer* Create(const D3DXVECTOR3 pos);

	//取得
	CBuilding* GetTarget(void) { return m_pTargetBuilding; }
	static CPictDestroyer* GetPict(int nID) { return m_apPict[nID]; }

	//設定
	void SetTarget(CBuilding* target) { m_pTargetBuilding = target; }
	void UnsetTarget(void);

private:
	static CPictDestroyer* m_apPict[MAX_OBJ];	//ピクトさんポインタ
	static int m_nNumAll;				//ピクトさん総数
	int m_nID;							//ピクトさんID
	CBuilding* m_pTargetBuilding;		//目的地
	int m_nCounterDestruction;			//破壊カウンター
};

//ブロッカーピクトクラス
class CPictBlocker : public CPict
{
public:
	//モーション種類
	typedef enum
	{
		MOTIONTYPE_ATTACK = 3
	} MOTIONTYPE;

	//コンストラクタ・デストラクタ
	CPictBlocker();
	CPictBlocker(const D3DXVECTOR3 pos);
	~CPictBlocker();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CPictBlocker* Create(const D3DXVECTOR3 pos);

	//取得
	CPictPolice* GetTarget(void) { return m_pTargetPolice; }
	static CPictBlocker* GetPict(int nID) { return m_apPict[nID]; }

	//設定
	void SetTarget(CPictPolice* target) { m_pTargetPolice = target; }
	void UnsetTarget(void);

private:
	static CPictBlocker* m_apPict[MAX_OBJ];	//ピクトさんポインタ
	static int m_nNumAll;				//ピクトさん総数
	int m_nID;							//ピクトさんID

	int m_nCounterAttack;				//攻撃カウンター
	CPictPolice* m_pTargetPolice;		//ターゲット警察
};

//ピクト警察クラス
class CPictPolice : public CPict
{
public:
	//モーション種類
	typedef enum
	{
		MOTIONTYPE_ATTACK = 3
	} MOTIONTYPE;

	//コンストラクタ・デストラクタ
	CPictPolice();
	CPictPolice(const D3DXVECTOR3 pos);
	~CPictPolice();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CPictPolice* Create(const D3DXVECTOR3 pos);

	//取得
	CPict* GetTarget(void) { return m_pTargetPict; }
	static CPictPolice* GetPict(int nID) { return m_apPict[nID]; }

	//設定
	void SetTarget(CPict* target) { m_pTargetPict = target; }
	void UnsetTarget(void) { m_pTargetPict = NULL; }

	//仮設定
	void SetBuilding(CBuilding* p) { m_pTargetBuilding = p; }

private:
	static CPictPolice* m_apPict[MAX_OBJ];	//ピクトさんポインタ
	static int m_nNumAll;				//ピクトさん総数
	int m_nID;							//ピクトさんID
	CBuilding* m_pTargetBuilding;		//目的地
	int m_nCounterAttack;				//攻撃カウンター
	CPict* m_pTargetPict;				//ターゲットピクト（味方）
};

#endif // !_PICT_H_
