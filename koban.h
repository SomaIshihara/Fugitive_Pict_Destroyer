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

//交番クラス
class CKoban : CObjectX
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
	static void CommonUpdate(void);	//交番共通更新処理

	//生成
	static CKoban* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel);

	//設定
	static void SetKobanParam(const int nSpawnSpan, const int nPoliceMax);

private:
	static CKoban* m_apKoban[MAX_OBJ];	//交番情報
	int m_nID;					//ID
	static int m_nNumAll;		//総数
	static int m_nCounterSpawn;	//沸きカウンタ
	static int m_nSpawnSpan;	//沸きの速さ
	static int m_nPoliceMax;	//警察の最大人数
};

#endif // !_KOBAN_H_
