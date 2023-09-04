//======================================================
//
//リザルトシーンのヘッダ[result.h]
//Author:石原颯馬
//
//======================================================
#ifndef _RESULT_H_
#define _RESULT_H_

//前方宣言
class CButton2D;

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

	//コンストラクタ・デストラクタ
	CResult();
	~CResult();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CButton2D* m_pButton;
	int m_nCounter;
};

#endif // !_RESULT_H_
