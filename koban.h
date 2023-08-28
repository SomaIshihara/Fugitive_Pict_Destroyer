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
	CKoban(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	~CKoban();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void CommonUpdate(void);	//交番共通更新処理

	//生成
	static CKoban* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);

	//設定
	static void SetKobanParam(const int nSpawnSpan, const int nPoliceMax);

	//モデル設定
	static void BindModel(CXModel* pModel) { m_pModel = pModel; }

private:
	static CKoban* m_apKoban[MAX_OBJ];	//交番情報
	static CXModel* m_pModel;	//クラス共通交番モデル
	static int m_nNumAll;		//総数
	static int m_nCounterSpawn;	//沸きカウンタ
	static int m_nSpawnSpan;	//沸きの速さ
	static int m_nPoliceMax;	//警察の最大人数
};

#endif // !_KOBAN_H_
