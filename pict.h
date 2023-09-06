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

#define PICT_MODEL_NUM		(10)	//ピクトさんに使うモデル数
#define PICT_MAX_LEVEL		(30)	//ピクトの最大レベル
#define PICT_DAMAGE_ALPHA	(0.9f)	//赤くする割合

//計算（建物も使用）
#define REQUIRE_EXP(lv)		((int)ceil(pow(1.34f,(lv - 2)) * 20))
#define PICT_ATK(lv)		((float)ceil(pow(1.125f,(lv - 1)) * 100) * 0.01f)
#define PICT_HAVENPICT(lv)	((int)ceil(pow(1.1f,(lv - 1)) * 500))
#define DROP_EXP(lv)		((int)ceil(REQUIRE_EXP(lv + 1) / 3))
#define HAVE_LIFE(lv)		((int)ceil(98 + PICT_HAVENPICT(lv) * PICT_ATK(lv) * 12))
#define HAVE_VALUE(lv)		((long long)ceil(pow(1.2f,(lv - 1)) * lv) * 1000000)

//前方宣言
class CModel;
class CMotion;
class CBuilding;
class CShadow;
class CPictPolice;
class CPictNormal;
class CPictTaxi;
class CItemBullet;

//ピクトさんクラス
class CPict : public CObject
{
public:
	//ピクト共通パラメータ構造体
	typedef struct
	{
		int nReqExp;		//要求EXP
		float fATKPow;		//攻撃力倍率
		int nUsePictNormal;	//一般人ピクトの使用可能人数
	} PictParam;

	//ピクトさん種類列挙
	typedef enum
	{
		TYPE_NORMAL = 0,
		TYPE_DESTROYER,
		TYPE_BLOCKER,
		TYPE_TAXI,
		TYPE_POLICE,
		TYPE_MAX
	} TYPE;

	//ピクトさん状態列挙
	typedef enum
	{
		STATE_FACE = 0,	//対象に向かっている
		STATE_ATTACK,	//攻撃中
		STATE_LEAVE,	//帰っている
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
	CPict(const D3DXVECTOR3 pos, const TYPE type);
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
	TYPE GetType(void) { return m_type; }
	static CObject* GetAgit(void) { return m_pAgitObj; }
	static D3DXVECTOR3 GetAgitPos(void) { return m_pAgitObj->GetPos(); }
	static PictParam GetPictParam(int nLv) { return m_pictParam[nLv]; }
	CObject* GetTargetObj(void) { return m_targetObj; }
	static int GetNumAll(void) { return m_nNumAll; }
	
	//設定
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	void SetState(STATE state) { m_state = state; }
	static void SetAgit(CObjectX* pAgit) { m_pAgitObj = pAgit; }
	void SetTargetObj(CObject* pObj);
	void UnsetTargetObj(void);
	void SetRedAlpha(void) { m_fRedAlpha = PICT_DAMAGE_ALPHA; }

	//当たり判定
	bool CollisionField(D3DXVECTOR3* pPosNew);

	//ピクト操縦
	void SetControll(void) { m_bControll = true; }
	void Controll(D3DXVECTOR3 move);
	void Uncontroll(void) { m_bControll = false; }
	bool IsControll(void) { return m_bControll; }

	//読み込み
	void LoadPictParam(const char* pPath);

	//乗車
	virtual void TakeTaxi(CPictTaxi* taxi) = 0;

private:
	//関数
	void Search(void);

	//変数
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

	static PictParam m_pictParam[PICT_MAX_LEVEL];	//ピクトのパラメータ

	float m_fRedAlpha;					//赤くする割合
	STATE m_state;						//状態
	TYPE m_type;						//ピクト種類

	static CObjectX* m_pAgitObj;		//アジトのポインタ

	CObject* m_targetObj;				//目的のオブジェ
	D3DXVECTOR3 m_PointPos;				//目的ポイントの位置
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
	CPictDestroyer(const D3DXVECTOR3 pos, const TYPE type = TYPE_DESTROYER);
	~CPictDestroyer();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CPictDestroyer* Create(const D3DXVECTOR3 pos);

	//取得
	static CPictDestroyer* GetPict(int nID) { return m_apPict[nID]; }
	static int GetNumAll(void) { return m_nNumAll; }
	int GetLife(void) { return m_nLife; }
	static int GetLv(void) { return m_nLv; }

	//乗車
	void TakeTaxi(CPictTaxi* taxi);

	//攻撃
	void AddDamage(int nDamage);

	//経験値付与
	static void AddExp(const int nExp);

	//レベル・経験値リセット
	static void ResetLevelExp(void) { m_nLv = 1; m_nExp = 0; }

private:
	static CPictDestroyer* m_apPict[MAX_OBJ];	//ピクトさんポインタ
	static int m_nNumAll;				//ピクトさん総数
	int m_nID;							//ピクトさんID
	int m_nCounterDestruction;			//破壊カウンター

	static int m_nLv;					//レベル
	static int m_nExp;					//所持経験値
	int m_nLife;						//体力
	int m_nHaveNormalPict;				//一般人ピクト所持数
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
	CPictBlocker(const D3DXVECTOR3 pos, const TYPE type = TYPE_BLOCKER);
	~CPictBlocker();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CPictBlocker* Create(const D3DXVECTOR3 pos);

	//取得
	static CPictBlocker* GetPict(int nID) { return m_apPict[nID]; }
	static int GetNumAll(void) { return m_nNumAll; }
	int GetLife(void) { return m_nLife; }
	static int GetLv(void) { return m_nLv; }

	//乗車
	void TakeTaxi(CPictTaxi* taxi);

	//攻撃
	void AddDamage(int nDamage);

	//経験値付与
	static void AddExp(const int nExp);

	//レベル・経験値リセット
	static void ResetLevelExp(void) { m_nLv = 1; m_nExp = 0; }

private:
	static CPictBlocker* m_apPict[MAX_OBJ];	//ピクトさんポインタ
	static int m_nNumAll;				//ピクトさん総数
	int m_nID;							//ピクトさんID

	int m_nCounterAttack;				//攻撃カウンター

	static int m_nLv;					//レベル
	static int m_nExp;					//所持経験値
	int m_nLife;						//体力
	int m_nHaveNormalPict;				//一般人ピクト所持数
};

//ピクタクシークラス
class CPictTaxi : public CPict
{
public:
	//静的const
	static const float RESCUE_LIFE;	//救助する体力

