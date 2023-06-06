//======================================================
//
//背景（2D）処理のヘッダ[bg.h]
//Author:石原颯馬
//
//======================================================
#ifndef _BG_H_
#define _BG_H_
#include "main.h"
#include "object2D.h"

class CBG : public CObject2D
{
public:
	//コンストラクタ・デストラクタ
	CBG(int nPriority = BG_PRIORITY);
	~CBG();

	//読み込み
	static HRESULT Load(const char* pPath);
	static void Unload(void);

	//生成
	static CBG* Create(void);

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャ
};

#endif // !_BG_H_