//======================================================
//
//爆発処理のヘッダ[explosion.h]
//Author:石原颯馬
//
//======================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "main.h"
#include "objectAnim2D.h"

class CExplosion : public CObjectAnim2D
{
public:
	//コンストラクタ・デストラクタ
	CExplosion();														//デフォルト
	CExplosion(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,
		const float fWidth, const float fHeight, const int nAnimSpeed);	//オーバーロード（位置向きandアニメーション）
	virtual ~CExplosion();

	//基本処理
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//生成
	static CExplosion* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const int nAnimSpeed);

private:
	int m_nIdxTexture;	//テクスチャ番号
};

#endif // !_OBJ_ANIM2D_H_