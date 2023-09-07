//======================================================
//
//ポーズ画面のヘッダ[game.h]
//Author:石原颯馬
//
//======================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//前方宣言
class CObject2D;
class CButton2D;
class CBG;

//タイトルクラス
class CPause
{
public:
	//コンストラクタ・デストラクタ
	CPause();
	~CPause();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//部分破棄
	void ReleaseSome(void);

private:
	CBG* m_pBG;					//背景
	CObject2D* m_pPolygon;		//機能を持たないポリゴン
	CButton2D* m_pContinue;		//再開ボタン
	CButton2D* m_pRestart;		//リスタートボタン
	CButton2D* m_pExit;			//終了ボタン
	CButton2D* m_pScreenShot;	//スクショモードボタン
	bool m_bScreenShotMode;		//スクショモードか
};

#endif // !_GAME_H_
