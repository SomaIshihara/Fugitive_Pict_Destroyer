//======================================================
//
//プレイヤー処理のヘッダ[player.h]
//Author:石原颯馬
//
//======================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "objectAnim2D.h"

class CPlayer : public CObjectAnim2D
{
public:
	//コンストラクタ・デストラクタ
	CPlayer();	//デフォルト
	CPlayer(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight,
		const int nPatWidth, const int nPatHeight, const int nAnimSpeed);	//オーバーロード（位置向きandアニメーション）
	~CPlayer();

	//読み込み
	static HRESULT Load(const char* pPath);
	static void Unload(void);

	//生成
	static CPlayer* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight,
		const int nPatWidth, const int nPatHeight, const int nAnimSpeed);

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//設定
	void AddDamage(int nDamage);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャ
	D3DXVECTOR3 m_move;						//移動量
	int m_nLife;							//体力
};

#endif // !_PLAYER_H_