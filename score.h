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

#define SCORE_DIGIT	(4)	//スコアの桁数

class CNumber;
class CSymbol;

class CScore : public CObject
{
public:
	//スコア記号列挙
	typedef enum
	{
		TYPE_HMILLION = 0,
		TYPE_TEN_HUNDRED,
		TYPE_PIC = 7,
		TYPE_MAX
	} SYMBOLTYPE;

	//コンストラクタ・デストラクタ
	CScore(int nPriority = PRIORITY_UI);				//デフォルト
	CScore(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fOneWidth, const float fOneHeight, int nPriority = PRIORITY_UI);	//オーバーロード
	~CScore();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CScore* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fOneWidth, const float fOneHeight);	//オブジェクトを生成(fOneWidth,fOneHeight:1桁当たりのサイズ)

	//取得（純粋仮想関数の関係上実装しているが、こいつにサイズやらはいらないのですべて0を返す）
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return CManager::FLOAT_ZERO; }
	float GetHeight(void) { return CManager::FLOAT_ZERO; }
	float GetDepth(void) { return CManager::FLOAT_ZERO; }
	int GetModelIdx(void) { return -1; }
	long long GetScore(void) { return m_nScore; }

	//設定
	void Set(const long long nScore);	//スコア設定
	void Add(const long long nAdd);	//スコア加算

private:
	//関数
	void CutNumber(void);				//数字分割

	//変数
	CNumber* m_pNumberHMillion[SCORE_DIGIT];	//億の数字
	CNumber* m_pNumberTenHundred[SCORE_DIGIT];	//万の数字
	int m_nIdxTextureNumber;			//テクスチャ番号（数字）
	int m_nIdxtextureSymbol;			//テクスチャ番号（記号）
	D3DXVECTOR3 m_pos;					//位置（1桁目の数字の中心を想定）
	D3DXVECTOR3 m_rot;					//向き（1桁目の数字の中心を想定）
	float m_fOneWidth;					//1桁当たりのサイズ幅
	float m_fOneHeight;					//1桁当たりのサイズ高さ
	long long m_nScore;			//スコア
};

#endif // !_SCORE_H_