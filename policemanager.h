//======================================================
//
//警察マネージャ処理のヘッダ[policemanager.h]
//Author:石原颯馬
//
//======================================================
#ifndef _POLICE_MANAGER_H_
#define _POLICE_MANAGER_H_
#include "main.h"
#include "manager.h"
#include "object.h"
#include <vector>

class CPoliceManager : public CObject	
{
public:
	//コンストラクタ・デストラクタ
	CPoliceManager(int nPriority = PRIORITY_DEFMM);	//デフォルト
	~CPoliceManager();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CPoliceManager* Create(void);

	//取得（純粋仮想関数の関係上実装しているが、こいつに位置やらサイズやらはいらないのですべて0を返す）
	D3DXVECTOR3 GetPos(void) { return CManager::VEC3_ZERO; }
	D3DXVECTOR3 GetRot(void) { return CManager::VEC3_ZERO; }
	float GetWidth(void) { return CManager::FLOAT_ZERO; }
	float GetHeight(void) { return CManager::FLOAT_ZERO; }
	float GetDepth(void) { return CManager::FLOAT_ZERO; }

	//設定
	static void AddPolice(const int nNum) { m_waitingPolice += nNum; }
	static void SetKobanParam(const int nSpawnSpan, const int nPoliceMax, const int nPatrollNum);

	//ダメージ報告
	static void ReportDamage(const int nID) { m_AttackList.emplace_back(nID); }

private:
	static int m_disPatchCT[MAX_OBJ];	//出動後のCT（建物ごとに管理）
	static std::vector<int> m_AttackList;

	static int m_nCounterSpawn;	//沸きカウンタ
	static int m_nSpawnSpan;	//沸きの速さ

	static int m_waitingPolice;	//待機中の警察の人数

	static int m_nPatrollNum;	//パトロールに行かせる人数
	static int m_nPoliceMax;	//警察の最大人数
	int m_nLv;					//今後出動する警察のレベル
};

#endif // !_MULTIPLE_BG_H_