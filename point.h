//======================================================
//
//移動ポイントのヘッダ[point.h]
//Author:石原颯馬
//
//======================================================
#ifndef _POINT_H_
#define _POINT_H_

#include <vector>

//前方宣言
class CObjectX;

//ポイントクラス
class CPoint
{
public:
	//静的const
	static const float MERGE_LENGTH;	//ポイントをマージする範囲
	static const float POINT_INTERVAL;	//ポイントの配置間隔

	//コンストラクタ・デストラクタ
	CPoint();
	~CPoint();

	//基本処理
	HRESULT Init(void);
	static void Update(void);
	static void ReleaseAll(void);
	void Release(void);

	//生成
	static CPoint* Create(const D3DXVECTOR3 pos);

	//探索
	static CPoint* Search(const D3DXVECTOR3 pos, const CPoint* pNowPoint = nullptr);

	//リスト
	static CPoint* GetTop(void) { return m_pTop; }
	CPoint* GetNext(void) { return m_pNext; }

	//取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	static int GetNumAll(void) { return m_nNumAll; }
	std::vector<CPoint*> GetConnect(void) { return m_connectPoint; }

private:
	static void Death(void);	//死亡フラグと化した旗をへし折る

	static CPoint* m_pTop;	//先頭オブジェクト
	static CPoint* m_pCur;	//最後尾オブジェクト
	CPoint* m_pNext;		//次のオブジェクト
	CPoint* m_pPrev;		//前のオブジェクト
	bool m_bDeath;			//死亡フラグ
	static int m_nNumAll;	//総数
	D3DXVECTOR3 m_pos;		//位置
	CObjectX* m_pFrag;		//デバッグ用旗

	std::vector<CPoint*> m_connectPoint;	//接続しているポイント
};

#endif // !_POINT_H_
