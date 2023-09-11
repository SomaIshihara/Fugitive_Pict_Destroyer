//======================================================
//
//レベル処理のヘッダ[havenum.h]
//Author:石原颯馬
//
//======================================================
#ifndef _LEVEL_H_
#define _LEVEL_H_
#include "main.h"
#include "object.h"
#include "manager.h"

class CNumber;
class CSymbol;

class CLevel : public CObject
{
public:
	//記号列挙
	static const int m_nSymbolX;

	//コンストラクタ・デストラクタ
	CLevel(int nPriority = PRIORITY_UI);				//デフォルト
	CLevel(const int nLevelDigit, int nPriority = PRIORITY_UI);	//オーバーロード
	~CLevel();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CLevel* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fOneWidth, const float fOneHeight, const int nLevelDigit,
		const int nIconTexNum);	//オブジェクトを生成(fOneWidth,fOneHeight:1桁当たりのサイズ)

	//取得（純粋仮想関数の関係上実装しているが、こいつにサイズやらはいらないのですべて0を返す）
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return CManager::FLOAT_ZERO; }
	float GetHeight(void) { return CManager::FLOAT_ZERO; }
	float GetDepth(void) { return CManager::FLOAT_ZERO; }
	int GetModelIdx(void) { return -1; }
	int GetLevel(void) { return m_nLevel; }

	//設定
	void SetNum(const int nNum) { m_nLevel = nNum; }
	void AddNum(const int nNum) { m_nLevel += nNum; }

private:
	void CutNumber(void);				//数字分割
	CNumber** m_ppNumber;				//数字
	int m_nIdxTexture;					//テクスチャ番号
	D3DXVECTOR3 m_pos;					//位置（1桁目の数字の中心を想定）
	D3DXVECTOR3 m_rot;					//向き（1桁目の数字の中心を想定）
	float m_fOneWidth;					//1桁当たりのサイズ幅
	float m_fOneHeight;					//1桁当たりのサイズ高さ
	int m_nLevel;						//所持数
	const int m_nLevelDigit;			//桁数
};

#endif // !_TIME_H_