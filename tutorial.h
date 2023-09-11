//======================================================
//
//チュートリアルシーンのヘッダ[tutorial.h]
//Author:石原颯馬
//
//======================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_
#include "manager.h"
#include "texture.h"

#define TUTORIAL_NUM	(CTexture::PRELOAD_TUTORIAL_04 - CTexture::PRELOAD_TUTORIAL_01 + 1)

//前方宣言
class CPlayer;
class CMeshField;
class CSlider;
class CPause;
class CScore;
class CHaveNum;
class CMeshSky;
class CBG;
class CButton2D;
class CFade;

//タイトルクラス
class CTutorial : public CScene
{
public:
	//静的const
	static const int TUTORIAL_DELETE_TIME;	//チュートリアルを消すのに長押しする時間
	static const float TUTORIAL_ALPHA_DEF;	//チュートリアルを半透明にするが、その不透明度

	//コンストラクタ・デストラクタ
	CTutorial();
	~CTutorial();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//取得
	static CPlayer* GetPlayer(void) { return m_pPlayer; }
	static CMeshField* GetMeshField(void) { return m_pMeshField; }
	static CScore* GetScoreObj(void) { return m_pScore; }
	static CMeshSky* GetSkyObj(void) { return m_pSky; }
	CSlider* GetSlider(void) { return m_pSlider; }
	static CHaveNum** GetHaveNumObj(void) { return &m_pHaveNum[0]; }

private:
	void CulcScore(void);					//スコア計算
	static CPlayer* m_pPlayer;				//プレイヤー
	CSlider* m_pSlider;						//スライダー
	static CMeshField* m_pMeshField;		//メッシュフィールド
	static CScore* m_pScore;				//スコア
	static CHaveNum* m_pHaveNum[3];			//所持数
	static CMeshSky* m_pSky;				//空オブジェ
	CBG* m_pWarning;						//体力警告の背景ではないやつ
	CButton2D* m_pTutorial[TUTORIAL_NUM];	//チュートリアルオブジェクト（かざしたら半透明にしたい）
	int m_nTutorialDelCnt[TUTORIAL_NUM];	//チュートリアル削除カウンタ
	CFade* m_pFade;
};

#endif // !_TUTORIAL_H_
