//======================================================
//
//交番プログラムのヘッダ[koban.h]
//Author:石原颯馬
//
//======================================================
#ifndef _KOBAN_H_
#define _KOBAN_H_
#include "objectX.h"
#include "xmodel.h"
#include <vector>

//交番クラス
class CKoban : public CObjectX
{
public:
	//コンストラクタ・デストラクタ
	CKoban();
	CKoban(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel);
	~CKoban();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CKoban* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel);

	//取得
	static CKoban* GetKoban(const int nID) { return m_apKoban[nID]; }
	static CKoban** GetKoban(void) { return &m_apKoban[0]; }
	static int GetNumAll(void) { return m_nNumAll; }

private:
	static CKoban* m_apKoban[MAX_OBJ];	//交番情報
	int m_nID;					//ID
	static int m_nNumAll;		//総数
};

#endif // !_KOBAN_H_
