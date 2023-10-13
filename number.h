//======================================================
//
//数字処理のヘッダ[number.h]
//Author:石原颯馬
//
//======================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"
#include "object2D.h"

class CNumber : public CObject2D
{
public:
	//コンストラクタ・デストラクタ
	CNumber(int nPriority = PRIORITY_UI);										//デフォルト
	CNumber(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,
		const float fWidth, const float fHeight, int nPriority = PRIORITY_UI);	//オーバーロード（位置向きパターン）
	~CNumber();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//数字専用テクスチャ設定
	void BindTexture(int nIdx);

	//設定
	void SetNumber(const long long nSource, const int nDigit);	//もととなる数字と桁数を入れると分割してくれる

	//生成
	static CNumber* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight);

private:
	int m_nIdxTexture;	//テクスチャ番号
};

#endif // !_OBJ_ANIM2D_H_