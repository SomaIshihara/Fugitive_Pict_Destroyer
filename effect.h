//======================================================
//
//エフェクト処理のヘッダ[effect.h]
//Author:石原颯馬
//
//======================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_
#include "main.h"
#include "object2D.h"

class CEffect : public CObject2D
{
public:
	//エフェクト構造体定義
	typedef struct
	{
		D3DXCOLOR col;		//色
		int nLife;			//寿命
	} Effect;

	//コンストラクタ・デストラクタ
	CEffect();								//デフォルト
	CEffect(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, 
		const D3DXCOLOR col, const int nLife);		//オーバーロード（エフェクトいろいろ設定可）
	~CEffect();

	//読み込み
	static HRESULT Load(const char* pPath);
	static void Unload(void);

	//生成
	static CEffect* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight,
		const D3DXCOLOR col, const int nLife);

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャ
	Effect m_effect;						//エフェクト構造体
	const int m_nDefLife;					//設定寿命
};

#endif // !_BG_H_