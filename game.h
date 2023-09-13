//======================================================
//
//ゲームシーンのヘッダ[game.h]
//Author:石原颯馬
//
//======================================================
#ifndef _GAME_H_
#define _GAME_H_
#include "manager.h"

//前方宣言
class CPlayer;
class CMeshField;
class CSlider;
class CPause;
class CResult;
class CTimer;
class CScore;
class CHaveNum;
class CLevel;
class CMeshSky;
class CBG;
class CCountDown;

//タイトルクラス
class CGame : public CScene
{
public:
	//静的const
	static const int CDSTART_TIME;	//カウントダウン開始時間

	//コンストラクタ・デストラクタ
	CGame();
	~CGame();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//取得
	static CPlayer* GetPlayer(void) { return m_pPlayer; }
	static CMeshField* GetMeshField(void) { return m_pMeshField; }
	static int GetATKNum(void) { return m_nATKBuilding; }
	static int GetDestNum(void) { return m_nDestBuilding; }
	static CScore* GetScoreObj(void) { return m_pScore; }
	static CMeshSky* GetSkyObj(void) { return m_pSky; }
	CSlider* GetSlider(void) { return m_pSlider; }
	static CHaveNum** GetHaveNumObj(void) { return &m_pHaveNum[0]; }
	static CLevel** GetLevelObj(void) { return &m_pLevel[0]; }

private:
	void CulcScore(void);				//スコア計算
	CPause* m_pPause;					//ポーズ
	CResult* m_pResult;					//リザルト（背景にゲーム画面を出したいので）
	static CPlayer* m_pPlayer;			//プレイヤー
	CSlider* m_pSlider;					//スライダー
	static CMeshField* m_pMeshField;	//メッシュフィールド
	static CTimer* m_pTimer;			//タイマー
	static CScore* m_pScore;			//スコア
	static CHaveNum* m_pHaveNum[3];		//所持数
	static CLevel* m_pLevel[2];			//レベル
	static CMeshSky* m_pSky;			//空オブジェ
	static int m_nATKBuilding;			//攻撃数
	static int m_nDestBuilding;			//全壊数
	CBG* m_pWarning;					//体力警告の背景ではないやつ
	CCountDown* m_pCountDown;			//カウントダウン
	int m_counterCDStart;				//カウントダウン開始遅延時間
};

#endif // !_GAME_H_
