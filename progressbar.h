//======================================================
//
//進捗バー処理のヘッダ[bg.h]
//Author:石原颯馬
//
//======================================================
#ifndef _PROGRESSBAR_H_
#define _PROGRESSBAR_H_
#include "main.h"
#include "manager.h"
#include "object.h"

class CObject2D;

class CProgressBar : public CObject
{
public:
	//コンストラクタ・デストラクタ
	CProgressBar(int nPriority = PRIORITY_BG);
	~CProgressBar();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CProgressBar* Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight);

	//取得（純粋仮想関数の関係上実装しているが、こいつに位置やらサイズやらはいらないのですべて0を返す）
	D3DXVECTOR3 GetPos(void) { return CManager::VEC3_ZERO; }
	D3DXVECTOR3 GetRot(void) { return CManager::VEC3_ZERO; }
	float GetWidth(void) { return CManager::FLOAT_ZERO; }
	float GetHeight(void) { return CManager::FLOAT_ZERO; }
	float GetDepth(void) { return CManager::FLOAT_ZERO; }
	float GetParcent(void) { return m_fParcent; }

	//設定
	void SetPercent(const float fParcent);

private:
	CObject2D* m_pFrame;	//枠（こっちで消す用）
	CObject2D* m_pBar;		//可変バー
	float m_fParcent;		//バー割合（0.0f-1.0f）
	float m_fDefWidth;		//初期の幅
};

#endif // !_PROGRESSBAR_H_