//======================================================
//
//カウントダウン処理のヘッダ[countdown.h]
//Author:石原颯馬
//
//======================================================
#ifndef _COUNTDOWN_H_
#define _COUNTDOWN_H_
#include "main.h"
#include "object.h"

//前方宣言
class CObject2D;

class CCountDown
{
public:
	//コンストラクタ・デストラクタ
	CCountDown(const int startCount);
	~CCountDown();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CCountDown* Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const int nCount);

	//取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	int GetCount(void) { return m_count; }

	//設定
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }
	
	//スタート
	void Start(void);

private:
	D3DXVECTOR3 m_pos;		//位置
	CObject2D* m_pFrame;	//枠オブジェクト
	CObject2D** m_ppCount;	//カウントオブジェクト（可変）
	const int m_startCount;	//カウント初期値
	int m_count;			//カウント
	int m_counterSec;		//1秒カウンター
	bool m_shouldCount;		//カウントするかどうか
};

#endif // !_COUNTDOWN_H_