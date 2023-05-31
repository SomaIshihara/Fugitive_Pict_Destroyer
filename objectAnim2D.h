//======================================================
//
//アニメーションありオブジェクト（2D）処理のヘッダ[objectAnim2D.h]
//Author:石原颯馬
//
//======================================================
#ifndef _OBJ_ANIM2D_H_
#define _OBJ_ANIM2D_H_

#include "main.h"
#include "object2D.h"

class CObjectAnim2D : public CObject2D
{
public:
	//コンストラクタ・デストラクタ
	CObjectAnim2D();																												//デフォルト
	CObjectAnim2D(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, 
		const float fWidth, const float fHeight, const int nPatWidth, const int nPatHeight,
		const int nAnimSpeed, const bool bLoop);	//オーバーロード（位置向きandアニメーション）
	virtual ~CObjectAnim2D();

	//読み込み
	static HRESULT Load(const char* pPath);
	static void Unload(void);

	//基本処理
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//生成
	static CObjectAnim2D* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight,
		const int nPatWidth, const int nPatHeight, const int nAnimSpeed, const bool bLoop);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャ
	int m_nAnimSpeed;	//アニメーション速度
	int m_nCounterAnim;	//アニメーションカウンタ
	int m_nPatternAnim;	//アニメーションパターン
	const int m_nPatternWidth;	//パターン幅
	const int m_nPatternHeight;	//パターン高さ
	bool m_bLoop;				//ループするか
};

#endif // !_OBJ_ANIM2D_H_