//==========================================
//
//フェード表示プログラムのヘッダ[fade.h]
//Author:石原颯馬
//
//==========================================
#ifndef _FADE_H_
#define _FADE_H_

#include "manager.h"

//フェードクラス
class CFade
{
public:
	//フェードの状態
	typedef enum
	{
		FADE_NONE = 0,	//何もしない
		FADE_IN,		//イン
		FADE_OUT,		//アウト
		FADE_MAX
	} FADE;

	//コンストラクタ・デストラクタ
	CFade();
	~CFade();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CFade* Create(void);

	//取得
	FADE Get(void) { return m_fade; }

private:
	void SetNext(CScene::MODE sceneNext);

	LPDIRECT3DVERTEXBUFFER9 m_pVtxbuff;		//頂点バッファ
	FADE m_fade;							//状態
	CScene::MODE m_modeNext;				//次のシーン
	float m_fAlpha;							//不透明度
};

#endif // !_FADE_H_
