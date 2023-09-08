//======================================================
//
//弾処理のヘッダ[bullet.h]
//Author:石原颯馬
//
//======================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"
#include "objectAnim2D.h"
#include "objectBillboard.h"
#include "picto.h"

//優先順位
#define BULLET_PRIORITY	(PRIORITY_DEFAULT)

class CBullet2D : public CObject2D
{
public:
	//コンストラクタ・デストラクタ
	CBullet2D(int nPriority = BULLET_PRIORITY);	//デフォルト
	CBullet2D(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fSpeed,
		int nPriority = BULLET_PRIORITY);	//オーバーロード（位置向きandアニメーション）
	~CBullet2D();

	//生成
	static CBullet2D* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fSpeed, const TYPE type);

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//衝突判定
	//bool CollisionEnemy(void);	//敵
	//bool CollisionBlock(void);	//ブロック

	//設定処理
	void SetType(TYPE type) { m_Type = type; }	//タイプ設定

private:
	int m_nIdxTexture;		//テクスチャ番号
	D3DXVECTOR3 m_move;		//移動量
	TYPE m_Type;			//誰の弾か
};

class CBulletBillboard : public CObjectBillboard
{
public:
	//コンストラクタ・デストラクタ
	CBulletBillboard(int nPriority = BULLET_PRIORITY);	//デフォルト
	CBulletBillboard(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fSpeed, const int nPower,
		CPicto* firePicto, int nPriority = BULLET_PRIORITY);	//オーバーロード（位置向きandアニメーション）
	~CBulletBillboard();

	//生成
	static CBulletBillboard* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fSpeed, 
		const int nPower, const CPicto::TYPE type, CPicto* firePicto);

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//衝突判定
	bool CollisionBuilding(void);	//敵
	bool CollisionPicto(void);	//ブロック

private:
	int m_nIdxTexture;		//テクスチャ番号
	D3DXVECTOR3 m_move;		//移動量
	CPicto::TYPE m_Type;		//誰の弾か
	int m_nPower;			//攻撃力
	CPicto* m_pfirePicto;		//発射したピクト
};


#endif // !_PLAYER_H_