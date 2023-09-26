//======================================================
//
//ピクトさん処理のヘッダ[picto.h]
//Author:石原颯馬
//
//======================================================
#ifndef _PICTO_H_
#define _PICTO_H_

#include "main.h"
#include "manager.h"
#include "object.h"
#include "objectX.h"
#include "collision.h"
#include <stack>

#define PICTO_MAX_LEVEL		(30)	//ピクトの最大レベル
#define PICTO_DAMAGE_ALPHA	(0.9f)	//赤くする割合
#define PICTO_TAXI_MAXLIFE	(10000)

//計算（建物も使用）
#define REQUIRE_EXP(lv)		((int)ceil(pow(1.34f,(lv - 2)) * 20))
#define PICTO_ATK(lv)		((float)ceil(pow(1.125f,(lv - 1)) * 100) * 0.01f)
#define PICTO_HAVENPICTO(lv)	((int)ceil(pow(1.1f,(lv - 1)) * 500))
#define DROP_EXP(lv)		((int)ceil(REQUIRE_EXP(lv + 1) / 1))
#define HAVE_LIFE(lv)		((int)ceil(98 + PICTO_HAVENPICTO(lv) * PICTO_ATK(lv) * 12))
#define HAVE_VALUE(lv)		((long long)ceil(pow(1.5f,(lv - 1)) * lv) * 1000000)

//前方宣言
class CModel;
class CMotion;
class CBuilding;
class CShadow;
class CPictoPolice;
class CPictoNormal;
class CPictoTaxi;
class CMeshField;
class CPoint;

//ピクトさんクラス
class CPicto : public CObject
{
public:
	//静的const
	static const float LOOSE_LENGTH;	//逃がしたものとする距離
	static const int BASE_MODEL_NUM;	//体部分のモデル数

	//経路探索用ノード構造体
	struct Node
	{
		bool bDesition;		//確定かどうか
		float fLengthMin;	//最短距離
		CPoint* pPoint;		//そのノードが属すポイント
		int nFromNum;		//来たノードの番号
	};

	//ピクトさん種類列挙
	typedef enum
	{
		TYPE_DESTROYER = 0,
		TYPE_BLOCKER,
		TYPE_NORMAL,
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
	CPicto();
	CPicto(const D3DXVECTOR3 pos, const TYPE type);
	virtual ~CPicto();

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
	static CPicto* GetPicto(int nID) { return m_apPicto[nID]; }
	CMotion* GetMotion(void) { return m_pMotion; }
	CCollision GetCollision(void) { return m_collision; }
	STATE GetState(void) { return m_state; }
	TYPE GetType(void) { return m_type; }
	static CObject* GetAgit(void) { return m_pAgitObj; }
	static D3DXVECTOR3 GetAgitPos(void) { return m_pAgitObj->GetPos(); }
	CObject* GetTargetObj(void) { return m_pTargetObj; }
	static int GetNumAll(void) { return m_nNumAll; }
	virtual int GetLife(void) = 0;
	
	//設定
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	void SetState(STATE state) { m_state = state; }
	static void SetAgit(CObjectX* pAgit) { m_pAgitObj = pAgit; }
	void SetTargetObj(CObject* pObj);
	virtual void UnsetTarget(void);
	void UnsetTargetAll(void);
	void SetRedAlpha(void) { m_fRedAlpha = PICTO_DAMAGE_ALPHA; }
	void SetModel(const char* pPath);

	//当たり判定
	bool CollisionField(D3DXVECTOR3* pPosNew);

	//乗車
	virtual void TakeTaxi(CPictoTaxi* taxi) = 0;

	//帰宅
	virtual void Return(void);

	//探索
	void Search(CObject* pTarget);

private:
	//変数
	static CPicto* m_apPicto[MAX_OBJ];	//ピクトさんポインタ
	static int m_nNumAll;				//ピクトさん総数
	int m_nID;							//ピクトさんID
	CMotion* m_pMotion;					//モーションポインタ

	D3DXMATRIX m_mtxWorld;				//ワールドマトリ
	CModel** m_ppModel;					//ピクトさんモデル（動的確保）
	int m_nNumModel;					//モデル数
	CCollision m_collision;				//コリジョン

	D3DXVECTOR3 m_pos;					//位置
	D3DXVECTOR3 m_move;					//移動量
	D3DXVECTOR3 m_rot;					//向き
	float m_fWidth, m_fHeight, m_fDepth;//サイズ

	bool m_bJump;						//ジャンプ中か
	int m_nCounterJumpTime;				//ジャンプ時間

	CShadow* m_pShadow;					//影オブジェクトポインタ

	float m_fRedAlpha;					//赤くする割合
	STATE m_state;						//状態
	TYPE m_type;						//ピクト種類

	static CObjectX* m_pAgitObj;		//アジトのポインタ
	static CMeshField* m_pField;		//床

	CObject* m_pTargetObj;				//目的のオブジェ
	CPoint* m_pPoint;					//目的のポイント

	std::stack<CPoint*> m_stack;		//経路スタック
	bool m_bNear;						//目標に近い
};

//デストロイヤーピクトクラス
class CPictoDestroyer : public CPicto
{
public:
	//モーション種類
	typedef enum
	{
		MOTIONTYPE_DESTROY = 2
	} MOTIONTYPE;

