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
	CNumber();																						//デフォルト
	CNumber(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight);	//オーバーロード（位置向きパターン）
	~CNumber();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//設定
	void SetNumber(const int nSource, const int nDigit);	//もととなる数字と桁数を入れると分割してくれる

	//生成
	static CNumber* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight);

private:
};

#endif // !_OBJ_ANIM2D_H_