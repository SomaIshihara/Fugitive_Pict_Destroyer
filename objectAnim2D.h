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
	CObjectAnim2D(int nPriority = 3);																			//デフォルト
	CObjectAnim2D(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, 
		const float fWidth, const float fHeight, const int nAnimSpeed, const bool bLoop, int nPriority = 3);	//オーバーロード（位置向きandアニメーション）
	virtual ~CObjectAnim2D();

	//基本処理
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//アニメーション2D用テクスチャ設定処理
	void BindTexture(int nIdx);

	//生成
	static CObjectAnim2D* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const int nAnimSpeed, const bool bLoop);

private:
	int m_nIdxTexture;	//テクスチャ番号
	int m_nAnimSpeed;	//アニメーション速度
	int m_nCounterAnim;	//アニメーションカウンタ
	int m_nPatternAnim;	//アニメーションパターン
	bool m_bLoop;		//ループするか
};

#endif // !_OBJ_ANIM2D_H_