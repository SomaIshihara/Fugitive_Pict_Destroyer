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

//優先順位
#define BULLET_PRIORITY	(PRIORITY_DEFAULT)

class CBullet : public CObject2D
{
public:
	//コンストラクタ・デストラクタ
	CBullet(int nPriority = BULLET_PRIORITY);	//デフォルト
	CBullet(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fSpeed,
		int nPriority = BULLET_PRIORITY);	//オーバーロード（位置向きandアニメーション）
	~CBullet();

	//生成
	static CBullet* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fSpeed, const TYPE type);

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//衝突判定
	bool CollisionEnemy(void);	//敵
#if 0
	bool CollisionPlayer(void);	//プレイヤー
#endif
	bool CollisionBlock(void);	//ブロック

	//設定処理
	void SetType(TYPE type) { m_Type = type; }	//タイプ設定

private:
	int m_nIdxTexture;		//テクスチャ番号
	D3DXVECTOR3 m_move;		//移動量
	TYPE m_Type;			//誰の弾か
};

#endif // !_PLAYER_H_