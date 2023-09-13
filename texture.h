//======================================================
//
//テクスチャ保存クラスのヘッダ[texture.h]
//Author:石原颯馬
//
//======================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"
#define MAX_STR		(256)
#define MAX_TEXTURE	(128)	//テクスチャ数

//テクスチャ保存クラス
class CTexture
{
public:
	//事前読み込みテクスチャ列挙
	typedef enum
	{
		PRELOAD_TITLELOGO = 0,
		PRELOAD_FADE,
		PRELOAD_SLIDER_01,
		PRELOAD_SLIDER_02,
		PRELOAD_SLIDER_03,
		PRELOAD_HIRE,
		PRELOAD_TITLE_START,
		PRELOAD_TITLE_RANK,
		PRELOAD_PAUSE,
		PRELOAD_PAUSEBUTTON_01,
		PRELOAD_PAUSEBUTTON_02,
		PRELOAD_PAUSEBUTTON_03,
		PRELOAD_PAUSEBUTTON_04,
		PRELOAD_RESULT_HEADLINE,
		PRELOAD_RESULT_01,
		PRELOAD_RESULT_02,
		PRELOAD_RESULT_03,
		PRELOAD_NUMBER,
		PRELOAD_SYMBOL,
		PRELOAD_WARNING_LIFE,
		PRELOAD_HAVEICON_01,
		PRELOAD_HAVEICON_02,
		PRELOAD_HAVEICON_03,
		PRELOAD_TUTORIAL_01,
		PRELOAD_TUTORIAL_02,
		PRELOAD_TUTORIAL_03,
		PRELOAD_TUTORIAL_04,
		PRELOAD_TUTORIAL,
		PRELOAD_LV,
		PRELOAD_ASPHALT,
		PRELOAD_COUNTDOWN_01,
		PRELOAD_COUNTDOWN_02,
	} PRELOAD;

	//パターンテクスチャ構造体定義
	typedef struct
	{
		LPDIRECT3DTEXTURE9 pTexture;	//テクスチャポインタ
		int nPatternWidth;				//パターン幅
		int nPatternHeight;				//パターン高さ
	} PatternTexture;
	//コンストラクタ・デストラクタ
	CTexture();
	~CTexture();

	//全読み込み・破棄
	HRESULT Load(const char* pPath);	//あらかじめ読み込みたいテクスチャ読み込み
	void Unload(void);					//追加した分も含めてすべて破棄

	int Regist(const char* pPath, const int nPatWidth = 1, const int nPatHeight = 1);	//追加する分の読み込み
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);											//テクスチャ取得
	int GetPatWidth(int nIdx);					//パターン幅取得
	int GetPatHeight(int nIdx);					//パターン高さ取得

private:
	PatternTexture m_apTexture[MAX_TEXTURE];		//テクスチャポインタ
	char m_aFilepath[MAX_TEXTURE][MAX_STR];			//テクスチャパス
	static int m_nNumAll;							//テクスチャ総数
};

#endif // !_TEXTURE_H_
