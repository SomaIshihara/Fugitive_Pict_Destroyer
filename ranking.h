//======================================================
//
//ランキングシーンのヘッダ[ranking.h]
//Author:石原颯馬
//
//======================================================
#ifndef _RANKING_H_
#define _RANKING_H_
#include "manager.h"

//前方宣言
class CFade;

//タイトルクラス
class CRanking : public CScene
{
public:
	//静的const
	static const int MAX_RANK;					//順位総数
	static const int RANK_DISP_X;				//表示数
	static const int RANK_DISP_Y;
	static const D3DXVECTOR3 RANK_START_POS;	//1位のスタート地点
	static const float RANK_X_DISTANCE;			//Xの描画間隔
	static const float RANK_Y_DISTANCE;			//Yの描画間隔
	static const float X_ONE_WIDTH;				//Xの描画サイズ幅
	static const float Y_ONE_HEIGHT;			//Xの描画サイズ幅
	static const float X_SCORE_DIST;			//Xのスコアと位の間隔
	static const int SYMBOL_RANK_NUM;			//「位」の位置

	//コンストラクタ・デストラクタ
	CRanking();
	~CRanking();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//設定
	static void Set(const long long nScore);

	//取得（ないのでぬるぽ）
	CSlider* GetSlider(void) { return nullptr; }

private:
	static void Load(long long* pRanking);
	static void Save(long long* pRanking);
	CFade* m_pFade;
};

#endif // !_RANKING_H_
