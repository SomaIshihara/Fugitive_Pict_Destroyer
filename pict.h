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

#define PICT_MODEL_NUM	(10)	//ピクトさんに使うモデル数

//前方宣言
class CModel;
class CMotion;
class CBuilding;
class CShadow;

//ピクトさんクラス
class CPict : public CObject
{
public:
	//コンストラクタ・デストラクタ
	CPict();
	CPict(const D3DXVECTOR3 pos);
	~CPict();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CPict* Create(const D3DXVECTOR3 pos);

	//取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	float GetDepth(void) { return m_fDepth; }
	int GetModelIdx(void) { return -1; }
	static CPict* GetPict(int nID) { return m_apPict[nID]; }
	CBuilding* GetTarget(void) { return m_pTargetBuilding; }
	D3DXVECTOR3 GetMove(void) { return m_move; }

	//設定
	void SetTarget(CBuilding* target) { m_pTargetBuilding = target; }
	void UnsetTarget(void) { m_pTargetBuilding = NULL; }

	//当たり判定
	void CollisionBlockX(D3DXVECTOR3* pPosNew);
	bool CollisionBlockY(D3DXVECTOR3* pPosNew);
	void CollisionBlockZ(D3DXVECTOR3* pPosNew);

	//ピクト操縦
	void SetControll(void) { m_bControll = true; }
	void Controll(D3DXVECTOR3 move);
	void Uncontroll(void) { m_bControll = false; }

private:
	static CPict* m_apPict[MAX_OBJ];	//ピクトさんポインタ
	static int m_nNumAll;				//ピクトさん総数
	int m_nID;							//ピクトさんID
	CMotion* m_pMotion;					//モーションポインタ
	D3DXVECTOR3 m_pos;					//位置
	D3DXVECTOR3 m_move;					//移動量
	float m_fWidth, m_fHeight, m_fDepth;//サイズ
	int m_nCounterJumpTime;				//ジャンプ時間
	bool m_bJump;						//ジャンプ中か
	D3DXVECTOR3 m_rot;					//向き
	D3DXMATRIX m_mtxWorld;				//ワールドマトリ
	CModel* m_apModel[PICT_MODEL_NUM];	//ピクトさんモデル
	int m_nNumModel;					//モデル数

	CBuilding* m_pTargetBuilding;		//目的地
	int m_nCounterDestruction;			//破壊カウンター

	bool m_bControll;					//操縦できるか

	CShadow* m_pShadow;					//影オブジェクトポインタ
};

#endif // !_PICT_H_
