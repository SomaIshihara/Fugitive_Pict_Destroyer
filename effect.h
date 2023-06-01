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
	//コンストラクタ・デストラクタ
	CEffect();
	~CEffect();

	//読み込み
	static HRESULT Load(const char* pPath);
	static void Unload(void);

	//生成
	static CEffect* Create(void);

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャ
};

#endif // !_BG_H_