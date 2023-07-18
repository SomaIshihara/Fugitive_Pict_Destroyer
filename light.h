//==========================================
//
//ライトプログラムのヘッダ[light.h]
//Author:石原颯馬
//
//==========================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

//マクロ
#define MAX_LIGHT	(4)	//ライト数

//クラス化したものがこちらです
class CLight
{
public:
	//コンストラクタ・デストラクタ
	CLight();
	~CLight();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

private:
	D3DLIGHT9 m_aLight[MAX_LIGHT];	//ライト構造体
};

#endif // !_LIGHT_H_