	//ピクタクシーモード列挙
	typedef enum
	{
		MODE_PICK = 0,	//収集
		MODE_SABO,		//サボり
		MODE_RESCUE,	//救助
		MODE_MAX
	} MODE;

	//コンストラクタ・デストラクタ
	CPictTaxi();
	CPictTaxi(const D3DXVECTOR3 pos, const TYPE type = TYPE_TAXI);
	~CPictTaxi();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CPictTaxi* Create(const D3DXVECTOR3 pos);

	//取得
	static CPictTaxi* GetPict(int nID) { return m_apPict[nID]; }
	static int GetNumAll(void) { return m_nNumAll; }
	int GetLife(void) { return m_nLife; }

	//乗車
	void SetTakeTaxi(const CPict::TYPE type, const int nTakeNum);

	//拾得
	void SetBullet(const int nBullet) { m_nHaveBullet += nBullet; }

	//設定
	void SetMode(const MODE mode) { m_mode = mode; }
	void UnsetTargetPict(void) { m_ptargetPict = NULL; }

	//乗車
	void TakeTaxi(CPictTaxi* taxi) {};

	//攻撃
	void AddDamage(int nDamage);

private:
	//関数
	bool SearchPick(void);				//アイテム類探す
	CItemBullet* SearchBullet(void);	//弾探索
	CPictNormal* SearchNormal(void);	//一般人ピクト探索
	CPict* SearchBattler(void);		//戦闘要員探索

	//変数
	static CPictTaxi* m_apPict[MAX_OBJ];	//ピクトさんポインタ
	static int m_nNumAll;					//ピクトさん総数
	int m_nID;								//ピクトさんID

	MODE m_mode;			//タクシーモード

	//ターゲット
	CItemBullet* m_pItemBullet;	//アイテム弾
	CPict* m_ptargetPict;		//ピクト（詳細不明）

	int m_nLife;			//体力

	int m_nTakeDestroyer;	//デストロイヤーが乗っている人数
	int m_nTakeBlocker;		//ブロッカーが乗っている人数
	int m_nTakeNormal;		//一般人が乗っている人数
	int m_nHaveBullet;		//弾の所持数
};

//一般人ピクトクラス
class CPictNormal : public CPict
{
public:
	//静的const
	static const int NORMAL_D_PERCENT;	//一般人ピクトがデストロイヤーになる確率
	static const int NORMAL_B_PERCENT;	//一般人ピクトがブロッカーになる確率
	static const int NORMAL_N_PERCENT;	//一般人ピクトでした
	static const int NORMAL_NUM_MIN;	//一般人ピクトの最低人数
	static const int NORMAL_NUM_DEGREE;	//一般人ピクトの人数振れ幅

	//モーション種類
	typedef enum
	{
		MOTIONTYPE_APPERL = 4
	} MOTIONTYPE;

	//コンストラクタ・デストラクタ
	CPictNormal();
	CPictNormal(const D3DXVECTOR3 pos, const TYPE type = TYPE_NORMAL);
	~CPictNormal();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CPictNormal* Create(const D3DXVECTOR3 pos);
	
	//取得
	static CPictNormal* GetPict(int nID) { return m_apPict[nID]; }
	static int GetNumAll(void) { return m_nNumAll; }

	//乗車
	void TakeTaxi(CPictTaxi* taxi);

private:
	static CPictNormal* m_apPict[MAX_OBJ];	//ピクトさんポインタ
	static int m_nNumAll;					//ピクトさん総数
	int m_nID;								//ピクトさんID
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
	CPictPolice(const D3DXVECTOR3 pos, const TYPE type = TYPE_POLICE);
	~CPictPolice();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CPictPolice* Create(const D3DXVECTOR3 pos);

	//取得
	CPict* GetTargetPict(void) { return m_pTargetPict; }
	static CPictPolice* GetPict(int nID) { return m_apPict[nID]; }
	static int GetNumAll(void) { return m_nNumAll; }
	int GetLife(void) { return m_nLife; }
	int GetLv(void) { return m_nLv; }

	//設定
	void SetTargetPict(CPict* target) { m_pTargetPict = target; }
	void UnsetTarget(void) { m_pTargetPict = NULL; }

	//建物設定
	void SetBuilding(CBuilding* pBuilding) { SetTargetObj((CObject*)pBuilding); }

	//乗車（するわけがない）
	void TakeTaxi(CPictTaxi* taxi) {}

	//攻撃
	void AddDamage(int nDamage);

private:
	static CPictPolice* m_apPict[MAX_OBJ];	//ピクトさんポインタ
	static int m_nNumAll;				//ピクトさん総数
	int m_nID;							//ピクトさんID
	int m_nCounterAttack;				//攻撃カウンター
	CPict* m_pTargetPict;				//ターゲットピクト（味方）

	int m_nLv;					//レベル
	int m_nLife;						//体力
	int m_nHaveNormalPict;				//一般人ピクト所持数
};

#endif // !_PICT_H_