	//コンストラクタ・デストラクタ
	CPictoDestroyer();
	CPictoDestroyer(const D3DXVECTOR3 pos, const TYPE type = TYPE_DESTROYER);
	~CPictoDestroyer();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CPictoDestroyer* Create(const D3DXVECTOR3 pos);

	//取得
	static CPictoDestroyer* GetPicto(int nID) { return m_apPicto[nID]; }
	static int GetNumAll(void) { return m_nNumAll; }
	int GetLife(void) { return m_nLife; }
	static int GetLv(void) { return m_nLv; }

	//乗車
	void TakeTaxi(CPictoTaxi* taxi);

	//攻撃
	void AddDamage(int nDamage);

	//経験値付与
	static void AddExp(const int nExp);

	//レベル・経験値リセット
	static void ResetLevelExp(void) { m_nLv = 1; m_nExp = 0; }

	//帰宅
	void Return(void);

private:
	static CPictoDestroyer* m_apPicto[MAX_OBJ];	//ピクトさんポインタ
	static int m_nNumAll;				//ピクトさん総数
	int m_nID;							//ピクトさんID
	int m_nCounterDestruction;			//破壊カウンター

	static int m_nLv;					//レベル
	static int m_nExp;					//所持経験値
	int m_nLife;						//体力
	int m_nHaveNormalPicto;				//一般人ピクト所持数
};

//ブロッカーピクトクラス
class CPictoBlocker : public CPicto
{
public:
	//モーション種類
	typedef enum
	{
		MOTIONTYPE_ATTACK = 3
	} MOTIONTYPE;

	//コンストラクタ・デストラクタ
	CPictoBlocker();
	CPictoBlocker(const D3DXVECTOR3 pos, const TYPE type = TYPE_BLOCKER);
	~CPictoBlocker();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CPictoBlocker* Create(const D3DXVECTOR3 pos);

	//取得
	static CPictoBlocker* GetPicto(int nID) { return m_apPicto[nID]; }
	static int GetNumAll(void) { return m_nNumAll; }
	int GetLife(void) { return m_nLife; }
	static int GetLv(void) { return m_nLv; }

	//乗車
	void TakeTaxi(CPictoTaxi* taxi);

	//攻撃
	void AddDamage(int nDamage);

	//経験値付与
	static void AddExp(const int nExp);

	//レベル・経験値リセット
	static void ResetLevelExp(void) { m_nLv = 1; m_nExp = 0; }

	//帰宅
	void Return(void);

private:
	static CPictoBlocker* m_apPicto[MAX_OBJ];	//ピクトさんポインタ
	static int m_nNumAll;				//ピクトさん総数
	int m_nID;							//ピクトさんID

	int m_nCounterAttack;				//攻撃カウンター

