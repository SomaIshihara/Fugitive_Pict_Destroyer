//======================================================
//
//スコア処理のヘッダ[score.h]
//Author:石原颯馬
//
//======================================================
#ifndef _SCORE_H_
#define _SCORE_H_
#include "main.h"
#include "object2D.h"

#define SCORE_DIGIT	(8)	//スコアの桁数
class CNumber;

class CScore : public CObject
{
public:
	//コンストラクタ・デストラクタ
	CScore();				//デフォルト
	CScore(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fOneWidth, const float fOneHeight);				//オーバーロード
	~CScore();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//読み込み
	static HRESULT Load(const char* pPath, int nPatWidth, int nPatHeight);
	static void Unload(void);

	//生成
	static CScore* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fOneWidth, const float fOneHeight);	//オブジェクトを生成(fOneWidth,fOneHeight:1桁当たりのサイズ)

	//取得（純粋仮想関数の関係上実装しているが、こいつにサイズやらはいらないのですべて0を返す）
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return FLOAT_ZERO; }
	float GetHeight(void) { return FLOAT_ZERO; }

	//設定
	void Set(const int nScore);	//スコア設定
	void Add(const int nAdd);	//スコア加算

private:
	void CutNumber(void);					//数字分割
	CNumber* m_pNumber[SCORE_DIGIT];		//数字
	static PatternTexture m_patTexture;		//テクスチャ
	D3DXVECTOR3 m_pos;						//位置（1桁目の数字の中心を想定）
	D3DXVECTOR3 m_rot;						//向き（1桁目の数字の中心を想定）
	float m_fOneWidth;						//1桁当たりのサイズ幅
	float m_fOneHeight;						//1桁当たりのサイズ高さ
	int m_nScore;							//スコア
};

#endif // !_SCORE_H_