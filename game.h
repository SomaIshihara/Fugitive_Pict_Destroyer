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
class CMeshSky;

//タイトルクラス
class CGame : public CScene
{
public:
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
	static CSlider* GetSlider(void) { return m_pSlider; }
	static CMeshField* GetMeshField(void) { return m_pMeshField; }
	static int GetATKNum(void) { return m_nATKBuilding; }
	static int GetDestNum(void) { return m_nDestBuilding; }
	static CScore* GetScoreObj(void) { return m_pScore; }
	static CMeshSky* GetSkyObj(void) { return m_pSky; }

private:
	void CulcScore(void);	//スコア計算
	CPause* m_pPause;					//ポーズ
	CResult* m_pResult;					//リザルト（背景にゲーム画面を出したいので）
	static CPlayer* m_pPlayer;			//プレイヤー
	static CSlider* m_pSlider;			//スライダー
	static CMeshField* m_pMeshField;	//メッシュフィールド
	static CTimer* m_pTimer;			//タイマー
	static CScore* m_pScore;			//スコア
	static CHaveNum* m_pHaveNum[3];		//所持数
	static CMeshSky* m_pSky;			//空オブジェ
	static int m_nATKBuilding;			//攻撃数
	static int m_nDestBuilding;			//全壊数
};

#endif // !_GAME_H_
