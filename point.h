//======================================================
//
//移動ポイントのヘッダ[point.h]
//Author:石原颯馬
//
//======================================================
#ifndef _POINT_H_
#define _POINT_H_

//ポイントクラス
class CPoint
{
public:
	//コンストラクタ・デストラクタ
	CPoint();
	~CPoint();

	//基本処理
	HRESULT Init(void);
	static void Update(void);
	static void ReleaseAll(void);

	//生成
	static CPoint* Create(const D3DXVECTOR3 pos);

	//探索
	static CPoint* Search(const D3DXVECTOR3 pos, const CPoint* pNowPoint = NULL);

	//リスト
	static CPoint* GetTop(void) { return m_pTop; }
	CPoint* GetNext(void) { return m_pNext; }

	//取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }

private:
	static CPoint* m_pTop;	//先頭オブジェクト
	static CPoint* m_pCur;	//最後尾オブジェクト
	CPoint* m_pNext;		//次のオブジェクト
	CPoint* m_pPrev;		//前のオブジェクト
	static int m_nNumAll;	//総数
	D3DXVECTOR3 m_pos;		//位置
};

#endif // !_POINT_H_
