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
	static void CommonUpdate(void);	//交番共通更新処理

	//生成
	static CKoban* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel);

	//設定
	static void SetKobanParam(const int nSpawnSpan, const int nPoliceMax, const int nPatrollNum);
	static void AddPolice(const int nNum) { m_waitingPolice += nNum; }

	//ダメージ報告
	static void ReportDamage(const int nID) { m_AttackList.emplace_back(nID); }

	//取得
	static CKoban* GetKoban(const int nID) { return m_apKoban[nID]; }
	static int GetNumAll(void) { return m_nNumAll; }

private:
	static CKoban* m_apKoban[MAX_OBJ];	//交番情報
	static int m_disPatchCT[MAX_OBJ];	//出動後のCT（建物ごとに管理）
	static std::vector<int> m_AttackList;
	int m_nID;					//ID
	static int m_nNumAll;		//総数

	static int m_nCounterSpawn;	//沸きカウンタ
	static int m_nSpawnSpan;	//沸きの速さ

	static int m_waitingPolice;	//待機中の警察の人数

	static int m_nPatrollNum;	//パトロールに行かせる人数
	static int m_nPoliceMax;	//警察の最大人数
};

#endif // !_KOBAN_H_
