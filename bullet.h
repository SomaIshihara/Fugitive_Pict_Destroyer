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

class CBullet : public CObject2D
{
public:
	//コンストラクタ・デストラクタ
	CBullet();	//デフォルト
	CBullet(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fSpeed);	//オーバーロード（位置向きandアニメーション）
	~CBullet();

	//読み込み
	static HRESULT Load(const char* pPath);
	static void Unload(void);

	//生成
	static CBullet* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fSpeed);

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//衝突判定
	bool CollisionEnemy(void);	//敵
	bool CollisionPlayer(void);	//プレイヤー

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャ
	D3DXVECTOR3 m_move;						//移動量
	TYPE m_Type;							//誰の弾か
};

#endif // !_PLAYER_H_