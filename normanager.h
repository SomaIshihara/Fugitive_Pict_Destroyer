//======================================================
//
//一般人マネージャのヘッダ[normanager.h]
//Author:石原颯馬
//
//======================================================
#ifndef _NORMANAGER_H_
#define _NORMANAGER_H_
#include "main.h"
#include "object.h"
#include "manager.h"

class CNorManager : public CObject
{
public:
	//静的const
	static const int RESEARCH_NUM;				//検索回数
	static const float NOBUILDING_TIME_PERCE;	//再探索の時間の割合

	//コンストラクタ・デストラクタ
	CNorManager(int nPriority = PRIORITY_DEFAULT);
	~CNorManager();

	//生成
	static CNorManager* Create(void);

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//取得（特になんも返す必要がない）
	D3DXVECTOR3 GetPos(void) { return CManager::VEC3_ZERO; }
	D3DXVECTOR3 GetRot(void) { return CManager::VEC3_ZERO; }
	float GetWidth(void) { return CManager::FLOAT_ZERO; }
	float GetHeight(void) { return CManager::FLOAT_ZERO; }
	float GetDepth(void) { return CManager::FLOAT_ZERO; }

	//設定
	void SetNum(const int maxNum) { m_maxPicto = maxNum; }
	void SetTime(const int time) { m_spawnTime = time; }

private:
	int m_maxPicto;		//一般人の上限
	int m_spawnTime;	//沸かすのにかかる時間
	int m_counter;		//沸きカウンター
};

#endif // !_NORMANAGER_H_