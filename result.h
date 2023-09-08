//======================================================
//
//リザルトシーンのヘッダ[result.h]
//Author:石原颯馬
//
//======================================================
#ifndef _RESULT_H_
#define _RESULT_H_

//前方宣言
class CBG;
class CButton2D;
class CFade;

//リザルトクラス
class CResult
{
public:
	//固定値
	static const int HEADLINE_TIME;	//見出し表示時間
	static const int ATK_TIME;		//攻撃数表示時間
	static const int DEST_TIME;		//全壊数表示時間
	static const int VALUE_TIME;	//被害総額表示時間
	static const int RANK_TIME;		//ボタン表示時間
	static const float FADE_ALPHA;	//画面を暗くする具合
	static const float FADE_SPEED;	//暗くする速度

	//コンストラクタ・デストラクタ
	CResult();
	~CResult();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CResult* Create(void);

private:
	CBG* m_pBG;				//背景オブジェ（不透明度いじくるため）
	float m_fAlpha;			//不透明度
	CButton2D* m_pButton;	//ボタン
	int m_nCounter;			//表示タイミングカウンタ
	CFade* m_pFade;
};

#endif // !_RESULT_H_
