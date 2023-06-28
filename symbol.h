//======================================================
//
//記号処理のヘッダ[symbol.h]
//Author:石原颯馬
//
//======================================================
#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include "main.h"
#include "object2D.h"

class CSymbol : public CObject2D
{
public:
	//コンストラクタ・デストラクタ
	CSymbol(int nPriority = PRIORITY_UI);										//デフォルト
	CSymbol(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,
		const float fWidth, const float fHeight, int nPriority = PRIORITY_UI);	//オーバーロード（位置向きパターン）
	~CSymbol();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//数字専用テクスチャ設定
	void BindTexture(int nIdx);

	//設定
	void SetSymbol(const int nNum);	//もととなる数字と桁数を入れると分割してくれる

	//生成
	static CSymbol* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight);

private:
	int m_nIdxTexture;	//テクスチャ番号
};

#endif // !_OBJ_ANIM2D_H_