//======================================================
//
//タイトルシーンのヘッダ[title.h]
//Author:石原颯馬
//
//======================================================
#ifndef _TITLE_H_
#define _TITLE_H_
#include "manager.h"

//前方宣言
class CButton3D;
class CPlayer;
class CSlider;
class CFade;

//タイトルクラス
class CTitle : public CScene
{
public:
	//コンストラクタ・デストラクタ
	CTitle();
	~CTitle();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//取得（ないのでぬるぽ）
	CSlider* GetSlider(void) { return nullptr; }

private:
	static CButton3D* m_pButtonStart;
	static CButton3D* m_pButtonRank;
	CFade* m_pFade;
};

#endif // !_TITLE_H_