	static int m_nLv;					//レベル
	static int m_nExp;					//所持経験値
	int m_nLife;						//体力
	int m_nHaveNormalPicto;				//一般人ピクト所持数
};

//ピクタクシークラス
class CPictoTaxi : public CPicto
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
	CPictoTaxi(const D3DXVECTOR3 pos, const TYPE type = TYPE_TAXI);
	~CPictoTaxi();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CPictoTaxi* Create(const D3DXVECTOR3 pos);

	//取得
	static CPictoTaxi* GetPicto(int nID) { return m_apPicto[nID]; }
	static int GetNumAll(void) { return m_nNumAll; }
	int GetLife(void) { return m_nLife; }

	//乗車
	void SetTakeTaxi(const CPicto::TYPE type, const int nTakeNum);

	//設定
	void SetMode(const MODE mode) { m_mode = mode; }

	//乗車
	void TakeTaxi(CPictoTaxi* taxi) {};

	//攻撃
	void AddDamage(int nDamage);

	//帰宅
	void Return(void);

private:
	//関数
	CPictoNormal* SearchNormal(void);	//一般人ピクト探索
	CPicto* SearchBattler(void);		//戦闘要員探索

	//変数
	static CPictoTaxi* m_apPicto[MAX_OBJ];	//ピクトさんポインタ
	static int m_nNumAll;					//ピクトさん総数
	int m_nID;								//ピクトさんID

	MODE m_mode;			//タクシーモード

	//ターゲット
	CPicto* m_pTargetPicto;	//ピクト（詳細不明）

	int m_nLife;			//体力

	int m_nTakeDestroyer;	//デストロイヤーが乗っている人数
	int m_nTakeBlocker;		//ブロッカーが乗っている人数
	int m_nTakeNormal;		//一般人が乗っている人数
};

//一般人ピクトクラス
class CPictoNormal : public CPicto
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
	CPictoNormal();
	CPictoNormal(const D3DXVECTOR3 pos, const TYPE type = TYPE_NORMAL);
	~CPictoNormal();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CPictoNormal* Create(const D3DXVECTOR3 pos);
	
	//取得
	static CPictoNormal* GetPicto(int nID) { return m_apPicto[nID]; }
	static int GetNumAll(void) { return m_nNumAll; }
	int GetLife(void) { return -1; }

	//乗車
	void TakeTaxi(CPictoTaxi* taxi);

	//帰宅
	void Return(void);

private:
	static CPictoNormal* m_apPicto[MAX_OBJ];	//ピクトさんポインタ
	static int m_nNumAll;					//ピクトさん総数
	int m_nID;								//ピクトさんID
};

//ピクト警察クラス
class CPictoPolice : public CPicto
{
public:
	//モーション種類
	typedef enum
	{
		MOTIONTYPE_ATTACK = 3
	} MOTIONTYPE;

	//コンストラクタ・デストラクタ
	CPictoPolice();
	CPictoPolice(const D3DXVECTOR3 pos, const TYPE type = TYPE_POLICE);
	~CPictoPolice();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CPictoPolice* Create(const D3DXVECTOR3 pos, const int nLv);

	//取得
	static CPictoPolice* GetPicto(int nID) { return m_apPicto[nID]; }
	static int GetNumAll(void) { return m_nNumAll; }
	int GetLife(void) { return m_nLife; }
	int GetLv(void) { return m_nLv; }

	//設定
	void SetBuilding(CObject* pBui) { m_pTargetBui = pBui; }
	void UnsetTarget(void);

	//建物設定
	void SetBuilding(CBuilding* pBuilding) { SetTargetObj((CObject*)pBuilding); }

	//乗車（するわけがない）
	void TakeTaxi(CPictoTaxi* taxi) {}

	//攻撃
	void AddDamage(int nDamage);

	//帰宅
	void Return(void);

private:
	static CPictoPolice* m_apPicto[MAX_OBJ];	//ピクトさんポインタ
	static int m_nNumAll;				//ピクトさん総数
	int m_nID;							//ピクトさんID
	int m_nCounterAttack;				//攻撃カウンター
	CObject* m_pTargetBui;				//ターゲット（パトロール中建物の記憶）

	int m_nLv;							//レベル
	int m_nLife;						//体力
	int m_nHaveNormalPicto;				//一般人ピクト所持数
};

#endif // !_PICTO_H_
